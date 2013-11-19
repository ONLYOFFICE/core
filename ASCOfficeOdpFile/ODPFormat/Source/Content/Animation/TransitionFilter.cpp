#include "TransitionFilter.h"

namespace Odp
{
	namespace Content
	{
		TransitionFilter::TransitionFilter()
		{
		}

		TransitionFilter::~TransitionFilter()
		{
		}

		TransitionFilter::TransitionFilter(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TransitionFilter& TransitionFilter::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TransitionFilter::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Formula = element.attribute("formula").value();//anim:formula
			SubItem = element.attribute("sub-item").value();//anim:sub-item
			Accelerate = element.attribute("accelerate").value();//smil:accelerate
			Accumulate = element.attribute("accumulate").value();//smil:accumulate
			Additive = element.attribute("additive").value();//smil:additive
			AutoReverse = element.attribute("autoReverse").value();//smil:autoReverse
			Begin = element.attribute("begin").value();//smil:begin
			By = element.attribute("by").value();//smil:by
			CalcMode = element.attribute("calcMode").value();//smil:calcMode
			Decelerate = element.attribute("decelerate").value();//smil:decelerate
			Direction = element.attribute("direction").value();//smil:direction
			Dur = element.attribute("dur").value();//smil:dur
			End = element.attribute("end").value();//smil:end
			FadeColor = element.attribute("fadeColor").value();//smil:fadeColor
			Fill = element.attribute("fill").value();//smil:fill
			FillDefault = element.attribute("fillDefault").value();//smil:fillDefault
			From = element.attribute("from").value();//smil:from
			Mode = element.attribute("mode").value();//smil:mode
			RepeatCount = element.attribute("repeatCount").value();//smil:repeatCount
			RepeatDur = element.attribute("repeatDur").value();//smil:repeatDur
			Restart = element.attribute("restart").value();//smil:restart
			RestartDefault = element.attribute("restartDefault").value();//smil:restartDefault
			SubType = element.attribute("subtype").value();//smil:subtype
			TargetElement = element.attribute("targetElement").value();//smil:targetElement
			To = element.attribute("to").value();//smil:to
			Type = element.attribute("type").value();//smil:type
			Values = element.attribute("values").value();//smil:values
		}

		const XML::XNode TransitionFilter::toXML() const
		{
			return 
				XML::XElement(ns.anim + "transitionFilter", 
				XML::XAttribute(ns.anim + "formula", Formula) +
				XML::XAttribute(ns.anim + "sub-item", SubItem) +
				XML::XAttribute(ns.smil + "accelerate", Accelerate) +
				XML::XAttribute(ns.smil + "accumulate", Accumulate) +
				XML::XAttribute(ns.smil + "additive", Additive) +
				XML::XAttribute(ns.smil + "autoReverse", AutoReverse) +
				XML::XAttribute(ns.smil + "begin", Begin) +
				XML::XAttribute(ns.smil + "by", By) +
				XML::XAttribute(ns.smil + "calcMode", CalcMode) +
				XML::XAttribute(ns.smil + "decelerate", Decelerate) +
				XML::XAttribute(ns.smil + "direction", Direction) +
				XML::XAttribute(ns.smil + "dur", Dur) +
				XML::XAttribute(ns.smil + "end", End) +
				XML::XAttribute(ns.smil + "fadeColor", FadeColor) +
				XML::XAttribute(ns.smil + "fill", Fill) +
				XML::XAttribute(ns.smil + "fillDefault", FillDefault) +
				XML::XAttribute(ns.smil + "from", From) +
				XML::XAttribute(ns.smil + "mode", Mode) +
				XML::XAttribute(ns.smil + "repeatCount", RepeatCount) +
				XML::XAttribute(ns.smil + "repeatDur", RepeatDur) +
				XML::XAttribute(ns.smil + "restart", Restart) +
				XML::XAttribute(ns.smil + "restartDefault", RestartDefault) +
				XML::XAttribute(ns.smil + "subtype", SubType) +
				XML::XAttribute(ns.smil + "targetElement", TargetElement) +
				XML::XAttribute(ns.smil + "to", To) +
				XML::XAttribute(ns.smil + "type", Type) +
				XML::XAttribute(ns.smil + "values", Values)
			);
		}
	} // namespace Content
} // namespace Odp