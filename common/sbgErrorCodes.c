// Local headers
#include "sbgErrorCodes.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

SBG_COMMON_LIB_API const char *sbgErrorCodeToString(SbgErrorCode errorCode)
{
    static const char *errorCodeToStr[] =
    {
        [SBG_NO_ERROR]                          = "SBG_NO_ERROR",
        [SBG_ERROR]                             = "SBG_ERROR",
        [SBG_NULL_POINTER]                      = "SBG_NULL_POINTER",
        [SBG_INVALID_CRC]                       = "SBG_INVALID_CRC",
        [SBG_INVALID_FRAME]                     = "SBG_INVALID_FRAME",
        [SBG_TIME_OUT]                          = "SBG_TIME_OUT",
        [SBG_WRITE_ERROR]                       = "SBG_WRITE_ERROR",
        [SBG_READ_ERROR]                        = "SBG_READ_ERROR",
        [SBG_BUFFER_OVERFLOW]                   = "SBG_BUFFER_OVERFLOW",
        [SBG_INVALID_PARAMETER]                 = "SBG_INVALID_PARAMETER",
        [SBG_NOT_READY]                         = "SBG_NOT_READY",
        [SBG_MALLOC_FAILED]                     = "SBG_MALLOC_FAILED",
        [SBG_CALIB_MAG_NOT_ENOUGH_POINTS]       = "SBG_CALIB_MAG_NOT_ENOUGH_POINTS",
        [SBG_CALIB_MAG_INVALID_TAKE]            = "SBG_CALIB_MAG_INVALID_TAKE",
        [SBG_CALIB_MAG_SATURATION]              = "SBG_CALIB_MAG_SATURATION",
        [SBG_CALIB_MAG_POINTS_NOT_IN_A_PLANE]   = "SBG_CALIB_MAG_POINTS_NOT_IN_A_PLANE",
        [SBG_DEVICE_NOT_FOUND]                  = "SBG_DEVICE_NOT_FOUND",
        [SBG_OPERATION_CANCELLED]               = "SBG_OPERATION_CANCELLED",
        [SBG_NOT_CONTINUOUS_FRAME]              = "SBG_NOT_CONTINUOUS_FRAME",
        [SBG_INCOMPATIBLE_HARDWARE]             = "SBG_INCOMPATIBLE_HARDWARE",
        [SBG_INVALID_VERSION]                   = "SBG_INVALID_VERSION"
    };

    assert(errorCode >= 0);
    assert(errorCode < SBG_ARRAY_SIZE(errorCodeToStr));

    return errorCodeToStr[errorCode];
}
