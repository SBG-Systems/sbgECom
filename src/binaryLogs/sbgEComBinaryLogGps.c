#include "sbgEComBinaryLogGps.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseGpsVelData(SbgStreamBuffer *pInputStream, SbgLogGpsVel *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->timeOfWeek		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->velocity[0]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocity[1]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocity[2]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocityAcc[0]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocityAcc[1]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocityAcc[2]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->course			= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->courseAcc		= sbgStreamBufferReadFloatLE(pInputStream);

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteGpsVelData(SbgStreamBuffer *pOutputStream, const SbgLogGpsVel *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->status);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeOfWeek);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocity[0]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocity[1]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocity[2]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocityAcc[0]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocityAcc[1]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocityAcc[2]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->course);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->courseAcc);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}

SbgErrorCode sbgEComBinaryLogParseGpsPosData(SbgStreamBuffer *pInputStream, SbgLogGpsPos *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status				= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->timeOfWeek			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->latitude			= sbgStreamBufferReadDoubleLE(pInputStream);
	pOutputData->longitude			= sbgStreamBufferReadDoubleLE(pInputStream);
	pOutputData->altitude			= sbgStreamBufferReadDoubleLE(pInputStream);
	pOutputData->undulation			= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->latitudeAccuracy	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->longitudeAccuracy	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->altitudeAccuracy	= sbgStreamBufferReadFloatLE(pInputStream);

	//
	// Test if we have a additional information such as base station id (since version 1.4)
	//
	if (sbgStreamBufferGetSpace(pInputStream) >= 5)
	{
		//
		// Read the additional information
		//
		pOutputData->numSvUsed			= sbgStreamBufferReadUint8LE(pInputStream);
		pOutputData->baseStationId		= sbgStreamBufferReadUint16LE(pInputStream);
		pOutputData->differentialAge	= sbgStreamBufferReadUint16LE(pInputStream);
	}
	else
	{
		//
		// Default the additional information
		//
		pOutputData->numSvUsed = 0;
		pOutputData->baseStationId = 0xFFFF;
		pOutputData->differentialAge = 0xFFFF;
	}

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteGpsPosData(SbgStreamBuffer *pOutputStream, const SbgLogGpsPos *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->status);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeOfWeek);

	sbgStreamBufferWriteDoubleLE(pOutputStream, pInputData->latitude);
	sbgStreamBufferWriteDoubleLE(pOutputStream, pInputData->longitude);
	sbgStreamBufferWriteDoubleLE(pOutputStream, pInputData->altitude);

	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->undulation);

	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->latitudeAccuracy);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->longitudeAccuracy);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->altitudeAccuracy);

	//
	// Write the additional information added in version 1.4
	//
	sbgStreamBufferWriteUint8LE(pOutputStream, pInputData->numSvUsed);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->baseStationId);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->differentialAge);
	
	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}

SbgErrorCode sbgEComBinaryLogParseGpsHdtData(SbgStreamBuffer *pInputStream, SbgLogGpsHdt *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status				= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->timeOfWeek			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->heading			= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->headingAccuracy	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->pitch				= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->pitchAccuracy		= sbgStreamBufferReadFloatLE(pInputStream);

	//
	// The baseline field have been added in version 2.0
	//
	if (sbgStreamBufferGetSpace(pInputStream) > 0)
	{
		pOutputData->baseline		= sbgStreamBufferReadFloatLE(pInputStream);
	}
	else
	{
		pOutputData->baseline		= 0.0f;
	}

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteGpsHdtData(SbgStreamBuffer *pOutputStream, const SbgLogGpsHdt *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->status);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeOfWeek);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->heading);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->headingAccuracy);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->pitch);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->pitchAccuracy);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->baseline);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}

SbgErrorCode sbgEComBinaryLogParseGpsRawData(SbgStreamBuffer *pInputStream, SbgLogRawData *pOutputData)
{
	return sbgEComBinaryLogParseRawData(pInputStream, pOutputData);
}

SbgErrorCode sbgEComBinaryLogWriteGpsRawData(SbgStreamBuffer *pOutputStream, const SbgLogRawData *pInputData)
{
	return sbgEComBinaryLogWriteRawData(pOutputStream, pInputData);
}
