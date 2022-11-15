// sbgCommonLib headers
#include <sbgCommon.h>
#include <string/sbgString.h>

// sbgECom headers
#include <sbgEComLib.h>

// Local headers
#include "sbgBasicLoggerAccumulators.h"
#include "sbgBasicLoggerHandler.h"

//----------------------------------------------------------------------//
//- Private functions                                                  -//
//----------------------------------------------------------------------//

/*!
 * Write IMU short data into an output stream.
 *
 * \param[in]	pImuShort				IMU short data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteImu(const SbgBasicLoggerImu *pImuShort, void *pStream)
{
	assert(pImuShort);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu32"\t%0.12lf\t%0.12lf\t%0.12lf\t%0.12lf\t%0.12lf\t%0.12lf\t%0.3lf\n",
		pImuShort->timestamp, pImuShort->status,
		pImuShort->deltaVelocity[0], pImuShort->deltaVelocity[1], pImuShort->deltaVelocity[2],
		sbgRadToDegd(pImuShort->deltaAngle[0]), sbgRadToDegd(pImuShort->deltaAngle[1]), sbgRadToDegd(pImuShort->deltaAngle[2]),
		pImuShort->temperature);

	fflush(pStream);
}

/*!
 * Write IMU data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteImuHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\tdeltaVelX\tdeltaVelY\tdeltaVelZ\tdeltaAngleX\tdeltaAngleY\tdeltaAngleZ\ttemperature\n");
	fprintf(pStream, "(us)\t(N/A)\t(m.s^-2)\t(m.s^-2)\t(m.s^-2)\t(deg.s^-1)\t(deg.s^-1)\t(deg.s^-1)\t(°C)\n");
	fflush(pStream);
}


/*!
 * Write a diag data into an output stream.
 *
 * \param[in]	pDiag					Diag data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteDiag(const SbgLogDiagData *pDiag, const char *pUtcTimeStr, void *pStream)
{
	assert(pDiag);
	assert(pUtcTimeStr);
	assert(pStream);

	switch (pDiag->type)
	{
	case SBG_DEBUG_LOG_TYPE_ERROR:
		fprintf(pStream, "%s *ERR * [%s]: %s\n", pUtcTimeStr, sbgErrorCodeToString(pDiag->errorCode), pDiag->string);
		break;
	case SBG_DEBUG_LOG_TYPE_WARNING:
		fprintf(pStream, "%s *WARN* [%s]: %s\n", pUtcTimeStr, sbgErrorCodeToString(pDiag->errorCode), pDiag->string);
		break;
	case SBG_DEBUG_LOG_TYPE_INFO:
		fprintf(pStream, "%s *INFO* %s\n", pUtcTimeStr, pDiag->string);
		break;
	case SBG_DEBUG_LOG_TYPE_DEBUG:
		fprintf(pStream, "%s *DBG * %s\n", pUtcTimeStr, pDiag->string);
		break;
	default:
		fprintf(pStream, "%s *UKN * [%s]: %s\n", pUtcTimeStr, sbgErrorCodeToString(pDiag->errorCode), pDiag->string);
	}

	fflush(pStream);
}

/*!
 * Write an UTC data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteUtcHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\tgpsTimeOfWeek\tyear\tmonth\tday\thour\tminute\tsecond\tnanosecond\n");
	fprintf(pStream, "(us)\t(NA)\t(ms)\t(yyyy)\t(mm)\t(dd)\t(hh)\t(mm)\t(ss)\t(ns)\n");
	fflush(pStream);
}

/*!
 * Write an UTC data into an output stream.
 *
 * \param[in]	pUtc					UTC data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteUtc(const SbgLogUtcData *pUtc, void *pStream)
{
	assert(pUtc);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%"PRIu32"\t%"PRIu16"\t%0.2"PRId8"\t%0.2"PRId8"\t%0.2"PRId8"\t%0.2"PRId8"\t%0.2"PRId8"\t%"PRId32"\n",
		pUtc->timeStamp,
		pUtc->status,
		pUtc->gpsTimeOfWeek,
		pUtc->year,
		pUtc->month,
		pUtc->day,
		pUtc->hour,
		pUtc->minute,
		pUtc->second,
		pUtc->nanoSecond
		);
	fflush(pStream);
}

/*!
 * Write a status data into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteStatusHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tgeneral\tcom2\tcom\taiding\n");
	fprintf(pStream, "(us)\t(NA)\t(NA)\t(NA)\t(NA)\n");
	fflush(pStream);
}

/*!
 * Write a status data into an output stream.
 *
 * \param[in]	pStatus					Status data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteStatus(const SbgLogStatusData *pStatus, void *pStream)
{
	assert(pStatus);
	assert(pStream);

	fprintf(pStream, "%" PRIu32 "\t%" PRIu16 "\t%" PRIu16 "\t%" PRIu32 "\t%" PRIu32 "\n",
		pStatus->timeStamp,
		pStatus->generalStatus,
		pStatus->comStatus2,
		pStatus->comStatus,
		pStatus->aidingStatus
		);

	fflush(pStream);
}

/*!
 * Write an EKF Euler data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEkfEulerHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\troll\tpitch\tyaw\trollStd\tpitchStd\tyawStd\n");
	fprintf(pStream, "(us)\t(NA)\t(deg)\t(deg)\t(deg)\t(deg)\t(deg)\t(deg)\n");
	fflush(pStream);
}

/*!
 * Write an EKF Euler data into an output stream.
 *
 * \param[in]	pData					EKF Euler data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEkfEuler(const SbgLogEkfEulerData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu32"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		sbgRadToDegf(pData->euler[0]),
		sbgRadToDegf(pData->euler[1]),
		sbgRadToDegf(pData->euler[2]),
		sbgRadToDegf(pData->eulerStdDev[0]),
		sbgRadToDegf(pData->eulerStdDev[1]),
		sbgRadToDegf(pData->eulerStdDev[2])
		);
	fflush(pStream);
}

/*!
 * Write an EKF Euler data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEkfQuatHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\tW\tX\tY\tZ\trollStd\tpitchStd\tyawStd\n");
	fprintf(pStream, "(us)\t(NA)\t(NA)\t(NA)\t(NA)\t(NA)\t(deg)\t(deg)\t(deg)\n");
	fflush(pStream);
}

/*!
 * Write an EKF quat data into an output stream.
 *
 * \param[in]	pData					EKF quat data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEkfQuat(const SbgLogEkfQuatData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu32"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->quaternion[0],
		pData->quaternion[1],
		pData->quaternion[2],
		pData->quaternion[3],
		sbgRadToDegf(pData->eulerStdDev[0]),
		sbgRadToDegf(pData->eulerStdDev[1]),
		sbgRadToDegf(pData->eulerStdDev[2])
		);
	fflush(pStream);
}

/*!
 * Write an EKF nav data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEkfNavHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\tvelN\tvelE\tvelD\tvelStdN\tvelStdE\tvelStdD\tlatitude\tlongitude\taltitude\tlatitudeStd\tlongitudeStd\taltitudeStd\tundulation\n");
	fprintf(pStream, "(us)\t(NA)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(deg)\t(deg)\t(m)\t(m)\t(m)\t(m)\t(m)\n");
	fflush(pStream);
}

/*!
 * Write an EKF nav data into an output stream.
 *
 * \param[in]	pData					EKF nav data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEkfNav(const SbgLogEkfNavData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu32"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->velocity[0],
		pData->velocity[1],
		pData->velocity[2],
		pData->velocityStdDev[0],
		pData->velocityStdDev[1],
		pData->velocityStdDev[2],
		pData->position[0],
		pData->position[1],
		pData->position[2],
		pData->positionStdDev[0],
		pData->positionStdDev[1],
		pData->positionStdDev[2],
		pData->undulation
		);
	fflush(pStream);
}

/*!
 * Write a ship motion data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteShipMotionHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\tsurge\tsway\theave\tsurgeVel\tswayVel\theaveVel\tsurgeAccel\tswayAccel\theaveAccel\theavePeriod\n");
	fprintf(pStream, "(us)\t(NA)\t(m)\t(m)\t(m)\t(m)\t(m)\t(m)\t(m)\t(m)\t(m)\t(s)\n");
	fflush(pStream);
}

/*!
 * Write a ship motion data into an output stream.
 *
 * \param[in]	pData					Ship motion data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteShipMotion(const SbgLogShipMotionData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
		pData->timeStamp,
		pData->status,
		pData->shipMotion[0],
		pData->shipMotion[1],
		pData->shipMotion[2],
		pData->shipVel[0],
		pData->shipVel[1],
		pData->shipVel[2],
		pData->shipAccel[0],
		pData->shipAccel[1],
		pData->shipAccel[2],
		pData->mainHeavePeriod
		);
	fflush(pStream);
}

/*!
 * Write a GPS vel data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsVelHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\ttimeOfWeek\tvelN\tvelE\tvelD\tvelNStd\tvelEStd\tvelDStd\tcourse\tcourseStd\n");
	fprintf(pStream, "(us)\t(NA)\t(ms)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(degrees)\t(degrees)\n");
	fflush(pStream);
}

/*!
 * Write a GPS vel data into an output stream.
 *
 * \param[in]	pData					GPS vel data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsVel(const SbgLogGpsVel *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu32"\t%"PRIu32"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->timeOfWeek,
		pData->velocity[0],
		pData->velocity[1],
		pData->velocity[2],
		pData->velocityAcc[0],
		pData->velocityAcc[1],
		pData->velocityAcc[2],
		pData->course,
		pData->courseAcc
		);
	fflush(pStream);
}

/*!
 * Write a GPS data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\ttimeOfWeek\tstatus\taltitude\n");
	fprintf(pStream, "(us)\t(ms)\t(NA)\t(m)\n");
	fflush(pStream);
}

/*!
 * Write a GPS data into an output stream.
 *
 * \param[in]	pData					GPS data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGps(const SbgLogGpsPos *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu32"\t%"PRIu32"\t%lf\n",
		pData->timeStamp,
		pData->timeOfWeek,
		pData->status,
		pData->altitude);
	fflush(pStream);
}

/*!
 * Write a GPS hdt data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsHdtHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\ttimeOfWeek\theading\theadingStd\tpitch\tpitchStd\tbaseline\n");
	fprintf(pStream, "(us)\t(NA)\t(ms)\t(degrees)\t(degrees)\t(degrees)\t(degrees)\t(m)\n");
	fflush(pStream);
}

/*!
 * Write a GPS hdt data into an output stream.
 *
 * \param[in]	pData					GPS hdt data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsHdt(const SbgLogGpsHdt *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%"PRIu32"\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->timeOfWeek,
		pData->heading,
		pData->headingAccuracy,
		pData->pitch,
		pData->pitchAccuracy,
		pData->baseline
		);
	fflush(pStream);
}

/*!
 * Write a GPS Satellite data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsSatHeader(void *pStream)
{
	assert(pStream);
}

/*!
 * Write a GPS Satellite data into an output stream.
 *
 * \param[in]	pData					GPS satellite data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsSat(const SbgLogSatGroupData *pData, void *pStream)
{
	size_t	satelliteIdx;
	size_t	signalIdx;

	assert(pData);
	assert(pStream);

	fprintf(pStream, "%10"PRIu32" us | Satellites: %3zu ====================================================================\n",
		pData->timeStamp,
		pData->nrSatellites);

	for (satelliteIdx = 0; satelliteIdx < pData->nrSatellites; satelliteIdx++)
	{
		const SbgLogSatData			*pSatEntry = &pData->pSatData[satelliteIdx];
		SbgEComSatElevationStatus	 elevationStatus;
		char						 elevationStatusChr;

		elevationStatus = sbgLogSatDataGetElevationStatus(pSatEntry);

		if (elevationStatus == SBG_ECOM_SAT_ELEVATION_STATUS_SETTING)
		{
			elevationStatusChr = 's';
		}
		else if (elevationStatus == SBG_ECOM_SAT_ELEVATION_STATUS_RISING)
		{
			elevationStatusChr = 'r';
		}
		else
		{
			elevationStatusChr = '*';
		}

		fprintf(pStream, "            [ID: %2"PRIu8" %-8s]  Elevation: %3"PRIi8"%c  Azimuth: %3"PRIi16"  nrSignals: %2zu  %-10s%-10s\n",
			pSatEntry->id,
			sbgLogSatDataGetConstellationIdAsStr(pSatEntry),
			pSatEntry->elevation,
			elevationStatusChr,
			pSatEntry->azimuth,
			pSatEntry->nrSignals,
			sbgLogSatDataGetHealthStatusAsStr(pSatEntry),
			sbgLogSatDataGetTrackingStatusAsStr(pSatEntry));

		for (signalIdx = 0; signalIdx < pSatEntry->nrSignals; signalIdx++)
		{
			const SbgLogSatSignalData	*pSignalEntry = &pSatEntry->pSignalData[signalIdx];
			char						 snrStr[16];

			if (sbgLogSatSignalDataSnrIsValid(pSignalEntry))
			{
				sprintf(snrStr, "%-3"PRIu8, pSignalEntry->snr);
			}
			else
			{
				strcpy(snrStr, "  -");
			}
			
			fprintf(pStream, "                %-14s    SNR: %3s    %-10s    %-10s\n",
				sbgLogSatSignalDataGetSignalIdAsStr(pSignalEntry),
				snrStr,
				sbgLogSatSignalDataGetHealthStatusAsStr(pSignalEntry),
				sbgLogSatSignalDataGetTrackingStatusAsStr(pSignalEntry));
		}
	}

	fflush(pStream);
}

/*!
 * Write an odometer data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteOdometerHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\tvelocity\n");
	fprintf(pStream, "(us)\t(NA)\t(m.s-1)\n");
	fflush(pStream);
}

/*!
 * Write an odometer data into an output stream.
 *
 * \param[in]	pData					Odometer data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteOdometer(const SbgLogOdometerData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->velocity
		);
	fflush(pStream);
}

/*!
 * Write a DVL data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteDvlHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\tvelX\tvelY\tvelZ\tvelQualityX\tvelQualityY\tvelQualityZ\n");
	fprintf(pStream, "(us)\t(NA)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\t(m.s-1)\n");
	fflush(pStream);
}

/*!
 * Write an DVL data into an output stream.
 *
 * \param[in]	pData					DVL data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteDvl(const SbgLogDvlData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->velocity[0],
		pData->velocity[1],
		pData->velocity[2],
		pData->velocityQuality[0],
		pData->velocityQuality[1],
		pData->velocityQuality[2]
		);
	fflush(pStream);
}

/*!
 * Write an air data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteAirHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\tpressure\taltitude\tpressureDiff\tairSpeed\tairTemp\n");
	fprintf(pStream, "(us)\t(NA)\t(pascal)\t(meters)\t(pascal)\t(m.s-1)\t(celcius degrees)\n");
	fflush(pStream);
}

/*!
 * Write an air data into an output stream.
 *
 * \param[in]	pData					Air data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteAir(const SbgLogAirData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->pressureAbs,
		pData->altitude,
		pData->pressureDiff,
		pData->trueAirspeed,
		pData->airTemperature
		);
	fflush(pStream);
}

/*!
 * Write an USBL data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteUsblHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\tlatitude\tlongitude\tdepth\tlatitudeStd\tlongitudeStd\tdepthStd\n");
	fprintf(pStream, "(us)\t(NA)\t(degrees)\t(degrees)\t(meters)\t(meters)\t(meters)\t(meters)\n");
	fflush(pStream);
}

/*!
 * Write an USBL data into an output stream.
 *
 * \param[in]	pData					Air data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteUsbl(const SbgLogUsblData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->latitude,
		pData->longitude,
		pData->depth,
		pData->latitudeAccuracy,
		pData->longitudeAccuracy,
		pData->depthAccuracy
		);
	fflush(pStream);
}

/*!
 * Write a depth data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteDepthHeader(void *pStream)
{
	assert(pStream);
	fprintf(pStream, "timestamp\tstatus\tpressure\tdepth\n");
	fprintf(pStream, "(us)\t(NA)\t(pascal)\t(meters)\n");
	fflush(pStream);
}

/*!
 * Write an depth data into an output stream.
 *
 * \param[in]	pData					Depth data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteDepth(const SbgLogDepth *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%lf\t%lf\n",
		pData->timeStamp,
		pData->status,
		pData->pressureAbs,
		pData->altitude
		);
	fflush(pStream);
}

/*!
 * Write a GPS raw data into an output stream.
 *
 * \param[in]	pData					GPS raw data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteRawRtcm(const SbgLogRawData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%zu\n",
		pData->bufferSize);
	fflush(pStream);
}

/*!
 * Write a GPS raw data into an output stream.
 *
 * \param[in]	pData					GPS raw data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteGpsRaw(const SbgLogRawData *pData, void *pStream)
{
	assert(pData);
	assert(pStream);

	fprintf(pStream, "%zu\n",
		pData->bufferSize);
	fflush(pStream);
}

/*!
 * Write event data header into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEventHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\ttimeOffset0\ttimeOffset1\ttimeOffset2\ttimeOffset3\n");
	fflush(pStream);
}

/*!
 * Write event data into an output stream.
 *
 * \param[in]	pEvent					Event data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteEvent(const SbgLogEvent *pEvent, void *pStream)
{
	assert(pEvent);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%"PRIu16"\t%"PRIu16"\t%"PRIu16"\t%"PRIu16"\n",
			pEvent->timeStamp,
			pEvent->status,
			pEvent->timeOffset0,
			pEvent->timeOffset1,
			pEvent->timeOffset2,
			pEvent->timeOffset3);
	fflush(pStream);
}

/*!
 * Write mag data into an output stream.
 *
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteMagHeader(void *pStream)
{
	assert(pStream);

	fprintf(pStream, "timestamp\tstatus\tmagX\tmagY\tmagZ\taccelX\taccelY\taccelZ\n");
	fprintf(pStream, "(us)\t(NA)\t(A.U)\t(A.U)\t(A.U)\t(m.s^-2)\t(m.s^-2)\t(m.s^-2)\n");
	fflush(pStream);
}

/*!
 * Write mag data into an output stream.
 *
 * \param[in]	pMag					Mag data.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerHandlerWriteMag(const SbgLogMag *pMag, void *pStream)
{
	assert(pMag);
	assert(pStream);

	fprintf(pStream, "%"PRIu32"\t%"PRIu16"\t%f\t%f\t%f\t%f\t%f\t%f\n",
		pMag->timeStamp,
		pMag->status,
		pMag->magnetometers[0], pMag->magnetometers[1], pMag->magnetometers[2],
		pMag->accelerometers[0], pMag->accelerometers[1], pMag->accelerometers[2]);
	fflush(pStream);
}

static SbgErrorCode sbgBasicLoggerHandlerGetUtcTimeFromTimeStamp(uint32_t timeStamp, SbgLogUtcData *pLastUtcData, struct tm *pRefUtcTimestamp, uint32_t *pUs)
{
	SbgErrorCode	 errorCode = SBG_NO_ERROR;

	assert(pLastUtcData);
	assert(pRefUtcTimestamp);
	assert(pUs);

	if (sbgEComLogUtcGetClockUtcStatus(pLastUtcData->status) != SBG_ECOM_UTC_INVALID)
	{
		time_t			 nrSeconds;
		int64_t			 nrMicroseconds;
		int32_t			 elapsedTime;

		pRefUtcTimestamp->tm_sec	= pLastUtcData->second;
		pRefUtcTimestamp->tm_min	= pLastUtcData->minute;
		pRefUtcTimestamp->tm_hour	= pLastUtcData->hour;
		pRefUtcTimestamp->tm_mday	= pLastUtcData->day;
		pRefUtcTimestamp->tm_mon	= pLastUtcData->month - 1;
		pRefUtcTimestamp->tm_year	= pLastUtcData->year - 1900;
		pRefUtcTimestamp->tm_isdst	= -1;

		nrSeconds		= mktime(pRefUtcTimestamp);
		nrMicroseconds	= (int64_t)nrSeconds * 1000000;
		elapsedTime		= timeStamp - pLastUtcData->timeStamp;
		nrMicroseconds	+= elapsedTime;

		nrSeconds		= nrMicroseconds / 1000000;
		nrMicroseconds	%= 1000000;

		*pRefUtcTimestamp	= *localtime(&nrSeconds);
		*pUs				= (uint32_t)nrMicroseconds;
	}
	else
	{
		errorCode = SBG_ERROR;
	}

	return errorCode;
}

//----------------------------------------------------------------------//
//- sbgBasicLoggerCommonHandler methods                                -//
//----------------------------------------------------------------------//

/*!
 * Check if a handler is initialized.
 *
 * \param[in]	pHandler				Common handler.
 * \return								True if initialized.
 */
