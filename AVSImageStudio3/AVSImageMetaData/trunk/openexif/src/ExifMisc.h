/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Open Exif Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2001 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Creation Date: 8/28/2002
 *
 * Original Author: 
 * Ricardo Rosario ricardo.rosario@kodak.com
 *
 * Contributor(s): 
 *
 * 
 */ 

#ifndef EXIF_MISC_H
#define EXIF_MISC_H

#include <stdio.h>
#include "ExifTypeDefs.h"
namespace Exif 
{

//! Copies file.
/*! \param overwriteIfExists determines whether or not to overwrite
           toPath file, if it already exists.
 \return true if success; false if failure.
*/
extern EXIF_DECL bool ExifCopyFile(const char * fromPath, const char * toPath,
                                   bool overwriteIfExists);
}
#endif  // PMT_MISC_H
