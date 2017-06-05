/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "VMLPictureMapping.h"
#include "VMLShapeMapping.h"

#include "OfficeDrawing/GeometryBooleanProperties.h"
#include "OfficeDrawing/GeometryTextBooleanProperties.h"
#include "OfficeDrawing/GroupShapeBooleanProperties.h"
#include "OfficeDrawing/MetafilePictBlip.h"

#include "../../DesktopEditor/common/String.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Document.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/raster/BgraFrame.h"

typedef struct
{
    DWORD   iType;              // Record type EMR_HEADER
    DWORD   nSize;              // Record size in bytes.  This may be greater
                                // than the sizeof(ENHMETAHEADER).
    RECT   rclBounds;          // Inclusive-inclusive bounds in device units
    RECT   rclFrame;           // Inclusive-inclusive Picture Frame .01mm unit
    DWORD   dSignature;         // Signature.  Must be ENHMETA_SIGNATURE.
    DWORD   nVersion;           // Version number
    DWORD   nBytes;             // Size of the metafile in bytes
    DWORD   nRecords;           // Number of records in the metafile
    WORD    nHandles;           // Number of handles in the handle table
                                // Handle index zero is reserved.
    WORD    sReserved;          // Reserved.  Must be zero.
    DWORD   nDescription;       // Number of chars in the unicode desc string
                                // This is 0 if there is no description string
    DWORD   offDescription;     // Offset to the metafile description record.
                                // This is 0 if there is no description string
    DWORD   nPalEntries;        // Number of entries in the metafile palette.
    SIZE   szlDevice;          // Size of the reference device in pels
    SIZE   szlMillimeters;     // Size of the reference device in millimeters
} ENHMETAHEADER3;

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
	bool VMLPictureMapping::ParseEmbeddedEquation( const std::string & xmlString, std::wstring & newXmlString)
	{
		newXmlString.clear();

		std::wstring sTempFolder = m_ctx->_doc->m_sTempFolder;
		if (sTempFolder.empty())
		{
            sTempFolder = NSFile::CFileBinary::GetTempPath();
		}

		std::wstring sTempXmlFile = NSDirectory::CreateTempFileWithUniqueName(sTempFolder, L"emb");

		NSFile::CFileBinary file; 
		file.CreateFileW(sTempXmlFile);
		file.WriteFile((BYTE*)xmlString.c_str(), xmlString.size());
		file.CloseFile();

		OOX::CPath path(sTempXmlFile);
		OOX::CDocument docEmbedded(path, path);

		bool res = false;
		for (int i = 0 ; i < docEmbedded.m_arrItems.size(); i++)
		{
			if (docEmbedded.m_arrItems[i]->getType() == OOX::et_w_p)
			{
				OOX::Logic::CParagraph *paragraph = dynamic_cast<OOX::Logic::CParagraph *>(docEmbedded.m_arrItems[i]);

				for (int j = 0; (paragraph) && (j < paragraph->m_arrItems.size()); j++)
				{
					if (paragraph->m_arrItems[j]->getType() == OOX::et_m_oMath)
					{
						res = true;
                        newXmlString = paragraph->m_arrItems[j]->toXML();
						break;
					}
					else if (paragraph->m_arrItems[j]->getType() == OOX::et_m_oMathPara)
					{
						OOX::Logic::COMathPara *mathPara = dynamic_cast<OOX::Logic::COMathPara *>(paragraph->m_arrItems[j]);
						
						for (int k = 0; (mathPara) && (k < mathPara->m_arrItems.size()); k++)
						{
							if (mathPara->m_arrItems[k]->getType() == OOX::et_m_oMath)
							{
								res = true;
                                newXmlString = mathPara->m_arrItems[k]->toXML();
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
	void VMLPictureMapping::appendStyleProperty(std::wstring* b, const std::wstring& propName, const std::wstring& propValue) const
	{
		if ( b != NULL )
		{
			b->operator += ( propName );
			b->operator += ( L":" );
			b->operator += ( propValue );
			b->operator +=( L";" );
		}
	}

	VMLPictureMapping::VMLPictureMapping(ConversionContext* ctx, XMLTools::CStringXmlWriter* writer, bool olePreview, IMapping* caller, bool isInlinePicture) : PropertiesMapping(writer)
	{
		m_ctx				=	ctx;
		m_isOlePreview		=	olePreview;
		m_imageData			=	NULL;
		m_nImageId			=	0;
		m_caller			=	caller;
		m_isInlinePicture	=	isInlinePicture;
		
		m_isBullete			=	false;
		m_isEquation		=	false;
		m_isEmbedded		=	false;

        m_imageData			=	new XMLTools::XMLElement( L"v:imagedata" );
	}

	VMLPictureMapping::~VMLPictureMapping()
	{
		RELEASEOBJECT(m_imageData);
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
	
		std::vector<OptionEntryPtr> options;
		
		PictureFrameType type;
		if ((pict->shapeContainer || pict->blipStoreEntry) && pict->shapeContainer->Children.size() > 0)
		{
			Shape* shape	=	static_cast<Shape*>(*(pict->shapeContainer->Children.begin()));
			options			=	pict->shapeContainer->ExtractOptions();

			//v:shapetype
			type.SetType(shape->Instance);
			
			VMLShapeTypeMapping* vmlShapeTypeMapping = new VMLShapeTypeMapping( m_pXmlWriter, m_isInlinePicture );

			type.Convert( vmlShapeTypeMapping );
			RELEASEOBJECT( vmlShapeTypeMapping );
		}
		else if (pict->embeddedData)
		{
			type.SetType(msosptPictureFrame);
		}
		m_pXmlWriter->WriteNodeBegin( L"v:shape", true );
		
		m_pXmlWriter->WriteAttribute( L"type", std::wstring( L"#" + VMLShapeTypeMapping::GenerateTypeId(&type)));

		count_vml_objects++;

		if (m_shapeId.empty())
			m_shapeId =	L"_x0000_s" + FormatUtils::IntToWideString(1024 + count_vml_objects);
		
		m_pXmlWriter->WriteAttribute( L"id", m_shapeId);

		if (m_isOlePreview)
		{
			m_pXmlWriter->WriteAttribute( L"o:ole", L"" );
		}
		else if (m_isBullete)
		{
            m_pXmlWriter->WriteAttribute( L"o:bullet", L"1" );
		}
//todooo oбъединить с shape_mapping		

		for (size_t i = 0; i < options.size(); i++)
		{
			OptionEntryPtr & iter = options[i];
			switch ( iter->pid )
			{
			case wzEquationXML:
				{
					m_isEquation = true;
					m_isEmbedded = true;
					
					m_embeddedData = std::string((char*)iter->opComplex.get(), iter->op);

					if (ParseEmbeddedEquation( m_embeddedData, m_equationXml))
					{
						m_isEmbedded = false;
					}
				}break;
			case metroBlob:
				{
					//встроенная неведомая хуйня
					m_isEmbedded	= true;
					m_embeddedData	= std::string((char*)iter->opComplex.get(), iter->op);
					
					//if (ParseEmbeddedBlob( m_embeddedData, m_blobXml)) // todoooo
					//{
					//	m_isEmbedded = false;
					//}
				}break;
//BORDERS
			case borderBottomColor:
				{
					RGBColor bottomColor( (int)iter->op, RedFirst );
					m_pXmlWriter->WriteAttribute( L"o:borderbottomcolor", L"#" + bottomColor.SixDigitHexCode);
				}
				break;
			case borderLeftColor:
				{  
					RGBColor leftColor( (int)iter->op, RedFirst );
					m_pXmlWriter->WriteAttribute( L"o:borderleftcolor", L"#" + leftColor.SixDigitHexCode);
				}  
				break;
			case borderRightColor:
				{  
					RGBColor rightColor( (int)iter->op, RedFirst );
					m_pXmlWriter->WriteAttribute( L"o:borderrightcolor",  L"#" + rightColor.SixDigitHexCode);
				}
				break;
			case borderTopColor:
				{
					RGBColor topColor( (int)iter->op, RedFirst );
					m_pXmlWriter->WriteAttribute( L"o:bordertopcolor", L"#" + topColor.SixDigitHexCode);
				}
				break;
//CROPPING
			case cropFromBottom:
				{  
					//cast to signed integer
					int cropBottom = (int)iter->op;
					appendValueAttribute(m_imageData, L"cropbottom", FormatUtils::IntToWideString( cropBottom ) + L"f" );
				}
				break;
			case cropFromLeft:
				{  
					//cast to signed integer
					int cropLeft = (int)iter->op;
					appendValueAttribute(m_imageData, L"cropleft", FormatUtils::IntToWideString( cropLeft ) + L"f" );
				}
				break;
			case cropFromRight:
				{
					//cast to signed integer
					int cropRight = (int)iter->op;
					appendValueAttribute(m_imageData, L"cropright", FormatUtils::IntToWideString( cropRight ) + L"f" );
				}
				break;
			case cropFromTop:
				{
					//cast to signed integer
					int cropTop = (int)iter->op;
					appendValueAttribute(m_imageData, L"croptop", FormatUtils::IntToWideString( cropTop ) + L"f" );
				}
				break;
//------------------------------------------------------------
			case PropertyId_rotation:
				{
					double dAngle = (double)((int)iter->op) / 65535.0;

					if (dAngle < -360.0)
						dAngle += 360.0;

					std::wstring v = strHeight;
					strHeight = strWidth; strWidth = v;

					appendStyleProperty(&strStyle, L"rotation", FormatUtils::DoubleToWideString(dAngle));
				}break;
			case posh:
				{
					appendStyleProperty(&strStyle, L"mso-position-horizontal", VMLShapeMapping::mapHorizontalPosition((PositionHorizontal)iter->op));
				}break;
			case posrelh:
				{
					appendStyleProperty(&strStyle, L"mso-position-horizontal-relative", VMLShapeMapping::mapHorizontalPositionRelative((PositionHorizontalRelative)iter->op));
				}break;
			case posv:
				{
					appendStyleProperty(&strStyle, L"mso-position-vertical", VMLShapeMapping::mapVerticalPosition((PositionVertical)iter->op));
				}break;
			case posrelv:
				{
					appendStyleProperty(&strStyle, L"mso-position-vertical-relative", VMLShapeMapping::mapVerticalPositionRelative((PositionVerticalRelative)iter->op));
				}break;
			case groupShapeBooleans:
				{
					GroupShapeBooleanProperties groupShapeBooleans(iter->op);

					if (groupShapeBooleans.fUsefBehindDocument && groupShapeBooleans.fBehindDocument)
					{
						//The shape is behind the text, so the z-index must be negative.
						appendStyleProperty(&strStyle, L"z-index", L"-1" );
					}
					//else if (!m_isInlinePicture)
					//{
					//	appendStyleProperty( &strStyle, L"z-index", FormatUtils::IntToWideString(zIndex + 0x7ffff));
					//}

					if (groupShapeBooleans.fHidden && groupShapeBooleans.fUsefHidden)
					{
						appendStyleProperty(&strStyle, L"visibility", L"hidden");
					}
				}
				break;
			}
		}

		strStyle +=  L"width:"  + strWidth + L"pt;" + L"height:" + strHeight + L"pt;";
		m_pXmlWriter->WriteAttribute( L"style", strStyle);

		m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );
		
		if (CopyPicture(pict))
		{
			//v:imageData
			appendValueAttribute(m_imageData, L"r:id", L"rId" + FormatUtils::IntToWideString(m_nImageId));
			appendValueAttribute(m_imageData, L"o:title", L"" );
			m_pXmlWriter->WriteString(m_imageData->GetXMLString());
		}

		{//borders			
			writePictureBorder( L"bordertop",		pict->brcTop );
			writePictureBorder( L"borderleft",		pict->brcLeft );
			writePictureBorder( L"borderbottom",	pict->brcBottom );
			writePictureBorder( L"borderright",	pict->brcRight );
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
				
				int lLenHeader = 114 + 22;

				unsigned char *newData = new unsigned char[pict->embeddedDataSize + lLenHeader];
				
				memcpy(newData, (unsigned char *)(&oWmfHeader), 22);
				memcpy(newData + 22, pict->embeddedDataHeader, 114 );
				
				memcpy(newData + lLenHeader, pict->embeddedData, pict->embeddedDataSize);

				pict->embeddedDataSize += lLenHeader;
				delete []pict->embeddedData;
				pict->embeddedData = newData;

			}
			m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(Global::msoblipDIB), 
					std::vector<unsigned char>(pict->embeddedData, (pict->embeddedData + pict->embeddedDataSize)), Global::msoblipDIB));
			
			m_nImageId	=	m_ctx->_docx->RegisterImage(m_caller, btWin32);
			result	=	true;
		}
		else if ((oBlipEntry != NULL) && (oBlipEntry->Blip != NULL))
		{
			switch (oBlipEntry->btWin32)
			{
				case Global::msoblipEMF:
				case Global::msoblipWMF:
				{
					MetafilePictBlip* metaBlip = static_cast<MetafilePictBlip*>(oBlipEntry->Blip);
					if (metaBlip)
					{//decompress inside MetafilePictBlip
						unsigned char *newData	= NULL;
						int newDataSize = metaBlip->oMetaFile.ToBuffer(newData);
						
						m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), std::vector<unsigned char>(newData, (newData + newDataSize))));
						
						RELEASEARRAYOBJECTS(newData);
					}
				}
				break;

				case Global::msoblipJPEG:
				case Global::msoblipCMYKJPEG:
				case Global::msoblipPNG:
				case Global::msoblipTIFF:
				case Global::msoblipDIB:
				{
					BitmapBlip* bitBlip = static_cast<BitmapBlip*>(oBlipEntry->Blip);
					if (bitBlip)
					{
						m_ctx->_docx->ImagesList.push_back(ImageFileStructure(GetTargetExt(oBlipEntry->btWin32), 
							std::vector<unsigned char>(bitBlip->m_pvBits, (bitBlip->m_pvBits + bitBlip->pvBitsSize)), oBlipEntry->btWin32));
					}
				}break;			

				default:
				{
					return false;
				}break;
			}

			m_nImageId	=	m_ctx->_docx->RegisterImage(m_caller, oBlipEntry->btWin32);
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

		case Global::msoblipDIB:
			return std::wstring(OpenXmlContentTypes::Bmp);
		
		default:
			return std::wstring(OpenXmlContentTypes::Png);
		}
	}
}
