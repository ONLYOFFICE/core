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

#include "Gd.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType Gd::getType () const
		{
			return OOX::et_a_gd;
		}
		void Gd::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

		}
		void Gd::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if		( oReader, _T("name"), name )
				WritingElement_ReadAttributes_Read_else_if	( oReader, _T("fmla"), fmla )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Gd::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"name", name);
			XmlMacroReadAttributeBase(node, L"fmla", fmla);
		}
		std::wstring Gd::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);
			oAttr.Write(_T("fmla"), fmla);

			return XmlUtils::CreateNode(_T("a:gd"), oAttr);
		}
		void Gd::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:gd"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute2(_T("name"), name);
			pWriter->WriteAttribute(_T("fmla"), fmla);
//			pWriter->EndAttributes();
							pWriter->WriteNodeEnd(_T(""), true, true);

//			pWriter->EndNode(_T("a:gd"));
		}
		void Gd::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			if (name.is_init() && fmla.is_init())
			{
				pWriter->WriteString2(0, name);

				size_t nStart = 0;
				size_t nCurrent = 0;

				const wchar_t* pData = fmla->c_str();
				size_t nLen = fmla->length();

				int nFound = 0;
				while (nCurrent < nLen)
				{
					if (pData[nCurrent] == (WCHAR)' ')
					{
						if (nStart < nCurrent)
						{
							if (0 == nFound)
							{
								if ((nCurrent - nStart) > 1)
								{
									pWriter->WriteInt1(1, GetFormulaType2(pData[nStart], pData[nStart + 1]));
								}
								else
								{
									pWriter->WriteInt1(1, 0);
								}
							}
							else
							{
								pWriter->WriteString1Data(nFound + 1, pData + nStart, (ULONG)(nCurrent - nStart));
							}
							nStart = nCurrent + 1;
							++nFound;
						}
					}
					++nCurrent;
				}
				if (nStart < nCurrent)
				{
					if (0 == nFound)
					{
						if ((nCurrent - nStart) > 1)
						{
							pWriter->WriteInt1(1, GetFormulaType2(pData[nStart], pData[nStart + 1]));
						}
						else
						{
							pWriter->WriteInt1(1, 0);
						}
					}
					else
					{
						pWriter->WriteString1Data(nFound + 1, pData + nStart, (ULONG)(nCurrent - nStart));
					}
				}
			}

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void Gd::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(5); // len + start attributes

			std::wstring _fmla = _T("");
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)
					name = pReader->GetString2();
				else if (1 == _at)
				{
					_fmla = GetFormulaName(pReader->GetLong());
				}
				else
				{
					if (1 < _at)
						_fmla += _T(" ");
					_fmla += pReader->GetString2();
				}
			}
			if (_T("") != _fmla)
				fmla = _fmla;
		}
		int Gd::GetFormulaType(const std::wstring& str) const
		{
			if (_T("*/") == str)
				return 0;
			if (_T("+-") == str)
				return 1;
			if (_T("+/") == str)
				return 2;
			if (_T("?:") == str)
				return 3;
			if (_T("abs") == str)
				return 4;
			if (_T("at2") == str)
				return 5;
			if (_T("cat2") == str)
				return 6;
			if (_T("cos") == str)
				return 7;
			if (_T("max") == str)
				return 8;
			if (_T("min") == str)
				return 16;
			if (_T("mod") == str)
				return 9;
			if (_T("pin") == str)
				return 10;
			if (_T("sat2") == str)
				return 11;
			if (_T("sin") == str)
				return 12;
			if (_T("sqrt") == str)
				return 13;
			if (_T("tan") == str)
				return 14;
			if (_T("val") == str)
				return 15;
			return 0;
		}
		int Gd::GetFormulaType2(const WCHAR& c1, const WCHAR& c2) const
		{
			switch (c1)
			{
			case (WCHAR)'*':
				return 0;
			case (WCHAR)'+':
				return ((WCHAR)'-' == c2) ? 1 : 2;
			case (WCHAR)'?':
				return 3;
			case (WCHAR)'a':
				return ((WCHAR)'b' == c2) ? 4 : 5;
			case (WCHAR)'c':
				return ((WCHAR)'a' == c2) ? 6 : 7;
			case (WCHAR)'m':
				return ((WCHAR)'a' == c2) ? 8 : (((WCHAR)'i' == c2) ? 16 : 9);
			case (WCHAR)'p':
				return 10;
			case (WCHAR)'s':
				return ((WCHAR)'a' == c2) ? 11 : (((WCHAR)'i' == c2) ? 12 : 13);
			case (WCHAR)'t':
				return 14;
			case (WCHAR)'v':
				return 15;
			default:
				break;
			}
			return 0;
		}
		std::wstring Gd::GetFormulaName(const int& val)
		{
			switch (val)
			{
			case 0: return _T("*/");
			case 1: return _T("+-");
			case 2: return _T("+/");
			case 3: return _T("?:");
			case 4: return _T("abs");
			case 5: return _T("at2");
			case 6: return _T("cat2");
			case 7: return _T("cos");
			case 8: return _T("max");
			case 16: return _T("min");
			case 9: return _T("mod");
			case 10: return _T("pin");
			case 11: return _T("sat2");
			case 12: return _T("sin");
			case 13: return _T("sqrt");
			case 14: return _T("tan");
			case 15: return _T("val");
			default:
				return _T("*/");
			}
		}
		void Gd::FillParentPointersForChilds()
		{
		}
		std::wstring Gd::GetODString() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);
			oAttr.Write(_T("fmla"), fmla);

			return XmlUtils::CreateNode(_T("gd"), oAttr);
		}
	} // namespace Logic
} // namespace PPTX
