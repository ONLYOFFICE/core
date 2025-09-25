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
#include "../DocxFlat.h"
#include "../Document.h"

#include "Pict.h"
//#include "Vml.h"
#include "VmlOfficeDrawing.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../../OOXML/PPTXFormat/DrawingConverter/ASCOfficeDrawingConverter.h"

namespace OOX
{
	namespace Logic
	{
		CBinData::CBinData(OOX::Document *pMain) : WritingElement(pMain) {}
		CBinData::~CBinData() {}
		
		void CBinData::fromXML(XmlUtils::CXmlNode &oNode)
		{
			m_sName = oNode.GetAttribute(L"w:name");

			m_sData = oNode.GetTextA();
		}
		
		void CBinData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			m_sData = oReader.GetText2A();
		}
		std::wstring CBinData::toXML() const
		{
			std::wstring sResult = L"<w:binData ";

			ComplexTypes_WriteAttribute2( L"w:name=\"", m_sName );

			sResult += L">";

			if (m_sData.IsInit())
			{
			}
			sResult += L"</w:binData>";

			return sResult;
		}
		EElementType CBinData::getType() const
		{
			return et_w_binData;
		}
		void CBinData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.GetAttributesCount() <= 0 )
				return;

			if ( !oReader.MoveToFirstAttribute() )
				return;

			std::wstring wsName = oReader.GetName();
			while( !wsName.empty() )
			{
				if ( L"w:name" == wsName )
					m_sName = oReader.GetText();

				if ( !oReader.MoveToNextAttribute() )
					break;

				wsName = oReader.GetName();
			}

			oReader.MoveToElement();
		}
		std::vector<BYTE> CBinData::GetBytes()
		{
			std::vector<BYTE> result;
			if (!m_sData.IsInit()) return result;
			
			int dstLen = Base64::Base64DecodeGetRequiredLength((int)m_sData->size());
			result.resize(dstLen);
			Base64::Base64Decode(m_sData->c_str(), (int)m_sData->size(), result.data(), &dstLen);
			result.resize(dstLen);
			
			//COfficeUtils oCOfficeUtils(NULL);
			//if (oCOfficeUtils.IsArchive(result.data(), result.size()))
			//{//gzip
			//	ULONG nBytesUncompress = result.size() * 10;
			//	BYTE* pDataUncompress = new BYTE[nBytesUncompress];
			//	if (S_OK == oCOfficeUtils.Uncompress(pDataUncompress, &nBytesUncompress, result.data(), result.size()))
			//	{
			//		result.resize(nBytesUncompress);
			//		memcpy(result.data(), pDataUncompress, nBytesUncompress);
			//		delete[]pDataUncompress;
			//	}
			//}

			return result;
		}

		CControl::CControl(OOX::Document *pMain) : WritingElement(pMain) {}
		CControl::~CControl() {}
		void CControl::fromXML(XmlUtils::CXmlNode &oNode)
		{
			XmlMacroReadAttributeBase( oNode, L"r:id",      m_rId  );
			XmlMacroReadAttributeBase( oNode, L"w:name",    m_sName );
			XmlMacroReadAttributeBase( oNode, L"w:shapeid", m_sShapeId );

			if (false == m_rId.IsInit())
			{
				XmlMacroReadAttributeBase( oNode, L"relationships:id", m_rId );
			}
		}
		void CControl::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd( oReader.GetDepth() );
		}
		std::wstring CControl::toXML() const
		{
			std::wstring sResult = L"<w:control";

			ComplexTypes_WriteAttribute ( L" r:id=\"",      m_rId );
			ComplexTypes_WriteAttribute2( L" w:name=\"",    m_sName );
			ComplexTypes_WriteAttribute2( L" w:shapeid=\"", m_sShapeId );

			sResult += L"/>";

			return sResult;
		}
		EElementType CControl::getType() const
		{
			return et_w_control;
		}
		void CControl::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"r:id",				m_rId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"relationships:id",	m_rId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:name",				m_sName )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:shapeid",			m_sShapeId )
			WritingElement_ReadAttributes_End( oReader )
		}

		CPicture::CPicture(OOX::Document *pMain) : WritingElementWithChilds<>(pMain) {}
		CPicture::~CPicture()
		{
		}
		void CPicture::fromXML(XmlUtils::CXmlNode &oNode)
		{
			if (oNode.IsValid() == false)
				return;

			m_sXml.Init();
			*m_sXml = oNode.GetXml();	//для pptx dll

			fromStringXML(m_sXml.get());
		}
		void CPicture::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			m_sXml.Init();
			*m_sXml = oReader.GetOuterXml();	//для pptx dll

			fromStringXML(m_sXml.get());
		}
		void CPicture::fromStringXML(const std::wstring & xml_string)
		{
			std::wstring sBegin(L"<root xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");

			std::wstring sEnd(L"</root>");
			std::wstring sXml = sBegin + xml_string + sEnd;

			XmlUtils::CXmlLiteReader oSubReader;

			oSubReader.FromString(sXml);
			oSubReader.ReadNextNode();//root
			oSubReader.ReadNextNode();//pict

			OOX::Vml::CImageData *pImageData = NULL;

			int nCurDepth = oSubReader.GetDepth();
			while (oSubReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = oSubReader.GetName();

				WritingElement* pItem = NULL;

				wchar_t wChar0 = sName[0];
				switch (wChar0)
				{
				case 'o':
				{
					wchar_t wChar2 = sName[2]; // o:_
					switch (wChar2)
					{
					case 'b':
						if (L"o:bottom" == sName)
							pItem = new OOX::VmlOffice::CStrokeChild(m_pMainDocument);

						break;

					case 'c':
						if (L"o:callout" == sName)
							pItem = new OOX::VmlOffice::CCallout(m_pMainDocument);
						else if (L"o:clippath" == sName)
							pItem = new OOX::VmlOffice::CClipPath(m_pMainDocument);
						else if (L"o:column" == sName)
							pItem = new OOX::VmlOffice::CStrokeChild(m_pMainDocument);
						else if (L"o:complex" == sName)
							pItem = new OOX::VmlOffice::CComplex(m_pMainDocument);

						break;

					case 'd':
						if (L"o:diagram" == sName)
							pItem = new OOX::VmlOffice::CDiagram(m_pMainDocument);

						break;

					case 'e':
						if (L"o:equationxml" == sName)
							pItem = new OOX::VmlOffice::CEquationXml(m_pMainDocument);
						else if (L"o:extrusion" == sName)
							pItem = new OOX::VmlOffice::CExtrusion(m_pMainDocument);

						break;

					case 'f':
						if (L"o:fill" == sName)
							pItem = new OOX::VmlOffice::CFill(m_pMainDocument);

						break;

					case 'i':
						if (L"o:ink" == sName)
							pItem = new OOX::VmlOffice::CInk(m_pMainDocument);

						break;

					case 'l':
						if (L"o:left" == sName)
							pItem = new OOX::VmlOffice::CStrokeChild(m_pMainDocument);
						else if (L"o:lock" == sName)
							pItem = new OOX::VmlOffice::CLock(m_pMainDocument);

						break;

					case 'O':
						if (L"o:OLEObject" == sName)
						{
							m_oOLEObject = new OOX::VmlOffice::COLEObject(m_pMainDocument);
							m_oOLEObject->fromXML(oSubReader);
						}break;

					case 'r':
						if (L"o:right" == sName)
							pItem = new OOX::VmlOffice::CStrokeChild(m_pMainDocument);

						break;

					case 's':
						if (L"o:shapedefaults" == sName)
							pItem = new OOX::VmlOffice::CShapeDefaults(m_pMainDocument);
						else if (L"o:shapelayout" == sName)
							pItem = new OOX::VmlOffice::CShapeLayout(m_pMainDocument);
						else if (L"o:signatureline" == sName)
							pItem = new OOX::VmlOffice::CSignatureLine(m_pMainDocument);
						else if (L"o:skew" == sName)
							pItem = new OOX::VmlOffice::CSkew(m_pMainDocument);

						break;

					case 't':
						if (L"o:top" == sName)
							pItem = new OOX::VmlOffice::CStrokeChild(m_pMainDocument);

						break;
					}

					break;
				}

				case 'v':
				{
					wchar_t wChar2 = sName[2]; // v:_
					switch (wChar2)
					{
					case 'a':
						if (L"v:arc" == sName)
						{
							m_oShapeElement = new OOX::Vml::CArc(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}break;

					case 'b':
						if (L"v:background" == sName)
							pItem = new OOX::Vml::CBackground(m_pMainDocument);

						break;

					case 'c':
						if (L"v:curve" == sName)
						{
							m_oShapeElement = new OOX::Vml::CCurve(m_pMainDocument);//???
							m_oShapeElement->fromXML(oSubReader);
						}break;
					case 'f':
						if (L"v:fill" == sName)
							pItem = new OOX::Vml::CFill(m_pMainDocument);
						else if (L"v:formulas" == sName)
							pItem = new OOX::Vml::CFormulas(m_pMainDocument);

						break;

					case 'g':
						if (L"v:group" == sName)
						{
							m_oShapeElement = new OOX::Vml::CGroup(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}break;

					case 'h':
						if (L"v:handles" == sName)
							pItem = new OOX::Vml::CHandles(m_pMainDocument);

						break;

					case 'i':
						if (L"v:image" == sName)
						{
							m_oShapeElement = new OOX::Vml::CImage(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}
						else if (L"v:imagedata" == sName)
						{
							pItem = pImageData = new OOX::Vml::CImageData(m_pMainDocument);
						}
						break;

					case 'l':
						if (L"v:line" == sName)
						{
							m_oShapeElement = new OOX::Vml::CLine(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}
						break;

					case 'o':
						if (L"v:oval" == sName)
						{
							m_oShapeElement = new OOX::Vml::COval(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}
						break;

					case 'p':
						if (L"v:path" == sName)
							pItem = new OOX::Vml::CPath(m_pMainDocument);
						else if (L"v:polyline" == sName)
						{
							m_oShapeElement = new OOX::Vml::CPolyLine(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}

						break;

					case 'r':
						if (L"v:rect" == sName)
						{
							m_oShapeElement = new OOX::Vml::CRect(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}
						else if (L"v:roundrect" == sName)
						{
							m_oShapeElement = new OOX::Vml::CRoundRect(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}
						break;

					case 's':
						if (L"v:shadow" == sName)
							pItem = new OOX::Vml::CShadow(m_pMainDocument);
						else if (L"v:shape" == sName)
						{
							m_oShapeElement = new OOX::Vml::CShape(m_pMainDocument);
							m_oShapeElement->fromXML(oSubReader);
						}
						else if (L"v:shapetype" == sName)
						{
							m_oShapeType = new OOX::Vml::CShapeType(m_pMainDocument);
							m_oShapeType->fromXML(oSubReader);
						}
						else if (L"v:stroke" == sName)
							pItem = new OOX::Vml::CStroke(m_pMainDocument);

						break;

					case 't':
						if (L"v:textbox" == sName)
							pItem = new OOX::Vml::CTextbox(m_pMainDocument);
						else if (L"v:textpath" == sName)
							pItem = new OOX::Vml::CTextPath(m_pMainDocument);

						break;
					}
					break;
				}
				case 'w':
					if (L"w:control" == sName)
					{
						m_oControl = new OOX::Logic::CControl(m_pMainDocument);
						m_oControl->fromXML(oSubReader);
					}
					else if (L"w:binData" == sName)
					{
						m_oBinData = new OOX::Logic::CBinData(m_pMainDocument);
						m_oBinData->fromXML(oSubReader);

						if (m_oBinData->m_sData.IsInit())
						{
							OOX::CDocxFlat* docx_flat = dynamic_cast<OOX::CDocxFlat*>(m_pMainDocument);
							if (docx_flat)
							{
								smart_ptr<OOX::Image> pImageFile = smart_ptr<OOX::Image>(new OOX::Image(m_pMainDocument, true));
								pImageFile->m_Data = m_oBinData->GetBytes();

								CImageFileFormatChecker fileChecker;
								std::wstring ext = fileChecker.DetectFormatByData(pImageFile->m_Data.data(), pImageFile->m_Data.size());

								if (ext.empty())
								{
									COfficeUtils oCOfficeUtils(NULL);
									if (oCOfficeUtils.IsArchive(pImageFile->m_Data.data(), pImageFile->m_Data.size()))
									{//gzip
										ULONG nBytesUncompress = pImageFile->m_Data.size() * 10;
										BYTE* pDataUncompress = new BYTE[nBytesUncompress];
										if (S_OK == oCOfficeUtils.Uncompress(pDataUncompress, &nBytesUncompress, pImageFile->m_Data.data(), pImageFile->m_Data.size()))
										{
											ext = fileChecker.DetectFormatByData(pDataUncompress, nBytesUncompress);

											if (false == ext.empty())
											{
												pImageFile->m_Data.resize(nBytesUncompress);
												memcpy(pImageFile->m_Data.data(), pDataUncompress, nBytesUncompress);
												delete []pDataUncompress;
											}
										}
									}
								}
								if (false == ext.empty())
								{
									OOX::CPath filename(L"image." + ext);
									pImageFile->set_filename(filename, false, true);

									NSCommon::smart_ptr<OOX::File> file = pImageFile.smart_dynamic_cast<OOX::File>();
									const OOX::RId rId = docx_flat->m_currentContainer->Add(file);

									if (m_oBinData->m_sName.IsInit())
									{
										docx_flat->m_mapImages[*m_oBinData->m_sName] = file;
									}
								}
							}
						}
					}
					break;
				}

				if (pItem)
				{
					m_arrItems.push_back(pItem);
					pItem->fromXML(oSubReader);
				}
			}
		}
		std::wstring CPicture::toXML() const
		{
			std::wstring sResult = m_oOLEObject.IsInit() ? L"<w:object>" : L"<w:pict>";

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->toXML();
				}
			}
			if (m_oShapeType.IsInit())
				sResult += m_oShapeType->toXML();

			if (m_oShapeElement.IsInit())
				sResult += m_oShapeElement->toXML();

			if (m_oControl.IsInit())
				sResult += m_oControl->toXML();

			if (m_oOLEObject.IsInit())
			{
				sResult += m_oOLEObject->toXML();
			}

			sResult += m_oOLEObject.IsInit() ? L"</w:object>" : L"</w:pict>";

			return sResult;
		}
		EElementType CPicture::getType() const
		{
			return et_w_pict;
		}
//--------------------------------------------------------------------------------------------------------------------------
		CObject::CObject(OOX::Document *pMain) : WritingElementWithChilds<>(pMain) {}
		CObject::~CObject()
		{
		}
		void CObject::fromXML(XmlUtils::CXmlNode& oNode)
		{
		}
		void CObject::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			m_sXml.Init();
			*m_sXml = oReader.GetOuterXml();	//для pptx dll

												//альтернатива pptx
			std::wstring sXml; //??? + ole наверно что то (лень ...)
			sXml += L"<root \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">";
			sXml += m_sXml.get();
			sXml += L"</root>";

			XmlUtils::CXmlLiteReader oSubReader;
			oSubReader.FromString(sXml);
			oSubReader.ReadNextNode();//root
			oSubReader.ReadNextNode();//pict

			ReadAttributes(oSubReader);

			int nCurDepth = oSubReader.GetDepth();
			while (oSubReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = oSubReader.GetName();

				WritingElement* pItem = NULL;

				wchar_t wChar0 = sName[0];
				switch (wChar0)
				{
				case 'o':
				{
					wchar_t wChar2 = sName[2]; // o:_
					switch (wChar2)
					{
					case 'b':
						if (L"o:bottom" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CStrokeChild, oSubReader)
						break;

					case 'c':
						if (L"o:callout" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CCallout, oSubReader)
						else if (L"o:clippath" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CClipPath, oSubReader)
						else if (L"o:column" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CStrokeChild, oSubReader)
						else if (L"o:complex" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CComplex, oSubReader)

						break;

					case 'd':
						if (L"o:diagram" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CDiagram, oSubReader)

						break;

					case 'e':
						if (L"o:equationxml" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CEquationXml, oSubReader)
						else if (L"o:extrusion" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CExtrusion, oSubReader)

						break;

					case 'f':
						if (L"o:fill" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CFill, oSubReader)

						break;

					case 'i':
						if (L"o:ink" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CInk, oSubReader)

						break;

					case 'l':
						if (L"o:left" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CStrokeChild, oSubReader)
						else if (L"o:lock" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CLock, oSubReader)

						break;

					case 'O':// собственно это и есть самый главный под-объект
						if (L"o:OLEObject" == sName)
							m_oOleObject = oSubReader;
						break;

					case 'r':
						if (L"o:right" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CStrokeChild, oSubReader)
						break;

					case 's':
						if (L"o:shapedefaults" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CShapeDefaults, oSubReader)
						else if (L"o:shapelayout" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CShapeLayout, oSubReader)
						else if (L"o:signatureline" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CSignatureLine, oSubReader)
						else if (L"o:skew" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CSkew, oSubReader)
						break;

					case 't':
						if (L"o:top" == sName)
							AssignPtrXmlContent(pItem, OOX::VmlOffice::CStrokeChild, oSubReader)
						break;
					}

					break;
				}

				case 'v':
				{
					wchar_t wChar2 = sName[2]; // v:_
					switch (wChar2)
					{
					case 'b':
						if (L"v:background" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CBackground, oSubReader)
						break;

					case 'f':
						if (L"v:fill" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CFill, oSubReader)
						else if (L"v:formulas" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CFormulas, oSubReader)
						break;

					case 'h':
						if (L"v:handles" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CHandles, oSubReader)
						break;

					case 'i':
						if (L"v:image" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CImage, oSubReader)
						else if (L"v:imagedata" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CImageData, oSubReader)
						break;
					case 'p':
						if (L"v:path" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CPath, oSubReader)
						break;
					case 'r':
						if (L"v:rect" == sName)
							m_oShape = oSubReader;
						break;
					case 's':
						if (L"v:shadow" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CShadow, oSubReader)
						else if (L"v:shape" == sName)
							m_oShape = oSubReader;
						else if (L"v:shapetype" == sName)
							m_oShapeType = oSubReader;
						else if (L"v:stroke" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CStroke, oSubReader)
						break;

					case 't':
						if (L"v:textbox" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CTextbox, oSubReader)
						else if (L"v:textpath" == sName)
							AssignPtrXmlContent(pItem, OOX::Vml::CTextPath, oSubReader)
						break;
					}
					break;
				}
				case 'w':
					if (L"w:control" == sName)
						m_oControl = oSubReader;
					else if (L"w:objectEmbed" == sName)
						m_oOleObject = oSubReader;
					else if (L"w:drawing" == sName)
						m_oDrawing = oSubReader;

					break;
				}

				if (pItem)
				{
					m_arrItems.push_back(pItem);
				}
			}
		}
		std::wstring CObject::toXML() const
		{
			return L"<w:object/>";
		}	
		EElementType CObject::getType() const
		{
			return et_w_object;
		}
		void CObject::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if		( oReader, L"w:dxaOrig", m_oDxaOrig )
			WritingElement_ReadAttributes_Read_else_if	( oReader, L"w:dyaOrig", m_oDyaOrig )
			WritingElement_ReadAttributes_End( oReader )
		}

	} // namespace Logic
} // namespace OOX
