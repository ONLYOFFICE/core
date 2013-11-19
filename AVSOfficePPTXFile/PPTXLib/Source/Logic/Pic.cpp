#include "Pic.h"
#include "SpTree.h"
#include "./../SlideLayout.h"
#include "./../SlideMaster.h"
#include "./../Slide.h"
#include "Media/MediaFile.h"
#include "Media/WavAudioFile.h"

namespace PPTX
{
	namespace Logic
	{

		Pic::Pic()
		{
		}


		Pic::~Pic()
		{
		}
	

		Pic::Pic(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Pic& Pic::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Pic::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			nvPicPr = element.element("nvPicPr");
			blipFill = element.element("blipFill");
			spPr = element.element("spPr");
			style = element.element("style");

			FillParentPointersForChilds();
		}


		const XML::XNode Pic::toXML() const
		{
			return XML::XElement(ns.p + "pic",
					XML::Write(nvPicPr) +
					XML::Write(blipFill) +
					XML::Write(spPr) +
					XML::Write(style)
				);
		}

		void Pic::FillParentPointersForChilds()
		{
			nvPicPr->SetParentPointer(*this);
			blipFill->SetParentPointer(*this);
			spPr->SetParentPointer(*this);
			if(style.is_init())
				style->SetParentPointer(*this);
		}

		void Pic::GetRect(RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr->xfrm.is_init())
			{
				pRect.left = spPr->xfrm->offX.get();
				pRect.top = spPr->xfrm->offY.get();
				pRect.right = pRect.left + spPr->xfrm->extX.get();
				pRect.bottom = pRect.top + spPr->xfrm->extY.get();
			}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		std::wstring Pic::GetFullPicName()const
		{
			if(blipFill->blip.is_init())
				return blipFill->blip->GetFullPicName();
			return L"";
		}

		std::wstring Pic::GetVideoLink()const
		{
			if(parentFileIs<Slide>())
				if(nvPicPr->nvPr->media.is<MediaFile>())
					if((nvPicPr->nvPr->media.as<MediaFile>().name == "videoFile") || (nvPicPr->nvPr->media.as<MediaFile>().name == "quickTimeFile"))
						return parentFileAs<Slide>().GetLinkFromRId(nvPicPr->nvPr->media.as<MediaFile>().link.get());
			return L"";
		}

		std::wstring Pic::GetAudioLink()const
		{
			if(parentFileIs<Slide>())
			{
				if(nvPicPr->nvPr->media.is<WavAudioFile>())
					return parentFileAs<Slide>().GetLinkFromRId(nvPicPr->nvPr->media.as<WavAudioFile>().embed.get());
				if(nvPicPr->nvPr->media.is<MediaFile>())
					if(nvPicPr->nvPr->media.as<MediaFile>().name == "audioFile")
						return parentFileAs<Slide>().GetLinkFromRId(nvPicPr->nvPr->media.as<MediaFile>().link.get());
			}
			return L"";
		}

		DWORD Pic::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			fill.SetParentFilePointer(*parentFile);

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
			return BGRA;
		}

		DWORD Pic::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			line.SetParentFilePointer(*parentFile);

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
	} // namespace Logic
} // namespace PPTX