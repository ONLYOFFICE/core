#ifndef DROPCAP_H
#define DROPCAP_H

#include "BaseItem.h"

namespace NSDocxRenderer
{
	class CDropCap : public CBaseItem
	{
	public:
		size_t nLines;

		std::wstring wsText;
		std::wstring wsFont;
		LONG nFontSize;

		CDropCap() : CBaseItem(ElemType::etDropCap) {}
		~CDropCap() = default;

		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) override;
		virtual void Clear() override {}
	};
}

#endif // DROPCAP_H
