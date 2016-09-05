/*
 * (c) Copyright Ascensio System SIA 2010-2016
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

#include <string>

namespace NExtractTools
{
class InputParams;
class InputParamsMailMerge;
}
namespace NExtractTools
{
    int docx2doct_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
    int docx_dir2doct_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sFontPath);
    int docx2doct			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
    int doct_bin2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
    int doct_bin2docx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
	int doct2docx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);

    int xlsx2xlst_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions);
    int xlsx_dir2xlst_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sFontPath, const std::wstring &sXmlOptions);
    int xlsx2xlst			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions);
	int xlst_bin2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
    int xlst_bin2xlsx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
	int xlst2xlsx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);

    int pptx2pptt_bin		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
    int pptx_dir2pptt_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sFontPath);
    int pptx2pptt			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
	int pptt_bin2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
    int pptt_bin2pptx_dir	(const std::wstring &sFrom, const std::wstring &sToResult, const std::wstring &sTo, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
	int pptt2pptx			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);

    int csv2xlst		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);
    int csv2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);
    int csv2xlst_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & XmlOptions, const std::wstring &sFontPath);
    int xlst2csv		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);
    int xlsx2csv		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);
    int xlst_bin2csv    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);

    int bin2pdf			(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bPaid, const std::wstring &sThemeDir);

    int ppsx2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int ppsx2pptx_dir   (const std::wstring &sFrom, const std::wstring &sTo);

    int ppt2pptx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int ppt2pptx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int ppt2pptt_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
    int ppt2pptt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);

    int rtf2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int rtf2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int rtf2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
    int rtf2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);
    int docx2rtf		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int docx_dir2rtf    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp);
    int doct2rtf        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);
    int doct_bin2rtf    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bFromChanges, const std::wstring &sThemeDir, const InputParams& params);

    int doc2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const InputParams& params);
    int doc2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const InputParams& params);
    int doc2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sPassword, const std::wstring &sTemp, const std::wstring &sFontPath);
    int doc2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sPassword, const std::wstring &sTemp, const std::wstring &sFontPath);
    int docx_dir2doc    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath);

    int xls2xlsx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, const InputParams& params);
    int xls2xlsx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, const InputParams& params);
    int xls2xlst        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sPassword, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions);
    int xls2xlst_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sPassword, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions);

    int txt2docx		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp);
    int txt2docx_dir    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp);
    int txt2doct        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);
    int txt2doct_bin    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp, const std::wstring &sFontPath);
    int docx_dir2txt    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sXmlOptions, const std::wstring &sTemp);

    int odf2oox         (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);
    int odf2oox_dir     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);
    int odf2oot         (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);
    int odf2oot_bin     (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);

    int docx2odt        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);
    int docx_dir2odt    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);
    int xlsx2ods        (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);
    int xlsx_dir2ods    (const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath);

	int fromMscrypt		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath, InputParams& params);
	int mscrypt2oox		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath, const InputParams& params);
	int mscrypt2oot		(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath, InputParams& params);
	int mscrypt2oot_bin	(const std::wstring &sFrom, const std::wstring &sTo, const std::wstring & sTemp, const std::wstring &sFontPath, InputParams& params);

//-------------------------------------------------------------------------------------------------------------------------------------------------
    int dir2zip			(const std::wstring &sFrom, const std::wstring &sTo);
    int zip2dir			(const std::wstring &sFrom, const std::wstring &sTo);

    int convertmailmerge (const InputParamsMailMerge& oMailMergeSend,const std::wstring &sFrom, const std::wstring &sTo, const std::wstring &sTemp, const std::wstring &sFontPath, bool bPaid, const std::wstring &sThemeDir, const InputParams& params);

    int fromDocxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
    int fromDoctBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
	int fromDocument    (const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, const std::wstring &sXmlOptions, const InputParams& params);

    int fromXlsxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
    int fromXlstBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
	int fromSpreadsheet (const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, const std::wstring &sXmlOptions, const InputParams& params);

    int fromPptxDir     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
    int fromPpttBin     (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
	int fromPresentation(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTemp, const std::wstring &sXmlOptions, const InputParams& params);

    int fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
    int fromCrossPlatform(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid, const InputParams& params);
    int fromCanvasPdf(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, const std::wstring &sTemp, const std::wstring &sFontPath, const std::wstring &sXmlOptions, const std::wstring &sThemeDir, bool bFromChanges, bool bPaid);
}
#endif // ASCCONVERTERS_H
