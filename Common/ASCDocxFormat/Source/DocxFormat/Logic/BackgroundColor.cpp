
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "BackgroundColor.h"
#include "ColorsTable.h"


namespace OOX
{
	namespace Logic
	{

		static const ColorsTable s_colors;

	
		BackgroundColor::BackgroundColor()
		{
		}


		BackgroundColor::BackgroundColor(const Common::Color& color)
		{
			fromBase(color);
		}


		BackgroundColor::BackgroundColor(const std::string& value)
		{
			fromString(value);
		}


		const BackgroundColor& BackgroundColor::operator= (const Common::Color& color)
		{
			fromBase(color);
			return *this;
		}


		const BackgroundColor& BackgroundColor::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string BackgroundColor::ToString() const
		{
			return s_colors.fromColor(*this);
		}


		void BackgroundColor::fromString(const std::string& value)
		{
			*this = s_colors.fromName(value);
		}

	} // namespace Logic
} // namespace OOX