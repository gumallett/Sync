#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include "playlist.h"
#include "sync.h"

static void print_usage(void);
static int parse_args(int, _TCHAR**, TCHAR*, TCHAR**);

int _tmain(int argc, _TCHAR* argv[]) {
	Playlist *list;
	TCHAR *playlist_file = L"E:\\Documents and Settings\\All Users\\Documents\\My Music\\My Playlists\\current.m3u";	
	List *dest_list = new_list();
	TCHAR *c;
	int i;

	//add_item(dest_list, L"K:\\MUSIC\\*");
	//add_item(dest_list, L"L:\\MUSIC\\*");

	if(argc == 2 && wcsncmp(L"/?", argv[1], 2) == 0) {
		print_usage();
	}

	if(argc > 2) {
		
	}

	//list = new_playlist(playlist_file);
	//sync(list, dest_list);

	//wait for user input
	getchar();

	return 0;
}

static void print_usage() {
	printf("Usage: sync -p [playlist file] [ [dest1] [dest2] ... ]");
	exit(0);
}

static int parse_args(int argc, _TCHAR *argv[], TCHAR *playlist_path, TCHAR *dests[]) {
	int i, j, k = 0;
	int err;

	for(i=1; i<argc; i++) {
		if(wcscmp(L"-p", argv[i]) == 0) {
			j = i + 1;

			if(j < argc) {
				wcscpy_s(playlist_path, MAX_PATH, argv[j]);
			}
			else {
				return -1;
			}
		}
		else {
			wcscpy_s(dests[k], 5, argv[i]);
			k++;
		}
	}
}