static bool sbgBasicLoggerCommonHandlerInitialized(SbgBasicLoggerCommonHandler *pHandler)
{
	assert(pHandler);

	if (pHandler->pDesc)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*!
 * Common handler constructor.
 *
 * \param[in]	pHandler				IMU short handler.
 * \param[in]	consoleEnabled			True to print IMU short data on console.
 * \param[in]	writingFile				True to write IMU short data into a file.
 */
static void sbgBasicLoggerCommonHandlerConstruct(SbgBasicLoggerCommonHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader, SbgEComClass class, SbgEComMsgId id)
{
	assert(pHandler);

	pHandler->consoleEnabled		= consoleEnabled;
	pHandler->writingFile			= writingFile;
	pHandler->pDesc					= sbgBasicLoggerFileGetDesc(class, id);
	pHandler->pFILE					= NULL;

	pHandler->consoleDecimation		= consoleDecimation;
	pHandler->fileDecimation		= fileDecimation;
	pHandler->writeHeader			= writeHeader;
	pHandler->headerWritten			= false;

	pHandler->pPathStr				= pPathStr;
	pHandler->class					= class;
	pHandler->id					= id;
}

/*!
 * Check if a file is opened.
 *
 * \param[in]	pHandler				Common handler.
 * \return								True if opened.
 */
static bool sbgBasicLoggerCommonHandlerFileOpened(SbgBasicLoggerCommonHandler *pHandler)
{
	assert(sbgBasicLoggerCommonHandlerInitialized(pHandler));
	return pHandler->pFILE;
}

/*!
 * Check if writing file is enabled.
 *
 * \param[in]	pHandler				Common handler.
 * \return								True if enabled.
 */
static bool sbgBasicLoggerCommonHandlerWritingFile(SbgBasicLoggerCommonHandler *pHandler)
{
	assert(sbgBasicLoggerCommonHandlerInitialized(pHandler));
	return pHandler->writingFile;
}

/*!
 * Check if console is enabled.
 *
 * \param[in]	pHandler				Common handler.
 * \return								True if enabled.
 */
static bool sbgBasicLoggerCommonHandlerConsoleEnabled(SbgBasicLoggerCommonHandler *pHandler)
{
	assert(sbgBasicLoggerCommonHandlerInitialized(pHandler));
	return pHandler->consoleEnabled;
}

/*!
 * Check if header writing is enabled.
 *
 * \param[in]	pHandler				Common handler.
 * \return								True if enabled.
 */
static bool sbgBasicLoggerCommonHandlerHeaderWriting(SbgBasicLoggerCommonHandler *pHandler)
{
	assert(sbgBasicLoggerCommonHandlerInitialized(pHandler));
	return pHandler->writeHeader;
}

/*!
 * Check if header have been written.
 *
 * \param[in]	pHandler				Common handler.
 * \return								True if written.
 */
static bool sbgBasicLoggerCommonHandlerHeaderWritten(SbgBasicLoggerCommonHandler *pHandler)
{
	assert(sbgBasicLoggerCommonHandlerInitialized(pHandler));
	return pHandler->headerWritten;
}

/*!
 * Get the path of the handler.
 *
 * \param[in]	pHandler				Common handler.
 * \return								File path.
 */
static FILE *sbgBasicLoggerCommonHandlerGetPath(SbgBasicLoggerCommonHandler *pHandler)
{
	if (!sbgBasicLoggerCommonHandlerFileOpened(pHandler))
	{
		if (pHandler->pDesc)
		{
			SbgErrorCode			 errorCode;
			SbgString				 fileStr;
			SbgString				 pathStr;

			errorCode = sbgStringConstructCString(&pathStr, pHandler->pPathStr);

			if (errorCode == SBG_NO_ERROR)
			{
				errorCode = sbgStringConstructCString(&fileStr, pHandler->pDesc->pFileName);

				if (errorCode == SBG_NO_ERROR)
				{
					errorCode = sbgStringAppend(&pathStr, &fileStr);

					if (errorCode == SBG_NO_ERROR)
					{
						pHandler->pFILE = fopen(sbgStringGetCString(&pathStr), "wt");
					}

					sbgStringDestroy(&fileStr);
				}

				sbgStringDestroy(&pathStr);
			}
		}
	}

	return pHandler->pFILE;
}

/*!
 * Write log info on an output stream.
 *
 * \param[in]	pHandler				Common handler.
 * \param[in]	pStream					Output stream.
 */
static void sbgBasicLoggerCommonHandlerWriteLogInfo(SbgBasicLoggerCommonHandler *pHandler, void *pStream)
{
	const SbgBasicLoggerFileDesc	*pMsgDesc;

	assert(sbgBasicLoggerCommonHandlerInitialized(pHandler));
	assert(pStream);

	pMsgDesc = sbgBasicLoggerFileGetDesc(pHandler->class, pHandler->id);

	if (pMsgDesc)
	{
		fprintf(pStream, "%14s:  ", pMsgDesc->pName);
	}
	else
	{
		fprintf(pStream, "class:%0.2u id:%0.2u:  ", pHandler->class, pHandler->id);
	}
}

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

//----------------------------------------------------------------------//
//- IMU short handler methods                                          -//
//----------------------------------------------------------------------//

void sbgBasicLoggerImuShortHandlerConstruct(SbgBasicLoggerImuHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader)
{
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, consoleDecimation, writingFile, pPathStr, fileDecimation, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_IMU_SHORT);

	sbgBasicLoggerImuAccZeroInit(&pHandler->consoleAcc);
	sbgBasicLoggerImuAccZeroInit(&pHandler->fileAcc);
}

