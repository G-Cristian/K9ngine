#ifndef _K9DEBUG_H_
#define _K9DEBUG_H_

#include <assert.h>

#ifdef _DEBUG
// TODO: make own assert
#ifndef K9_ASSERT

#define K9_ASSERT(exp) assert(exp)
#else
#define K9_ASSERT(exp)
#endif

#endif // !K9_ASSERT

#endif // !_K9DEBUG_H_
