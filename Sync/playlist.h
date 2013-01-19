#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include <stdint.h>
#include <tchar.h>
#include <Windows.h>
#include "set.h"

#define MAXSIZE 5000

typedef struct playlist_struct {
	TCHAR *filename;
	TCHAR *directory;
	uint32_t size;
	Set *entries;
} Playlist;

typedef struct file_entry_struct {
	TCHAR *filename;
	TCHAR *directory;
	TCHAR *full_path;
} FileEntry;

int16_t fentry_compare(const FileEntry *item1, const FileEntry *item2);

Playlist *new_playlist(TCHAR* filename);
FileEntry *new_fentry(const TCHAR *dir, const TCHAR *entry_name);
uint64_t fentry_hashcode(const FileEntry *entry);


#endif // !__PLAYLIST_H__