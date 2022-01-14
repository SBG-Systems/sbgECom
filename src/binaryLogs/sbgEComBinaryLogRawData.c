// sbgCommonLib headers
#include <sbgCommon.h>

// Local headers
#include "sbgEComBinaryLogRawData.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseRawData(SbgStreamBuffer *pInputStream, SbgLogRawData *pOutputData)
{
	SbgErrorCode	errorCode = SBG_NO_ERROR;
	size_t			payloadSize;

	assert(pInputStream);
	assert(pOutputData);

	payloadSize = sbgStreamBufferGetSize(pInputStream);

	if (payloadSize <= SBG_ECOM_RAW_DATA_MAX_BUFFER_SIZE)
	{
		errorCode = sbgStreamBufferReadBuffer(pInputStream, pOutputData->rawBuffer, payloadSize);
		pOutputData->bufferSize = payloadSize;
	}
	else
	{
		errorCode = SBG_BUFFER_OVERFLOW;
	}

	return errorCode;
}

SbgErrorCode sbgEComBinaryLogWriteRawData(SbgStreamBuffer *pOutputStream, const SbgLogRawData *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	return sbgStreamBufferWriteBuffer(pOutputStream, pInputData->rawBuffer, pInputData->bufferSize);
}
