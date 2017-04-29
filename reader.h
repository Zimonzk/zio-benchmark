#ifndef READER_H_INCLUDED
#define READER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _NOMAIN_
#include "kontur/zim_con.h"

typedef struct kostring {
    size_t length;
    size_t allocated_length;
    size_t block_bytes;
    char* cstring;
} kostring;

/* "length" is the ammount of integers that "integers" can currently hold */
typedef struct intlist {
    size_t length;
    size_t allocated_length;
    size_t block_ints;
    int* integers;
} intlist;

void kostring_zero(kostring*);
void kostring_write(kostring*, char*);
void kostring_resize(kostring*, size_t);
void kostring_append(kostring*, char);
void kostring_free(kostring*);

/* intlist */
void intlist_zero(intlist*);
void intlist_write(intlist*, char*);
void intlist_append(intlist*, int);
void intlist_resize(intlist*, size_t);
void intlist_free(intlist*);

/* filestuff */
void readlin(FILE*, kostring*);

#endif // READER_H_INCLUDED
