#ifdef __cplusplus
#undef __cplusplus
#endif

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include "playlist.h"
#include "sync.h"

static void print_usage(void);
static int parse_args(int, const _TCHAR**, TCHAR*, List*);

int _tmain(int argc, _TCHAR* argv[]) {
	Playlist *list;
	TCHAR *playlist_file = malloc(MAX_PATH * sizeof(TCHAR));
	List *dest_list = new_list();
	int err;

	if(argc == 2 && wcsncmp(L"/?", argv[1], 2) == 0) {
		print_usage();
	}

	if(argc > 2) {
		err = parse_args(argc, argv, playlist_file, dest_list);

		if(err == -1) {
			print_usage();
		}

		list = new_playlist(playlist_file);
		sync(list, dest_list);
	}
	else {
		print_usage();
	}

	//wait for user input
	getchar();

	return 0;
}

static void print_usage() {
	printf("Usage: sync -p [playlist file] [ [dest1] [dest2] ... ]");
	exit(0);
}

static int parse_args(int argc, const _TCHAR *argv[], TCHAR *playlist_path, List *dest_list) {
	int i, j, k = 0;
	DestStruct *dest;

	for(i=1; i<argc; i++) {
		j = i + 1;

		if(wcscmp(L"-p", argv[i]) == 0) {
			if(i == 1) {
				if(j < argc) {
					wcscpy_s(playlist_path, MAX_PATH, argv[j]);
					i = j;
				}
				else {
					return -1;
				}
			}
			else {
				return -1;
			}

			continue;
		}		
		
		dest = create_dest_struct(argv[i]);
		add_item(dest_list, dest);	
	}

	return 1;
}