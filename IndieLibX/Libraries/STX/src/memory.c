#if defined(ANDROID)
#include <malloc.h>
#endif

unsigned int getNativeHeapSize()
{
#if defined(ANDROID)
    struct mallinfo info = mallinfo();
    return (int) info.usmblks;
#else
    return 0;
#endif
}

unsigned int getNativeHeapAllocatedSize()
{
#if defined(ANDROID)
    struct mallinfo info = mallinfo();
    return (int) info.uordblks;
#else
    return 0;
#endif
}

unsigned int getNativeHeapFreeSize()
{
#if defined(ANDROID)
    struct mallinfo info = mallinfo();
    return (int) info.fordblks;
#else
    return 0;
#endif
}

