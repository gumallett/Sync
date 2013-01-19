#ifndef __UTIL_H__
#define __UTIL_H__

#include <tchar.h>
#include <stdint.h>
#include <Windows.h>

#define bool uint16_t
#define ABS(x) (x < 0) ? -(x) : (x) 

void die(const char *msg);
TCHAR *get_dir(const TCHAR *file);
TCHAR *get_filename(const TCHAR *file);
bool is_relative(const TCHAR *path);
uint64_t hashcode(const TCHAR * somestring);
uint64_t last_modified(const TCHAR *path);

#endif