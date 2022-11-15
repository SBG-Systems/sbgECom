/*!
 * \file			sbgEComBinaryLogDiag.h
 * \ingroup			binaryLogs
 * \author			SBG Systems
 * \date			12 June 2019
 *
 * \brief			Parse diagnostic logs emitted by the device.
 *
 * \copyright		Copyright (C) 2022, SBG Systems SAS. All rights reserved.
 * \beginlicense	The MIT license
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * \endlicense
 */

#ifndef SBG_ECOM_BINARY_LOG_DIAG_H
#define SBG_ECOM_BINARY_LOG_DIAG_H

// sbgCommonLib headers
#include <sbgCommon.h>
#include <streamBuffer/sbgStreamBuffer.h>

// Project headers
#include <protocol/sbgEComProtocol.h>

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------//
//- Constant definitions                                               -//
//----------------------------------------------------------------------//

/*!
 * Maximum size of the log string, in bytes.
 */
#define SBG_ECOM_LOG_DIAG_MAX_STRING_SIZE					(SBG_ECOM_MAX_PAYLOAD_SIZE - 6)

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * Diagnostic log structure.
 */
typedef struct _SbgLogDiagData
{
	uint32_t							 timestamp;									/*!< Timestamp, in microseconds. */
	SbgDebugLogType						 type;										/*!< Log type. */
	SbgErrorCode						 errorCode;									/*!< Error code. */
	char								 string[SBG_ECOM_LOG_DIAG_MAX_STRING_SIZE];	/*!< Log string, null-terminated. */
} SbgLogDiagData;

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

/*!
 *	Parse data for SBG_ECOM_LOG_DIAG messages and fill the corresponding structure.
 *
 *	\param[in]	pInputStream				Input stream buffer to read the payload from.
 *	\param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 *	\return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseDiagData(SbgStreamBuffer *pInputStream, SbgLogDiagData *pOutputData);


/*!
 * Write data for SBG_ECOM_LOG_DIAG messages to the output stream buffer from the provided structure.
 *
 * \param[out]	pOutputStream				Output stream buffer to write the payload to.
 * \param[in]	pInputData					Pointer on the input structure that stores data to write.
 * \return									SBG_NO_ERROR if the message has been generated in the provided buffer.
 */
SbgErrorCode sbgEComBinaryLogWriteDiagData(SbgStreamBuffer *pOutputStream, const SbgLogDiagData *pInputData);

#ifdef __cplusplus
}
#endif

#endif // SBG_ECOM_BINARY_LOG_DIAG_H
