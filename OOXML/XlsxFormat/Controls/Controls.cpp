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
#include "Controls.h"
#include "../../XlsbFormat/Biff12_unions/ACTIVEXCONTROLS.h"
#include "../../XlsbFormat/Biff12_records/ActiveX.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CControlPr::CControlPr()
		{
		}
		CControlPr::~CControlPr()
		{
		}
		void CControlPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CControlPr::toXML() const
		{
			return L"";
		}
		EElementType CControlPr::getType () const
		{
			return et_x_ControlPr;
		}
		void CControlPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<controlPr");
			WritingStringNullableAttrEncodeXmlString(L"altText", m_oAltText, *m_oAltText);
			WritingStringNullableAttrBool2(L"autoFill", m_oAutoFill);
			WritingStringNullableAttrBool2(L"autoLine", m_oAutoLine);
			WritingStringNullableAttrBool2(L"autoPict", m_oAutoPict);
			WritingStringNullableAttrBool2(L"dde", m_oDde);
			WritingStringNullableAttrBool2(L"defaultSize", m_oDefaultSize);
			WritingStringNullableAttrBool2(L"disabled", m_oDisabled);
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			WritingStringNullableAttrBool2(L"locked", m_oLocked);
			WritingStringNullableAttrEncodeXmlString(L"cf", m_oCf, *m_oCf);
			WritingStringNullableAttrEncodeXmlString(L"macro", m_oMacro, *m_oMacro);
			WritingStringNullableAttrEncodeXmlString(L"linkedCell", m_oLinkedCell, *m_oLinkedCell);
			WritingStringNullableAttrEncodeXmlString(L"listFillRange", m_oListFillRange, *m_oListFillRange);
			WritingStringNullableAttrBool2(L"recalcAlways", m_oRecalcAlways);
			WritingStringNullableAttrBool2(L"print", m_oPrint);
			WritingStringNullableAttrBool2(L"uiObject", m_oUiObject);
			writer.WriteString(L">");
			if (m_oAnchor.IsInit())
			{
				m_oAnchor->toXML(writer);
			}
			writer.WriteString(L"</controlPr>");
		}
		void CControlPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"anchor" == sName )
				{
					m_oAnchor = oReader;
				}
			}
		}
		void CControlPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"altText"),		m_oAltText )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"autoFill"),		m_oAutoFill )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"autoLine"),		m_oAutoLine )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"autoPict"),		m_oAutoPict )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"dde"),			m_oDde )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"defaultSize"),	m_oDefaultSize )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"disabled"),		m_oDisabled )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"r:id"),			m_oRid )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oRid )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"locked"),		m_oLocked )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"macro"),			m_oMacro )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"print"),			m_oPrint )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"uiObject"),		m_oUiObject )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"cf"),			m_oCf )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"linkedCell"),	m_oLinkedCell )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"listFillRange"),	m_oListFillRange )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"recalcAlways"),	m_oRecalcAlways )
			WritingElement_ReadAttributes_End( oReader )
		}

		CControl::CControl()
		{
		}
		CControl::~CControl()
		{
		}
		void CControl::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CControl::toXML() const
		{
			return _T("");
		}
		EElementType CControl::getType () const
		{
			return et_x_Control;
		}
		void CControl::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			bool bAlternateContent = m_oControlPr.IsInit();
			if (bAlternateContent)
			{
				writer.WriteString(L"<mc:AlternateContent xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\"><mc:Choice Requires=\"x14\">");
				toXML2(writer, true);
				writer.WriteString(L"</mc:Choice><mc:Fallback>");
				toXML2(writer, false);
				writer.WriteString(L"</mc:Fallback></mc:AlternateContent>");
			}
			else
			{
				toXML2(writer, true);
			}
		}
		void CControl::toXML2(NSStringUtils::CStringBuilder& writer, bool bControlPr) const
		{
			writer.WriteString(L"<control");
			WritingStringNullableAttrEncodeXmlString(L"name", m_oName, *m_oName);
			WritingStringNullableAttrEncodeXmlString(L"progId", m_oProgId, *m_oProgId);
			WritingStringNullableAttrString(L"dvAspect", m_oDvAspect, m_oDvAspect->ToString());
			WritingStringNullableAttrEncodeXmlString(L"link", m_oLink, *m_oLink);
			WritingStringNullableAttrString(L"oleUpdate", m_oOleUpdate, m_oOleUpdate->ToString());
			WritingStringNullableAttrBool2(L"autoLoad", m_oAutoLoad);
			WritingStringNullableAttrInt(L"shapeId", m_oShapeId, m_oShapeId->GetValue());
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			if (bControlPr && m_oControlPr.IsInit())
			{
				writer.WriteString(L">");
				m_oControlPr->toXML(writer);
				writer.WriteString(L"</control>");
			}
			else
			{
				writer.WriteString(L"/>");
			}
		}
		void CControl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"controlPr") == sName )
				{
					m_oControlPr = oReader;
				}
			}
		}
		void CControl::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		void CControl::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ActiveX*>(obj.get());
			if(ptr != nullptr)
			{
				if(ptr->shapeId > 0)
					m_oShapeId = ptr->shapeId;

				if(!ptr->strName.value().empty())
					m_oName = ptr->strName.value();

				if(!ptr->strRelID.value.value().empty())
					m_oRid = ptr->strRelID.value.value();
			}
		}
		void CControl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if	  ( oReader, (L"progId"),		m_oProgId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"name"),			m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"dvAspect"),		m_oDvAspect )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"link"),			m_oLink )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"oleUpdate"),	m_oOleUpdate )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"autoLoad"),		m_oAutoLoad )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"shapeId"),		m_oShapeId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"r:id"),			m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"), m_oRid )
			WritingElement_ReadAttributes_End( oReader )
		}

		CControls::CControls()
		{
		}
		void CControls::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CControls::toXML() const
		{
			return _T("");
		}
		EElementType CControls::getType () const
		{
			return et_x_Controls;
		}
		void CControls::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		CControls::~CControls()
		{
		}
		void CControls::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_mapControls.empty()) return;

			writer.WriteString(L"<controls>");
			for(std::map<unsigned int, nullable<CControl>>::const_iterator it = m_mapControls.begin(); it != m_mapControls.end(); it++)
			{
				if (it->second.IsInit())
					it->second->toXML(writer);
			}
			writer.WriteString(L"</controls>");
		}
		void CControls::read(XmlUtils::CXmlLiteReader& oReader, bool bOldVersion)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"control" == sName )
				{
					nullable<CControl> pControl(oReader);

					if(pControl->m_oShapeId.IsInit())
					{
						if (bOldVersion)
						{
							m_mapControlsAlternative[pControl->m_oShapeId->GetValue()] = pControl;
						}
						else
						{
							m_mapControls[pControl->m_oShapeId->GetValue()] = pControl;
						}
					}
				}
				else if ( L"AlternateContent" == sName )
				{
					int nSubDepth = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nSubDepth ) )
					{
						std::wstring sSubName = XmlUtils::GetNameNoNS(oReader.GetName());
						if ( L"Choice" == sSubName )
						{
							read(oReader, false);
						}
						else if ( L"Fallback" == sSubName )
						{
							read(oReader, true);
						}
					}
				}
			}
		}
		void CControls::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			read(oReader, false);
		}
		void CControls::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::ACTIVEXCONTROLS*>(obj.get());
			if(ptr != nullptr)
			{
				for(auto &activeX: ptr->m_arBrtActiveX)
				{
					nullable<CControl> pControl(activeX);

					if(pControl->m_oShapeId.IsInit())
					{
						m_mapControls[pControl->m_oShapeId->GetValue()] = pControl;
					}
				}
			}

		}

		CListItem::CListItem() {}
		CListItem::~CListItem() {}
		void CListItem::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CListItem::toXML() const
		{
			return L"";
		}
		EElementType CListItem::getType () const
		{
			return et_x_ListItem;
		}
		void CListItem::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<item");
				WritingStringNullableAttrEncodeXmlString(L"val", m_oVal, m_oVal.get());
			writer.WriteString(L"/>");
		}
		void CListItem::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );
			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CListItem::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if	( oReader, (L"val"), m_oVal )
			WritingElement_ReadAttributes_End( oReader )
		}

		CListItems::CListItems() {}
		CListItems::~CListItems() {}
		void CListItems::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CListItems::toXML() const
		{
			return L"";
		}
		EElementType CListItems::getType () const
		{
			return et_x_ListItems;
		}
		void CListItems::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_arrItems.empty()) return;

			writer.WriteString(L"<itemLst>");

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i].IsInit() )
				{
					m_arrItems[i]->toXML(writer);
				}
			}
			if(m_oExtLst.IsInit())
			{
				writer.WriteString(m_oExtLst->toXMLWithNS(L""));
			}
			writer.WriteString(L"</itemLst>");
		}
		void CListItems::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (L"item" == sName)
				{
					nullable<CListItem> oItem(oReader);

					if (oItem.IsInit())
						m_arrItems.push_back(oItem);
				}
				else if ( L"extLst" == sName )
					m_oExtLst = oReader;
			}
		}

		CFormControlPr::CFormControlPr()
		{
		}
		CFormControlPr::~CFormControlPr()
		{
		}
		void CFormControlPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFormControlPr::toXML() const
		{
			return L"";
		}
		EElementType CFormControlPr::getType () const
		{
			return et_x_FormControlPr;
		}
		void CFormControlPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<formControlPr xmlns=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"");

			WritingStringNullableAttrBool2(L"firstButton",		m_oFirstButton);
			WritingStringNullableAttrString(L"objectType",		m_oObjectType, m_oObjectType->ToString());
			WritingStringNullableAttrString(L"checked",			m_oChecked, m_oChecked->ToString());
			WritingStringNullableAttrString(L"dropStyle",		m_oDropStyle, m_oDropStyle->ToString());
			WritingStringNullableAttrInt(L"dx",					m_oDx, m_oDx->GetValue());
			WritingStringNullableAttrInt(L"dropLines",			m_oDropLines, m_oDropLines->GetValue());
			WritingStringNullableAttrInt(L"inc",				m_oInc, m_oInc->GetValue());
			WritingStringNullableAttrInt(L"min",				m_oMin, m_oMin->GetValue());
			WritingStringNullableAttrInt(L"max",				m_oMax, m_oMax->GetValue());
			WritingStringNullableAttrInt(L"page",				m_oPage, m_oPage->GetValue());
			WritingStringNullableAttrInt(L"sel",				m_oSel, m_oSel->GetValue());
			WritingStringNullableAttrString(L"selType",			m_oSelType, m_oSelType->ToString());
			WritingStringNullableAttrString(L"textHAlign",		m_oTextHAlign, m_oTextHAlign->ToString());
			WritingStringNullableAttrString(L"textVAlign",		m_oTextVAlign, m_oTextVAlign->ToString());
			WritingStringNullableAttrInt(L"val",				m_oVal, *m_oVal);
			WritingStringNullableAttrInt(L"widthMin",			m_oWidthMin, m_oWidthMin->GetValue());
			WritingStringNullableAttrString(L"editVal",			m_oEditVal, m_oEditVal->ToString());
			WritingStringNullableAttrEncodeXmlString(L"multiSel",	m_oMultiSel, *m_oMultiSel);
			WritingStringNullableAttrEncodeXmlString(L"fmlaGroup",	m_oFmlaGroup, *m_oFmlaGroup);
			WritingStringNullableAttrEncodeXmlString(L"fmlaLink",	m_oFmlaLink, *m_oFmlaLink);
			WritingStringNullableAttrEncodeXmlString(L"fmlaRange",	m_oFmlaRange, *m_oFmlaRange);
			WritingStringNullableAttrEncodeXmlString(L"fmlaTxbx",	m_oFmlaTxbx, *m_oFmlaTxbx);
			WritingStringNullableAttrBool2(L"colored",			m_oColored);
			WritingStringNullableAttrBool2(L"horiz",			m_oHoriz);
			WritingStringNullableAttrBool2(L"justLastX",		m_oJustLastX);
			WritingStringNullableAttrBool2(L"lockText",			m_oLockText);
			WritingStringNullableAttrBool2(L"noThreeD",			m_oNoThreeD);
			WritingStringNullableAttrBool2(L"noThreeD2",		m_oNoThreeD2);
			WritingStringNullableAttrBool2(L"multiLine",		m_oMultiLine);
			WritingStringNullableAttrBool2(L"verticalBar",		m_oVerticalBar);
			WritingStringNullableAttrBool2(L"passwordEdit",		m_oPasswordEdit);

			writer.WriteString(L">");

			if(m_oItemLst.IsInit())
			{
				m_oItemLst->toXML(writer);
			}
			if(m_oExtLst.IsInit())
			{
				writer.WriteString(m_oExtLst->toXMLWithNS(L""));
			}
			writer.WriteString(L"</formControlPr>");
		}
		void CFormControlPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"itemLst" == sName )
					m_oItemLst = oReader;
				else if ( L"extLst" == sName )
					m_oExtLst = oReader;
			}
		}
		void CFormControlPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, L"dx",			m_oDx )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"dropStyle",	m_oDropStyle )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"dropLines",	m_oDropLines )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"objectType",	m_oObjectType )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"checked",		m_oChecked )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"inc",			m_oInc )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"min",			m_oMin )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"max",			m_oMax )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"page",			m_oPage )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"sel",			m_oSel )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"selType",		m_oSelType )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"textHAlign",	m_oTextHAlign )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"textVAlign",	m_oTextVAlign )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"val",			m_oVal )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"widthMin",		m_oWidthMin )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"editVal",		m_oEditVal )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"colored",		m_oColored )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"firstButton",	m_oFirstButton )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"horiz",		m_oHoriz )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"justLastX",	m_oJustLastX )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"lockText",		m_oLockText )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"multiSel",		m_oMultiSel )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"noThreeD",		m_oNoThreeD )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"noThreeD2",	m_oNoThreeD2 )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"multiLine",	m_oMultiLine )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"verticalBar",	m_oVerticalBar )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"passwordEdit",	m_oPasswordEdit )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"fmlaGroup",	m_oFmlaGroup )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"fmlaLink",		m_oFmlaLink )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"fmlaRange",	m_oFmlaRange )
				WritingElement_ReadAttributes_Read_else_if	( oReader, L"fmlaTxbx",		m_oFmlaTxbx )
			WritingElement_ReadAttributes_End( oReader )
		}

		CCtrlPropFile::CCtrlPropFile(OOX::Document* pMain) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
		}
		CCtrlPropFile::CCtrlPropFile(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::FileGlobalEnumerated(pMain), OOX::IFileContainer(pMain)
		{
			m_bSpreadsheets = true;
			read( oRootPath, oPath );
		}
		CCtrlPropFile::~CCtrlPropFile()
		{
		}
		void CCtrlPropFile::read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		const OOX::FileType CCtrlPropFile::type() const
		{
			return OOX::Spreadsheet::FileTypes::CtrlProp;
		}
		const CPath CCtrlPropFile::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		const CPath CCtrlPropFile::DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& CCtrlPropFile::GetReadPath()
		{
			return m_oReadPath;
		}
		void CCtrlPropFile::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;
			IFileContainer::Read( oRootPath, oPath );

			XmlUtils::CXmlLiteReader oReader;

			if ( !oReader.FromFile( oPath.GetPath() ) )
				return;

			if ( !oReader.ReadNextNode() )
				return;

			m_oFormControlPr = oReader;
		}
		void CCtrlPropFile::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			if(false == m_oFormControlPr.IsInit()) return;

			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");
			m_oFormControlPr->toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath, sXml.GetData());

			oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
	} //Spreadsheet
} // namespace OOX

