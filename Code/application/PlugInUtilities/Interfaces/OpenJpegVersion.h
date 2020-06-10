/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */
#ifndef OPENJPEGVERSION_H
#define OPENJPEGVERSION_H

/// Define an integer preprocessor OPJ_VERSION_NUMBER

#include <openjpeg.h>
// Opticks Dependencies/64/include/openjpeg/opj_config.h doesn't provide
// version numbers. But it's pkg-config.pc claims it is 2.0.0
// Otherwise, newer openjpeg versions do provide version numbers.
#ifndef OPJ_VERSION_MAJOR
#define OPJ_VERSION_MAJOR 2
#endif
#ifndef OPJ_VERSION_MINOR
#define OPJ_VERSION_MINOR 0
#endif
#ifndef OPJ_VERSION_BUILD
#define OPJ_VERSION_BUILD 0
#endif
#ifndef OPJ_VERSION_NUMBER
#define OPJ_VERSION_NUMBER (100*100*(OPJ_VERSION_MAJOR) + 100*(OPJ_VERSION_MINOR) + (OPJ_VERSION_BUILD))
#endif

#endif
