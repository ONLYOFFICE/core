#pragma once
#ifndef PPTX_THEME_FMTSCHEME_INCLUDE_H_
#define PPTX_THEME_FMTSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Logic/UniFill.h"
#include "./../Logic/Ln.h"
#include "./../Logic/EffectStyle.h"

namespace PPTX
{
	namespace nsTheme
	{
		class FmtScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(FmtScheme)

			FmtScheme& operator=(const FmtScheme& oSrc)
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

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				fillStyleLst.clear();
				lnStyleLst.clear();
				effectStyleLst.clear();
				bgFillStyleLst.clear();

                name = node.GetAttribute(_T("name"));

				XmlUtils::CXmlNode oNode1 = node.ReadNode(_T("a:fillStyleLst"));
				oNode1.LoadArray(_T("*"), fillStyleLst);

				XmlUtils::CXmlNode oNode2 = node.ReadNode(_T("a:lnStyleLst"));
				oNode2.LoadArray(_T("a:ln"), lnStyleLst);

				XmlUtils::CXmlNode oNode3 = node.ReadNode(_T("a:effectStyleLst"));
				oNode3.LoadArray(_T("a:effectStyle"), effectStyleLst);

				XmlUtils::CXmlNode oNode4 = node.ReadNode(_T("a:bgFillStyleLst"));
				oNode4.LoadArray(_T("*"), bgFillStyleLst);

				FillParentPointersForChilds();
			}

			virtual CString toXML() const
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

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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

			void GetLineStyle(int number, Logic::Ln& lnStyle) const
			{
				int index = number - 1;
				if ((index < 0) || (index >= (int)lnStyleLst.size()))
					return;

				lnStyle = lnStyleLst[index];

				lnStyle.SetParentFilePointer(parentFile);
			}

			void GetFillStyle(int number, Logic::UniFill& fillStyle) const
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

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecordArray(0, 0, fillStyleLst);
				pWriter->WriteRecordArray(1, 0, lnStyleLst);
				pWriter->WriteRecordArray(2, 0, effectStyleLst);
				pWriter->WriteRecordArray(3, 0, bgFillStyleLst);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

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
							pReader->SkipRecord();
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

		public:
			CString name;
			std::vector<Logic::UniFill>		fillStyleLst;
			std::vector<Logic::Ln>			lnStyleLst;
			std::vector<Logic::EffectStyle>	effectStyleLst;
			std::vector<Logic::UniFill>		bgFillStyleLst;
		protected:
			virtual void FillParentPointersForChilds()
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
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_FMTSCHEME_INCLUDE_H
