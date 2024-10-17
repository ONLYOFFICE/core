#pragma once
#include "BaseItem.h"

namespace NSDocxRenderer
{
	class CDropCap : public CBaseItem
	{
	public:
		size_t nLines;

		std::wstring wsText;
		std::wstring wsFont;
		LONG nFontSize; // Pt * 2
		LONG nOffset;

		CDropCap() = default;
		~CDropCap() = default;

		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const override final;
		virtual void Clear() override {}

	private:
		void BuildXml(NSStringUtils::CStringBuilder& oWriter, const std::wstring& wsTag) const;
	};
}
