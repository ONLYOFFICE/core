#pragma once
#ifndef OOX_OLEOBJECTS_FILE_INCLUDE_H_
#define OOX_OLEOBJECTS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class COleObject : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(COleObject)
			COleObject()
			{
			}
			virtual ~COleObject()
			{
			}

		public:
			virtual CString	  toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				CString sRoot;
				writer.WriteString(CString(_T("<oleObject")));
				if(m_oProgId.IsInit())
				{
					writer.WriteString(L" progId=\"");
					writer.WriteEncodeXmlString(m_oProgId.get());
					writer.WriteString(L"\"");
				}
				if(m_oDvAspect.IsInit())
				{
					writer.WriteString(L" dvAspect=\"");
					writer.WriteString(m_oDvAspect->ToString());
					writer.WriteString(L"\"");
				}
				if(m_oLink.IsInit())
				{
					writer.WriteString(L" link=\"");
					writer.WriteEncodeXmlString(m_oLink.get());
					writer.WriteString(L"\"");
				}
				if(m_oOleUpdate.IsInit())
				{
					writer.WriteString(L" oleUpdate=\"");
					writer.WriteString(m_oOleUpdate->ToString());
					writer.WriteString(L"\"");
				}
				if(m_oAutoLoad.IsInit())
				{
					writer.WriteString(L" autoLoad=\"");
					writer.WriteString(m_oAutoLoad->ToString2(SimpleTypes::onofftostring1));
					writer.WriteString(L"\"");
				}
				if(m_oShapeId.IsInit())
				{
					writer.WriteString(L" shapeId=\"");
					writer.WriteString(m_oShapeId->ToString());
					writer.WriteString(L"\"");
				}
				if(m_oRid.IsInit())
				{
					writer.WriteString(L" r:id=\"");
					writer.WriteString(m_oRid->GetValue());
					writer.WriteString(L"\"");
				}

				writer.WriteString(CString(_T("/>")));
			}
			void toXMLPptx(XmlUtils::CStringWriter& writer, CString qqq) const
			{
				CString sRoot;
				writer.WriteString(CString(_T("<o:OLEObject")));
				if(m_oDvAspect.IsInit())
				{
					writer.WriteString(L" DrawAspect=\"");
					if(SimpleTypes::Spreadsheet::Content == m_oDvAspect->GetValue())
						writer.WriteString(L"Content");
					else
						writer.WriteString(L"Icon");
					writer.WriteString(L"\"");
				}
				if(m_oRid.IsInit())
				{
					writer.WriteString(L" r:id=\"");
					writer.WriteString(m_oRid->GetValue());
					writer.WriteString(L"\"");
				}
				if(m_oProgId.IsInit())
				{
					writer.WriteString(L" ProgID=\"");
					writer.WriteEncodeXmlString(m_oProgId.get());
					writer.WriteString(L"\"");
				}
				if(m_oShapeId.IsInit())
				{
					writer.WriteString(L" ShapeID=\"");
					writer.WriteString(m_oShapeId->ToString());
					writer.WriteString(L"\"");
				}
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
				if(m_oFilepathBin.IsInit())
				{
					writer.WriteString(L" pathbin=\"");
					writer.WriteEncodeXmlString(m_oFilepathBin.get());
					writer.WriteString(L"\"");
				}

				if(m_oFilepathImg.IsInit())
				{
					writer.WriteString(L" pathimg=\"");
					writer.WriteEncodeXmlString(m_oFilepathImg.get());
					writer.WriteString(L"\"");
				}

				if(m_oRidImg.IsInit())
				{
					writer.WriteString(L" idimg=\"");
					writer.WriteString(m_oRidImg->GetValue());
					writer.WriteString(L"\"");
				}

				writer.WriteString(CString(_T("/>")));
			}
			virtual void		 fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				//todo objectPr
				oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_OleObject;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if	 ( oReader, _T("progId"),	  m_oProgId )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("dvAspect"),	  m_oDvAspect )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("link"),	  m_oLink )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("oleUpdate"),	  m_oOleUpdate )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("autoLoad"),	  m_oAutoLoad )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("shapeId"),	  m_oShapeId )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("r:id"),	  m_oRid )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("pathbin"),	  m_oFilepathBin )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("pathimg"),	  m_oFilepathImg )
					WritingElement_ReadAttributes_Read_else_if	 ( oReader, _T("idimg"),	  m_oRidImg )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<CString > m_oProgId;
			nullable<SimpleTypes::Spreadsheet::ST_DvAspect<> > m_oDvAspect;
			nullable<CString > m_oLink;
			nullable<SimpleTypes::Spreadsheet::ST_OleUpdate<> > m_oOleUpdate;
			nullable<SimpleTypes::COnOff<> > m_oAutoLoad;
			nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oShapeId;
			nullable<SimpleTypes::CRelationshipId > m_oRid;
			//internal
			nullable<CString > m_oFilepathBin;
			nullable<CString > m_oFilepathImg;
			nullable<SimpleTypes::CRelationshipId > m_oRidImg;
		};

		class COleObjects : public WritingElement
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(COleObjects)
			COleObjects()
			{
			}
			virtual ~COleObjects()
			{
				for(std::map<int, COleObject*>::const_iterator it = m_mapOleObjects.begin(); it != m_mapOleObjects.end(); it++)
				{
					delete it->second;
				}
				m_mapOleObjects.clear();
			}

		public:
			virtual CString	  toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_mapOleObjects.size() > 0)
				{
					writer.WriteString(L"<oleObjects>");
					for(std::map<int, COleObject*>::const_iterator it = m_mapOleObjects.begin(); it != m_mapOleObjects.end(); it++)
					{
						it->second->toXML(writer);
					}
					writer.WriteString(CString(_T("</oleObjects>")));
				}
			}
			virtual void		 fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

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
							CString sSubName = XmlUtils::GetNameNoNS(oReader.GetName());
							if ( _T("Fallback") == sSubName || _T("Choice") == sSubName )
							{
								int nSubSubDepth = oReader.GetDepth();
								while( oReader.ReadNextSiblingNode( nSubSubDepth ) )
								{
									CString sSubSubName = XmlUtils::GetNameNoNS(oReader.GetName());
									if ( _T("oleObject") == sSubSubName )
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
										m_mapOleObjects[pOleObject->m_oShapeId->GetValue()] = pOleObject;
									}
								}
							}
						}
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_OleObjects;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			std::map<int, COleObject*> m_mapOleObjects;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_OLEOBJECTS_FILE_INCLUDE_H_
