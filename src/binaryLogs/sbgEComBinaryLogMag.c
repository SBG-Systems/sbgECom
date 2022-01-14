#include "sbgEComBinaryLogMag.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseMagData(SbgStreamBuffer *pInputStream, SbgLogMag *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp			= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->status				= sbgStreamBufferReadUint16LE(pInputStream);

	pOutputData->magnetometers[0]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->magnetometers[1]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->magnetometers[2]	= sbgStreamBufferReadFloatLE(pInputStream);
				
	pOutputData->accelerometers[0]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->accelerometers[1]	= sbgStreamBufferReadFloatLE(pInputStream);
	pOutputData->accelerometers[2]	= sbgStreamBufferReadFloatLE(pInputStream);

	//
	// Return if any error has occurred while parsing the frame
	//
	return sbgStreamBufferGetLastError(pInputStream);
}

SbgErrorCode sbgEComBinaryLogWriteMagData(SbgStreamBuffer *pOutputStream, const SbgLogMag *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->status);
	
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->magnetometers[0]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->magnetometers[1]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->magnetometers[2]);
		
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->accelerometers[0]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->accelerometers[1]);
	sbgStreamBufferWriteFloatLE(pOutputStream, pInputData->accelerometers[2]);

	//
	// Return if any error has occurred while writing the frame
	//
	return sbgStreamBufferGetLastError(pOutputStream);
}

SbgErrorCode sbgEComBinaryLogParseMagCalibData(SbgStreamBuffer *pInputStream, SbgLogMagCalib *pOutputData)
{
	assert(pInputStream);
	assert(pOutputData);

	//
	// Read the frame payload
	//
	pOutputData->timeStamp	= sbgStreamBufferReadUint32LE(pInputStream);
	pOutputData->reserved	= sbgStreamBufferReadUint16LE(pInputStream);

	//
	// Read the raw magnetic calibration data buffer
	//
	return sbgStreamBufferReadBuffer(pInputStream, pOutputData->magData, sizeof(pOutputData->magData));
}

SbgErrorCode sbgEComBinaryLogWriteMagCalibData(SbgStreamBuffer *pOutputStream, const SbgLogMagCalib *pInputData)
{
	assert(pOutputStream);
	assert(pInputData);

	//
	// Write the frame payload
	//
	sbgStreamBufferWriteUint32LE(pOutputStream, pInputData->timeStamp);
	sbgStreamBufferWriteUint16LE(pOutputStream, pInputData->reserved);

	//
	// Write the raw magnetic calibration data buffer
	//
	return sbgStreamBufferWriteBuffer(pOutputStream, pInputData->magData, sizeof(pInputData->magData));
}