void sbgBasicLoggerImuHandlerConstruct(SbgBasicLoggerImuHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader)
{
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, consoleDecimation, writingFile, pPathStr, fileDecimation, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_IMU_DATA);

	sbgBasicLoggerImuAccZeroInit(&pHandler->consoleAcc);
	sbgBasicLoggerImuAccZeroInit(&pHandler->fileAcc);
}

void sbgBasicLoggerImuFastHandlerConstruct(SbgBasicLoggerImuHandler *pHandler, bool consoleEnabled, size_t consoleDecimation, bool writingFile, const char *pPathStr, size_t fileDecimation, bool writeHeader)
{
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, consoleDecimation, writingFile, pPathStr, fileDecimation, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_1, SBG_ECOM_LOG_FAST_IMU_DATA);

	sbgBasicLoggerImuAccZeroInit(&pHandler->consoleAcc);
	sbgBasicLoggerImuAccZeroInit(&pHandler->fileAcc);
}

void sbgBasicLoggerImuHandlerProcess(SbgBasicLoggerImuHandler *pHandler, const SbgBasicLoggerImu *pNewData)
{
	assert(pNewData);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if ((sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler)) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteImuHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerImuAccAdd(&pHandler->fileAcc, pNewData);

			if (pHandler->fileAcc.nrAcc >= pHandler->commonHandler.fileDecimation)
			{
				SbgBasicLoggerImu			 decimated;

				sbgBasicLoggerImuAccGet(&pHandler->fileAcc, &decimated);

				sbgBasicLoggerHandlerWriteImu(&decimated, pOutputFile);
			}
		}
		else
		{
			SBG_LOG_ERROR(SBG_NULL_POINTER, "no file found for IMU short log");
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerImuAccAdd(&pHandler->consoleAcc, pNewData);

		if (pHandler->consoleAcc.nrAcc >= pHandler->commonHandler.consoleDecimation)
		{
			SbgBasicLoggerImu			 decimated;

			sbgBasicLoggerImuAccGet(&pHandler->consoleAcc, &decimated);

			sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
			sbgBasicLoggerHandlerWriteImu(&decimated, stdout);
		}
	}
}

