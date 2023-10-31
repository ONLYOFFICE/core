#ifndef DROPCAP_H
#define DROPCAP_H

#include "BaseItem.h"

namespace NSDocxRenderer
{
	class CDropCap : public COutputObject
	{
	public:
		size_t nLines;

		std::wstring wsText;
		std::wstring wsFont;
		LONG nFontSize; // Pt * 2
		LONG nOffset;

//		CDropCap() : CBaseItem(ElemType::etDropCap) {}
		~CDropCap() = default;

		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const override;
		virtual void Clear() override {}
	};
}

#endif // DROPCAP_H
