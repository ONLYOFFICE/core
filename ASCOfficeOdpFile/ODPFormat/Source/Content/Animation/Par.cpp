#include "Par.h"

namespace Odp
{
	namespace Content
	{
		Par::Par()
		{
		}

		Par::~Par()
		{
		}

		Par::Par(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Par& Par::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Par::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			AnimId = element.attribute(ns.anim + "id").value();//anim:id
			GroupId = element.attribute("group-id").value();//presentation:group-id
			MasterElement = element.attribute("master-element").value();//presentation:master-element
			NodeType = element.attribute("node-type").value();//presentation:node-type
			PresetClass = element.attribute("preset-class").value();//presentation:preset-class
			PresetId = element.attribute("preset-id").value();//presentation:preset-id
			PresetSubType = element.attribute("preset-sub-type").value();//presentation:preset-sub-type
			Accelerate = element.attribute("accelerate").value();//smil:accelerate
			AutoReverse = element.attribute("autoReverse").value();//smil:autoReverse
			Begin = element.attribute("begin").value();//smil:begin
			Decelerate = element.attribute("decelerate").value();//smil:decelerate
			Dur = element.attribute("dur").value();//smil:dur
			End = element.attribute("end").value();//smil:end
			EndSync = element.attribute("endsync").value();//smil:endsync
			Fill = element.attribute("fill").value();//smil:fill
			FillDefault = element.attribute("fillDefault").value();//smil:fillDefault
			RepeatCount = element.attribute("repeatCount").value();//smil:repeatCount
			RepeatDur = element.attribute("repeatDur").value();//smil:repeatDur
			Restart = element.attribute("restart").value();//smil:restart
			RestartDefault = element.attribute("restartDefault").value();//smil:restartDefault

			XML::Fill(Childs, element, "par", "transitionFilter");
		}

		const XML::XNode Par::toXML() const
		{
			return 
				XML::XElement(ns.anim + "par", 
				XML::XAttribute(ns.anim + "id", AnimId) +
				XML::XAttribute(ns.presentation + "group-id", GroupId) +
				XML::XAttribute(ns.presentation + "master-element", MasterElement) +
				XML::XAttribute(ns.presentation + "node-type", NodeType) +
				XML::XAttribute(ns.presentation + "preset-class", PresetClass) +
				XML::XAttribute(ns.presentation + "preset-id", PresetId) +
				XML::XAttribute(ns.presentation + "preset-sub-type", PresetSubType) +
				XML::XAttribute(ns.smil + "accelerate", Accelerate) +
				XML::XAttribute(ns.smil + "autoReverse", AutoReverse) +
				XML::XAttribute(ns.smil + "begin", Begin) +
				XML::XAttribute(ns.smil + "decelerate", Decelerate) +
				XML::XAttribute(ns.smil + "dur", Dur) +
				XML::XAttribute(ns.smil + "end", End) +
				XML::XAttribute(ns.smil + "endsync", EndSync) +
				XML::XAttribute(ns.smil + "fill", Fill) +
				XML::XAttribute(ns.smil + "fillDefault", FillDefault) +
				XML::XAttribute(ns.smil + "repeatCount", RepeatCount) +
				XML::XAttribute(ns.smil + "repeatDur", RepeatDur) +
				XML::XAttribute(ns.smil + "restart", Restart) +
				XML::XAttribute(ns.smil + "restartDefault", RestartDefault) +
				XML::Write(Childs)
			);
		}
	} // namespace Content
} // namespace Odp