#pragma once

#include "../../../OOXML/PPTXFormat/Logic/Timing/Timing.h"
#include "../PPTXWriter/ImageManager.h"
#include "Animation/intermediate_anim.h"

namespace PPT {
	namespace Converter {
		class Timing
		{
		public:
			Timing(const Intermediate::SlideAnimation& slideAnim, const std::unordered_set<int>& shapesID);
			PPTX::Logic::Timing Convert(CExMedia* pExMedia, CRelsGenerator *pRels);

			bool HasAnimation() const;

		private:
			bool TryToConvertTiming2010();
			bool TryToConvertTiming1995();

		private:
			const Intermediate::SlideAnimation& slideAnim;
			CExMedia* pExMedia;
			CRelsGenerator* pRels;
			const std::unordered_set<int> shapesID;
			PPTX::Logic::Timing timing;
		};
	}
}
