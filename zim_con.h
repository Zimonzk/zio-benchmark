/*
 * This the header for libzimcon, a basic logging utility
 * Version v0.2
 */

#ifndef __zim_con__
#define __zim_con__

#ifndef _NOMAIN_
extern short int debug = 1;
#endif // _NOMAIN_

extern void mlog(char *, ...);
extern void mwarn(char *, ...);
extern void merror(char *, ...);
extern void mdebug(char *, ...);

#endif // __zim_con__
