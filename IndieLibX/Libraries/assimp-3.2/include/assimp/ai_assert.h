/** @file ai_assert.h
 */
#ifndef AI_DEBUG_H_INC
#define AI_DEBUG_H_INC

#ifdef ASSIMP_BUILD_DEBUG
#   include <assert.h>
#   define  ai_assert(expression) assert(expression)
#else
#   define  ai_assert(expression)
#endif


#if 0//defined(OS_IPHONE) || defined(IPHONE_SIMULATOR)
namespace std
{
	inline float abs(float a)
	{
		return fabs(a);
	}
	inline float sqrtf(float a)
	{
		return sqrtf(a);
	}
	inline float sin(float a)
	{
		return sinf(a);
	}
	inline float cos(float a)
	{
		return cosf(a);
	}
};
#endif
#endif
