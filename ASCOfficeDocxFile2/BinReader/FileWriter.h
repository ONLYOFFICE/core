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
#ifndef FILE_WRITER
#define FILE_WRITER

#include "../../DesktopEditor/common/Path.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/VbaProject.h"

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
#include "CustormXmlWriter.h"

namespace BinDocxRW 
{
	class CComments;
}

namespace NSBinPptxRW
{
	class CDrawingConverter;
}
namespace OOX
{
	class CApp;
	class CCore;
}

namespace Writers
{
	class FileWriter
	{
	public:		
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
		DefaultThemeWriter		m_oTheme;
		CustomXmlWriter			m_oCustomXmlWriter;
		
		smart_ptr<OOX::VbaProject>		m_pVbaProject;
	
 		NSBinPptxRW::CDrawingConverter* m_pDrawingConverter;
		bool							m_bSaveChartAsImg;
		std::wstring					m_sThemePath;
		int								m_nDocPrIndex;
		BinDocxRW::CComments*			m_pComments;
		OOX::CApp*						m_pApp;
		OOX::CCore*						m_pCore;

		FileWriter (std::wstring sDirOutput,std::wstring sFontDir, bool bNoFontDir, int nVersion, bool bSaveChartAsImg, NSBinPptxRW::CDrawingConverter* pDrawingConverter, std::wstring sThemePath)
									:	m_pDrawingConverter(pDrawingConverter), m_sThemePath(sThemePath), m_bSaveChartAsImg(bSaveChartAsImg),
										m_oFontTableWriter		(sDirOutput, sFontDir, bNoFontDir),
										m_oHeaderFooterWriter	(sDirOutput),
										m_oFootnotesWriter		(sDirOutput),
										m_oEndnotesWriter		(sDirOutput),
										m_oMediaWriter			(sDirOutput),
										m_oStylesWriter			(sDirOutput, nVersion),
										m_oNumberingWriter		(sDirOutput),
										m_oDocumentWriter		(sDirOutput, m_oHeaderFooterWriter),
										m_oSettingWriter		(sDirOutput, m_oHeaderFooterWriter),
										m_oCommentsWriter		(sDirOutput),
										m_oChartWriter			(sDirOutput),
										m_oDocumentRelsWriter	(sDirOutput),
										m_oWebSettingsWriter	(sDirOutput),
										m_nDocPrIndex(0),
										m_pComments(NULL),
										m_oCustomXmlWriter		(sDirOutput, pDrawingConverter),
										m_pApp					(NULL),
										m_pCore					(NULL)
		{
		}
		~FileWriter()
		{
			RELEASEOBJECT(m_pApp);
			RELEASEOBJECT(m_pCore);
		}
		int getNextDocPr()
		{
			m_nDocPrIndex++;
			return m_nDocPrIndex;
		}

		void Write()
		{
			m_oCommentsWriter.Write();
			m_oChartWriter.Write();
			m_oStylesWriter.Write();
			m_oNumberingWriter.Write();
			m_oFontTableWriter.Write();
			m_oHeaderFooterWriter.Write();
			m_oFootnotesWriter.Write();
			m_oEndnotesWriter.Write();
	//Setting пишем после HeaderFooter, чтобы заполнить evenAndOddHeaders
			m_oSettingWriter.Write();
			m_oWebSettingsWriter.Write();
	//Document пишем после HeaderFooter, чтобы заполнить sectPr
			m_oDocumentWriter.Write();
	//Rels и ContentTypes пишем в конце
			m_oDocumentRelsWriter.Write();
		}
	};
}
#endif	// #ifndef FILE_WRITER
