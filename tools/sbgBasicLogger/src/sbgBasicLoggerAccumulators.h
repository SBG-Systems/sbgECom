/*!
 * \file			sbgBasicLoggerAccumulators.h
 * \author			SBG Systems
 * \date			July 29, 2021
 *
 * \brief			Helper accumulators for IMU data.
 *
 * \copyright		Copyright (C) 2022, SBG Systems SAS. All rights reserved.
 * \beginlicense	Proprietary license
 * 
 * This source code is intended for use only by SBG Systems SAS and
 * those that have explicit written permission to use it from
 * SBG Systems SAS.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 * 
 * \endlicense
 */

#ifndef SBG_BASIC_LOGGER_ACCUMULATORS_H
#define SBG_BASIC_LOGGER_ACCUMULATORS_H

// sbgCommonLib headers
#include <sbgCommon.h>

// sbgECom headers
#include <sbgEComLib.h>

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * Define a set of IMU data
 */
typedef struct _SbgBasicLoggerImu
{
	uint32_t				timestamp;					/*!< The data time stamp in us */
	uint16_t				status;						/*!< IMU data status as defined in sbgECom logs */
	double					deltaAngle[3];				/*!< X, Y, Z delta angles values in rad.s^-1 */
	double					deltaVelocity[3];			/*!< X, Y, Z delta velocities values in m.s^-2 */
	double					temperature;				/*!< IMU temperature in °C. */
} SbgBasicLoggerImu;

/*!
 * Used to accumulate and decimate gyros / accels values.
 */
typedef struct _SbgBasicLoggerImuAcc
{
	size_t						 nrAcc;						/*!< Number of accumulated samples. */
	SbgBasicLoggerImu			 imuAccumulated;			/*!< Accumulated IMU data. */
} SbgBasicLoggerImuAcc;


/*!
 * Define a set of raw IMU data.
 */
typedef struct _SbgBasicLoggerImuRaw
{
	uint32_t	timestamp;						/*!< Time in us since the sensor power up. */
	uint16_t	status;							/*!< IMU status bitmask. */

	int64_t		rawAccelerometers[3];			/*!< X,Y,Z raw accelerometers signed data. No scale factor defined. */
	int64_t		rawGyroscopes[3];				/*!< X,Y,Z raw gyroscopes signed data. No scale factor defined. */
	int64_t		rawMagnetometers[3];			/*!< X,Y,Z raw magnetometers signed data. No scale factor defined. */

	int64_t		rawTempAccels[3];				/*!< X,Y,Z raw accelerometers temperature signed data. No scale factor defined. */
	int64_t		rawTempGyros[3];				/*!< X,Y,Z raw gyroscopes temperature signed data. No scale factor defined. */
	int64_t		rawTempMags[3];					/*!< X,Y,Z raw magnetometers temperature signed data. No scale factor defined. */

	int64_t		rawAuxValues[3];				/*!< Spare raw sensor values for specifics needs. */
	int64_t		rawTempAuxValues[3];			/*!< Spare raw sensor temperature values for specifics needs. */
} SbgBasicLoggerImuRaw;

/*!
 * Used to accumulate and decimate raw IMU values.
 */
typedef struct _SbgBasicLoggerRawImuAcc
{
	size_t						 nrAcc;			/*!< Number of accumulated samples. */
	SbgBasicLoggerImuRaw		 rawImuAcc;		/*!< Accumulated raw IMU data. */
} SbgBasicLoggerRawImuAcc;

//----------------------------------------------------------------------//
//- Private methods for SbgEComBasicLoggerAcc                          -//
//----------------------------------------------------------------------//

/*!
 * Zero initialize / reset an IMU accumulator instance.
 *
 * \param[in]	pAccumulator			Instance on the IMU accumulator to reset.
 */
void sbgBasicLoggerImuAccZeroInit(SbgBasicLoggerImuAcc *pAccumulator);

/*!
 * Accumulate a new set of IMU data
 *
 * \param[in]	pAccumulator			Instance on the IMU accumulator.
 * \param[in]	pNewImuData				New set of IMU data to accumulate
 */
void sbgBasicLoggerImuAccAdd(SbgBasicLoggerImuAcc *pAccumulator, const SbgBasicLoggerImu *pNewImuData);

/*!
 * Compute the decimated IMU values and reset the accumulator for next iteration.
 *
 * \param[in]	pAccumulator			Instance on the IMU accumulator.
 * \param[out]	pDecimatedImu			The decimated IMU values.
 */
void sbgBasicLoggerImuAccGet(SbgBasicLoggerImuAcc *pAccumulator, SbgBasicLoggerImu *pDecimatedImu);


#endif // SBG_BASIC_LOGGER_ACCUMULATORS_H
