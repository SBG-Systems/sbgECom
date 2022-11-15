#include "sbgEComBinaryLogStatus.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseStatusData(SbgStreamBuffer *pInputStream, SbgLogStatusData *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->generalStatus	= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->comStatus2		= sbgStreamBufferReadUint16LE(pInputStream);
	pOutputData->comStatus		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->aidingStatus	= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->reserved2		= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->reserved3		= sbgStreamBufferReadUint16LE(pInputStream);
	
	//
	// Test if we have a additional information such as uptime (since version 1.7)
	//
	if (sbgStreamBufferGetSpace(pInputStream) >= sizeof(uint32_t))
	{
		//
		// Read the additional information
		//
		pOutputData->uptime		= sbgStreamBufferReadUint32LE(pInputStream);
	}
	else
	{
		//
		// Default the additional information
		//
		pOutputData->uptime = 0;
	}
	
	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteStatusData(SbgStreamBuffer *pOutputStream, const SbgLogStatusData *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->generalStatus);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->comStatus2);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->comStatus);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->aidingStatus);
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->reserved2);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->reserved3);
	
	//
	// Write the additional information added in version 1.7
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->uptime);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}
