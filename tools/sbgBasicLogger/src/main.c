/*!
 * \file			main.c
 * \author			SBG Systems
 * \date			July 29, 2021
 *
 * \brief			Tool to manage SBG logs in text format.
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

// sbgCommonLib headers
#include <sbgCommon.h>
#include <interfaces/sbgInterface.h>
#include <interfaces/sbgInterfaceSerial.h>
#include <interfaces/sbgInterfaceUdp.h>
#include <network/sbgNetwork.h>
#include <string/sbgString.h>

// sbgECom headers
#include <sbgECom.h>
#include <sbgEComGetVersion.h>
#include <binaryLogs/sbgEComBinaryLogImu.h>

// Argtable3 headers
#include <argtable3.h>

// Local headers
#include "sbgBasicLoggerHandler.h"

//----------------------------------------------------------------------//
//- Constant definitions                                               -//
//----------------------------------------------------------------------//

/*!
 * Program name.
 */
#define PROGRAM_NAME					"sbgBasicLogger"

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * User arguments.
 */
typedef struct _UserArgs
{
	bool				 writeHeader;						/*!< True to write data header. */
	bool				 writingFiles;						/*!< True to write data into files. */
	size_t				 fileDecimation;					/*!< File decimation. */
	bool				 consoleEnabled;					/*!< True to enable console. */
	size_t				 consoleDecimation;					/*!< Console decimation. */
	SbgString			 path;								/*!< Output path string to write log files into. */
	const char			*pPathStr;							/*!< Output path string shortcut. */
} UserArgs;

/*!
 * SbgBasicLoggerManager.
 */
typedef struct _SbgBasicLoggerManager
{
	UserArgs							 userArgs;			/*!< User arguments. */

	SbgEComHandle						 ecomHandle;		/*!< ECom handler. */
	SbgInterface						 ecomInterface;		/*!< SBG interface. */

	SbgLogUtcData						 lastUtcData;		/*!< Latest UTC time reference used to time stamp messages. */

	SbgBasicLoggerImuHandler			 imuShort;			/*!< IMU short logs handler. */
	SbgBasicLoggerImuHandler			 imu;				/*!< IMU logs handler. */
	SbgBasicLoggerImuHandler			 imuFast;			/*!< IMU fast logs handler. */
	SbgBasicLoggerStatusHandler			 status;			/*!< Status logs handler. */
	SbgBasicLoggerDiagHandler			 diag;				/*!< Diagnostic logs handler. */
	SbgBasicLoggerUtcHandler			 utc;				/*!< Diagnostic logs handler. */

	SbgBasicLoggerEkfEulerHandler		 ekfEuler;			/*!< EKF Euler logs handler. */
	SbgBasicLoggerEkfQuatHandler		 ekfQuat;			/*!< EKF quat logs handler. */
	SbgBasicLoggerEkfNavHandler			 ekfNav;			/*!< EKF nav logs handler. */

	SbgBasicLoggerShipMotionHandler		 shipMotion;		/*!< Ship motion logs handler. */
	SbgBasicLoggerShipMotionHpHandler	 shipMotionHp;		/*!< Ship motion HP logs handler. */

	SbgBasicLoggerGpsHdtHandler			 gps1Hdt;			/*!< GPS 1 hdt logs handler. */
	SbgBasicLoggerGpsPosHandler			 gps1Pos;			/*!< GPS 1 pos logs handler. */
	SbgBasicLoggerGpsVelHandler			 gps1Vel;			/*!< GPS 1 vel logs handler. */
	SbgBasicLoggerGpsRawHandler			 gps1Raw;			/*!< GPS 1 raw logs handler. */
	SbgBasicLoggerGpsSatHandler			 gps1Sat;			/*!< GPS 1 Satellite in View logs handler. */

	SbgBasicLoggerGpsHdtHandler			 gps2Hdt;			/*!< GPS 2 hdt logs handler. */
	SbgBasicLoggerGpsPosHandler			 gps2Pos;			/*!< GPS 2 pos logs handler. */
	SbgBasicLoggerGpsVelHandler			 gps2Vel;			/*!< GPS 2 vel logs handler. */
	SbgBasicLoggerGpsRawHandler			 gps2Raw;			/*!< GPS 2 raw logs handler. */
	SbgBasicLoggerGpsSatHandler			 gps2Sat;			/*!< GPS 2 Satellite in View logs handler. */

	SbgBasicLoggerOdometerHandler		 odometer;			/*!< Odometer logs handler. */
	SbgBasicLoggerDvlHandler			 dvlBottom;			/*!< DVL bottom logs handler. */
	SbgBasicLoggerDvlHandler			 dvlWater;			/*!< DVL water logs handler. */
	SbgBasicLoggerAirHandler			 air;				/*!< Air logs handler. */
	SbgBasicLoggerUsblHandler			 usbl;				/*!< USBL logs handler. */
	SbgBasicLoggerDepthHandler			 depth;				/*!< Depth logs handler. */
	SbgBasicLoggerRawRtcmHandler		 rawRtcm;			/*!< Raw RTCM logs handler. */

	SbgBasicLoggerEventHandler			 eventInA;			/*!< Event in A logs handler. */
	SbgBasicLoggerEventHandler			 eventInB;			/*!< Event in B logs handler. */
	SbgBasicLoggerEventHandler			 eventInC;			/*!< Event in C logs handler. */
	SbgBasicLoggerEventHandler			 eventInD;			/*!< Event in D logs handler. */
	SbgBasicLoggerEventHandler			 eventInE;			/*!< Event in E logs handler. */

	SbgBasicLoggerEventHandler			 eventOutA;			/*!< Event out A logs handler. */
	SbgBasicLoggerEventHandler			 eventOutB;			/*!< Event out B logs handler. */

	SbgBasicLoggerMagHandler			 mag;				/*!< Mag logs handler. */
} SbgBasicLoggerManager;

