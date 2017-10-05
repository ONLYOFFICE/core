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
#ifndef ASCCONVERTERS_H
#define ASCCONVERTERS_H

#ifndef X2T_USE_DYNAMIC_LIBRARY
#define X2T_DECL_EXPORT
#else
#include "../../DesktopEditor/common/base_export.h"
#define X2T_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <string>

namespace NExtractTools
{
class InputParams;
class InputParamsMailMerge;
}
namespace NExtractTools
{
    int docx2doct_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int docx_dir2doct_bin	(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int docx2doct			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doct_bin2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
    int doct_bin2docx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
	int doct2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);

    int dotm2docm			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int dotm2docm_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	int dotx2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int dotx2docx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int docm2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int docm2docx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int dotm2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int dotm2docx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);

	int xlsx2xlst_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	int xlsx_dir2xlst_bin	(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params, bool bXmlOptions, const std::wstring &sXlsxFile);
    int xlsx2xlst			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	int xlst_bin2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
    int xlst_bin2xlsx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
	int xlst2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
   
	int xltx2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xltx2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int xltm2xlsm			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xltm2xlsm_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	int xlsm2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xlsm2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	int xltm2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xltm2xlsx_dir		(const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);

    int pptx2pptt_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int pptx_dir2pptt_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int pptx2pptt			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
	int pptt_bin2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
    int pptt_bin2pptx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
	int pptt2pptx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);

    int csv2xlst		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int csv2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int csv2xlst_bin    (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int xlst2csv		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xlsx2csv		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xlst_bin2csv    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    int bin2pdf			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params);

    int ppsx2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppsx2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int potx2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int potx2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	int ppsm2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppsm2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int potm2pptm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int potm2pptm_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
	int ppsm2pptm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppsm2pptm_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int pptm2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int pptm2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);
    int potm2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int potm2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo, InputParams& params);

    int ppt2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppt2pptx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppt2pptt_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppt2pptt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppt2pptm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int ppt2pptm_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    int rtf2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int rtf2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int rtf2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int rtf2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int docx2rtf		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int docx_dir2rtf    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doct2rtf        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);
    int doct_bin2rtf    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bFromChanges, const std::wstring &sThemeDir, InputParams& params);

    int doc2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doc2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doc2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doc2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int docx_dir2doc    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doc2docm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int doc2docm_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    int xls2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xls2xlsx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xls2xlst        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xls2xlst_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xls2xlsm		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int xls2xlsm_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    int txt2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int txt2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int txt2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int txt2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);
    int docx_dir2txt    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, InputParams& params);

    int odf2oox         (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int odf2oox_dir     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int odf2oot         (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int odf2oot_bin     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

    int odf_flat2oox	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int odf_flat2oox_dir(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int odf_flat2oot	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int odf_flat2oot_bin(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

	int docx2odt        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int docx_dir2odt    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int xlsx2ods        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int xlsx_dir2ods    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int pptx2odp        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int pptx_dir2odp    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

	int fromMscrypt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	int mscrypt2oox		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	int mscrypt2oot		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	int mscrypt2oot_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	
	int oox2mscrypt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

	int html2doct_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
	int html2doct		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);
    int html2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, InputParams& params);

//-------------------------------------------------------------------------------------------------------------------------------------------------
    int dir2zip			(const std::wstring &sFrom, const std::wstring &sTo);
    int zip2dir			(const std::wstring &sFrom, const std::wstring &sTo);

    int convertmailmerge (const InputParamsMailMerge& oMailMergeSend,const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, bool bPaid, const std::wstring &sThemeDir, InputParams& params);

    int fromDocxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
    int fromDoctBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
	int fromDocument    (const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params);

	int fromXlsxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params, const std::wstring &sXlsxFile);
    int fromXlstBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
	int fromSpreadsheet (const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params);

    int fromPptxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
    int fromPpttBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
	int fromPresentation(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, InputParams& params);

    int fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
    int fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);
    int fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, InputParams& params);

	int fromInputParams(InputParams& oInputParams);
	
	X2T_DECL_EXPORT int FromFile(const std::wstring& file);
	X2T_DECL_EXPORT int FromXml(const std::wstring& xml);
}
#endif // ASCCONVERTERS_H
