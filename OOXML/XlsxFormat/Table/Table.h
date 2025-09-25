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
#pragma once

#include "Autofilter.h"
#include "../../DocxFormat/IFileContainer.h"
#include "../FileTypes_Spreadsheet.h"

namespace SimpleTypes
{
	namespace Spreadsheet
	{
		class CTableType;
		class CTotalsRowFunction;
		class CXmlDataType;
	}
}

namespace OOX
{
	namespace Drawing
	{
		class COfficeArtExtensionList;
	}

	namespace Spreadsheet
	{
		class CAltTextTable : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CAltTextTable)
            WritingElement_XlsbConstructors(CAltTextTable)
			CAltTextTable()
			{
			}
			virtual ~CAltTextTable()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
			virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType () const
			{
				return et_x_AltTextTable;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<std::wstring > m_oAltText;
			nullable<std::wstring > m_oAltTextSummary;
		};

		class CTableStyleInfo : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTableStyleInfo)
            WritingElement_XlsbConstructors(CTableStyleInfo)
			CTableStyleInfo()
			{
			}
			virtual ~CTableStyleInfo()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_TableStyleInfo;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<std::wstring > m_oName;
			nullable<SimpleTypes::COnOff> m_oShowColumnStripes;
			nullable<SimpleTypes::COnOff> m_oShowFirstColumn;
			nullable<SimpleTypes::COnOff> m_oShowLastColumn;
			nullable<SimpleTypes::COnOff> m_oShowRowStripes;
		};
		class CXmlColumnPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CXmlColumnPr)
			CXmlColumnPr() {}
			virtual ~CXmlColumnPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint mapId;
			nullable_string xpath;
			nullable_bool denormalized;
			nullable<SimpleTypes::Spreadsheet::CXmlDataType> xmlDataType;

			//ext
		};
		class CTableColumn : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTableColumn)
            WritingElement_XlsbConstructors(CTableColumn)
			CTableColumn() {}
			virtual ~CTableColumn() {}
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_TableColumn;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable_string m_oDataCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oDataDxfId;
			nullable_string m_oHeaderRowCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oHeaderRowDxfId;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oId;
			nullable_string m_oName;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oQueryTableFieldId;
			nullable_string m_oTotalsRowCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oTotalsRowDxfId;
			nullable<SimpleTypes::Spreadsheet::CTotalsRowFunction> m_oTotalsRowFunction;
			nullable_string m_oTotalsRowLabel;
			nullable_string m_oUniqueName;
			nullable_string m_oUid;

			nullable_string m_oTotalsRowFormula;
			nullable_string m_oCalculatedColumnFormula;
			nullable<CXmlColumnPr> m_oXmlColumnPr;
		//ext
		};

		class CTableColumns : public WritingElementWithChilds<CTableColumn>
		{
		public:
			WritingElement_AdditionMethods(CTableColumns)
            WritingElement_XlsbConstructors(CTableColumns)
			CTableColumns()
			{
			}
			virtual ~CTableColumns()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType () const
			{
				return et_x_TableColumns;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(std::vector<XLS::BaseObjectPtr>& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};

		class CTable : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTable)
            WritingElement_XlsbConstructors(CTable)
			CTable()
			{
			}
			virtual ~CTable()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, int nIndex);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
			virtual EElementType getType () const
			{
				return et_x_Table;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable_string										m_oComment;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oConnectionId;
			nullable_string										m_oDataCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oDataDxfId;
			nullable_string										m_oDisplayName;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oHeaderRowBorderDxfId;
			nullable_string										m_oHeaderRowCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oHeaderRowCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oHeaderRowDxfId;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oId;
			nullable_bool										m_oInsertRow;
			nullable_bool										m_oInsertRowShift;
			nullable_string										m_oName;
			nullable_bool										m_oPublished;
			nullable<SimpleTypes::CRelationshipId >				m_oRef;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oTableBorderDxfId;
			nullable<SimpleTypes::Spreadsheet::CTableType>		m_oTableType;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oTotalsRowBorderDxfId;
			nullable_string										m_oTotalsRowCellStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oTotalsRowCount;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oTotalsRowDxfId;
			nullable_bool										m_oTotalsRowShown;

			nullable<CAutofilter >								m_oAutoFilter;
			nullable<CSortState >								m_oSortState;
			nullable<CTableColumns >							m_oTableColumns;
			nullable<CTableStyleInfo >							m_oTableStyleInfo;

			nullable<OOX::Drawing::COfficeArtExtensionList>		m_oExtLst;
		};

		class CTablePart : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CTablePart)
            WritingElement_XlsbConstructors(CTablePart)
			CTablePart()
			{
			}
			virtual ~CTablePart()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			XLS::BaseObjectPtr toBin();
            void toBin(XLS::StreamCacheWriterPtr& writer);
			virtual EElementType getType () const
			{
				return et_x_TablePart;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CRelationshipId > m_oRId;
		};

		class CTableParts : public WritingElementWithChilds<CTablePart>
		{
		public:
			WritingElement_AdditionMethods(CTableParts)
            WritingElement_XlsbConstructors(CTableParts)
			CTableParts()
			{
			}
			virtual ~CTableParts()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
            void toBin(XLS::StreamCacheWriterPtr& writer);
			XLS::BaseObjectPtr toBin();

			virtual EElementType getType () const
			{
				return et_x_TableParts;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
            void ReadAttributes(XLS::BaseObjectPtr& obj);

		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber> m_oCount;
		};

		//необработанные child:
		//<extLst>
		class CTableFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CTableFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CTableFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
				read( oRootPath, oPath );
			}
			virtual ~CTableFile()
			{
			}
            void readBin(const CPath& oPath);
			XLS::BaseObjectPtr WriteBin() const;
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;
			virtual const CPath DefaultDirectory() const
			{
				return type().DefaultDirectory();
			}
			virtual const CPath DefaultFileName() const
			{
				return type().DefaultFileName();
			}
			const CPath& GetReadPath()
			{
				return m_oReadPath;
			}
			nullable<CTable> m_oTable;
		private:
			CPath m_oReadPath;
		};
