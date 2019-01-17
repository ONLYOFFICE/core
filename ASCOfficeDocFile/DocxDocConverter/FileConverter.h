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
#pragma once

#include "DocFile.h"

#include "../../Common/DocxFormat/Source/DocxFormat/Docx.h"
//#include "../../Common/DocxFormat/Source/Utility/Parse.h"
#include "../../Common/DocxFormat/Source/Utility/Unit.h"

#include "InternalElements.h"
#include "TableUtils.h"

#include "../Common/Callback.h"

namespace DOCXTODOC
{
	class CPrCopier
	{
	public:
		CPrCopier(PrlList* dest, PrlList* source): m_dest(dest), m_source(source)
		{
			if ((NULL != m_dest) && (NULL != m_source))
			{
				AddOrReplaceProperties();
			}
		}

	private:
		inline void AddOrReplaceProperties()
		{
			for (vector<ASCDocFileFormat::Prl>::const_iterator iter = m_source->begin(); iter != m_source->end(); ++iter)
			{
				RemovePropertyByCode(iter->GetSprmCode());
				m_dest->push_back(*iter);
			}    
		}

		inline void RemovePropertyByCode(unsigned short sprm)
		{
			for (vector<ASCDocFileFormat::Prl>::iterator iter = m_dest->begin(); iter != m_dest->end(); ++iter)
			{
				if (iter->GetSprmCode() == sprm)
				{
					m_dest->erase(iter);

					RemovePropertyByCode(sprm);

					break;
				}
			}
		}

	private:

		PrlList* m_dest;
		PrlList* m_source;
	};

	class CFileTransformer
	{
	public:

		CFileTransformer ();
		~CFileTransformer();

		long Convert(const wchar_t* ooxFolder, const wchar_t* docFile, const ProgressCallback* ffCallBack);

	private:

		bool UpdateProgress (const ProgressCallback* ffCallBack, long nComplete);

		void ConvertDocument (const OOX::Document& oXmlDoc);
		void ConvertContent  (const vector<OOX::Logic::TextItem>& oItems);
		template<class T> ASCDocFileFormat::Paragraph ConvertParagraph (const OOX::Logic::Paragraph& oXmlParagraph);

		// Document
		std::wstring GetFontNameByThemeName(const std::wstring& themeName);
		vector<ASCDocFileFormat::XAS> BuildXASs(const OOX::Logic::Table& docxTable, const vector<OOX::Logic::TableCell>& tableCells);

		bool ValidParagraph (const OOX::Logic::Paragraph& oXmlParagraph);
		template<class T> ASCDocFileFormat::Hyperlink ConvertHyperlink( const OOX::Logic::Hyperlink& docxHyperlink, const PrlList& styleDocRunProperties);
		template<class T> void UpdateItemByCondition( T* docItem, const wstring& condition);

		const PrlList ConvertRunProperties (const OOX::Logic::RunProperty& docxRunProperties);
		const PrlList ConvertParagraphProperties( const OOX::Logic::ParagraphProperty& docxParagraphProperties);
		PrlList ConvertTableProperties( const OOX::Logic::TableProperty& docxTableProperties, unsigned int cellsCount = 0);

		ASCDocFileFormat::Constants::VerticalMergeFlag ConvertTableVerticalMergeFlag( const nullable<OOX::Logic::VMerge>& vMerge);

		template<class T> void ConvertFldSimple( const OOX::Logic::FldSimple& fldSimpleDocx, const PrlList& styleDocRunProperties, ASCDocFileFormat::Paragraph& docParagraph, wstring& runType);

		// настройки страниц документа

		ASCDocFileFormat::SectionProperties ConvertSectionProperties (const OOX::Logic::SectorProperty& oSection);
		bool AddLineNumberingSettings (const OOX::Logic::SectorProperty& oSection, PrlList& arrSettings);

		// верхний колонтитул
		ASCDocFileFormat::Header	ConvertHeader (const OOX::Header& oHeader);
		
		// нижний колонтитул
		ASCDocFileFormat::Footer	ConvertFooter (const OOX::Footer& oFooter);

		// Table

		template<class T> ASCDocFileFormat::Table CreateTable (const OOX::Logic::Table& oXmlTable);
		const PrlList ConvertTableRowCellsProperties(const OOX::Logic::Table& oXmlTable, int nY, int nCY, const vector<OOX::Logic::TableCell>& docxTableCells, const OOX::Styles::Style& oStyle);
		const PrlList ConvertTableRowProperties(int nY, const OOX::Logic::TableRowProperties& oXmlRow);

		// Fonts

		void AddInternalFont(const std::wstring& strFontName);
		void ConvertFontTable(const OOX::FontTable& oFontTable);
		ASCDocFileFormat::PANOSE ConvertPanose (const std::wstring& strPanose);

		// Styles

		PrlList GetParagraphPropertiesFromStyleHierarchy (const string& styleID, PrlList* styleDocRunLinkProperties);
		PrlList GetRunPropertiesFromStyleHierarchy (const string& styleID);
		PrlList GetTablePropertiesFromStyleHierarchy (const string& styleID);

		void ConvertStyleSheet (const OOX::Styles& oStyleSheet);
		vector<ASCDocFileFormat::LSD> ConvertLatentStyles (/*const OOX::Styles::LattentStyles& latentStyles*/);
		vector<ASCDocFileFormat::LPStd> ConvertStyleDefinitions (const vector<OOX::Styles::Style>& arrStyles);


