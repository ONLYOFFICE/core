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

#include "Paragraph.h"

namespace PPTX
{
	namespace Logic
	{
		Paragraph& Paragraph::operator=(const Paragraph& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			pPr			= oSrc.pPr;
			endParaRPr	= oSrc.endParaRPr;

			for (size_t i=0 ; i < oSrc.RunElems.size(); i++)
			{
				RunElems.push_back(oSrc.RunElems[i]);
			}

			return *this;
		}
		void Paragraph::fromXML2(XmlUtils::CXmlNode& node, bool bClear)
		{
			if (bClear)
			{
				RunElems.clear();
			}

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("pPr") == strName)
						pPr = oNode;
					else if (_T("endParaRPr") == strName)
						endParaRPr = oNode;
					else if ((_T("r") == strName) || (_T("fld") == strName) || (_T("br") == strName) || (_T("m") == strName))
						RunElems.push_back(RunElem(oNode));
					else if (_T("AlternateContent") == strName)
					{
						//code as in SpTreeElem.cpp
						XmlUtils::CXmlNode oNodeChoice;
						if (oNode.GetNode(_T("mc:Choice"), oNodeChoice))
						{
							XmlUtils::CXmlNode oNodeFall;
							std::wstring sRequires;
							//todo better check (a14 can be math, slicer)
							if(oNodeChoice.GetAttributeIfExist(L"Requires", sRequires) && L"a14" == sRequires)
							{
								fromXML2(oNodeChoice, false);
							}
							else if (oNode.GetNode(_T("mc:Fallback"), oNodeFall))
							{
								fromXML2(oNodeFall, false);
							}
						}
					}
				}
			}

			FillParentPointersForChilds();
		}
		void Paragraph::fromXML2(XmlUtils::CXmlLiteReader& oReader, bool bClear)
		{
			if (bClear)
			{
				RunElems.clear();
			}

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("pPr") == strName)
					pPr = oReader;
				else if (_T("endParaRPr") == strName)
					endParaRPr = oReader;
				else if ((_T("r") == strName) || (_T("fld") == strName) || (_T("br") == strName) || (_T("m") == strName))
					RunElems.push_back(RunElem(oReader));
				else if (_T("AlternateContent") == strName)
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 ) )
					{
						std::wstring strName1 = oReader.GetName();
						if (_T("mc:Choice") == strName1)
						{//GetAttributeIfExist(L"Requires", sRequires) && L"a14" == sRequires)
							fromXML2(oReader, false);
							break;
						}
						else if (_T("mc:Fallback") == strName1)
						{
							fromXML2(oReader, false);
						}

					}
				}
			}
		}
		void Paragraph::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Paragraph::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML2(oReader, true);
		}
		OOX::EElementType Paragraph::getType() const
		{
			return OOX::et_a_p;
		}
		void Paragraph::fromXML(XmlUtils::CXmlNode& node)
		{
			fromXML2(node, true);
		}
		void Paragraph::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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
		void Paragraph::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, pPr);
			pWriter->WriteRecord2(1, endParaRPr);
			pWriter->WriteRecordArray(2, 0, RunElems);
		}
		void Paragraph::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

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
									LONG _end = pReader->GetPos() + pReader->GetRecordSize() + 4;

									pReader->Skip(1); // start attr

									Logic::Run* pRun = new Logic::Run();

									while (true)
									{
										BYTE _at2 = pReader->GetUChar_TypeNode();

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
								}break;
								case PARRUN_TYPE_FLD:
								{
									LONG _end = pReader->GetPos() + pReader->GetRecordSize() + 4;

									pReader->Skip(1); // start attr

									Logic::Fld* pRun = new Logic::Fld();

									while (true)
									{
										BYTE _at2 = pReader->GetUChar_TypeNode();
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
								}break;
								case PARRUN_TYPE_BR:
								{
									LONG _end = pReader->GetPos() + pReader->GetRecordSize() + 4;

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
								}break;
								case PARRUN_TYPE_MATH:
								{
									Logic::MathParaWrapper* pRun = new Logic::MathParaWrapper();
									pRun->fromPPTY(_type, pReader);

									RunElem elm;
									RunElems.push_back(elm);
									RunElems.back().InitRun(pRun);
								}break;
								case PARRUN_TYPE_MATHPARA:
								{
									Logic::MathParaWrapper* pRun = new Logic::MathParaWrapper();
									pRun->fromPPTY(_type, pReader);

									RunElem elm;
									RunElems.push_back(elm);
									RunElems.back().InitRun(pRun);
								}break;
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
		std::wstring Paragraph::GetText(bool bParagraphSeparator) const
		{
			std::wstring result = _T("");

			for (size_t i = 0; i < RunElems.size(); ++i)
			{
				result += RunElems[i].GetText();
			}

			if (bParagraphSeparator)
			{
				result = result + _T("\n");
			}
			return result;
		}
		void Paragraph::FillParentPointersForChilds()
		{
			if(pPr.IsInit())
				pPr->SetParentPointer(this);
			if(endParaRPr.IsInit())
				endParaRPr->SetParentPointer(this);

			size_t count = RunElems.size();
			for (size_t i = 0; i < count; ++i)
				RunElems[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
