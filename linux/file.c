// Copyright (c) 2015 Electric Power Research Institute, Inc.
// author: Mark Slicker <mark.slicker@gmail.com>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int file_type (const char *name) {
  struct stat sb;
  if (stat (name, &sb) == 0) {
    switch (sb.st_mode & S_IFMT) {
    case S_IFREG: return FILE_REGULAR;
    case S_IFDIR: return FILE_DIR;
    }
  } return FILE_NONE;
}

void process_dir (const char *name, void *ctx,
		  void (*func) (const char *, void *ctx)) {
  DIR *dp = opendir (name);
  struct dirent *ep;
  if (!dp) { perror ("process_dir"); exit (0); }
  while (ep = readdir (dp))
    if (ep->d_type == DT_REG) {
        // add 1 for '/' separator and 1 for null terminator
        size_t path_size
            = strlen(name) + 1 + strlen(ep->d_name) + 1;
        char* path = malloc(path_size);
        if (path == NULL) {
            // handle error
            // No error handling at this point.
        }
        snprintf(path, path_size, "%s/%s", name, ep->d_name);
        func (path, ctx);
        free(path);
    }
  closedir (dp);
}
