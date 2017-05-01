#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <signal.h>

#include <windows.h>

#include "zio-con.h"
#include "zio-list.h"
#include "zio-reader.h"


#define NUM 1000000
#define CHU 100000


void benchmark_routine(void (*routine)(void), void (*empty_routine)(void), unsigned long cycles);
void empty_1(void);
void routine_1(void);
void handle_signal(int);

alist ali;

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

    mlog("Writing %ld integers into empty list (intlist_append)", (long) NUM);

    /*disable my debug messages since they slow down the process quite a bit*/
    debug = 0;
    GetSystemTimeAsFileTime(&sysFileTime); //Start meassuring time
    start_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                         sysFileTime.dwHighDateTime) << 32LL);

    for(count = 0; count < NUM; count++) {
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
        alist_create_type(ali, long, 3);

        alist_append_type(ali, long, 34);
        alist_append_type(ali, long, 1234);
        alist_append_type(ali, long, 12);
        alist_append_type(ali, long, -3);


        mlog("Erster Wert in ali: %ld, laenge von ali: %ld, max. laenge von ali: %ld", *((long*) ali.start_ptr), ali.length, ali.allocated_length);
        mlog("Blockgroesze von ali: %ld, Einheitsgroesze von ali: %ld", ali.block_units, ali.usize);

        mlog("------");

        alist_free(&ali);
        alist_create_type(ali, long, CHU);

        benchmark_routine(empty_1, routine_1, NUM);
        mlog("Erster Wert in ali: %ld, laenge von ali: %ld, max. laenge von ali: %ld", *((long*) ali.start_ptr), ali.length, ali.allocated_length);
        mlog("Blockgroesze von ali: %ld, Einheitsgroesze von ali: %ld", ali.block_units, ali.usize);
    }

    return 0;
}

void empty_1(void)
{
    return;
}

void routine_1(void)
{
    alist_append_type(ali, long, 1234);
}

void benchmark_routine(void (*empty_routine)(void), void (*routine)(void), unsigned long cycles)
{
    /*timer loop variables*/
    unsigned long count;
    FILETIME sysFileTime;
    LONGLONG start_100nanos;
    LONGLONG end_100nanos;

    mlog("Running empty_routine %ld times", cycles);

    /*disable my debug messages since they slow down the process quite a bit*/
    debug = 0;
    GetSystemTimeAsFileTime(&sysFileTime); //Start meassuring time
    start_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                         sysFileTime.dwHighDateTime) << 32LL);

    for(count = 0; count < cycles; count++) {
        (*empty_routine)();
    }

    GetSystemTimeAsFileTime(&sysFileTime); //Stop meassuring time
    end_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                       sysFileTime.dwHighDateTime) << 32LL);
     /*re-enable my debug messages*/
    debug = 1;

    mlog("Elapsed seconds (empty): %f",
         ((float) (end_100nanos - start_100nanos))/10000000);

    /*--------------------------------------*/
    mlog("Running routine %ld times", cycles);

    /*disable my debug messages since they slow down the process quite a bit*/
    debug = 0;
    GetSystemTimeAsFileTime(&sysFileTime); //Start meassuring time
    start_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                         sysFileTime.dwHighDateTime) << 32LL);

    for(count = 0; count < cycles; count++) {
        (*routine)();
    }

    GetSystemTimeAsFileTime(&sysFileTime); //Stop meassuring time
    end_100nanos = (LONGLONG)sysFileTime.dwLowDateTime + ((LONGLONG)(
                       sysFileTime.dwHighDateTime) << 32LL);
     /*re-enable my debug messages*/
    debug = 1;

    mlog("Elapsed seconds (routine): %f",
         ((float) (end_100nanos - start_100nanos))/10000000);
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
