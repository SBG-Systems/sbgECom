/*!
 * \file			sbgEComBinaryLogSat.h
 * \author			SBG Systems
 * \date			1 March 2022
 *
 * \brief			Handle binary satellite logs.
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

// sbgCommonLib headers
#include <sbgCommon.h>
#include <streamBuffer/sbgStreamBuffer.h>

// Project headers
#include <defs/sbgEComDefsGnss.h>

// Local headers
#include "sbgEComBinaryLogSat.h"

//----------------------------------------------------------------------//
//- Constant definitions                                               -//
//----------------------------------------------------------------------//

#define SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET				(0)														/*!< Offset of the tracking status field, in bits. */
#define SBG_ECOM_LOG_SAT_TRACKING_STATUS_WIDTH				(3)														/*!< Width of the tracking status field, in bits. */
#define SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK				((1u << SBG_ECOM_LOG_SAT_TRACKING_STATUS_WIDTH) - 1)	/*!< Tracking status field mask. */

#define SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET				(3)														/*!< Offset of the health status field, in bits. */
#define SBG_ECOM_LOG_SAT_HEALTH_STATUS_WIDTH				(2)														/*!< Width of the health status field, in bits. */
#define SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK					((1u << SBG_ECOM_LOG_SAT_HEALTH_STATUS_WIDTH) - 1)		/*!< Health status field mask. */

#define SBG_ECOM_LOG_SAT_ELEVATION_STATUS_OFFSET			(5)														/*!< Offset of the elevation status field, in bits. */
#define SBG_ECOM_LOG_SAT_ELEVATION_STATUS_WIDTH				(2)														/*!< Width of the elevation status field, in bits. */
#define SBG_ECOM_LOG_SAT_ELEVATION_STATUS_MASK				((1u << SBG_ECOM_LOG_SAT_ELEVATION_STATUS_WIDTH) - 1)	/*!< Elevation status field mask. */

#define SBG_ECOM_LOG_SAT_CONSTELLATION_ID_OFFSET			(7)														/*!< Offset of the constellation ID field, in bits. */
#define SBG_ECOM_LOG_SAT_CONSTELLATION_ID_WIDTH				(4)														/*!< Width of the constellation ID field, in bits. */
#define SBG_ECOM_LOG_SAT_CONSTELLATION_ID_MASK				((1u << SBG_ECOM_LOG_SAT_CONSTELLATION_ID_WIDTH) - 1)	/*!< Constellation ID field mask. */

#define SBG_ECOM_LOG_SAT_SIGNAL_SNR_VALID					(1u << 5)												/*!< Set if the SNR value is valid. */

//----------------------------------------------------------------------//
//- Private functions                                                  -//
//----------------------------------------------------------------------//

/*!
 * Get a bit field from a set of flags.
 *
 * \param[in]	flags						Flags.
 * \param[in]	offset						Field offset, in bits.
 * \param[in]	mask						Field mask.
 * \return									Field value.
 */
#define sbgEComBinaryLogSatGetField(flags, offset, mask)				(((flags) >> (offset)) & (mask))

/*!
 * Set a bit field from a set of flags.
 *
 * \param[in/out]	flags						Flags.
 * \param[in]		value						Field value.
 * \param[in]		offset						Field offset, in bits.
 * \param[in]		mask						Field mask.
 */
#define sbgEComBinaryLogSatSetField(flags, value, offset, mask)			(flags) &= ~((mask) << (offset)); (flags) |= ((value) & (mask)) << (offset)



