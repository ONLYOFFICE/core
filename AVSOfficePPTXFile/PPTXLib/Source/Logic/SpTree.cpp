#include "SpTree.h"

namespace PPTX
{
	namespace Logic
	{
		SpTree::SpTree()
		{
		}

		SpTree::~SpTree()
		{
			if(!SpTreeElems->empty()){
				SpTreeElems->clear();
			}
		}

		SpTree::SpTree(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SpTree& SpTree::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SpTree::fromXML(const XML::XNode& node)
		{
			SpTreeElems->clear();
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;
			
			nvGrpSpPr = element.element("nvGrpSpPr");
			grpSpPr = element.element("grpSpPr");
			XML::Fill(SpTreeElems, element, "sp", "pic", "cxnSp", "grpSp", "graphicFrame");

			FillParentPointersForChilds();
		}

		const XML::XNode SpTree::toXML() const
		{
			return XML::XElement(m_namespace + m_name.get(),
					XML::Write(nvGrpSpPr) +
					XML::Write(grpSpPr) +
					XML::Write(SpTreeElems)
				);
		}

		void SpTree::FillParentPointersForChilds()
		{
			nvGrpSpPr->SetParentPointer(*this);
			grpSpPr->SetParentPointer(*this);
			for(std::list<SpTreeElem>::iterator i = SpTreeElems->begin(); i != SpTreeElems->end(); i++)
				i->SetParentPointer(*this);
		}

		void SpTree::NormalizeRect(RECT& rect)const
		{
			if(grpSpPr->xfrm.is_init())
			{
				if( (grpSpPr->xfrm->chExtX.get_value_or(0) != 0) && (grpSpPr->xfrm->chExtY.get_value_or(0) != 0) )
				{
					double ScaleX = grpSpPr->xfrm->extX.get_value_or(0)/( double(grpSpPr->xfrm->chExtX.get()) );
					double ScaleY = grpSpPr->xfrm->extY.get_value_or(0)/( double(grpSpPr->xfrm->chExtY.get()) );
					double RectWidth = ScaleX * (rect.right - rect.left);
					double RectHeight = ScaleY * (rect.bottom - rect.top);
					rect.left = (rect.left - grpSpPr->xfrm->chOffX.get()) * ScaleX + grpSpPr->xfrm->offX.get();
					rect.top = (rect.top - grpSpPr->xfrm->chOffY.get()) * ScaleY + grpSpPr->xfrm->offY.get();
					rect.right = rect.left + RectWidth;
					rect.bottom = rect.top + RectHeight;
				}
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(rect);
		}
	} // namespace Logic
} // namespace PPTX