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

#include "FmtScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
			FmtScheme& FmtScheme::operator=(const FmtScheme& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name	= oSrc.name;

				fillStyleLst = oSrc.fillStyleLst;
				lnStyleLst = oSrc.lnStyleLst;
				effectStyleLst = oSrc.effectStyleLst;
				bgFillStyleLst = oSrc.bgFillStyleLst;

				return *this;
			}
			void FmtScheme::fromXML(XmlUtils::CXmlNode& node)
			{
				fillStyleLst.clear();
				lnStyleLst.clear();
				effectStyleLst.clear();
				bgFillStyleLst.clear();

				name = node.GetAttribute(_T("name"));

				XmlUtils::CXmlNode oNode1 = node.ReadNode(_T("a:fillStyleLst"));
				XmlMacroLoadArray(oNode1, _T("*"), fillStyleLst, Logic::UniFill);

				XmlUtils::CXmlNode oNode2 = node.ReadNode(_T("a:lnStyleLst"));
				XmlMacroLoadArray(oNode2, _T("a:ln"), lnStyleLst, Logic::Ln);

				XmlUtils::CXmlNode oNode3 = node.ReadNode(_T("a:effectStyleLst"));
				XmlMacroLoadArray(oNode3, _T("a:effectStyle"), effectStyleLst, Logic::EffectStyle);

				XmlUtils::CXmlNode oNode4 = node.ReadNode(_T("a:bgFillStyleLst"));
				XmlMacroLoadArray(oNode4, _T("*"), bgFillStyleLst, Logic::UniFill);

				FillWithDefaults();
				FillParentPointersForChilds();
			}
			std::wstring FmtScheme::toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);

				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(_T("a:fillStyleLst"), fillStyleLst);
				oValue.WriteArray(_T("a:lnStyleLst"), lnStyleLst);
				oValue.WriteArray(_T("a:effectStyleLst"), effectStyleLst);
				oValue.WriteArray(_T("a:bgFillStyleLst"), bgFillStyleLst);

				return XmlUtils::CreateNode(_T("a:fmtScheme"), oAttr, oValue);
			}
			void FmtScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:fmtScheme"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute2(_T("name"), name);
				pWriter->EndAttributes();

				pWriter->WriteArray(_T("a:fillStyleLst"), fillStyleLst);
				pWriter->WriteArray(_T("a:lnStyleLst"), lnStyleLst);
				//pWriter->WriteArray(_T("a:effectStyleLst"), effectStyleLst);
				// вот такой поуерпоинт (эффекты пока не читаем - а они нужны. даже если и не используются нигде)
				pWriter->WriteString(_T("<a:effectStyleLst><a:effectStyle><a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"20000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"38000\"/></a:srgbClr></a:outerShdw>\
</a:effectLst></a:effectStyle><a:effectStyle><a:effectLst><a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\">\
<a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr></a:outerShdw></a:effectLst></a:effectStyle><a:effectStyle><a:effectLst>\
<a:outerShdw blurRad=\"40000\" dist=\"23000\" dir=\"5400000\" rotWithShape=\"0\"><a:srgbClr val=\"000000\"><a:alpha val=\"35000\"/></a:srgbClr>\
</a:outerShdw></a:effectLst></a:effectStyle></a:effectStyleLst>"));

				pWriter->WriteArray(_T("a:bgFillStyleLst"), bgFillStyleLst);

				pWriter->EndNode(_T("a:fmtScheme"));
			}
			void FmtScheme::GetLineStyle(int number, Logic::Ln& lnStyle) const
			{
				int index = number - 1;
				if ((index < 0) || (index >= (int)lnStyleLst.size()))
					return;

				lnStyle = lnStyleLst[index];

				lnStyle.SetParentFilePointer(parentFile);
			}
			void FmtScheme::GetFillStyle(int number, Logic::UniFill& fillStyle) const
			{
				if( (number >= 1) && (number <= 999) )
				{
					int index = number - 1;
					if ((index < 0) || (index >= (int)fillStyleLst.size()))
						return;

					fillStyle = fillStyleLst[index];

					fillStyle.SetParentFilePointer(parentFile);
				}
				else if(number >= 1001)
				{
					int index = number - 1001;
					if ((index < 0) || (index >= (int)bgFillStyleLst.size()))
						return;

					fillStyle = bgFillStyleLst[index];

					fillStyle.SetParentFilePointer(parentFile);
				}
			}
			void FmtScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecordArray(0, 0, fillStyleLst);
				pWriter->WriteRecordArray(1, 0, lnStyleLst);
				pWriter->WriteRecordArray(2, 0, effectStyleLst);
				pWriter->WriteRecordArray(3, 0, bgFillStyleLst);
			}
			void FmtScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();

					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						name = pReader->GetString2();
					else
						break;
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							pReader->Skip(4); // len
							ULONG _c = pReader->GetULong();

							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								fillStyleLst.push_back(Logic::UniFill());
								fillStyleLst[i].fromPPTY(pReader);
							}

							break;
						}
						case 1:
						{
							pReader->Skip(4); // len
							ULONG _c = pReader->GetULong();

							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								lnStyleLst.push_back(Logic::Ln());
								lnStyleLst[i].fromPPTY(pReader);
							}

							break;
						}
						case 2:
						{
							pReader->Skip(4); // len
							ULONG _c = pReader->GetULong();

							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								effectStyleLst.push_back(Logic::EffectStyle());
								effectStyleLst[i].fromPPTY(pReader);
							}

							break;
						}
						case 3:
						{
							pReader->Skip(4); // len
							ULONG _c = pReader->GetULong();

							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								bgFillStyleLst.push_back(Logic::UniFill());
								bgFillStyleLst[i].fromPPTY(pReader);
							}

							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}
			void FmtScheme::FillWithDefaults()
			{
				Logic::SolidFill* pSolidFill = NULL;
				Logic::SchemeClr* pSchemeClr = NULL;

				// -------------- fill styles -------------------------
				if (fillStyleLst.size() < 3)
				{
					fillStyleLst.clear();

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSchemeClr = new Logic::SchemeClr();
					pSchemeClr->val.set(L"phClr");
					pSolidFill->Color.Color.reset(pSchemeClr);
					fillStyleLst.push_back(Logic::UniFill());
					fillStyleLst.back().m_type = Logic::UniFill::solidFill;
					fillStyleLst.back().Fill.reset(pSolidFill);

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSolidFill->Color.SetRGBColor(0,0,0);
					fillStyleLst.push_back(Logic::UniFill());
					fillStyleLst.back().m_type = Logic::UniFill::solidFill;
					fillStyleLst.back().Fill.reset(pSolidFill);

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSolidFill->Color.SetRGBColor(0,0,0);
					fillStyleLst.push_back(Logic::UniFill());
					fillStyleLst.back().m_type = Logic::UniFill::solidFill;
					fillStyleLst.back().Fill.reset(pSolidFill);
				}

				// -------------- back styles -------------------------
				if (bgFillStyleLst.size() < 3)
				{
					bgFillStyleLst.clear();

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSchemeClr = new Logic::SchemeClr();
					pSchemeClr->val.set(L"phClr");
					pSolidFill->Color.Color.reset(pSchemeClr);
					bgFillStyleLst.push_back(Logic::UniFill());
					bgFillStyleLst.back().m_type = Logic::UniFill::solidFill;
					bgFillStyleLst.back().Fill.reset(pSolidFill);

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSolidFill->Color.SetRGBColor(0,0,0);
					bgFillStyleLst.push_back(Logic::UniFill());
					bgFillStyleLst.back().m_type = Logic::UniFill::solidFill;
					bgFillStyleLst.back().Fill.reset(pSolidFill);

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSolidFill->Color.SetRGBColor(0,0,0);
					bgFillStyleLst.push_back(Logic::UniFill());
					bgFillStyleLst.back().m_type = Logic::UniFill::solidFill;
					bgFillStyleLst.back().Fill.reset(pSolidFill);
				}

				// -------------- line styles -------------------------
				if (lnStyleLst.size() < 3)
				{
					lnStyleLst.clear();

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSchemeClr = new Logic::SchemeClr();
					pSchemeClr->val.set(L"phClr");
					pSchemeClr->Modifiers.push_back(Logic::ColorModifier());
					pSchemeClr->Modifiers.back().name = L"shade";
					pSchemeClr->Modifiers.back().val = 95000;
					pSchemeClr->Modifiers.push_back(Logic::ColorModifier());
					pSchemeClr->Modifiers.back().name = L"satMod";
					pSchemeClr->Modifiers.back().val = 105000;
					pSolidFill->Color.Color.reset(pSchemeClr);
					lnStyleLst.push_back(Logic::Ln());
					lnStyleLst.back().w = 9525;
					lnStyleLst.back().Fill.m_type = Logic::UniFill::solidFill;
					lnStyleLst.back().Fill.Fill.reset(pSolidFill);

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSchemeClr = new Logic::SchemeClr();
					pSchemeClr->val.set(L"phClr");
					pSolidFill->Color.Color.reset(pSchemeClr);
					lnStyleLst.push_back(Logic::Ln());
					lnStyleLst.back().w = 25400;
					lnStyleLst.back().Fill.m_type = Logic::UniFill::solidFill;
					lnStyleLst.back().Fill.Fill.reset(pSolidFill);

					pSolidFill = new Logic::SolidFill();
					pSolidFill->m_namespace = L"a";
					pSchemeClr = new Logic::SchemeClr();
					pSchemeClr->val.set(L"phClr");
					pSolidFill->Color.Color.reset(pSchemeClr);
					lnStyleLst.push_back(Logic::Ln());
					lnStyleLst.back().w = 38100;
					lnStyleLst.back().Fill.m_type = Logic::UniFill::solidFill;
					lnStyleLst.back().Fill.Fill.reset(pSolidFill);
				}

				FillParentPointersForChilds();
			}
			void FmtScheme::FillParentPointersForChilds()
			{
				size_t count = 0;

				count = fillStyleLst.size();
				for (size_t i = 0; i < count; ++i)
					fillStyleLst[i].SetParentPointer(this);

				count = lnStyleLst.size();
				for (size_t i = 0; i < count; ++i)
					lnStyleLst[i].SetParentPointer(this);

				count = effectStyleLst.size();
				for (size_t i = 0; i < count; ++i)
					effectStyleLst[i].SetParentPointer(this);

				count = bgFillStyleLst.size();
				for (size_t i = 0; i < count; ++i)
					bgFillStyleLst[i].SetParentPointer(this);
			}
	} // namespace nsTheme
} // namespace PPTX
