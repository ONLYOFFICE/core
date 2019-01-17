/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
//
//  X2tConverter.h
//  X2tConverter
//
//  Created by alexey.musinov on 25.03.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface X2tConverter : NSObject

- (int)sdk_docx2doct_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_docx2doct:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_doct_bin2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;
- (int)sdk_doct2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;

- (int)sdk_xlsx2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlsx2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlst_bin2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;
- (int)sdk_xlst2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;

- (int)sdk_pptx2pptt_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_pptx2pptt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_pptt_bin2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;
- (int)sdk_pptt2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;

- (int)sdk_csv2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_csv2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlst2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlsx2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;

- (int)sdk_dir2zip:(NSString*)nsFrom nsTo:(NSString*)nsTo;
- (int)sdk_zip2dir:(NSString*)nsFrom nsTo:(NSString*)nsTo;

@end