//----------------------------------------------------------------------//
//- Private variables                                                  -//
//----------------------------------------------------------------------//

/*!
 * SBG basic logger singleton.
 */
static SbgBasicLoggerManager					 gSbgBasicLogger;

//----------------------------------------------------------------------//
//- Private functions                                                  -//
//----------------------------------------------------------------------//

/*!
 * Callback definition used to route log error messages.
 *
 * \param[in]	pFileName				The file in which the log was triggered.
 * \param[in]	pFunctionName			The function where the log was triggered.
 * \param[in]	line					The line in the file where the log was triggered.
 * \param[in]	pCategory				Category for this log or "None" if no category has been specified.
 * \param[in]	type					Associated log message type.
 * \param[in]	errorCode				Associated error code or SBG_NO_ERROR for INFO & DEBUG level logs.
 * \param[in]	pMessage				The message to log.
 */
static void onLogCallback(const char *pFileName, const char *pFunctionName, uint32_t line, const char *pCategory, SbgDebugLogType logType, SbgErrorCode errorCode, const char *pMessage)
{
	assert(pFunctionName);
	assert(pCategory);
	assert(pMessage);

	SBG_UNUSED_PARAMETER(pFileName);
	SBG_UNUSED_PARAMETER(pCategory);

	switch (logType)
	{
	case SBG_DEBUG_LOG_TYPE_ERROR:
		fprintf(stderr, "*ERR * %10s %s(%"PRIu32"): %s\n\r", sbgErrorCodeToString(errorCode), pFunctionName, line, pMessage);
		break;
	case SBG_DEBUG_LOG_TYPE_WARNING:
		fprintf(stderr, "*WARN* %10s %s(%"PRIu32"): %s\n\r", sbgErrorCodeToString(errorCode), pFunctionName, line, pMessage);
		break;
	case SBG_DEBUG_LOG_TYPE_INFO:
		fprintf(stderr, "*INFO* %s(%"PRIu32"): %s\n\r", pFunctionName, line, pMessage);
		break;
	case SBG_DEBUG_LOG_TYPE_DEBUG:
		fprintf(stderr, "*DBG * %s(%"PRIu32") : % s\n\r", pFunctionName, line, pMessage);
		break;
	default:
		fprintf(stderr, "error: unexpected logType (0x%x)\n\r", logType);
		break;
	}
}

//----------------------------------------------------------------------//
//- SbgBasicLoggerManager methods                                      -//
//----------------------------------------------------------------------//

/*!
 * Process an UTC data.
 *
 * \param[in]	pManager				SBG basic logger.
 * \param[in]	pUtc					UTC data.
 */