/*!
 * Check the value of a health status.
 *
 * \param[in]	healthStatus				Health status.
 * \return									SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComBinaryLogSatCheckHealthStatus(uint8_t healthStatus)
{
	SbgErrorCode						 errorCode = SBG_INVALID_FRAME;

	switch (healthStatus)
	{
	case SBG_ECOM_SAT_HEALTH_STATUS_UNKNOWN:
	case SBG_ECOM_SAT_HEALTH_STATUS_HEALTHY:
	case SBG_ECOM_SAT_HEALTH_STATUS_UNHEALTHY:
		errorCode = SBG_NO_ERROR;
		break;
	}

	if (errorCode != SBG_NO_ERROR)
	{
		SBG_LOG_ERROR(errorCode, "invalid health status: %" PRIu8, healthStatus);
	}

	return errorCode;
}

/*!
 * Check the value of a tracking status.
 *
 * \param[in]	trackingStatus				Tracking status.
 * \return									SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComBinaryLogSatCheckTrackingStatus(uint8_t trackingStatus)
{
	SbgErrorCode						 errorCode = SBG_INVALID_FRAME;

	switch (trackingStatus)
	{
	case SBG_ECOM_SAT_TRACKING_STATUS_UNKNOWN:
	case SBG_ECOM_SAT_TRACKING_STATUS_SEARCHING:
	case SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_UNKNOWN:
	case SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_NOT_USED:
	case SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_REJECTED:
	case SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_USED:	
		errorCode = SBG_NO_ERROR;
		break;
	}

	if (errorCode != SBG_NO_ERROR)
	{
		SBG_LOG_ERROR(errorCode, "invalid tracking status: %" PRIu8, trackingStatus);
	}

	return errorCode;
}

/*!
 * Returns tracking status string from enum value
 *
 * \param[in]	trackingStatus						Tracking status enum to convert
 * \return											Tracking status as a read only C string.
 */
static const char *sbgEComBinaryLogSatTrackingStatusToStr(SbgEComSatTrackingStatus trackingStatus)
{
	static const char				*enumToStrLut[] = 
	{
		[SBG_ECOM_SAT_TRACKING_STATUS_UNKNOWN]				= "unkown",
		[SBG_ECOM_SAT_TRACKING_STATUS_SEARCHING]			= "searching",
		[SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_UNKNOWN]		= "tracking",
		[SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_NOT_USED]	= "unused",
		[SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_REJECTED]	= "rejected",
		[SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_USED]		= "used",
	};

	if (trackingStatus < SBG_ARRAY_SIZE(enumToStrLut))
	{
		return enumToStrLut[trackingStatus];
	}
	else
	{
		return enumToStrLut[SBG_ECOM_SAT_TRACKING_STATUS_UNKNOWN];
	}
}

/*!
 * Returns health status string from enum value
 *
 * \param[in]	trackingStatus						Tracking status enum to convert
 * \return											Tracking status as a read only C string.
 */
static const char *sbgEComBinaryLogSatHealthStatusToStr(SbgEComSatHealthStatus healthStatus)
{
	static const char				*enumToStrLut[] = 
	{
		[SBG_ECOM_SAT_HEALTH_STATUS_UNKNOWN]	= "unkown",
		[SBG_ECOM_SAT_HEALTH_STATUS_HEALTHY]	= "healthy",
		[SBG_ECOM_SAT_HEALTH_STATUS_UNHEALTHY]	= "unhealthy",
	};

	if (healthStatus < SBG_ARRAY_SIZE(enumToStrLut))
	{
		return enumToStrLut[healthStatus];
	}
	else
	{
		return enumToStrLut[SBG_ECOM_SAT_HEALTH_STATUS_UNKNOWN];
	}
}

