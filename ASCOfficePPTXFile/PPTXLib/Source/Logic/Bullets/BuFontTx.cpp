#include "BuFontTx.h"

namespace PPTX
{
	namespace Logic
	{

		BuFontTx::BuFontTx()
		{
		}


		BuFontTx::~BuFontTx()
		{
		}


		BuFontTx::BuFontTx(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuFontTx& BuFontTx::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuFontTx::fromXML(const XML::XNode& node)
		{
		}


		const XML::XNode BuFontTx::toXML() const
		{
			return XML::XElement(ns.a + "buFontTx");
		}

	} // namespace Logic
} // namespace PPTX