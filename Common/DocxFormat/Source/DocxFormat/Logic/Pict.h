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

#include "../Drawing/Drawing.h"
#include "../../Common/SimpleTypes_Word.h"

#include "VmlOfficeDrawing.h"
#include "Vml.h"

namespace OOX
{
	namespace Logic
	{
		class CBinData : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBinData)
			CBinData(OOX::Document *pMain = NULL) : WritingElement(pMain) {}
			virtual ~CBinData() {}

            virtual void fromXML(XmlUtils::CXmlNode &oNode)
			{
			}

            virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				m_sData = oReader.GetText2A();
			}

            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<w:binData ");

				ComplexTypes_WriteAttribute2( _T("w:name=\""), m_sName );

				sResult += _T(">");

				if (m_sData.IsInit())
				{
				}

				sResult += _T("</w:binData>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_binData;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				std::wstring wsName = oReader.GetName();
				while( !wsName.empty() )
				{
					if ( _T("w:name") == wsName )
						m_sName = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		public:

            nullable<std::wstring>		m_sName;
			nullable<std::string>		m_sData;
		};

		//--------------------------------------------------------------------------------
		// CControl 9.2.2.1 (Part 4)
		//--------------------------------------------------------------------------------	
		class CControl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CControl)
			CControl(OOX::Document *pMain = NULL) : WritingElement(pMain) {}
			virtual ~CControl() {}

			virtual void fromXML(XmlUtils::CXmlNode &oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("r:id"),      m_rId  );
				XmlMacroReadAttributeBase( oNode, _T("w:name"),    m_sName );
				XmlMacroReadAttributeBase( oNode, _T("w:shapeid"), m_sShapeId );

				if (false == m_rId.IsInit())
				{
					XmlMacroReadAttributeBase( oNode, _T("relationships:id"), m_rId );
				}
			}

            virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}

            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<w:control ");

				ComplexTypes_WriteAttribute ( _T("r:id=\""),      m_rId );
				ComplexTypes_WriteAttribute2( _T("w:name=\""),    m_sName );
				ComplexTypes_WriteAttribute2( _T("w:shapeid=\""), m_sShapeId );

				sResult += _T("/>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_control;
			}

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("r:id"),				m_rId )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("relationships:id"),	m_rId )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:name"),				m_sName )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:shapeid"),			m_sShapeId )
				WritingElement_ReadAttributes_End( oReader )
			}

		public:

            nullable<SimpleTypes::CRelationshipId>      m_rId;
            nullable<std::wstring>                      m_sName;
            nullable<std::wstring>                      m_sShapeId;
		};

		//--------------------------------------------------------------------------------
		// CPicture 9.2.2.2 (Part 4)
		//--------------------------------------------------------------------------------	
		class CPicture : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CPicture)
			CPicture(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain) {}
			virtual ~CPicture() 
			{
			}

			virtual void fromXML(XmlUtils::CXmlNode &oNode)
			{
				if ( oNode.IsValid() == false)
					return;

				m_sXml.Init();
                *m_sXml = oNode.GetXml();	//для pptx dll

				fromStringXML(m_sXml.get());
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{			
				if ( oReader.IsEmptyNode() )
					return;
                
                m_sXml.Init();
                *m_sXml = oReader.GetOuterXml();	//для pptx dll

				fromStringXML(m_sXml.get());
			}
			void fromStringXML(const std::wstring & xml_string)
			{                
                std::wstring sBegin(L"<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
                
                std::wstring sEnd(L"</root>");
                std::wstring sXml = sBegin + xml_string + sEnd;
                                
				XmlUtils::CXmlLiteReader oSubReader;
				
				oSubReader.FromString(sXml);
				oSubReader.ReadNextNode();//root
				oSubReader.ReadNextNode();//pict

				int nCurDepth = oSubReader.GetDepth();
				while ( oSubReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oSubReader.GetName();

					WritingElement* pItem = NULL;

					wchar_t wChar0 = sName[0];
					switch (wChar0)
					{
					case 'o':
						{
							wchar_t wChar2 = sName[2]; // o:_
							switch ( wChar2 )
							{
							case 'b':
								if ( _T("o:bottom") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( m_pMainDocument );

								break;

							case 'c':
								if ( _T("o:callout") == sName )
									pItem = new OOX::VmlOffice::CCallout( m_pMainDocument );
								else if ( _T("o:clippath") == sName )
									pItem = new OOX::VmlOffice::CClipPath( m_pMainDocument );
								else if ( _T("o:column") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( m_pMainDocument );
								else if ( _T("o:complex") == sName )
									pItem = new OOX::VmlOffice::CComplex( m_pMainDocument );

								break;

							case 'd':
								if ( _T("o:diagram") == sName )
									pItem = new OOX::VmlOffice::CDiagram( m_pMainDocument );

								break;

							case 'e':
								if ( _T("o:equationxml") == sName )
									pItem = new OOX::VmlOffice::CEquationXml( m_pMainDocument );
								else if ( _T("o:extrusion") == sName )
									pItem = new OOX::VmlOffice::CExtrusion( m_pMainDocument );

								break;

							case 'f':
								if ( _T("o:fill") == sName )
									pItem = new OOX::VmlOffice::CFill( m_pMainDocument );

								break;

							case 'i':
								if ( _T("o:ink") == sName )
									pItem = new OOX::VmlOffice::CInk( m_pMainDocument );

								break;

							case 'l':
								if ( _T("o:left") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( m_pMainDocument );
								else if ( _T("o:lock") == sName )
									pItem = new OOX::VmlOffice::CLock( m_pMainDocument );

								break;

							case 'O':
								if ( _T("o:OLEObject") == sName )
									pItem = new OOX::VmlOffice::COLEObject( m_pMainDocument );

								break;

							case 'r':
								if ( _T("o:right") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( m_pMainDocument );

								break;

							case 's':
								if ( _T("o:shapedefaults") == sName )
									pItem = new OOX::VmlOffice::CShapeDefaults( m_pMainDocument );
								else if ( _T("o:shapelayout") == sName )
									pItem = new OOX::VmlOffice::CShapeLayout( m_pMainDocument );
								else if ( _T("o:signatureline") == sName )
									pItem = new OOX::VmlOffice::CSignatureLine( m_pMainDocument );
								else if ( _T("o:skew") == sName )
									pItem = new OOX::VmlOffice::CSkew( m_pMainDocument );

								break;

							case 't':
								if ( _T("o:top") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( m_pMainDocument );

								break;
							}

							break;
						}

					case 'v':
						{
							wchar_t wChar2 = sName[2]; // v:_
							switch ( wChar2 )
							{
							case 'a':
								if ( _T("v:arc") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CArc(m_pMainDocument);
									m_oShapeElement->fromXML(oSubReader );
								}break;

							case 'b':
								if ( _T("v:background") == sName )
									pItem = new OOX::Vml::CBackground( m_pMainDocument );

								break;

							case 'c':
								if ( _T("v:curve") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CCurve(m_pMainDocument);//???
									m_oShapeElement->fromXML(oSubReader );
								}break;
							case 'f':
								if ( _T("v:fill") == sName )
									pItem = new OOX::Vml::CFill( m_pMainDocument );
								else if ( _T("v:formulas") == sName )
									pItem = new OOX::Vml::CFormulas( m_pMainDocument );

								break;

							case 'g':
								if ( _T("v:group") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CGroup(m_pMainDocument);
									m_oShapeElement->fromXML(oSubReader );
								}break;

							case 'h':
								if ( _T("v:handles") == sName )
									pItem = new OOX::Vml::CHandles( m_pMainDocument );

								break;

							case 'i':
								if ( _T("v:image") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CImage(m_pMainDocument);
									m_oShapeElement->fromXML(oSubReader );
								}
								else if ( _T("v:imagedata") == sName )
								{
									pItem = new OOX::Vml::CImageData( m_pMainDocument );
								}
								break;

							case 'l':
								if ( _T("v:line") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CLine( m_pMainDocument );
									m_oShapeElement->fromXML(oSubReader );
								}
								break;

							case 'o':
								if ( _T("v:oval") == sName )
								{
									m_oShapeElement =  new OOX::Vml::COval( m_pMainDocument );
									m_oShapeElement->fromXML(oSubReader );
								}
								break;

							case 'p':
								if ( _T("v:path") == sName )
									pItem = new OOX::Vml::CPath( m_pMainDocument );
								else if ( _T("v:polyline") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CPolyLine( m_pMainDocument );
									m_oShapeElement->fromXML(oSubReader );
								}

								break;

							case 'r':
								if ( _T("v:rect") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CRect( m_pMainDocument );
									m_oShapeElement->fromXML(oSubReader );
								}
								else if ( _T("v:roundrect") == sName )
								{
									m_oShapeElement =  new OOX::Vml::CRoundRect( m_pMainDocument );
									m_oShapeElement->fromXML(oSubReader );
								}
								break;

							case 's':
								if ( _T("v:shadow") == sName )
									pItem = new OOX::Vml::CShadow( m_pMainDocument );
								else if ( _T("v:shape") == sName )
								{
									m_oShapeElement = new OOX::Vml::CShape( m_pMainDocument );
									m_oShapeElement->fromXML(oSubReader );
								}
								else if ( _T("v:shapetype") == sName )
								{
									m_oShapeType = new OOX::Vml::CShapeType( m_pMainDocument );
									m_oShapeType->fromXML(oSubReader );
								}
								else if ( _T("v:stroke") == sName )
									pItem = new OOX::Vml::CStroke( m_pMainDocument );

								break;

							case 't':
								if ( _T("v:textbox") == sName )
									pItem = new OOX::Vml::CTextbox( m_pMainDocument );
								else if ( _T("v:textpath") == sName )
									pItem = new OOX::Vml::CTextPath( m_pMainDocument );

								break;
							}
							break;
						}
					case 'w':
						if ( L"w:control" == sName )
						{
							m_oControl = new OOX::Logic::CControl( m_pMainDocument );
							m_oControl->fromXML(oSubReader );
					}
						else if ( L"w:binData" == sName )
						{
							m_oBinData = new OOX::Logic::CBinData( m_pMainDocument );
							m_oBinData->fromXML(oSubReader );
						}

						break;
					}

					if ( pItem )
					{
						m_arrItems.push_back( pItem );
						pItem->fromXML(oSubReader );
					}
				}
			}

            virtual std::wstring toXML() const
			{
                std::wstring sResult = _T("<w:pict>");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        sResult += m_arrItems[i]->toXML();
                    }
                }
				if (m_oShapeType.IsInit())
					sResult += m_oShapeType->toXML();

				if (m_oShapeElement.IsInit())
					sResult += m_oShapeElement->toXML();

				if ( m_oControl.IsInit() )
					sResult += m_oControl->toXML();

				if (m_oBinData.IsInit())
					sResult += m_oBinData->toXML();

				sResult += _T("</w:pict>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_pict;
			}

            nullable<std::wstring>          m_sXml;

			nullable<OOX::Logic::CControl>	m_oControl;
			nullable<OOX::Logic::CBinData>	m_oBinData;

			nullable<OOX::Vml::CShapeType>	m_oShapeType;
			nullable<OOX::WritingElement>	m_oShapeElement;
			
			// TO DO: Добавить класс, читающий movie
		};


		//--------------------------------------------------------------------------------
		// CObject 17.3.3.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CObject : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CObject)
			CObject(OOX::Document *pMain = NULL) : WritingElementWithChilds<>(pMain) {}
			virtual ~CObject() 
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				m_sXml.Init();
                *m_sXml = oReader.GetOuterXml();	//для pptx dll

				//альтернатива pptx
                std::wstring sXml; //??? + ole наверно что то (лень ...)
				sXml += _T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
				sXml += m_sXml.get();
				sXml += _T("</root>");

				XmlUtils::CXmlLiteReader oSubReader;
				oSubReader.FromString(sXml);
				oSubReader.ReadNextNode();//root
				oSubReader.ReadNextNode();//pict

				ReadAttributes(oSubReader);

				int nCurDepth = oSubReader.GetDepth();
				while ( oSubReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oSubReader.GetName();

					WritingElement* pItem = NULL;

					wchar_t wChar0 = sName[0];
					switch (wChar0)
					{
					case 'o':
						{
							wchar_t wChar2 = sName[2]; // o:_
							switch ( wChar2 )
							{
							case 'b':
								if ( _T("o:bottom") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( oSubReader );
								break;

							case 'c':
								if ( _T("o:callout") == sName )
									pItem = new OOX::VmlOffice::CCallout( oSubReader );
								else if ( _T("o:clippath") == sName )
									pItem = new OOX::VmlOffice::CClipPath( oSubReader );
								else if ( _T("o:column") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( oSubReader );
								else if ( _T("o:complex") == sName )
									pItem = new OOX::VmlOffice::CComplex( oSubReader );

								break;

							case 'd':
								if ( _T("o:diagram") == sName )
									pItem = new OOX::VmlOffice::CDiagram( oSubReader );

								break;

							case 'e':
								if ( _T("o:equationxml") == sName )
									pItem = new OOX::VmlOffice::CEquationXml( oSubReader );
								else if ( _T("o:extrusion") == sName )
									pItem = new OOX::VmlOffice::CExtrusion( oSubReader );

								break;

							case 'f':
								if ( _T("o:fill") == sName )
									pItem = new OOX::VmlOffice::CFill( oSubReader );

								break;

							case 'i':
								if ( _T("o:ink") == sName )
									pItem = new OOX::VmlOffice::CInk( oSubReader );

								break;

							case 'l':
								if ( _T("o:left") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( oSubReader );
								else if ( _T("o:lock") == sName )
									pItem = new OOX::VmlOffice::CLock( oSubReader );

								break;

							case 'O':// собственно это и есть самый главный под-объект
								if ( _T("o:OLEObject") == sName )
									m_oOleObject = oSubReader ;
								break;

							case 'r':
								if ( _T("o:right") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( oSubReader );
								break;

							case 's':
								if ( _T("o:shapedefaults") == sName )
									pItem = new OOX::VmlOffice::CShapeDefaults( oSubReader );
								else if ( _T("o:shapelayout") == sName )
									pItem = new OOX::VmlOffice::CShapeLayout( oSubReader );
								else if ( _T("o:signatureline") == sName )
									pItem = new OOX::VmlOffice::CSignatureLine( oSubReader );
								else if ( _T("o:skew") == sName )
									pItem = new OOX::VmlOffice::CSkew( oSubReader );
								break;

							case 't':
								if ( _T("o:top") == sName )
									pItem = new OOX::VmlOffice::CStrokeChild( oSubReader );
								break;
							}

							break;
						}

					case 'v':
						{
							wchar_t wChar2 = sName[2]; // v:_
							switch ( wChar2 )
							{
							case 'b':
								if ( _T("v:background") == sName )
									pItem = new OOX::Vml::CBackground( oSubReader );
								break;

							case 'f':
								if ( _T("v:fill") == sName )
									pItem = new OOX::Vml::CFill( oSubReader );
								else if ( _T("v:formulas") == sName )
									pItem = new OOX::Vml::CFormulas( oSubReader );
								break;

							case 'h':
								if ( _T("v:handles") == sName )
									pItem = new OOX::Vml::CHandles( oSubReader );
								break;

							case 'i':
								if ( _T("v:image") == sName )
									pItem = new OOX::Vml::CImage( oSubReader );
								else if ( _T("v:imagedata") == sName )
									pItem = new OOX::Vml::CImageData( oSubReader );
								break;
							case 'p':
								if ( _T("v:path") == sName )
									pItem = new OOX::Vml::CPath( oSubReader );
								break;
							case 'r':
								if ( _T("v:rect") == sName )
									m_oShape = oSubReader;
								break;
							case 's':
								if ( _T("v:shadow") == sName )
									pItem = new OOX::Vml::CShadow( oSubReader );
								else if ( _T("v:shape") == sName )
									m_oShape = oSubReader;
								else if ( _T("v:shapetype") == sName )
									m_oShapeType = oSubReader;
								else if ( _T("v:stroke") == sName )
									pItem = new OOX::Vml::CStroke( oSubReader );
								break;

							case 't':
								if ( _T("v:textbox") == sName )
									pItem = new OOX::Vml::CTextbox( oSubReader );
								else if ( _T("v:textpath") == sName )
									pItem = new OOX::Vml::CTextPath( oSubReader );
								break;
							}
							break;
						}
					case 'w':
						if (L"w:control" == sName )
							m_oControl = oSubReader;
						else if (L"w:objectEmbed" == sName)
							m_oOleObject = oSubReader;
						else if (L"w:drawing" == sName)
							m_oDrawing = oSubReader;

						break;
					}

					if ( pItem )
					{
						m_arrItems.push_back( pItem );
					}
				}
			}
            virtual std::wstring toXML() const
			{
				return _T("<w:object/>");
			}

			virtual EElementType getType() const
			{
				return et_w_object;
			}

            nullable<std::wstring>					m_sXml;
//-----------------------------------------------------------------------
			nullable_int							m_oDxaOrig;
			nullable_int							m_oDyaOrig;

			nullable<OOX::Logic::CControl>			m_oControl;
			nullable<OOX::Logic::CDrawing>			m_oDrawing;

			nullable<OOX::Vml::CShapeType>			m_oShapeType;
			nullable<OOX::VmlOffice::COLEObject>	m_oOleObject;
			
			nullable<OOX::Vml::CShape>				m_oShape;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("w:dxaOrig"), m_oDxaOrig )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("w:dyaOrig"), m_oDyaOrig )
				WritingElement_ReadAttributes_End( oReader )
			}
		};



	} // namespace Logic
} // namespace OOX