static void sbgBasicLoggerManagerProcessUtc(SbgBasicLoggerManager *pManager, const SbgLogUtcData *pUtcData)
{
	assert(pManager);
	assert(pUtcData);

	pManager->lastUtcData = *pUtcData;
	sbgBasicLoggerUtcHandlerProcess(&pManager->utc, pUtcData);
}

/*!
 * Process a new IMU short data.
 *
 * \param[in]	pManager				SBG basic logger.
 * \param[in]	pImuShort				New IMU short data.
 */
static void sbgBasicLoggerManagerProcessImuShort(SbgBasicLoggerManager *pManager, const SbgLogImuShort *pImuShort)
{
	SbgBasicLoggerImu			 newData;

	assert(pManager);
	assert(pImuShort);

	newData.timestamp		= pImuShort->timeStamp;
	newData.status			= pImuShort->status;
	newData.temperature		= sbgLogImuShortGetTemperature(pImuShort);

	for (size_t i = 0; i < 3; i++)
	{
		newData.deltaAngle[i]		= sbgLogImuShortGetDeltaAngle(pImuShort, i);
		newData.deltaVelocity[i]	= sbgLogImuShortGetDeltaVelocity(pImuShort, i);
	}

	sbgBasicLoggerImuHandlerProcess(&pManager->imuShort, &newData);
}

/*!
 * Process a new IMU data.
 *
 * \param[in]	pManager				SBG basic logger.
 * \param[in]	pImu					New IMU data.
 */
static void sbgBasicLoggerManagerProcessImu(SbgBasicLoggerManager *pManager, const SbgLogImuData *pImu)
{
	SbgBasicLoggerImu			 newData;

	assert(pManager);
	assert(pImu);

	newData.timestamp		= pImu->timeStamp;
	newData.status			= pImu->status;
	newData.temperature		= pImu->temperature;

	for (size_t i = 0; i < 3; i++)
	{
		newData.deltaAngle[i]		= pImu->deltaAngle[i];
		newData.deltaVelocity[i]	= pImu->deltaVelocity[i];
	}

	sbgBasicLoggerImuHandlerProcess(&pManager->imu, &newData);
}

/*!
 * Process a new IMU fast data.
 *
 * \param[in]	pManager				SBG basic logger.
 * \param[in]	pImuShort				New IMU fast data.
 */
static void sbgBasicLoggerManagerProcessImuFast(SbgBasicLoggerManager *pManager, const SbgLogFastImuData *pFast)
{
	SbgBasicLoggerImu			 newData;

	assert(pManager);
	assert(pFast);

	newData.timestamp		= pFast->timeStamp;
	newData.status			= pFast->status;
	newData.temperature		= 0.0f;

	for (size_t i = 0; i < 3; i++)
	{
		newData.deltaAngle[i]		= pFast->gyroscopes[i];
		newData.deltaVelocity[i]	= pFast->accelerometers[i];
	}

	sbgBasicLoggerImuHandlerProcess(&pManager->imuFast, &newData);
}

/*!
 * Callback used to handle received logs.
 *
 * \param[in]	pECom					SbgECom instance.
 * \param[in]	msgClass				Class of the received message.
 * \param[in]	msg						Received message ID.
 * \param[in]	pLogData				Received data.
 * \param[in]	pUserArg				Optional user argument.
 * \return								SBG_NO_ERROR if successful.
 */
