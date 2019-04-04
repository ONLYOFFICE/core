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
#ifndef OOX_OLEOBJECTS_FILE_INCLUDE_H_
#define OOX_OLEOBJECTS_FILE_INCLUDE_H_

#include "../CommonInclude.h"
#include "../Drawing/FromTo.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CExtAnchor : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CExtAnchor)
			CExtAnchor()
			{
			}
			virtual ~CExtAnchor()
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<anchor");
				WritingStringNullableAttrBool(L"moveWithCells", m_oMoveWithCells);
				WritingStringNullableAttrBool(L"sizeWithCells", m_oSizeWithCells);
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
			virtual void		 fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("from") == sName )
					{
						m_oFrom = oReader;
					}
					else if ( _T("to") == sName )
					{
						m_oTo = oReader;
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_ExtAnchor;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if	 ( oReader, _T("moveWithCells"),	  m_oMoveWithCells )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("sizeWithCells"),	  m_oSizeWithCells )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("z-order"),	  m_oZOrder )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<> > m_oMoveWithCells;
			nullable<SimpleTypes::COnOff<> > m_oSizeWithCells;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oZOrder;

			nullable<CFromTo > m_oFrom;
			nullable<CFromTo > m_oTo;
		};

		class COleObjectPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleObjectPr)
			COleObjectPr()
			{
			}
			virtual ~COleObjectPr()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("anchor") == sName )
					{
						m_oAnchor = oReader;
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_OleObjectPr;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("altText"),		m_oAltText )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("autoFill"),		m_oAutoFill )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("autoLine"),		m_oAutoLine )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("autoPict"),		m_oAutoPict )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("dde"),			m_oDde )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("defaultSize"),	m_oDefaultSize )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("disabled"),		m_oDisabled )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("r:id"),			m_oRid )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("locked"),		m_oLocked )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("macro"),			m_oMacro )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("print"),			m_oPrint )
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("uiObject"),		m_oUiObject )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring>				m_oAltText;
			nullable<SimpleTypes::COnOff<>>		m_oAutoFill;
			nullable<SimpleTypes::COnOff<>>		m_oAutoLine;
			nullable<SimpleTypes::COnOff<>>		m_oAutoPict;
			nullable<SimpleTypes::COnOff<>>		m_oDde;
			nullable<SimpleTypes::COnOff<>>		m_oDefaultSize;
			nullable<SimpleTypes::COnOff<>>		m_oDisabled;
			nullable<SimpleTypes::CRelationshipId> m_oRid;
			nullable<SimpleTypes::COnOff<>>		m_oLocked;
			nullable<std::wstring>				m_oMacro;
			nullable<SimpleTypes::COnOff<>>		m_oPrint;
			nullable<SimpleTypes::COnOff<>>		m_oUiObject;

			nullable<CExtAnchor>			m_oAnchor;
		};

		class COleObject : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleObject)
			COleObject()
			{
			}
			virtual ~COleObject()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void toXML2(NSStringUtils::CStringBuilder& writer, bool bObjectPr) const
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
            void toXMLPptx(NSStringUtils::CStringBuilder& writer, std::wstring qqq)
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("objectPr") == sName )
					{
						m_oObjectPr = oReader;
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_OleObject;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if	 ( oReader, _T("progId"),		m_oProgId )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("dvAspect"),	m_oDvAspect )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("link"),		m_oLink )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("oleUpdate"),m_oOleUpdate )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("autoLoad"),	m_oAutoLoad )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("shapeId"),	m_oShapeId )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("r:id"),		m_oRid )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<std::wstring >								m_oProgId;
			nullable<SimpleTypes::Spreadsheet::ST_DvAspect<>>	m_oDvAspect;
			nullable<std::wstring >								m_oLink;
			nullable<SimpleTypes::Spreadsheet::ST_OleUpdate<>>	m_oOleUpdate;
			nullable<SimpleTypes::COnOff<>>						m_oAutoLoad;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oShapeId;
			nullable<SimpleTypes::CRelationshipId>				m_oRid;
			nullable<COleObjectPr>								m_oObjectPr;
	//internal
			smart_ptr<OOX::OleObject>							m_OleObjectFile;
			nullable<SimpleTypes::CRelationshipId>				m_oRidImg;
		};

		class COleObjects : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(COleObjects)
			COleObjects()
			{
			}
			virtual ~COleObjects()
			{
                for(boost::unordered_map<unsigned int, COleObject*>::const_iterator it = m_mapOleObjects.begin(); it != m_mapOleObjects.end(); ++it)
				{
					delete it->second;
				}
				m_mapOleObjects.clear();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("oleObject") == sName )
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
					else if ( _T("AlternateContent") == sName )
					{
						int nSubDepth = oReader.GetDepth();
						while( oReader.ReadNextSiblingNode( nSubDepth ) )
						{
							std::wstring sSubName = XmlUtils::GetNameNoNS(oReader.GetName());
							if ( _T("Fallback") == sSubName || _T("Choice") == sSubName )
							{
								bool bFound = false;
								int nSubSubDepth = oReader.GetDepth();
								while( oReader.ReadNextSiblingNode( nSubSubDepth ) )
								{
									std::wstring sSubSubName = XmlUtils::GetNameNoNS(oReader.GetName());
									if ( _T("oleObject") == sSubSubName )
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

			virtual EElementType getType () const
			{
				return et_x_OleObjects;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
            boost::unordered_map<unsigned int, COleObject*> m_mapOleObjects;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_OLEOBJECTS_FILE_INCLUDE_H_
