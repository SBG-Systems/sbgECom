#include "sbgEComBinaryLogEvent.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseEvent(SbgStreamBuffer *pInputStream, SbgLogEvent *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status			= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->timeOffset0	= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->timeOffset1	= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->timeOffset2	= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->timeOffset3	= sbgStreamBufferReadUint16LE(pInputStream);
	
	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteEvent(SbgStreamBuffer *pOutputStream, const SbgLogEvent *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->status);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->timeOffset0);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->timeOffset1);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->timeOffset2);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->timeOffset3);
	
	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}
