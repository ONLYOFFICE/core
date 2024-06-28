#pragma once
#include "../../../../DesktopEditor/common/StringBuilder.h"
#include "../../resources/Constants.h"
#include <vector>
#include <memory>

namespace NSDocxRenderer
{
	// взаимное расположение по вертикали со следующим объектом
	enum class eVerticalCrossingType
	{
		vctUnknown,
		vctCurrentInsideNext,
		vctCurrentOutsideNext,
		vctCurrentAboveNext,
		vctCurrentBelowNext,
		vctDublicate,
		vctTopAndBottomBordersMatch,
		vctTopBorderMatch,
		vctBottomBorderMatch,
		vctNoCrossingCurrentAboveNext,
		vctNoCrossingCurrentBelowNext
	};

	// взаимное расположение по горизонтали со следующим объектом
	enum class eHorizontalCrossingType
	{
		hctUnknown,
		hctCurrentInsideNext,
		hctCurrentOutsideNext,
		hctCurrentLeftOfNext,
		hctCurrentRightOfNext,
		hctDublicate,
		hctLeftAndRightBordersMatch,
		hctLeftBorderMatch,
		hctRightBorderMatch,
		hctNoCrossingCurrentLeftOfNext,
		hctNoCrossingCurrentRightOfNext
	};

	class CBaseItem
	{
	public:
		double m_dTop {0.0};
		double m_dBaselinePos {0.0};
		double m_dHeight {0.0};

		double m_dLeft {0.0};
		double m_dRight {0.0};
		double m_dWidth {0.0};

		CBaseItem() = default;
		virtual ~CBaseItem() = default;

		virtual void Clear() = 0;
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) const = 0;
		virtual void ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const = 0;

		virtual eVerticalCrossingType GetVerticalCrossingType(const CBaseItem* oSrc) const;
		virtual eHorizontalCrossingType GetHorizontalCrossingType(const CBaseItem* oSrc) const;
		virtual void RecalcWithNewItem(const CBaseItem* pObj);

		bool AreObjectsNoCrossingByVertically(const CBaseItem* pObj) const noexcept;
		bool AreObjectsNoCrossingByHorizontally(const CBaseItem* pObj) const noexcept;

		CBaseItem& operator=(const CBaseItem& oSrc);
	};

	class COutputObject : public CBaseItem
	{
	public:
		enum class eOutputType
		{
			etAny		= 0,
			etParagraph = 1,
			etShape     = 2,
			etTable     = 3
		};

		COutputObject() : m_eType(eOutputType::etAny) {}
		COutputObject(eOutputType eType) : m_eType(eType) {}
		virtual ~COutputObject() = default;

		COutputObject& operator= (const COutputObject& oObj);

		eOutputType m_eType;
	};
}
