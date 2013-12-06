
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Color.h"
#include "Parse.h"
#include "ColorsTable.h"

namespace OOX
{
	namespace Logic
	{
		static const ColorsTable s_colors;

        Color::Color() : m_Auto(false)
		{
		}


        Color::Color(const Common::Color& color) : m_Auto(false)
		{
			fromBase(color);
		}


        Color::Color(const std::string& value) : m_Auto(false)
		{
			fromString(value);
		}


		const Color& Color::operator= (const Common::Color& color)
		{
            m_Auto = false;
			fromBase(color);
			return *this;
		}

        const Color& Color::operator= (const Color& color)
        {
            m_Auto = color.m_Auto;
            fromBase(color);
            return *this;
        }

		const Color& Color::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}

		const std::string Color::ToString() const
		{
            if (m_Auto)
                return "auto";

			char str[8];
			sprintf(str, "%02x%02x%02x", Red.get(), Green.get(), Blue.get());
            
			return std::string(str);
		}

		void Color::fromString(const std::string& value)
		{
            if (value == "auto")
            {
                m_Auto = true;            
            }
            else if(s_colors.isFromName(value))
			{
				*this = s_colors.fromName(value);
			}
			else
			{
				Red		=	HexString2Int(value.substr(0, 2));
				Green	=	HexString2Int(value.substr(2, 2));
				Blue	=	HexString2Int(value.substr(4, 2));
			}
		}

	} // namespace Logic
} // namespace OOX