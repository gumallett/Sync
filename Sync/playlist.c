#include "playlist.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static TCHAR *set_full_path(const FileEntry *entry, const TCHAR *dir) {
	TCHAR *rel_dir = entry->directory;
	TCHAR *filename = entry->filename;
	TCHAR *full_path = malloc(MAX_PATH * sizeof(TCHAR));

	if(is_relative(rel_dir)) {
		wcscpy_s(full_path, MAX_PATH, dir);
		wcscat_s(full_path, MAX_PATH, rel_dir);
	}
	else {
		wcscpy_s(full_path, MAX_PATH, rel_dir);
	}
	
	wcscat_s(full_path, MAX_PATH, filename);

	return full_path;
}

uint64_t fentry_hashcode(const FileEntry *entry) {
	uint64_t hash = 101;
	TCHAR *filename = entry->filename;

	while(*filename) {
		hash = hash * 7 + *filename++;
	}

	//hash += last_modified(entry->full_path);
	return hash;
}

static void addEntry(Playlist *playlist, FileEntry *entry) {	
	uint64_t hash = fentry_hashcode(entry);

	//wprintf(L"entry: %s\thash: %llu\tdir: %s\n", entry->filename, hash, entry->directory);

	set_add(playlist->entries, hash, (void *)entry);
	playlist->size++;
}

static void load_playlist(Playlist *list, TCHAR *filename) {
	FILE *file;
	TCHAR buf[1024];
	TCHAR c;
	uint32_t i = 0;

	FileEntry *entry;

	_tfopen_s(&file, filename, L"r");

	if(file == NULL) {
		printf("Couldn't open playlist file, error: %d", errno);
		return;
	}

	while((c = fgetwc(file)) != WEOF) {
		if(c == L'\n') {
			buf[i] = '\0';
			entry = new_fentry(list->directory, buf);
			addEntry(list, entry);
			i = 0;
			continue;
		}

		buf[i] = c;
		i++;
	}

	fclose(file);
}

int16_t fentry_compare(const FileEntry *item1, const FileEntry *item2) {
	int16_t cmp = wcscmp(item1->filename, item2->filename);
	uint64_t lm1, lm2;
	int64_t diff;

	if(cmp == 0) {
		lm1 = last_modified(item1->full_path);
		lm2 = last_modified(item2->full_path);
		diff = lm1 - lm2;
		diff = ABS(diff);

		if(diff <= 37000000000L) {
			return 0;
		}
		else {
			return 1;
		}
	}

	return cmp;
}

Playlist *new_playlist(TCHAR* filename) {
	Playlist *list = malloc(sizeof(Playlist));

	if(list == NULL) {
		die("Could not allocate memory.");
	}

	list->size = 0;
	list->filename = filename;	
	list->entries = new_set(DEFAULT_CAPACITY);
	list->directory = get_dir(filename);

	load_playlist(list, filename);	

	return list;
}

FileEntry *new_fentry(const TCHAR *dir, const TCHAR *entry_name) {
	FileEntry *entry = malloc(sizeof(FileEntry));

	entry->directory = get_dir(entry_name);
	entry->filename = get_filename(entry_name);
	entry->full_path = set_full_path(entry, dir);

	return entry;
}