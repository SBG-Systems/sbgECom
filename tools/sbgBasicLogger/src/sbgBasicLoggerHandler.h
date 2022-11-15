/*!
 * \file			sbgBasicLoggerHandler.h
 * \author			SBG Systems
 * \date			July 30, 2021
 *
 * \brief			Helper to handle logs.
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

#ifndef SBG_BASIC_LOGGER_HANDLER_H
#define SBG_BASIC_LOGGER_HANDLER_H

// sbgCommonLib headers
#include <sbgCommon.h>

// sbgECom headers
#include <sbgEComLib.h>

// Local headers
#include "sbgBasicLoggerAccumulators.h"
#include "sbgBasicLoggerFile.h"

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * Common log handler.
 */
typedef struct _SbgBasicLoggerCommonHandler
{
	bool								 consoleEnabled;	/*!< True to print data on the console. */
	size_t								 consoleDecimation;	/*!< Console decimation. */
	size_t								 fileDecimation;	/*!< File decimation. */
	bool								 writingFile;		/*!< True to save data into a file. */
	bool								 writeHeader;		/*!< True to write header. */
	bool								 headerWritten;		/*!< True if headers have been written. */
	const SbgBasicLoggerFileDesc		*pDesc;				/*!< File descriptor. */
	FILE								*pFILE;				/*!< File. */

	const char							*pPathStr;			/*!< String path directory. */
	SbgEComClass						 class;				/*!< Handler class. */
	SbgEComMsgId						 id;				/*!< Handler message ID. */
} SbgBasicLoggerCommonHandler;

/*!
 * IMU short log handler.
 */
typedef struct _SbgBasicLoggerImuHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */

	SbgBasicLoggerImuAcc				 consoleAcc;		/*!< Decimated IMU data for the console. */
	SbgBasicLoggerImuAcc				 fileAcc;			/*!< Decimated IMU data for the file. */
} SbgBasicLoggerImuHandler;


/*!
 * Diag log handler.
 */
typedef struct _SbgBasicLoggerDiagHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerDiagHandler;

/*!
 * UTC log handler.
 */
typedef struct _SbgBasicLoggerUtcHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerUtcHandler;

/*!
 * Status log handler.
 */
typedef struct _SbgBasicLoggerStatusHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerStatusHandler;

/*!
 * EKF Euler log handler.
 */
typedef struct _SbgBasicLoggerEkfEulerHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerEkfEulerHandler;

/*!
 * EKF quat log handler.
 */
typedef struct _SbgBasicLoggerEkfQuatHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerEkfQuatHandler;

/*!
 * EKF nav log handler.
 */
typedef struct _SbgBasicLoggerEkfNavHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerEkfNavHandler;

/*!
 * Ship motion log handler.
 */
typedef struct _SbgBasicLoggerShipMotionHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerShipMotionHandler;

/*!
 * Ship motion HP log handler.
 */
typedef struct _SbgBasicLoggerShipMotionHpHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerShipMotionHpHandler;

/*!
 * GPS vel log handler.
 */
typedef struct _SbgBasicLoggerGpsVelHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerGpsVelHandler;

/*!
 * GPS pos log handler.
 */
typedef struct _SbgBasicLoggerGpsPosHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerGpsPosHandler;

/*!
 * GPS hdt log handler.
 */
typedef struct _SbgBasicLoggerGpsHdtHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerGpsHdtHandler;

/*!
 * GPS RAW log handler.
 */
typedef struct _SbgBasicLoggerGpsRawHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerGpsRawHandler;

/*!
 * GPS Satellite in View log handler.
 */
typedef struct _SbgBasicLoggerGpsSatHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerGpsSatHandler;

/*!
 * Odometer log handler.
 */
typedef struct _SbgBasicLoggerOdometerHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerOdometerHandler;

/*!
 * DVL log handler.
 */
typedef struct _SbgBasicLoggerDvlHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerDvlHandler;

/*!
 * Air log handler.
 */
typedef struct _SbgBasicLoggerAirHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerAirHandler;

/*!
 * USBL log handler.
 */
typedef struct _SbgBasicLoggerUsblHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerUsblHandler;

