#pragma once

#include "Defines.h"

#include "Paragraph.h"
#include "ChpxFkp.h"
#include "PapxFkp.h"
#include "PlcBteChpx.h"
#include "PlcBtePapx.h"
#include "Clx.h"

#include "Table.h"
#include "DefTableShdOperand.h"
#include "DefTableShd80Operand.h"
#include "CellRangeVertAlign.h"
#include "Shd80.h"
#include "SttbfFfn.h"
#include "SttbfBkmk.h"
#include "LID.h"
#include "STSH.h"
#include "LSPD.h"
#include "ListFormattingInformation.h"
#include "PlfLfo.h"
#include "List.h"
#include "PlcfSed.h"
#include "Plcfbkf.h"
#include "SectionProperties.h"
#include "YAS.h"
#include "PositionCodeOperand.h"
#include "PChgTabs.h"

#include "Run.h"
#include "Hyperlink.h"
#include "BookmarkStart.h"
#include "BookmarkEnd.h"
#include "InlineShape.h"

#include "DopBase.h"
#include "PlcfSpa.h"

#include "ShapeBuilder.h"
#include "WordStreamWriter.h"

namespace AVSDocFileFormat
{
	typedef PLC<EmptyPLCData> Plcfhdd;

	class CDocFile
	{
	public:
		CDocFile();
		~CDocFile();

		HRESULT SaveToFile (const CString& sFileName);

		void AddTextItem( const ITextItem& textItem );
		void AddFootnote( const Footnote& footnote );
		void AddEndnote( const Endnote& endnote );
		void AddSectionProperties( const SectionProperties& _sectionProperties );
		wstring GetMainDocumentText() const;
		unsigned long GetMainDocumentTextItemsCount() const;
		wstring GetAllText() const;
		unsigned long GetAllTextSize() const;
		unsigned long GetAllTextItemsCount() const;

		inline void SetFontTable(const SttbfFfn& oFontTable)
		{
			m_oFontTable = oFontTable;
		}

		inline void SetListFormattingInformation (const ListFormattingInformation& oFormatInfo)
		{
			listFormatInfo = oFormatInfo;
		}

		inline void SetListFormattingOverrideInformation (const PlfLfo& oInfo)
		{
			listFormatOverrideInfo = oInfo;
		}

		inline void SetStyleSheet (const STSH& oStyleSheet)
		{
			m_oStyleSheet = oStyleSheet;
		}

		inline const STSH& GetStyleSheet() const
		{
			return m_oStyleSheet;
		}

	protected:

		CP OffsetToCP( unsigned int offset ) const;

		long WriteInformationBlock ();
		long WriteDocument();
		long WriteFontTable();
		long WriteStyleSheet();
		long WriteNumbering();

		void WriteSectionProperties();
		long WriteMainDocument(unsigned int* _textPosition, vector<CP>* _AllFootnotesReferences, vector<CP>* _AllEndnotesReferences);
		void AddFldCharsData(unsigned int nTextPos, const TextItem& oItem, map<CP, Fld>* cpFldMap);
		void AddHyperlinksData(unsigned int nTextPos, const TextItem& oItem, map<CP, Fld>* cpFldMap);
		void AddInlineShapesData(unsigned int nTextPos, const TextItem& oItem, map<CP, Fld>* cpFldMap);

