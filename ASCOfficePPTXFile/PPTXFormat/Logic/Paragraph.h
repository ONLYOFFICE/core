#pragma once
#ifndef PPTX_LOGIC_PARAGRAPH_INCLUDE_H_
#define PPTX_LOGIC_PARAGRAPH_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TextParagraphPr.h"
#include "RunProperties.h"
#include "RunElem.h"

namespace PPTX
{
	namespace Logic
	{
		class Paragraph : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Paragraph)

			Paragraph& operator=(const Paragraph& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				pPr			= oSrc.pPr;
				endParaRPr	= oSrc.endParaRPr;

				for (int i=0 ; i < oSrc.RunElems.size(); i++) 
					RunElems.push_back(oSrc.RunElems[i]);

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				RunElems.clear();

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("pPr") == strName)
							pPr = oNode;
						else if (_T("endParaRPr") == strName)
							endParaRPr = oNode;
						else if ((_T("r") == strName) || (_T("fld") == strName) || (_T("br") == strName))
							RunElems.push_back(RunElem(oNode));
					}
				}
				
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteNullable(pPr);
				oValue.WriteArray(RunElems);
				oValue.WriteNullable(endParaRPr);

				return XmlUtils::CreateNode(_T("a:p"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:p"));
				pWriter->EndAttributes();

				if (pPr.is_init())
					pPr->m_name = _T("a:pPr");
				pWriter->Write(pPr);

				size_t nCount = RunElems.size();
				for (size_t i = 0; i < nCount; ++i)
					RunElems[i].toXmlWriter(pWriter);

				pWriter->Write(endParaRPr);						
				
				pWriter->EndNode(_T("a:p"));
			}


			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord2(0, pPr);
				pWriter->WriteRecord2(1, endParaRPr);
				pWriter->WriteRecordArray(2, 0, RunElems);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							pPr = new Logic::TextParagraphPr();
							pPr->fromPPTY(pReader);
							break;
						}
						case 1:
						{
							endParaRPr = new Logic::RunProperties();
							endParaRPr->m_name = _T("a:endParaRPr");
							endParaRPr->fromPPTY(pReader);
							break;
						}
						case 2:
						{
							pReader->Skip(4);

							ULONG _c = pReader->GetULong();
							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(5); // type (0) + len
								BYTE _type = pReader->GetUChar();

								switch (_type)
								{
									case PARRUN_TYPE_RUN:
									{
										LONG _end = pReader->GetPos() + pReader->GetLong() + 4;

										pReader->Skip(1); // start attr

										Logic::Run* pRun = new Logic::Run();
										
										while (true)
										{
											BYTE _at2 = pReader->GetUChar();
											if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
												break;

											if (0 == _at2)
												pRun->SetText(pReader->GetString2());
										}

										while (pReader->GetPos() < _end)
										{
											BYTE _rec = pReader->GetUChar();

											if (0 == _rec)
											{
												pRun->rPr = new Logic::RunProperties();
												pRun->rPr->m_name = _T("a:rPr");
												pRun->rPr->fromPPTY(pReader);
											}
											else
												pReader->SkipRecord();
										}

										RunElem elm;
										RunElems.push_back(elm);
										RunElems.back().InitRun(pRun);

										pReader->Seek(_end);
										break;
									}
									case PARRUN_TYPE_FLD:
									{
										LONG _end = pReader->GetPos() + pReader->GetLong() + 4;

										pReader->Skip(1); // start attr

										Logic::Fld* pRun = new Logic::Fld();
										
										while (true)
										{
											BYTE _at2 = pReader->GetUChar();
											if (_at2 == NSBinPptxRW::g_nodeAttributeEnd)
												break;

											if (0 == _at2)
												pRun->id = pReader->GetString2();
											else if (1 == _at2)
												pRun->type = pReader->GetString2();
											else if (2 == _at2)
												pRun->SetText(pReader->GetString2());
										}

										while (pReader->GetPos() < _end)
										{
											BYTE _rec = pReader->GetUChar();

											if (0 == _rec)
											{
												pRun->rPr = new Logic::RunProperties();
												pRun->rPr->m_name = _T("a:rPr");
												pRun->rPr->fromPPTY(pReader);
											}
											else if (1 == _rec)
											{												
												pRun->pPr = new Logic::TextParagraphPr();
												pRun->pPr->m_name = _T("a:pPr");
												pRun->pPr->fromPPTY(pReader);
											}
											else
												pReader->SkipRecord();
										}

										RunElem elm;
										RunElems.push_back(elm);
										RunElems.back().InitRun(pRun);

										pReader->Seek(_end);
										break;
									}
									case PARRUN_TYPE_BR:
									{
										LONG _end = pReader->GetPos() + pReader->GetLong() + 4;

										Logic::Br* pRun = new Logic::Br();
										while (pReader->GetPos() < _end)
										{
											BYTE _rec = pReader->GetUChar();

											if (0 == _rec)
											{
												pRun->rPr = new Logic::RunProperties();
												pRun->rPr->m_name = _T("a:rPr");
												pRun->rPr->fromPPTY(pReader);
											}
											else
												pReader->SkipRecord();
										}

										RunElem elm;
										RunElems.push_back(elm);
										RunElems.back().InitRun(pRun);

										pReader->Seek(_end);
										break;
									}
									default:
										break;
								}
							}
							break;
						}
						default:
						{
							break;
						}
					}
				}
				pReader->Seek(_end_rec);
			}

			CString GetText()const
			{
				CString result = _T("");
				
				size_t count = RunElems.size();
				for (size_t i = 0; i < count; ++i)
					result += RunElems[i].GetText();
				
				result = result + _T("\n");
				return result;
			}

		public:
			nullable<TextParagraphPr>	pPr;
			std::vector<RunElem>			RunElems;
			nullable<RunProperties>		endParaRPr;
		protected:
			virtual void FillParentPointersForChilds()
			{
				if(pPr.IsInit())
					pPr->SetParentPointer(this);
				if(endParaRPr.IsInit())
					endParaRPr->SetParentPointer(this);

				size_t count = RunElems.size();
				for (size_t i = 0; i < count; ++i)
					RunElems[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PARAGRAPH_INCLUDE_H