//----------------------------------------------------------------------//
//- IMU cluster handler methods                                        -//
//----------------------------------------------------------------------//


//----------------------------------------------------------------------//
//- Diagnostic handler methods                                         -//
//----------------------------------------------------------------------//

void sbgBasicLoggerDiagHandlerConstruct(SbgBasicLoggerDiagHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, 0, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_DIAG);
}

void sbgBasicLoggerDiagHandlerProcess(SbgBasicLoggerDiagHandler *pHandler, SbgLogUtcData *pLastUtcData, const SbgLogDiagData *pDiag)
{
	struct tm			 utcTime;
	char				 utcTimeStr[128];
	uint32_t			 us;

	assert(pHandler);
	assert(pDiag);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	if (sbgBasicLoggerHandlerGetUtcTimeFromTimeStamp(pDiag->timestamp, pLastUtcData, &utcTime, &us) == SBG_NO_ERROR)
	{
		char			 dateStr[64];

		strftime(dateStr, sizeof(dateStr), "%Y-%m-%dT%H:%M:%S", &utcTime);
		snprintf(utcTimeStr, sizeof(utcTimeStr), "%s:%.3" PRIu32 "Z", dateStr, us / 1000);
	}
	else
	{
		snprintf(utcTimeStr, sizeof(utcTimeStr), "%10u", pDiag->timestamp);
	}

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			sbgBasicLoggerHandlerWriteDiag(pDiag, utcTimeStr, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerHandlerWriteDiag(pDiag, utcTimeStr, stdout);
	}
}

