// sbgCommonLib
#include <sbgCommon.h>

// sbgECom headers
#include <sbgEComLib.h>

// Local header
#include "sbgBasicLoggerAccumulators.h"

//----------------------------------------------------------------------//
//- Private methods for SbgEComBasicLoggerAcc                          -//
//----------------------------------------------------------------------//

void sbgBasicLoggerImuAccZeroInit(SbgBasicLoggerImuAcc *pAccumulator)
{
	assert(pAccumulator);

	memset(pAccumulator, 0x00, sizeof(*pAccumulator));
}

void sbgBasicLoggerImuAccAdd(SbgBasicLoggerImuAcc *pAccumulator, const SbgBasicLoggerImu *pNewImuData)
{
	assert(pAccumulator);
	assert(pNewImuData);

	pAccumulator->imuAccumulated.timestamp	= pNewImuData->timestamp;
	pAccumulator->imuAccumulated.status 	|= pNewImuData->status;

	for (size_t i = 0; i < 3; i++)
	{
		pAccumulator->imuAccumulated.deltaAngle[i] += pNewImuData->deltaAngle[i];
		pAccumulator->imuAccumulated.deltaVelocity[i] += pNewImuData->deltaVelocity[i];
	}

	pAccumulator->imuAccumulated.temperature += pNewImuData->temperature;

	pAccumulator->nrAcc++;
}

void sbgBasicLoggerImuAccGet(SbgBasicLoggerImuAcc *pAccumulator, SbgBasicLoggerImu *pDecimatedImu)
{
	assert(pAccumulator);
	assert(pDecimatedImu);

	pDecimatedImu->timestamp	= pAccumulator->imuAccumulated.timestamp;
	pDecimatedImu->status		= pAccumulator->imuAccumulated.status;

	for (size_t i = 0; i < 3; i++)
	{
		pDecimatedImu->deltaAngle[i]	= pAccumulator->imuAccumulated.deltaAngle[i] / (double)pAccumulator->nrAcc;
		pDecimatedImu->deltaVelocity[i]	= pAccumulator->imuAccumulated.deltaVelocity[i] / (double)pAccumulator->nrAcc;
	}

	pDecimatedImu->temperature = pAccumulator->imuAccumulated.temperature / (double)pAccumulator->nrAcc;

	sbgBasicLoggerImuAccZeroInit(pAccumulator);
}

