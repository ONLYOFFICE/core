#include "TextShapeType.h"

namespace PPTX
{
	namespace Limit
	{
		TextShapeType::TextShapeType()
		{
			add("textArchDown");
			add("textArchDownPour");
			add("textArchUp");
			add("textArchUpPour");
			add("textButton");
			add("textButtonPour");
			add("textCanDown");
			add("textCanUp");
			add("textCascadeDown");
			add("textCascadeUp");
			add("textChevron");
			add("textChevronInverted");
			add("textCircle");
			add("textCirclePour");
			add("textCurveDown");
			add("textCurveUp");
			add("textDeflate");
			add("textDeflateBottom");
			add("textDeflateInflate");
			add("textDeflateInflateDeflate");
			add("textDeflateTop");
			add("textDoubleWave1");
			add("textFadeDown");
			add("textFadeLeft");
			add("textFadeRight");
			add("textFadeUp");
			add("textInflate");
			add("textInflateBottom");
			add("textInflateTop");
			add("textNoShape");
			add("textPlain");
			add("textRingInside");
			add("textRingOutside");
			add("textSlantDown");
			add("textSlantUp");
			add("textStop");
			add("textTriangle");
			add("textTriangleInverted");
			add("textWave1");
			add("textWave2");
			add("textWave4");
 		}

		const std::string TextShapeType::no_find() const
		{
			return "textNoShape";
		}
	} // namespace Limit
} // namespace PPTX