//----------------------------------------------------------------------//
//- UTC handler methods                                                -//
//----------------------------------------------------------------------//

void sbgBasicLoggerUtcHandlerConstruct(SbgBasicLoggerUtcHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_UTC_TIME);
}

void sbgBasicLoggerUtcHandlerProcess(SbgBasicLoggerUtcHandler *pHandler, const SbgLogUtcData *pUtc)
{
	assert(pHandler);
	assert(pUtc);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteUtcHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteUtc(pUtc, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteUtc(pUtc, stdout);
	}
}

//----------------------------------------------------------------------//
//- Status handler methods                                             -//
//----------------------------------------------------------------------//

void sbgBasicLoggerStatusHandlerConstruct(SbgBasicLoggerStatusHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_STATUS);
}

void sbgBasicLoggerStatusHandlerProcess(SbgBasicLoggerStatusHandler *pHandler, const SbgLogStatusData *pStatus)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pStatus);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteStatusHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteStatus(pStatus, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteStatus(pStatus, stdout);
	}
}

//----------------------------------------------------------------------//
//- EKF Euler handler methods                                          -//
//----------------------------------------------------------------------//

void sbgBasicLoggerEkfEulerHandlerConstruct(SbgBasicLoggerEkfEulerHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_EULER);
}

void sbgBasicLoggerEkfEulerHandlerProcess(SbgBasicLoggerEkfEulerHandler *pHandler, const SbgLogEkfEulerData *pEkfEuler)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pEkfEuler);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteEkfEulerHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteEkfEuler(pEkfEuler, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteEkfEuler(pEkfEuler, stdout);
	}
}