/*!
 * Parse signal data from a stream buffer.
 *
 * \param[in]	pStreamBuffer				Stream buffer.
 * \param[out]	pSignalData					Signal data.
 * \return									SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComBinaryLogParseSignalData(SbgStreamBuffer *pStreamBuffer, SbgLogSatSignalData *pSignalData)
{
	SbgErrorCode						 errorCode;
	uint8_t								 signalId;

	assert(pSignalData);

	signalId			= sbgStreamBufferReadUint8(pStreamBuffer);
	pSignalData->flags	= sbgStreamBufferReadUint8(pStreamBuffer);
	pSignalData->snr	= sbgStreamBufferReadUint8(pStreamBuffer);

	errorCode = sbgStreamBufferGetLastError(pStreamBuffer);

	if (errorCode == SBG_NO_ERROR)
	{
		if (sbgEComSignalIdIsValid(signalId))
		{
			uint8_t								 healthStatus;

			pSignalData->id = signalId;

			healthStatus = sbgEComBinaryLogSatGetField(pSignalData->flags, SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET, SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);

			errorCode = sbgEComBinaryLogSatCheckHealthStatus(healthStatus);

			if (errorCode == SBG_NO_ERROR)
			{
				uint8_t								 trackingStatus;

				trackingStatus = sbgEComBinaryLogSatGetField(pSignalData->flags, SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET, SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

				errorCode = sbgEComBinaryLogSatCheckTrackingStatus(trackingStatus);
			}
		}
		else
		{
			errorCode = SBG_INVALID_FRAME;
			SBG_LOG_ERROR(errorCode, "invalid signal ID: %" PRIu8, signalId);
		}
	}

	return errorCode;
}

/*!
 * Check the value of an elevation status.
 *
 * \param[in]	elevationStatus				Elevation status value.
 * \return									SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComBinaryLogSatCheckElevationStatus(uint8_t elevationStatus)
{
	SbgErrorCode						 errorCode = SBG_INVALID_FRAME;

	switch (elevationStatus)
	{
	case SBG_ECOM_SAT_ELEVATION_STATUS_UNKNOWN:
	case SBG_ECOM_SAT_ELEVATION_STATUS_SETTING:
	case SBG_ECOM_SAT_ELEVATION_STATUS_RISING:
		errorCode = SBG_NO_ERROR;
		break;
	}

	if (errorCode != SBG_NO_ERROR)
	{
		SBG_LOG_ERROR(errorCode, "invalid elevation status: %" PRIu8, elevationStatus);
	}

	return errorCode;
}

/*!
 * Parse satellite data from a stream buffer.
 *
 * \param[in]	pStreamBuffer				Stream buffer.
 * \param[out]	pSatGroupData				Satellite group data.
 * \return									SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComBinaryLogParseSatData(SbgStreamBuffer *pStreamBuffer, SbgLogSatData *pSatData)
{
	SbgErrorCode						 errorCode;

	assert(pSatData);

	pSatData->id					= sbgStreamBufferReadUint8(pStreamBuffer);
	pSatData->elevation				= sbgStreamBufferReadInt8(pStreamBuffer);
	pSatData->azimuth				= sbgStreamBufferReadUint16(pStreamBuffer);
	pSatData->flags					= sbgStreamBufferReadUint16(pStreamBuffer);
	pSatData->nrSignals				= sbgStreamBufferReadUint8(pStreamBuffer);
	pSatData->signalDataArraySize	= pSatData->nrSignals;

	errorCode = sbgStreamBufferGetLastError(pStreamBuffer);

	if (errorCode == SBG_NO_ERROR)
	{
		if (pSatData->nrSignals <= SBG_ECOM_SAT_MAX_NR_SIGNALS)
		{
			pSatData->pSignalData = malloc(sizeof(*pSatData->pSignalData) * pSatData->signalDataArraySize);

			if (pSatData->pSignalData)
			{
				uint8_t								 constellationId;

				constellationId = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_CONSTELLATION_ID_OFFSET, SBG_ECOM_LOG_SAT_CONSTELLATION_ID_MASK);

				if (sbgEComConstellationIdIsValid(constellationId))
				{
					uint8_t								 elevationStatus;

					elevationStatus = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_ELEVATION_STATUS_OFFSET, SBG_ECOM_LOG_SAT_ELEVATION_STATUS_MASK);

					errorCode = sbgEComBinaryLogSatCheckElevationStatus(elevationStatus);

					if (errorCode == SBG_NO_ERROR)
					{
						uint8_t								 healthStatus;

						healthStatus = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET, SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);

						errorCode = sbgEComBinaryLogSatCheckHealthStatus(healthStatus);

						if (errorCode == SBG_NO_ERROR)
						{
							uint8_t								 trackingStatus;

							trackingStatus = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET, SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

							errorCode = sbgEComBinaryLogSatCheckTrackingStatus(trackingStatus);

							if (errorCode == SBG_NO_ERROR)
							{
								for (size_t i = 0; i < pSatData->nrSignals; i++)
								{
									errorCode = sbgEComBinaryLogParseSignalData(pStreamBuffer, &pSatData->pSignalData[i]);

									if (errorCode != SBG_NO_ERROR)
									{
										break;
									}
								}
							}
						}
					}
				}
				else
				{
					errorCode = SBG_INVALID_FRAME;
					SBG_LOG_ERROR(errorCode, "invalid constellation id: %" PRIu8, constellationId);
				}

				if (errorCode != SBG_NO_ERROR)
				{
					free(pSatData->pSignalData);
					pSatData->pSignalData = NULL;
				}
			}
			else
			{
				errorCode = SBG_MALLOC_FAILED;
				SBG_LOG_ERROR(errorCode, "unable to allocate signal data array");
			}
		}
		else
		{
			errorCode = SBG_INVALID_FRAME;
			SBG_LOG_ERROR(errorCode, "invalid number of signals: %zu", pSatData->nrSignals);
		}
	}

	return errorCode;
}

/*!
 * Satellite data constructor.
 *
 * \param[in]	pSatData					Satellite data.
 * \param[in]	nrSignals					Number of signals.
 * \param[in]	id							Satellite ID.
 * \param[in]	elevation					Elevation, in degrees.
 * \param[in]	azimuth						Azimuth, in degrees.
 * \param[in]	constellationId				Constellation ID.
 * \param[in]	elevationStatus				Elevation status.
 * \param[in]	healthStatus				Health status.
 * \param[in]	trackingStatus				Tracking status.
 * \return									SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgLogSatDataConstruct(SbgLogSatData *pSatData, size_t nrSignals, uint8_t id, int8_t elevation, uint16_t azimuth, SbgEComConstellationId constellationId, SbgEComSatElevationStatus elevationStatus, SbgEComSatHealthStatus healthStatus, SbgEComSatTrackingStatus trackingStatus)
{
	SbgErrorCode						 errorCode = SBG_NO_ERROR;
	uint16_t							 flags = 0;

	assert(pSatData);
	assert(nrSignals <= SBG_ECOM_SAT_MAX_NR_SIGNALS);

	sbgEComBinaryLogSatSetField(flags, constellationId,	SBG_ECOM_LOG_SAT_CONSTELLATION_ID_OFFSET,	SBG_ECOM_LOG_SAT_CONSTELLATION_ID_MASK);
	sbgEComBinaryLogSatSetField(flags, elevationStatus,	SBG_ECOM_LOG_SAT_ELEVATION_STATUS_OFFSET,	SBG_ECOM_LOG_SAT_ELEVATION_STATUS_MASK);
	sbgEComBinaryLogSatSetField(flags, healthStatus,	SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET,		SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);
	sbgEComBinaryLogSatSetField(flags, trackingStatus,	SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET,	SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

	pSatData->id					= id;
	pSatData->elevation				= elevation;
	pSatData->azimuth				= azimuth;
	pSatData->flags					= flags;
	pSatData->signalDataArraySize	= nrSignals;
	pSatData->nrSignals				= 0;

	pSatData->pSignalData = malloc(sizeof(*pSatData->pSignalData) * pSatData->signalDataArraySize);

	if (!pSatData->pSignalData)
	{
		errorCode = SBG_MALLOC_FAILED;
		SBG_LOG_ERROR(errorCode, "unable to allocate signal data array");
	}

	return errorCode;
}

/*!
 * Satellite data destructor.
 *
 * \param[in]	pSatData					Satellite data.
 */
