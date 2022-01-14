/*!
 *    \file         sbgInterfaceFile.h
 *    \author       SBG Systems (Raphael Siryani)
 *    \date         01 April 2013
 *
 *    \brief        This file implements a file interface for read only operations.
 *
 *    \section CodeCopyright Copyright Notice
 *    The MIT license
 *
 *    Copyright (C) 2007-2020, SBG Systems SAS. All rights reserved.
 *
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#ifndef SBG_INTERFACE_FILE_H
#define SBG_INTERFACE_FILE_H

//----------------------------------------------------------------------//
//- Header (open extern C block)                                       -//
//----------------------------------------------------------------------//
#ifdef __cplusplus
extern "C" {
#endif

// sbgCommonLib headers
#include <sbgCommon.h>

// Local headers
#include "sbgInterface.h"

//----------------------------------------------------------------------//
//- Public methods                                                     -//
//----------------------------------------------------------------------//

/*!
 *	Open a file as an interface for read only operations.
 *
 *	\param[in]	pHandle							Pointer on an allocated interface instance to initialize.
 *	\param[in]	filePath						File path to open.
 *	\return										SBG_NO_ERROR if the interface has been created.
 */
SBG_COMMON_LIB_API SbgErrorCode sbgInterfaceFileOpen(SbgInterface *pHandle, const char *filePath);

/*!
 *	Open a file as an interface for write only operations.
 *
 *	\param[in]	pHandle							Pointer on an allocated interface instance to initialize.
 *	\param[in]	filePath						File path to open.
 *	\return										SBG_NO_ERROR if the interface has been created.
 */
SBG_COMMON_LIB_API SbgErrorCode sbgInterfaceFileWriteOpen(SbgInterface *pHandle, const char *filePath);

/*!
 *	Returns the file size in bytes.
 *
 *	\param[in]	pInterface						Valid handle on an initialized interface.
 *	\return										The file size in bytes.
 */
SBG_COMMON_LIB_API size_t sbgInterfaceFileGetSize(SbgInterface *pHandle);

/*!
 *	Returns the current cursor position in the file in bytes.
 *
 *	\param[in]	pInterface						Valid handle on an initialized interface.
 *	\return										The current cursor position in bytes.
 */
SBG_COMMON_LIB_API size_t sbgInterfaceFileGetCursor(const SbgInterface *pHandle);

//----------------------------------------------------------------------//
//- Footer (close extern C block)                                      -//
//----------------------------------------------------------------------//
#ifdef __cplusplus
}
#endif

#endif /* SBG_INTERFACE_FILE_H */
