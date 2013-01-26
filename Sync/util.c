#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void die(const char *msg) {
	printf(msg);
	exit(1);
}

TCHAR *get_dir(const TCHAR *file) {
	TCHAR dir[512];
	TCHAR drive[10];
	TCHAR *full_dir = malloc(512 * sizeof(TCHAR));
	errno_t err;

	if(full_dir == NULL) {
		die("Could not allocated memory.");
	}

	err = _wsplitpath_s(file, drive, 10, dir, 512, NULL, 0, NULL, 0);

	if(err != 0) {
		printf("Error splitting path.");
		return NULL;
	}
	
	wcscpy_s(full_dir, 512, drive);
	wcscat_s(full_dir, 512, dir);

	return full_dir;
}

TCHAR *get_filename(const TCHAR *file) {
	TCHAR *filename = malloc(FILENAME_MAX * sizeof(TCHAR));
	TCHAR ext[32];
	errno_t err;

	if(filename == NULL) {
		die("Could not allocated memory.");
	}

	err = _wsplitpath_s(file, NULL, 0, NULL, 0, filename, FILENAME_MAX, ext, 32);

	if(err != 0) {
		printf("Error splitting path.");
		return NULL;
	}

	wcscat_s(filename, FILENAME_MAX, ext);

	return filename;
}

bool is_relative(const TCHAR *path) {
	return wcschr(path, L':') == NULL;
}

//Hashes a string
uint64_t hashcode(const TCHAR *somestring) {
	uint64_t hash = 101;
	
	while(*somestring) {
		hash = hash * 31 + *somestring++;
	}

	return hash;
}

uint64_t last_modified(const TCHAR *path) {
	HANDLE file;
	WIN32_FIND_DATA file_info;
	FILETIME modtime;
	ULARGE_INTEGER modtime2;

	file = FindFirstFile(path, &file_info);
	modtime = file_info.ftLastWriteTime;
	modtime2.HighPart = modtime.dwHighDateTime;
	modtime2.LowPart = modtime.dwLowDateTime;

	return modtime2.QuadPart;
}