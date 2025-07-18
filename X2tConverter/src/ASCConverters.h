/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "../../OOXML/Base/Base.h"
#include <string>
#include <functional>

namespace NExtractTools
{
	class InputParams;
	class InputParamsMailMerge;
	class ConvertParams;
}

namespace NExtractTools
{
	#define DECLARE_CONVERT_FUNC(name) _UINT32 name(const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams)
	typedef std::function<_UINT32(const std::wstring&, const std::wstring&, InputParams&, ConvertParams&)> CONVERT_FUNC;

	// zip
	_UINT32 dir2zip(const std::wstring& sFrom, const std::wstring& sTo, bool bSorted = false, int method = 8 /*Z_DEFLATED*/, short level = -1, bool bDateTime = true);
	_UINT32 zip2dir(const std::wstring& sFrom, const std::wstring& sTo);
	DECLARE_CONVERT_FUNC(dir2zipMscrypt);

	// crypt
	DECLARE_CONVERT_FUNC(mscrypt2oot);
	DECLARE_CONVERT_FUNC(mscrypt2oot_bin);
	DECLARE_CONVERT_FUNC(mitcrypt2oox);
	DECLARE_CONVERT_FUNC(mitcrypt2oot_bin);
	DECLARE_CONVERT_FUNC(mscrypt2oox);
	DECLARE_CONVERT_FUNC(oox2mscrypt);
	DECLARE_CONVERT_FUNC(fromMscrypt);
	DECLARE_CONVERT_FUNC(fromMitcrypt);

	DECLARE_CONVERT_FUNC(msVbaProject2dir);

	// docx
	DECLARE_CONVERT_FUNC(docx2doct_bin);
	DECLARE_CONVERT_FUNC(docx_dir2doct_bin);
	DECLARE_CONVERT_FUNC(docx2doct);
	DECLARE_CONVERT_FUNC(docx_dir2doct);

	DECLARE_CONVERT_FUNC(doct_bin2docx);
	DECLARE_CONVERT_FUNC(doct_bin2docx_dir);
	DECLARE_CONVERT_FUNC(doct2docx);

	DECLARE_CONVERT_FUNC(docxflat2doct);
	DECLARE_CONVERT_FUNC(docxflat2doct_bin);
	DECLARE_CONVERT_FUNC(docxflat2odt);
	DECLARE_CONVERT_FUNC(docxflat2docx);

	DECLARE_CONVERT_FUNC(package2ooxml);
	DECLARE_CONVERT_FUNC(package2ooxml_dir);
	DECLARE_CONVERT_FUNC(package2bin_t);
	DECLARE_CONVERT_FUNC(package2bin);

	DECLARE_CONVERT_FUNC(dotm2docm);
	DECLARE_CONVERT_FUNC(dotm2docm_dir);
	DECLARE_CONVERT_FUNC(dotx2docx);
	DECLARE_CONVERT_FUNC(dotx2docx_dir);
	DECLARE_CONVERT_FUNC(docm2docx);
	DECLARE_CONVERT_FUNC(docm2docx_dir);
	DECLARE_CONVERT_FUNC(dotm2docx);
	DECLARE_CONVERT_FUNC(dotm2docx_dir);

	// xlsx
	DECLARE_CONVERT_FUNC(xlsx2xlst);
	DECLARE_CONVERT_FUNC(xlsx2xlst_bin);
	DECLARE_CONVERT_FUNC(xlsxflat2xlst);
	DECLARE_CONVERT_FUNC(xlsxflat2xlst_bin);
	DECLARE_CONVERT_FUNC(xlsxflat2xlsx);
	DECLARE_CONVERT_FUNC(xlsxflat2xlsx_dir);
	DECLARE_CONVERT_FUNC(xlsx_dir2xlst);
	DECLARE_CONVERT_FUNC(xlsx_dir2xlst_bin);
	DECLARE_CONVERT_FUNC(xlst_bin2xlsx);
	DECLARE_CONVERT_FUNC(xlst_bin2xlsx_dir);
	DECLARE_CONVERT_FUNC(xlst2xlsx);

	DECLARE_CONVERT_FUNC(xltx2xlsx);
	DECLARE_CONVERT_FUNC(xltx2xlsx_dir);
	DECLARE_CONVERT_FUNC(xltm2xlsm);
	DECLARE_CONVERT_FUNC(xltm2xlsm_dir);
	DECLARE_CONVERT_FUNC(xlsm2xlsx);
	DECLARE_CONVERT_FUNC(xlsm2xlsx_dir);
	DECLARE_CONVERT_FUNC(xltm2xlsx);
	DECLARE_CONVERT_FUNC(xltm2xlsx_dir);
	DECLARE_CONVERT_FUNC(xlsb2xlsx_dir);

	// pptx
	DECLARE_CONVERT_FUNC(pptx2pptt_bin);
	DECLARE_CONVERT_FUNC(pptx_dir2pptt);
	DECLARE_CONVERT_FUNC(pptx_dir2pptt_bin);
	DECLARE_CONVERT_FUNC(pptx2pptt);
	DECLARE_CONVERT_FUNC(pptt_bin2pptx);
	DECLARE_CONVERT_FUNC(pptt_bin2pptx_dir);
	DECLARE_CONVERT_FUNC(pptt2pptx);

