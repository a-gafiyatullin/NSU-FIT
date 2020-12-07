#include <stdio.h>
#include "wave.h"

int main(int argc, char *argv[])
{
	char error_msg[] =
		"usage: hide-in-wave [-h (default) / -g] -d [dst file] -s [src file]";
	if (argc < 2) {
		fprintf(stderr, "%s\n", error_msg);
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
			fprintf(stderr, "%s\n", error_msg);
			return -1;
		}
	}

	if (hide) {
		if (!dst_file_name || !src_file_name) {
			fprintf(stderr, "%s\n", error_msg);
			return -1;
		}

		if (hide_src_in_dst(src_file_name, dst_file_name)) {
			fprintf(stderr, "Can't hide information!\n");
			return -1;
		}
	} else {
		if (!dst_file_name) {
			fprintf(stderr, "%s\n", error_msg);
			return -1;
		}
		if (get_src_from_dst(dst_file_name)) {
			fprintf(stderr, "Can't get information!\n");
			return -1;
		}
	}

	return 0;
}