//----------------------------------------------------------------------//
//- EKF quat handler methods                                          -//
//----------------------------------------------------------------------//

void sbgBasicLoggerEkfQuatHandlerConstruct(SbgBasicLoggerEkfQuatHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_QUAT);
}

void sbgBasicLoggerEkfQuatHandlerProcess(SbgBasicLoggerEkfQuatHandler *pHandler, const SbgLogEkfQuatData *pEkfQuat)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pEkfQuat);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteEkfQuatHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteEkfQuat(pEkfQuat, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteEkfQuat(pEkfQuat, stdout);
	}
}

//----------------------------------------------------------------------//
//- EKF nav handler methods                                            -//
//----------------------------------------------------------------------//

void sbgBasicLoggerEkfNavHandlerConstruct(SbgBasicLoggerEkfNavHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EKF_NAV);
}

void sbgBasicLoggerEkfNavHandlerProcess(SbgBasicLoggerEkfNavHandler *pHandler, const SbgLogEkfNavData *pEkfNav)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pEkfNav);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteEkfNavHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteEkfNav(pEkfNav, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteEkfNav(pEkfNav, stdout);
	}
}

//----------------------------------------------------------------------//
//- Ship motion methods                                                -//
//----------------------------------------------------------------------//

void sbgBasicLoggerShipMotionHandlerConstruct(SbgBasicLoggerShipMotionHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_SHIP_MOTION);
}

