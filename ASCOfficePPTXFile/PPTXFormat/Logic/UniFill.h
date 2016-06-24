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
#ifndef PPTX_LOGIC_UNIFILL_INCLUDE_H_
#define PPTX_LOGIC_UNIFILL_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "Fills/BlipFill.h"
#include "Fills/NoFill.h"
#include "Fills/SolidFill.h"
#include "Fills/GradFill.h"
#include "Fills/PattFill.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"

namespace PPTX
{
	namespace Logic
	{
		class UniFill : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(UniFill)
		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CString name = XmlUtils::GetNameNoNS(node.GetName());

				if (name == _T("blipFill"))
				{
					m_type = blipFill;
					Fill.reset(new Logic::BlipFill(node));
				}
				else if(name == _T("noFill"))
				{
					m_type = noFill;
					Fill.reset(new Logic::NoFill(node));
				}
				else if(name == _T("solidFill"))
				{
					m_type = solidFill;
					Fill.reset(new Logic::SolidFill(node));
				}
				else if(name == _T("gradFill"))
				{
					m_type = gradFill;
					Fill.reset(new Logic::GradFill(node));
				}
				else if(name == _T("pattFill"))
				{
					m_type = pattFill;
					Fill.reset(new Logic::PattFill(node));
				}
				else if(name == _T("grpFill"))
				{
					m_type = grpFill;
					Fill.reset(new Logic::GrpFill(node));
				}
				else
				{
					m_type = notInit;
					Fill.reset();
				}
			}

