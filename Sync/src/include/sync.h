#ifndef __SYNC_H__
#define __SYNC_H__

#include "playlist.h"

typedef struct destination_struct {
	uint64_t disk_free_space;
	int full;
	TCHAR *path;
} DestStruct;

void sync(Playlist *playlist, List *destination_list);
DestStruct *create_dest_struct(const TCHAR *path);
void destroy_dest_struct(DestStruct *dest);

#endif