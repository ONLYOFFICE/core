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
#include "../Drawing/FromTo.h"
#include "../Ole/OleObjects.h"

namespace OOX
{
	namespace Spreadsheet
	{
//------------------------------------------------------------------------------------------------------------------------------
		class CListItem : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CListItem)
			
			CListItem();
			virtual ~CListItem();
			
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable_string	m_oVal;
		};
//-----------------------------------------------------------------------------------------------------------------------------
		class CListItems : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CListItems)
			
			CListItems();
			virtual ~CListItems();
			
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader){}
			
			std::vector<nullable<CListItem>> m_arrItems;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
		};
//-------------------------------------------------------------------------------------------------------------------------------
		class CFormControlPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFormControlPr)
			CFormControlPr();
			virtual ~CFormControlPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oDropLines;
			nullable<SimpleTypes::Spreadsheet::CObjectType>				m_oObjectType;
			nullable<SimpleTypes::Spreadsheet::CChecked>				m_oChecked;
			nullable<SimpleTypes::Spreadsheet::CDropStyle>				m_oDropStyle;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oDx;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oInc;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oMin;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oMax;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oPage;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oSel;
			nullable<SimpleTypes::Spreadsheet::CSelType>				m_oSelType;
			nullable<SimpleTypes::Spreadsheet::CHorizontalAlignment>	m_oTextHAlign;
			nullable<SimpleTypes::Spreadsheet::CVerticalAlignment>		m_oTextVAlign;
			nullable_int												m_oVal;
			nullable<SimpleTypes::CUnsignedDecimalNumber>				m_oWidthMin;
			nullable<SimpleTypes::Spreadsheet::CEditValidation>			m_oEditVal;
			nullable_string		m_oFmlaGroup;
			nullable_string		m_oFmlaLink;
			nullable_string		m_oFmlaRange;
			nullable_string		m_oFmlaTxbx;		
			nullable_bool		m_oColored;
			nullable_bool		m_oFirstButton;
			nullable_bool		m_oHoriz;
			nullable_bool		m_oJustLastX;
			nullable_bool		m_oLockText;
			nullable_string		m_oMultiSel;
			nullable_bool		m_oNoThreeD;
			nullable_bool		m_oNoThreeD2;
			nullable_bool		m_oMultiLine;
			nullable_bool		m_oVerticalBar;
			nullable_bool		m_oPasswordEdit;

			nullable<CListItems>							m_oItemLst;
			nullable<OOX::Drawing::COfficeArtExtensionList>	m_oExtLst;
//---------------------
			nullable_string									m_oText;
			nullable<SimpleTypes::CColorType>				m_oFillColor;
			nullable<SimpleTypes::CColorType>				m_oBorderColor;
		};
//------------------------------------------------------------------------------------
		class CControlPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CControlPr)
			CControlPr();
			virtual ~CControlPr();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string							m_oAltText;
			nullable_bool							m_oAutoFill;
			nullable_bool							m_oAutoLine;
			nullable_bool							m_oAutoPict;
			nullable_bool							m_oDde;
			nullable_bool							m_oDefaultSize;
			nullable_bool							m_oDisabled;
			nullable_string							m_oCf;
			nullable_string							m_oLinkedCell;
			nullable_string							m_oListFillRange;
			nullable<SimpleTypes::CRelationshipId>	m_oRid;
			nullable_bool							m_oLocked;
			nullable_string							m_oMacro;
			nullable_bool							m_oPrint;
			nullable_bool							m_oRecalcAlways;
			nullable_bool							m_oUiObject;

			nullable<CExtAnchor>					m_oAnchor;
		};
//------------------------------------------------------------------------------------
		class CControl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CControl)
			WritingElement_XlsbConstructors(CControl)
			CControl();
			virtual ~CControl();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, bool bControlPr) const;
   
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
					void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XLS::BaseObjectPtr& obj);
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			nullable_string										m_oName;
			nullable_string										m_oProgId;
			nullable<SimpleTypes::Spreadsheet::ST_DvAspect>		m_oDvAspect;
			nullable_string										m_oLink;
			nullable<SimpleTypes::Spreadsheet::ST_OleUpdate>	m_oOleUpdate;
			nullable_bool										m_oAutoLoad;
			nullable<SimpleTypes::CUnsignedDecimalNumber>		m_oShapeId;
			nullable<SimpleTypes::CRelationshipId>				m_oRid;
			nullable<CControlPr>								m_oControlPr;
//--for-write-from-bin--------------
			nullable<CFormControlPr>							m_oFormControlPr;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CControls : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CControls)
			WritingElement_XlsbConstructors(CControls)
			CControls();
			virtual ~CControls();

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual void toXML(NSStringUtils::CStringBuilder& writer) const;
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			
			void read(XmlUtils::CXmlLiteReader& oReader, bool bOldVersion = false);
			void fromBin(XLS::BaseObjectPtr& obj);

			virtual EElementType getType () const;

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:
			std::map<unsigned int, nullable<CControl>> m_mapControls;
			std::map<unsigned int, nullable<CControl>> m_mapControlsAlternative;
		};
//------------------------------------------------------------------------------------------------------------------------
		class CCtrlPropFile : public OOX::FileGlobalEnumerated, public OOX::IFileContainer
		{
		public:
			CCtrlPropFile(OOX::Document* pMain);
			CCtrlPropFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath);
			virtual ~CCtrlPropFile();

			virtual void read(const CPath& oPath);
			virtual void read(const CPath& oRootPath, const CPath& oPath);

			virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const;
			virtual const OOX::FileType type() const;

			virtual const CPath DefaultDirectory() const;
			virtual const CPath DefaultFileName() const;

			const CPath& GetReadPath();

			nullable<CFormControlPr> m_oFormControlPr;

		private:
			CPath m_oReadPath;
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
		};

	} //Spreadsheet
} // namespace OOX
