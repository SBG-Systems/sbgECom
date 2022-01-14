// sbgCommonLib headers
#include <sbgCommon.h>

// Local headers
#include "sbgEComBinaryLogRtcm.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseRtcmRawData(SbgStreamBuffer *pInputStream, SbgLogRawData *pOutputData)
{
	return sbgEComBinaryLogParseRawData(pInputStream, pOutputData);
}

SbgErrorCode sbgEComBinaryLogWriteRtcmRawData(SbgStreamBuffer *pOutputStream, const SbgLogRawData *pInputData)
{
	return sbgEComBinaryLogWriteRawData(pOutputStream, pInputData);
}
