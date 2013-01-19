
#include <tchar.h>
#include "playlist.h"
#include "util.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "sync.h"

static void add_files(Set *set, TCHAR *directory) {
	HANDLE file;
	WIN32_FIND_DATA file_info;
	BOOL ret = TRUE;
	TCHAR *filename;

	FileEntry *entry;
	uint64_t hash;

	file = FindFirstFile(directory, &file_info);

	while(ret && file != NULL && file != INVALID_HANDLE_VALUE) {
		ret = FindNextFile(file, &file_info);
		filename = file_info.cFileName;
		directory = get_dir(directory);

		if(wcscmp(filename, L".") == 0 || wcscmp(filename, L"..") == 0 || file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			continue;
		}

		entry = new_fentry(directory, filename);
		hash = fentry_hashcode(entry);
		set_add(set, hash, entry);
	}

	FindClose(file);
}

static void print_set(Set *set) {
	Node *ptr;
	FileEntry *fe;

	LIST_FOR_EACH(ptr, set->iterator) {
		fe = (FileEntry *) ptr->data;
		wprintf(L"%s\n", fe->filename);
	}
}

void sync(Playlist *playlist, List *destination_list) {
	Set *file_set = new_set(DEFAULT_CAPACITY);
	Set *filesToAdd, *filesToRemove;
	Node *ptr;
	
	LIST_FOR_EACH(ptr, destination_list) {
		add_files(file_set, (TCHAR *) ptr->data);
	}

	filesToAdd = set_diff(playlist->entries, file_set, (uint64_t (*)(const void *)) fentry_hashcode, (int16_t (*)(const void *, const void *)) fentry_compare);
	printf("Files to add: %d\n", filesToAdd->size);
	print_set(filesToAdd);
	filesToRemove = set_diff(file_set, playlist->entries, (uint64_t (*)(const void *)) fentry_hashcode, (int16_t (*)(const void *, const void *)) fentry_compare);
	printf("Files to remove: %d\n", filesToRemove->size);
	print_set(filesToRemove);
}