static SbgErrorCode onLogReceived(SbgEComHandle *pECom, SbgEComClass msgClass, SbgEComMsgId msg, const SbgBinaryLogData *pLogData, void *pUserArg)
{
	SbgBasicLoggerManager *pManager;

	assert(pLogData);
	assert(pUserArg);

	SBG_UNUSED_PARAMETER(pECom);

	pManager = (SbgBasicLoggerManager*)pUserArg;

	if (msgClass == SBG_ECOM_CLASS_LOG_ECOM_0)
	{
		switch (msg)
		{
		case SBG_ECOM_LOG_STATUS:
			sbgBasicLoggerStatusHandlerProcess(&pManager->status, &pLogData->statusData);
			break;

		case SBG_ECOM_LOG_UTC_TIME:
			sbgBasicLoggerManagerProcessUtc(pManager, &pLogData->utcData);
			break;

		case SBG_ECOM_LOG_IMU_DATA:
			sbgBasicLoggerManagerProcessImu(pManager, &pLogData->imuData);
			break;

		case SBG_ECOM_LOG_MAG:
			sbgBasicLoggerMagHandlerProcess(&pManager->mag, &pLogData->magData);
			break;

		case SBG_ECOM_LOG_MAG_CALIB:
			//
			// Known but not sbgBasicLoggerManagerProcessed.
			//
			break;

		case SBG_ECOM_LOG_EKF_EULER:
			sbgBasicLoggerEkfEulerHandlerProcess(&pManager->ekfEuler, &pLogData->ekfEulerData);
			break;

		case SBG_ECOM_LOG_EKF_QUAT:
			sbgBasicLoggerEkfQuatHandlerProcess(&pManager->ekfQuat, &pLogData->ekfQuatData);
			break;

		case SBG_ECOM_LOG_EKF_NAV:
			sbgBasicLoggerEkfNavHandlerProcess(&pManager->ekfNav, &pLogData->ekfNavData);
			break;

		case SBG_ECOM_LOG_SHIP_MOTION:
			sbgBasicLoggerShipMotionHandlerProcess(&pManager->shipMotion, &pLogData->shipMotionData);
			break;

		case SBG_ECOM_LOG_GPS1_VEL:
			sbgBasicLoggerGpsVelHandlerProcess(&pManager->gps1Vel, &pLogData->gpsVelData);
			break;

		case SBG_ECOM_LOG_GPS1_POS:
			sbgBasicLoggerGpsPosHandlerProcess(&pManager->gps1Pos, &pLogData->gpsPosData);
			break;

		case SBG_ECOM_LOG_GPS1_HDT:
			sbgBasicLoggerGpsHdtHandlerProcess(&pManager->gps1Hdt, &pLogData->gpsHdtData);
			break;

		case SBG_ECOM_LOG_GPS1_RAW:
			sbgBasicLoggerGpsRawHandlerProcess(&pManager->gps1Raw, &pLogData->gpsRawData);
			break;

		case SBG_ECOM_LOG_GPS1_SAT:
			sbgBasicLoggerGpsSatHandlerProcess(&pManager->gps1Sat, &pLogData->satGroupData);
			break;

		case SBG_ECOM_LOG_GPS2_VEL:
			sbgBasicLoggerGpsVelHandlerProcess(&pManager->gps2Vel, &pLogData->gpsVelData);
			break;

		case SBG_ECOM_LOG_GPS2_POS:
			sbgBasicLoggerGpsPosHandlerProcess(&pManager->gps2Pos, &pLogData->gpsPosData);
			break;

		case SBG_ECOM_LOG_GPS2_HDT:
			sbgBasicLoggerGpsHdtHandlerProcess(&pManager->gps2Hdt, &pLogData->gpsHdtData);
			break;

		case SBG_ECOM_LOG_GPS2_RAW:
			sbgBasicLoggerGpsRawHandlerProcess(&pManager->gps2Raw, &pLogData->gpsRawData);
			break;

		case SBG_ECOM_LOG_GPS2_SAT:
			sbgBasicLoggerGpsSatHandlerProcess(&pManager->gps2Sat, &pLogData->satGroupData);
			break;

		case SBG_ECOM_LOG_ODO_VEL:
			sbgBasicLoggerOdometerHandlerProcess(&pManager->odometer, &pLogData->odometerData);
			break;

		case SBG_ECOM_LOG_EVENT_A:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventInA, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_EVENT_B:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventInB, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_EVENT_C:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventInC, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_EVENT_D:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventInD, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_EVENT_E:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventInE, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_DVL_BOTTOM_TRACK:
			sbgBasicLoggerDvlHandlerProcess(&pManager->dvlBottom, &pLogData->dvlData);
			break;

		case SBG_ECOM_LOG_DVL_WATER_TRACK:
			sbgBasicLoggerDvlHandlerProcess(&pManager->dvlWater, &pLogData->dvlData);
			break;

		case SBG_ECOM_LOG_SHIP_MOTION_HP:
			sbgBasicLoggerShipMotionHpHandlerProcess(&pManager->shipMotionHp, &pLogData->shipMotionData);
			break;

		case SBG_ECOM_LOG_AIR_DATA:
			sbgBasicLoggerAirHandlerProcess(&pManager->air, &pLogData->airData);
			break;

		case SBG_ECOM_LOG_USBL:
			sbgBasicLoggerUsblHandlerProcess(&pManager->usbl, &pLogData->usblData);
			break;

		case SBG_ECOM_LOG_IMU_SHORT:
			sbgBasicLoggerManagerProcessImuShort(pUserArg, &pLogData->imuShort);
			break;

		case SBG_ECOM_LOG_EVENT_OUT_A:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventOutA, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_EVENT_OUT_B:
			sbgBasicLoggerEventHandlerProcess(&pManager->eventOutB, &pLogData->eventMarker);
			break;

		case SBG_ECOM_LOG_DEPTH:
			sbgBasicLoggerDepthHandlerProcess(&pManager->depth, &pLogData->depthData);
			break;

		case SBG_ECOM_LOG_DIAG:
			sbgBasicLoggerDiagHandlerProcess(&pManager->diag, &pManager->lastUtcData, &pLogData->diagData);
			break;

		case SBG_ECOM_LOG_RTCM_RAW:
			sbgBasicLoggerRawRtcmHandlerProcess(&pManager->rawRtcm, &pLogData->rtcmRawData);
			break;

		default:
			fprintf(stdout, "unkwown log: %u", msg);
			break;
		}
	}
	else if (msgClass == SBG_ECOM_CLASS_LOG_ECOM_1)
	{
		switch (msg)
		{
		case SBG_ECOM_LOG_FAST_IMU_DATA:
			sbgBasicLoggerManagerProcessImuFast(pUserArg, &pLogData->fastImuData);
			break;

		default:
			fprintf(stdout, "unkwown log1: %u", msg);
			break;
		}
	}
	else if (msgClass == SBG_ECOM_CLASS_LOG_NMEA_0)
	{
		//
		// Known but not sbgBasicLoggerManagerProcessed.
		//
	}
	else if (msgClass == SBG_ECOM_CLASS_LOG_NMEA_1)
	{
		//
		// Known but not sbgBasicLoggerManagerProcessed.
		//
	}
	else if (msgClass == SBG_ECOM_CLASS_LOG_THIRD_PARTY_0)
	{
		//
		// Known but not sbgBasicLoggerManagerProcessed.
		//
	}
	else if (msgClass == SBG_ECOM_CLASS_LOG_CMD_0)
	{
		//
		// Known but not sbgBasicLoggerManagerProcessed.
		//
	}
	else
	{
		fprintf(stdout, "unkwown class: %u", msgClass);
	}

	return SBG_NO_ERROR;
}