static void sbgLogSatDataDestroy(SbgLogSatData *pSatData)
{
	assert(pSatData);

	free(pSatData->pSignalData);
	pSatData->pSignalData = NULL;
}

/*!
 * Select one of two health statuses according to their priority rules.
 *
 * \param[in]	healthStatus1				First health status.
 * \param[in]	healthStatus2				Second health status.
 */
static SbgEComSatHealthStatus sbgEComBinaryLogSatSelectHealthStatus(SbgEComSatHealthStatus healthStatus1, SbgEComSatHealthStatus healthStatus2)
{
	SbgEComSatHealthStatus				 healthStatus;

	if (healthStatus2 > healthStatus1)
	{
		healthStatus = healthStatus2;
	}
	else
	{
		healthStatus = healthStatus1;
	}

	return healthStatus;
}

/*!
 * Select one of two tracking statuses according to their priority rules.
 *
 * \param[in]	trackingStatus1				First tracking status.
 * \param[in]	trackingStatus2				Second tracking status.
 */
static SbgEComSatTrackingStatus sbgEComBinaryLogSatSelectTrackingStatus(SbgEComSatTrackingStatus trackingStatus1, SbgEComSatTrackingStatus trackingStatus2)
{
	SbgEComSatTrackingStatus			 trackingStatus;

	if (trackingStatus2 > trackingStatus1)
	{
		trackingStatus = trackingStatus2;
	}
	else
	{
		trackingStatus = trackingStatus1;
	}

	return trackingStatus;
}

