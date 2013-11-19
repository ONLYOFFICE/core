#pragma once
#ifndef PPTX_LOGIC_FONTCOLLECTION_INCLUDE_H_
#define PPTX_LOGIC_FONTCOLLECTION_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "TextFont.h"
#include "SupplementalFont.h"

namespace PPTX
{
	namespace Logic
	{

		class FontCollection : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FontCollection)

			FontCollection& operator=(const FontCollection& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				latin	= oSrc.latin;
				ea		= oSrc.ea;
				cs		= oSrc.cs;
				Fonts.Copy(oSrc.Fonts);

				m_name	= oSrc.m_name;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				XmlUtils::CXmlNodes oNodes;
				if (node.GetNodes(_T("*"), oNodes))
				{
					int nCount = oNodes.GetCount();
					for (int i = 0; i < nCount; ++i)
					{
						XmlUtils::CXmlNode oNode;
						oNodes.GetAt(i, oNode);

						CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

						if (_T("latin") == strName)
							latin = oNode;
						else if (_T("ea") == strName)
							ea = oNode;
						else if (_T("cs") == strName)
							cs = oNode;
						else if (_T("font") == strName)
							Fonts.Add(SupplementalFont(oNode));
					}
				}

				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(latin);
				oValue.Write(ea);
				oValue.Write(cs);
				oValue.WriteArray(Fonts);

				return XmlUtils::CreateNode(m_name, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);
				pWriter->EndAttributes();

				latin.toXmlWriter(pWriter);
				ea.toXmlWriter(pWriter);				
				cs.toXmlWriter(pWriter);
				
				size_t nCount = Fonts.GetCount();
				for (size_t i = 0; i < nCount; ++i)
					Fonts[i].toXmlWriter(pWriter);

				pWriter->EndNode(m_name);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, latin);
				pWriter->WriteRecord1(1, ea);
				pWriter->WriteRecord1(2, cs);
				pWriter->WriteRecordArray(3, 0, Fonts);
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
							latin.fromPPTY(pReader);
							latin.m_name = _T("a:latin");
							break;
						}
						case 1:
						{
							ea.fromPPTY(pReader);
							ea.m_name = _T("a:ea");
							break;
						}
						case 2:
						{
							cs.fromPPTY(pReader);
							cs.m_name = _T("a:cs");
							break;
						}
						case 3:
						{
							pReader->Skip(4);
							ULONG _c = pReader->GetULong();
							for (ULONG i = 0; i < _c; ++i)
							{
								pReader->Skip(1); // type
								Fonts.Add();
								Fonts[i].m_name = _T("a:font");
								Fonts[i].fromPPTY(pReader);
							}
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			TextFont					latin;
			TextFont					ea;
			TextFont					cs;
			CAtlArray<SupplementalFont> Fonts;
		//private:
		public:
			CString m_name;
		protected:
			virtual void FillParentPointersForChilds()
			{
				latin.SetParentPointer(this);
				ea.SetParentPointer(this);
				cs.SetParentPointer(this);

				size_t count = Fonts.GetCount();
				for (size_t i = 0; i < count; ++i)
					Fonts[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FONTCOLLECTION_INCLUDE_H_