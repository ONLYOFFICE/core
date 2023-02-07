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

#include "VMLPictureMapping.h"
#include "VMLShapeMapping.h"

#include "OfficeDrawing/MetafilePictBlip.h"

#include "../../DesktopEditor/common/StringExt.h"
#include "../../OOXML/DocxFormat/Document.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/raster/BgraFrame.h"

#include "../PptFile/Reader/ReadStructures.h"

#include "../../OOXML/DocxFormat/Math/oMathPara.h"
#include "../../OOXML/DocxFormat/Logic/Paragraph.h"

using namespace DocFileFormat;


namespace DocFileFormat
{
    struct __BITMAPINFOHEADER
    {
        _UINT32     biSize;
        _INT32      biWidth;
        _INT32      biHeight;
        _UINT16     biPlanes;
        _UINT16     biBitCount;
        _UINT32     biCompression;
        _UINT32     biSizeImage;
        _INT32      biXPelsPerMeter;
        _INT32      biYPelsPerMeter;
        _UINT32     biClrUsed;
        _UINT32     biClrImportant;
    };

    struct __BITMAPCOREHEADER
    {
        _UINT32     bcSize;                 /* used to get to color table */
        _UINT16     bcWidth;
        _UINT16     bcHeight;
        _UINT16     bcPlanes;
        _UINT16     bcBitCount;
    };
    Global::BlipType GetFormatPict(unsigned char* data, int size)
	{
		Global::BlipType btWin32 = Global::msoblipDIB;
				 
		int offset = 0, biSizeImage = 0;

        __BITMAPINFOHEADER * header = (__BITMAPINFOHEADER*)data;
		if (!header) return btWin32;

		if (header->biWidth > 100000 || header->biHeight > 100000 || header->biSize != 40)
		{
            __BITMAPCOREHEADER * header_core = (__BITMAPCOREHEADER *)data;

			if (header_core->bcSize != 12)
			{
				btWin32 = Global::msoblipWMF;
			}
			else
			{
				offset = 12; //sizeof(BITMAPCOREHEADER)		
				int stride =  (size - offset) / header_core->bcHeight;

				biSizeImage = size - offset;
				
				if (stride >= header_core->bcWidth && header_core->bcBitCount >=24 )
				{
					btWin32 = Global::msoblipPNG;	
				}
			}
		}
		else
		{
			offset = 40; //sizeof(BITMAPINFOHEADER)
			int sz_bitmap = header->biHeight * header->biWidth * header->biBitCount/ 8;
			
			int stride = (size - offset) / header->biHeight;

			if (stride >= header->biWidth && header->biBitCount >= 24)
			{
				btWin32 = Global::msoblipPNG;	
			}
		}
		return btWin32;
	}
	bool VMLPictureMapping::ParseEmbeddedBlob( const std::string & xmlString, std::wstring & newXmlString)
	{
		newXmlString.clear();

		std::wstring sTempFolder = m_context->_doc->m_sTempFolder;
		if (sTempFolder.empty())
		{
            sTempFolder = NSFile::CFileBinary::GetTempPath();
		}

		std::wstring sTempXmlFile = NSDirectory::CreateTempFileWithUniqueName(sTempFolder, L"emb");

		NSFile::CFileBinary file; 
		file.CreateFileW(sTempXmlFile);
		file.WriteFile((BYTE*)xmlString.c_str(), xmlString.size());
		file.CloseFile();

		COfficeUtils officeUtils(NULL);

		BYTE *utf8Data = NULL; 
		ULONG utf8DataSize = 0;
		if (S_OK != officeUtils.LoadFileFromArchive(sTempXmlFile, L"drs/shapexml.xml", &utf8Data, utf8DataSize))
		{
			if (S_OK == officeUtils.IsFileExistInArchive(sTempXmlFile, L"drs/diagrams"))
			{
				officeUtils.LoadFileFromArchive(sTempXmlFile, L"drs/diagrams/drawing1.xml", &utf8Data, utf8DataSize);
			}
			else if (S_OK != officeUtils.LoadFileFromArchive(sTempXmlFile, L"drs/e2oDoc.xml", &utf8Data, utf8DataSize))
			{

			}
		}

		if (utf8Data && utf8DataSize > 0)
		{
			newXmlString = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(utf8Data, utf8DataSize);

			delete []utf8Data;
		}
		NSFile::CFileBinary::Remove(sTempXmlFile);

		if (newXmlString.empty()) return false;

		return true;
	}
	bool VMLPictureMapping::ParseEmbeddedEquation(std::pair<boost::shared_array<char>, size_t> & data, std::wstring & xmlString)
	{
		xmlString.clear();

		if (data.second == 0 || !data.first) return false;

		std::wstring sTempFolder = m_context->_doc->m_sTempFolder;
		if (sTempFolder.empty())
		{
            sTempFolder = NSFile::CFileBinary::GetTempPath();
		}

		std::wstring sTempXmlFile = NSDirectory::CreateTempFileWithUniqueName(sTempFolder, L"emb");

		NSFile::CFileBinary file; 
		file.CreateFileW(sTempXmlFile);
		file.WriteFile((BYTE*)data.first.get(), data.second);
		file.CloseFile();

		OOX::CPath path(sTempXmlFile);
		OOX::CDocument docEmbedded(NULL, path, path);

		bool res = false;
        for (std::vector<OOX::WritingElement*>::iterator it = docEmbedded.m_arrItems.begin(); it != docEmbedded.m_arrItems.end(); ++it)
		{
			if ((*it)->getType() == OOX::et_w_p)
			{
				OOX::Logic::CParagraph *paragraph = dynamic_cast<OOX::Logic::CParagraph *>(*it);

                for (std::vector<OOX::WritingElement*>::iterator jt = paragraph->m_arrItems.begin();
						(paragraph) && (jt != paragraph->m_arrItems.end()); jt++)
				{
					if ((*jt)->getType() == OOX::et_m_oMath)
					{
						res = true;
						xmlString = (*jt)->toXML();
						break;
					}
					else if ((*jt)->getType() == OOX::et_m_oMathPara)
					{
						OOX::Logic::COMathPara *mathPara = dynamic_cast<OOX::Logic::COMathPara *>(*jt);
						
                        for (std::vector<OOX::WritingElement*>::iterator kt = mathPara->m_arrItems.begin();
														(mathPara) && (kt != mathPara->m_arrItems.end()); kt++)
						{
							if ((*kt)->getType() == OOX::et_m_oMath)
							{
								res = true;
								xmlString = (*kt)->toXML();
								break;
							}
						}
					}
					if (res) break;
				}
				if (res) break;
			}
		}
		NSFile::CFileBinary::Remove(sTempXmlFile);
		return res;
	}
//---------------------------------------------------------------
	void VMLPictureMapping::appendStyleProperty(std::wstring& style, const std::wstring& propName, const std::wstring& propValue) const
	{
		style += ( propName );
		style += ( L":" );
		style += ( propValue );
		style +=( L";" );
	}