/*!
 * Update the health and tracking statuses of satellite data.
 *
 * \param[in]	pSatData					Satellite data.
 * \param[in]	healthStatus				Health status.
 * \param[in]	trackingStatus				Tracking status.
 */
static void sbgLogSatDataUpdateStatus(SbgLogSatData *pSatData, SbgEComSatHealthStatus healthStatus, SbgEComSatTrackingStatus trackingStatus)
{
	uint16_t							 flags;

	assert(pSatData);

	flags = pSatData->flags;

	healthStatus	= sbgEComBinaryLogSatSelectHealthStatus(sbgLogSatDataGetHealthStatus(pSatData), healthStatus);
	trackingStatus	= sbgEComBinaryLogSatSelectTrackingStatus(sbgLogSatDataGetTrackingStatus(pSatData), trackingStatus);

	sbgEComBinaryLogSatSetField(flags, healthStatus,	SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET,		SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);
	sbgEComBinaryLogSatSetField(flags, trackingStatus,	SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET,	SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

	pSatData->flags = flags;
}

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParseSatGroupData(SbgStreamBuffer *pStreamBuffer, SbgLogSatGroupData *pSatGroupData)
{
	SbgErrorCode						 errorCode;

	assert(pSatGroupData);
	assert(pStreamBuffer);

	pSatGroupData->timeStamp				= sbgStreamBufferReadUint32LE(pStreamBuffer);
	pSatGroupData->reserved					= sbgStreamBufferReadUint32LE(pStreamBuffer);
	pSatGroupData->nrSatellites				= sbgStreamBufferReadUint8(pStreamBuffer);
	pSatGroupData->satDataArraySize			= pSatGroupData->nrSatellites;

	errorCode = sbgStreamBufferGetLastError(pStreamBuffer);

	if (errorCode == SBG_NO_ERROR)
	{
		if (pSatGroupData->nrSatellites <= SBG_ECOM_SAT_MAX_NR_SATELLITES)
		{
			pSatGroupData->pSatData = malloc(sizeof(*pSatGroupData->pSatData) * pSatGroupData->satDataArraySize);

			if (pSatGroupData->pSatData)
			{
				for (size_t i = 0; i < pSatGroupData->nrSatellites; i++)
				{
					errorCode = sbgEComBinaryLogParseSatData(pStreamBuffer, &pSatGroupData->pSatData[i]);

					if (errorCode != SBG_NO_ERROR)
					{
						break;
					}
				}

				if (errorCode != SBG_NO_ERROR)
				{
					free(pSatGroupData->pSatData);
					pSatGroupData->pSatData = NULL;
				}
			}
			else
			{
				errorCode = SBG_MALLOC_FAILED;
				SBG_LOG_ERROR(errorCode, "unable to allocate satellite data array");
			}
		}
		else
		{
			errorCode = SBG_INVALID_FRAME;
			SBG_LOG_ERROR(errorCode, "invalid number of satellites: %zu", pSatGroupData->nrSatellites);
		}
	}

	return errorCode;
}

SbgErrorCode sbgEComBinaryLogWriteSatGroupData(SbgStreamBuffer *pStreamBuffer, const SbgLogSatGroupData *pSatGroupData)
{
	assert(pStreamBuffer);
	assert(pSatGroupData);
	assert(pSatGroupData->nrSatellites <= UINT8_MAX);

	sbgStreamBufferWriteUint32LE(pStreamBuffer,		pSatGroupData->timeStamp);
	sbgStreamBufferWriteUint32LE(pStreamBuffer,		pSatGroupData->reserved);
	sbgStreamBufferWriteUint8(pStreamBuffer,		(uint8_t)pSatGroupData->nrSatellites);

	for (size_t i = 0; i < pSatGroupData->nrSatellites; i++)
	{
		const SbgLogSatData					*pSatData = &pSatGroupData->pSatData[i];

		assert(pSatData->nrSignals <= UINT8_MAX);

		sbgStreamBufferWriteUint8(pStreamBuffer,		pSatData->id);
		sbgStreamBufferWriteInt8(pStreamBuffer,			pSatData->elevation);
		sbgStreamBufferWriteUint16(pStreamBuffer,		pSatData->azimuth);
		sbgStreamBufferWriteUint16(pStreamBuffer,		pSatData->flags);
		sbgStreamBufferWriteUint8(pStreamBuffer,		(uint8_t)pSatData->nrSignals);

		for (size_t j = 0; j < pSatData->nrSignals; j++)
		{
			const SbgLogSatSignalData			*pSignalData = &pSatData->pSignalData[j];

			sbgStreamBufferWriteUint8(pStreamBuffer,	pSignalData->id);
			sbgStreamBufferWriteUint8(pStreamBuffer,	pSignalData->flags);
			sbgStreamBufferWriteUint8(pStreamBuffer,	pSignalData->snr);
		}
	}

	return sbgStreamBufferGetLastError(pStreamBuffer);
}

SbgErrorCode sbgLogSatGroupDataConstruct(SbgLogSatGroupData *pSatGroupData, size_t nrSatellites, uint32_t timeStamp)
{
	SbgErrorCode						 errorCode = SBG_NO_ERROR;

	assert(pSatGroupData);
	assert(nrSatellites <= SBG_ECOM_SAT_MAX_NR_SATELLITES);

	pSatGroupData->timeStamp				= timeStamp;
	pSatGroupData->reserved					= 0;
	pSatGroupData->nrSatellites				= 0;
	pSatGroupData->satDataArraySize			= nrSatellites;

	pSatGroupData->pSatData = malloc(sizeof(*pSatGroupData->pSatData) * pSatGroupData->satDataArraySize);

	if (!pSatGroupData->pSatData)
	{
		errorCode = SBG_MALLOC_FAILED;
		SBG_LOG_ERROR(errorCode, "unable to allocate satellite data array");
	}

	return SBG_NO_ERROR;
}

void sbgLogSatGroupDataDestroy(SbgLogSatGroupData *pSatGroupData)
{
	assert(pSatGroupData);

	for (size_t i = 0; i < pSatGroupData->nrSatellites; i++)
	{
		sbgLogSatDataDestroy(&pSatGroupData->pSatData[i]);
	}

	free(pSatGroupData->pSatData);
	pSatGroupData->pSatData = NULL;
}

SbgLogSatData *sbgLogSatGroupDataAdd(SbgLogSatGroupData *pSatGroupData, size_t nrSignals, uint8_t id, int8_t elevation, uint16_t azimuth, SbgEComConstellationId constellationId, SbgEComSatElevationStatus elevationStatus, SbgEComSatHealthStatus healthStatus, SbgEComSatTrackingStatus trackingStatus)
{
	SbgLogSatData						*pSatData = NULL;

	assert(pSatGroupData);

	if (pSatGroupData->nrSatellites < pSatGroupData->satDataArraySize)
	{
		SbgErrorCode						 errorCode;
		SbgLogSatData						*pTmpSatData;

		pTmpSatData = &pSatGroupData->pSatData[pSatGroupData->nrSatellites];

		errorCode = sbgLogSatDataConstruct(pTmpSatData, nrSignals, id, elevation, azimuth, constellationId, elevationStatus, healthStatus, trackingStatus);

		if (errorCode == SBG_NO_ERROR)
		{
			pSatData = pTmpSatData;
			pSatGroupData->nrSatellites++;
		}
	}

	return pSatData;
}

SbgLogSatData *sbgLogSatGroupGet(SbgLogSatGroupData *pSatGroupData, uint8_t id)
{
	SbgLogSatData						*pSatData = NULL;

	assert(pSatGroupData);

	for (size_t i = 0; i < pSatGroupData->nrSatellites; i++)
	{
		if (pSatGroupData->pSatData[i].id == id)
		{
			pSatData = &pSatGroupData->pSatData[i];
			break;
		}
	}

	return pSatData;
}

SbgEComConstellationId sbgLogSatDataGetConstellationId(const SbgLogSatData *pSatData)
{
	uint8_t								 value;

	assert(pSatData);

	value = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_CONSTELLATION_ID_OFFSET, SBG_ECOM_LOG_SAT_CONSTELLATION_ID_MASK);

	return (SbgEComConstellationId)value;
}

