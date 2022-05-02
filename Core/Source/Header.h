#pragma once

#ifdef RSO_USING_DLL
#define RSO_STATIC ""
#else
#define RSO_STATIC "s"
#endif //RSO_USING_DLL

#ifdef _DLL
#define RSO_RT_LIBRARY "MD"
#else
#define RSO_RT_LIBRARY "MT"
#endif // _DLL

#ifdef _DEBUG
#define RSO_DEBUG_TYPE "d"
#else
#define RSO_DEBUG_TYPE ""
#endif

#define MAKE_STR( Value_ ) #Value_

#pragma comment( lib, "Rso/Core_" RSO_STATIC RSO_RT_LIBRARY RSO_DEBUG_TYPE )
