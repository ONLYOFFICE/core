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
#ifndef FILE_WRITER
#define FILE_WRITER

#include "NumberingWriter.h"
#include "fontTableWriter.h"
#include "HeaderFooterWriter.h"
#include "DocumentWriter.h"
#include "MediaWriter.h"
#include "StylesWriter.h"
#include "SettingWriter.h"
#include "CommentsWriter.h"
#include "ChartWriter.h"
#include "DocumentRelsWriter.h"
#include "webSettingsWriter.h"
#include "DefaultThemeWriter.h"

namespace BinDocxRW {
	class CComments;
}
namespace Writers
{
	class FileWriter
	{
	public:
		NSBinPptxRW::CDrawingConverter* m_pDrawingConverter;
		CString m_sThemePath;
		bool m_bSaveChartAsImg;
		
		ContentTypesWriter		m_oContentTypesWriter;
		FontTableWriter			m_oFontTableWriter;
		DocumentWriter			m_oDocumentWriter;
		MediaWriter				m_oMediaWriter;
		StylesWriter			m_oStylesWriter;
		NumberingWriter			m_oNumberingWriter;
		HeaderFooterWriter		m_oHeaderFooterWriter;
		FootnotesWriter			m_oFootnotesWriter;
		EndnotesWriter			m_oEndnotesWriter;
		SettingWriter			m_oSettingWriter;
		CommentsWriter			m_oCommentsWriter;
		ChartWriter				m_oChartWriter;
		DocumentRelsWriter		m_oDocumentRelsWriter;
		WebSettingsWriter		m_oWebSettingsWriter;
		DefaultThemeWriter		m_oDefaultTheme;
		
		int m_nDocPrIndex;
		BinDocxRW::CComments* m_pComments;
	public:
		FileWriter(CString sDirOutput,CString sFontDir, int nVersion, bool bSaveChartAsImg, NSBinPptxRW::CDrawingConverter* pDrawingConverter, CString sThemePath):
										m_pDrawingConverter(pDrawingConverter),m_sThemePath(sThemePath),m_bSaveChartAsImg(bSaveChartAsImg),
										m_oContentTypesWriter(sDirOutput), m_oFontTableWriter(sDirOutput, sFontDir),
										m_oHeaderFooterWriter(sDirOutput, m_oContentTypesWriter),
										m_oFootnotesWriter(sDirOutput, m_oContentTypesWriter),
										m_oEndnotesWriter(sDirOutput, m_oContentTypesWriter),
										m_oMediaWriter(sDirOutput),
										m_oStylesWriter(sDirOutput, nVersion),
										m_oNumberingWriter(sDirOutput, m_oContentTypesWriter),
										m_oDocumentWriter(sDirOutput, m_oHeaderFooterWriter),
										m_oSettingWriter(sDirOutput, m_oHeaderFooterWriter),
										m_oCommentsWriter(sDirOutput, m_oContentTypesWriter),
										m_oChartWriter(sDirOutput, m_oContentTypesWriter),
										m_oDocumentRelsWriter(sDirOutput),
										m_oWebSettingsWriter(sDirOutput),
										m_nDocPrIndex(0),
										m_pComments(NULL)
		{
		}
	public: int getNextDocPr()
			{
				m_nDocPrIndex++;
				return m_nDocPrIndex;
			}
	};
}
#endif	// #ifndef FILE_WRITER
