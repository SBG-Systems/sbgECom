/*!
 * \file			sbgBasicLoggerAccumulators.h
 * \author			SBG Systems
 * \date			July 29, 2021
 *
 * \brief			Ease output log files management.
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

#ifndef SBG_BASIC_LOGGER_FILE_H
#define SBG_BASIC_LOGGER_FILE_H

// sbgCommonLib headers
#include <sbgCommon.h>

// sbgECom headers
#include <sbgEComLib.h>

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * SBG basic logger file descriptor.
 */
typedef struct _SbgBasicLoggerFileDesc
{
	SbgEComClass						 class;						/*!< Message class. */
	SbgEComMsgId						 id;						/*!< Message ID. */
	const char							*pName;						/*!< The log name. */
	const char							*pFileName;					/*!< File name suffix. */
} SbgBasicLoggerFileDesc;

//----------------------------------------------------------------------//
//- Public functions                                                   -//
//----------------------------------------------------------------------//

/*!
 * Return the file descriptor.
 *
 * \param[in]	msgClass				Message class.
 * \param[in]	msgId					Message ID.
 * \return								File descriptor.
 */
const SbgBasicLoggerFileDesc *sbgBasicLoggerFileGetDesc(SbgEComClass msgClass, SbgEComMsgId msgId);

#endif // SBG_BASIC_LOGGER_FILE_H
