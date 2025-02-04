#pragma once
#include "../../../../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
	class CParagraphStyle
	{
	public:
		CParagraphStyle();
		CParagraphStyle(const std::wstring& wsStyleId, const std::wstring& wsName);
		~CParagraphStyle();

		void ToXml(NSStringUtils::CStringBuilder& oWriter);

		std::wstring wsStyleId;
		std::wstring wsName;
		std::wstring wsBasedOn;

		bool bIsDefault {false};
		bool bIsSemiHidden {false};
		bool bIsUnhideWhenUsed {true};
		LONG nUiPriority {0};
	};
}
