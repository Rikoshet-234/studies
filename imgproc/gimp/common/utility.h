#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H


#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef CLAMP
#define CLAMP(x, a, b) (MAX((a), MIN((b), (x))))
#endif


#endif