//---------------------------------------------------------------------------------------------------------------------------------
		class CXmlPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CXmlPr)
			CXmlPr() {}
			virtual ~CXmlPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint mapId;
			nullable_string xpath;
			nullable<SimpleTypes::Spreadsheet::CXmlDataType> xmlDataType;

			//ext
		};
		class CXmlCellPr : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CXmlCellPr)
			CXmlCellPr() {}
			virtual ~CXmlCellPr() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint id;
			nullable_string uniqueName;

			nullable<CXmlPr> xmlPr;
			//ext
		};
		class CSingleXmlCell : public WritingElement
		{
		public:
			WritingElement_AdditionMethods(CSingleXmlCell)
			CSingleXmlCell() {}
			virtual ~CSingleXmlCell() {}

			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual std::wstring toXML() const { return L""; }
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const;

			nullable_uint connectionId;
			nullable_uint id;
			nullable_string r; //ref

			nullable<CXmlCellPr> xmlCellPr;
			//ext
		};
		class CSingleXmlCells : public WritingElementWithChilds<CSingleXmlCell>
		{
		public:
			WritingElement_AdditionMethods(CSingleXmlCells)
			CSingleXmlCells() {}
			virtual ~CSingleXmlCells() {}

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const {}
			virtual std::wstring toXML() const { return L""; }
			virtual void fromXML(XmlUtils::CXmlNode& node) {}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual EElementType getType() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
		};
		class CTableSingleCellsFile : public OOX::File
		{
		public:
			CTableSingleCellsFile(OOX::Document* pMain);
			CTableSingleCellsFile(OOX::Document* pMain, const CPath& uri);
			CTableSingleCellsFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CTableSingleCellsFile();

			virtual void read(const CPath& oFilePath);
			virtual void read(const CPath& oRootPath, const CPath& oFilePath);
			virtual void write(const CPath& oFilePath, const CPath& oDirectory, CContentTypes& oContent) const;

			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;

			nullable<CSingleXmlCells> singleXmlCells;
		};
	} //Spreadsheet
} // namespace OOX
