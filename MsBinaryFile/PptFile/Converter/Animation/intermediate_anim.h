#pragma once

#include "../../../../OOXML/PPTXFormat/Logic/Timing/Timing.h"

#include "../../Records/SlideProgTagsContainer.h"
#include "../../PPTXWriter/ImageManager.h"
#include "../../Records/Animations/AnimationInfoContainer.h"
#include <unordered_set>

namespace PPT 
{
	namespace Intermediate
	{
		struct SValue
		{
			enum
			{
				str,
				dbl
			};
			SValue(const std::wstring& str) : strVal(str), type(SValue::str) {}
			SValue(const wchar_t* str) : strVal(str), type(SValue::str) {}
			SValue(const double& dbl) : dblVal(dbl), type(SValue::dbl) {}

			std::wstring strVal;
			double dblVal;
			const int type;
		};

		struct SOldAnimation
		{
			_INT32 shapeId;
			CRecordAnimationInfoContainer* anim;

			_UINT32 getAnimDur() const;
		};

		struct SlideAnimation
		{
			CRecordPP10SlideBinaryTagExtension* pAnim_2010 = nullptr;
			std::vector<SOldAnimation> arrAnim_1995;
			std::unordered_set<_INT32> realShapesIds;
		};
	}
}
