
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ColorsTable.h"
#include "math.h"

namespace OOX
{
	namespace Logic
	{
		ColorsTable::ColorsTable()
		{			
			m_storage.push_back(std::make_pair("black", Common::Color(0x00, 0x00, 0x00)));
			m_storage.push_back(std::make_pair("blue", Common::Color(0x00, 0x00, 0xFF)));
			m_storage.push_back(std::make_pair("cyan", Common::Color(0x00, 0xFF, 0xFF)));
			m_storage.push_back(std::make_pair("green", Common::Color(0x00, 0x80, 0x00)));
			m_storage.push_back(std::make_pair("magenta", Common::Color(0xFF, 0x00, 0xFF)));
			m_storage.push_back(std::make_pair("red", Common::Color(0xFF, 0x00, 0x00)));
			m_storage.push_back(std::make_pair("yellow", Common::Color(0xFF, 0xFF, 0x00)));
			m_storage.push_back(std::make_pair("white", Common::Color(0xFF, 0xFF, 0xFF)));
			m_storage.push_back(std::make_pair("darkBlue", Common::Color(0x00, 0x00, 0x8B)));
			m_storage.push_back(std::make_pair("darkCyan", Common::Color(0x00, 0x8B, 0x8B)));
			m_storage.push_back(std::make_pair("darkGreen", Common::Color(0x00, 0x64, 0x00)));
			m_storage.push_back(std::make_pair("darkMagenta", Common::Color(0x8B, 0x00, 0x8B)));
			m_storage.push_back(std::make_pair("darkRed", Common::Color(0x8B, 0x00, 0x00)));
			m_storage.push_back(std::make_pair("darkYellow", Common::Color(0x80, 0x80, 0x00)));
			m_storage.push_back(std::make_pair("darkGray", Common::Color(0xA9, 0xA9, 0xA9)));
			m_storage.push_back(std::make_pair("lightGray", Common::Color(0xD3, 0xD3, 0xD3)));
			m_storage.push_back(std::make_pair("auto", Common::Color(0x00, 0x00, 0x00)));
		}

		const Common::Color ColorsTable::fromName(const std::string& name) const
		{
			for (std::vector<std::pair<std::string, Common::Color> >::const_iterator iter = m_storage.begin(); iter != m_storage.end(); ++iter)
			{
				if ((*iter).first == name)
					return (*iter).second;
			}

			return Common::Color(0xFF, 0xFF, 0xFF);
		}

		const bool ColorsTable::isFromName(const std::string& name) const
		{
			for (std::vector<std::pair<std::string, Common::Color> >::const_iterator iter = m_storage.begin(); iter != m_storage.end(); ++iter)
			{
				if ((*iter).first == name)
					return true;
			}

			return false;
		}

		const std::string ColorsTable::fromColor(const Common::Color& color) const
		{
			std::string name;
			int diff = 255 * 255 * 3;

			for (std::vector<std::pair<std::string, Common::Color> >::const_iterator iter = m_storage.begin(); iter != m_storage.end(); ++iter)
			{
				int new_diff =	(color.Red - (*iter).second.Red) * (color.Red - (*iter).second.Red) +
												(color.Green - (*iter).second.Green) * (color.Green - (*iter).second.Green) + 
												(color.Blue - (*iter).second.Blue) * (color.Blue - (*iter).second.Blue);

				if (new_diff < diff)
				{
					diff = new_diff;
					name = (*iter).first;
				}
			}

			return name;
		}

	} // namespace Logic
} // namespace OOX