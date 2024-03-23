#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

char *combinePath(char *path1, char *path2) {
	size_t parentPathSize = strlen(path1);
	char *path = malloc(sizeof(char) * (parentPathSize + 1 + strlen(path2) + 1));
	strcpy(path, path1);
	path[parentPathSize] = '/';
	strcpy(path + parentPathSize + 1, path2);

	return path;
}

bool traverse(char *filename, time_t ts) {
	struct stat st;

	if(stat(filename, &st) == -1) {
		perror("stat");
		return true;
	}

	if(S_ISDIR(st.st_mode)) {
		DIR *dir = opendir(filename);

		if(NULL == dir) {
			perror("opendir");
			return true;
		}

		struct dirent *sub;

		while((sub = readdir(dir)) != NULL) {
			if(strcmp(sub->d_name, ".") == 0 || strcmp(sub->d_name, "..") == 0)
				continue;

			char *subPath = combinePath(filename, sub->d_name);

			if(NULL == subPath)
				return true;
			
			if(traverse(subPath, ts))
				return true;
		}

		if(errno != 0) {
			perror("readdir");
			return true;
		}

		return false;
	}

	if(st.st_mtim.tv_sec > ts)	
		printf("%s\n", filename);

	return false;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "Invalid number of arguments\n");
		return 1;
	}

	char *root = argv[1];
	char *tsFilename = argv[2];

	struct stat tsStat;

	time_t ts;

	if(stat(tsFilename, &tsStat) == -1) {
		if(errno == ENOENT) {
			ts = 0;
			errno = 0;
		} else {
			perror("stat");
			return 1;
		}
	} else
		ts = tsStat.st_mtim.tv_sec;

	if(traverse(root, ts)) {
		fprintf(stderr, "An error occured, exiting\n");
		return 1;
	}

	return 0;
}
