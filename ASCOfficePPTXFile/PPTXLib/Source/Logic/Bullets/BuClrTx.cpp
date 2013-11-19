#include "BuClrTx.h"

namespace PPTX
{
	namespace Logic
	{

		BuClrTx::BuClrTx()
		{
		}


		BuClrTx::~BuClrTx()
		{
		}


		BuClrTx::BuClrTx(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuClrTx& BuClrTx::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuClrTx::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode BuClrTx::toXML() const
		{
			return XML::XElement(ns.a + "buClrTx");
		}

	} // namespace Logic
} // namespace PPTX