	DECLARE_CONVERT_FUNC(ppsx2pptx);
	DECLARE_CONVERT_FUNC(ppsx2pptx_dir);
	DECLARE_CONVERT_FUNC(potx2pptx);
	DECLARE_CONVERT_FUNC(potx2pptx_dir);
	DECLARE_CONVERT_FUNC(ppsm2pptx);
	DECLARE_CONVERT_FUNC(ppsm2pptx_dir);
	DECLARE_CONVERT_FUNC(potm2pptm);
	DECLARE_CONVERT_FUNC(potm2pptm_dir);
	DECLARE_CONVERT_FUNC(ppsm2pptm);
	DECLARE_CONVERT_FUNC(ppsm2pptm_dir);
	DECLARE_CONVERT_FUNC(pptm2pptx);
	DECLARE_CONVERT_FUNC(pptm2pptx_dir);
	DECLARE_CONVERT_FUNC(potm2pptx);
	DECLARE_CONVERT_FUNC(potm2pptx_dir);

	// vsdx
	DECLARE_CONVERT_FUNC(vsdx2vsdt);
	DECLARE_CONVERT_FUNC(vsdx2vsdt_bin);
	DECLARE_CONVERT_FUNC(vsdx_dir2vsdt);
	DECLARE_CONVERT_FUNC(vsdx_dir2vsdt_bin);
	DECLARE_CONVERT_FUNC(vsdt_bin2vsdx);
	DECLARE_CONVERT_FUNC(vsdt_bin2vsdx_dir);
	DECLARE_CONVERT_FUNC(vsdt2vsdx);

	// doc
	DECLARE_CONVERT_FUNC(doc2docx);
	DECLARE_CONVERT_FUNC(doc2docx_dir);
	DECLARE_CONVERT_FUNC(doc2doct);
	DECLARE_CONVERT_FUNC(doc2doct_bin);
	DECLARE_CONVERT_FUNC(docx_dir2doc);
	DECLARE_CONVERT_FUNC(doc2docm);
	DECLARE_CONVERT_FUNC(doc2docm_dir);

	// rtf
	DECLARE_CONVERT_FUNC(rtf2docx_dir);
	DECLARE_CONVERT_FUNC(docx_dir2rtf);
	DECLARE_CONVERT_FUNC(rtf2doct_bin);
	DECLARE_CONVERT_FUNC(doct_bin2rtf);
	DECLARE_CONVERT_FUNC(rtf2docx);
	DECLARE_CONVERT_FUNC(docx2rtf);
	DECLARE_CONVERT_FUNC(rtf2doct);
	DECLARE_CONVERT_FUNC(doct2rtf);

	// txt
	DECLARE_CONVERT_FUNC(txt2docx_dir);
	DECLARE_CONVERT_FUNC(docx_dir2txt);
	DECLARE_CONVERT_FUNC(txt2doct_bin);
	DECLARE_CONVERT_FUNC(txt2docx);
	DECLARE_CONVERT_FUNC(txt2doct);
	DECLARE_CONVERT_FUNC(docx2txt);

	// html
	DECLARE_CONVERT_FUNC(html2doct_bin);
	DECLARE_CONVERT_FUNC(html_zip2doct_bin);
	DECLARE_CONVERT_FUNC(html_zip2doct);
	DECLARE_CONVERT_FUNC(html2doct);
	DECLARE_CONVERT_FUNC(html2docx);
	DECLARE_CONVERT_FUNC(html_zip2docx);

	DECLARE_CONVERT_FUNC(html2docx_dir);
	DECLARE_CONVERT_FUNC(html_zip2docx_dir);

	DECLARE_CONVERT_FUNC(doct_bin2html);
	DECLARE_CONVERT_FUNC(doct_bin2html_zip);

	// mht
	DECLARE_CONVERT_FUNC(mht2docx_dir);

	// epub
	DECLARE_CONVERT_FUNC(epub2docx_dir);
	DECLARE_CONVERT_FUNC(doct_bin2epub);

	// fb2
	DECLARE_CONVERT_FUNC(fb2docx_dir);
	DECLARE_CONVERT_FUNC(fb2docx);
	DECLARE_CONVERT_FUNC(doct_bin2fb);

	//md
	DECLARE_CONVERT_FUNC(md2html);

	// ppt
	DECLARE_CONVERT_FUNC(ppt2pptx);
	DECLARE_CONVERT_FUNC(ppt2pptx_dir);
	DECLARE_CONVERT_FUNC(ppt2pptt_bin);
	DECLARE_CONVERT_FUNC(ppt2pptt);
	DECLARE_CONVERT_FUNC(ppt2pptm);
	DECLARE_CONVERT_FUNC(ppt2pptm_dir);

