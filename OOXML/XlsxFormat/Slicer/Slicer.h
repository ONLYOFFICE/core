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


namespace OOX
{
	namespace Spreadsheet
	{
		class CSlicer : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSlicer)
            WritingElement_XlsbConstructors(CSlicer)
			CSlicer(){}
			virtual ~CSlicer(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
            void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_Slicer;
			}
			//Attributes
			nullable_string m_oName;
			nullable_string m_oUid;
			nullable_string m_oCache;
			nullable_string m_oCaption;
			nullable_uint m_oStartItem;//0
			nullable_uint m_oColumnCount;//1
			nullable_bool m_oShowCaption;//True
			nullable_uint m_oLevel;//0
			nullable_string m_oStyle;
			nullable_bool m_oLockedPosition;//False
			nullable_uint m_oRowHeight;
			//Members
			nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
		};

		class CSlicers : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CSlicers)
            WritingElement_XlsbConstructors(CSlicers)
			CSlicers(){}
			virtual ~CSlicers(){}
			virtual void fromXML(XmlUtils::CXmlNode& node){}
			virtual std::wstring toXML() const{return L"";}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const{}
			virtual void toXML(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
            void fromBin(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual EElementType getType() const
			{
				return et_x_Slicers;
			}
			//Members
			std::vector<CSlicer> m_oSlicer;
		};

		class CSlicerFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CSlicerFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;
			}
			CSlicerFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
			{
				m_bSpreadsheets = true;

				read( oRootPath, oPath );
			}
            void readBin(const CPath& oPath);
			virtual void read(const CPath& oPath)
			{
				//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
				CPath oRootPath;
				read(oRootPath, oPath);
			}
			virtual void read(const CPath& oRootPath, const CPath& oPath);
			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const
			{
				return OOX::Spreadsheet::FileTypes::Slicer;
			}
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
			nullable<CSlicers> m_oSlicers;
		private:

			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		};

	} //Spreadsheet
} // namespace OOX
