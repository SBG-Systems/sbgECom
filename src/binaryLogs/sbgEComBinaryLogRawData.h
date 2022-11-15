/*!
 * \file			sbgEComBinaryLogRawData.h
 * \ingroup			binaryLogs
 * \author			SBG Systems
 * \date			16 November 2020
 *
 * \brief			Parse logs used to store a binary stream such as RAW GNSS data.
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

#ifndef SBG_ECOM_BINARY_LOG_RAW_DATA_H
#define SBG_ECOM_BINARY_LOG_RAW_DATA_H

// sbgCommonLib headers
#include <sbgCommon.h>
#include <streamBuffer/sbgStreamBuffer.h>

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------//
//- Log raw Data const definitions                                     -//
//----------------------------------------------------------------------//

#define SBG_ECOM_RAW_DATA_MAX_BUFFER_SIZE						(4086u)	/*!< Maximum buffer size in bytes that can be stored in the raw data log. */

//----------------------------------------------------------------------//
//- Log structure definitions                                          -//
//----------------------------------------------------------------------//

/*!
 * Structure that stores raw data message.
 */
typedef struct _SbgLogRawData
{
	uint8_t			rawBuffer[SBG_ECOM_RAW_DATA_MAX_BUFFER_SIZE];	/*!< Buffer that contains raw data. */
	size_t			bufferSize;										/*!< Raw buffer size in bytes. */
} SbgLogRawData;

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

/*!
 * Parse raw data message and fill the corresponding structure.
 *
 * \param[in]	pInputStream				Input stream buffer to read the payload from.
 * \param[out]	pOutputData					Pointer on the output structure that stores parsed data.
 * \return									SBG_NO_ERROR if the payload has been parsed.
 */
SbgErrorCode sbgEComBinaryLogParseRawData(SbgStreamBuffer *pInputStream, SbgLogRawData *pOutputData);

/*!
 * Write raw data message to the output stream buffer from the provided structure.
 *
 * \param[out]	pOutputStream				Output stream buffer to write the payload to.
 * \param[in]	pInputData					Pointer on the input structure that stores data to write.
 * \return									SBG_NO_ERROR if the message has been generated in the provided buffer.
 */
SbgErrorCode sbgEComBinaryLogWriteRawData(SbgStreamBuffer *pOutputStream, const SbgLogRawData *pInputData);

#ifdef __cplusplus
}
#endif

#endif // SBG_ECOM_BINARY_LOG_RAW_DATA_H
