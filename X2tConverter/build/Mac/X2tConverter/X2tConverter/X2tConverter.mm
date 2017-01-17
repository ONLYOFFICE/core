/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#import "X2tConverter.h"

#include "cextracttools.h"
#include "ASCConverters.h"

#include <iostream>
#include <fstream>

static std::wstring nsstring_to_wstring(NSString* nsstring)
{
    NSStringEncoding encode = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingUTF32LE);
    NSData* data = [nsstring dataUsingEncoding:encode];
    return std::wstring((wchar_t*) data.bytes, data.length / sizeof(wchar_t));
}

@implementation X2tConverter

- (int)sdk_docx2doct_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::docx2doct_bin(from, to, temp, oInputParams);
}
- (int)sdk_docx2doct:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::docx2doct(from, to, temp, oInputParams);
}
- (int)sdk_doct_bin2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::doct_bin2docx(from, to, temp, bFromChanges, themeDir, oInputParams);
}
- (int)sdk_doct2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::doct2docx(from, to, temp, bFromChanges, themeDir, oInputParams);
}
- (int)sdk_xlsx2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::xlsx2xlst_bin(from, to, temp, oInputParams);
    
}
- (int)sdk_xlsx2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::xlsx2xlst(from, to, temp, oInputParams);
}
- (int)sdk_xlst_bin2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::xlst_bin2xlsx(from, to, temp, bFromChanges, themeDir, oInputParams);
}
- (int)sdk_xlst2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::xlst2xlsx(from, to, temp, bFromChanges, themeDir, oInputParams);
}
- (int)sdk_pptx2pptt_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::pptx2pptt_bin(from, to, temp, oInputParams);
}
- (int)sdk_pptx2pptt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::pptx2pptt(from, to, temp, oInputParams);
}
- (int)sdk_pptt_bin2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir{
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::pptt_bin2pptx(from, to, temp, bFromChanges, themeDir, oInputParams);
}
- (int)sdk_pptt2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir{
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    std::wstring themeDir = nsstring_to_wstring(nsThemeDir);
    bool bFromChanges = (bool)fromChanges.boolValue;
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::pptt2pptx(from, to, temp, bFromChanges, themeDir, oInputParams);
}
- (int)sdk_csv2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::csv2xlst(from, to, temp, oInputParams);
}
- (int)sdk_csv2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::csv2xlsx(from, to, temp, oInputParams);
}
- (int)sdk_xlst2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::xlst2csv(from, to, temp, oInputParams);
}
- (int)sdk_xlsx2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    std::wstring temp = nsstring_to_wstring(nsTemp);
    
    // TODO: extract xml options
    
    NExtractTools::InputParams oInputParams;
    oInputParams.m_sFontDir = new std::wstring(nsstring_to_wstring(nsFontPath));
    
    return NExtractTools::xlsx2csv(from, to, temp, oInputParams);
}

- (int)sdk_dir2zip:(NSString*)nsFrom nsTo:(NSString*)nsTo {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    
    return NExtractTools::dir2zip(from, to);
}
- (int)sdk_zip2dir:(NSString*)nsFrom nsTo:(NSString*)nsTo {
    std::wstring from = nsstring_to_wstring(nsFrom);
    std::wstring to = nsstring_to_wstring(nsTo);
    
    return NExtractTools::zip2dir(from, to);
}

@end
