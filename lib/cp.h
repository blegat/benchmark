#ifndef MY_CP
#define MY_CP

#include "benchmark.h"

#ifndef FILE_SIZE
#define SIZE 0x40000 // 256 KiB
#endif

void create_file (char *in);
void rm (char *fname);
void read_write (timer *t, recorder *rec, char *in, char *out,
    int len, int flags);
void gets_puts (timer *t, recorder *rec, char *in, char *out,
    int len, int has_buf, size_t buf_size);

#endif
