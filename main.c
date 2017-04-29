#include <stdio.h>
#include <stdlib.h>

#include "zim_con.h"
#include "reader.h"

int main()
{
    intlist ints;

    debug = 1;

    merror("Hello world!\n");

    intlist_zero(&ints);
    ints.block_ints = 200;

    intlist_append(&ints, 1337);

    mlog("Intlist wert #1: %i", ints.integers[0]);

    return 0;

}
