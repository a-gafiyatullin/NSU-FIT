#include <stdio.h>
#include "wave.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "usage: hide-in-wave [-h (default) / -g]"
				" -d [dst file] -s [src file]");
		return -1;
	}

	char *dst_file_name = NULL;
	char *src_file_name = NULL;
	int hide = 1;

	int i = 0;
	while (++i < argc) {
		switch (argv[i][1]) {
		case 'g':
			hide = 0;
			break;
		case 'd':
			dst_file_name = argv[++i];
			break;
		case 's':
			src_file_name = argv[++i];
			break;
		case 'h':
			break;
		default:
			perror("Unrecognized option!");
			return -1;
		}
	}

	if (!dst_file_name || !src_file_name) {
		fprintf(stderr, "usage: hide-in-wave [-h (default) / -g]"
				" -d [dst file] -s [src file]");
		return -1;
	}

	if (hide) {
		if (hide_src_in_dst(src_file_name, dst_file_name)) {
			perror("Can't hide information!");
			return -1;
		}
	} else {
		if (get_src_from_dst(dst_file_name) < 0) {
			perror("Can't get information!");
			return -1;
		}
	}

	return 0;
}
