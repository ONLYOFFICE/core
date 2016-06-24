/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#ifndef OOX_LOGIC_PICT_INCLUDE_H_
#define OOX_LOGIC_PICT_INCLUDE_H_

#include "../../Base/Nullable.h"

#include "../../Common/SimpleTypes_Word.h"

#include "../WritingElement.h"
#include "../RId.h"

#include "VmlOfficeDrawing.h"
#include "Vml.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// CControl 9.2.2.1 (Part 4)
		//--------------------------------------------------------------------------------	
		class CControl : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CControl)
			CControl() {}
			virtual ~CControl() {}

		public:

			virtual void         fromXML(XmlUtils::CXmlNode &oNode)
			{
				oNode.ReadAttributeBase( _T("r:id"),      m_rId  );
				oNode.ReadAttributeBase( _T("w:name"),    m_sName );
				oNode.ReadAttributeBase( _T("w:shapeid"), m_sShapeId );
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd( oReader.GetDepth() );
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<w:control ");

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
				if ( oReader.GetAttributesCount() <= 0 )
					return;

				if ( !oReader.MoveToFirstAttribute() )
					return;

				CWCharWrapper wsName = oReader.GetName();
				while( !wsName.IsNull() )
				{
					if ( _T("r:id") == wsName )
						m_rId = oReader.GetText();
					else if ( _T("w:name") == wsName )
						m_sName = oReader.GetText();
					else if ( _T("w:shapeid") == wsName )
						m_sShapeId = oReader.GetText();

					if ( !oReader.MoveToNextAttribute() )
						break;

					wsName = oReader.GetName();
				}

				oReader.MoveToElement();
			}

		public:

			nullable<SimpleTypes::CRelationshipId> m_rId;
			nullable<CString>                      m_sName;
			nullable<CString>                      m_sShapeId;
		};

		//--------------------------------------------------------------------------------
		// CPicture 9.2.2.2 (Part 4)
		//--------------------------------------------------------------------------------	
		class CPicture : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CPicture)
			CPicture() {}
			virtual ~CPicture() 
			{
			}

		public:
			virtual void         fromXML(XmlUtils::CXmlNode &oNode)
			{
				// TO DO: Реализовать CPicture::fromXML(XmlUtils::CXmlNode &oNode)
			}

			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{			
				if ( oReader.IsEmptyNode() )
					return;
                
                m_sXml.Init();
                m_sXml->Append(oReader.GetOuterXml());	//для pptx dll
                
                //альтернатива pptx
                //				CString sXml;
                //				sXml.Format(_T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">%ls</root>"), m_sXml.get());
                
                CString sBegin(_T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
                
                CString sEnd(_T("</root>"));
                CString sXml = sBegin + m_sXml.get() + sEnd;
                                
				XmlUtils::CXmlLiteReader oSubReader;
				
				oSubReader.FromString(sXml);
				oSubReader.ReadNextNode();//root
				oSubReader.ReadNextNode();//pict

				int nCurDepth = oSubReader.GetDepth();
				while ( oSubReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oSubReader.GetName();

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

							case 'O':
								if ( _T("o:OLEObject") == sName )
									pItem = new OOX::VmlOffice::COLEObject( oSubReader );

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
							case 'a':
								if ( _T("v:arc") == sName )
									m_oShapeArc =  oSubReader;

								break;

							case 'b':
								if ( _T("v:background") == sName )
									pItem = new OOX::Vml::CBackground( oSubReader );

								break;

							case 'c':
								if ( _T("v:curve") == sName )
									m_oShapeCurve =  oSubReader;//???

								break;

							case 'f':
								if ( _T("v:fill") == sName )
									pItem = new OOX::Vml::CFill( oSubReader );
								else if ( _T("v:formulas") == sName )
									pItem = new OOX::Vml::CFormulas( oSubReader );

								break;

							case 'g':
								if ( _T("v:group") == sName )
									m_oShapeGroup =  oSubReader;

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

							case 'l':
								if ( _T("v:line") == sName )
									m_oShapeLine =  oSubReader;

								break;

							case 'o':
								if ( _T("v:oval") == sName )
									m_oShapeOval =  oSubReader;
								break;

							case 'p':
								if ( _T("v:path") == sName )
									pItem = new OOX::Vml::CPath( oSubReader );
								else if ( _T("v:polyline") == sName )
									m_oShapePolyLine =  oSubReader;

								break;

							case 'r':
								if ( _T("v:rect") == sName )
									m_oShapeRect =  oSubReader;
								else if ( _T("v:roundrect") == sName )
									m_oShapeRoundRect =  oSubReader;

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
						if ( _T("w:control") == sName )
							m_oControl = oSubReader;

						break;
					}

					if ( pItem )
					{
						m_arrItems.push_back( pItem );
					}
				}
			}

			virtual CString      toXML() const
			{
				CString sResult = _T("<w:pict>");

				for (unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
				{
					if ( m_arrItems[nIndex] )
						sResult += m_arrItems[nIndex]->toXML();
				}

				if ( m_oControl.IsInit() )
					sResult += m_oControl->toXML();

				sResult += _T("</w:pict>");

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_pict;
			}

		public:

			nullable<CString> m_sXml;

			// Childs
			nullable<OOX::Logic::CControl>	m_oControl;
//top childs
			nullable<OOX::Vml::CShapeType>	m_oShapeType;//custom
			nullable<OOX::Vml::COval>		m_oShapeOval;
			nullable<OOX::Vml::CRect>		m_oShapeRect;
			nullable<OOX::Vml::CLine>		m_oShapeLine;
			nullable<OOX::Vml::CPolyLine>	m_oShapePolyLine;
			nullable<OOX::Vml::CArc>		m_oShapeArc;
			nullable<OOX::Vml::CCurve>		m_oShapeCurve;
			nullable<OOX::Vml::CRoundRect>	m_oShapeRoundRect;		

			nullable<OOX::Vml::CGroup>		m_oShapeGroup;
			
			nullable<OOX::Vml::CShape>		m_oShape;
			
			// TO DO: Добавить класс, читающий movie
		};


		//--------------------------------------------------------------------------------
		// CObject 17.3.3.19 (Part 1)
		//--------------------------------------------------------------------------------	
		class CObject : public WritingElementWithChilds<>
		{
		public:
			WritingElement_AdditionConstructors(CObject)
			CObject() {}
			virtual ~CObject() 
			{
			}
	
		public:
			virtual void         fromXML(XmlUtils::CXmlNode& oNode)
			{
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				m_sXml.Init();
				m_sXml->Append(oReader.GetOuterXml());	//для pptx dll

				//альтернатива pptx
				CString sXml; //??? + ole наверно что то (лень ...) 
				sXml.Append(_T("<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">"));
				sXml.Append(m_sXml.get());
				sXml.Append(_T("</root>"));

				XmlUtils::CXmlLiteReader oSubReader;
				oSubReader.FromString(sXml);
				oSubReader.ReadNextNode();//root
				oSubReader.ReadNextNode();//pict

				int nCurDepth = oSubReader.GetDepth();
				while ( oSubReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CWCharWrapper sName = oSubReader.GetName();

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
						if ( _T("w:control") == sName )
							m_oControl = oSubReader;
						break;
					}

					if ( pItem )
					{
						m_arrItems.push_back( pItem );
					}
				}
			}
			virtual CString      toXML() const
			{
				return _T("<w:object />");
			}

			virtual EElementType getType() const
			{
				return et_w_object;
			}
			nullable<CString> m_sXml;

			// Childs
			nullable<OOX::Logic::CControl>			m_oControl;
//top childs
			nullable<OOX::Vml::CShapeType>			m_oShapeType;//?? нужен ли отдельно тута???
			nullable<OOX::VmlOffice::COLEObject>	m_oOleObject;
			
			nullable<OOX::Vml::CShape>				m_oShape;
//minor childs

		};



	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_PICT_INCLUDE_H_