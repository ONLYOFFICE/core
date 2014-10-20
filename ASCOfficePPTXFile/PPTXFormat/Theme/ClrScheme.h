#pragma once
#ifndef PPTX_THEME_CLRSCHEME_INCLUDE_H_
#define PPTX_THEME_CLRSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Logic/UniColor.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ClrScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ClrScheme)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = node.GetAttribute(_T("name"));

				Logic::UniColor lColor;
				Scheme.clear();

                XmlUtils::CXmlNode node1;

                node1 = node.ReadNode(_T("a:dk1"));
                lColor.GetColorFrom( node1);        Scheme.insert(std::pair<CString,Logic::UniColor>(_T("dk1"), lColor));
                node1 = node.ReadNode(_T("a:lt1"));
                lColor.GetColorFrom( node1 );       Scheme.insert(std::pair<CString,Logic::UniColor>(_T("lt1"), lColor));
                node1 = node.ReadNode(_T("a:dk2"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("dk2"), lColor));
                node1 = node.ReadNode(_T("a:lt2"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("lt2"), lColor));
                node1 = node.ReadNode(_T("a:accent1"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("accent1"), lColor));
                node1 = node.ReadNode(_T("a:accent2"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("accent2"), lColor));
                node1 = node.ReadNode(_T("a:accent3"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("accent3"), lColor));
                node1 = node.ReadNode(_T("a:accent4"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("accent4"), lColor));
                node1 = node.ReadNode(_T("a:accent5"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("accent5"), lColor));
                node1 = node.ReadNode(_T("a:accent6"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("accent6"), lColor));
                node1 = node.ReadNode(_T("a:hlink"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("hlink"), lColor));
                node1 = node.ReadNode(_T("a:folHlink"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<CString,Logic::UniColor>(_T("folHlink"), lColor));

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;

				for (std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
				{
					oValue.Write2(_T("a:") + pPair->first, pPair->second.toXML());
				}

				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);

				return XmlUtils::CreateNode(_T("a:clrScheme"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:clrScheme"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("name"), name);				
				pWriter->EndAttributes();

				CString arr[12] = {_T("dk1"), _T("lt1"), _T("dk2"), _T("lt2"), _T("accent1"), _T("accent2"), _T("accent3"), _T("accent4"), 
					_T("accent5"), _T("accent6"), _T("hlink"), _T("folHlink")};

				for (LONG i = 0; i < 12; ++i)
				{
					std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.find(arr[i]);
				
					if (pPair != Scheme.end())
					{
						pWriter->StartNode(_T("a:") + arr[i]);
						pWriter->EndAttributes();
						pPair->second.toXmlWriter(pWriter);
						pWriter->EndNode(_T("a:") + arr[i]);
					}
				}

				/*
				POSITION pos = Scheme.GetStartPosition();
				while (NULL != pos)
				{
					const std::map<CString, Logic::UniColor>::iterator pPair = Scheme.GetNext(pos);
					pWriter->StartNode(_T("a:") + pPair->m_key);
					pWriter->EndAttributes();
					pPair->second.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:") + pPair->m_key);					
				}
				*/

				pWriter->EndNode(_T("a:clrScheme"));
			}

			virtual DWORD GetRGBAFromScheme(const CString& str)const
			{
				std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetRGBA();
			}
			virtual DWORD GetARGBFromScheme(const CString& str)const
			{
				std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetARGB();
			}
			virtual DWORD GetBGRAFromScheme(const CString& str)const
			{
				std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetBGRA();
			}
			virtual DWORD GetABGRFromScheme(const CString& str)const
			{
				std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetABGR();
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				for (std::map<CString, Logic::UniColor>::const_iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
				{
					pWriter->WriteRecord1(SchemeClr_GetBYTECode(pPair->first), pPair->second);
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attribute

				while (true)
				{
					BYTE _at = pReader->GetUChar();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						name = pReader->GetString2();
				}

				while (pReader->GetPos() < _e)
				{
					BYTE _rec = pReader->GetUChar();
					
					Logic::UniColor color;
					color.fromPPTY(pReader);

					Scheme.insert(std::pair<CString,Logic::UniColor>(SchemeClr_GetStringCode(_rec), color));
				}

				pReader->Seek(_e);
			}

		public:
			CString name;
			std::map<CString, Logic::UniColor> Scheme;

			ClrScheme& operator=(const ClrScheme& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name = oSrc.name;
				
				Scheme.clear();
				for (std::map<CString, Logic::UniColor>::const_iterator pPair = oSrc.Scheme.begin(); pPair != oSrc.Scheme.end(); ++pPair)
				{
					Scheme.insert(std::pair<CString,Logic::UniColor>(pPair->first, pPair->second));
				}

				return *this;
			}

		protected:
			virtual void FillParentPointersForChilds()
			{
				for (std::map<CString, Logic::UniColor>::iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
				{
					pPair->second.SetParentPointer(this);
				}
			}
			//Logic::UniColor dk1; //Dark 1 
			//Logic::UniColor lt1; //Light 1 
			//Logic::UniColor dk2; //Dark 2 
			//Logic::UniColor lt2; //Light 2 
			//Logic::UniColor accent1; //Accent 1 
			//Logic::UniColor accent2; //Accent 2 
			//Logic::UniColor accent3; //Accent 3 
			//Logic::UniColor accent4; //Accent 4 
			//Logic::UniColor accent5; //Accent 5 
			//Logic::UniColor accent6; //Accent 6 
			//Logic::UniColor hlink; //Hyperlink 
			//Logic::UniColor folHlink; //Followed Hyperlink 
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_CLRSCHEME_INCLUDE_H