void sbgBasicLoggerShipMotionHandlerProcess(SbgBasicLoggerShipMotionHandler *pHandler, const SbgLogShipMotionData *pShipMotion)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pShipMotion);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteShipMotionHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteShipMotion(pShipMotion, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteShipMotion(pShipMotion, stdout);
	}
}

//----------------------------------------------------------------------//
//- Ship motion HP methods                                             -//
//----------------------------------------------------------------------//

void sbgBasicLoggerShipMotionHpHandlerConstruct(SbgBasicLoggerShipMotionHpHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_SHIP_MOTION_HP);
}

void sbgBasicLoggerShipMotionHpHandlerProcess(SbgBasicLoggerShipMotionHpHandler *pHandler, const SbgLogShipMotionData *pShipMotion)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pShipMotion);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteShipMotionHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteShipMotion(pShipMotion, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteShipMotion(pShipMotion, stdout);
	}
}

//----------------------------------------------------------------------//
//- GPS vel handler methods                                            -//
//----------------------------------------------------------------------//

void sbgBasicLoggerGpsVel1HandlerConstruct(SbgBasicLoggerGpsVelHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS1_VEL);
}

void sbgBasicLoggerGpsVel2HandlerConstruct(SbgBasicLoggerGpsVelHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS2_VEL);
}

void sbgBasicLoggerGpsVelHandlerProcess(SbgBasicLoggerGpsVelHandler *pHandler, const SbgLogGpsVel *pGps)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pGps);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteGpsVelHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteGpsVel(pGps, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteGpsVel(pGps, stdout);
	}
}

//----------------------------------------------------------------------//
//- GPS pos handler methods                                            -//
//----------------------------------------------------------------------//

void sbgBasicLoggerGpsPos1HandlerConstruct(SbgBasicLoggerGpsPosHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS1_POS);
}

void sbgBasicLoggerGpsPos2HandlerConstruct(SbgBasicLoggerGpsPosHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS2_POS);
}

void sbgBasicLoggerGpsPosHandlerProcess(SbgBasicLoggerGpsPosHandler *pHandler, const SbgLogGpsPos *pGps)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pGps);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteGpsHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteGps(pGps, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteGps(pGps, stdout);
	}
}

//----------------------------------------------------------------------//
//- GPS hdt handler methods                                            -//
//----------------------------------------------------------------------//

void sbgBasicLoggerGpsHdt1HandlerConstruct(SbgBasicLoggerGpsHdtHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS1_HDT);
}

void sbgBasicLoggerGpsHdt2HandlerConstruct(SbgBasicLoggerGpsHdtHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS2_HDT);
}

void sbgBasicLoggerGpsHdtHandlerProcess(SbgBasicLoggerGpsHdtHandler *pHandler, const SbgLogGpsHdt *pGps)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pGps);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteGpsHdtHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteGpsHdt(pGps, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteGpsHdt(pGps, stdout);
	}
}

//----------------------------------------------------------------------//
//- Raw GPS handler methods                                            -//
//----------------------------------------------------------------------//

void sbgBasicLoggerGps1RawHandlerConstruct(SbgBasicLoggerGpsRawHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, 0, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS1_RAW);
}

void sbgBasicLoggerGps2RawHandlerConstruct(SbgBasicLoggerGpsRawHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, 0, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS2_RAW);
}

void sbgBasicLoggerGpsRawHandlerProcess(SbgBasicLoggerGpsRawHandler *pHandler, const SbgLogRawData *pRawGps)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pRawGps);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			sbgBasicLoggerHandlerWriteGpsRaw(pRawGps, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteGpsRaw(pRawGps, stdout);
	}
}

//----------------------------------------------------------------------//
//- GNSS Satellite in View handler methods                             -//
//----------------------------------------------------------------------//

void sbgBasicLoggerGps1SatHandlerConstruct(SbgBasicLoggerGpsSatHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, 0, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS1_SAT);
}

void sbgBasicLoggerGps2SatHandlerConstruct(SbgBasicLoggerGpsSatHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));

	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, 0, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_GPS2_SAT);
}

void sbgBasicLoggerGpsSatHandlerProcess(SbgBasicLoggerGpsSatHandler *pHandler, const SbgLogSatGroupData *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteGpsSatHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteGpsSat(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteGpsSat(pData, stdout);
	}
}
//----------------------------------------------------------------------//
//- Odometer handler methods                                           -//
//----------------------------------------------------------------------//

void sbgBasicLoggerOdometerHandlerConstruct(SbgBasicLoggerOdometerHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_ODO_VEL);
}

void sbgBasicLoggerOdometerHandlerProcess(SbgBasicLoggerOdometerHandler *pHandler, const SbgLogOdometerData *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteOdometerHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteOdometer(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteOdometer(pData, stdout);
	}
}

