#ifndef SNN_PORTME_H
#define SNN_PORTME_H

#ifndef HAS_KLIB
#define	HAS_KLIB 1
#endif

/************************/
/* Data types and settings */
/************************/
/* Configuration : HAS_FLOAT
	Define to 1 if the platform supports floating point.
*/
#ifndef HAS_FLOAT
#define HAS_FLOAT 0
#endif
/* Configuration : HAS_TIME_H
	Define to 1 if platform has the time.h header file,
	and implementation of functions thereof.
*/
#ifndef HAS_TIME_H
#define HAS_TIME_H 0
#endif
/* Configuration : USE_CLOCK
	Define to 1 if platform has the time.h header file,
	and implementation of functions thereof.
*/
#ifndef USE_CLOCK
#define USE_CLOCK 0
#endif
/* Configuration : HAS_STDIO
	Define to 1 if the platform has stdio.h.
*/
#ifndef HAS_STDIO
#define HAS_STDIO 0
#endif

#ifndef	HAS_STDLIB
#define	HAS_STDLIB 0
#endif

#endif  /* SNN_PORTME_H*/