#pragma once
#ifndef PPTX_LOGIC_GD_INCLUDE_H_
#define PPTX_LOGIC_GD_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		static void ParseString(CString strDelimeters, CString strSource, std::vector<CString>* pArrayResults)
		{
			if (NULL == pArrayResults)
				return;

			CString resToken;
			int curPos= 0;

			resToken = strSource.Tokenize(strDelimeters, curPos);
			while (resToken != _T(""))
			{
				pArrayResults->push_back(resToken);
				resToken = strSource.Tokenize(strDelimeters, curPos);
			};
		}

		class Gd : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Gd)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				node.ReadAttributeBase(L"name", name);
				node.ReadAttributeBase(L"fmla", fmla);
			}

			virtual CString toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);
				oAttr.Write(_T("fmla"), fmla);

				return XmlUtils::CreateNode(_T("a:gd"), oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:gd"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("name"), name);
				pWriter->WriteAttribute(_T("fmla"), fmla);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:gd"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

				if (name.is_init() && fmla.is_init())
				{
					pWriter->WriteString2(0, name);
					
					int nStart = 0;
					int nCurrent = 0;

					WCHAR* pData = fmla->GetBuffer();
					int nLen = fmla->GetLength();

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

					/*
					std::vector<CString> arr;
					ParseString(_T(" "), fmla.get(), &arr);

					size_t nLen = arr.GetCount();
					for (size_t i = 0; i < nLen; ++i)
					{
						if (i == 0)
						{
							int nVal = GetFormulaType(arr[i]);
							pWriter->WriteInt1((int)(i + 1), nVal);
						}
						else
							pWriter->WriteString1((int)i + 1, arr[i]);
					}
					*/
				}
				
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				pReader->Skip(5); // len + start attributes

				CString _fmla = _T("");
				while (true)
				{
					BYTE _at = pReader->GetUChar();
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

			int GetFormulaType(const CString& str) const
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

			AVSINLINE int GetFormulaType2(const WCHAR& c1, const WCHAR& c2) const
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

			static CString GetFormulaName(const int& val)
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

		public:
			nullable_string			name;
			nullable_string			fmla;
		protected:
			virtual void FillParentPointersForChilds(){};
		public:
			virtual CString GetODString()const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);
				oAttr.Write(_T("fmla"), fmla);

				return XmlUtils::CreateNode(_T("gd"), oAttr);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_GD_INCLUDE_H_