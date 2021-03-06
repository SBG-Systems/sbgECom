/*!
 *      \file       sbgCommonLibVersion.h
 *      \author     SBG Systems (Raphael Siryani)
 *      \date       26/07/2017
 *
 *      \brief      SBG Systems platform support dynamic library.
 *
 *      \section CodeCopyright Copyright Notice
 *      The MIT license
 *
 *      Copyright (C) 2007-2020, SBG Systems SAS. All rights reserved.
 *
 *      Permission is hereby granted, free of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in all
 *      copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *      SOFTWARE.
 */

#ifndef SBG_COMMON_LIB_VERSION_H
#define SBG_COMMON_LIB_VERSION_H

//----------------------------------------------------------------------//
//- Version definitions                                                -//
//----------------------------------------------------------------------//

#define SBG_COMMON_LIB_VERSION_MAJOR			1
#define SBG_COMMON_LIB_VERSION_MINOR			5
#define SBG_COMMON_LIB_VERSION_BUILD			1238
#define SBG_COMMON_LIB_VERSION_QUALIFIER		SBG_VERSION_QUALIFIER_STABLE

#define SBG_COMMON_LIB_VERSION					SBG_VERSION_SOFTWARE(SBG_COMMON_LIB_VERSION_MAJOR,SBG_COMMON_LIB_VERSION_MINOR,SBG_COMMON_LIB_VERSION_BUILD,SBG_COMMON_LIB_VERSION_QUALIFIER)

/*
 * Specific macros for RC file.
 */
#ifndef SBG_STR
#define SBG_STR(X)								#X
#endif

#ifndef SBG_ASSTR
#define SBG_ASSTR(X)							SBG_STR(X)
#endif

#define SBG_COMMON_LIB_VERSION_W				SBG_COMMON_LIB_VERSION_MAJOR,SBG_COMMON_LIB_VERSION_MINOR,SBG_COMMON_LIB_VERSION_BUILD
#define SBG_COMMON_LIB_VERSION_R				SBG_ASSTR(SBG_COMMON_LIB_VERSION_MAJOR) "." SBG_ASSTR(SBG_COMMON_LIB_VERSION_MINOR) "." SBG_ASSTR(SBG_COMMON_LIB_VERSION_BUILD) "-stable\0"

#endif /* SBG_COMMON_LIB_VERSION_H */
