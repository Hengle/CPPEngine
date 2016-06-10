#if defined(ANDROID) || defined(_LINUX)
#include <malloc.h>
#endif

long getNativeHeapSize()
{
#if defined(ANDROID) || defined(_LINUX)
    struct mallinfo info = mallinfo();
    return (long) info.usmblks;
#else
    return 0;
#endif
}

long getNativeHeapAllocatedSize()
{
#if defined(ANDROID) || defined(_LINUX)
    struct mallinfo info = mallinfo();
    return (long) info.uordblks;
#else
    return 0;
#endif
}

long getNativeHeapFreeSize()
{
#if defined(ANDROID) || defined(_LINUX)
    struct mallinfo info = mallinfo();
    return (long) info.fordblks;
#else
    return 0;
#endif
}

