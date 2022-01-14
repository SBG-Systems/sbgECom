#include "sbgEComBinaryLogDvl.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseDvlData(SbgStreamBuffer *pInputStream, SbgLogDvlData *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status				= sbgStreamBufferReadUint16LE(pInputStream);

	pOutputData->velocity[0]		= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocity[1]		= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocity[2]		= sbgStreamBufferReadFloatLE(pInputStream);

	pOutputData->velocityQuality[0]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocityQuality[1]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->velocityQuality[2]	= sbgStreamBufferReadFloatLE(pInputStream);

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteDvlData(SbgStreamBuffer *pOutputStream, const SbgLogDvlData *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->status);

	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocity[0]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocity[1]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocity[2]);

	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocityQuality[0]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocityQuality[1]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->velocityQuality[2]);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}
