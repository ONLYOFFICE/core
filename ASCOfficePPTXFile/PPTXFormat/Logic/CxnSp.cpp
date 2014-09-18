//#include "./stdafx.h"

#include "CxnSp.h"
#include "SpTree.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"

namespace PPTX
{
	namespace Logic
	{

		CxnSp::CxnSp()
		{
		}

		CxnSp::~CxnSp()
		{
		}

		CxnSp::CxnSp(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const CxnSp& CxnSp::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CxnSp::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					CString strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("nvCxnSpPr") == strName)
						nvCxnSpPr = oNode;
					else if (_T("spPr") == strName)
						spPr = oNode;
					else if (_T("style") == strName)
						style = oNode;
				}
			}

			FillParentPointersForChilds();
		}

		CString CxnSp::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.Write(nvCxnSpPr);
			oValue.Write(spPr);
			oValue.WriteNullable(style);

			return XmlUtils::CreateNode(_T("<p:cxnSp>"), oValue);
		}

		void CxnSp::FillParentPointersForChilds()
		{
			nvCxnSpPr.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if(style.is_init())
				style->SetParentPointer(this);
		}

		void CxnSp::GetRect(RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr.xfrm.IsInit())
			{
				pRect.left		= spPr.xfrm->offX.get();
				pRect.top		= spPr.xfrm->offY.get();
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		DWORD CxnSp::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			line.SetParentFilePointer(parentFile);

			if(style.is_init())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().Theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().Theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().Theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if(spPr.ln.is_init())
				spPr.ln->Merge(line);
			return BGRA;
		}

		DWORD CxnSp::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			fill.SetParentFilePointer(parentFile);

			if (style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().Theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().Theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().Theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				BGRA = style->fillRef.Color.GetBGRA();
			}

			if(spPr.Fill.is_init())
				spPr.Fill.Merge(fill);
			return BGRA;
		}
	} // namespace Logic
} // namespace PPTX