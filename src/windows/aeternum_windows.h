#ifndef _aeternum_windows_h
#define _aeternum_windows_h

int aeternum_utf8_to_utf16(const char* utf8Buffer, wchar_t** utf16Buffer);
int aeternum_utf16_to_utf8(const wchar_t* utf16Buffer, char** utf8Buffer);

char* aeternum_make_command(char** args, int num_args);

int aeternum_exec(char* command, char* out_file, char* err_file);

#endif