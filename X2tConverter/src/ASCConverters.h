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
#ifndef ASCCONVERTERS_H
#define ASCCONVERTERS_H

#ifndef X2T_USE_DYNAMIC_LIBRARY
#define X2T_DECL_EXPORT
#else
#include "../../DesktopEditor/common/base_export.h"
#define X2T_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <string>
#include "../../OOXML/Base/Base.h"

namespace NExtractTools
{
class InputParams;
class InputParamsMailMerge;
}
namespace NExtractTools
{
    _UINT32 docx2doct_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 docx2doct			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 docx_dir2doct		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sDocxFile);
	_UINT32 docx_dir2doct_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sDocxFile);
	_UINT32 doct_bin2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 doct_bin2docx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 doct2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);

    _UINT32 docxflat2doct		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 docxflat2doct_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 docxflat2odt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 docxflat2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

	_UINT32 package2ooxml		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 package2ooxml_dir	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 package2bin_t		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 package2bin			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

	_UINT32 dotm2docm			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 dotm2docm_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	_UINT32 dotx2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 dotx2docx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 docm2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 docm2docx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 dotm2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 dotm2docx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);

    _UINT32 xlsx2xlst			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsx2xlst_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xlsxflat2xlst		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsxflat2xlst_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsxflat2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsxflat2xlsx_dir	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsx_dir2xlst		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, bool bXmlOptions, const std::wstring &sXlsxFile);
	_UINT32 xlsx_dir2xlst_bin	(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params, bool bXmlOptions, const std::wstring &sXlsxFile);
	_UINT32 xlst_bin2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 xlst_bin2xlsx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 xlst2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);
   
	_UINT32 xltx2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xltx2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 xltm2xlsm			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xltm2xlsm_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	_UINT32 xlsm2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xlsm2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	_UINT32 xltm2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xltm2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	_UINT32 xlsb2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    _UINT32 pptx2pptt_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 pptx_dir2pptt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sPptxFile);
	_UINT32 pptx_dir2pptt_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params, const std::wstring &sPptxFile);
    _UINT32 pptx2pptt			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 pptt_bin2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 pptt_bin2pptx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 pptt2pptx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);

    _UINT32 csv2xlst		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 csv2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 csv2xlsx_dir	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 csv2xlst_bin    (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 xlst2csv		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsx_dir2csv	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 xlsx2csv		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xlst_bin2csv    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    _UINT32 bin2pdf			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params);

    _UINT32 ppsx2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppsx2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 potx2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 potx2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	_UINT32 ppsm2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppsm2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 potm2pptm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 potm2pptm_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	_UINT32 ppsm2pptm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppsm2pptm_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 pptm2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 pptm2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    _UINT32 potm2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 potm2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);

    _UINT32 ppt2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppt2pptx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppt2pptt_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppt2pptt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppt2pptm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 ppt2pptm_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    _UINT32 rtf2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 rtf2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 rtf2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 rtf2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 docx2rtf		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 docx_dir2rtf    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	_UINT32 doct2rtf        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);
	_UINT32 doct_bin2rtf    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sThemeDir, InputParams& params);

    _UINT32 doc2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 doc2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 doc2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 doc2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 docx_dir2doc    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 doc2docm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 doc2docm_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    _UINT32 xls2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xls2xlsx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xls2xlst        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xls2xlst_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xls2xlsm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 xls2xlsm_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    _UINT32 txt2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 txt2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 txt2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 txt2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 docx_dir2txt    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    _UINT32 odf2oox         (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 odf2oox_dir     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 odf2oot         (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 odf2oot_bin     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

    _UINT32 odf_flat2oox	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 odf_flat2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 odf_flat2oot	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 odf_flat2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

	_UINT32 docx2odt        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 docx_dir2odt    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params, bool bTemplate);
    _UINT32 xlsx2ods        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 xlsx_dir2ods    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params, bool bTemplate);
    _UINT32 pptx2odp        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 pptx_dir2odp    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params, bool bTemplate);

	_UINT32 fromMscrypt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 fromMitcrypt	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 mscrypt2oox		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 mitcrypt2oox	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 mscrypt2oot		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 mscrypt2oot_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 mitcrypt2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	
	_UINT32 oox2mscrypt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

	_UINT32 msVbaProject2dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
		
	_UINT32 html2doct_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	_UINT32 html2doct		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    _UINT32 html2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

//-------------------------------------------------------------------------------------------------------------------------------------------------
    _UINT32 dir2zip			(const std::wstring &sFrom, const std::wstring &sTo, bool bSorted = false, int method = 8/*Z_DEFLATED*/, short level = -1, bool bDateTime = false);
	_UINT32 dir2zipMscrypt  (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    _UINT32 zip2dir			(const std::wstring &sFrom, const std::wstring &sTo);

    _UINT32 convertmailmerge (const InputParamsMailMerge& oMailMergeSend,const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params);

	_UINT32 fromDocxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sDocxFile);
	_UINT32 fromDoctBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params);
	_UINT32 fromDocument    (const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params);

	_UINT32 fromXlsxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sXlsxFile);
	_UINT32 fromXlsbXlsxDir (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sXlsxFile);
	_UINT32 fromXlstBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params);
	_UINT32 fromSpreadsheet (const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params);

	_UINT32 fromPptxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params, const std::wstring &sPptxFile);
	_UINT32 fromPpttBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params);
	_UINT32 fromPresentation(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params);

	_UINT32 fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params);
	_UINT32 fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params);
	_UINT32 fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bPaid, InputParams& params);

	_UINT32 fromInputParams(InputParams& oInputParams);
	_UINT32 detectMacroInFile(InputParams& oInputParams);

	void createJSCaches();
	
	X2T_DECL_EXPORT _UINT32 FromFile(const std::wstring& file);
	X2T_DECL_EXPORT _UINT32 FromXml(const std::wstring& xml);
}
#endif // ASCCONVERTERS_H