/*!
 * Depth log handler.
 */
typedef struct _SbgBasicLoggerDepthHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerDepthHandler;

/*!
 * RTCM log handler.
 */
typedef struct _SbgBasicLoggerRawRtcmHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerRawRtcmHandler;

/*!
 * Event log handler.
 */
typedef struct _SbgBasicLoggerEventHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerEventHandler;

/*!
 * Magnetometer log handler.
 */
typedef struct _SbgBasicLoggerMagHandler
{
	SbgBasicLoggerCommonHandler			 commonHandler;		/*!< Common with all handler logs. */
} SbgBasicLoggerMagHandler;

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

/*!
 * IMU short handler constructor.
 *
 * \param[in]	pHandler				IMU short handler.
 * \param[in]	consoleEnabled			True to print IMU short data on console.
 * \param[in]	consoleDecimation		Console decimation.
 * \param[in]	writingFile				True to write IMU short data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	fileDecimation			File decimation.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerImuShortHandlerConstruct(SbgBasicLoggerImuHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader);

/*!
 * IMU data handler constructor.
 *
 * \param[in]	pHandler				IMU data handler.
 * \param[in]	consoleEnabled			True to print IMU short data on console.
 * \param[in]	consoleDecimation		Console decimation.
 * \param[in]	writingFile				True to write IMU short data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	fileDecimation			File decimation.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerImuHandlerConstruct(SbgBasicLoggerImuHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader);

/*!
 * IMU fast handler constructor.
 *
 * \param[in]	pHandler				IMU fast data handler.
 * \param[in]	consoleEnabled			True to print IMU short data on console.
 * \param[in]	consoleDecimation		Console decimation.
 * \param[in]	writingFile				True to write IMU short data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	fileDecimation			File decimation.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerImuFastHandlerConstruct(SbgBasicLoggerImuHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader);

/*!
 * Process an IMU short handler with a new data.
 *
 * \param[in]	pHandler				IMU short handler.
 * \param[in]	pNewData				New IMU short data.
 */
void sbgBasicLoggerImuHandlerProcess(SbgBasicLoggerImuHandler *pHandler, const SbgBasicLoggerImu *pNewData);


/*!
 * Diagnostic handler constructor.
 *
 * \param[in]	pHandler				Diag handler.
 * \param[in]	consoleEnabled			True to print diag data on console.
 * \param[in]	writingFile				True to write diag data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 */
void sbgBasicLoggerDiagHandlerConstruct(SbgBasicLoggerDiagHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr);

/*!
 * Process a diagnostic data.
 *
 * \param[in]	pHandler				Diag log handler.
 * \param[in]	pLastUtcData			Last UTC data, used of timestamp.
 * \param[in]	pDiag					Diagnostic data.
 */
void sbgBasicLoggerDiagHandlerProcess(SbgBasicLoggerDiagHandler *pHandler, SbgLogUtcData *pLastUtcData, const SbgLogDiagData *pDiag);

/*!
 * UTC handler constructor.
 *
 * \param[in]	pHandler				UTC handler.
 * \param[in]	consoleEnabled			True to print UTC data on console.
 * \param[in]	writingFile				True to write UTC data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerUtcHandlerConstruct(SbgBasicLoggerUtcHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process an UTC data.
 *
 * \param[in]	pHandler				UTC log handler.
 * \param[in]	pLastUtcData			Last UTC data, used of timestamp.
 * \param[in]	pDiag					Diagnostic data.
 */
void sbgBasicLoggerUtcHandlerProcess(SbgBasicLoggerUtcHandler *pHandler, const SbgLogUtcData *pUtc);

/*!
 * Status handler constructor.
 *
 * \param[in]	pHandler				Status handler.
 * \param[in]	consoleEnabled			True to print status data on console.
 * \param[in]	writingFile				True to write status data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerStatusHandlerConstruct(SbgBasicLoggerStatusHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a status data.
 *
 * \param[in]	pHandler				Status log handler.
 * \param[in]	pDiag					Status data.
 */
void sbgBasicLoggerStatusHandlerProcess(SbgBasicLoggerStatusHandler *pHandler, const SbgLogStatusData *pStatus);