const char *sbgLogSatDataGetConstellationIdAsStr(const SbgLogSatData *pSatData)
{
	return sbgEComConstellationToStr(sbgLogSatDataGetConstellationId(pSatData));
}

SbgEComSatElevationStatus sbgLogSatDataGetElevationStatus(const SbgLogSatData *pSatData)
{
	uint8_t								 value;

	assert(pSatData);

	value = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_ELEVATION_STATUS_OFFSET, SBG_ECOM_LOG_SAT_ELEVATION_STATUS_MASK);

	return (SbgEComSatElevationStatus)value;
}

const char *sbgLogSatDataGetElevationStatusAsStr(const SbgLogSatData *pSatData)
{
	SbgEComSatElevationStatus		 enumIdx;
	static const char				*enumToStrLut[] = 
	{
		[SBG_ECOM_SAT_ELEVATION_STATUS_UNKNOWN]	= "unkown",
		[SBG_ECOM_SAT_ELEVATION_STATUS_SETTING]	= "setting",
		[SBG_ECOM_SAT_ELEVATION_STATUS_RISING]	= "rising",
	};

	assert(pSatData);
	
	enumIdx = sbgLogSatDataGetElevationStatus(pSatData);

	if (enumIdx < SBG_ARRAY_SIZE(enumToStrLut))
	{
		return enumToStrLut[enumIdx];
	}
	else
	{
		return enumToStrLut[SBG_ECOM_SAT_ELEVATION_STATUS_UNKNOWN];
	}
}

