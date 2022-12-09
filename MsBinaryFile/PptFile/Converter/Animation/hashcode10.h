#pragma once

#include "intermediate_anim.h"

namespace PPT 
{
	namespace Converter {

		// I couldn't calculate simplest correct hash. Not correct now
		class HashCode10
		{
		public:
			HashCode10() {};
			bool IsValidHash(const Intermediate::SlideAnimation& sldAnim)const;

		private:
			// 1995 hash
			static constexpr size_t RASize = 256 * 256;   // row * col
			static std::array<_UINT32, RASize> InitRandomArray();
			static _UINT32 GetHash1995(const std::vector<Intermediate::SOldAnimation>& anim95);
			static std::vector<Intermediate::SOldAnimation> MergeSlideShapesWithAnim95Struct(const std::vector<Intermediate::SOldAnimation>& vecAnim95, std::unordered_set<_INT32> sldShapesID);
			static _UINT32 GetWroteHash(CRecordPP10SlideBinaryTagExtension* pTagExt);
		};
	}
}
