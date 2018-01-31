﻿/*
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
#pragma once
#ifndef OOX_WORKSHEET_FILE_INCLUDE_H_
#define OOX_WORKSHEET_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "SheetData.h"
#include "Cols.h"
#include "Hyperlinks.h"
#include "MergeCells.h"
#include "WorksheetChildOther.h"
#include "ConditionalFormatting.h"

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
//необработанные child:
		//<cellWatches>
		//<colBreaks>
		//<customProperties>
		//<dataConsolidate>
		//<dataValidations>
		//<extLst>
		//<phoneticPr>
		//<protectedRanges>
		//<rowBreaks>
		//<scenarios>
		//<sheetCalcPr>
		//<sheetProtection>
		//<smartTags>
		//<sortState>
		//<webPublishItems>

		class CWorksheet : public OOX::File, public OOX::IFileContainer
		{
		public:
			CWorksheet(OOX::Document* pMain);
			CWorksheet(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath, const std::wstring & rId);
			virtual ~CWorksheet();
			virtual void read(const CPath& oPath)
			{
				//don't use this. instead use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			void PrepareComments(OOX::Spreadsheet::CComments* pComments, OOX::CVmlDrawing* pVmlDrawing);
			void PrepareToWrite();
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Worksheet;
			}
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath() const
			{
				return m_oReadPath;
			}
            const OOX::RId AddHyperlink (std::wstring& sHref);
			void ClearItems();
			CPath	m_oReadPath;

		public:
			bool	m_bPrepareForBinaryWriter;

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
            boost::unordered_map<std::wstring, CCommentItem*>       m_mapComments;
			std::vector<OOX::Spreadsheet::CConditionalFormatting*>	m_arrConditionalFormatting;
			nullable<OOX::Spreadsheet::CSheetPr>					m_oSheetPr;
			nullable<OOX::Spreadsheet::CHeaderFooter>				m_oHeaderFooter;
			nullable<OOX::Spreadsheet::CLegacyDrawingHFWorksheet>	m_oLegacyDrawingHF;
			nullable<OOX::Spreadsheet::CPictureWorksheet>			m_oPicture;

			nullable<OOX::Drawing::COfficeArtExtensionList>			m_oExtLst;

//--------------------------------------------------------------------------------------------
			CComments												*m_pComments;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_WORKSHEET_FILE_INCLUDE_H_