/*!
 * Construct all log handlers.
 *
 * \param[in]	pManager				SBG basic logger.
 */
static void sbgBasicLoggerConstructLogHandlers(SbgBasicLoggerManager *pManager)
{
	assert(pManager);

	sbgBasicLoggerImuShortHandlerConstruct(&pManager->imuShort,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.consoleDecimation,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.fileDecimation,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerImuHandlerConstruct(&pManager->imu,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.consoleDecimation,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.fileDecimation,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerImuFastHandlerConstruct(&pManager->imuFast,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.consoleDecimation,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.fileDecimation,
		pManager->userArgs.writeHeader);


	sbgBasicLoggerDiagHandlerConstruct(&pManager->diag,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr);

	sbgBasicLoggerUtcHandlerConstruct(&pManager->utc,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerStatusHandlerConstruct(&pManager->status,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEkfEulerHandlerConstruct(&pManager->ekfEuler,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEkfQuatHandlerConstruct(&pManager->ekfQuat,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEkfNavHandlerConstruct(&pManager->ekfNav,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerShipMotionHandlerConstruct(&pManager->shipMotion,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerShipMotionHpHandlerConstruct(&pManager->shipMotionHp,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGpsHdt1HandlerConstruct(&pManager->gps1Hdt,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGpsPos1HandlerConstruct(&pManager->gps1Pos,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGpsVel1HandlerConstruct(&pManager->gps1Vel,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGps1RawHandlerConstruct(&pManager->gps1Raw,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr);

	sbgBasicLoggerGps1SatHandlerConstruct(&pManager->gps1Sat,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr);

	sbgBasicLoggerGpsHdt2HandlerConstruct(&pManager->gps2Hdt,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGpsPos2HandlerConstruct(&pManager->gps2Pos,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGpsVel2HandlerConstruct(&pManager->gps2Vel,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerGps2RawHandlerConstruct(&pManager->gps2Raw,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr);

	sbgBasicLoggerGps2SatHandlerConstruct(&pManager->gps2Sat,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr);

	sbgBasicLoggerOdometerHandlerConstruct(&pManager->odometer,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerDvlBottomHandlerConstruct(&pManager->dvlBottom,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerDvlWaterHandlerConstruct(&pManager->dvlWater,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerAirHandlerConstruct(&pManager->air,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerUsblHandlerConstruct(&pManager->usbl,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerDepthHandlerConstruct(&pManager->depth,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerRawRtcmHandlerConstruct(&pManager->rawRtcm,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr);

	sbgBasicLoggerEventInAHandlerConstruct(&pManager->eventInA,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEventInBHandlerConstruct(&pManager->eventInB,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEventInCHandlerConstruct(&pManager->eventInC,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEventInDHandlerConstruct(&pManager->eventInD,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEventInEHandlerConstruct(&pManager->eventInE,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEventOutAHandlerConstruct(&pManager->eventOutA,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerEventOutBHandlerConstruct(&pManager->eventOutB,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);

	sbgBasicLoggerMagHandlerConstruct(&pManager->mag,
		pManager->userArgs.consoleEnabled,
		pManager->userArgs.writingFiles,
		pManager->userArgs.pPathStr,
		pManager->userArgs.writeHeader);
}

/*!
 * Receive logs.
 *
 * \param[in]	pManager					Sbg Basic logger.
 */
static SbgErrorCode sbgBasicLoggerManagerReceiveLogs(SbgBasicLoggerManager *pManager)
{
	SbgErrorCode						 errorCode;

	assert(pManager);

	errorCode = sbgEComInit(&pManager->ecomHandle, &pManager->ecomInterface);

	if (errorCode == SBG_NO_ERROR)
	{
		sbgBasicLoggerConstructLogHandlers(pManager);

		sbgEComSetReceiveLogCallback(&pManager->ecomHandle, onLogReceived, pManager);

		while (1)
		{
			errorCode = sbgEComHandle(&pManager->ecomHandle);

			if (errorCode == SBG_NOT_READY)
			{
				if (pManager->ecomInterface.type == SBG_IF_TYPE_FILE)
				{
					break;
				}

				//
				// Save CPU time.
				//
				sbgSleep(1);
			}
		}

		sbgEComClose(&pManager->ecomHandle);
	}

	return errorCode;
}

//----------------------------------------------------------------------//
// Public functions                                                     //
//----------------------------------------------------------------------//

/*!
 * Program entry point.
 *
 * \param[in]	argc					Number of input arguments.
 * \param[in]	argv					Input arguments as an array of strings.
 * \return								EXIT_SUCCESS if successful.
 */
int main(int argc, char **argv)
{
	SbgBasicLoggerManager						*pManager;
	int									 exitCode = EXIT_SUCCESS;
	bool								 printHelp = false;

	struct arg_lit						*pHelpArg;
	struct arg_lit						*pVersionArg;
	struct arg_str						*pUdpAddrArg;
	struct arg_int						*pUdpPortInArg;
	struct arg_int						*pUdpPortOutArg;
	struct arg_str						*pSerialDeviceArg;
	struct arg_int						*pSerialBaudrateArg;
	struct arg_file						*pInputFileArg;
	struct arg_lit						*pWriteLogs;
	struct arg_str						*pWriteLogsDirArg;
	struct arg_int						*pFileDecimationArg;
	struct arg_int						*pScreenDecimationArg;
	struct arg_lit						*pPrintLogs;
	struct arg_lit						*pLogsHeader;
	struct arg_end						*pEndArg;

	void								*argTable[] =
	{
		pHelpArg				= arg_lit0(		"h",	"help",												"display this help and exit"),
		pVersionArg				= arg_lit0(		"v",	"version",											"display sbgECom version and exit"),

		pUdpAddrArg				= arg_str0(		"a",	"addr-ip",					"IP address",			"open an UDP interface"),
		pUdpPortInArg			= arg_int0(		"I",	"udp-port-in",				"UDP port in",			"UDP port in"),
		pUdpPortOutArg			= arg_int0(		"O",	"udp-port-out",				"UDP port out",			"UDP port out"),

		pSerialDeviceArg		= arg_str0(		"s",	"serial-device",			"SERIAL_DEVICE",		"open a serial interface"),
		pSerialBaudrateArg		= arg_int0(		"r",	"serial-baudrate",			"SERIAL_BAUDRATE",		"serial baudrate"),

		pInputFileArg			= arg_file0(	"i",	"input-file",				"INPUT-FILE",			"input file"),

		pWriteLogs				= arg_lit0(		"w",	"write-logs",										"write logs in different files"),
		pWriteLogsDirArg		= arg_str0(		NULL,	"dir",						"DIRECTORY",			"directory to write logs into"),

		pFileDecimationArg		= arg_int0(		"d",	"file-decimation",			"FILE DECIMATION",		"file decimation"),
		pScreenDecimationArg	= arg_int0(		"c",	"console-decimation",		"CONSOLE DECIMATION",	"output stream decimation"),

		pPrintLogs				= arg_lit0(		"p",	"print-logs",										"print the logs on the output stream"),

		pLogsHeader				= arg_lit0(		"H",	"disable-header",									"disable header for files"),

		pEndArg					= arg_end(20),
	};

	memset(&gSbgBasicLogger, 0, sizeof(gSbgBasicLogger));

	pManager = &gSbgBasicLogger;

	sbgCommonLibSetLogCallback(onLogCallback);

	if (arg_nullcheck(argTable) == 0)
	{
		int								 argError;

		argError = arg_parse(argc, argv, argTable);

		if (pHelpArg->count != 0)
		{
			printf("Usage: %s", PROGRAM_NAME);
			arg_print_syntax(stdout, argTable, "\n\n");
			printf("Manage sbgECom logs in text format.\n\n");

			printf("Serial example: %s -s <SERIAL-PORT> -r <BAUDRATE> -p\n", PROGRAM_NAME);
			printf("   UDP example: %s -a <IP_ADDR> -I <UDP_PORT_IN> -O <UDP_PORT_OUT> -p\n", PROGRAM_NAME);
			printf("  File example: %s -i <BINARY_FILE> -p\n\n", PROGRAM_NAME);

			printf("Logs affected by decimation:\n\t- IMU short\n\t- IMU fast\n\t- IMU data\n\n");

			arg_print_glossary(stdout, argTable, "  %-50s %s\n");
		}
		else if (pVersionArg->count != 0)
		{
			printf("%s\n", sbgEComGetVersionAsString());
		}
		else if (argError == 0)
		{
			if (pLogsHeader->count != 0)
			{
				pManager->userArgs.writeHeader = false;
			}
			else
			{
				pManager->userArgs.writeHeader = true;
			}

			if (pWriteLogs->count != 0)
			{
				pManager->userArgs.writingFiles		= true;
				pManager->userArgs.fileDecimation	= 0;

				if (pWriteLogsDirArg->count != 0)
				{
					SbgErrorCode		 errorCode;

					errorCode = sbgStringConstructCString(&pManager->userArgs.path, pWriteLogsDirArg->sval[0]);

					if (errorCode == SBG_NO_ERROR)
					{
						if ((errorCode == SBG_NO_ERROR) && !sbgStringEndsWith(&pManager->userArgs.path, "/"))
						{
							sbgStringAppendCString(&pManager->userArgs.path, "/");
						}
					}
					else
					{
						fprintf(stderr, "Invalid path format.\n");
						exitCode	= EXIT_FAILURE;
						printHelp	= true;
					}
				}
				else
				{
					sbgStringConstructCString(&pManager->userArgs.path, "");
				}

				pManager->userArgs.pPathStr = sbgStringGetCString(&pManager->userArgs.path);
			}

			if (pFileDecimationArg->count != 0)
			{
				pManager->userArgs.fileDecimation = pFileDecimationArg->ival[0];
			}
			else
			{
				pManager->userArgs.fileDecimation = 1;
			}

			if (pPrintLogs->count != 0)
			{
				pManager->userArgs.consoleEnabled		= true;
				pManager->userArgs.consoleDecimation	= 0;
			}

			if (pScreenDecimationArg->count != 0)
			{
				pManager->userArgs.consoleDecimation = pScreenDecimationArg->ival[0];
			}
			else
			{
				pManager->userArgs.consoleDecimation = 1;
			}

			if ((pSerialDeviceArg->count != 0) && (pSerialBaudrateArg->count != 0) &&
					((pInputFileArg->count != 0) ||
					(pUdpAddrArg->count != 0) ||
					(pUdpPortInArg->count != 0) ||
					(pUdpPortOutArg->count != 0)))
			{
				fprintf(stderr, "Too many arguments.\n");
				exitCode	= EXIT_FAILURE;
				printHelp	= true;
			}

			if ((pInputFileArg->count != 0) &&
					((pSerialDeviceArg->count != 0) ||
					(pSerialBaudrateArg->count != 0) ||
					(pUdpAddrArg->count != 0) ||
					(pUdpPortInArg->count != 0) ||
					(pUdpPortOutArg->count != 0)))
			{
				fprintf(stderr, "Too many arguments.\n");
				exitCode	= EXIT_FAILURE;
				printHelp	= true;
			}

			if ((pUdpAddrArg->count != 0) && (pUdpPortInArg->count != 0) && (pUdpPortOutArg->count != 0) &&
					((pInputFileArg->count != 0) ||
					(pSerialDeviceArg->count != 0) ||
					(pSerialBaudrateArg->count != 0)))
			{
				fprintf(stderr, "Too many arguments.\n");
				exitCode	= EXIT_FAILURE;
				printHelp	= true;
			}

			if (!pManager->userArgs.consoleEnabled && !pManager->userArgs.writingFiles)
			{
				fprintf(stderr, "Specify if you want to print (-p) logs on console or write (-w) into files.\n");
				exitCode	= EXIT_FAILURE;
				printHelp	= true;
			}

			if (exitCode == EXIT_SUCCESS)
			{
				if ((pSerialDeviceArg->count != 0) && (pSerialBaudrateArg->count != 0))
				{
					SbgErrorCode		 errorCode;

					errorCode = sbgInterfaceSerialCreate(&pManager->ecomInterface, pSerialDeviceArg->sval[0], pSerialBaudrateArg->ival[0]);

					if (errorCode != SBG_NO_ERROR)
					{
						SBG_LOG_ERROR(errorCode, "unable to open serial interface");
						exitCode = EXIT_FAILURE;
					}
				}
				else if (pInputFileArg->count != 0)
				{
					SbgErrorCode		 errorCode;

					errorCode = sbgInterfaceFileOpen(&pManager->ecomInterface, pInputFileArg->filename[0]);

					if (errorCode != SBG_NO_ERROR)
					{
						SBG_LOG_ERROR(errorCode, "unable to open file interface");
						exitCode = EXIT_FAILURE;
					}
				}
				else if ((pUdpAddrArg->count != 0) && (pUdpPortInArg->count != 0) && (pUdpPortOutArg->count != 0))
				{
					SbgErrorCode		 errorCode;

					errorCode = sbgInterfaceUdpCreate(&pManager->ecomInterface, sbgNetworkIpFromString(pUdpAddrArg->sval[0]), pUdpPortOutArg->ival[0], pUdpPortInArg->ival[0]);

					if (errorCode != SBG_NO_ERROR)
					{
						SBG_LOG_ERROR(errorCode, "unable to open file interface");
						exitCode = EXIT_FAILURE;
					}
				}
				else
				{
					exitCode	= EXIT_FAILURE;
					printHelp	= true;
				}
			}

			if (exitCode == EXIT_SUCCESS)
			{
				SbgErrorCode			 errorCode;

				errorCode = sbgBasicLoggerManagerReceiveLogs(pManager);

				if (errorCode != SBG_NO_ERROR)
				{
					exitCode = EXIT_FAILURE;
				}

				sbgInterfaceDestroy(&pManager->ecomInterface);
			}
		}
		else
		{
			printHelp = true;
		}

		if (printHelp)
		{
			arg_print_errors(stderr, pEndArg, PROGRAM_NAME);
			fprintf(stderr, "Try '%s --help' for more information.\n", PROGRAM_NAME);
			exitCode = EXIT_FAILURE;
		}

		arg_freetable(argTable, SBG_ARRAY_SIZE(argTable));
	}
	else
	{
		SBG_LOG_ERROR(SBG_MALLOC_FAILED, "unable to allocate memory");
		exitCode = EXIT_FAILURE;
	}

	return exitCode;
}
