#include "BuSzTx.h"

namespace PPTX
{
	namespace Logic
	{

		BuSzTx::BuSzTx()
		{
		}


		BuSzTx::~BuSzTx()
		{
		}


		BuSzTx::BuSzTx(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuSzTx& BuSzTx::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuSzTx::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode BuSzTx::toXML() const
		{
			return XML::XElement(ns.a + "buSzTx");
		}

	} // namespace Logic
} // namespace PPTX