//----------------------------------------------------------------------//
//- DVL handler methods                                                -//
//----------------------------------------------------------------------//

void sbgBasicLoggerDvlBottomHandlerConstruct(SbgBasicLoggerDvlHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_DVL_BOTTOM_TRACK);
}

void sbgBasicLoggerDvlWaterHandlerConstruct(SbgBasicLoggerDvlHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_DVL_WATER_TRACK);
}

void sbgBasicLoggerDvlHandlerProcess(SbgBasicLoggerDvlHandler *pHandler, const SbgLogDvlData *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteDvlHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteDvl(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteDvl(pData, stdout);
	}
}

//----------------------------------------------------------------------//
//- Air handler methods                                                -//
//----------------------------------------------------------------------//

void sbgBasicLoggerAirHandlerConstruct(SbgBasicLoggerAirHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_AIR_DATA);
}

void sbgBasicLoggerAirHandlerProcess(SbgBasicLoggerAirHandler *pHandler, const SbgLogAirData *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteAirHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteAir(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteAir(pData, stdout);
	}
}

//----------------------------------------------------------------------//
//- USBL handler methods                                               -//
//----------------------------------------------------------------------//

void sbgBasicLoggerUsblHandlerConstruct(SbgBasicLoggerUsblHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_USBL);
}

void sbgBasicLoggerUsblHandlerProcess(SbgBasicLoggerUsblHandler *pHandler, const SbgLogUsblData *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteUsblHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteUsbl(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteUsbl(pData, stdout);
	}
}

//----------------------------------------------------------------------//
//- Depth handler methods                                              -//
//----------------------------------------------------------------------//

void sbgBasicLoggerDepthHandlerConstruct(SbgBasicLoggerDepthHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_DEPTH);
}

void sbgBasicLoggerDepthHandlerProcess(SbgBasicLoggerDepthHandler *pHandler, const SbgLogDepth *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteDepthHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteDepth(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerHandlerWriteDepth(pData, stdout);
	}
}

//----------------------------------------------------------------------//
//- Raw RTCM handler methods                                           -//
//----------------------------------------------------------------------//

void sbgBasicLoggerRawRtcmHandlerConstruct(SbgBasicLoggerRawRtcmHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, 0, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_RTCM_RAW);
}

void sbgBasicLoggerRawRtcmHandlerProcess(SbgBasicLoggerRawRtcmHandler *pHandler, const SbgLogRawData *pData)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pData);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			sbgBasicLoggerHandlerWriteRawRtcm(pData, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteRawRtcm(pData, stdout);
	}
}

//----------------------------------------------------------------------//
//- Events handler methods                                             -//
//----------------------------------------------------------------------//

void sbgBasicLoggerEventInAHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_A);
}

void sbgBasicLoggerEventInBHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_B);
}

void sbgBasicLoggerEventInCHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_C);
}

void sbgBasicLoggerEventInDHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_D);
}

void sbgBasicLoggerEventInEHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_E);
}

void sbgBasicLoggerEventOutAHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_OUT_A);
}

void sbgBasicLoggerEventOutBHandlerConstruct(SbgBasicLoggerEventHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_EVENT_OUT_B);
}

void sbgBasicLoggerEventHandlerProcess(SbgBasicLoggerEventHandler *pHandler, const SbgLogEvent *pEvent)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pEvent);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteEventHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteEvent(pEvent, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteEvent(pEvent, stdout);
	}
}

//----------------------------------------------------------------------//
//- Mag handler methods                                                -//
//----------------------------------------------------------------------//

void sbgBasicLoggerMagHandlerConstruct(SbgBasicLoggerMagHandler *pHandler, bool consoleEnabled, bool writingFile, const char *pPathStr, bool writeHeader)
{
	assert(pHandler);
	assert(!sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	sbgBasicLoggerCommonHandlerConstruct(&pHandler->commonHandler, consoleEnabled, 0, writingFile, pPathStr, 0, writeHeader, SBG_ECOM_CLASS_LOG_ECOM_0, SBG_ECOM_LOG_MAG);
}

void sbgBasicLoggerMagHandlerProcess(SbgBasicLoggerMagHandler *pHandler, const SbgLogMag *pMag)
{
	assert(pHandler);
	assert(sbgBasicLoggerCommonHandlerInitialized(&pHandler->commonHandler));
	assert(pMag);

	if (sbgBasicLoggerCommonHandlerWritingFile(&pHandler->commonHandler))
	{
		FILE				*pOutputFile;

		pOutputFile = sbgBasicLoggerCommonHandlerGetPath(&pHandler->commonHandler);

		if (pOutputFile)
		{
			if (sbgBasicLoggerCommonHandlerHeaderWriting(&pHandler->commonHandler) && (!sbgBasicLoggerCommonHandlerHeaderWritten(&pHandler->commonHandler)))
			{
				sbgBasicLoggerHandlerWriteMagHeader(pHandler->commonHandler.pFILE);
				pHandler->commonHandler.headerWritten = true;
			}

			sbgBasicLoggerHandlerWriteMag(pMag, pOutputFile);
		}
	}

	if (sbgBasicLoggerCommonHandlerConsoleEnabled(&pHandler->commonHandler))
	{
		sbgBasicLoggerCommonHandlerWriteLogInfo(&pHandler->commonHandler, stdout);
		sbgBasicLoggerHandlerWriteMag(pMag, stdout);
	}
}
