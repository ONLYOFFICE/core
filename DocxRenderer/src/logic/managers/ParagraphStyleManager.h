#pragma once
#include <list>

#include "../elements/Paragraph.h"
#include "../styles/ParagraphStyle.h"

namespace NSDocxRenderer
{
	class CParagraphStyleManager
	{
	public:
		CParagraphStyleManager();
		~CParagraphStyleManager();

		std::wstring GetDefaultParagraphStyleId(const CParagraph& oParagraph) const noexcept;
		double GetAvgFontSize() const noexcept;

		void UpdateAvgFontSize(double dFontSize);
		void ToXml(NSStringUtils::CStringBuilder& oWriter);

	private:
		std::list<CParagraphStyle> m_arDefaultParagraphStyles;
		// std::list<CParagraphStyle> m_arCustomParagraphStyles;

		double m_dAvgFontSize = 0;
		int m_nN = 0;
	};
}
