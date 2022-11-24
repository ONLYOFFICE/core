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

#include "OleObjects.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CExtAnchor::CExtAnchor()
		{
		}
		CExtAnchor::~CExtAnchor()
		{
		}
		void CExtAnchor::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CExtAnchor::toXML() const
		{
			return (L"");
		}
		void CExtAnchor::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<anchor");
			WritingStringNullableAttrBool2(L"moveWithCells", m_oMoveWithCells);
			WritingStringNullableAttrBool2(L"sizeWithCells", m_oSizeWithCells);
			WritingStringNullableAttrInt(L"z-order", m_oZOrder, m_oZOrder->GetValue());
			writer.WriteString(L">");
			if (m_oFrom.IsInit())
			{
				m_oFrom->toXML2(writer, L"from");
			}
			if (m_oTo.IsInit())
			{
				m_oTo->toXML2(writer, L"to");
			}
			writer.WriteString(L"</anchor>");
		}
		void CExtAnchor::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"from") == sName )
				{
					m_oFrom = oReader;
				}
				else if ( (L"to") == sName )
				{
					m_oTo = oReader;
				}
			}
		}
		EElementType CExtAnchor::getType () const
		{
			return et_x_ExtAnchor;
		}
		void CExtAnchor::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if( oReader, (L"moveWithCells"), m_oMoveWithCells )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"sizeWithCells"), m_oSizeWithCells )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"z-order"), m_oZOrder )
			WritingElement_ReadAttributes_End( oReader )
		}

		COleObjectPr::COleObjectPr()
		{
		}
		COleObjectPr::~COleObjectPr()
		{
		}
		void COleObjectPr::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring COleObjectPr::toXML() const
		{
			return (L"");
		}
		void COleObjectPr::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<objectPr");
			WritingStringNullableAttrEncodeXmlString(L"altText", m_oAltText, m_oAltText.get());
			WritingStringNullableAttrBool(L"autoFill", m_oAutoFill);
			WritingStringNullableAttrBool(L"autoLine", m_oAutoLine);
			WritingStringNullableAttrBool(L"autoPict", m_oAutoPict);
			WritingStringNullableAttrBool(L"dde", m_oDde);
			WritingStringNullableAttrBool(L"defaultSize", m_oDefaultSize);
			WritingStringNullableAttrBool(L"disabled", m_oDisabled);
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			WritingStringNullableAttrBool(L"locked", m_oLocked);
			WritingStringNullableAttrEncodeXmlString(L"macro", m_oMacro, m_oMacro.get());
			WritingStringNullableAttrBool(L"print", m_oPrint);
			WritingStringNullableAttrBool(L"uiObject", m_oUiObject);
			writer.WriteString(L">");
			if (m_oAnchor.IsInit())
			{
				m_oAnchor->toXML(writer);
			}
			writer.WriteString(L"</objectPr>");
		}
		void COleObjectPr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"anchor") == sName )
				{
					m_oAnchor = oReader;
				}
			}
		}
		EElementType COleObjectPr::getType () const
		{
			return et_x_OleObjectPr;
		}
		void COleObjectPr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"locked"),			m_oLocked )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"macro"),			m_oMacro )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"print"),			m_oPrint )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"uiObject"),		m_oUiObject )
			WritingElement_ReadAttributes_End( oReader )
		}

		COleObject::COleObject()
		{
		}
		COleObject::~COleObject()
		{
		}
		void COleObject::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring COleObject::toXML() const
		{
			return (L"");
		}
		void COleObject::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			bool bAlternateContent = m_oObjectPr.IsInit();
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
		void COleObject::toXML2(NSStringUtils::CStringBuilder& writer, bool bObjectPr) const
		{
			writer.WriteString(L"<oleObject");
			WritingStringNullableAttrEncodeXmlString(L"progId", m_oProgId, m_oProgId.get());
			WritingStringNullableAttrString(L"dvAspect", m_oDvAspect, m_oDvAspect->ToString());
			WritingStringNullableAttrEncodeXmlString(L"link", m_oLink, m_oLink.get());
			WritingStringNullableAttrString(L"oleUpdate", m_oOleUpdate, m_oOleUpdate->ToString());
			WritingStringNullableAttrBool(L"autoLoad", m_oAutoLoad);
			WritingStringNullableAttrInt(L"shapeId", m_oShapeId, m_oShapeId->GetValue());
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			if (bObjectPr && m_oObjectPr.IsInit())
			{
				writer.WriteString(L">");
				m_oObjectPr->toXML(writer);
				writer.WriteString(L"</oleObject>");
			}
			else
			{
				writer.WriteString(L"/>");
			}
		}
		void COleObject::toXMLPptx(NSStringUtils::CStringBuilder& writer, std::wstring qqq)
		{
			std::wstring sRoot;
			writer.WriteString(L"<o:OLEObject");
			if(m_oDvAspect.IsInit())
			{
				writer.WriteString(L" DrawAspect=\"");
				if(SimpleTypes::Spreadsheet::Content == m_oDvAspect->GetValue())
					writer.WriteString(L"Content");
				else
					writer.WriteString(L"Icon");
				writer.WriteString(L"\"");
			}
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			WritingStringNullableAttrEncodeXmlString(L"ProgID", m_oProgId, m_oProgId.get());
			WritingStringNullableAttrInt(L"ShapeID", m_oShapeId, m_oShapeId->GetValue());
			writer.WriteString(L" Type=\"Embed\"");
			if(m_oOleUpdate.IsInit())
			{
				writer.WriteString(L" UpdateMode=\"");
				if(SimpleTypes::Spreadsheet::Always == m_oOleUpdate->GetValue())
					writer.WriteString(L"Always");
				else
					writer.WriteString(L"OnCall");
				writer.WriteString(L"\"");
			}
			if (m_OleObjectFile.IsInit())
			{
				if (m_OleObjectFile->isMsPackage())
					WritingStringAttrString(L"mspackage", L"true");

				WritingStringAttrEncodeXmlString(L"pathbin", m_OleObjectFile->filename().GetPath());
				WritingStringAttrEncodeXmlString(L"pathimg", m_OleObjectFile->filename_cache().GetPath());
			}
			writer.WriteString(L"/>");
		}
		void COleObject::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"objectPr") == sName )
				{
					m_oObjectPr = oReader;
				}
			}
		}
		void COleObject::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType COleObject::getType () const
		{
			return et_x_OleObject;
		}
		void COleObject::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::OleObject*>(obj.get());
			if(ptr != nullptr)
			{
				if(ptr->dwAspect == 0x00000001)
					m_oDvAspect         = SimpleTypes::Spreadsheet::EDvAspect::Content;
				else if(ptr->dwAspect == 0x00000004)
					m_oDvAspect         = SimpleTypes::Spreadsheet::EDvAspect::Icon;

				if(ptr->dwOleUpdate == 0x00000001)
					m_oOleUpdate         = SimpleTypes::Spreadsheet::EOleUpdate::Always;
				else if(ptr->dwOleUpdate == 0x00000003)
					m_oOleUpdate         = SimpleTypes::Spreadsheet::EOleUpdate::OnCall;

				if(ptr->shapeId > 0)
					m_oShapeId = ptr->shapeId;

				m_oAutoLoad = ptr->fAutoLoad;

				if(!ptr->strProgID.value().empty())
					m_oProgId = ptr->strProgID.value();

				if(ptr->fLinked != 0 && !ptr->link.getAssembledFormula().empty())
					m_oLink = ptr->link.getAssembledFormula();

				if(ptr->fLinked == 0 && !ptr->strRelID.value.value().empty())
					m_oRid = ptr->strRelID.value.value();
			}
		}
		void COleObject::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, (L"progId"),		m_oProgId )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"dvAspect"),	m_oDvAspect )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"link"),		m_oLink )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"oleUpdate"),	m_oOleUpdate )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"autoLoad"),	m_oAutoLoad )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"shapeId"),	m_oShapeId )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"r:id"),		m_oRid )
				WritingElement_ReadAttributes_Read_else_if	( oReader, (L"relationships:id"), m_oRid )
			WritingElement_ReadAttributes_End( oReader )
		}

		COleObjects::COleObjects()
		{
		}
		COleObjects::~COleObjects()
		{
			for(boost::unordered_map<unsigned int, COleObject*>::const_iterator it = m_mapOleObjects.begin(); it != m_mapOleObjects.end(); ++it)
			{
				delete it->second;
			}
			m_mapOleObjects.clear();
		}
		void COleObjects::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring COleObjects::toXML() const
		{
			return (L"");
		}
		void COleObjects::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_mapOleObjects.size() > 0)
			{
				writer.WriteString(L"<oleObjects>");
				for(boost::unordered_map<unsigned int, COleObject*>::const_iterator it = m_mapOleObjects.begin(); it != m_mapOleObjects.end(); ++it)
				{
					it->second->toXML(writer);
				}
				writer.WriteString(L"</oleObjects>");
			}
		}
		void COleObjects::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"oleObject") == sName )
				{
					COleObject* pOleObject = new COleObject(oReader);
					if(pOleObject->m_oShapeId.IsInit())
					{
						m_mapOleObjects[pOleObject->m_oShapeId->GetValue()] = pOleObject;
					}
					else
					{
						delete pOleObject;
					}
				}
				else if ( (L"AlternateContent") == sName )
				{
					int nSubDepth = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nSubDepth ) )
					{
						std::wstring sSubName = XmlUtils::GetNameNoNS(oReader.GetName());
						if ( (L"Fallback") == sSubName || (L"Choice") == sSubName )
						{
							bool bFound = false;
							int nSubSubDepth = oReader.GetDepth();
							while( oReader.ReadNextSiblingNode( nSubSubDepth ) )
							{
								std::wstring sSubSubName = XmlUtils::GetNameNoNS(oReader.GetName());
								if ( (L"oleObject") == sSubSubName )
								{
									COleObject* pOleObject = new COleObject(oReader);
									if(pOleObject->m_oShapeId.IsInit())
									{
										m_mapOleObjects[pOleObject->m_oShapeId->GetValue()] = pOleObject;
										bFound = true;
										break;
									}
									else
									{
										delete pOleObject;
									}
								}
							}
							if (bFound)
							{
								break;
							}
						}
					}
				}
			}
		}
		void COleObjects::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::OLEOBJECTS*>(obj.get());
			if(ptr != nullptr)
			{
				for(auto &oleObject: ptr->m_arBrtOleObject)
				{
					COleObject* pOleObject = new COleObject(oleObject);
					if(pOleObject->m_oShapeId.IsInit())
					{
							m_mapOleObjects[pOleObject->m_oShapeId->GetValue()] = pOleObject;
					}
					else
					{
							delete pOleObject;
					}
				}
			}
		}
		EElementType COleObjects::getType () const
		{
			return et_x_OleObjects;
		}
		void COleObjects::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
