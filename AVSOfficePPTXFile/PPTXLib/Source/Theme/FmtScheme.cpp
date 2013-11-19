#include "FmtScheme.h"

namespace PPTX
{
	namespace nsTheme
	{
		FmtScheme::FmtScheme()
		{
		}

		FmtScheme::~FmtScheme()
		{
		}

		FmtScheme::FmtScheme(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FmtScheme& FmtScheme::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FmtScheme::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			fillStyleLst->clear();
			lnStyleLst->clear();
			effectStyleLst->clear();
			bgFillStyleLst->clear();

			name = element.attribute("name").value();
			XML::Fill(fillStyleLst, element.element("fillStyleLst"));
			XML::Fill(lnStyleLst, element.element("lnStyleLst"), "ln");
			XML::Fill(effectStyleLst, element.element("effectStyleLst"), "effectStyle");
			XML::Fill(bgFillStyleLst, element.element("bgFillStyleLst"));

			FillParentPointersForChilds();
		}

		const XML::XNode FmtScheme::toXML() const
		{
			return XML::XElement(ns.a + "fmtScheme",
					XML::XAttribute("name", name) +
					XML::XElement(ns.a + "fillStyleLst", XML::Write(fillStyleLst)) +
					XML::XElement(ns.a + "lnStyleLst", XML::Write(lnStyleLst)) +
					XML::XElement(ns.a + "effectStyleLst", XML::Write(effectStyleLst)) +
					XML::XElement(ns.a + "bgFillStyleLst", XML::Write(bgFillStyleLst))
				);
		}

		void FmtScheme::FillParentPointersForChilds()
		{
			size_t i = 0;
			for(std::list<Logic::UniFill>::iterator i = fillStyleLst->begin(); i != fillStyleLst->end(); i++)
				i->SetParentPointer(*this);
			for(std::list<Logic::Ln>::iterator i = lnStyleLst->begin(); i != lnStyleLst->end(); i++)
				i->SetParentPointer(*this);
			for(std::list<Logic::EffectStyle>::iterator i = effectStyleLst->begin(); i != effectStyleLst->end(); i++)
				i->SetParentPointer(*this);
			for(std::list<Logic::UniFill>::iterator i = bgFillStyleLst->begin(); i != bgFillStyleLst->end(); i++)
				i->SetParentPointer(*this);
		}

		void FmtScheme::GetLineStyle(int number, Logic::Ln& lnStyle)const
		{
			int i = 1;
			for(std::list<Logic::Ln>::const_iterator iter = lnStyleLst->begin(); iter != lnStyleLst->end(); iter++, i++)
			{
				if(number == i)
				{
					lnStyle.fromXML(iter->toXML());
					lnStyle.SetParentFilePointer(*parentFile);
					break;
				}
			}
		}

		void FmtScheme::GetFillStyle(int number, Logic::UniFill& fillStyle)const
		{
			if( (number >= 1) && (number <= 999) )
			{
				int i = 1;
				for(std::list<Logic::UniFill>::const_iterator iter = fillStyleLst->begin(); iter != fillStyleLst->end(); iter++, i++)
				{
					if(number == i)
					{
						fillStyle.fromXML(iter->toXML());
						fillStyle.SetParentFilePointer(*parentFile);
						break;
					}
				}
			}
			else if(number >= 1001)
			{
				int i = 1001;
				for(std::list<Logic::UniFill>::const_iterator iter = bgFillStyleLst->begin(); iter != bgFillStyleLst->end(); iter++, i++)
				{
					if(number == i)
					{
						fillStyle.fromXML(iter->toXML());
						fillStyle.SetParentFilePointer(*parentFile);
						break;
					}
				}
			}
		}
	} // namespace nsTheme
} // namespace PPTX