#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <signal.h>

#include <windows.h>

#include "zio-con.h"
#include "zio-list.h"
#include "zio-reader.h"


#define NUM 1000000
#define CHU 1000000

void handle_signal(int);


int main()
{
    intlist ints;

    /*timer loop variables*/
    unsigned long count;
    FILETIME sysFileTime;
    LONGLONG start_100nanos;
    LONGLONG end_100nanos;

    debug = 1;

    /*setup signal handling
    * it's actually quite nice to know when a segfault appears
    * instead of the programm just hanging*/
    signal(SIGSEGV, handle_signal);

    mlog("Testing ground for zio-utils");

    intlist_zero(&ints);
    ints.block_ints = 200;

    intlist_append(&ints, 1337);

    mlog("Intlist wert #1: %i", ints.integers[0]);

    /*reset the list for the actual benchmark*/
    intlist_free(&ints);
    intlist_zero(&ints);

    ints.block_ints = CHU; //we want this many integers to be allocated at once

    mlog("Writing %i integers into empty list (intlist_append)", NUM);

    /*disable my debug messages since they slow down the process quite a bit*/
    debug = 0;
    GetSystemTimeAsFileTime(&sysFileTime); //Start meassuring time
    start_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                         sysFileTime.dwHighDateTime) << 32LL);

    for(count = 0; count <= NUM; count++) {
        intlist_append(&ints, 42);
    }

    GetSystemTimeAsFileTime(&sysFileTime); //Stop meassuring time
    end_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                       sysFileTime.dwHighDateTime) << 32LL);
     /*re-enable my debug messages*/
    debug = 1;

    mlog("Elapsed seconds: %f",
         ((float) (end_100nanos - start_100nanos))/10000000);

    intlist_free(&ints);

    {
        alist ali;
        alist_create_type(ali, long, 1000);

        alist_append_type(ali, long, 34);

        mlog("Erster Wert in ali: %i, laenge von ali: %i, max. laenge von ali: %i", *((long*) ali.start_ptr), ali.length, ali.allocated_length);
        mlog("Blockgroesze von ali: %i, Einheitsgroesze von ali: %i", ali.block_units, ali.usize);
    }

    return 0;
}

void handle_signal(int sig)
{
    switch(sig) {
    case SIGSEGV:
        merror("Segfault appeared!");
        break;
    default:
        break;
    }

    signal(sig, SIG_DFL);
    raise(sig);
}