/*!
 * EKF Euler handler constructor.
 *
 * \param[in]	pHandler				EKF Euler handler.
 * \param[in]	consoleEnabled			True to print EKF Euler data on console.
 * \param[in]	writingFile				True to write EKF Euler data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEkfEulerHandlerConstruct(SbgBasicLoggerEkfEulerHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a EKF Euler data.
 *
 * \param[in]	pHandler				EKF Euler handler.
 * \param[in]	pEkfEuler				EKF Euler data.
 */
void sbgBasicLoggerEkfEulerHandlerProcess(SbgBasicLoggerEkfEulerHandler *pHandler, const SbgLogEkfEulerData *pEkfEuler);

/*!
 * EKF quat handler constructor.
 *
 * \param[in]	pHandler				EKF quat handler.
 * \param[in]	consoleEnabled			True to print EKF quat data on console.
 * \param[in]	writingFile				True to write EKF quat data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEkfQuatHandlerConstruct(SbgBasicLoggerEkfQuatHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a EKF quat data.
 *
 * \param[in]	pHandler				EKF quat handler.
 * \param[in]	pEkfQuat				EKF quat data.
 */
void sbgBasicLoggerEkfQuatHandlerProcess(SbgBasicLoggerEkfQuatHandler *pHandler, const SbgLogEkfQuatData *pEkfEuler);

/*!
 * EKF nav handler constructor.
 *
 * \param[in]	pHandler				EKF nav handler.
 * \param[in]	consoleEnabled			True to print EKF nav data on console.
 * \param[in]	writingFile				True to write EKF nav data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEkfNavHandlerConstruct(SbgBasicLoggerEkfNavHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a EKF nav data.
 *
 * \param[in]	pHandler				EKF nav handler.
 * \param[in]	pEkfEuler				EKF nav data.
 */
void sbgBasicLoggerEkfNavHandlerProcess(SbgBasicLoggerEkfNavHandler *pHandler, const SbgLogEkfNavData *pEkfEuler);

/*!
 * Ship motion handler constructor.
 *
 * \param[in]	pHandler				Ship motion handler.
 * \param[in]	consoleEnabled			True to print ship motion data on console.
 * \param[in]	writingFile				True to write ship motion data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerShipMotionHandlerConstruct(SbgBasicLoggerShipMotionHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a ship motion data.
 *
 * \param[in]	pHandler				SHip motion handler.
 * \param[in]	pShipMotion				Ship motion data.
 */
void sbgBasicLoggerShipMotionHandlerProcess(SbgBasicLoggerShipMotionHandler *pHandler, const SbgLogShipMotionData *pShipMotion);

/*!
 * Ship motion HP handler constructor.
 *
 * \param[in]	pHandler				Ship motion HP handler.
 * \param[in]	consoleEnabled			True to print ship motion HP data on console.
 * \param[in]	writingFile				True to write ship motion HP data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerShipMotionHpHandlerConstruct(SbgBasicLoggerShipMotionHpHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a ship motion HP data.
 *
 * \param[in]	pHandler				Ship motion HP handler.
 * \param[in]	pShipMotion				Ship motion HP data.
 */
void sbgBasicLoggerShipMotionHpHandlerProcess(SbgBasicLoggerShipMotionHpHandler *pHandler, const SbgLogShipMotionData *pShipMotion);

/*!
 * GPS 1 vel handler constructor.
 *
 * \param[in]	pHandler				GPS vel handler.
 * \param[in]	consoleEnabled			True to print GPS vel data on console.
 * \param[in]	writingFile				True to write GPS vel data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGpsVel1HandlerConstruct(SbgBasicLoggerGpsVelHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * GPS 2 vel handler constructor.
 *
 * \param[in]	pHandler				GPS vel handler.
 * \param[in]	consoleEnabled			True to print GPS vel data on console.
 * \param[in]	writingFile				True to write GPS vel data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGpsVel2HandlerConstruct(SbgBasicLoggerGpsVelHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a GPS vel data.
 *
 * \param[in]	pHandler				GPS vel handler.
 * \param[in]	pGps					GPS vel data.
 */