			void GetFillFrom(XmlUtils::CXmlNode& element)
			{
				XmlUtils::CXmlNodes oNodes;
				if (element.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("blipFill") == strName)
						{
							m_type = blipFill;
							Fill.reset(new Logic::BlipFill(oNode));
							return;
						}
						if (_T("noFill") == strName)
						{
							m_type = noFill;
							Fill.reset(new Logic::NoFill(oNode));
							return;
						}
						if (_T("solidFill") == strName)
						{
							m_type = solidFill;
							Fill.reset(new Logic::SolidFill(oNode));
							return;
						}
						if (_T("gradFill") == strName)
						{
							m_type = gradFill;
							Fill.reset(new Logic::GradFill(oNode));
							return;
						}
						if (_T("pattFill") == strName)
						{
							m_type = pattFill;
							Fill.reset(new Logic::PattFill(oNode));
							return;
						}
						if (_T("grpFill") == strName)
						{
							m_type = grpFill;
							Fill.reset(new Logic::GrpFill(oNode));
							return;
						}
					}
				}

				m_type = notInit;
				Fill.reset();
			}
			virtual CString toXML() const
			{
				if (Fill.IsInit())
					Fill->toXML();
				return _T("");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				if (Fill.is_init())
					Fill->toPPTY(pWriter);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG read_start = pReader->GetPos();
				LONG read_end = read_start + pReader->GetLong() + 4;

				m_type = notInit;

				if (pReader->GetPos() < read_end)
				{
					BYTE _type = pReader->GetUChar();
					LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

					switch (_type)
					{
						case FILL_TYPE_BLIP:
						{
							pReader->Skip(1);

							PPTX::Logic::BlipFill* pFill = new PPTX::Logic::BlipFill();
							pFill->m_namespace = _T("a");
							
							while (true)
							{
								BYTE _at = pReader->GetUChar_TypeNode();
								if (_at == NSBinPptxRW::g_nodeAttributeEnd)
									break;

								switch (_at)
								{
									case 0:
										pReader->Skip(4); // dpi
										break;
									case 1:
										pReader->Skip(1); // rotWithShape
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

															if (!pFill->blip.is_init())
																pFill->blip = new PPTX::Logic::Blip();

															pFill->blip->Effects.push_back(UniEffect());
															pFill->blip->Effects[0].InitPointer(pEffect);

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
													pReader->Skip(6); // len + start attributes + type

													// -------------------
													CString strUrl = pReader->GetString2();
													CString strTempFile = _T("");
													CString strOrigBase64 = _T("");


													if (0 == strUrl.Find(_T("data:")))
													{
														bool bBase64 = false;
														
														strOrigBase64 = strUrl;
														int nFind = strUrl.Find(_T(","));

														CString sImageExtension;

														CString sFormatDataString = strUrl.Mid(5,nFind-5);
														sFormatDataString.MakeLower();
														{
															int nFind1 = sFormatDataString.Find(_T("base64"));
															if (nFind1 >=0 ) bBase64 = true;
															
															nFind1 = sFormatDataString.Find(_T("image/"));
															if (nFind1>=0)
															{
																int nFind2 = sFormatDataString.Find(_T(";"));
																if (nFind2 < 0) nFind2  = sFormatDataString.GetLength();
														
																sImageExtension = sFormatDataString.Mid(nFind1 + 6, nFind2 - 6 - nFind1);
															}
														}
														strUrl.Delete(0, nFind + 1);

														CStringA __s = (CStringA)strUrl;
														int len = __s.GetLength();
														BYTE* pDstBuffer = NULL;
														int dstLen = 0;

														if (true)//(bBase64)
														{
															bBase64 = true;
															int dstLenTemp = Base64::Base64DecodeGetRequiredLength(len);

															pDstBuffer = new BYTE[dstLenTemp];
															dstLen = dstLenTemp;
															Base64::Base64Decode(__s.GetBuffer(), len, pDstBuffer, &dstLen);
														}
														else
														{
															pDstBuffer = (BYTE*) __s.GetBuffer();
															dstLen = len;
														}
														if (sImageExtension.GetLength() < 1) 
														{
															CImageFileFormatChecker checker;
															sImageExtension = std_string2string(checker.DetectFormatByData(pDstBuffer, dstLen));								
														}
                                                        //папки media может не быть в случае, когда все картинки base64(поскольку файл временный, папку media не создаем)
                                                        CString tempFilePath = pReader->m_strFolder + FILE_SEPARATOR_STR;
														
														OOX::CPath pathTemp = FileSystem::Directory::CreateTempFileWithUniqueName(tempFilePath, _T("img")) + _T(".") + sImageExtension;

                                                        CFile oTempFile;
                                                        oTempFile.CreateFile(pathTemp.GetPath());
														oTempFile.WriteFile((void*)pDstBuffer, (DWORD)dstLen);
														oTempFile.CloseFile();
														
														strUrl = strTempFile =pathTemp.GetPath(); // strTempFile для удаления
														if (bBase64)
														{
															RELEASEARRAYOBJECTS(pDstBuffer);
														}
													}
													else
													{
														if (0 != strUrl.Find(_T("http:")) &&
															0 != strUrl.Find(_T("https:")) &&
															0 != strUrl.Find(_T("ftp:")) &&
															0 != strUrl.Find(_T("file:")))
														{
															if (0 == strUrl.Find(_T("theme")))
															{
                                                                strUrl = pReader->m_strFolderExternalThemes + FILE_SEPARATOR_STR + strUrl;
															}
															else
															{
                                                                strUrl = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media") + FILE_SEPARATOR_STR + strUrl;
															}
														}
													}
													// -------------------													
													
													NSBinPptxRW::CRelsGeneratorInfo oRelsGeneratorInfo = pReader->m_pRels->WriteImage(strUrl, pFill->oleData, strOrigBase64);

													// -------------------
													if (strTempFile != _T(""))
													{
                                                        CDirectory::DeleteFile(strTempFile);
													}
													// -------------------

													if (!pFill->blip.is_init())
														pFill->blip = new PPTX::Logic::Blip();

													pFill->blip->embed = new PPTX::RId((size_t)oRelsGeneratorInfo.m_nImageRId);
													if (pFill->blip.is_init())
														pFill->blip->m_namespace = _T("a");

													if(oRelsGeneratorInfo.m_nOleRId > 0)
													{
														pFill->blip->oleRid = PPTX::RId((size_t)oRelsGeneratorInfo.m_nOleRId).get();
														pFill->blip->oleFilepathBin = oRelsGeneratorInfo.m_sFilepathBin;
														pFill->blip->oleFilepathImg = oRelsGeneratorInfo.m_sFilepathImg;
														pFill->blip->oleRidImg = pFill->blip->embed->get();
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
										pFill->srcRect = new PPTX::Logic::Rect();
										pFill->srcRect->fromPPTY(pReader);
										break;
									}
									case 2:
									{
										pFill->tile = new PPTX::Logic::Tile();
										pFill->tile->fromPPTY(pReader);
										break;
									}
									case 3:
									{
										pFill->stretch = new PPTX::Logic::Stretch();
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

							m_type = blipFill;
							Fill = pFill;
							break;
						}
						case FILL_TYPE_GRAD:
						{
							pReader->Skip(1);

							PPTX::Logic::GradFill* pFill = new PPTX::Logic::GradFill();
							pFill->m_namespace = _T("a");

							while (true)
							{
								BYTE _at = pReader->GetUChar_TypeNode();
								if (_at == NSBinPptxRW::g_nodeAttributeEnd)
									break;

								switch (_at)
								{
									case 0:
										pFill->flip = new PPTX::Limit::Flip();
										pFill->flip->SetBYTECode(pReader->GetUChar());
										break;
									case 1:
										pFill->rotWithShape = pReader->GetBool();
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
										LONG _s1 = pReader->GetPos();
										LONG _e1 = _s1 + pReader->GetLong() + 4;

										ULONG _count = pReader->GetULong();
										for (ULONG i = 0; i < _count; ++i)
										{
											if (pReader->GetPos() >= _e1)
												break;

											pReader->Skip(1); // type
											pReader->Skip(4); // len

											size_t _countGs = pFill->GsLst.size();
											pFill->GsLst.push_back(Gs());

											pReader->Skip(1); // start attr
											pReader->Skip(1); // pos type
											pFill->GsLst[_countGs].pos = pReader->GetLong();
											pReader->Skip(1); // end attr

											pReader->Skip(1);
											pFill->GsLst[_countGs].color.fromPPTY(pReader);
										}

										pReader->Seek(_e1);
										break;
									}
									case 1:
									{
										pFill->lin = new PPTX::Logic::Lin();
										pFill->lin->fromPPTY(pReader);
										break;
									}
									case 2:
									{
										pFill->path = new PPTX::Logic::Path();
										pFill->path->fromPPTY(pReader);
										break;
									}
									case 3:
									{
										pFill->tileRect = new PPTX::Logic::Rect();
										pFill->tileRect->fromPPTY(pReader);
										pFill->tileRect->m_name = _T("a:tileRect");
									}
									default:
									{
										// пока никаких настроек градиента нет
										pReader->SkipRecord();
										break;
									}
								}
							}

							m_type = gradFill;
							Fill = pFill;
							break;
						}
						case FILL_TYPE_PATT:
						{
							pReader->Skip(1);
							PPTX::Logic::PattFill* pFill = new PPTX::Logic::PattFill();

							while (true)
							{
								BYTE _at = pReader->GetUChar_TypeNode();
								if (_at == NSBinPptxRW::g_nodeAttributeEnd)
									break;

								switch (_at)
								{
									case 0:
										pFill->prst = new PPTX::Limit::PattFillVal();
										pFill->prst->SetBYTECode(pReader->GetUChar());
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
										pFill->fgClr.fromPPTY(pReader);
										break;
									}
									case 1:
									{
										pFill->bgClr.fromPPTY(pReader);
										break;
									}
									default:
									{
										// пока никаких настроек градиента нет
										pReader->SkipRecord();
									}
								}
							}

							pFill->m_namespace = _T("a");

							m_type = pattFill;
							Fill = pFill;
							break;
						}
						case FILL_TYPE_SOLID:
						{
							pReader->Skip(1); // type + len

							PPTX::Logic::SolidFill* pFill = new PPTX::Logic::SolidFill();
							pFill->m_namespace = _T("a");

							pFill->Color.fromPPTY(pReader);

							m_type = solidFill;
							Fill = pFill;
							break;
						}
						case FILL_TYPE_NOFILL:
						{
							m_type = noFill;
							Fill = new PPTX::Logic::NoFill();
							break;
						}
						case FILL_TYPE_GRP:
						{
							m_type = grpFill;
							Fill = new PPTX::Logic::GrpFill();
							break;
						}
					}
				}

				pReader->Seek(read_end);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				if (Fill.is_init())
					Fill->toXmlWriter(pWriter);
			}

			virtual bool is_init()const{return (Fill.IsInit());};
			
			template<class T> AVSINLINE const bool	is() const	{ return Fill.is<T>(); }
			template<class T> AVSINLINE T&			as()		{ return Fill.as<T>(); }
			template<class T> AVSINLINE const T&	as() const 	{ return Fill.as<T>(); }

			const UniFill& operator+=(const UniFill& fill)
			{
				if(!fill.is_init())
					return (*this);
				*this = fill;
				return (*this);
			}
			void Merge(UniFill& fill)const
			{
				if(!fill.is_init())
				{
					fill = *this;
					return;
				}

				if(m_type != fill.m_type)
				{
					fill = *this;
					return;
				}
				if(m_type == solidFill)
				{
					as<SolidFill>().Merge(fill.as<SolidFill>());
					return;
				}
				if(m_type == gradFill)
				{
					as<GradFill>().Merge(fill.as<GradFill>());
					return;
				}
			}


		public:
			smart_ptr<WrapperWritingElement>	Fill;
			enum Type {notInit, noFill, solidFill, gradFill, blipFill, pattFill, grpFill};
			Type								m_type;

		protected:
			virtual void FillParentPointersForChilds()
			{
				if(is_init())
					Fill->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_UNIFILL_INCLUDE_H
