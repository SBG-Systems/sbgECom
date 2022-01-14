#include "sbgEComBinaryLogUtc.h"

//----------------------------------------------------------------------//
//- Private global definitions                                         -//
//----------------------------------------------------------------------//

/*!< Lookup table for clock status enum */
static const char *gClockStatusStr[] =
{
	[SBG_ECOM_CLOCK_ERROR]			= "SBG_ECOM_CLOCK_ERROR",
	[SBG_ECOM_CLOCK_FREE_RUNNING]	= "SBG_ECOM_CLOCK_FREE_RUNNING",
	[SBG_ECOM_CLOCK_STEERING]		= "SBG_ECOM_CLOCK_STEERING",
	[SBG_ECOM_CLOCK_VALID]			= "SBG_ECOM_CLOCK_VALID"
};

/*!< Lookup table for UTC status enum */
static const char *gUtcStatusStr[] =
{
	[SBG_ECOM_UTC_INVALID]			= "SBG_ECOM_UTC_INVALID",
	[SBG_ECOM_UTC_NO_LEAP_SEC]		= "SBG_ECOM_UTC_NO_LEAP_SEC",
	[SBG_ECOM_UTC_VALID]			= "SBG_ECOM_UTC_VALID",
};

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

const char *sbgEcomLogUtcGetClockStatusAsString(const SbgLogUtcData *pLogUtc)
{
	SbgEComClockStatus		clockStatus;
	
	assert(pLogUtc);

	clockStatus = sbgEComLogUtcGetClockStatus(pLogUtc->status);
	assert(clockStatus < SBG_ARRAY_SIZE(gClockStatusStr));

	return gClockStatusStr[clockStatus];
}

const char *sbgEcomLogUtcGetUtcStatusAsString(const SbgLogUtcData *pLogUtc)
{
	SbgEComClockUtcStatus	utcStatus;
	
	assert(pLogUtc);

	utcStatus = sbgEComLogUtcGetClockUtcStatus(pLogUtc->status);
	assert(utcStatus < SBG_ARRAY_SIZE(gUtcStatusStr));

	return gUtcStatusStr[utcStatus];
}

SbgErrorCode sbgEComBinaryLogParseUtcData(SbgStreamBuffer *pInputStream, SbgLogUtcData *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status			= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->year			= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->month			= sbgStreamBufferReadInt8LE(pInputStream);
	pOutputData->day			= sbgStreamBufferReadInt8LE(pInputStream);
	pOutputData->hour			= sbgStreamBufferReadInt8LE(pInputStream);
	pOutputData->minute			= sbgStreamBufferReadInt8LE(pInputStream);
	pOutputData->second			= sbgStreamBufferReadInt8LE(pInputStream);
	pOutputData->nanoSecond		= sbgStreamBufferReadInt32LE(pInputStream);
	pOutputData->gpsTimeOfWeek	= sbgStreamBufferReadUint32LE(pInputStream);

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteUtcData(SbgStreamBuffer *pOutputStream, const SbgLogUtcData *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->status);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->year);
	sbgStreamBufferWriteInt8LE(pOutputStream, pInputData->month);
	sbgStreamBufferWriteInt8LE(pOutputStream, pInputData->day);
	sbgStreamBufferWriteInt8LE(pOutputStream, pInputData->hour);
	sbgStreamBufferWriteInt8LE(pOutputStream, pInputData->minute);
	sbgStreamBufferWriteInt8LE(pOutputStream, pInputData->second);
	sbgStreamBufferWriteInt32LE(pOutputStream, pInputData->nanoSecond);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->gpsTimeOfWeek);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}
