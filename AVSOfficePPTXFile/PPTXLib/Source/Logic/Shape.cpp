#include "Shape.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"
#include "SpTree.h"

namespace PPTX
{
	namespace Logic
	{
		Shape::Shape()
		{
		}

		Shape::~Shape()
		{
		}

		Shape::Shape(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Shape& Shape::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Shape::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			attrUseBgFill = element.attribute("useBgFill").value();

			nvSpPr = element.element("nvSpPr");
			spPr = element.element("spPr");
			style = element.element("style");
			txBody = element.element("txBody");

			FillParentPointersForChilds();
		}

		const XML::XNode Shape::toXML() const
		{
			return XML::XElement(ns.p + "sp",
					XML::XAttribute("useBgFill", attrUseBgFill) +
					XML::Write(nvSpPr) +
					XML::Write(spPr) +
					XML::Write(style) +
					XML::Write(txBody)
				);
		}

		void Shape::FillParentPointersForChilds()
		{
			nvSpPr->SetParentPointer(*this);
			spPr->SetParentPointer(*this);
			if(style.is_init())
				style->SetParentPointer(*this);
			if(txBody.is_init())
				txBody->SetParentPointer(*this);
			levelUp = NULL;
		}

		void Shape::GetRect(RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr->xfrm.is_init())
			{
				pRect.left = spPr->xfrm->offX.get_value_or(0);
				pRect.top = spPr->xfrm->offY.get_value_or(0);
				pRect.right = pRect.left + spPr->xfrm->extX.get_value_or(0);
				pRect.bottom = pRect.top + spPr->xfrm->extY.get_value_or(0);
			}
			//else
			//{
			//	if(bMergeWithLevelUp)
			//	{
			//		FillLevelUp();
			//		if(levelUp != NULL)
			//			levelUp->GetRect(pRect);
			//	}
			//}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		void Shape::FillLevelUp()const
		{
			if((levelUp == NULL) && (nvSpPr->nvPr->ph.is_init()))
			{
				if((nvSpPr->nvPr->ph->type.is_init()) || (nvSpPr->nvPr->ph->idx.is_init()))
				{
					if(parentFileIs<Slide>())
						parentFileAs<Slide>().Layout->GetLevelUp(*this);
					else if(parentFileIs<SlideLayout>())
						parentFileAs<SlideLayout>().Master->GetLevelUp(*this);
				}
			}
		}

		void Shape::FillShapeProperties(ShapeProperties& props)const
		{
			//props.SetParentFilePointer(*parentFile);
			if(parentFileIs<SlideMaster>())
			{
				std::string type = nvSpPr->nvPr->ph.is_init()?nvSpPr->nvPr->ph->type.get_value_or("body"):"";
				//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
				//	type = "text-box";
				parentFileAs<SlideMaster>().FillShapeProperties(props, type);
				props.FillFromTextBody(txBody);
				if(style.is_init())
					props.FillFontRef(style->fontRef.get());

				//props.SetParentFilePointer(parentFile);
			}
			else if(parentFileIs<SlideLayout>())
			{
				//FillLevelUp();
				//if(levelUp != NULL)
				//	levelUp->GetShapeProperties(props);
				//else
				//{
					std::string type = nvSpPr->nvPr->ph.is_init()?nvSpPr->nvPr->ph->type.get_value_or("body"):"";
					//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
					//	type = "text-box";
					parentFileAs<SlideLayout>().FillShapeProperties(props, type);
				//}

				props.FillFromTextBody(txBody);
				if(style.is_init())
					props.FillFontRef(style->fontRef.get());

				//props.SetParentFilePointer(parentFile);
			}
			else if(parentFileIs<Slide>())
			{
				//FillLevelUp();
				//if(levelUp != NULL)
				//	levelUp->GetShapeProperties(props);
				//else
				//{
					std::string type = nvSpPr->nvPr->ph.is_init()?nvSpPr->nvPr->ph->type.get_value_or("body"):"";
					//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
					//	type = "text-box";
					parentFileAs<Slide>().FillShapeProperties(props, type);
				//}

				props.FillFromTextBody(txBody);
				if(style.is_init())
					props.FillFontRef(style->fontRef.get());

				//props.SetParentFilePointer(parentFile);
			}
			props.SetParentFilePointer(*parentFile);
		}

		DWORD Shape::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			//fill.SetParentFilePointer(*parentFile);

			if(style.is_init())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().Theme->GetFillStyle(style->fillRef->idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().Theme->GetFillStyle(style->fillRef->idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().Theme->GetFillStyle(style->fillRef->idx.get_value_or(0), fill);

				BGRA = style->fillRef->Color.GetBGRA();
			}

			if(spPr->Fill.is_init())
				spPr->Fill.Merge(fill);

			//if((!fill.is_init()) && (bMergeWithLevelUp))
			//{
			//	FillLevelUp();
			//	if(levelUp != NULL)
			//		BGRA = levelUp->GetFill(fill);
			//}

			return BGRA;
		}

		DWORD Shape::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			//line.SetParentFilePointer(*parentFile);

			//if(bMergeWithLevelUp)
			//{
			//	FillLevelUp();
			//	if(levelUp != NULL)
			//		BGRA = levelUp->GetLine(line);
			//}
			if(style.is_init())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().Theme->GetLineStyle(style->lnRef->idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().Theme->GetLineStyle(style->lnRef->idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().Theme->GetLineStyle(style->lnRef->idx.get_value_or(0), line);

				BGRA = style->lnRef->Color.GetBGRA();
			}

			if(spPr->ln.is_init())
				spPr->ln->Merge(line);
			return BGRA;
		}

		void Shape::Merge(Shape& shape)const
		{
			shape.nvSpPr = nvSpPr;
			spPr->Merge(*shape.spPr);
			if(style.is_init())
			{
				shape.style = ShapeStyle();
				shape.style->fromXML(style->toXML());
				shape.style->SetParentFilePointer(*parentFile);
			}
			if(txBody.is_init())
				txBody->Merge(shape.txBody);
		}

		void Shape::GetShapeFullDescription(Shape& shape, int level)const
		{
			if(level == 0)
				//shape.SetParentFilePointer(*parentFile);
				shape.SetParentPointer(*parentElement);

			if(!nvSpPr->nvPr->ph.is_init())
			{
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::SlideMaster>())
			{
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::SlideLayout>())
			{
				FillLevelUp();
				if(levelUp != NULL)
					levelUp->GetShapeFullDescription(shape, level + 1);
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::Slide>())
			{
				FillLevelUp();
				if(levelUp != NULL)
					levelUp->GetShapeFullDescription(shape, level + 1);
				Merge(shape);
				return;
			}
		}
	} // namespace Logic
} // namespace PPTX