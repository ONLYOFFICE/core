#include "TxEl.h"

namespace PPTX
{
	namespace Logic
	{

		TxEl::TxEl()
		{
		}

		TxEl::~TxEl()
		{
		}

		TxEl::TxEl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TxEl& TxEl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TxEl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("charRg").exist())
			{
				charRg = true;
				st = element.element("charRg").attribute("st").value();
				end = element.element("charRg").attribute("end").value();
			}
			else if(element.element("pRg").exist())
			{
				charRg = false;
				st = element.element("pRg").attribute("st").value();
				end = element.element("pRg").attribute("end").value();
			}
			else
			{
				charRg.reset();
				st.reset();
				end.reset();
			}
		}

		const XML::XNode TxEl::toXML() const
		{
			if(charRg.is_init())
			{
				if(charRg.get())
					return XML::XElement(ns.p + "txEl", XML::XElement(ns.p + "charRg", XML::XAttribute("st", st) + XML::XAttribute("end", end)));
				return XML::XElement(ns.p + "txEl", XML::XElement(ns.p + "pRg", XML::XAttribute("st", st) + XML::XAttribute("end", end)));
			}
			return XML::XElement(ns.p + "txEl");
		}
	} // namespace Logic
} // namespace PPTX