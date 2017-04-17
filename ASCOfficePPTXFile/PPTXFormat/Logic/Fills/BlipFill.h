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
#pragma once
#ifndef PPTX_LOGIC_BLIPFILL_INCLUDE_H_
#define PPTX_LOGIC_BLIPFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../Rect.h"
#include "Blip.h"
#include "Tile.h"
#include "Stretch.h"
#include "../Effects/AlphaModFix.h"
#include "../../../../Common/DocxFormat/Source/SystemUtility/File.h"

namespace PPTX
{
	namespace Logic
	{

		class BlipFill : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(BlipFill)
			
			BlipFill(std::wstring ns = L"p")
			{
				m_namespace = ns;
			}

			BlipFill& operator=(const BlipFill& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;
				
				blip			= oSrc.blip;
				srcRect			= oSrc.srcRect;
				tile			= oSrc.tile;
				stretch			= oSrc.stretch;

				dpi				= oSrc.dpi;
				rotWithShape	= oSrc.rotWithShape;

				m_namespace = oSrc.m_namespace;
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());
					if (_T("blip") == strName)
						blip = oReader;
					else if (_T("srcRect") == strName)
						srcRect = oReader;
					else if (_T("tile") == strName)
						tile = oReader;
					else if (_T("stretch") == strName)
						stretch = oReader;
				}
			}
			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_blipFill;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS	( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dpi"), dpi)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("rotWithShape"), rotWithShape )
				WritingElement_ReadAttributes_End	( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());

				node.ReadAttributeBase(L"dpi", dpi);
				node.ReadAttributeBase(L"rotWithShape", rotWithShape);

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
						if (_T("blip") == strName)
						{
							if (!blip.IsInit())	
								blip = oNode;
						}
						else if (_T("srcRect") == strName)
						{
							if (!srcRect.IsInit())	
								srcRect = oNode;
						}
						else if (_T("tile") == strName)
						{
							if (!tile.IsInit())	
								tile = oNode;
						}
						else if (_T("stretch") == strName)
						{
							if (!stretch.IsInit())	
								stretch = oNode;
						}
					}
				}

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("dpi"), dpi);
				oAttr.Write(_T("rotWithShape"), rotWithShape);

				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(blip);
				oValue.WriteNullable(srcRect);
				oValue.WriteNullable(tile);
				oValue.WriteNullable(stretch);

				std::wstring strName = (_T("") == m_namespace) ? _T("blipFill") : (m_namespace + _T(":blipFill"));
				return XmlUtils::CreateNode(strName, oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring strName = (_T("") == m_namespace) ? _T("blipFill") : (m_namespace + _T(":blipFill"));
				pWriter->StartNode(strName);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("dpi"), dpi);
				pWriter->WriteAttribute(_T("rotWithShape"), rotWithShape);
				pWriter->EndAttributes();

				pWriter->Write(blip);
				
				if (srcRect.is_init())
				{
					pWriter->StartNode(_T("a:srcRect"));

					pWriter->StartAttributes();
					pWriter->WriteAttribute(_T("l"), srcRect->l);
					pWriter->WriteAttribute(_T("t"), srcRect->t);
					pWriter->WriteAttribute(_T("r"), srcRect->r);
					pWriter->WriteAttribute(_T("b"), srcRect->b);
					pWriter->EndAttributes();

					pWriter->EndNode(_T("a:srcRect"));
				}

				pWriter->Write(tile);
				pWriter->Write(stretch);

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(FILL_TYPE_BLIP);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, dpi);
				pWriter->WriteBool2(1, rotWithShape);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord2(0, blip);
				pWriter->WriteRecord2(1, srcRect);
				pWriter->WriteRecord2(2, tile);
				pWriter->WriteRecord2(3, stretch);

				pWriter->EndRecord();
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				pReader->Skip(4); // len
				BYTE _type = pReader->GetUChar(); // FILL_TYPE_BLIP
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1);

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
						case 0:
							dpi = pReader->GetLong();
							break;
						case 1:
							rotWithShape = pReader->GetBool();
							break;
						default:
							break;
					}
				}

				while (pReader->GetPos() < _e)
				{
					BYTE rec = pReader->GetUChar();

					switch (rec)
					{
						case 0:
						{
							LONG _s2 = pReader->GetPos();
							LONG _e2 = _s2 + pReader->GetLong() + 4;

							pReader->Skip(1);

							while (true)
							{
								BYTE _at = pReader->GetUChar_TypeNode();
								if (NSBinPptxRW::g_nodeAttributeEnd == _at)
									break;

								if (_at == 0)
									pReader->Skip(1);
							}

							while (pReader->GetPos() < _e2)
							{
								BYTE _t = pReader->GetUChar();

								switch (_t)
								{
									case 0:
									case 1:
									{
										// id. embed / link
										pReader->Skip(4);
										break;
									}
									case 10:
									case 11:
									{
										// id. embed / link
										pReader->GetString2();
										break;
									}
									case 2:
									{
										pReader->Skip(4);
										ULONG count_effects = pReader->GetULong();
										for (ULONG _eff = 0; _eff < count_effects; ++_eff)
										{
											pReader->Skip(1); // type 
											ULONG rec_len = pReader->GetULong();
											if (0 == rec_len)
												continue;
											
											BYTE rec = pReader->GetUChar();
											
											if (rec == EFFECT_TYPE_ALPHAMODFIX)
											{
												// alpha!!!
												LONG _e22 = pReader->GetPos() + pReader->GetLong() + 4;

												pReader->Skip(1); // startattr

												PPTX::Logic::AlphaModFix* pEffect = new PPTX::Logic::AlphaModFix();
												while (true)
												{
													BYTE _at = pReader->GetUChar_TypeNode();
													if (NSBinPptxRW::g_nodeAttributeEnd == _at)
														break;

													if (_at == 0)
														pEffect->amt = pReader->GetLong();
												}

												if (!blip.is_init())
													blip = new PPTX::Logic::Blip();

												blip->Effects.push_back(UniEffect());
												blip->Effects[0].InitPointer(pEffect);

												pReader->Seek(_e22);
											}
											else
											{
												pReader->SkipRecord();
											}
										}
										break;
									}
									case 3:
									{
                                    //todoo проверить с временным файлом на линухе
										pReader->Skip(6); // len + start attributes + type

										// -------------------
										std::wstring strImagePath = pReader->GetString2();

										std::wstring strOrigBase64;
                                        std::wstring strTempFile ;

										bool bIsUrl = false;
										
										if (0 == strImagePath.find(_T("data:")))
										{
											bool bBase64 = false;
											
											strOrigBase64 = strImagePath;
											int nFind = (int)strImagePath.find(_T(","));

											std::wstring sImageExtension;

											std::wstring sFormatDataString = XmlUtils::GetLower(strImagePath.substr(5,nFind-5));
											{
												int nFind1 = (int)sFormatDataString.find(_T("base64"));
												if (nFind1 >=0 ) bBase64 = true;
												
												nFind1 = (int)sFormatDataString.find(_T("image/"));
												if (nFind1 >= 0)
												{
													int nFind2 = (int)sFormatDataString.find(_T(";"));
													if (nFind2 < 0) nFind2  = (int)sFormatDataString.length();
											
													sImageExtension = sFormatDataString.substr(nFind1 + 6, nFind2 - 6 - nFind1);
												}
											}
											strImagePath.erase(0, nFind + 1);

											std::string __s = std::string(strImagePath.begin(), strImagePath.end());
											int len = (int)__s.length();
											BYTE* pDstBuffer = NULL;
											int dstLen = 0;

											if (true)//(bBase64)
											{
												bBase64 = true;
												int dstLenTemp = Base64::Base64DecodeGetRequiredLength(len);

												pDstBuffer = new BYTE[dstLenTemp];
												dstLen = dstLenTemp;
												Base64::Base64Decode(__s.c_str(), len, pDstBuffer, &dstLen);
											}
											else
											{
												pDstBuffer = (BYTE*) __s.c_str();
												dstLen = len;
											}
											if (sImageExtension.length() < 1) 
											{
												CImageFileFormatChecker checker;
												sImageExtension = checker.DetectFormatByData(pDstBuffer, dstLen);								
											}
                                            //папки media может не быть в случае, когда все картинки base64(поскольку файл временный, папку media не создаем)
                                            std::wstring tempFilePath = pReader->m_strFolder + FILE_SEPARATOR_STR;
											
											OOX::CPath pathTemp = NSFile::CFileBinary::CreateTempFileWithUniqueName(tempFilePath, _T("img")) + _T(".") + sImageExtension;

                                            CFile oTempFile;
                                            oTempFile.CreateFile(pathTemp.GetPath());
											oTempFile.WriteFile((void*)pDstBuffer, (DWORD)dstLen);
											oTempFile.CloseFile();
											
											strImagePath = strTempFile =pathTemp.GetPath(); // strTempFile для удаления
											if (bBase64)
											{
												RELEASEARRAYOBJECTS(pDstBuffer);
											}
										}
										else
										{
											if (0 != strImagePath.find(_T("http:")) &&
												0 != strImagePath.find(_T("https:")) &&
												0 != strImagePath.find(_T("ftp:")) &&
												0 != strImagePath.find(_T("file:")))
											{
												if (0 == strImagePath.find(_T("theme")))
												{
                                                    strImagePath = pReader->m_strFolderExternalThemes + FILE_SEPARATOR_STR  + strImagePath;
												}
												else
												{
                                                    strImagePath = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media")  + FILE_SEPARATOR_STR + strImagePath;
												}
											}
											else
												bIsUrl = true;
										}
										// -------------------													
										//в случае url не надо нормализовать путь
										if(!bIsUrl)
										{
											OOX::CPath pathUrl = strImagePath;
											strImagePath = pathUrl.GetPath();
										}

										NSBinPptxRW::_relsGeneratorInfo oRelsGeneratorInfo = pReader->m_pRels->WriteImage(strImagePath, oleFile, oleData, strOrigBase64);

										// -------------------
										if (strTempFile != _T(""))
										{
                                            CDirectory::DeleteFile(strTempFile);
										}
										// -------------------

										if (!blip.is_init())
											blip = new PPTX::Logic::Blip();

										blip->embed = new OOX::RId((size_t)oRelsGeneratorInfo.nImageRId);
										blip->oleFilepathImage	= oRelsGeneratorInfo.sFilepathImage;
										
										if(oRelsGeneratorInfo.nOleRId > 0)
										{
											blip->oleRid			= OOX::RId((size_t)oRelsGeneratorInfo.nOleRId).get();
											blip->oleFilepathBin	= oRelsGeneratorInfo.sFilepathOle;
										}

										pReader->Skip(1); // end attribute
										break;
									}
									default:
									{
										pReader->SkipRecord();
										break;
									}
								}
							}

							pReader->Seek(_e2);
							break;
						}
						case 1:
						{
							srcRect = new PPTX::Logic::Rect();
							srcRect->fromPPTY(pReader);							
							break;
						}
						case 2:
						{
							tile = new PPTX::Logic::Tile();
							tile->fromPPTY(pReader);
							break;
						}
						case 3:
						{
							stretch = new PPTX::Logic::Stretch();
							pReader->SkipRecord();
							break;
						}
						default:
						{
							// пока никаких настроек градиента нет
							pReader->SkipRecord();
						}
					}
				}

				pReader->Seek(_e);
			}
			
			mutable std::wstring m_namespace;

			nullable<Blip>		blip;
			nullable<Rect>		srcRect;
			nullable<Tile>		tile;
			nullable<Stretch>	stretch;

			nullable_int		dpi;
			nullable_bool		rotWithShape;

	//internal
			smart_ptr<OOX::OleObject>	oleFile;
			std::wstring				oleData;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(blip.IsInit())
					blip->SetParentPointer(this);
				if(srcRect.IsInit())
					srcRect->SetParentPointer(this);
				if(tile.IsInit())
					tile->SetParentPointer(this);
				if(stretch.IsInit())
					stretch->SetParentPointer(this);
			}

			AVSINLINE void Normalize()
			{
				dpi.normalize_positive();
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BLIPFILL_INCLUDE_H_
