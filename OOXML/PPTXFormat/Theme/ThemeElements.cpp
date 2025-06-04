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

#include "ThemeElements.h"

namespace PPTX
{
	namespace nsTheme
	{
		void ThemeElements::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				for (size_t i = 0; i < oNodes.size(); ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (L"clrScheme" == strName)
						clrScheme.fromXML(oNode);
					else if (L"fontScheme" == strName)
						fontScheme.fromXML(oNode);
					else if (L"fmtScheme" == strName)
						fmtScheme.fromXML(oNode);
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

								if (uri == L"{1342405F-259F-4C95-8CDF-A9DCE2D418A2}")
								{
									fmtConnectorScheme = nodeExt.ReadNodeNoNS(L"fmtConnectorScheme");
								}
								else if (uri == L"{D75FF423-9257-4291-A4FE-1B2448832E17}")
								{
									themeScheme = nodeExt.ReadNodeNoNS(L"themeScheme");
								}
								else if (uri == L"{27CD58D4-7086-4B73-B2AB-7AEBE2148A8C}")
								{
									fmtSchemeEx = nodeExt.ReadNodeNoNS(L"fmtSchemeEx");
								}
								else if (uri == L"{C6430689-8E98-42EC-ADBF-087148533A3F}")
								{
									fmtConnectorSchemeEx = nodeExt.ReadNodeNoNS(L"fmtConnectorSchemeEx"); //- schemeID
								}
								else if (uri == L"{56243398-1771-4C39-BF73-A5702A9C147F}")
								{
									fillStyles = nodeExt.ReadNodeNoNS(L"fillStyles");
								}
								else if (uri == L"{6CAB99AB-0A78-4BAB-B597-526D62367CB4}")
								{
									lineStyles = nodeExt.ReadNodeNoNS(L"lineStyles");
								}
								else if (uri == L"{EBE24D50-EC5C-4D6F-A1A3-C5F0A18B936A}")
								{
									fontStylesGroup = nodeExt.ReadNodeNoNS(L"fontStylesGroup");
							}
								else if (uri == L"{494CE47F-D151-47DC-95E8-85652EA8A67E}")
								{
									variationStyleSchemeLst = nodeExt.ReadNodeNoNS(L"variationStyleSchemeLst");
								}
							}
						}
					}
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring ThemeElements::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(clrScheme);
			oValue.Write(fontScheme);
			oValue.Write(fmtScheme);

			if (fmtConnectorScheme.IsInit() || fillStyles.IsInit() || lineStyles.IsInit() || fontStylesGroup.IsInit() || variationStyleSchemeLst.IsInit())
			{
				oValue.m_strValue += L"<a:extLst>";

				if (fmtConnectorScheme.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{1342405F-259F-4C95-8CDF-A9DCE2D418A2}\">";
					oValue.Write(*fmtConnectorScheme);
					oValue.m_strValue += L"</a:ext>";
				}
				if (themeScheme.IsInit())
				{
					themeScheme->node_name = L"vt:themeScheme";
					oValue.m_strValue += L"<a:ext uri=\"{D75FF423-9257-4291-A4FE-1B2448832E17}\">";
					oValue.Write(*themeScheme);
					oValue.m_strValue += L"</a:ext>";
				}
				if (fmtSchemeEx.IsInit())
				{
					fmtSchemeEx->node_name = L"vt:fmtSchemeEx";
					oValue.m_strValue += L"<a:ext uri=\"{27CD58D4-7086-4B73-B2AB-7AEBE2148A8C}\">";
					oValue.Write(*fmtSchemeEx);
					oValue.m_strValue += L"</a:ext>";
				}
				if (fmtConnectorSchemeEx.IsInit())
				{
					fmtConnectorSchemeEx->node_name = L"vt:fmtConnectorSchemeEx";
					oValue.m_strValue += L"<a:ext uri=\"{C6430689-8E98-42EC-ADBF-087148533A3F}\">";
					oValue.Write(*fmtConnectorSchemeEx);
					oValue.m_strValue += L"</a:ext>";
				}
				if (fillStyles.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{56243398-1771-4C39-BF73-A5702A9C147F}\">";
					oValue.Write(*fillStyles);
					oValue.m_strValue += L"</a:ext>";
				}
				if (lineStyles.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{6CAB99AB-0A78-4BAB-B597-526D62367CB4}\">";
					oValue.Write(*lineStyles);
					oValue.m_strValue += L"</a:ext>";
				}
				if (fontStylesGroup.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{EBE24D50-EC5C-4D6F-A1A3-C5F0A18B936A}\">";
					oValue.Write(*fontStylesGroup);
					oValue.m_strValue += L"</a:ext>";
				}
				if (variationStyleSchemeLst.IsInit())
				{
					oValue.m_strValue += L"<a:ext uri=\"{494CE47F-D151-47DC-95E8-85652EA8A67E}\">";
					oValue.Write(*variationStyleSchemeLst);
					oValue.m_strValue += L"</a:ext>";
				}
				oValue.m_strValue += L"</a:extLst>";
			}
			return XmlUtils::CreateNode(L"a:themeElements", oValue);
		}
		void ThemeElements::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, clrScheme);
			pWriter->WriteRecord1(1, fontScheme);
			pWriter->WriteRecord1(2, fmtScheme);
			pWriter->WriteRecord2(3, fmtConnectorScheme);
			pWriter->WriteRecord2(4, fillStyles);
			pWriter->WriteRecord2(5, lineStyles);
			pWriter->WriteRecord2(6, fontStylesGroup);
			pWriter->WriteRecord2(7, variationStyleSchemeLst);
			pWriter->WriteRecord2(8, themeScheme);
			pWriter->WriteRecord2(9, fmtSchemeEx);
			pWriter->WriteRecord2(10, fmtConnectorSchemeEx);
		}
		void ThemeElements::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:themeElements");
			pWriter->EndAttributes();

			clrScheme.toXmlWriter(pWriter);
			fontScheme.toXmlWriter(pWriter);
			fmtScheme.toXmlWriter(pWriter);

			if (fmtConnectorScheme.IsInit() || fillStyles.IsInit() || lineStyles.IsInit() || fontStylesGroup.IsInit() || variationStyleSchemeLst.IsInit() ||
				themeScheme.IsInit() || fmtSchemeEx.IsInit() || fmtConnectorSchemeEx.IsInit())
			{
				pWriter->StartNode(L"a:extLst");
				pWriter->EndAttributes();

				if (fmtConnectorScheme.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{1342405F-259F-4C95-8CDF-A9DCE2D418A2}");
					pWriter->EndAttributes();
					fmtConnectorScheme->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (themeScheme.IsInit())
				{
					themeScheme->node_name = L"vt:themeScheme";
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{D75FF423-9257-4291-A4FE-1B2448832E17}");
					pWriter->EndAttributes();
					themeScheme->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (fmtSchemeEx.IsInit())
				{
					fmtSchemeEx->node_name = L"vt:fmtSchemeEx";
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{27CD58D4-7086-4B73-B2AB-7AEBE2148A8C}");
					pWriter->EndAttributes();
					fmtSchemeEx->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (fmtConnectorSchemeEx.IsInit())
				{
					fmtConnectorSchemeEx->node_name = L"vt:fmtConnectorSchemeEx";
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{C6430689-8E98-42EC-ADBF-087148533A3F}");
					pWriter->EndAttributes();
					fmtConnectorSchemeEx->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (fillStyles.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{56243398-1771-4C39-BF73-A5702A9C147F}");
					pWriter->EndAttributes();
					fillStyles->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (lineStyles.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{6CAB99AB-0A78-4BAB-B597-526D62367CB4}");
					pWriter->EndAttributes();
					lineStyles->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (fontStylesGroup.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{EBE24D50-EC5C-4D6F-A1A3-C5F0A18B936A}");
					pWriter->EndAttributes();
					fontStylesGroup->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				if (variationStyleSchemeLst.IsInit())
				{
					pWriter->StartNode(L"a:ext");
					pWriter->WriteAttribute(L"uri", L"{494CE47F-D151-47DC-95E8-85652EA8A67E}");
					pWriter->EndAttributes();
					variationStyleSchemeLst->toXmlWriter(pWriter);
					pWriter->EndNode(L"a:ext");
				}
				pWriter->EndNode(L"a:extLst");
			}
			pWriter->EndNode(L"a:themeElements");
		}
		void ThemeElements::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						clrScheme.fromPPTY(pReader);						
					}break;
					case 1:
					{
						fontScheme.fromPPTY(pReader);						
					}break;
					case 2:
					{
						fmtScheme.fromPPTY(pReader);						
					}break;
					case 3:
					{
						fmtConnectorScheme.Init(); fmtConnectorScheme->nameNode = L"vt:fmtConnectorScheme"; 
						fmtConnectorScheme->xmlns_attr = L" xmlns:vt=\"http://schemas.microsoft.com/office/visio/2012/theme\"";
						fmtConnectorScheme->fromPPTY(pReader);
  					}break;
					case 4:
					{
						fillStyles.Init();
						fillStyles->fromPPTY(pReader);
					}break;
					case 5:
					{
						lineStyles.Init();
						lineStyles->fromPPTY(pReader);
					}break;
					case 6:
					{
						fontStylesGroup.Init();
						fontStylesGroup->fromPPTY(pReader);
					}break;
					case 7:
					{
						variationStyleSchemeLst.Init();
						variationStyleSchemeLst->fromPPTY(pReader);
					}break;
					case 8:
					{
						themeScheme.Init();
						themeScheme->fromPPTY(pReader);
					}break;
					case 9:
					{
						fmtSchemeEx.Init();
						fmtSchemeEx->fromPPTY(pReader);
					}break;
					case 10:
					{
						fmtConnectorSchemeEx.Init();
						fmtConnectorSchemeEx->fromPPTY(pReader);
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void ThemeElements::FillParentPointersForChilds()
		{
			clrScheme.SetParentPointer(this);
			fontScheme.SetParentPointer(this);
			fmtScheme.SetParentPointer(this);
		
			if (fmtConnectorScheme.IsInit()) fmtConnectorScheme->SetParentPointer(this);
			if (fillStyles.IsInit()) fillStyles->SetParentPointer(this);
			if (lineStyles.IsInit()) lineStyles->SetParentPointer(this);
			if (fontStylesGroup.IsInit()) fontStylesGroup->SetParentPointer(this);
			if (variationStyleSchemeLst.IsInit()) variationStyleSchemeLst->SetParentPointer(this);
			if (themeScheme.IsInit()) themeScheme->SetParentPointer(this);
			if (fmtSchemeEx.IsInit()) fmtSchemeEx->SetParentPointer(this);
			if (fmtConnectorSchemeEx.IsInit()) fmtConnectorSchemeEx->SetParentPointer(this);
		}
	} // namespace nsTheme
} // namespace PPTX
