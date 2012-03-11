#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "aeternum.h"
#include "aeternum_windows.h"

static int json;

int aeternum_utf8_to_utf16(const char* utf8Buffer, wchar_t** utf16Buffer) {
  int size = MultiByteToWideChar(CP_UTF8,
                                 0,
                                 utf8Buffer,
                                 -1,
                                 NULL,
                                 0);
  *utf16Buffer = (wchar_t*)malloc(size * sizeof(wchar_t));
  if (!*utf16Buffer) {
    return 0;
  }

  return MultiByteToWideChar(CP_UTF8,
                             0,
                             utf8Buffer,
                             -1,
                             *utf16Buffer,
                             size);
}

int aeternum_utf16_to_utf8(const wchar_t* utf16Buffer, char** utf8Buffer) {
  int size = WideCharToMultiByte(CP_UTF8,
                                 0,
                                 utf16Buffer,
                                 -1,
                                 NULL,
                                 0,
                                 NULL,
                                 NULL);
  *utf8Buffer = (char*)malloc(size * sizeof(char));
  if (!*utf8Buffer) {
    return 0;
  }

  return WideCharToMultiByte(CP_UTF8,
                             0,
                             utf16Buffer,
                             -1,
                             *utf8Buffer,
                             size,
                             NULL,
                             NULL);
}

char* aeternum_get_error(void) {
  DWORD error = 0;
  int size = 0;
  wchar_t* message = NULL;
  char* final_message = NULL;

  error = GetLastError();

  size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER,
                       NULL,
                       error,
                       0,
                       message,
                       0,
                       NULL);

  if (size > 0) {
    aeternum_utf16_to_utf8(message, &final_message);
    return final_message;
  }
  else {
    return NULL;
  }
}

char* aeternum_make_command(char** args, int num_args) {
  int size = 0;
  char* command = NULL;

  int i;
  for (i = 0; i < num_args; ++i) {
    //size of string plus space (includes terminating null character)
    char* tostrlen = args[i];
    size += strlen(args[i]) + 1;
  }

  command = (char*)malloc(size * sizeof(char));
  if (!command)
    return NULL;

  i = 0;
  //copy first argument including \0
  strncpy(command, args[i], strlen(args[i])+1);

  for (++i; i < num_args; ++i) {
    //concatenate each argument plus space
    strncat(command, " ", 1);
    strncat(command, args[i], strlen(args[i]));
  }

  //terminate with null character (overwrites space)
  command[size-1] = '\0';

  return command;
}

options_t aeternum_options(int argc, char *argv[]) {
  options_t opts = options_parse(argc, argv);

  opts.command = aeternum_make_command(opts.child_args, opts.num_args);

  printf(opts.command);

  return opts;
}

int aeternum_exec(char* command, char* out_file, char* err_file) {
  wchar_t *stdout_file = NULL, *stderr_file = NULL, *wcommand = NULL;
  STARTUPINFOW startup;
  PROCESS_INFORMATION info;
  SECURITY_ATTRIBUTES fSecAttr;
  HANDLE hStdOutFile = NULL, hStdErrFile = NULL;
  BY_HANDLE_FILE_INFORMATION outInfo, errInfo;
  int pid = 0;

  fSecAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  fSecAttr.bInheritHandle = TRUE;
  fSecAttr.lpSecurityDescriptor = NULL;

  if (out_file) {
    if (!aeternum_utf8_to_utf16(out_file, &stdout_file))
      return 0;
    hStdOutFile = CreateFileW(stdout_file,
                              GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              &fSecAttr,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  }  else if (err_file) {
    /* Use the nul file, since a valid handle is required */
    hStdOutFile = CreateFileW(L"NUL",
                              GENERIC_WRITE,
                              0,
                              &fSecAttr,
                              OPEN_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  }
  if (hStdOutFile == INVALID_HANDLE_VALUE) {
    return 0;
  }

  if (err_file) {
    if (!aeternum_utf8_to_utf16(err_file, &stderr_file))
      return 0;
    hStdErrFile = CreateFileW(stderr_file,
                              GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE,
                              &fSecAttr,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  } else if (out_file) {
    /* Use the nul file, since a valid handle is required */
    hStdErrFile = CreateFileW(L"NUL",
                              GENERIC_WRITE,
                              0,
                              &fSecAttr,
                              OPEN_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);
  }
  if (hStdErrFile == INVALID_HANDLE_VALUE) {
    return 0;
  }

  /* When both are defined, make sure they aren't the same file */
  if (err_file && out_file) {
    if (!GetFileInformationByHandle(hStdOutFile,
                                    &outInfo)) {
      return 0;
    }
    if (!GetFileInformationByHandle(hStdErrFile,
                                    &errInfo)) {
      return 0;
    }
    if (errInfo.dwVolumeSerialNumber == outInfo.dwVolumeSerialNumber
        && errInfo.nFileIndexHigh == outInfo.nFileIndexHigh
        && errInfo.nFileIndexLow == outInfo.nFileIndexLow) {
      /* If stderr_file and stdout_file point to the same place, set */
      /* one to the other */
      CloseHandle(hStdErrFile);
      hStdErrFile = hStdOutFile;
    }
  }

  startup.cb = sizeof(startup);
  startup.lpReserved = NULL;
  startup.lpDesktop = NULL;
  startup.lpTitle = NULL;
  startup.dwFlags = STARTF_USESTDHANDLES;
  startup.cbReserved2 = 0;
  startup.lpReserved2 = NULL;
  startup.hStdInput = NULL;
  startup.hStdOutput = hStdOutFile;
  startup.hStdError = hStdErrFile;

  if (!aeternum_utf8_to_utf16(command, &wcommand))
    return 0;

  if (CreateProcessW(NULL,
                     wcommand,
                     NULL,
                     NULL,
                     1,
                     CREATE_UNICODE_ENVIRONMENT | CREATE_NEW_PROCESS_GROUP | DETACHED_PROCESS,
                     NULL,
                     NULL,
                     &startup,
                     &info)) {
    /* Spawn succeeded, break connections with child */
    pid = info.dwProcessId;
    CloseHandle(info.hThread);
    CloseHandle(info.hProcess);
  } else {
    return 0;
  }

  return pid;
}

void aeternum_start(options_t opts) {
  int pid;

  assert(opts.child_args != NULL);

  if (opts.json) json = 1;
  if (!opts.command) {
    if (json)
      printf("{\"error\": \"Could not make command.\"}");
    else
      printf("Could not make command.");
  }

  pid = aeternum_exec(opts.command, opts.outfile, opts.errfile);

  if (pid) {
    if (json)
      printf("{\"pid\": %d}", pid);
    else
      printf("Child has been spawned and daemonized. PID: %d\n", pid);
  }
  else {
    printf("crap");
    if (json)
      printf("{\"error\": \"%s\"}", aeternum_get_error());
    else
      printf("An error occurred: %s", aeternum_get_error());
  }
}