	VMLPictureMapping::VMLPictureMapping(ConversionContext* ctx, XMLTools::CStringXmlWriter* writer, bool olePreview, IMapping* caller, bool isInlinePicture, bool inGroup) : PropertiesMapping(writer)
	{
		m_context			=	ctx;
		m_isOlePreview		=	olePreview;
		m_imageData			=	NULL;
		m_nImageId			=	0;
		m_caller			=	caller;
		m_isInlinePicture	=	isInlinePicture;
		m_inGroup			=	inGroup;

        m_imageData			=	new XMLTools::XMLElement( L"v:imagedata" );
	}

	VMLPictureMapping::~VMLPictureMapping()
	{
		RELEASEOBJECT(m_imageData);
	}
	std::wstring VMLPictureMapping::GetShapeID(const Shape* pShape) const
	{
		std::wstring strXmlAttr;

		if (NULL != pShape)
		{
			strXmlAttr += std::wstring(L"_x0000_i");
			strXmlAttr += FormatUtils::IntToWideString(pShape->GetShapeID());
		}

		return strXmlAttr;
	}
	void VMLPictureMapping::Apply( IVisitable* visited  )
	{
		PictureDescriptor* pict = dynamic_cast<PictureDescriptor*>(visited);
		if (!pict) return;

		double xScaling = pict->mx / 1000.0;
		double yScaling = pict->my / 1000.0;

		TwipsValue width( ( pict->dxaGoal - ( pict->dxaCropLeft + pict->dxaCropRight ) ) * xScaling );
		TwipsValue height( ( pict->dyaGoal - ( pict->dyaCropTop + pict->dyaCropBottom ) ) * yScaling );

		std::wstring strWidth = FormatUtils::DoubleToWideString( width.ToPoints() );
		std::wstring strHeight = FormatUtils::DoubleToWideString( height.ToPoints() );
		std::wstring strStyle;
	
		std::vector<ODRAW::OfficeArtFOPTEPtr> options;
		
		PictureFrameType type;
		Shape* pShape = NULL;
		if ((pict->shapeContainer || pict->blipStoreEntry) && pict->shapeContainer->Children.size() > 0)
		{
			pShape	=	static_cast<Shape*>(*(pict->shapeContainer->Children.begin()));
			options	=	pict->shapeContainer->ExtractOptions();

			//v:shapetype
			type.SetType(pShape->Instance);
			
			VMLShapeTypeMapping* vmlShapeTypeMapping = new VMLShapeTypeMapping( m_pXmlWriter, m_isInlinePicture );

			type.Convert( vmlShapeTypeMapping );
			RELEASEOBJECT( vmlShapeTypeMapping );

		}
		else if (pict->embeddedData)
		{
			type.SetType(msosptPictureFrame);
		}
		m_pXmlWriter->WriteNodeBegin( L"v:shape", true );		

		//m_shapeId = GetShapeID(pShape); - todooo одинаковые картинки (одинаковый spid) - Anexo№3.doc
		

		if (m_shapeId.empty())
		{
			m_context->_doc->GetOfficeArt()->m_uLastShapeId++;
			m_shapeId =	L"_x0000_i" + FormatUtils::IntToWideString(m_context->_doc->GetOfficeArt()->m_uLastShapeId);
		}
		
		m_pXmlWriter->WriteAttribute( L"id", m_shapeId);
		
		m_pXmlWriter->WriteAttribute( L"type", std::wstring( L"#" + VMLShapeTypeMapping::GenerateTypeId(&type)));

//todooo oбъединить с shape_mapping		

		for (size_t i = 0; i < options.size(); i++)
		{
			ODRAW::OfficeArtFOPTEPtr & iter = options[i];
			switch (iter->opid)
			{
			case ODRAW::wzEquationXML:
				{
					ODRAW::XmlString *pXml = dynamic_cast<ODRAW::XmlString*>(iter.get());
					if (pXml)
					{
						m_isEquation = true;
						m_isEmbedded = true;

						m_embeddedData = pXml->data;

						if (ParseEmbeddedEquation(m_embeddedData, m_equationXml))
						{
							m_isEmbedded = false;
						}
					}
				}break;
			case ODRAW::metroBlob:
				{//встроенная неведомая хуйня
					ODRAW::MetroBlob* blob = dynamic_cast<ODRAW::MetroBlob*>(iter.get());
					if (blob)
					{
						m_isBlob = true;
						m_isEmbedded = true;
						//if (ParseEmbeddedBlob( blob->data.first, blob->data.second)) // todoooo
						//{
						//	m_isEmbedded = false;
						//}
					}
				}break;
			//BORDERS
			case ODRAW::borderBottomColor:
				if (!pict->brcBottom)
				{
					ODRAW::OfficeArtCOLORREF bottomColor((_UINT32)iter->op);
					m_context->_doc->CorrectColor(bottomColor);
					if (false == bottomColor.sColorRGB.empty())
						m_pXmlWriter->WriteAttribute(L"o:borderbottomcolor", L"#" + bottomColor.sColorRGB);
				}
				break;
			case ODRAW::borderLeftColor:
				if (!pict->brcLeft)
				{
					ODRAW::OfficeArtCOLORREF leftColor((_UINT32)iter->op);
					m_context->_doc->CorrectColor(leftColor);
					if (false == leftColor.sColorRGB.empty())
						m_pXmlWriter->WriteAttribute(L"o:borderleftcolor", L"#" + leftColor.sColorRGB);
				}
				break;
			case ODRAW::borderRightColor:
				if (!pict->brcRight)
				{
					ODRAW::OfficeArtCOLORREF rightColor((_UINT32)iter->op);
					m_context->_doc->CorrectColor(rightColor);
					if (false == rightColor.sColorRGB.empty())
						m_pXmlWriter->WriteAttribute(L"o:borderrightcolor", L"#" + rightColor.sColorRGB);
				}
				break;
			case ODRAW::borderTopColor:
				if (!pict->brcTop)
				{
					ODRAW::OfficeArtCOLORREF topColor((_UINT32)iter->op);
					m_context->_doc->CorrectColor(topColor);
					if (false == topColor.sColorRGB.empty())
						m_pXmlWriter->WriteAttribute(L"o:bordertopcolor", L"#" + topColor.sColorRGB);
				}
				break;
				//CROPPING
			case ODRAW::cropFromBottom:
				{
					//cast to signed integer
					int cropBottom = (int)iter->op;
					appendValueAttribute(m_imageData, L"cropbottom", FormatUtils::IntToWideString(cropBottom) + L"f");
				}
				break;
			case ODRAW::cropFromLeft:
				{
					//cast to signed integer
					int cropLeft = (int)iter->op;
					appendValueAttribute(m_imageData, L"cropleft", FormatUtils::IntToWideString(cropLeft) + L"f");
				}
				break;
			case ODRAW::cropFromRight:
				{
					//cast to signed integer
					int cropRight = (int)iter->op;
					appendValueAttribute(m_imageData, L"cropright", FormatUtils::IntToWideString(cropRight) + L"f");
				}
				break;
			case ODRAW::cropFromTop:
				{
					//cast to signed integer
					int cropTop = (int)iter->op;
					appendValueAttribute(m_imageData, L"croptop", FormatUtils::IntToWideString(cropTop) + L"f");
				}
				break;
			//------------------------------------------------------------
			case ODRAW::ePropertyId_rotation:
				{
					double dAngle = (double)((int)iter->op) / 65535.0;

					if (dAngle < -360.0)
						dAngle += 360.0;

					std::wstring v = strHeight;
					strHeight = strWidth; strWidth = v;

					appendStyleProperty(strStyle, L"rotation", FormatUtils::DoubleToWideString(dAngle));
				}break;
			case ODRAW::posh:
				{
					appendStyleProperty(strStyle, L"mso-position-horizontal", VMLShapeMapping::mapHorizontalPosition((PositionHorizontal)iter->op));
				}break;
			case ODRAW::posrelh:
				{
					if (false == m_inGroup)
						appendStyleProperty(strStyle, L"mso-position-horizontal-relative", VMLShapeMapping::mapHorizontalPositionRelative((PositionHorizontalRelative)iter->op));
				}break;
			case ODRAW::posv:
				{
					appendStyleProperty(strStyle, L"mso-position-vertical", VMLShapeMapping::mapVerticalPosition((PositionVertical)iter->op));
				}break;
			case ODRAW::posrelv:
				{
					if (false == m_inGroup)
						appendStyleProperty(strStyle, L"mso-position-vertical-relative", VMLShapeMapping::mapVerticalPositionRelative((PositionVerticalRelative)iter->op));
				}break;
			case ODRAW::groupShapeBooleanProperties:
				{
					ODRAW::GroupShapeBooleanProperties* booleans = dynamic_cast<ODRAW::GroupShapeBooleanProperties*>(iter.get());

					if (booleans->fUsefBehindDocument && booleans->fBehindDocument)
					{
						//The shape is behind the text, so the z-index must be negative.
						appendStyleProperty(strStyle, L"z-index", L"-1");
					}
					//else if (!m_isInlinePicture)
					//{
					//	appendStyleProperty( &strStyle, L"z-index", FormatUtils::IntToWideString(zIndex + 0x7ffff));
					//}

					if (booleans->fHidden && booleans->fUsefHidden)
					{
						appendStyleProperty(strStyle, L"visibility", L"hidden");
					}
				}break;
			case ODRAW::blipBooleanProperties:
				{
					ODRAW::BlipBooleanProperties * bools = (ODRAW::BlipBooleanProperties *)(iter.get());
					if (bools)
					{
						if (bools->fUsefPictureGray && bools->fPictureGray)
							appendValueAttribute(m_imageData, L"grayscale", L"t");
						if (bools->fUsefPictureBiLevel && bools->fPictureBiLevel)
							appendValueAttribute(m_imageData, L"bilevel", L"t");
					}
				}break;
			default:
				{
				}break;
			}
		}

		strStyle +=  L"width:"  + strWidth + L"pt;" + L"height:" + strHeight + L"pt;";

		m_pXmlWriter->WriteAttribute( L"style", strStyle);

		if (m_isOlePreview)
		{
			m_pXmlWriter->WriteAttribute( L"o:ole", L"t" );
		}
		else if (m_isBullete)
		{
            m_pXmlWriter->WriteAttribute( L"o:bullet", L"1" );
		}

		{//borders color		
			if (pict->brcTop)
				m_pXmlWriter->WriteAttribute( L"o:bordertopcolor", 
					pict->brcTop->ico.empty() ? FormatUtils::IntToFormattedWideString(pict->brcTop->cv, L"#%06x") : pict->brcTop->ico);
			if (pict->brcLeft)
				m_pXmlWriter->WriteAttribute( L"o:borderleftcolor", 
					pict->brcTop->ico.empty() ? FormatUtils::IntToFormattedWideString(pict->brcLeft->cv, L"#%06x") : pict->brcLeft->ico);
			if (pict->brcBottom)
				m_pXmlWriter->WriteAttribute( L"o:borderbottomcolor", 
					pict->brcTop->ico.empty() ? FormatUtils::IntToFormattedWideString(pict->brcBottom->cv, L"#%06x") : pict->brcBottom->ico);
			if (pict->brcRight)
				m_pXmlWriter->WriteAttribute( L"o:borderrightcolor", 
					pict->brcTop->ico.empty() ? FormatUtils::IntToFormattedWideString(pict->brcRight->cv, L"#%06x") : pict->brcRight->ico);

		}
		m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
		
		//v:imageData
		if (CopyPicture(pict))
		{
			appendValueAttribute(m_imageData, L"r:id", L"rId" + FormatUtils::IntToWideString(m_nImageId));
			appendValueAttribute(m_imageData, L"o:title", L"" );
			m_pXmlWriter->WriteString(m_imageData->GetXMLString());
		}
		else
			m_isPictureBroken = true;

		{//borders			
			writePictureBorder( L"bordertop",		pict->brcTop );
			writePictureBorder( L"borderleft",		pict->brcLeft );
			writePictureBorder( L"borderbottom",	pict->brcBottom );
			writePictureBorder( L"borderright",		pict->brcRight );
		}

		m_pXmlWriter->WriteNodeEnd( L"v:shape" );
	}

