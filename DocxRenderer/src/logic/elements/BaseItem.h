#pragma once
#include "../DesktopEditor/common/StringBuilder.h"
#include <vector>

namespace NSDocxRenderer
{
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
		enum class ElemType
		{
			etContText  = 0,
			etTextLine  = 1,
			etParagraph = 2,
			etImage     = 3,
			etShape     = 4,
			etCell      = 5,
			etRow       = 6,
			etTable     = 7,
		};

		ElemType m_eType;

		bool m_bIsNotNecessaryToUse {false};

		//General
		double m_dLeft {0.0};
		double m_dTop {0.0};
		double m_dWidth {0.0};
		double m_dHeight {0.0};

		//Secondary
		double m_dBaselinePos {0.0};
		double m_dRight {0.0};

	public:
		CBaseItem(const ElemType& eType): m_eType(eType) {}
		virtual ~CBaseItem() {}
		virtual void Clear() = 0;

		CBaseItem& operator=(const CBaseItem& oSrc);

		virtual void AddContent(CBaseItem* pObj);
		virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) = 0;

		eVerticalCrossingType GetVerticalCrossingType(const CBaseItem* oSrc);
		eHorizontalCrossingType GetHorizontalCrossingType(const CBaseItem* oSrc);

		bool AreObjectsNoCrossingByVertically(const CBaseItem* pObj);
		bool AreObjectsNoCrossingByHorizontally(const CBaseItem* pObj);

		double CalculateBeforeSpacing(double dPreviousBaseline);

		template<typename T>
		static void SortByLeft(std::vector<T*>& oArray)
		{
			std::sort(oArray.begin(), oArray.end(), [](T* a, T* b) {
				return a->IsCurrentLeftOfNext(a);
			});
		}

		template<typename T>
		static void SortByBaseline(std::vector<T*>& oArray)
		{
			std::sort(oArray.begin(), oArray.end(), [](T* a, T* b) {
				return a->IsCurrentAboveOfNext(b);
			});
		}

	private:
		bool IsCurrentLeftOfNext(const CBaseItem* oSrc);
		bool IsCurrentAboveOfNext(const CBaseItem* oSrc);
	};
}
