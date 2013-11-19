#pragma once

#include "DocFile.h"

#include "Docx.h"
#include "Parse.h"
#include "Unit.h"

#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

#include "InternalElements.h"
#include "TableUtils.h"

#pragma comment(lib, "Utility.lib")
#pragma comment(lib, "Common.lib")
#pragma comment(lib, "DocxFormat.lib")
#pragma comment(lib, "XML.lib")
#pragma comment(lib, "OfficeSvmFile.lib")

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
			for (vector<AVSDocFileFormat::Prl>::const_iterator iter = m_source->begin(); iter != m_source->end(); ++iter)
			{
				RemovePropertyByCode(iter->GetSprmCode());
				m_dest->push_back(*iter);
			}    
		}

		inline void RemovePropertyByCode(unsigned short sprm)
		{
			for (vector<AVSDocFileFormat::Prl>::iterator iter = m_dest->begin(); iter != m_dest->end(); ++iter)
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

		long Convert(const WCHAR* ooxFolder, const WCHAR* docFile, const ProgressCallback* ffCallBack);

	private:

		BOOL UpdateProgress (const ProgressCallback* ffCallBack, long nComplete);

		void ConvertDocument (const OOX::Document& oXmlDoc);
		void ConvertContent  (const vector<OOX::Logic::TextItem>& oXmlItems);
		template<class T> AVSDocFileFormat::Paragraph ConvertParagraph (const OOX::Logic::Paragraph& oXmlParagraph);

		// Document
		string GetFontNameByThemeName(const string& themeName);
		vector<AVSDocFileFormat::XAS> BuildXASs(const OOX::Logic::Table& docxTable, const vector<OOX::Logic::TableCell>& tableCells);

		BOOL ValidParagraph (const OOX::Logic::Paragraph& oXmlParagraph);
		template<class T> AVSDocFileFormat::Hyperlink ConvertHyperlink( const OOX::Logic::Hyperlink& docxHyperlink, const PrlList& styleDocRunProperties);
		template<class T> void UpdateItemByCondition( T* docItem, const wstring& condition);

		const PrlList ConvertRunProperties (const OOX::Logic::RunProperty& docxRunProperties);
		const PrlList ConvertParagraphProperties( const OOX::Logic::ParagraphProperty& docxParagraphProperties);
		PrlList ConvertTableProperties( const OOX::Logic::TableProperty& docxTableProperties, unsigned int cellsCount = 0);

		AVSDocFileFormat::Constants::VerticalMergeFlag ConvertTableVerticalMergeFlag( const nullable_property<OOX::Logic::VMerge>& vMerge);

		template<class T> void ConvertFldSimple( const OOX::Logic::FldSimple& fldSimpleDocx, const PrlList& styleDocRunProperties, AVSDocFileFormat::Paragraph& docParagraph, wstring& runType);

		// настройки страниц документа

		AVSDocFileFormat::SectionProperties ConvertSectionProperties (const OOX::Logic::SectorProperty& oSection);
		bool AddLineNumberingSettings (const OOX::Logic::SectorProperty& oSection, PrlList& arrSettings);

		// верхний колонтитул
		AVSDocFileFormat::Header	ConvertHeader (const OOX::Header& oXmlDOCXHeader);
		// нижний колонтитул
		AVSDocFileFormat::Footer	ConvertFooter (const OOX::Footer& oXmlDOCXFooter);

		// Table

		template<class T> AVSDocFileFormat::Table CreateTable (const OOX::Logic::Table& oXmlTable);
		const PrlList ConvertTableRowCellsProperties(const OOX::Logic::Table& oXmlTable, int nY, int nCY, const vector<OOX::Logic::TableCell>& docxTableCells, const OOX::Styles::Style& oStyle);
		const PrlList ConvertTableRowProperties(int nY, const OOX::Logic::TableRowProperties& oXmlRow);

		// Fonts

		void AddInternalFont (const std::string& strFontName);
		void ConvertFontTable (const OOX::FontTable& oXmlFontTable);
		AVSDocFileFormat::PANOSE ConvertPanose (const std::string& strPanose);

		// Styles

		PrlList GetParagraphPropertiesFromStyleHierarchy (const string& styleID, PrlList* styleDocRunLinkProperties);
		PrlList GetRunPropertiesFromStyleHierarchy (const string& styleID);
		PrlList GetTablePropertiesFromStyleHierarchy (const string& styleID);

		void ConvertStyleSheet (const OOX::Styles& oStyleSheet);
		vector<AVSDocFileFormat::LSD> ConvertLatentStyles (/*const OOX::Styles::LattentStyles& latentStyles*/);
		vector<AVSDocFileFormat::LPStd> ConvertStyleDefinitions (const vector<OOX::Styles::Style>& arrStyles);


		std::string GetStyleID (const OOX::Logic::Paragraph& oXmlParagraph);

		// Numbering

		const vector<AVSDocFileFormat::LSTF> ConvertAbstractNums (const vector<OOX::Numbering::AbstractNum>& arrNums);
		const vector<AVSDocFileFormat::LVL> ConvertLVLs (const OOX::Numbering& oXmlNumbering, const vector<OOX::Numbering::AbstractNum>& _abstractNums);
		const AVSDocFileFormat::LVL ConvertLVL (const OOX::Numbering::Level& _level);
		const AVSDocFileFormat::Xst ConvertLvlText (const string& _lvlText, AVSDocFileFormat::Constants::MSONFC _nfc);
		void ConvertNumbering (const OOX::Numbering& _numbering);
		const AVSDocFileFormat::PlfLfo ConvertNums (const vector<OOX::Numbering::Num>& _nums);
		int FindAbstractNumIdWithStyleRef (const OOX::Numbering& oXmlNumbering, const std::string& refLink);

		// run rule transform
		template<class T> AVSDocFileFormat::Run ConvertRun(const OOX::Logic::Run& oXml, const PrlList& styleDocRunProperties, std::wstring& strRunType);
		template<class T> BOOL TextXmlRunTransform (const OOX::Logic::Text& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL FldCharXmlRunTransform (const OOX::Logic::FldChar& oXml, AVSDocFileFormat::Run& oBinRun, std::wstring& strRunType);
		template<class T> BOOL InstrTextXmlRunTransform (const OOX::Logic::InstrText& oXml, AVSDocFileFormat::Run& oBinRun, std::wstring& strRunType);
		template<class T> BOOL DrawingXmlRunTransform (const OOX::Logic::Drawing& oDrawing, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL PictXmlRunTransform (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL DelTextXmlRunTransform (const OOX::Logic::DelText& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL SymbolXmlRunTransform (const OOX::Logic::Symbol& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL BreakXmlRunTransform (const OOX::Logic::Break& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL TabXmlRunTransform (const OOX::Logic::Tab& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL FootnoteReferenceXmlRunTransform (const OOX::Logic::FootnoteReference& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL FootnoteRefXmlRunTransform (const OOX::Logic::FootnoteRef& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL EndnoteReferenceXmlRunTransform (const OOX::Logic::EndnoteReference& oXml, AVSDocFileFormat::Run& oBinRun);
		template<class T> BOOL EndnoteRefXmlRunTransform (const OOX::Logic::EndnoteRef& oXml, AVSDocFileFormat::Run& oBinRun);

		// Images

		template <class T, class TXmlImage> OOX::Image* GetImageDOCX (const TXmlImage* pXmlImage);
		template <class T> BOOL CreateImage (const OOX::Logic::Pict& oPicture, AVSDocFileFormat::Run& oBinRun);
		template <class T> OOX::Image* GetImageWithId(const OOX::RId& nID);
		template<class T> BOOL CreateTextureFillShape (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun);

		// Group

		template<class T> AVSDocFileFormat::COArtGroup* CreateGroup(const OOX::Logic::Group& oXmlGroup);
		template<class T> BOOL PictGroupXmlTransform (const OOX::Logic::Pict& oXml, AVSDocFileFormat::Run& oBinRun);


		// TextBox

		BOOL BuildContentTbRef (const OOX::Logic::Pict& oXml);
		BOOL TransformTb (const std::vector<OOX::Logic::TextItem>& oXmlItems, std::vector<AVSDocFileFormat::TextItem>& oTextItems);

	private:

		std::map<std::string, short>	m_mapFontTableMap;
		std::vector<std::string>		m_arrInternalFonts;

		map<string, AVSDocFileFormat::LID> lidMap;
		map<string, AVSDocFileFormat::StyleID> predefinedStyleIDMap;
		std::map<string, short>			m_mapStyleSheetMap;

		map<string, AVSDocFileFormat::Constants::StyleType> styleTypeMap;
		map<string, AVSDocFileFormat::Constants::MSONFC> numFmtMap;
		map<int, int> idLsidMap;
		map<int, short> idIndexMap;
		map<string, byte> kulMap;
		map<string, AVSDocFileFormat::Constants::SBkcOperand> sectionBreakTypeMap;
		map<string, byte> verticalPositionCodeMap;
		map<string, byte> horizontalPositionCodeMap;
		map<string, byte> textFrameWrappingMap;
		map<string, AVSDocFileFormat::Constants::Fts> tableCellWidthMap;
		map<string, AVSDocFileFormat::Constants::TabJC> customTabStopAlignment;
		map<string, AVSDocFileFormat::Constants::TabLC> customTabStopLeader;

		Docx::Folder inputFolder;
		AVSDocFileFormat::CDocFile*		m_pDOCFile;

		bool							m_bHaveSeparateFldChar;
		BOOL							m_bIsInlineShape;

		AVSDocFileFormat::COArtBuilder	m_oOArtBuilder;	// Office Drawing Binary File Format - Builder
		AVSDocFileFormat::InlineShape	m_oInlineShape;

		BOOL	m_bIsHaveRunPr;
	};
}