		std::string GetStyleID (const OOX::Logic::Paragraph& oXmlParagraph);

		// Numbering

		const vector<ASCDocFileFormat::LSTF> ConvertAbstractNums (const vector<OOX::Numbering::AbstractNum>& arrNums);
		const vector<ASCDocFileFormat::LVL> ConvertLVLs (const OOX::Numbering& numbering, const vector<OOX::Numbering::AbstractNum>& _abstractNums);
		const ASCDocFileFormat::LVL ConvertLVL (const OOX::Numbering::Level& _level);
		const ASCDocFileFormat::Xst ConvertLvlText (const std::wstring& _lvlText, ASCDocFileFormat::Constants::MSONFC _nfc);
		void ConvertNumbering (const OOX::Numbering& _numbering);
		const ASCDocFileFormat::PlfLfo ConvertNums (const vector<OOX::Numbering::Num>& _nums);
		int FindAbstractNumIdWithStyleRef (const OOX::Numbering& numbering, const std::wstring& refLink);

		// run rule transform
		template<class T> ASCDocFileFormat::Run ConvertRun(const OOX::Logic::Run& oXml, const PrlList& styleDocRunProperties, std::wstring& strRunType);
		template<class T> bool TextXmlRunTransform (const OOX::Logic::Text& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool FldCharXmlRunTransform (const OOX::Logic::FldChar& oXml, ASCDocFileFormat::Run& oBinRun, std::wstring& strRunType);
		template<class T> bool InstrTextXmlRunTransform (const OOX::Logic::InstrText& oXml, ASCDocFileFormat::Run& oBinRun, std::wstring& strRunType);
		template<class T> bool DrawingXmlRunTransform (const OOX::Logic::Drawing& oDrawing, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool PictXmlRunTransform (const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool DelTextXmlRunTransform (const OOX::Logic::DelText& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool SymbolXmlRunTransform (const OOX::Logic::Symbol& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool BreakXmlRunTransform (const OOX::Logic::Break& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool TabXmlRunTransform (const OOX::Logic::Tab& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool FootnoteReferenceXmlRunTransform (const OOX::Logic::FootnoteReference& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool FootnoteRefXmlRunTransform (const OOX::Logic::FootnoteRef& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool EndnoteReferenceXmlRunTransform (const OOX::Logic::EndnoteReference& oXml, ASCDocFileFormat::Run& oBinRun);
		template<class T> bool EndnoteRefXmlRunTransform (const OOX::Logic::EndnoteRef& oXml, ASCDocFileFormat::Run& oBinRun);

		// Images

		template <class T, class TXmlImage> OOX::Image* GetImageDOCX (const TXmlImage* pXmlImage);
		template <class T> bool CreateImage (const OOX::Logic::Pict& oPicture, ASCDocFileFormat::Run& oBinRun);
		template <class T> OOX::Image* GetImageWithId(const OOX::RId& nID);
		template<class T> bool CreateTextureFillShape (const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun);

		// Group

		template<class T> ASCDocFileFormat::COArtGroup* CreateGroup(const OOX::Logic::Group& oXmlGroup);
		template<class T> bool PictGroupXmlTransform (const OOX::Logic::Pict& oXml, ASCDocFileFormat::Run& oBinRun);


		// TextBox

		bool BuildContentTbRef (const OOX::Logic::Pict& oXml);
		bool TransformTb (const std::vector<OOX::Logic::TextItem>& oXmlItems, std::vector<ASCDocFileFormat::TextItem>& oTextItems);

	private:

		std::map<std::wstring, short>	m_mapFontTableMap;
		std::vector<std::wstring>		m_arrInternalFonts;

		map<string, ASCDocFileFormat::LID> lidMap;
		map<string, ASCDocFileFormat::StyleID> predefinedStyleIDMap;
		std::map<string, short>			m_mapStyleSheetMap;

		map<string, ASCDocFileFormat::Constants::StyleType> styleTypeMap;
		map<string, ASCDocFileFormat::Constants::MSONFC> numFmtMap;
		map<int, int> idLsidMap;
		map<int, short> idIndexMap;
		map<string, unsigned char> kulMap;
		map<string, ASCDocFileFormat::Constants::SBkcOperand> sectionBreakTypeMap;
		map<string, unsigned char> verticalPositionCodeMap;
		map<string, unsigned char> horizontalPositionCodeMap;
		map<string, unsigned char> textFrameWrappingMap;
		map<string, ASCDocFileFormat::Constants::Fts> tableCellWidthMap;
		map<string, ASCDocFileFormat::Constants::TabJC> customTabStopAlignment;
		map<string, ASCDocFileFormat::Constants::TabLC> customTabStopLeader;

		Docx::Folder					m_docxInputFile;
		ASCDocFileFormat::CDocFile*		m_pDocFile;

		bool							m_bHaveSeparateFldChar;
		bool							m_bIsInlineShape;

		ASCDocFileFormat::COArtBuilder	m_oOArtBuilder;	// Office Drawing Binary File Format - Builder
		ASCDocFileFormat::InlineShape	m_oInlineShape;

		bool	m_bIsHaveRunPr;
	};
}