void sbgBasicLoggerGpsVelHandlerProcess(SbgBasicLoggerGpsVelHandler *pHandler, const SbgLogGpsVel *pGps);

/*!
 * GPS handler constructor.
 *
 * \param[in]	pHandler				GPS handler.
 * \param[in]	consoleEnabled			True to print GPS data on console.
 * \param[in]	writingFile				True to write GPS data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGpsPos1HandlerConstruct(SbgBasicLoggerGpsPosHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * GPS 2 handler constructor.
 *
 * \param[in]	pHandler				GPS handler.
 * \param[in]	consoleEnabled			True to print GPS data on console.
 * \param[in]	writingFile				True to write GPS data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGpsPos2HandlerConstruct(SbgBasicLoggerGpsPosHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a GPS data.
 *
 * \param[in]	pHandler				GPS handler.
 * \param[in]	pGps					GPS data.
 */
void sbgBasicLoggerGpsPosHandlerProcess(SbgBasicLoggerGpsPosHandler *pHandler, const SbgLogGpsPos *pGps);

/*!
 * GPS 1 hdt handler constructor.
 *
 * \param[in]	pHandler				GPS hdt handler.
 * \param[in]	consoleEnabled			True to print GPS vel data on console.
 * \param[in]	writingFile				True to write GPS vel data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGpsHdt1HandlerConstruct(SbgBasicLoggerGpsHdtHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * GPS 2 hdt handler constructor.
 *
 * \param[in]	pHandler				GPS vel handler.
 * \param[in]	consoleEnabled			True to print GPS hdt data on console.
 * \param[in]	writingFile				True to write GPS hdt data into a file.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGpsHdt2HandlerConstruct(SbgBasicLoggerGpsHdtHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a GPS hdt data.
 *
 * \param[in]	pHandler				GPS vel handler.
 * \param[in]	pGps					GPS vel data.
 */
void sbgBasicLoggerGpsHdtHandlerProcess(SbgBasicLoggerGpsHdtHandler *pHandler, const SbgLogGpsHdt *pGps);

/*!
 * GPS raw handler constructor.
 *
 * \param[in]	pHandler				GPS raw handler.
 * \param[in]	consoleEnabled			True to print GPS data on console.
 * \param[in]	writingFile				True to write GPS data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGps1RawHandlerConstruct(SbgBasicLoggerGpsRawHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr);

/*!
 * GPS raw handler constructor.
 *
 * \param[in]	pHandler				GPS raw handler.
 * \param[in]	consoleEnabled			True to print GPS data on console.
 * \param[in]	writingFile				True to write GPS data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGps2RawHandlerConstruct(SbgBasicLoggerGpsRawHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr);

/*!
 * Process a GPS raw data.
 *
 * \param[in]	pHandler				GPS raw handler.
 * \param[in]	pGps					GPS raw data.
 */
void sbgBasicLoggerGpsRawHandlerProcess(SbgBasicLoggerGpsRawHandler *pHandler, const SbgLogRawData *pRawGps);

/*!
 * GPS 1 Satellite in View handler constructor.
 *
 * \param[in]	pHandler				GPS Sat handler.
 * \param[in]	consoleEnabled			True to print GPS data on console.
 * \param[in]	writingFile				True to write GPS data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGps1SatHandlerConstruct(SbgBasicLoggerGpsSatHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr);

/*!
 * GPS 2 Satellite in View handler constructor.
 *
 * \param[in]	pHandler				GPS Sat handler.
 * \param[in]	consoleEnabled			True to print GPS data on console.
 * \param[in]	writingFile				True to write GPS data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerGps2SatHandlerConstruct(SbgBasicLoggerGpsSatHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr);

/*!
 * Process a GPS Satellite in View data.
 *
 * \param[in]	pHandler				GPS Sat handler.
 * \param[in]	pData					GPS Sat data.
 */
void sbgBasicLoggerGpsSatHandlerProcess(SbgBasicLoggerGpsSatHandler *pHandler, const SbgLogSatGroupData *pData);

/*!
 * Odometer handler constructor.
 *
 * \param[in]	pHandler				Odometer handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerOdometerHandlerConstruct(SbgBasicLoggerOdometerHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process an odometer data.
 *
 * \param[in]	pHandler				Odometer handler.
 * \param[in]	pData					Odometer data.
 */
