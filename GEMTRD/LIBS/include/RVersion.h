#ifndef ROOT_RVersion
#define ROOT_RVersion

/* Version information automatically generated by installer. */

/*
 * These macros can be used in the following way:
 *
 *    #if ROOT_VERSION_CODE >= ROOT_VERSION(2,23,4)
 *       #include <newheader.h>
 *    #else
 *       #include <oldheader.h>
 *    #endif
 *
*/

#define ROOT_RELEASE "5.34/36"
#define ROOT_RELEASE_DATE "Apr  5 2016"
#define ROOT_RELEASE_TIME "10:24:40"
#define ROOT_SVN_REVISION 49361
#define ROOT_GIT_COMMIT "v5-34-34-76-g57bae4c"
#define ROOT_GIT_BRANCH "heads/v5-34-00-patches"
#define ROOT_VERSION_CODE 336420
#define ROOT_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#endif