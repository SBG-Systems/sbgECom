/*!
 * \file			sbgEComBinaryLogSat.h
 * \ingroup			binaryLogs
 * \author			SBG Systems
 * \date			1 March 2022
 *
 * \brief			Parse space vehicles in view information log.
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

#ifndef SBG_ECOM_BINARY_LOG_SAT_H
#define SBG_ECOM_BINARY_LOG_SAT_H

// sbgCommonLib headers
#include <sbgCommon.h>
#include <streamBuffer/sbgStreamBuffer.h>

// Project headers
#include <defs/sbgEComDefsGnss.h>

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------//
//- Constant definitions                                               -//
//----------------------------------------------------------------------//

/*!
 * Maximum number of satellites in a satellite group.
 */
#define SBG_ECOM_SAT_MAX_NR_SATELLITES						(64)

/*!
 * Maximum number of signals per satellite.
 */
#define SBG_ECOM_SAT_MAX_NR_SIGNALS							(8)

//----------------------------------------------------------------------//
//- Enumeration definitions                                            -//
//----------------------------------------------------------------------//

/*!
 * Tracking status.
 *
 * The tracking status embeds the solution status when the latter is known.
 *
 * All values must be strictly lower than 8.
 *
 * These are on-the-wire values.
 */
typedef enum _SbgEComSatTrackingStatus
{
	SBG_ECOM_SAT_TRACKING_STATUS_UNKNOWN			= 0,							/*!< Unknown tracking status such as no signal / idle. */
	SBG_ECOM_SAT_TRACKING_STATUS_SEARCHING			= 1,							/*!< Signal is beeing searched and can't be used yet. */
	SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_UNKNOWN	= 2,							/*!< Signal is tracked but don't know if used or not in the solution. */
	SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_NOT_USED	= 3,							/*!< Signal is tracked and is not used in the solution. */
	SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_REJECTED	= 4,							/*!< Signal is tracjed and is rejected from the solution. */
	SBG_ECOM_SAT_TRACKING_STATUS_TRACKING_USED		= 5,							/*!< Signal is tracked and used in the solution. */
} SbgEComSatTrackingStatus;

/*!
 * Health status.
 *
 * All values must be strictly lower than 4.
 *
 * These are on-the-wire values.
 */
typedef enum _SbgEComSatHealthStatus
{
	SBG_ECOM_SAT_HEALTH_STATUS_UNKNOWN			= 0,								/*!< Don't know the satellite or the signal health status. */
	SBG_ECOM_SAT_HEALTH_STATUS_HEALTHY			= 1,								/*!< The satellite or the signal is healthy and can be used. */
	SBG_ECOM_SAT_HEALTH_STATUS_UNHEALTHY		= 2,								/*!< The satellite or the signal is not healthy and can't be used. */
} SbgEComSatHealthStatus;

/*!
 * Elevation status.
 *
 * All values must be strictly lower than 4.
 *
 * These are on-the-wire values.
 */
typedef enum _SbgEComSatElevationStatus
{
	SBG_ECOM_SAT_ELEVATION_STATUS_UNKNOWN		= 0,								/*!< Don't know if the satellite elevation is setting or rising. */
	SBG_ECOM_SAT_ELEVATION_STATUS_SETTING		= 1,								/*!< The satellite elevation is setting. */
	SBG_ECOM_SAT_ELEVATION_STATUS_RISING		= 2,								/*!< The satellite elevation is rising */
} SbgEComSatElevationStatus;

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * Satellite signal data.
 *
 * The flags include the snr available, health and tracking statuses.
 */
typedef struct _SbgLogSatSignalData
{
	SbgEComSignalId						 id;										/*!< Signal ID. */
	uint8_t								 flags;										/*!< Flags. */
	uint8_t								 snr;										/*!< Signal-to-noise ratio, in dB. */
} SbgLogSatSignalData;

/*!
 * Satellite data.
 *
 * The flags include the constellation ID, the elevation status, the health status, and the tracking status.
 *
 * Satellite data and signal data each have their own health and tracking statuses. The statuses of satellite
 * data may be a priority-based summary of the statuses of signal data, or they may reflect information that
 * is limited to a satellite and unavailable for its signals.
 *
 * The priority rules are :
 *  - health status : unhealthy -> healthy -> unknown
 *  - tracking status : tracking and used -> tracking and not used -> tracking (solution status unknown) -> searching -> unknown
 *
 * For example, if satellite data have an unknown tracking status, and have three signals, one with the
 * searching status, another with the tracking and not used status, and the last with the tracking and used status,
 * the satellite data tracking status will be tracking and used.
 * But if those satellite data are initially set with a healthy health status, and all three signals added have the
 * unknown health status, the satellite data health status remains healthy.
 */
