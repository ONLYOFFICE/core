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

#include "../CommonInclude.h"

#include "SheetData.h"
#include "Cols.h"
#include "Hyperlinks.h"
#include "MergeCells.h"
#include "WorksheetChildOther.h"
#include "ConditionalFormatting.h"
#include "DataValidation.h"

#include "../Drawing/Drawing.h"
#include "../Chart/Chart.h"
#include "../Table/Table.h"
#include "../Ole/OleObjects.h"
#include "../Controls/Controls.h"

namespace OOX
{
	class CVmlDrawing;

	namespace Spreadsheet
	{
		class CComments;
		class CCommentItem;
		class CLegacyDrawingWorksheet;
		class CThreadedComments;
		class CPersonList;
//необработанные child:
		//<customProperties>
		//<extLst>
		//<phoneticPr>
		//<scenarios>
		//<sheetCalcPr>
		//<smartTags>
		//<webPublishItems>

		class CWorksheet : public OOX::File, public OOX::IFileContainer, public WritingElement
		{
		public:
			CWorksheet(OOX::Document* pMain);
            CWorksheet(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath, const std::wstring & rId, bool isChartSheet = false);
			virtual ~CWorksheet();

            void readBin(const CPath& oPath);
			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			
			void toXMLStart(NSStringUtils::CStringBuilder& writer) const;
			void toXMLEnd(NSStringUtils::CStringBuilder& writer) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath() const;
			void ClearItems();

            const OOX::RId AddHyperlink (std::wstring& sHref);
			smart_ptr<OOX::WritingElement> FindVmlObject(const std::wstring &spid);

			void PrepareToWrite();
			void PrepareAfterRead();

			void ReadWorksheetOptions(XmlUtils::CXmlLiteReader& oReader);

			CPath	m_oReadPath;
			
			bool	m_bPrepareForBinaryWriter;
			bool	m_bWriteDirectlyToFile;
            bool    m_bIsChartSheet;
            mutable bool bIsWritten;

			nullable<OOX::Spreadsheet::CCols>						m_oCols;
			nullable<OOX::Spreadsheet::CDimension>					m_oDimension;
			nullable<OOX::Spreadsheet::CDrawingWorksheet>			m_oDrawing;
			nullable<OOX::Spreadsheet::CHyperlinks>					m_oHyperlinks;
			nullable<OOX::Spreadsheet::CMergeCells>					m_oMergeCells;
			nullable<OOX::Spreadsheet::CSheetData>					m_oSheetData;
			nullable<OOX::Spreadsheet::CSheetFormatPr>				m_oSheetFormatPr;
			nullable<OOX::Spreadsheet::CSheetViews>					m_oSheetViews;
			nullable<OOX::Spreadsheet::CPageMargins>				m_oPageMargins;
			nullable<OOX::Spreadsheet::CPageSetup>					m_oPageSetup;
			nullable<OOX::Spreadsheet::CPrintOptions>				m_oPrintOptions;
			nullable<OOX::Spreadsheet::CAutofilter>					m_oAutofilter;
			nullable<OOX::Spreadsheet::CTableParts>					m_oTableParts;
			nullable<OOX::Spreadsheet::CLegacyDrawingWorksheet>		m_oLegacyDrawing;
			nullable<OOX::Spreadsheet::COleObjects>					m_oOleObjects;
			nullable<OOX::Spreadsheet::CControls>					m_oControls;
			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;
			nullable<OOX::Spreadsheet::CSheetPr>					m_oSheetPr;
			nullable<OOX::Spreadsheet::CHeaderFooter>				m_oHeaderFooter;
			nullable<OOX::Spreadsheet::CLegacyDrawingHFWorksheet>	m_oLegacyDrawingHF;
			nullable<OOX::Spreadsheet::CPictureWorksheet>			m_oPicture;
			nullable<OOX::Spreadsheet::CRowColBreaks>				m_oRowBreaks;
			nullable<OOX::Spreadsheet::CRowColBreaks>				m_oColBreaks;
			nullable<OOX::Spreadsheet::CSheetProtection>			m_oSheetProtection;
			nullable<OOX::Spreadsheet::CDataValidations>			m_oDataValidations;
			nullable<OOX::Spreadsheet::CDataConsolidate>			m_oDataConsolidate;
			nullable<OOX::Spreadsheet::CSortState>					m_oSortState;
			nullable<OOX::Spreadsheet::CProtectedRanges>			m_oProtectedRanges;
			nullable<OOX::Spreadsheet::CCellWatches>				m_oCellWatches;

			nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;

//--------------------------------------------------------------------------------------------
			CComments			*m_pComments;
			CThreadedComments	*m_pThreadedComments;

			std::map<std::wstring, CConditionalFormattingRule*> m_mapConditionalFormattingEx;
//--------------------------------------------------------------------------------------------

			std::map<std::wstring, CCommentItem*> m_mapComments;
			std::map<std::wstring, unsigned int> m_mapStyleMerges2003;

		private:
			void PrepareDataValidations();
			void PrepareConditionalFormatting();
			void PrepareComments(OOX::Spreadsheet::CComments* pComments, OOX::Spreadsheet::CThreadedComments* pThreadedComments, OOX::Spreadsheet::CLegacyDrawingWorksheet* pLegacyDrawing);
		};

	} //Spreadsheet
} // namespace OOX

