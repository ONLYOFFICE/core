#include "StyleRef.h"

namespace PPTX
{
	namespace Logic
	{

		StyleRef::StyleRef()
		{
		}


		StyleRef::~StyleRef()
		{
		}


		StyleRef::StyleRef(const XML::XNode& node)
		{
			fromXML(node);
		}


		const StyleRef& StyleRef::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void StyleRef::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_name = element.XName.get().Name;
			m_namespace = element.XName.get().Ns;

			idx = element.attribute("idx").value();
			Color.GetColorFrom(element);
			FillParentPointersForChilds();
		}


		const XML::XNode StyleRef::toXML() const
		{
			if(m_namespace.is_init()){
				return XML::XElement(m_namespace + m_name.get(),
					XML::XAttribute("idx", idx) +
					XML::Write(Color)
				);
			}
			else{
				return XML::XElement(m_name.get(),
					XML::XAttribute("idx", idx) +
					XML::Write(Color)
				);
			}
		}

		void StyleRef::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX