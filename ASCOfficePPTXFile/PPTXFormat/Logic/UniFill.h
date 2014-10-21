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
								BYTE _at = pReader->GetUChar();
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
											BYTE _at = pReader->GetUChar();
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
																BYTE _at = pReader->GetUChar();
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
														strOrigBase64 = strUrl;

														int nFind = strUrl.Find(_T(","));
														strUrl.Delete(0, nFind + 1);

														CStringA __s = (CStringA)strUrl;
														int len = __s.GetLength();
                                                        int dstLenTemp = Base64::Base64DecodeGetRequiredLength(len);

														BYTE* pDstBuffer = new BYTE[dstLenTemp];
														int dstLen = dstLenTemp;
                                                        Base64::Base64Decode(__s.GetBuffer(), len, pDstBuffer, &dstLen);

														strTempFile = pReader->m_strFolder + _T("\\media\\temp.jpg");
														CFile oTempFile;
														oTempFile.CreateFile(strTempFile);
														oTempFile.WriteFile((void*)pDstBuffer, (DWORD)dstLen);
														oTempFile.CloseFile();
														
														strUrl = strTempFile;
														RELEASEARRAYOBJECTS(pDstBuffer);
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
																strUrl = pReader->m_strFolderThemes + _T("\\") + strUrl;
															}
															else
															{
																strUrl = pReader->m_strFolder + _T("\\media\\") + strUrl;
															}
														}
													}
													// -------------------													
													
													LONG lId = pReader->m_pRels->WriteImage(strUrl, strOrigBase64);

													// -------------------
													if (strTempFile != _T(""))
													{
                                                        //::DeleteFile(strTempFile);
                                                        CDirectory::DeleteFile(strTempFile);
													}
													// -------------------

													if (!pFill->blip.is_init())
														pFill->blip = new PPTX::Logic::Blip();

													pFill->blip->embed = new PPTX::RId((size_t)lId);
													if (pFill->blip.is_init())
														pFill->blip->m_namespace = _T("a");

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
										pReader->SkipRecord();
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
								BYTE _at = pReader->GetUChar();
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
								BYTE _at = pReader->GetUChar();
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
//grpFill (Group Fill)  §20.1.8.35 
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
