#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include "playlist.h"
#include "sync.h"

static void print_usage();

int _tmain(int argc, _TCHAR* argv[]) {
	char buf[10];
	Playlist *list;
	TCHAR *playlist_file = L"E:\\Documents and Settings\\All Users\\Documents\\My Music\\My Playlists\\current.m3u";	
	List *dest_list = new_list();

	add_item(dest_list, L"K:\\MUSIC\\*");
	add_item(dest_list, L"L:\\MUSIC\\*");

	if(argc == 2 && wcsncmp(L"/?", argv[1], 2) == 0) {
		print_usage();
	}

	list = new_playlist(playlist_file);

	if(list != NULL) {
		printf("size: %d", list->size);
	}

	sync(list, dest_list);

	//wait for user input
	fgets(buf, 10, stdin);

	return 0;
}

static void print_usage() {
	printf("Usage: sync -p [playlist file]");
	exit(0);
}