SbgEComSatHealthStatus sbgLogSatDataGetHealthStatus(const SbgLogSatData *pSatData)
{
	uint8_t								 value;

	assert(pSatData);

	value = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET, SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);

	return (SbgEComSatHealthStatus)value;
}

const char *sbgLogSatDataGetHealthStatusAsStr(const SbgLogSatData *pSatData)
{
	assert(pSatData);

	return sbgEComBinaryLogSatHealthStatusToStr(sbgLogSatDataGetHealthStatus(pSatData));
}

SbgEComSatTrackingStatus sbgLogSatDataGetTrackingStatus(const SbgLogSatData *pSatData)
{
	uint8_t								 value;

	assert(pSatData);

	value = sbgEComBinaryLogSatGetField(pSatData->flags, SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET, SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

	return (SbgEComSatTrackingStatus)value;
}

const char *sbgLogSatDataGetTrackingStatusAsStr(const SbgLogSatData *pSatData)
{
	assert(pSatData);

	return sbgEComBinaryLogSatTrackingStatusToStr(sbgLogSatDataGetTrackingStatus(pSatData));
}

SbgLogSatSignalData *sbgLogSatDataAdd(SbgLogSatData *pSatData, SbgEComSignalId id, SbgEComSatHealthStatus healthStatus, SbgEComSatTrackingStatus trackingStatus, bool snrValid, uint8_t snr)
{
	SbgLogSatSignalData					*pSignalData = NULL;

	assert(pSatData);

	if (pSatData->nrSignals < pSatData->signalDataArraySize)
	{
		uint8_t								 flags = 0;

		sbgEComBinaryLogSatSetField(flags, healthStatus,	SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET,		SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);
		sbgEComBinaryLogSatSetField(flags, trackingStatus,	SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET,	SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

		if (snrValid)
		{
			flags |= SBG_ECOM_LOG_SAT_SIGNAL_SNR_VALID;
		}

		pSignalData = &pSatData->pSignalData[pSatData->nrSignals];

		pSignalData->id				= id;
		pSignalData->flags			= flags;
		pSignalData->snr			= snr;

		pSatData->nrSignals++;

		sbgLogSatDataUpdateStatus(pSatData, healthStatus, trackingStatus);
	}

	return pSignalData;
}

SbgLogSatSignalData *sbgLogSatDataGet(SbgLogSatData *pSatData, SbgEComSignalId id)
{
	SbgLogSatSignalData					*pSignalData = NULL;

	assert(pSatData);

	for (size_t i = 0; i < pSatData->nrSignals; i++)
	{
		if (pSatData->pSignalData[i].id == id)
		{
			pSignalData = &pSatData->pSignalData[i];
			break;
		}
	}

	return pSignalData;
}

const char *sbgLogSatSignalDataGetSignalIdAsStr(const SbgLogSatSignalData *pSignalData)
{
	return sbgEComSignalToStr(pSignalData->id);
}

bool sbgLogSatSignalDataSnrIsValid(const SbgLogSatSignalData *pSignalData)
{
	if (pSignalData->flags&SBG_ECOM_LOG_SAT_SIGNAL_SNR_VALID)
	{
		return true;
	}
	else
	{
		return false;
	}
}

SbgEComSatHealthStatus sbgLogSatSignalDataGetHealthStatus(const SbgLogSatSignalData *pSignalData)
{
	uint8_t								 value;

	assert(pSignalData);

	value = sbgEComBinaryLogSatGetField(pSignalData->flags, SBG_ECOM_LOG_SAT_HEALTH_STATUS_OFFSET, SBG_ECOM_LOG_SAT_HEALTH_STATUS_MASK);

	return (SbgEComSatHealthStatus)value;
}

const  char *sbgLogSatSignalDataGetHealthStatusAsStr(const SbgLogSatSignalData *pSignalData)
{
	assert(pSignalData);

	return sbgEComBinaryLogSatHealthStatusToStr(sbgLogSatSignalDataGetHealthStatus(pSignalData));
}

SbgEComSatTrackingStatus sbgLogSatSignalDataGetTrackingStatus(const SbgLogSatSignalData *pSignalData)
{
	uint8_t								 value;

	assert(pSignalData);

	value = sbgEComBinaryLogSatGetField(pSignalData->flags, SBG_ECOM_LOG_SAT_TRACKING_STATUS_OFFSET, SBG_ECOM_LOG_SAT_TRACKING_STATUS_MASK);

	return (SbgEComSatTrackingStatus)value;
}

const char *sbgLogSatSignalDataGetTrackingStatusAsStr(const SbgLogSatSignalData *pSignalData)
{
	assert(pSignalData);

	return sbgEComBinaryLogSatTrackingStatusToStr(sbgLogSatSignalDataGetTrackingStatus(pSignalData));
}
