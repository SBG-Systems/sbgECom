// sbgCommonLib headers
#include <sbgCommon.h>

#ifdef USE_BUILD_NUMBER_GENERATOR
#include <sbgCommonLibVersion.h>
#endif

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

SBG_COMMON_LIB_API uint32_t sbgCommonLibGetVersion(void)
{
#ifdef USE_BUILD_NUMBER_GENERATOR
	return SBG_COMMON_LIB_VERSION;
#else
	return 0;
#endif
}

SBG_COMMON_LIB_API bool sbgCommonLibIsDebug(void)
{
#ifdef NDEBUG
	return false;
#else
	return true;
#endif
}