	// csv
	DECLARE_CONVERT_FUNC(csv2xlst);
	DECLARE_CONVERT_FUNC(csv2xlsx);
	DECLARE_CONVERT_FUNC(csv2xlsx_dir);
	DECLARE_CONVERT_FUNC(csv2xlst_bin);
	DECLARE_CONVERT_FUNC(xlst2csv);
	DECLARE_CONVERT_FUNC(xlsx_dir2csv);
	DECLARE_CONVERT_FUNC(xlsx2csv);
	DECLARE_CONVERT_FUNC(xlst_bin2csv);

	// xls
	DECLARE_CONVERT_FUNC(xls2xlsx);
	DECLARE_CONVERT_FUNC(xls2xlsx_dir);
	DECLARE_CONVERT_FUNC(xls2xlst);
	DECLARE_CONVERT_FUNC(xls2xlst_bin);
	DECLARE_CONVERT_FUNC(xls2xlsm);
	DECLARE_CONVERT_FUNC(xls2xlsm_dir);

	// xml => xlsx
	DECLARE_CONVERT_FUNC(xml2xlsx);
	DECLARE_CONVERT_FUNC(xml2xlsx_dir);

	// odf
	DECLARE_CONVERT_FUNC(odf2oox);
	DECLARE_CONVERT_FUNC(odf2oox_dir);
	DECLARE_CONVERT_FUNC(odf2oot);
	DECLARE_CONVERT_FUNC(odf2oot_bin);

	DECLARE_CONVERT_FUNC(odf_flat2oox);
	DECLARE_CONVERT_FUNC(odf_flat2oox_dir);
	DECLARE_CONVERT_FUNC(odf_flat2oot);
	DECLARE_CONVERT_FUNC(odf_flat2oot_bin);

	DECLARE_CONVERT_FUNC(docx2odt);
	DECLARE_CONVERT_FUNC(docx_dir2odt);
	DECLARE_CONVERT_FUNC(xlsx2ods);
	DECLARE_CONVERT_FUNC(xlsx_dir2ods);
	DECLARE_CONVERT_FUNC(pptx2odp);
	DECLARE_CONVERT_FUNC(pptx_dir2odp);

	DECLARE_CONVERT_FUNC(otf2odf);

	// pdf/image
	DECLARE_CONVERT_FUNC(bin2pdf);
	_UINT32 bin2Image(unsigned char* pBuffer, long lBufferLen, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams);
	DECLARE_CONVERT_FUNC(bin2imageBase64);

	// using NSDoctRenderer::DoctRendererFormat::FormatFile
	// DECLARE_CONVERT_FUNC(doct_bin2pdf);
	// DECLARE_CONVERT_FUNC(doct_bin2image);

	DECLARE_CONVERT_FUNC(pdfoform2docx_dir);
	DECLARE_CONVERT_FUNC(docx_dir2pdfoform);
	DECLARE_CONVERT_FUNC(pdfoform2doct_bin);
	DECLARE_CONVERT_FUNC(doct_bin2pdfoform);
	DECLARE_CONVERT_FUNC(pdfoform2docx);
	DECLARE_CONVERT_FUNC(docx2pdfoform);
	DECLARE_CONVERT_FUNC(pdfoform2doct);
	DECLARE_CONVERT_FUNC(doct2pdfoform);

	// iWork
	DECLARE_CONVERT_FUNC(pages2odf);
	DECLARE_CONVERT_FUNC(numbers2odf);
	DECLARE_CONVERT_FUNC(key2odf);

	//HWP
	DECLARE_CONVERT_FUNC(hwp2docx);
	DECLARE_CONVERT_FUNC(hwp2docx_dir);

	//HWPX
	DECLARE_CONVERT_FUNC(hwpx2docx);
	DECLARE_CONVERT_FUNC(hwpx2docx_dir);

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	_UINT32 convertmailmerge(const InputParamsMailMerge& oMailMergeSend, const std::wstring& sFrom, const std::wstring& sTo, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromDocxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromDoctBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromDocument(const std::wstring &sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromXlsxDir    (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromXlsbXlsxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromXlstBin    (const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromSpreadsheet(const std::wstring &sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromPptxDir(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromPpttBin(const std::wstring &sFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromPresentation(const std::wstring &sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromT(const std::wstring &sFrom, int nFormatFrom, const std::wstring &sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromCrossPlatform(const std::wstring& sFrom, int nFormatFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromCanvasPdf(const std::wstring& sFrom, int nFormatFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromVsdtBin(const std::wstring& sFrom, const std::wstring& sTo, int nFormatTo, InputParams& params, ConvertParams& convertParams);
	_UINT32 fromDraw(const std::wstring& sFrom, int nFormatFrom, InputParams& params, ConvertParams& convertParams);

	_UINT32 fromInputParams(InputParams& oInputParams);
	_UINT32 detectMacroInFile(InputParams& oInputParams);

	void createJSCaches();
	void createJSSnapshots();

	X2T_DECL_EXPORT _UINT32 FromFile(const std::wstring& file);
	X2T_DECL_EXPORT _UINT32 FromXml(const std::wstring& xml);
}
#endif // ASCCONVERTERS_H
