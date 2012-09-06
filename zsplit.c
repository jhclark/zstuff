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
  fprintf(stderr, "usage: zsplit out_prefix < in\n");
  exit(1);
}

int main(int argc, char** argv) {
  if(argc != 2)
    showusage();

  // TODO: Better arg parsing
  char* out_prefix = argv[1];

  if(colNum < 0)
    showusage();

#ifdef USE_ZLIB
  gzFile out;
#else
  FILE* out;
#endif

  char filename[MAX_FILENAME_LEN];
    sprintf(filename, "%s%06d.gz", prefix, i);
#ifdef USE_ZLIB
    shards[i] = gzopen(filename, "wb");
#else
    char cmd[MAX_FILENAME_LEN];
    sprintf(cmd, "gzip > %s", filename);
    shards[i] = popen(cmd, "w");
#endif
    if(shards[i] == NULL) {
      fprintf(stderr, "Could not open file/pipe for writing: %s", filename);
      exit(1);
    }
  }

  char str[MAX_LINE_LEN];
  while(fgets(str, MAX_LINE_LEN, stdin)) {
    int n = strlen(str);

    char* colBegin = str;
    char* colEnd = strstr(str, delim);
    if(colEnd == NULL)
      colEnd = str + n - 1; // -1 to hack off newline
    
    for(int i=1; i <= colNum; ++i) {
      colBegin = colEnd += delimLen;
      colEnd = strstr(colBegin, delim);
      if(colEnd == NULL)
	colEnd = str + n - 1;
    }

    // now we have the column -- hash it
    unsigned hashCode = 0;
    char* lastPos = MIN(colEnd, colBegin + maxCharsToHash);
    for(char* c = colBegin; c != lastPos; ++c) {
      hashCode = hashCode * 31 + *c;
    }

    unsigned shard = hashCode % shardCount;
#ifdef USE_ZLIB
    gzputs(shards[shard], str);
#else
    fputs(str, shards[shard]);
#endif
  }

  for(int i=0; i<shardCount; ++i) {
#ifdef USE_ZLIB
    gzclose(shards[i]);
#else
    pclose(shards[i]);
#endif
  }

  return 0;
}
