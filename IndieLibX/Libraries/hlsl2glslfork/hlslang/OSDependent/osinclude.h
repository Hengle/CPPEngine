#ifndef __OSINCLUDE0_H
#define __OSINCLUDE0_H

#if defined(_MSC_VER)
#include "Windows/osinclude.h"
#elif defined(__APPLE__) && !defined(OS_IPHONE) && !defined(IPHONE_SIMULATOR)
#include "Mac/osinclude.h"
#else
#include "Linux/osinclude.h"
#endif

#endif // __OSINCLUDE0_H
