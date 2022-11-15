// sbgCommonLib headers
#include <sbgCommon.h>
#include <streamBuffer/sbgStreamBuffer.h>

// Project headers
#include <sbgEComIds.h>

// Local headers
#include "sbgEComBinaryLogAirData.h"
#include "sbgEComBinaryLogDepth.h"
#include "sbgEComBinaryLogDiag.h"
#include "sbgEComBinaryLogDvl.h"
#include "sbgEComBinaryLogEkf.h"
#include "sbgEComBinaryLogEvent.h"
#include "sbgEComBinaryLogGps.h"
#include "sbgEComBinaryLogImu.h"
#include "sbgEComBinaryLogMag.h"
#include "sbgEComBinaryLogOdometer.h"
#include "sbgEComBinaryLogRtcm.h"
#include "sbgEComBinaryLogShipMotion.h"
#include "sbgEComBinaryLogStatus.h"
#include "sbgEComBinaryLogUsbl.h"
#include "sbgEComBinaryLogUtc.h"
#include "sbgEComBinaryLogSat.h"
#include "sbgEComBinaryLogs.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SbgErrorCode sbgEComBinaryLogParse(SbgEComClass msgClass, SbgEComMsgId msg, const void *pPayload, size_t payloadSize, SbgBinaryLogData *pOutputData)
{
	SbgErrorCode		errorCode = SBG_NO_ERROR;
	SbgStreamBuffer		inputStream;

	assert(pPayload);
	assert(payloadSize > 0);
	assert(pOutputData);

	//
	// Create an input stream buffer that points to the frame payload so we can easily parse it's content
	//
	sbgStreamBufferInitForRead(&inputStream, pPayload, payloadSize);

	//
	// Handle the different classes of messages differently
	//
	if (msgClass == SBG_ECOM_CLASS_LOG_ECOM_0)
	{
		//
		// Parse the incoming log according to its type
		//
		switch (msg)
		{
		case SBG_ECOM_LOG_STATUS:
			errorCode = sbgEComBinaryLogParseStatusData(&inputStream, &pOutputData->statusData);
			break;
		case SBG_ECOM_LOG_IMU_DATA:
			errorCode = sbgEComBinaryLogParseImuData(&inputStream, &pOutputData->imuData);
			break;
		case SBG_ECOM_LOG_IMU_SHORT:
			errorCode = sbgEComBinaryLogParseImuShort(&inputStream, &pOutputData->imuShort);
			break;
		case SBG_ECOM_LOG_EKF_EULER:
			errorCode = sbgEComBinaryLogParseEkfEulerData(&inputStream, &pOutputData->ekfEulerData);
			break;
		case SBG_ECOM_LOG_EKF_QUAT:
			errorCode = sbgEComBinaryLogParseEkfQuatData(&inputStream, &pOutputData->ekfQuatData);
			break;
		case SBG_ECOM_LOG_EKF_NAV:
			errorCode = sbgEComBinaryLogParseEkfNavData(&inputStream, &pOutputData->ekfNavData);
			break;
		case SBG_ECOM_LOG_SHIP_MOTION:
		case SBG_ECOM_LOG_SHIP_MOTION_HP:
			errorCode = sbgEComBinaryLogParseShipMotionData(&inputStream, &pOutputData->shipMotionData);
			break;
		case SBG_ECOM_LOG_ODO_VEL:
			errorCode = sbgEComBinaryLogParseOdometerData(&inputStream, &pOutputData->odometerData);
			break;
		case SBG_ECOM_LOG_UTC_TIME:
			errorCode = sbgEComBinaryLogParseUtcData(&inputStream, &pOutputData->utcData);
			break;
		case SBG_ECOM_LOG_GPS1_VEL:
		case SBG_ECOM_LOG_GPS2_VEL:
			errorCode = sbgEComBinaryLogParseGpsVelData(&inputStream, &pOutputData->gpsVelData);
			break;
		case SBG_ECOM_LOG_GPS1_POS:
		case SBG_ECOM_LOG_GPS2_POS:
			errorCode = sbgEComBinaryLogParseGpsPosData(&inputStream, &pOutputData->gpsPosData);
			break;
		case SBG_ECOM_LOG_GPS1_HDT:
		case SBG_ECOM_LOG_GPS2_HDT:
			errorCode = sbgEComBinaryLogParseGpsHdtData(&inputStream, &pOutputData->gpsHdtData);
			break;
		case SBG_ECOM_LOG_GPS1_RAW:
		case SBG_ECOM_LOG_GPS2_RAW:
			errorCode = sbgEComBinaryLogParseGpsRawData(&inputStream, &pOutputData->gpsRawData);
			break;
		case SBG_ECOM_LOG_GPS1_SAT:
		case SBG_ECOM_LOG_GPS2_SAT:
			errorCode = sbgEComBinaryLogParseSatGroupData(&inputStream, &pOutputData->satGroupData);
			break;
		case SBG_ECOM_LOG_RTCM_RAW:
			errorCode = sbgEComBinaryLogParseRtcmRawData(&inputStream, &pOutputData->rtcmRawData);
			break;
		case SBG_ECOM_LOG_MAG:
			errorCode = sbgEComBinaryLogParseMagData(&inputStream, &pOutputData->magData);
			break;
		case SBG_ECOM_LOG_MAG_CALIB:
			errorCode = sbgEComBinaryLogParseMagCalibData(&inputStream, &pOutputData->magCalibData);
			break;
		case SBG_ECOM_LOG_DVL_BOTTOM_TRACK:
			errorCode = sbgEComBinaryLogParseDvlData(&inputStream, &pOutputData->dvlData);
			break;
		case SBG_ECOM_LOG_DVL_WATER_TRACK:
			errorCode = sbgEComBinaryLogParseDvlData(&inputStream, &pOutputData->dvlData);
			break;
		case SBG_ECOM_LOG_AIR_DATA:
			errorCode = sbgEComBinaryLogParseAirData(&inputStream, &pOutputData->airData);
			break;
		case SBG_ECOM_LOG_USBL:
			errorCode = sbgEComBinaryLogParseUsblData(&inputStream, &pOutputData->usblData);
			break;
		case SBG_ECOM_LOG_DEPTH:
			errorCode = sbgEComBinaryLogParseDepth(&inputStream, &pOutputData->depthData);
			break;
		case SBG_ECOM_LOG_EVENT_A:
		case SBG_ECOM_LOG_EVENT_B:
		case SBG_ECOM_LOG_EVENT_C:
		case SBG_ECOM_LOG_EVENT_D:
		case SBG_ECOM_LOG_EVENT_E:
		case SBG_ECOM_LOG_EVENT_OUT_A:
		case SBG_ECOM_LOG_EVENT_OUT_B:
			errorCode = sbgEComBinaryLogParseEvent(&inputStream, &pOutputData->eventMarker);
			break;
		case SBG_ECOM_LOG_DIAG:
			errorCode = sbgEComBinaryLogParseDiagData(&inputStream, &pOutputData->diagData);
			break;

		default:
			errorCode = SBG_ERROR;
		}
	}
	else if (msgClass == SBG_ECOM_CLASS_LOG_ECOM_1)
	{
		//
		// Parse the message depending on the message ID
		//
		switch ((SbgEComLog1)msg)
		{
		case SBG_ECOM_LOG_FAST_IMU_DATA:
			errorCode = sbgEComBinaryLogParseFastImuData(&inputStream, &pOutputData->fastImuData);
			break;
		default:
			errorCode = SBG_ERROR;
		}
	}
	else
	{
		//
		// Un-handled message class
		//
		errorCode = SBG_ERROR;
	}

	return errorCode;
}

void sbgEComBinaryLogCleanup(SbgBinaryLogData *pLogData, SbgEComClass msgClass, SbgEComMsgId msgId)
{
	assert(pLogData);

	if (msgClass == SBG_ECOM_CLASS_LOG_ECOM_0)
	{
		switch (msgId)
		{
		case SBG_ECOM_LOG_GPS1_SAT:
		case SBG_ECOM_LOG_GPS2_SAT:
			sbgLogSatGroupDataDestroy(&pLogData->satGroupData);
			break;
		}
	}
}
