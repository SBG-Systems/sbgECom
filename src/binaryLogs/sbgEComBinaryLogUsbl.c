#include "sbgEComBinaryLogUsbl.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseUsblData(SbgStreamBuffer *pInputStream, SbgLogUsblData *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status				= sbgStreamBufferReadUint16LE(pInputStream);

	pOutputData->latitude			= sbgStreamBufferReadDoubleLE(pInputStream);
	pOutputData->longitude			= sbgStreamBufferReadDoubleLE(pInputStream);

	pOutputData->depth				= sbgStreamBufferReadFloatLE(pInputStream);

	pOutputData->latitudeAccuracy	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->longitudeAccuracy	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->depthAccuracy		= sbgStreamBufferReadFloatLE(pInputStream);

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteUsblData(SbgStreamBuffer *pOutputStream, const SbgLogUsblData *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->status);

	sbgStreamBufferWriteDoubleLE(pOutputStream, pInputData->latitude);
	sbgStreamBufferWriteDoubleLE(pOutputStream, pInputData->longitude);

	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->depth);
	
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->latitudeAccuracy);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->longitudeAccuracy);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->depthAccuracy);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}