	void VMLPictureMapping::writePictureBorder( const std::wstring & name, const BorderCode* brc )
	{
		if (!brc || name.empty()) return;

		m_pXmlWriter->WriteNodeBegin( L"w10:" + name, true );
		m_pXmlWriter->WriteAttribute( L"type", getBorderType( brc->brcType ));
		m_pXmlWriter->WriteAttribute( L"width", FormatUtils::IntToWideString( brc->dptLineWidth ));
		m_pXmlWriter->WriteNodeEnd	( L"", true );
	}

	bool VMLPictureMapping::CopyPicture (PictureDescriptor* pict)
	{
		if (!pict) return false;
		bool result = false;

		BlipStoreEntry* oBlipEntry = pict->blipStoreEntry;

		if (pict->embeddedData && pict->embeddedDataSize > 0)
		{
			Global::BlipType btWin32 = GetFormatPict(pict->embeddedData, pict->embeddedDataSize); 

			if (btWin32 == Global::msoblipWMF)
			{
				CMetaHeader oMetaHeader;
		
				oMetaHeader.rcBounds.right  = pict->mfp.xExt ;
				oMetaHeader.rcBounds.bottom = pict->mfp.yExt ;

				WmfPlaceableFileHeader oWmfHeader = {};
				oMetaHeader.ToWMFHeader(&oWmfHeader);
				
				int lLenHeader = 22 + (pict->embeddedDataHeader ? 114 : 0);

				unsigned char *newData = new unsigned char[pict->embeddedDataSize + lLenHeader];
				
				memcpy(newData, (unsigned char *)(&oWmfHeader), 22);

				if (pict->embeddedDataHeader)
				{
					memcpy(newData + 22, pict->embeddedDataHeader, 114 );
				}
				
				memcpy(newData + lLenHeader, pict->embeddedData, pict->embeddedDataSize);

				pict->embeddedDataSize += lLenHeader;
				delete []pict->embeddedData;
				pict->embeddedData = newData;
			}

			m_context->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(btWin32), 
				pict->embeddedData, pict->embeddedDataSize, btWin32));
			
