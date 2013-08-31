#ifndef MY_CP
#define MY_CP

#include "benchmark.h"

void create_file (char *in, size_t file_size);
void rm (char *fname);
void read_write (timer *t, recorder *rec, char *in, char *out,
    size_t file_size, size_t len, int flags);
void gets_puts (timer *t, recorder *rec, char *in, char *out,
    size_t file_size, size_t len, int has_buf, size_t buf_size);
void mmap_munmap (timer *t, recorder *rec, char *in, char *out,
    size_t file_size, size_t len);

#endif
