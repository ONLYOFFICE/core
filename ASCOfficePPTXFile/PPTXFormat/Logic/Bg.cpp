//#include "./stdafx.h"

#include "Bg.h"
#include "../Slide.h"
#include "../SlideMaster.h"
#include "../SlideLayout.h"

namespace PPTX
{
	namespace Logic
	{

		Bg::Bg()
		{
		}


		Bg::~Bg()
		{
		}


		Bg::Bg(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}


		const Bg& Bg::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Bg::fromXML(XmlUtils::CXmlNode& node)
		{
			node.ReadAttributeBase(L"bwMode", bwMode);
			bgPr	= node.ReadNodeNoNS(_T("bgPr"));
			bgRef	= node.ReadNodeNoNS(_T("bgRef"));

			FillParentPointersForChilds();
		}


		CString Bg::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.WriteLimitNullable(_T("bwMode"), bwMode);

			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(bgPr);
			oValue.WriteNullable(bgRef);

			return XmlUtils::CreateNode(_T("p:bg"), oAttr, oValue);
		}

		void Bg::FillParentPointersForChilds()
		{
			//if(background != 0)
			//	background->SetParentPointer(this);
			if(bgPr.IsInit())
				bgPr->SetParentPointer(this);
			if(bgRef.IsInit())
				bgRef->SetParentPointer(this);
		}

		void Bg::GetBackground(Logic::BgPr& bg, DWORD& ARGB)const
		{
			if(bgPr.IsInit())
				bg = bgPr.get();
			else// if(cSld->bg->bgRef.is_init())
			{
				ARGB = bgRef->Color.GetARGB();
				if(parentFileIs<Slide>())
					parentFileAs<Slide>().Theme->themeElements.fmtScheme.GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
				else if(parentFileIs<SlideLayout>())
					parentFileAs<SlideLayout>().Theme->themeElements.fmtScheme.GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
				else if(parentFileIs<SlideMaster>())
					parentFileAs<SlideMaster>().Theme->themeElements.fmtScheme.GetFillStyle(bgRef->idx.get_value_or(0), bg.Fill);
			}
		}

	} // namespace Logic
} // namespace PPTX