			m_nImageId	=	m_context->_docx->RegisterImage(m_caller, btWin32);
			result	=	true;
		}
		else if ((oBlipEntry != NULL) && (oBlipEntry->Blip != NULL))
		{
			switch (oBlipEntry->btWin32)
			{
				case Global::msoblipEMF:
				case Global::msoblipWMF:
				case Global::msoblipPICT:
				{
					MetafilePictBlip* metaBlip = static_cast<MetafilePictBlip*>(oBlipEntry->Blip);
					if (metaBlip)
					{//decompress inside MetafilePictBlip
						unsigned char *newData	= NULL;
						unsigned int newDataSize = metaBlip->oMetaFile.ToBuffer(newData);

						boost::shared_array<unsigned char> arData(newData);						
						m_context->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), arData, newDataSize));
					}
				}break;
				case Global::msoblipDIB:
				{//user_manual_v52.doc
				
  					BitmapBlip* bitBlip = static_cast<BitmapBlip*>(oBlipEntry->Blip);
					if (bitBlip)
					{
						std::wstring file_name = m_context->_doc->m_sTempFolder + FILE_SEPARATOR_STR + L"tmp_image";
						oBlipEntry->btWin32 = ImageHelper::SaveImageToFileFromDIB(bitBlip->m_pvBits, bitBlip->pvBitsSize, file_name);

						unsigned char* pData = NULL;
						DWORD nData = 0;
						if (NSFile::CFileBinary::ReadAllBytes(file_name, &pData, nData))
						{
							m_context->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), 
								boost::shared_array<unsigned char>(pData), nData, oBlipEntry->btWin32));
							
							break;
						}
					}				
				}
				case Global::msoblipJPEG:
				case Global::msoblipCMYKJPEG:
				case Global::msoblipPNG:
				case Global::msoblipTIFF:
				{
					BitmapBlip* bitBlip = static_cast<BitmapBlip*>(oBlipEntry->Blip);
					if (bitBlip)
					{
						m_context->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), 
							bitBlip->m_pvBits, bitBlip->pvBitsSize, oBlipEntry->btWin32));
					}
				}break;			

				default:
				{
					return false;
				}break;
			}

			m_nImageId	=	m_context->_docx->RegisterImage(m_caller, oBlipEntry->btWin32);
			result	=	true;
		}

		return result;
	}
}