typedef struct _SbgLogSatData
{
	uint8_t 							 id;										/*!< Satellite ID. */
	int8_t								 elevation;									/*!< Elevation, in degrees [-90; +90], valid if and only if the elevation is known. */
	uint16_t							 azimuth;									/*!< Azimuth, in degrees [0; 359], valid if and only if the elevation is known. */
	uint16_t							 flags;										/*!< Flags. */
	size_t								 nrSignals;									/*!< Number of signals. */
	size_t								 signalDataArraySize;						/*!< Size of the signal data array. */
	SbgLogSatSignalData					*pSignalData;								/*!< Signal data array. */
} SbgLogSatData;

/*!
 * Satellite group data.
 */
typedef struct _SbgLogSatGroupData
{
	uint32_t							 timeStamp;									/*!< Time since the sensor power up, in us. */
	uint32_t							 reserved;									/*!< Reserved for future use. */
	size_t								 nrSatellites;								/*!< Number of satellites. */
	size_t								 satDataArraySize;							/*!< Size of the satellite data array. */
	SbgLogSatData						*pSatData;									/*!< Satellite data array. */
} SbgLogSatGroupData;

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

/*!
 * Parse satellite group data from a stream buffer.
 *
 * This function constructs the satellite group data, which must be destroyed once not needed any more.
 *
 * \param[in]	pStreamBuffer				Stream buffer.
 * \param[out]	pSatGroupData				Satellite group data.
 * \return									SBG_NO_ERROR if successful.
 */
SbgErrorCode sbgEComBinaryLogParseSatGroupData(SbgStreamBuffer *pStreamBuffer, SbgLogSatGroupData *pSatGroupData);

/*!
 * Write satellite group data to a stream buffer.
 *
 * \param[out]	pStreamBuffer				Stream buffer.
 * \param[in]	pSatGroupData				Satellite group data.
 * \return									SBG_NO_ERROR if successful.
 */
SbgErrorCode sbgEComBinaryLogWriteSatGroupData(SbgStreamBuffer *pStreamBuffer, const SbgLogSatGroupData *pSatGroupData);

/*!
 * Satellite group data constructor.
 *
 * \param[in]	pSatGroupData				Satellite group data.
 * \param[in]	nrSatellites				Number of satellites.
 * \param[in]	timeStamp					Time stamp, in us.
 * \return									SBG_NO_ERROR if successful.
 */
SbgErrorCode sbgLogSatGroupDataConstruct(SbgLogSatGroupData *pSatGroupData, size_t nrSatellites, uint32_t timeStamp);

/*!
 * Satellite group data destructor.
 *
 * \param[in]	pSatGroupData				Satellite group data.
 */
void sbgLogSatGroupDataDestroy(SbgLogSatGroupData *pSatGroupData);

/*!
 * Add satellite data to satellite group data.
 *
 * \param[in]	pSatGroupData				Satellite group data.
 * \param[in]	nrSignals					Number of signals.
 * \param[in]	id							Satellite ID.
 * \param[in]	elevation					Elevation, in degrees.
 * \param[in]	azimuth						Azimuth, in degrees.
 * \param[in]	constellationId				Constellation ID.
 * \param[in]	elevationStatus				Elevation status.
 * \param[in]	healthStatus				Health status.
 * \param[in]	trackingStatus				Tracking status.
 * \return									Satellite data, NULL if an error occurs.
 */
SbgLogSatData *sbgLogSatGroupDataAdd(SbgLogSatGroupData *pSatGroupData, size_t nrSignals, uint8_t id, int8_t elevation, uint16_t azimuth, SbgEComConstellationId constellationId, SbgEComSatElevationStatus elevationStatus, SbgEComSatHealthStatus healthStatus, SbgEComSatTrackingStatus trackingStatus);

/*!
 * Get satellite data from satellite group data.
 *
 * \param[in]	pSatGroupData				Satellite group data.
 * \param[in]	id							Satellite ID.
 * \return									Satellite data, NULL if not found.
 */
SbgLogSatData *sbgLogSatGroupGet(SbgLogSatGroupData *pSatGroupData, uint8_t id);

/*!
 * Get the constellation ID of satellite data.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Constellation ID.
 */
