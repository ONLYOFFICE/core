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
#include "Shapes.h"

#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/raster/ImageFileFormatChecker.h"
#include "../../DesktopEditor/graphics/pro/Image.h"
#include "../../DesktopEditor/raster/BgraFrame.h"

#include "../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../Binary/Presentation/XmlWriter.h"
#include "../Binary/Presentation/imagemanager.h"
#include "../DocxFormat/Media/OleObject.h"
#include "../DocxFormat/Media/Image.h"

#include "../../Common/OfficeFileFormatChecker.h"

namespace OOX
{
	namespace Draw
	{
		EElementType CRel::getType() const
		{
			return et_dr_Rel;
		}
		void CRel::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "id", Rid)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CRel::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;
		}
		void CRel::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Rel");
			pWriter->StartAttributes();
			if (Rid.IsInit())
				pWriter->WriteAttribute2(L"r:id", Rid->ToString());
			pWriter->EndAttributes();
			pWriter->WriteNodeEnd(L"Rel");
		}
		EElementType CIcon::getType() const
		{
			return et_dr_Icon;
		}
		void CIcon::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			content = oReader.GetText2();
		}
		void CIcon::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteString1(0, content);
		}
		void CIcon::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			
			content = pReader->GetString2();
			
			pReader->Seek(_end_rec);
		}
		void CIcon::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Icon");
			pWriter->EndAttributes();
			pWriter->WriteString(content);
			pWriter->WriteNodeEnd(L"Icon");
		}
		EElementType CForeignData::getType() const
		{
			return et_dr_ForeignData;
		}
		void CForeignData::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "ForeignType", ForeignType)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "CompressionType", CompressionType)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "CompressionLevel", CompressionLevel)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ObjectType", ObjectType)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ShowAsIcon", ShowAsIcon)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ObjectWidth", ObjectWidth)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ObjectHeight", ObjectHeight)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ExtentX", ExtentX)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ExtentY", ExtentY)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		nullable<SimpleTypes::Draw::CCompressionType> CompressionType;
		nullable_double CompressionLevel;
		void CForeignData::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"Rel" == sName)
				{
					Rel = oReader;
				}
			}
		}
		void CForeignData::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			if (ForeignType.IsInit()) pWriter->WriteByte1(0, ForeignType->GetValue());
			pWriter->WriteUInt2(1, ObjectType);
			pWriter->WriteBool2(2, ShowAsIcon);
			pWriter->WriteDoubleReal2(3, ObjectWidth);
			pWriter->WriteDoubleReal2(4, ObjectHeight);
			pWriter->WriteDoubleReal2(5, ExtentX);
			pWriter->WriteDoubleReal2(6, ExtentY);
			if (CompressionType.IsInit()) pWriter->WriteByte1(7, CompressionType->GetValue());
			pWriter->WriteDoubleReal2(8, CompressionLevel);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			if (Rel.IsInit() && Rel->Rid.IsInit())
			{
				CPath out = pWriter->m_pCommon->m_pMediaManager->m_strDstMedia;

				smart_ptr<OOX::File> pFile = pWriter->GetRels()->Find(Rel->Rid->GetValue());
				OOX::OleObject* pOleObject = dynamic_cast<OOX::OleObject*>(pFile.GetPointer());

				CPath image_path;
				CPath ole_path;
				if (pOleObject)
				{
					image_path = pOleObject->filename_cache();
					ole_path = pOleObject->filename();
				}
				else
				{
					OOX::Media* pMedia = dynamic_cast<OOX::Media*>(pFile.GetPointer());
					if (pMedia)
					{
						image_path = pMedia->filename();
					}
				}
				std::wstring image_name_new;
				if (false == image_path.GetPath().empty())
				{
					//todooo check correct path inside container
					image_name_new = image_path.GetFilename();
					std::wstring image_ext_new = image_path.GetExtention();

					std::map<std::wstring, NSShapeImageGen::CMediaInfo>::iterator pFind = pWriter->m_pCommon->m_pMediaManager->m_mapMediaFiles.find(image_path.GetFilename());
					if (pFind == pWriter->m_pCommon->m_pMediaManager->m_mapMediaFiles.end())
					{
						NSShapeImageGen::CMediaInfo oInfo;
						oInfo.m_lID = ++pWriter->m_pCommon->m_pMediaManager->m_lNextIDImage;
						oInfo.m_sExt = image_path.GetExtention();
						image_name_new = oInfo.m_sName = L"image" + std::to_wstring(oInfo.m_lID);

						pWriter->m_pCommon->m_pMediaManager->m_mapMediaFiles.insert(std::make_pair(image_path.GetFilename(), oInfo));
					}
					else
					{
						image_name_new = pFind->second.m_sName;
					}

					NSFile::CFileBinary::Copy(image_path.GetPath(), out.GetPath() + FILE_SEPARATOR_STR + image_name_new + image_ext_new);

					pWriter->StartRecord(1);
					pWriter->WriteString(image_name_new + image_ext_new);
					pWriter->EndRecord();

					CImageFileFormatChecker checker;
					if (checker.isImageFile(image_path.GetPath()))
					{
						if (checker.eFileType == _CXIMAGE_FORMAT_WMF || checker.eFileType == _CXIMAGE_FORMAT_EMF)
						{
							MetaFile::IMetaFile* pMetafile = MetaFile::Create(pWriter->m_pCommon->m_pMediaManager->m_pFontManager->GetApplication());

							if (pMetafile->LoadFromFile(image_path.GetPath().c_str()))
							{
								double w = ExtentX.IsInit() ? *ExtentX : ObjectWidth.get_value_or(0);
								double h = ExtentY.IsInit() ? *ExtentY : ObjectHeight.get_value_or(0);

								std::wstring sInternalSvg = pMetafile->ConvertToSvg(w > 1 ? w : 0, h > 1 ? h : 0);

								if (!sInternalSvg.empty())
								{
									NSFile::CFileBinary::SaveToFile(out.GetPath() + FILE_SEPARATOR_STR + image_name_new + L".svg", sInternalSvg);
								}
								else
								{// не смогли сконвертировать в svg - пробуем в png									

									std::wstring strSaveItem = out.GetPath() + FILE_SEPARATOR_STR + image_name_new + L".png";
									pMetafile->ConvertToRaster(strSaveItem.c_str(), 4 /*CXIMAGE_FORMAT_PNG*/, 0, 0);
								}
							}
							RELEASEOBJECT(pMetafile);
						}
						else if (checker.eFileType != _CXIMAGE_FORMAT_JPG && checker.eFileType != _CXIMAGE_FORMAT_SVG && checker.eFileType != _CXIMAGE_FORMAT_PNG)
						{
							std::wstring strSaveItem = out.GetPath() + FILE_SEPARATOR_STR + image_name_new + L".png";

							CBgraFrame oFrame;
							if (true == oFrame.OpenFile(image_path.GetPath().c_str()))
							{
								oFrame.SaveFile(strSaveItem, 4); // png
							}
						}
					}
				}
				if (false == ole_path.GetPath().empty())
				{
					if (image_name_new.empty())
					{
						NSShapeImageGen::CMediaInfo oInfo;
						oInfo.m_lID = ++pWriter->m_pCommon->m_pMediaManager->m_lNextIDImage;
						image_name_new = oInfo.m_sName = L"image" + std::to_wstring(oInfo.m_lID);

						pWriter->m_pCommon->m_pMediaManager->m_mapMediaFiles.insert(std::make_pair(image_path.GetFilename(), oInfo));
					}
					std::wstring ole_name_new = image_name_new + ole_path.GetExtention();
					//todooo check correct path inside container
					NSFile::CFileBinary::Copy(ole_path.GetPath(), out.GetPath() + FILE_SEPARATOR_STR + ole_name_new);

					pWriter->StartRecord(2);
					pWriter->WriteString(ole_name_new);
					pWriter->EndRecord();
				}
			}
		}
		void CForeignData::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			std::wstring image_filename, ole_filename;

			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					ForeignType.Init();
					ForeignType->SetValueFromByte(pReader->GetUChar());
				}break;
				case 1:
				{
					ObjectType = pReader->GetULong();
				}break;
				case 2:
				{
					ShowAsIcon = pReader->GetBool();
				}break;
				case 3:
				{
					ObjectWidth = pReader->GetDoubleReal();
				}break;
				case 4:
				{
					ObjectHeight = pReader->GetDoubleReal();
				}break;
				case 5:
				{
					ExtentX = pReader->GetDoubleReal();
				}break;
				case 6:
				{
					ExtentY = pReader->GetDoubleReal();
				}break;
				case 7:
				{
					CompressionType.Init();
					CompressionType->SetValueFromByte(pReader->GetUChar());
				}break;
				case 8:
				{
					CompressionLevel = pReader->GetDoubleReal();
				}break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 1:
				{
					pReader->Skip(4);
					image_filename = pReader->GetString2();
				}break;
				case 2:
				{
					pReader->Skip(4);
					ole_filename = pReader->GetString2();
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);

			if (ole_filename.empty() == false)
			{
				int idOle = pReader->m_nCountEmbedded++; //todoooo -> countEmbeddedObjects
				int idImage = pReader->m_nCountImage++; 

				std::wstring ole_filename_dst = L"oleObject" + std::to_wstring(idOle) + CPath(ole_filename).GetExtention();
				std::wstring image_filename_dst = L"image" + std::to_wstring(idImage) + CPath(image_filename).GetExtention();

				std::wstring srcMedia = pReader->m_strFolder + FILE_SEPARATOR_STR + L"media" + FILE_SEPARATOR_STR;

				NSFile::CFileBinary::Copy(srcMedia + ole_filename, pReader->m_pRels->m_pManager->GetDstEmbed() + FILE_SEPARATOR_STR + ole_filename_dst);
				NSFile::CFileBinary::Copy(srcMedia + image_filename, pReader->m_pRels->m_pManager->GetDstMedia() + FILE_SEPARATOR_STR + image_filename_dst);

				OleObject* pOle = new OleObject(NULL, false, false);
				pOle->set_filename(pReader->m_pRels->m_pManager->GetDstEmbed() + FILE_SEPARATOR_STR + ole_filename_dst, false);
				pOle->set_filename_cache(pReader->m_pRels->m_pManager->GetDstMedia() + FILE_SEPARATOR_STR + image_filename_dst);

				COfficeFileFormatChecker checker;
				if (checker.isOOXFormatFile(pOle->filename().GetPath()))
				{
					pOle->set_MsPackage(true);
				}
				
				smart_ptr<OOX::File> oFile(pOle);

				Rel.Init(); Rel->Rid.Init();
				Rel->Rid->SetValue(pReader->GetRels()->Add(oFile).get());

				if (image_filename_dst.empty() == false)
				{
					Image* pImage = new Image(NULL, false);
					pImage->set_filename(pReader->m_pRels->m_pManager->GetDstMedia() + FILE_SEPARATOR_STR + image_filename_dst, false);

					smart_ptr<OOX::File> oFileCache(pImage);
					pOle->Add(oFileCache);
				}
			}
			else
			{
				std::wstring srcMedia = pReader->m_strFolder + FILE_SEPARATOR_STR + L"media" + FILE_SEPARATOR_STR;
				int idImage = pReader->m_nCountImage++;
				std::wstring image_filename_dst = L"image" + std::to_wstring(idImage) + CPath(image_filename).GetExtention();


				NSFile::CFileBinary::Copy(srcMedia + image_filename, pReader->m_pRels->m_pManager->GetDstMedia() + FILE_SEPARATOR_STR + image_filename_dst);

				Image* pImage = new Image(NULL, false);
				pImage->set_filename(pReader->m_pRels->m_pManager->GetDstMedia() + FILE_SEPARATOR_STR + image_filename_dst, false);

				smart_ptr<OOX::File> oFile(pImage);
				Rel.Init(); Rel->Rid.Init();
				Rel->Rid->SetValue(pReader->GetRels()->Add(oFile).get());
			}
		}
		void CForeignData::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"ForeignData");
			pWriter->StartAttributes();
			if (ForeignType.IsInit()) pWriter->WriteAttribute(L"ForeignType", ForeignType->ToString());
			if (CompressionType.IsInit()) pWriter->WriteAttribute(L"CompressionType", CompressionType->ToString());
			pWriter->WriteAttribute(L"CompressionLevel", CompressionLevel);
			pWriter->WriteAttribute2(L"ObjectType", ObjectType);
			pWriter->WriteAttribute(L"ShowAsIcon", ShowAsIcon);
			pWriter->WriteAttribute(L"ObjectWidth", ObjectWidth);
			pWriter->WriteAttribute(L"ObjectHeight", ObjectHeight);
			pWriter->WriteAttribute(L"ExtentX", ExtentX);
			pWriter->WriteAttribute(L"ExtentY", ExtentY);
			pWriter->EndAttributes();

			if (Rel.IsInit())
				Rel->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"ForeignData");
		}
		EElementType CText::getType() const
		{
			return et_dr_Text;
		}
		void CText::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}
		void CText::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			int nDepth = oReader.GetDepth();
			XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;
		
			while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
			{
				if (eNodeType == XmlUtils::XmlNodeType_Text
					|| eNodeType == XmlUtils::XmlNodeType_Whitespace
					|| eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE
					|| eNodeType == XmlUtils::XmlNodeType_CDATA)
				{
					const char* pValue = oReader.GetTextChar();

					if ('\0' != pValue[0])
					{
						CText_text* pText = new CText_text();
						NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen(pValue), pText->content);
						m_arrItems.push_back(pText);
					}
				}
				else if (eNodeType == XmlUtils::XmlNodeType_Element)
				{
					std::wstring sName = oReader.GetName();

					WritingElement* pItem = NULL;
					if (L"cp" == sName)
					{
						pItem = new CText_cp();
					}
					else if (L"pp" == sName)
					{
						pItem = new CText_pp();
					}
					else if (L"tp" == sName)
					{
						pItem = new CText_tp();
					}
					else if (L"fld" == sName)
					{
						pItem = new CText_fld();
					}
					if (pItem)
					{
						pItem->fromXML(oReader);
						m_arrItems.push_back(pItem);
					}
				}
			}
		}
		void CText::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				int type = 0xff;					 
				switch (m_arrItems[i]->getType())
				{
					case et_dr_text_cp: type = 0; break;
					case et_dr_text_pp: type = 1; break;
					case et_dr_text_tp: type = 2; break;
					case et_dr_text_fld: type = 3; break;
					case et_dr_text_text: type = 4; break;
				}
				if (type != 0xff)
					pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
			}
		}
		void CText::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					m_arrItems.push_back(new CText_cp());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_arrItems.push_back(new CText_pp());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_arrItems.push_back(new CText_tp());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_arrItems.push_back(new CText_fld());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 4:
				{
					m_arrItems.push_back(new CText_text());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CText::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Text");
			pWriter->StartAttributes();
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				m_arrItems[i]->toXmlWriter(pWriter);
			}

			pWriter->WriteNodeEnd(L"Text");
		}
		EElementType CText_text::getType() const
		{
			return et_dr_text_text;
		}
		void CText_text::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, content);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CText_text::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					content = pReader->GetString2();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CText_text::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->WriteStringXML(content);
		}
		EElementType CText_fld::getType() const
		{
			return et_dr_text_fld;
		}
		void CText_fld::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CText_fld::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			content = oReader.GetText2();
		}
		void CText_fld::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, IX);
			pWriter->WriteString1(1, content);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CText_fld::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					IX = pReader->GetULong();
				}break;
				case 1:
				{
					content = pReader->GetString2();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CText_fld::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"fld");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"IX", IX);
			pWriter->EndAttributes();
			
			pWriter->WriteStringXML(content);
			
			pWriter->WriteNodeEnd(L"fld");
		}
		EElementType CText_tp::getType() const
		{
			return et_dr_text_tp;
		}
		void CText_tp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CText_tp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;
		}
		void CText_tp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, IX);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CText_tp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					IX = pReader->GetULong();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CText_tp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"tp");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"IX", IX);
			pWriter->EndAttributes();
			pWriter->WriteNodeEnd(L"tp");
		}
		EElementType CText_pp::getType() const
		{
			return et_dr_text_pp;
		}
		void CText_pp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CText_pp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;
		}
		void CText_pp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, IX);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CText_pp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					IX = pReader->GetULong();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CText_pp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"pp");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"IX", IX);
			pWriter->EndAttributes();
			pWriter->WriteNodeEnd(L"pp");
		}
		EElementType CText_cp::getType() const
		{
			return et_dr_text_cp;
		}
		void CText_cp::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CText_cp::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;
		}
		void CText_cp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, IX);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CText_cp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					IX = pReader->GetULong();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CText_cp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"cp");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"IX", IX);
			pWriter->EndAttributes();
			pWriter->WriteNodeEnd(L"cp");
		}
		EElementType CRow::getType() const
		{
			return et_dr_Row;
		}
		void CRow::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Del", Del)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "N", N)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LocalName", LocalName)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "T", T)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CRow::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				WritingElement* pItem = NULL;
				if (L"Cell" == sName)
				{
					pItem = new CCell();
				}
				else if (L"Trigger" == sName)
				{
					pItem = new CTrigger();
				}
				if (pItem)
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back(pItem);
				}
			}
		}
		void CRow::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, IX);
			pWriter->WriteString2(1, N);
			pWriter->WriteString2(2, LocalName);
			pWriter->WriteString2(3, T);
			pWriter->WriteBool2(4, Del);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				int type = 0xff;					 //todooo predefine type for ???
				switch (m_arrItems[i]->getType())
				{
				case et_dr_Cell: type = 0; break;
				case et_dr_Trigger: type = 1; break;
				}
				if (type != 0xff)
					pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
			}
		}
		void CRow::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					IX = pReader->GetULong();
				}break;
				case 1:
				{
					N = pReader->GetString2A();
				}break;
				case 2:
				{
					LocalName = pReader->GetString2();
				}break;
				case 3:
				{
					T = pReader->GetString2A();
				}break;
				case 4:
				{
					Del = pReader->GetBool();
				}break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					m_arrItems.push_back(new CCell());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_arrItems.push_back(new CTrigger());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CRow::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Row");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"N", N);
			pWriter->WriteAttribute2(L"LocalName", LocalName);
			pWriter->WriteAttribute2(L"T", T);
			pWriter->WriteAttribute2(L"IX", IX);
			pWriter->WriteAttribute(L"Del", Del);
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arrItems.size(); ++i)
				m_arrItems[i]->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"Row");
		}
		EElementType CSection::getType() const
		{
			return et_dr_Section;
		}
		void CSection::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "IX", IX)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Del", Del)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "N", N)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CSection::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				WritingElement* pItem = NULL;
				if (L"Cell" == sName)
				{
					pItem = new CCell();
				}
				else if (L"Trigger" == sName)
				{
					pItem = new CTrigger();
				}
				else if (L"Row" == sName)
				{
					pItem = new CRow();
				}
				if (pItem)
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back(pItem);
				}
			}
		}
		void CSection::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, IX);
			pWriter->WriteString2(1, N);
			pWriter->WriteBool2(2, Del);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				int type = 0xff;					 //todooo predefine type for ???
				switch (m_arrItems[i]->getType())
				{
				case et_dr_Cell: type = 0; break;
				case et_dr_Trigger: type = 1; break;
				case et_dr_Row: type = 6; break;
				}
				if (type != 0xff)
					pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
			}
		}
		void CSection::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					IX = pReader->GetULong();
				}break;
				case 1:
				{
					N = pReader->GetString2A();
				}break;
				case 2:
				{
					Del = pReader->GetBool();
				}break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					m_arrItems.push_back(new CCell());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_arrItems.push_back(new CTrigger());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 6:
				{
					m_arrItems.push_back(new CRow());
					m_arrItems.back()->fromPPTY(pReader);
				}break;

				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CSection::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Section");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"N", N);
			pWriter->WriteAttribute2(L"IX", IX);
			pWriter->WriteAttribute(L"Del", Del);
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arrItems.size(); ++i)
				m_arrItems[i]->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"Section");
		}
		EElementType CRefBy::getType() const
		{
			return et_dr_RefBy;
		}
		void CRefBy::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "T", T)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CRefBy::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;
		}
		void CRefBy::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, ID);
			pWriter->WriteString2(1, T);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CRefBy::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					ID = pReader->GetULong();
				}break;
				case 1:
				{
					T = pReader->GetString2();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CRefBy::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"RefBy");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"T", T);
			pWriter->WriteAttribute2(L"ID", ID);
			pWriter->EndAttributes();
			pWriter->WriteNodeEnd(L"RefBy");
		}
		EElementType CCell::getType() const
		{
			return et_dr_Cell;
		}
		void CCell::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributesA_Read_ifChar(oReader, "N", N)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "U", U)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "E", E)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "F", F)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "V", V)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CCell::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			
			if (oReader.IsEmptyNode())
				return;

			int nDepth = oReader.GetDepth();
			XmlUtils::XmlNodeType eNodeType = XmlUtils::XmlNodeType_EndElement;

			while (oReader.Read(eNodeType) && oReader.GetDepth() >= nDepth && XmlUtils::XmlNodeType_EndElement != eNodeType)
			{
				if (eNodeType == XmlUtils::XmlNodeType_Text
					|| eNodeType == XmlUtils::XmlNodeType_Whitespace
					|| eNodeType == XmlUtils::XmlNodeType_SIGNIFICANT_WHITESPACE
					|| eNodeType == XmlUtils::XmlNodeType_CDATA)
				{
					const char* pValue = oReader.GetTextChar();

					if ('\0' != pValue[0])
					{
						std::wstring val;
						NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pValue, (LONG)strlen(pValue), val);
						content += val;
					}
				}
				else if (eNodeType == XmlUtils::XmlNodeType_Element)
				{
					std::wstring sName = oReader.GetName();

					if (L"RefBy" == sName)
					{
						RefBy = oReader;
					}
				}
			}
		}
		void CCell::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, N);
			pWriter->WriteString2(1, U);
			pWriter->WriteString2(2, E);
			pWriter->WriteString2(3, F);
			pWriter->WriteString2(4, V);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, RefBy);
			
			if (false == content.empty())
			{
				pWriter->StartRecord(1);
				pWriter->WriteString(content);
				pWriter->EndRecord();
			}
		}
		void CCell::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					N = pReader->GetString2A();
				}break;
				case 1:
				{
					U = pReader->GetString2A();
				}break;
				case 2:
				{
					E = pReader->GetString2A();
				}break;
				case 3:
				{
					F = pReader->GetString2A();
				}break;
				case 4:
				{
					V = pReader->GetString2A();
				}break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					RefBy.Init();
					RefBy->fromPPTY(pReader);
				}break;
				case 1:
				{
					content = pReader->GetString2();
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CCell::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Cell");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"N", N);
			pWriter->WriteAttributeUtf8(L"V", V);
			pWriter->WriteAttribute2(L"U", U);
			pWriter->WriteAttribute2(L"E", E);
			pWriter->WriteAttributeUtf8(L"F", F);
			pWriter->EndAttributes();

			if (RefBy.IsInit())
				RefBy->toXmlWriter(pWriter);

			if (false == content.empty())
			{
				pWriter->WriteStringXML(content);
			}

			pWriter->WriteNodeEnd(L"Cell");
		}
		EElementType CTrigger::getType() const
		{
			return et_dr_Trigger;
		}
		void CTrigger::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributesA_Read_ifChar(oReader, "N", N)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CTrigger::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"RefBy" == sName)
				{
					RefBy = oReader;
				}
			}
		}
		void CTrigger::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, N);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			
			pWriter->WriteRecord2(0, RefBy);
		}
		void CTrigger::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
					case 0:
					{
						N = pReader->GetString2A();
					}break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
					case 0:
					{
						RefBy.Init();
						RefBy->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CTrigger::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Trigger");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"N", N);

			pWriter->EndAttributes();
			
			if (RefBy.IsInit())
				RefBy->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"Trigger");
		}
		EElementType CShape::getType() const
		{
			return et_dr_Shape;
		}
		void CShape::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "ID", ID)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "OriginalID", OriginalID)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Type", Type)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Del", Del)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "MasterShape", MasterShape)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "NameU", NameU)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "UniqueID", UniqueID)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Name", Name)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomName", IsCustomName)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "IsCustomNameU", IsCustomNameU)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "Master", Master)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "LineStyle", LineStyle)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FillStyle", FillStyle)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "TextStyle", TextStyle)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CShape::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				WritingElement* pItem = NULL;
				if (L"Cell" == sName)
				{
					pItem = new CCell();
				}
				else if (L"Trigger" == sName)
				{
					pItem = new CTrigger();
				}
				else if (L"Section" == sName)
				{
					pItem = new CSection();
				}
				else if (L"Text" == sName)
				{
					pItem = new CText();
				}
				else if (L"ForeignData" == sName)
				{
					pItem = new CForeignData();
				}
				else if (L"Shapes" == sName)
				{
					pItem = new CShapes();
				}
				if (pItem)
				{
					pItem->fromXML(oReader);
					m_arrItems.push_back(pItem);
				}
			}
		}
		void CShape::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, ID);
			if (Type.IsInit()) pWriter->WriteByte1(1, Type->GetValue());
			pWriter->WriteUInt2(2, OriginalID);
			pWriter->WriteBool2(3, Del);
			pWriter->WriteUInt2(4, MasterShape);
			pWriter->WriteString2(5, UniqueID);
			pWriter->WriteString2(6, NameU);
			pWriter->WriteString2(7, Name);
			pWriter->WriteBool2(8, IsCustomName);
			pWriter->WriteBool2(9, IsCustomNameU);
			pWriter->WriteUInt2(10, Master);
			pWriter->WriteUInt2(11, LineStyle);
			pWriter->WriteUInt2(12, FillStyle);
			pWriter->WriteUInt2(13, TextStyle);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				int type = 0xff;					 //todooo predefine type for ???
				switch (m_arrItems[i]->getType())
				{
					case et_dr_Cell: type = 0; break;
					case et_dr_Trigger: type = 1; break;
					case et_dr_Section: type = 2; break;
					case et_dr_Text: type = 3; break;
					case et_dr_ForeignData: type = 4; break;
					case et_dr_Shapes: type = 5; break;
				}
				if (type != 0xff)
					pWriter->WriteRecord2(type, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
			}
		}
		void CShape::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					ID = pReader->GetULong();
				}break;
				case 1:
				{
					Type.Init(); Type->SetValueFromByte(pReader->GetUChar());
				}break;
				case 2:
				{
					OriginalID = pReader->GetULong();
				}break;
				case 3:
				{
					Del = pReader->GetBool();
				}break;
				case 4:
				{
					MasterShape = pReader->GetULong();
				}break;				
				case 5:
				{
					UniqueID = pReader->GetString2();
				}break;
				case 6:
				{
					NameU = pReader->GetString2();
				}break;
				case 7:
				{
					Name = pReader->GetString2();
				}break;
				case 8:
				{
					IsCustomName = pReader->GetBool();
				}break;
				case 9:
				{
					IsCustomNameU = pReader->GetBool();
				}break;
				case 10:
				{
					Master = pReader->GetULong();
				}break;
				case 11:
				{
					LineStyle = pReader->GetULong();
				}break;
				case 12:
				{
					FillStyle = pReader->GetULong();
				}break;
				case 13:
				{
					TextStyle = pReader->GetULong();
				}break;
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					m_arrItems.push_back(new CCell());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 1:
				{
					m_arrItems.push_back(new CTrigger());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 2:
				{
					m_arrItems.push_back(new CSection());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 3:
				{
					m_arrItems.push_back(new CText());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 4:
				{
					m_arrItems.push_back(new CForeignData());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				case 5:
				{
					m_arrItems.push_back(new CShapes());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CShape::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Shape");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"ID", ID);
			pWriter->WriteAttribute2(L"OriginalID", OriginalID);
			if (Type.IsInit()) pWriter->WriteAttribute2(L"Type", Type->ToString());
			pWriter->WriteAttribute(L"Del", Del);
			pWriter->WriteAttribute2(L"MasterShape", MasterShape);
			pWriter->WriteAttribute2(L"UniqueID", UniqueID);
			pWriter->WriteAttribute2(L"NameU", NameU);
			pWriter->WriteAttribute2(L"Name", Name);
			pWriter->WriteAttribute(L"IsCustomName", IsCustomName);
			pWriter->WriteAttribute(L"IsCustomNameU", IsCustomNameU);
			pWriter->WriteAttribute2(L"Master", Master);
			pWriter->WriteAttribute2(L"LineStyle", LineStyle);
			pWriter->WriteAttribute2(L"FillStyle", FillStyle);
			pWriter->WriteAttribute2(L"TextStyle", TextStyle);
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arrItems.size(); ++i)
				m_arrItems[i]->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"Shape");
		}
		EElementType CShapes::getType() const
		{
			return et_dr_Shapes;
		}
		void CShapes::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"Shape" == sName)
				{
					CShape* pItem = new CShape();
					*pItem = oReader;

					if (pItem)
						m_arrItems.push_back(pItem);
				}
			}
		}
		void CShapes::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					m_arrItems.push_back(new CShape());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void CShapes::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (size_t i = 0; i < m_arrItems.size(); ++i)
				pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
		void CShapes::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Shapes");
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arrItems.size(); ++i)
				m_arrItems[i]->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"Shapes");
		}
		EElementType CConnect::getType() const
		{
			return et_dr_Connect;
		}
		void CConnect::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_StartChar_No_NS(oReader)
				WritingElement_ReadAttributes_Read_ifChar(oReader, "FromSheet", FromSheet)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "FromCell", FromCell)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "FromPart", FromPart)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ToSheet", ToSheet)
				WritingElement_ReadAttributesA_Read_else_ifChar(oReader, "ToCell", ToCell)
				WritingElement_ReadAttributes_Read_else_ifChar(oReader, "ToPart", ToPart)
			WritingElement_ReadAttributes_EndChar_No_NS(oReader)
		}
		void CConnect::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);
			if (oReader.IsEmptyNode())
				return;
		}
		void CConnect::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, FromSheet);
			pWriter->WriteString2(1, FromCell);
			pWriter->WriteInt2(2, FromPart);
			pWriter->WriteUInt2(3, ToSheet);
			pWriter->WriteString2(4, ToCell);
			pWriter->WriteInt2(5, ToPart);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CConnect::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Skip(1); // start attributes
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;
				switch (_at)
				{
				case 0:
				{
					FromSheet = pReader->GetULong();
				}break;
				case 1:
				{
					FromCell = pReader->GetString2A();
				}break;
				case 2:
				{
					FromPart = pReader->GetLong();
				}break;
				case 3:
				{
					ToSheet = pReader->GetULong();
				}break;
				case 4:
				{
					ToCell = pReader->GetString2A();
				}break;
				case 5:
				{
					ToPart = pReader->GetLong();
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void CConnect::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Connect");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"FromSheet", FromSheet);
			pWriter->WriteAttribute2(L"FromCell", FromCell);
			pWriter->WriteAttribute(L"FromPart", FromPart);
			pWriter->WriteAttribute2(L"ToSheet", ToSheet);
			pWriter->WriteAttribute2(L"ToCell", ToCell);
			pWriter->WriteAttribute(L"ToPart", ToPart);
			pWriter->EndAttributes();
			pWriter->WriteNodeEnd(L"Connect");
		}
		void CConnects::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;
			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"Connect" == sName)
				{
					CConnect* pItem = new CConnect();
					*pItem = oReader;

					if (pItem)
						m_arrItems.push_back(pItem);
				}
			}
		}
		EElementType CConnects::getType() const
		{
			return et_dr_Connects;
		}
		void CConnects::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();
				switch (_rec)
				{
				case 0:
				{
					m_arrItems.push_back(new CConnect());
					m_arrItems.back()->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
				}break;
				}
			}
			pReader->Seek(end);
		}
		void CConnects::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (size_t i = 0; i < m_arrItems.size(); ++i)
				pWriter->WriteRecord2(0, dynamic_cast<OOX::WritingElement*>(m_arrItems[i]));
		}
		void CConnects::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"Connects");
			pWriter->EndAttributes();

			for (size_t i = 0; i < m_arrItems.size(); ++i)
				m_arrItems[i]->toXmlWriter(pWriter);

			pWriter->WriteNodeEnd(L"Connects");
		}
	}
} // namespace OOX
