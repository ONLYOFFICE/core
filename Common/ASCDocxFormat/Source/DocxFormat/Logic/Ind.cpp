
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Ind.h"


namespace OOX
{
	namespace Logic
	{

		Ind::Ind()
		{
		}


		Ind::~Ind()
		{
		}


		Ind::Ind(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Ind& Ind::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		
		void Ind::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			//Left			= element.attribute("left").value();
			if (element.attribute("left").exist())
			{
				int left = element.attribute("left").value();
				UniversalUnit value(left, UniversalUnit::Dx);
				Left = value;
			}
			//Right			= element.attribute("right").value();
			if (element.attribute("right").exist())
			{
				int right = element.attribute("right").value();
				UniversalUnit value(right, UniversalUnit::Dx);
				Right = value;
			}
			if (element.attribute("hanging").exist())
			{
				int hanging = element.attribute("hanging").value();
				UniversalUnit value(hanging, UniversalUnit::Dx);
				Hanging = value;
			}
			//Hanging		=	element.attribute("hanging").value();
			if (element.attribute("firstLine").exist())
			{
				int first = element.attribute("firstLine").value();
				UniversalUnit value(first, UniversalUnit::Dx);
				FirstLine = value;
			}
		}


		const XML::XNode Ind::toXML() const
		{
		return XML::XElement();
		}


		const bool Ind::isSimple() const
		{
			return (!Left.is_init() && !Right.is_init() && !Hanging.is_init() && !FirstLine.is_init());
		}

	} // namespace Logic
} // namespace OOX