SbgEComConstellationId sbgLogSatDataGetConstellationId(const SbgLogSatData *pSatData);

/*!
 * Get the constellation ID of satellite data as a read only C string.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Constellation ID as a read only C string.
 */
const char *sbgLogSatDataGetConstellationIdAsStr(const SbgLogSatData *pSatData);

/*!
 * Get the elevation status of satellite data.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Elevation status.
 */
SbgEComSatElevationStatus sbgLogSatDataGetElevationStatus(const SbgLogSatData *pSatData);

/*!
 * Get the elevation status of satellite data as a read only C string.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Elevation status as a read only C string.
 */
const char *sbgLogSatDataGetElevationStatusAsStr(const SbgLogSatData *pSatData);

/*!
 * Get the health status of satellite data.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Health status.
 */
SbgEComSatHealthStatus sbgLogSatDataGetHealthStatus(const SbgLogSatData *pSatData);

/*!
 * Get the health status of satellite data as a read only C string.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Health status as a read only C string.
 */
const char *sbgLogSatDataGetHealthStatusAsStr(const SbgLogSatData *pSatData);

/*!
 * Get the tracking status of satellite data.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Tracking status.
 */
SbgEComSatTrackingStatus sbgLogSatDataGetTrackingStatus(const SbgLogSatData *pSatData);

/*!
 * Get the tracking status of satellite data as a read only C string.
 *
 * \param[in]	pSatData					Satellite data.
 * \return									Tracking status as a read only C string.
 */
const char *sbgLogSatDataGetTrackingStatusAsStr(const SbgLogSatData *pSatData);

/*!
 * Add signal data to satellite data.
 *
 * The health and tracking statuses of the satellite data are updated according to their respective
 * priority rules.
 *
 * \param[in]	pSatData					Satellite data.
 * \param[in]	id							Signal ID.
 * \param[in]	healthStatus				Health status.
 * \param[in]	trackingStatus				Tracking status.
 * \param[in]	snrValid					Set to true if the SNR value is valid.
 * \param[in]	snr							Signal-to-noise ratio, in dB.
 * \return									Signal data, NULL if an error occurs.
 */
SbgLogSatSignalData *sbgLogSatDataAdd(SbgLogSatData *pSatData, SbgEComSignalId id, SbgEComSatHealthStatus healthStatus, SbgEComSatTrackingStatus trackingStatus, bool snrValid, uint8_t snr);

/*!
 * Get signal data from satellite data.
 *
 * \param[in]	pSatData					Satellite data.
 * \param[in]	id							Signal ID.
 * \return									Signal data, NULL if not found.
 */
SbgLogSatSignalData *sbgLogSatDataGet(SbgLogSatData *pSatData, SbgEComSignalId id);

/*!
 * Get a signal id as a read only C string.
 * 
 * \param[in]	pSignalData					Signal data.
 * \return									Signal id as a read only C string.
 */
const char *sbgLogSatSignalDataGetSignalIdAsStr(const SbgLogSatSignalData *pSignalData);

/*! 
 * Returns true if the SNR value is valid.
 *
 * \param[in]	pSignalData					Signal data.
 * \return									true if the SNR value is valid.
 */
bool sbgLogSatSignalDataSnrIsValid(const SbgLogSatSignalData *pSignalData);

/*!
 * Get the health status of signal data.
 *
 * \param[in]	pSignalData					Signal data.
 * \return									Health status.
 */
SbgEComSatHealthStatus sbgLogSatSignalDataGetHealthStatus(const SbgLogSatSignalData *pSignalData);

/*!
 * Get the health status of signal data as a read only C string.
 *
 * \param[in]	pSignalData					Signal data.
 * \return									Health status as a read only C string.
 */
const char *sbgLogSatSignalDataGetHealthStatusAsStr(const SbgLogSatSignalData *pSignalData);

/*!
 * Get the tracking status of signal data.
 *
 * \param[in]	pSignalData					Signal data.
 * \return									Tracking status.
 */
SbgEComSatTrackingStatus sbgLogSatSignalDataGetTrackingStatus(const SbgLogSatSignalData *pSignalData);

/*!
 * Get the tracking status of signal data as a read only C string.
 *
 * \param[in]	pSignalData					Signal data.
 * \return									Tracking status as a read only C string.
 */
const char *sbgLogSatSignalDataGetTrackingStatusAsStr(const SbgLogSatSignalData *pSignalData);

#ifdef __cplusplus
}
#endif

#endif // SBG_ECOM_BINARY_LOG_SAT_H
