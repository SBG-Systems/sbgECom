// sbgCommonLib headers
#include <sbgCommon.h>

// sbgECom headers
#include <sbgEComLib.h>
#include <sbgEComIds.h>

// Local headers
#include "sbgBasicLoggerFile.h"

//----------------------------------------------------------------------//
//- Private variables                                                  -//
//----------------------------------------------------------------------//

static SbgBasicLoggerFileDesc gSbgBasicLoggerFileDescs[] =
{
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_STATUS,				.pName = "STATUS",			.pFileName = "log_status.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_UTC_TIME,			.pName = "UTC",				.pFileName = "log_utc.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_DIAG,				.pName = "DIAG",			.pFileName = "log_diag.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_IMU_DATA,			.pName = "IMU_DATA",		.pFileName = "log_imu.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_IMU_SHORT,			.pName = "IMU_SHORT",		.pFileName = "log_imuShort.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_MAG,					.pName = "MAG",				.pFileName = "log_mag.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_MAG_CALIB,			.pName = "MAG_CALIB",		.pFileName = "log_magCalib.txt"			},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EKF_EULER,			.pName = "EULER",			.pFileName = "log_euler.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EKF_QUAT,			.pName = "QUAT",			.pFileName = "log_quat.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EKF_NAV,				.pName = "NAV",				.pFileName = "log_nav.txt"				},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_SHIP_MOTION,			.pName = "SHIP_MOTION",		.pFileName = "log_shipMotion.txt"		},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_SHIP_MOTION_HP,		.pName = "SHIP_MOTION_HP",	.pFileName = "log_shipMotionHP.txt"		},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS1_VEL,			.pName = "GPS1_VEL",		.pFileName = "log_gps1Vel.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS1_POS,			.pName = "GPS1_POS",		.pFileName = "log_gps1Pos.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS1_HDT,			.pName = "GPS1_HDT",		.pFileName = "log_gps1Hdt.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS1_RAW,			.pName = "GPS1_RAW",		.pFileName = "log_gps1Raw.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS1_SAT,			.pName = "GPS1_SAT",		.pFileName = "log_gps1Sat.txt"			},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS2_VEL,			.pName = "GPS2_VEL",		.pFileName = "log_gps2Vel.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS2_POS,			.pName = "GPS2_POS",		.pFileName = "log_gps2Pos.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS2_HDT,			.pName = "GPS2_HDT",		.pFileName = "log_gps2Hdt.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS2_RAW,			.pName = "GPS2_RAW",		.pFileName = "log_gps2Raw.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_GPS2_SAT,			.pName = "GPS2_SAT",		.pFileName = "log_gps2Sat.txt"			},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_RTCM_RAW,			.pName = "RTCM_RAW",		.pFileName = "log_rtcmRaw.txt"			},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_ODO_VEL,				.pName = "ODO_VEL",			.pFileName = "log_odo.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_DVL_BOTTOM_TRACK,	.pName = "DVL_BOTTOM",		.pFileName = "log_dvl_bottom.txt"		},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_DVL_WATER_TRACK,		.pName = "DVL_WATER",		.pFileName = "log_dvl_water.txt"		},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_AIR_DATA,			.pName = "AIR_DATA",		.pFileName = "log_air.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_USBL,				.pName = "USBL",			.pFileName = "log_usbl.txt"				},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_DEPTH,				.pName = "DEPTH",			.pFileName = "log_depth.txt"			},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_A,				.pName = "EVENT_IN_A",		.pFileName = "log_eventInA.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_B,				.pName = "EVENT_IN_B",		.pFileName = "log_eventInB.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_C,				.pName = "EVENT_IN_C",		.pFileName = "log_eventInC.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_D,				.pName = "EVENT_IN_D",		.pFileName = "log_eventInD.txt"			},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_E,				.pName = "EVENT_IN_E",		.pFileName = "log_eventInE.txt"			},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_OUT_A,			.pName = "EVENT_OUT_A",		.pFileName = "log_eventOutA.txt"		},
	{	.class = SBG_ECOM_CLASS_LOG_ECOM_0,			.id = SBG_ECOM_LOG_EVENT_OUT_B,			.pName = "EVENT_OUT_B",		.pFileName = "log_eventOutB.txt"		},

	{	.class = SBG_ECOM_CLASS_LOG_ECOM_1,			.id = SBG_ECOM_LOG_FAST_IMU_DATA,		.pName = "IMU_FAST",		.pFileName = "log_imuFast.txt"			},

};

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

const SbgBasicLoggerFileDesc *sbgBasicLoggerFileGetDesc(SbgEComClass msgClass, SbgEComMsgId msgId)
{
	const SbgBasicLoggerFileDesc				*pDesc = NULL;

	for (size_t i = 0; i < SBG_ARRAY_SIZE(gSbgBasicLoggerFileDescs); i++)
	{
		const SbgBasicLoggerFileDesc			*pTmpDesc;

		pTmpDesc = &gSbgBasicLoggerFileDescs[i];

		if ((pTmpDesc->class == msgClass) && (pTmpDesc->id == msgId))
		{
			pDesc = pTmpDesc;
			break;
		}
	}

	return pDesc;
}