namespace DocFileFormat
{
	std::wstring VMLPictureMapping::GetTargetExt (Global::BlipType nType)
	{
		switch (nType)
		{
		case Global::msoblipDIB:
			return std::wstring(L".bmp");

		case Global::msoblipEMF:
			return std::wstring(L".emf");

		case Global::msoblipJPEG:
		case Global::msoblipCMYKJPEG:
			return std::wstring(L".jpg");

		case Global::msoblipPNG:
			return std::wstring(L".png");

		case Global::msoblipTIFF:
			return std::wstring(L".tif");

		case Global::msoblipWMF:
			return std::wstring(L".wmf");

		case Global::msoblipPICT:
			return std::wstring(L".pcz");

		default:
			return std::wstring(L".png");
		}
	}

	std::wstring VMLPictureMapping::GetContentType (Global::BlipType nType)
	{
		switch (nType)
		{
		case Global::msoblipEMF:
			return std::wstring(OpenXmlContentTypes::Emf);

			//case msoblipGIF:
			//  return wstring( L"image/gif" );

			//case msoblipICON:
			//  return wstring( L"image/x-icon" );

		case Global::msoblipJPEG:
		case Global::msoblipCMYKJPEG:
			return std::wstring(OpenXmlContentTypes::Jpeg);

			//case msoblipPCX:
			//  return wstring( L"image/pcx" );

		case Global::msoblipPNG:
			return std::wstring(OpenXmlContentTypes::Png);

		case Global::msoblipTIFF:
			return std::wstring(OpenXmlContentTypes::Tiff);

		case Global::msoblipWMF:
			return std::wstring(OpenXmlContentTypes::Wmf);

		case Global::msoblipPICT:
			return std::wstring(OpenXmlContentTypes::Pcz);

		case Global::msoblipDIB:
			return std::wstring(OpenXmlContentTypes::Bmp);
		
		default:
			return std::wstring(OpenXmlContentTypes::Png);
		}
	}
}
