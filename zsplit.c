#define _POSIX_C_SOURCE 2 // required for popen

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// TODO: Use zlib on non-unix systems?
//#define USE_ZLIB // way slower for lots of files
#ifdef USE_ZLIB
#include "zlib.h"
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))

#define MAX_LINE_LEN 102400
#define MAX_FILENAME_LEN 1024

void showusage() {
  fprintf(stderr, "usage: zsplit out_prefix max_lines < in\n");
  exit(1);
}

int main(int argc, char** argv) {
  if (argc != 3)
    showusage();

  // TODO: Better arg parsing
  char* out_prefix = argv[1];
  int max_lines = atoi(argv[2]);

#ifdef USE_ZLIB
  gzFile out = NULL;
#else
  FILE* out = NULL;
#endif

  char str[MAX_LINE_LEN];
  int line_idx = 0;
  int file_idx = 0;
  while(fgets(str, MAX_LINE_LEN, stdin)) {

    // open a new file
    if (out == NULL || line_idx == max_lines) {
      if (out != NULL) {
        file_idx++;
#ifdef USE_ZLIB
        gzclose(out);
#else
        pclose(out);
#endif        
      }
      char filename[MAX_FILENAME_LEN];
      sprintf(filename, "%s%06d.gz", out_prefix, file_idx);
      
#ifdef USE_ZLIB
      out = gzopen(filename, "wb");
#else
      char cmd[MAX_FILENAME_LEN];
      sprintf(cmd, "gzip > %s", filename);
      out = popen(cmd, "w");
#endif
      if (out == NULL) {
        fprintf(stderr, "Could not open file/pipe for writing: %s", filename);
        exit(1);
      }
    }

#ifdef USE_ZLIB
    gzputs(out, str);
#else
    fputs(str, out);
#endif
    line_idx++;
  }

#ifdef USE_ZLIB
  gzclose(out);
#else
  pclose(out);
#endif

  return 0;
}
