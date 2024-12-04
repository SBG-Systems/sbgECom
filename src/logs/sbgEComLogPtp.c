// sbgCommonLib headers
#include <sbgCommon.h>
#include <streamBuffer/sbgStreamBuffer.h>

// Local headers
#include "sbgEComLogPtp.h"

//----------------------------------------------------------------------//
//- Private functions                                                  -//
//----------------------------------------------------------------------//

/*!
 * Convert a value into a state.
 *
 * \param[in]   value                       Value.
 * \param[out]  pState                      State.
 * \return                                  SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComLogPtpConvertState(uint8_t value, SbgEComLogPtpState *pState)
{
    SbgErrorCode                         errorCode = SBG_INVALID_PARAMETER;

    assert(pState);

    switch ((SbgEComLogPtpState)value)
    {
    case SBG_ECOM_LOG_PTP_STATE_DISABLED:
    case SBG_ECOM_LOG_PTP_STATE_FAULTY:
    case SBG_ECOM_LOG_PTP_STATE_MASTER:
    case SBG_ECOM_LOG_PTP_STATE_PASSIVE:
        *pState     = (SbgEComLogPtpState)value;
        errorCode   = SBG_NO_ERROR;
        break;
    }

    if (errorCode == SBG_INVALID_PARAMETER)
    {
        SBG_LOG_ERROR(errorCode, "unable to convert state: invalid value %" PRIu8, value);
    }

    return errorCode;
}

/*!
 * Convert a value into a time scale.
 *
 * \param[in]   value                       Value.
 * \param[out]  pTimeScale                  Time scale.
 * \return                                  SBG_NO_ERROR if successful.
 */
static SbgErrorCode sbgEComLogPtpConvertTimeScale(uint8_t value, SbgEComLogPtpTimeScale *pTimeScale)
{
    SbgErrorCode                         errorCode = SBG_INVALID_PARAMETER;

    assert(pTimeScale);

    switch ((SbgEComLogPtpTimeScale)value)
    {
    case SBG_ECOM_LOG_PTP_TIME_SCALE_TAI:
    case SBG_ECOM_LOG_PTP_TIME_SCALE_UTC:
    case SBG_ECOM_LOG_PTP_TIME_SCALE_GPS:
        *pTimeScale = (SbgEComLogPtpTimeScale)value;
        errorCode   = SBG_NO_ERROR;
        break;
    }

    if (errorCode == SBG_INVALID_PARAMETER)
    {
        SBG_LOG_ERROR(errorCode, "unable to convert time scale: invalid value %" PRIu8, value);
    }

    return errorCode;
}

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

void sbgEComLogPtpZeroInit(SbgEComLogPtp *pLogData)
{
    assert(pLogData);

    memset(pLogData, 0, sizeof(*pLogData));

    pLogData->state                         = SBG_ECOM_LOG_PTP_STATE_FAULTY;
    pLogData->timeScale                     = SBG_ECOM_LOG_PTP_TIME_SCALE_TAI;
    pLogData->timeScaleOffset               = 0.0;
    pLogData->localClockIdentity            = UINT64_MAX;
    pLogData->masterClockIdentity           = UINT64_MAX;
    pLogData->masterIpAddress               = UINT32_MAX;
    pLogData->meanPathDelay                 = NAN;
    pLogData->meanPathDelayStdDev           = NAN;
    pLogData->clockOffset                   = NAN;
    pLogData->clockOffsetStdDev             = NAN;
    pLogData->clockFreqOffset               = NAN;
    pLogData->clockFreqOffsetStdDev         = NAN;
}

