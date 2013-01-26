
#include <tchar.h>
#include "playlist.h"
#include "util.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "sync.h"

static void print_set(Set *set);
static void copy_file(const FileEntry *src, const FileEntry *dest);
static void add_files(Set *files_to_add, List *dest_list);
static void put_files(Set *set, TCHAR *directory);
static void remove_files(Set *files_to_remove);
static BOOL is_full(DestStruct *dest);

void sync(Playlist *playlist, List *destination_list) {
	Set *file_set = new_set(DEFAULT_CAPACITY);
	Set *filesToAdd, *filesToRemove;
	Node *ptr;
	
	LIST_FOR_EACH(ptr, destination_list) {
		DestStruct *dest = (DestStruct *) ptr->data;
		put_files(file_set, dest->path);
	}

	filesToAdd = set_diff(playlist->entries, file_set, (uint64_t (*)(const void *)) fentry_hashcode, (int16_t (*)(const void *, const void *)) fentry_compare);
	printf("Files to add: %d\n", filesToAdd->size);
	print_set(filesToAdd);
	filesToRemove = set_diff(file_set, playlist->entries, (uint64_t (*)(const void *)) fentry_hashcode, (int16_t (*)(const void *, const void *)) fentry_compare);
	printf("Files to remove: %d\n", filesToRemove->size);
	print_set(filesToRemove);

	remove_files(filesToRemove);
	add_files(filesToAdd, destination_list);
}

DestStruct *create_dest_struct(const TCHAR *path) {
	DestStruct *dest_struct;
	BOOL err;
	ULARGE_INTEGER disk_free_space;

	dest_struct = malloc(sizeof(DestStruct));

	if(dest_struct == NULL) {
		die("Could not allocate memory");
	}

	dest_struct->path = malloc(MAX_PATH * sizeof(TCHAR));
	dest_struct->full = 0;

	if(dest_struct->path == NULL) {
		die("Could not allocate memory");
	}

	wcscpy_s(dest_struct->path, MAX_PATH, path);
	err = GetDiskFreeSpaceEx(path, &disk_free_space, NULL, NULL);

	if(!err) {
		printf("Could not get disk free space from: %s", path);
		dest_struct->disk_free_space = 0;
		return dest_struct;
	}

	dest_struct->disk_free_space = disk_free_space.QuadPart;

	return dest_struct;
}

void destroy_dest_struct(DestStruct *dest) {
	free(dest->path);
	free(dest);
}

static void copy_file(const FileEntry *src, const FileEntry *dest) {
	BOOL success;

	wprintf(L"Copying file %s to %s.\n", src->filename, dest->full_path);

	success = CopyFile(src->full_path, dest->full_path, FALSE);

	if(!success) {
		printf("Failed to copy file: %s", src->filename);
	}
}

static void put_files(Set *set, TCHAR *directory) {
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

static void add_files(Set *files_to_add, List *dest_list) {
	Node *ptr;
	FileEntry *entry, *dest_entry;
	Node *working_dest = dest_list->head;
	DestStruct *dest_struct = (DestStruct *) working_dest->data;

	wcscat_s(dest_struct->path, MAX_PATH, L"\\");
	
	LIST_FOR_EACH(ptr, files_to_add->iterator) {
		entry = (FileEntry *) ptr->data;

		if(is_full((DestStruct *) working_dest->data)) {
			working_dest = working_dest->next;
			dest_struct = (DestStruct *) working_dest->data;
			wcscat_s(dest_struct->path, MAX_PATH, L"\\");

			if(working_dest == NULL) {
				printf("Aborting adding files. Out of space.\n");
				return;
			}
		}

		dest_entry = new_fentry(dest_struct->path, entry->filename);
		copy_file(entry, dest_entry);
		destroy_fentry(dest_entry);
	}
}

static BOOL is_full(DestStruct *dest) {
	ULARGE_INTEGER free_space;
	BOOL success;

	success = GetDiskFreeSpaceEx(dest->path, &free_space, NULL, NULL);

	if(!success) {
		printf("Could not get disk free space for: %s", dest->path);
		return dest->full;
	}

	dest->disk_free_space = free_space.QuadPart;

	if(free_space.QuadPart < 100000000) {
		dest->full = 1;
		return 1;
	}

	return 0;
}

static void remove_files(Set *files_to_remove) {
	Node *ptr;
	FileEntry *entry;
	BOOL success;

	LIST_FOR_EACH(ptr, files_to_remove->iterator) {
		entry = (FileEntry *) ptr->data;
		success = DeleteFile(entry->full_path);

		if(!success) {
			printf("Failed to delete file: %s", entry->full_path);
		}
	}
}

static void print_set(Set *set) {
	Node *ptr;
	FileEntry *fe;

	LIST_FOR_EACH(ptr, set->iterator) {
		fe = (FileEntry *) ptr->data;
		wprintf(L"%s\n", fe->filename);
	}
}