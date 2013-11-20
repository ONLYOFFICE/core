
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "OleWrap.h"
#include "Wrap.h"


namespace OOX
{
	namespace Logic
	{

		OleWrap::OleWrap()
		{
		}


		OleWrap::OleWrap(const Common::Wrap& wrap)
		{
			fromBase(wrap);
		}


		OleWrap::OleWrap(const std::string& value)
		{
			fromString(value);
		}


		OleWrap::OleWrap(const XML::XElement& element)			
		{
			// TODO зачем использовать OOX::Logic::Wrap, если это попытка от него избавиться
			nullable_property<OOX::Logic::Wrap>	wrap;
			wrap = element.element("wrap");


			nullable_property<std::string> wrapCoords;
			wrapCoords = element.attribute("wrapcoords").value();
						
			if(wrapCoords.is_init())
				WrapCoords = wrapCoords;
			if(wrap.is_init())
				OleWrap(wrap->Type);
			else 
				OleWrap("none");
			
		}

		
		const OleWrap& OleWrap::operator = (const Common::Wrap& wrap)
		{
			fromBase(wrap);
			return *this;
		}


		const OleWrap& OleWrap::operator = (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string OleWrap::ToString() const
		{
			switch (type())
			{
			case square:
				return "square";
			case tight:
				return "tight";
			case topAndBottom:
				return "topAndBottom";
			case none:
				return "none";
			default:
				return "square";
			}
		}


		void OleWrap::fromString(const std::string& value)
		{
			if (value == "square")
				setSquare();
			else if (value == "tight")
				setTight();
			else if (value == "topAndBottom")
				setTopAndBottom();
			else if (value == "none")
				setNone();
			else 
				setSquare();
		}
	} // namespace Logic
} // namespace OOX