void sbgBasicLoggerOdometerHandlerProcess(SbgBasicLoggerOdometerHandler *pHandler, const SbgLogOdometerData *pData);

/*!
 * DVL bottom handler constructor.
 *
 * \param[in]	pHandler				DVL bottom handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerDvlBottomHandlerConstruct(SbgBasicLoggerDvlHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * DVL water handler constructor.
 *
 * \param[in]	pHandler				DVL water handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerDvlWaterHandlerConstruct(SbgBasicLoggerDvlHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a DVL data.
 *
 * \param[in]	pHandler				DVL handler.
 * \param[in]	pData					DVL data.
 */
void sbgBasicLoggerDvlHandlerProcess(SbgBasicLoggerDvlHandler *pHandler, const SbgLogDvlData *pData);

/*!
 * Air handler constructor.
 *
 * \param[in]	pHandler				Air handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerAirHandlerConstruct(SbgBasicLoggerAirHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process an air data.
 *
 * \param[in]	pHandler				Air handler.
 * \param[in]	pData					Air data.
 */
void sbgBasicLoggerAirHandlerProcess(SbgBasicLoggerAirHandler *pHandler, const SbgLogAirData *pData);

/*!
 * USBL handler constructor.
 *
 * \param[in]	pHandler				USBL handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerUsblHandlerConstruct(SbgBasicLoggerUsblHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a USBL data.
 *
 * \param[in]	pHandler				USBL handler.
 * \param[in]	pData					USBL data.
 */
void sbgBasicLoggerUsblHandlerProcess(SbgBasicLoggerUsblHandler *pHandler, const SbgLogUsblData *pData);

/*!
 * Depth handler constructor.
 *
 * \param[in]	pHandler				Depth handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerDepthHandlerConstruct(SbgBasicLoggerDepthHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a depth data.
 *
 * \param[in]	pHandler				Depth handler.
 * \param[in]	pData					Depth data.
 */
void sbgBasicLoggerDepthHandlerProcess(SbgBasicLoggerDepthHandler *pHandler, const SbgLogDepth *pData);

/*!
 * Raw RTCM handler constructor.
 *
 * \param[in]	pHandler				Raw RTCM handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerRawRtcmHandlerConstruct(SbgBasicLoggerRawRtcmHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr);

/*!
 * Process a raw RTCM data.
 *
 * \param[in]	pHandler				Raw RTCM handler.
 * \param[in]	pData					Raw RTCM data.
 */
void sbgBasicLoggerRawRtcmHandlerProcess(SbgBasicLoggerRawRtcmHandler *pHandler, const SbgLogRawData *pData);


/*!
 * Event in A handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventInAHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Event in B handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventInBHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Event in C handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventInCHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Event in D handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventInDHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Event in E handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventInEHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Event out A handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventOutAHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Event out B handler constructor.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	consoleEnabled			True to print data on console.
 * \param[in]	writingFile				True to write data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerEventOutBHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process an event data.
 *
 * \param[in]	pHandler				Event handler.
 * \param[in]	pEvent					Event data.
 */
void sbgBasicLoggerEventHandlerProcess(SbgBasicLoggerEventHandler *pHandler, const SbgLogEvent *pEvent);

/*!
 * Mag handler constructor.
 *
 * \param[in]	pHandler				Mag handler.
 * \param[in]	consoleEnabled			True to print mag data on console.
 * \param[in]	writingFile				True to write mag data into a file.
 * \param[in]	pPathStr				Path to write file logs.
 * \param[in]	writeHeader				True to print and write header.
 */
void sbgBasicLoggerMagHandlerConstruct(SbgBasicLoggerMagHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader);

/*!
 * Process a mag data.
 *
 * \param[in]	pHandler				Mag handler.
 * \param[in]	pMag					Mag data.
 */
void sbgBasicLoggerMagHandlerProcess(SbgBasicLoggerMagHandler *pHandler, const SbgLogMag *pMag);

#endif // SBG_BASIC_LOGGER_HANDLER_H
