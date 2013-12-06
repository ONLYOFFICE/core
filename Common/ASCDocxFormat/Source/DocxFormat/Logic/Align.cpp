
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Align.h"

namespace OOX
{
	namespace Logic
	{
		Align::Align()
		{
		}

		Align::Align(const Common::Align& align)
		{
			fromBase(align);
		}

		Align::Align(const std::string& value)
		{
			fromString(value);
		}
	
		Align::Align(const std::wstring& value)
		{
			fromString(value);
		}

		const Align& Align::operator= (const Common::Align& align)
		{
			fromBase(align);
			return *this;
		}

		const Align& Align::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}

		const std::string Align::ToString() const
		{
			switch (type())
			{
			case left:
				return "left";
			case center:
				return "center";
			case right:
				return "right";
			case both:
				return "both";
			default:
				return "left";
			}
			
			return "left";
		}

		const std::wstring Align::ToStringW() const
		{
			switch (type())
			{
			case left:
				return L"left";
			case center:
				return L"center";
			case right:
				return L"right";
			case both:
				return L"both";
			default:
				return L"left";
			}

			return L"left";
		}

		void Align::fromString(const std::wstring& value)
		{
			if (value == L"center")
				setCenter();
			else if (value == L"right")
				setRight();
			else if (value == L"both")
				setBoth();
			else
				setLeft();
		}

		void Align::fromString(const std::string& value)
		{
			if (value == "center")
				setCenter();
			else if (value == "right")
				setRight();
			else if (value == "both")
				setBoth();
			else
				setLeft();
		}

	} // namespace Logic
} // namespace OOX