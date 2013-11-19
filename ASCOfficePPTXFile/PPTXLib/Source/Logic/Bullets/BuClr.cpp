#include "BuClr.h"

namespace PPTX
{
	namespace Logic
	{

		BuClr::BuClr()
		{
		}


		BuClr::~BuClr()
		{
		}


		BuClr::BuClr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BuClr& BuClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BuClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
		}


		const XML::XNode BuClr::toXML() const
		{
			return XML::XElement(ns.a + "buClr",
				XML::Write(Color)
				);
		}

		void BuClr::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX