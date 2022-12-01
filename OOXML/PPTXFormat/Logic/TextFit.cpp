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

#include "TextFit.h"

namespace PPTX
{
	namespace Logic
	{		
		TextFit::TextFit()
		{
			type = FitEmpty;
		}
		bool TextFit::is_init()
		{
			return (type != FitEmpty);
		}
		void TextFit::GetTextFitFrom(XmlUtils::CXmlNode& element)
		{
			type = FitEmpty;

			XmlUtils::CXmlNode oNode;
			if (element.GetNode(_T("a:noAutofit"), oNode))
				type = FitNo;
			else if (element.GetNode(_T("a:spAutoFit"), oNode))
				type = FitSpAuto;
			else if (element.GetNode(_T("a:normAutofit"), oNode))
			{
				type = FitNormAuto;

				nullable_string sFontScale;
				nullable_string sLnSpcRed;

				XmlMacroReadAttributeBase(oNode, L"fontScale", sFontScale);
				XmlMacroReadAttributeBase(oNode, L"lnSpcReduction", sLnSpcRed);

				if (sFontScale.is_init())
				{
					int nFound = (int)sFontScale->rfind(wchar_t('%'));
					if (nFound < 0)
						fontScale = *sFontScale;
					else
					{
						std::wstring sRet = sFontScale->substr(0, nFound);
						double dRet = XmlUtils::GetDouble(sRet);
						int val = (int)(dRet * 1000);
						fontScale = val;
					}
				}

				if (sLnSpcRed.is_init())
				{
					int nFound = (int)sLnSpcRed->rfind(wchar_t('%'));
					if (nFound < 0)
						lnSpcReduction = *sLnSpcRed;
					else
					{
						std::wstring sRet = sLnSpcRed->substr(0, nFound);
						double dRet = XmlUtils::GetDouble(sRet);
						int val = (int)(dRet * 1000);
						lnSpcReduction = val;
					}
				}
			}
		}
		OOX::EElementType TextFit::getType() const
		{
			return OOX::et_a_textFit;
		}
		void TextFit::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring strName = oReader.GetName();

			if (_T("a:noAutofit") == strName)
				type = FitNo;
			else if (_T("a:spAutoFit") == strName)
				type = FitSpAuto;
			else if (_T("a:normAutofit") == strName)
			{
				type = FitNormAuto;

				ReadAttributes(oReader);
			}
		}
		void TextFit::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			nullable_string sFontScale;
			nullable_string sLnSpcRed;

			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("fontScale"),			sFontScale)
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("lnSpcReduction"),	sLnSpcRed)
			WritingElement_ReadAttributes_End( oReader )

			Normalize(sFontScale, sLnSpcRed);
		}
		void TextFit::fromXML(XmlUtils::CXmlNode& node)
		{
			type = FitEmpty;

			std::wstring strName = node.GetName();

			if (_T("a:noAutofit") == strName)
				type = FitNo;
			else if (_T("a:spAutoFit") == strName)
				type = FitSpAuto;
			else if (_T("a:normAutofit") == strName)
			{
				type = FitNormAuto;

				nullable_string sFontScale;
				nullable_string sLnSpcRed;

				XmlMacroReadAttributeBase(node, L"fontScale", sFontScale);
				XmlMacroReadAttributeBase(node, L"lnSpcReduction", sLnSpcRed);

				Normalize(sFontScale, sLnSpcRed);
			}
		}
		void TextFit::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (type == FitNo)
			{
				pWriter->WriteString(_T("<a:noAutofit/>"));
				return;
			}

			if (type == FitSpAuto)
			{
				pWriter->WriteString(_T("<a:spAutoFit/>"));
				return;
			}

			if (type == FitNormAuto)
			{
				pWriter->StartNode(_T("a:normAutofit"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("fontScale"), fontScale);
				pWriter->WriteAttribute(_T("lnSpcReduction"), lnSpcReduction);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:normAutofit"));
			}
		}
		void TextFit::Merge(TextFit& fit)const
		{
			if(type != FitEmpty)
			{
				if((type == FitNo) || (type == FitSpAuto))
				{
					fit.type = type;
					fit.fontScale.reset();
					fit.lnSpcReduction.reset();
				}
				else if(type == FitNormAuto)
				{
					fit.type = type;
					if(fontScale.is_init())
						fit.fontScale = *fontScale;
					if(lnSpcReduction.is_init())
						fit.lnSpcReduction = *lnSpcReduction;
				}
			}
		}
		void TextFit::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt1(0, type);
			pWriter->WriteInt2(1, fontScale);
			pWriter->WriteInt2(2, lnSpcReduction);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TextFit::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					type = (eFit)pReader->GetLong();
					break;
				}
				case 1:
				{
					fontScale = pReader->GetLong();
					break;
				}
				case 2:
				{
					lnSpcReduction = pReader->GetLong();
					break;
				}
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void TextFit::FillParentPointersForChilds(){}
		void TextFit::Normalize(nullable_string & sFontScale, nullable_string & sLnSpcRed)
		{
			if (sFontScale.is_init())
			{
				int nFound = (int)sFontScale->rfind(wchar_t('%'));
				if (nFound < 0)
					fontScale = *sFontScale;
				else
				{
					std::wstring sRet = sFontScale->substr(0, nFound);
					double dRet = XmlUtils::GetDouble(sRet);
					int val = (int)(dRet * 1000);
					fontScale = val;
				}
			}

			if (sLnSpcRed.is_init())
			{
				int nFound = (int)sLnSpcRed->rfind(wchar_t('%'));
				if (nFound < 0)
					lnSpcReduction = *sLnSpcRed;
				else
				{
					std::wstring sRet = sLnSpcRed->substr(0, nFound);
					double dRet = XmlUtils::GetDouble(sRet);
					int val = (int)(dRet * 1000);
					lnSpcReduction = val;
				}
			}
		}
	} // namespace Logic
} // namespace PPTX