		long WriteMainDocumentFields(const map<CP, Fld>& _cpFldMap);
		long WriteFootnoteDocumentFields(const map<CP, Fld>& _cpFldMap);
		long WriteEndnoteDocumentFields(const map<CP, Fld>& _cpFldMap);
		long WriteHeadersAndFootersDocumentFields(const map<CP, Fld>& _cpFldMap);
		long WriteBookmarks(const vector<pair<CP, wstring>>& _bookmarksStartsCPsWithIDs, const vector<pair<CP, wstring>>& _bookmarksEndsCPsWithIDs, const vector<wstring>& _bookmarksNames);
		short GetBookmarkIndexByID(const wstring& _id, const vector<pair<CP, wstring>>& _bookmarksCPsWithIDs);
		long WriteFootnoteDocument(unsigned int* _textPosition, vector<CP>* _AllFootnotesReferences);
		long WriteEndnoteDocument(unsigned int* _textPosition, vector<CP>* _AllEndnotesReferences);
		wstring GetHeadersOrFootersProperties(const ITextItem* _headerOrFooter, unsigned int& _headersOrFootersOffset, unsigned int* _headerOrFooterOffset, map<CP, Fld>* cpFldMap);
		long WriteHeadersAndFootersDocument(unsigned int* _textPosition);

		// 		
		int WriteFibRgLw97();

		int GetFibRgLw97_ccpText();					//	колличество всех символов обычного текста
		int GetFibRgLw97_ccpTxbx();					//	колличество всех символов текстовых надписей в документе
		int GetFibRgLw97_ccpHdrTxbx ();				//	колличество всех символов текстовых надписей в колонтитулах
		
		// OFFICE ART

		int WriteFibMainTBRefs();
		int WriteFibHeadTBRefs();
		
		int CalculateOffTbRefs (long nBuffPos);		//	Индексация символов
		int WriteStrTbRefs (long nBuffPos);			//	Запись символов текста в основной поток 

		void CalculateMainSpa ();					// настройка позиций в тексте для якорей автофигур или flow картинок (MAINDOC)
		void CalculateHeaderSpa ();					// настройка позиций в тексте для якорей автофигур или flow картинок (HEADER)

		long WriteOfficeDrawings ();				// запись всей информации по автофигурам и flow картинкам

		// Helpers

		bool InitStream(const CString& stName, IStream*& pStream, bool bDefaultSizes);
		long Write (IStream* stream, unsigned long position, const void* data, ULONG size, ULONG* writtenSize);
		long Write (IStream* stream, unsigned long position, const void* data, ULONG size);
		long ReloadFromFileBuffer (CString strFileData, DWORD dwOffTbID, DWORD dwSizefTbID);
		long ReloadStreamFileBuffer (CString strFileData, IStream* pStream);

	private:

		IStorage*				m_pIStorage;
		
		IStream*				m_pTableStream;
		IStream*				m_pSummaryInformationStream;
		IStream*				m_pDocumentSummaryInformationStream;

		unsigned long			m_nBuffOk;
		unsigned int			m_nTextCurPos;

		STSH					m_oStyleSheet;

		int						ccpText;				//	FibRgLw97
		int						ccpFtn;					//	FibRgLw97
		int						ccpHdd;					//	FibRgLw97
		int						ccpAtn;					//	FibRgLw97
		int						ccpEdn;					//	FibRgLw97
		int						ccpTxbx;				//	FibRgLw97
		int						ccpHdrTxbx;				//	FibRgLw97

		vector <Chpx>			m_arChpx;
		vector <unsigned int>	m_arRunsOffsets;
		vector <PapxInFkp>		m_arPapxInFkp;
		vector <unsigned int>	m_arParagraphsOffSets;

		COArtStorage*			m_oartStorage;

		// позиции якорей в тексте документа (индексы символов)
		vector <unsigned int>	m_aSpaCP;
		vector <unsigned int>	m_aHeadSpaCP;

		vector <unsigned int>	m_arTxbxCP;
		vector <unsigned int>	m_arTxbxHdrCP;

		vector <unsigned int>	m_arTxbxBkdCP;
		vector <unsigned int>	m_arTxbxHdrBkdCP;
		
		//

		list<TextItem> textItems;
		list<TextItem> m_Footnotes;
		list<TextItem> endnotes;
		SttbfFfn m_oFontTable;
		ListFormattingInformation listFormatInfo;
		PlfLfo listFormatOverrideInfo;
		list<SectionProperties> sectionProperties;
	};
}