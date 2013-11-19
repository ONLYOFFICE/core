#include "TextListStyle.h"

namespace PPTX
{
	namespace Logic
	{

		TextListStyle::TextListStyle()
		{
		}


		TextListStyle::~TextListStyle()
		{
		}


		TextListStyle::TextListStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TextListStyle& TextListStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TextListStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			/*defPPr*/ levels[9] = element.element("defPPr");
			/*lvl1pPr*/levels[0] = element.element("lvl1pPr");
			/*lvl2pPr*/levels[1] = element.element("lvl2pPr");
			/*lvl3pPr*/levels[2] = element.element("lvl3pPr");
			/*lvl4pPr*/levels[3] = element.element("lvl4pPr");
			/*lvl5pPr*/levels[4] = element.element("lvl5pPr");
			/*lvl6pPr*/levels[5] = element.element("lvl6pPr");
			/*lvl7pPr*/levels[6] = element.element("lvl7pPr");
			/*lvl8pPr*/levels[7] = element.element("lvl8pPr");
			/*lvl9pPr*/levels[8] = element.element("lvl9pPr");

			FillParentPointersForChilds();
		}


		const XML::XNode TextListStyle::toXML() const
		{
			return	XML::XElement(m_namespace + m_name.get(),
				XML::Write(/*defPPr*/  levels[9]) +
				XML::Write(/*lvl1pPr*/ levels[0]) +
				XML::Write(/*lvl2pPr*/ levels[1]) +
				XML::Write(/*lvl3pPr*/ levels[2]) +
				XML::Write(/*lvl4pPr*/ levels[3]) +
				XML::Write(/*lvl5pPr*/ levels[4]) +
				XML::Write(/*lvl6pPr*/ levels[5]) +
				XML::Write(/*lvl7pPr*/ levels[6]) +
				XML::Write(/*lvl8pPr*/ levels[7]) +
				XML::Write(/*lvl9pPr*/ levels[8])
			);
		}

		void TextListStyle::FillParentPointersForChilds()
		{
			//if(defPPr.is_init())
			//	defPPr->SetParentPointer(*this);
			//if(lvl1pPr.is_init())
			//	lvl1pPr->SetParentPointer(*this);
			//if(lvl2pPr.is_init())
			//	lvl2pPr->SetParentPointer(*this);
			//if(lvl3pPr.is_init())
			//	lvl3pPr->SetParentPointer(*this);
			//if(lvl4pPr.is_init())
			//	lvl4pPr->SetParentPointer(*this);
			//if(lvl5pPr.is_init())
			//	lvl5pPr->SetParentPointer(*this);
			//if(lvl6pPr.is_init())
			//	lvl6pPr->SetParentPointer(*this);
			//if(lvl7pPr.is_init())
			//	lvl7pPr->SetParentPointer(*this);
			//if(lvl8pPr.is_init())
			//	lvl8pPr->SetParentPointer(*this);
			//if(lvl9pPr.is_init())
			//	lvl9pPr->SetParentPointer(*this);
			for(int i = 0; i < 10; i++)
				if(levels[i].is_init())
					levels[i]->SetParentPointer(*this);

		}

		void TextListStyle::Merge(nullable_property<TextListStyle>& lstStyle)const
		{
			if(!lstStyle.is_init())
				lstStyle = TextListStyle();
			for(int i = 0; i < 10; i++)
				if(levels[i].is_init())
					levels[i]->Merge(lstStyle->levels[i]);
		}
	} // namespace Logic
} // namespace PPTX