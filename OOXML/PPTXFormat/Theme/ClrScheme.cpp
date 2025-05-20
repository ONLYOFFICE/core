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

#include "ClrScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		void ClrScheme::fromXML(XmlUtils::CXmlNode& node)
		{
			name = node.GetAttribute(_T("name"));

			Scheme.clear();

			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					Logic::UniColor lColor;

					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
					if (L"dk1" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"dk1", lColor));
					}
					else if (L"lt1" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"lt1", lColor));
					}
					else if (L"dk2" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"dk2", lColor));
					}
					else if (L"lt2" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"lt2", lColor));
					}
					else if (L"accent1" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"accent1", lColor));
					}
					else if (L"accent2" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"accent2", lColor));
					}
					else if (L"accent3" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"accent3", lColor));
					}
					else if (L"accent4" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"accent4", lColor));
					}
					else if (L"accent5" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"accent5", lColor));
					}
					else if (L"accent6" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"accent6", lColor));
					}
					else if (L"hlink" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"hlink", lColor));
					}
					else if (L"folHlink" == strName)
					{
						lColor.GetColorFrom(oNode); Scheme.insert(std::pair<std::wstring, Logic::UniColor>(L"folHlink", lColor));
					}
					else if (L"extLst" == strName)
					{
						std::vector<XmlUtils::CXmlNode> nodesExtLst;
						if (oNode.GetNodes(L"*", nodesExtLst))
						{
							for (auto nodeExt : nodesExtLst)
							{
								std::wstring uri;
								XmlMacroReadAttributeBase(nodeExt, L"uri", uri);

								uri = XmlUtils::GetUpper(uri);

								if (uri == L"{093E89EA-6996-430E-BFF9-83A9FAAAAB73}")
								{
									bkgnd = nodeExt.ReadNodeNoNS(L"bkgnd");
								}
								else if (uri == L"{DDD2D869-C2EF-471E-B8FA-914AFA308C9F}")
								{
									variationClrSchemeLst = nodeExt.ReadNodeNoNS(L"variationClrSchemeLst");
								}
							}
						}
					}
				}
			}
			FillWithDefaults();
			FillParentPointersForChilds();
		}
		std::wstring ClrScheme::toXML() const
		{
			XmlUtils::CNodeValue oValue;

			for (std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
			{
				oValue.Write2(L"a:" + pPair->first, pPair->second.toXML());
			}
			if (variationClrSchemeLst.IsInit() || bkgnd.IsInit())
			{
				oValue.m_strValue += L"<a:extLst>";

				if (bkgnd.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{093E89EA-6996-430E-BFF9-83A9FAAAAB73}\">";
					oValue.Write(*bkgnd);
					oValue.m_strValue += L"</a:ext>";
				}
				if (variationClrSchemeLst.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{DDD2D869-C2EF-471E-B8FA-914AFA308C9F}\">";
					oValue.Write(*variationClrSchemeLst);
					oValue.m_strValue += L"</a:ext>";
				}
				oValue.m_strValue += L"</a:extLst>";
			}
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);

			return XmlUtils::CreateNode(_T("a:clrScheme"), oAttr, oValue);
		}
		void ClrScheme::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:clrScheme"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(_T("name"), name);
			pWriter->EndAttributes();

			std::wstring arr[12] = {_T("dk1"), _T("lt1"), _T("dk2"), _T("lt2"), _T("accent1"), _T("accent2"), _T("accent3"), _T("accent4"),
				_T("accent5"), _T("accent6"), _T("hlink"), _T("folHlink")};

			for (LONG i = 0; i < 12; ++i)
			{
				std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(arr[i]);

				if (pPair != Scheme.end())
				{
					pWriter->StartNode(_T("a:") + arr[i]);
					pWriter->EndAttributes();
					pPair->second.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:") + arr[i]);
				}
			}
			if (bkgnd.IsInit() || variationClrSchemeLst.IsInit())
			{
				pWriter->StartNode(L"a:extLst");
				pWriter->EndAttributes();

				if (bkgnd.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{2703A3B3-D2E1-43D9-8057-6E9D74E0F44A}");
					pWriter->EndAttributes();
					bkgnd->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (variationClrSchemeLst.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{DDD2D869-C2EF-471E-B8FA-914AFA308C9F}");
					pWriter->EndAttributes();
					variationClrSchemeLst->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				pWriter->EndNode(L"a:extLst");
			}

			pWriter->EndNode(L"a:clrScheme");
		}
		DWORD ClrScheme::GetRGBAFromScheme(const std::wstring& str)const
		{
			std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
			if (Scheme.end() == pPair)
				return 0;
			return pPair->second.GetRGBA();
		}
		DWORD ClrScheme::GetARGBFromScheme(const std::wstring& str)const
		{
			std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
			if (Scheme.end() == pPair)
			{//default color map extension
				if (str == L"tx1")		pPair = Scheme.find(L"dk1");
				else if (str == L"tx2") pPair = Scheme.find(L"dk2");
				else if (str == L"bg1") pPair = Scheme.find(L"lt1");
				else if (str == L"bg2") pPair = Scheme.find(L"lt2");

				if (Scheme.end() == pPair) return 0;
				else return pPair->second.GetARGB();
			}
			else
				return pPair->second.GetARGB();
		}
		DWORD ClrScheme::GetBGRAFromScheme(const std::wstring& str)const
		{
			std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
			if (Scheme.end() == pPair)
				return 0;
			return pPair->second.GetBGRA();
		}
		DWORD ClrScheme::GetABGRFromScheme(const std::wstring& str)const
		{
			std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
			if (Scheme.end() == pPair)
				return 0;
			return pPair->second.GetABGR();
		}
		void ClrScheme::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, name);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			for (std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
			{
				pWriter->WriteRecord1(SchemeClr_GetBYTECode(pPair->first), pPair->second);
			}
			pWriter->WriteRecord2(20, bkgnd);
			pWriter->WriteRecord2(21, variationClrSchemeLst);
		}
		void ClrScheme::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attribute

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
			}

			while (pReader->GetPos() < _e)
			{
				BYTE _rec = pReader->GetUChar();
				if (_rec == 20)
				{
					bkgnd.Init();
					bkgnd->fromPPTY(pReader);
				}
				else if (_rec == 21)
				{
					variationClrSchemeLst.Init();
					variationClrSchemeLst->fromPPTY(pReader);
				}
				else if (pReader->GetPos() + 4 < _e)
				{
					Logic::UniColor color;
					color.fromPPTY(pReader);

					Scheme.insert(std::pair<std::wstring,Logic::UniColor>(SchemeClr_GetStringCode(_rec), color));
				}
				else
					break;
			}

			pReader->Seek(_e);
		}
		ClrScheme& ClrScheme::operator=(const ClrScheme& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			name = oSrc.name;

			Scheme.clear();
			for (std::map<std::wstring, Logic::UniColor>::const_iterator pPair = oSrc.Scheme.begin(); pPair != oSrc.Scheme.end(); ++pPair)
			{
				Scheme.insert(std::pair<std::wstring,Logic::UniColor>(pPair->first, pPair->second));
			}
			bkgnd = oSrc.bkgnd;
			variationClrSchemeLst = oSrc.variationClrSchemeLst;

			return *this;
		}
		void ClrScheme::FillWithDefaults()
		{
			Logic::UniColor lColor;
			std::map<std::wstring, Logic::UniColor>::iterator it;
			it = Scheme.find(L"dk1");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0,0,0);
				Scheme[L"dk1"] = lColor;
			}
			it = Scheme.find(L"lt1");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(255,255,255);
				Scheme[L"lt1"] = lColor;
			}
			it = Scheme.find(L"dk2");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x1F,0x49, 0x7D);
				Scheme[L"dk2"] = lColor;
			}
			it = Scheme.find(L"lt2");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0xEE,0xEC,0xE1);
				Scheme[L"lt2"] = lColor;
			}
			it = Scheme.find(L"accent1");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x4F, 0x81, 0xBD);
				Scheme[L"accent1"] = lColor;
			}
			it = Scheme.find(L"accent2");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0xC0,0x50,0x4D);
				Scheme[L"accent2"] = lColor;
			}
			it = Scheme.find(L"accent3");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x9B,0xBB,0x59);
				Scheme[L"accent3"] = lColor;
			}
			it = Scheme.find(L"accent4");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x80,0x64,0xA2);
				Scheme[L"accent4"] = lColor;
			}
			it = Scheme.find(L"accent5");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x4B,0xAC,0xC6);
				Scheme[L"accent5"] = lColor;
			}
			it = Scheme.find(L"accent6");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0xF7,0x96,0x46);
				Scheme[L"accent6"] = lColor;
			}
			it = Scheme.find(L"hlink");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x00,0x00,0xFF);
				Scheme[L"hlink"] = lColor;
			}
			it = Scheme.find(L"folHlink");
			if (Scheme.end() == it || !it->second.is_init())
			{
				lColor.SetRGBColor(0x80, 0x00, 0x80);
				Scheme[L"folHlink"] = lColor;
			}

			FillParentPointersForChilds();
		}
		void ClrScheme::FillParentPointersForChilds()
		{
			for (std::map<std::wstring, Logic::UniColor>::iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
			{
				pPair->second.SetParentPointer(this);
			}
			if (bkgnd.IsInit()) bkgnd->SetParentPointer(this);
			if (variationClrSchemeLst.IsInit()) variationClrSchemeLst->SetParentPointer(this);
		}
	} // namespace nsTheme
} // namespace PPTX