SbgErrorCode sbgEComLogPtpReadFromStream(SbgEComLogPtp *pLogData, SbgStreamBuffer *pStreamBuffer)
{
    SbgErrorCode                         errorCode;
    uint8_t                              state;
    uint8_t                              timeScale;

    assert(pStreamBuffer);
    assert(pLogData);

    pLogData->timeStamp                 = sbgStreamBufferReadUint32LE(pStreamBuffer);
    state                               = sbgStreamBufferReadUint8(pStreamBuffer);
    timeScale                           = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->timeScaleOffset           = sbgStreamBufferReadDoubleLE(pStreamBuffer);

    pLogData->localClockIdentity        = sbgStreamBufferReadUint64LE(pStreamBuffer);
    pLogData->localClockPriority1       = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->localClockPriority2       = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->localClockClass           = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->localClockAccuracy        = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->localClockLog2Variance    = sbgStreamBufferReadUint16LE(pStreamBuffer);
    pLogData->localClockTimeSource      = sbgStreamBufferReadUint8(pStreamBuffer);

    pLogData->masterClockIdentity       = sbgStreamBufferReadUint64LE(pStreamBuffer);
    pLogData->masterClockPriority1      = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->masterClockPriority2      = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->masterClockClass          = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->masterClockAccuracy       = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->masterClockLog2Variance   = sbgStreamBufferReadUint16LE(pStreamBuffer);
    pLogData->masterClockTimeSource     = sbgStreamBufferReadUint8(pStreamBuffer);
    pLogData->masterIpAddress           = sbgStreamBufferReadUint32LE(pStreamBuffer);

    pLogData->meanPathDelay             = sbgStreamBufferReadFloatLE(pStreamBuffer);
    pLogData->meanPathDelayStdDev       = sbgStreamBufferReadFloatLE(pStreamBuffer);
    pLogData->clockOffset               = sbgStreamBufferReadDoubleLE(pStreamBuffer);
    pLogData->clockOffsetStdDev         = sbgStreamBufferReadFloatLE(pStreamBuffer);
    pLogData->clockFreqOffset           = sbgStreamBufferReadFloatLE(pStreamBuffer);
    pLogData->clockFreqOffsetStdDev     = sbgStreamBufferReadFloatLE(pStreamBuffer);

    errorCode = sbgStreamBufferGetLastError(pStreamBuffer);

    if (errorCode == SBG_NO_ERROR)
    {
        errorCode = sbgEComLogPtpConvertState(state, &pLogData->state);
    }

    if (errorCode == SBG_NO_ERROR)
    {
        errorCode = sbgEComLogPtpConvertTimeScale(timeScale, &pLogData->timeScale);
    }

    return errorCode;
}

SbgErrorCode sbgEComLogPtpWriteToStream(const SbgEComLogPtp *pLogData, SbgStreamBuffer *pStreamBuffer)
{
    assert(pStreamBuffer);
    assert(pLogData);

    sbgStreamBufferWriteUint32LE(pStreamBuffer, pLogData->timeStamp);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->state);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->timeScale);
    sbgStreamBufferWriteDoubleLE(pStreamBuffer, pLogData->timeScaleOffset);

    sbgStreamBufferWriteUint64LE(pStreamBuffer, pLogData->localClockIdentity);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->localClockPriority1);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->localClockPriority2);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->localClockClass);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->localClockAccuracy);
    sbgStreamBufferWriteUint16LE(pStreamBuffer, pLogData->localClockLog2Variance);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->localClockTimeSource);

    sbgStreamBufferWriteUint64LE(pStreamBuffer, pLogData->masterClockIdentity);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->masterClockPriority1);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->masterClockPriority2);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->masterClockClass);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->masterClockAccuracy);
    sbgStreamBufferWriteUint16LE(pStreamBuffer, pLogData->masterClockLog2Variance);
    sbgStreamBufferWriteUint8(pStreamBuffer,    pLogData->masterClockTimeSource);
    sbgStreamBufferWriteUint32LE(pStreamBuffer, pLogData->masterIpAddress);

    sbgStreamBufferWriteFloatLE(pStreamBuffer,  pLogData->meanPathDelay);
    sbgStreamBufferWriteFloatLE(pStreamBuffer,  pLogData->meanPathDelayStdDev);
    sbgStreamBufferWriteDoubleLE(pStreamBuffer, pLogData->clockOffset);
    sbgStreamBufferWriteFloatLE(pStreamBuffer,  pLogData->clockOffsetStdDev);
    sbgStreamBufferWriteFloatLE(pStreamBuffer,  pLogData->clockFreqOffset);
    sbgStreamBufferWriteFloatLE(pStreamBuffer,  pLogData->clockFreqOffsetStdDev);

    return sbgStreamBufferGetLastError(pStreamBuffer);
}
