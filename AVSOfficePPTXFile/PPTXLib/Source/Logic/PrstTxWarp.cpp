#include "PrstTxWarp.h"

namespace PPTX
{
	namespace Logic
	{

		PrstTxWarp::PrstTxWarp()
		{
		}


		PrstTxWarp::~PrstTxWarp()
		{
		}
	

		PrstTxWarp::PrstTxWarp(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PrstTxWarp& PrstTxWarp::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PrstTxWarp::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			prst = element.attribute("prst").value();
			XML::Fill(avLst, element.element("avLst"), "gd");

			FillParentPointersForChilds();
		}


		const XML::XNode PrstTxWarp::toXML() const
		{
			return XML::XElement(ns.a + "prstTxWarp",
					XML::XAttribute("prst", prst) +
					XML::XElement(ns.a + "avLst", XML::Write(avLst))
				);
		}

		void PrstTxWarp::FillParentPointersForChilds()
		{
			for(std::list<Gd>::iterator i = avLst->begin(); i != avLst->end(); i++)
				i->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX