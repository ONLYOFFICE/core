
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Spacing.h"


namespace OOX
{
	namespace Logic
	{

		Spacing::Spacing()
		{
		}


		Spacing::~Spacing()
		{
		}
	

		Spacing::Spacing(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Spacing& Spacing::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Spacing::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if (element.attribute("after").exist())
			{
				int after = element.attribute("after").value();
				After = UniversalUnit(after, UniversalUnit::Dx);
			}
			
			if (element.attribute("before").exist())
			{
				int before = element.attribute("before").value();
				Before = UniversalUnit(before, UniversalUnit::Dx);
			}
			
			AfterLines = element.attribute("afterLines").value();
			BeforeLines = element.attribute("beforeLines").value();
			LineRule = element.attribute("lineRule").value();

			if (element.attribute("line").exist())
			{
				int line = element.attribute("line").value();
				Line = UniversalUnit(line, UniversalUnit::Dx);
			}

			BeforeAutospacing	= element.attribute("beforeAutospacing").value();
			AfterAutospacing	= element.attribute("afterAutospacing").value();
		}


		const XML::XNode Spacing::toXML() const
		{
			return
				XML::XElement(ns.w + "spacing", 
					XML::XAttribute(ns.w + "after", After) +									
					XML::XAttribute(ns.w + "afterAutospacing", AfterAutospacing) +
					XML::XAttribute(ns.w + "afterLines", AfterLines) +
					XML::XAttribute(ns.w + "before", Before) +
					XML::XAttribute(ns.w + "beforeAutospacing", BeforeAutospacing) +	
					XML::XAttribute(ns.w + "beforeLines", BeforeLines) +
					XML::XAttribute(ns.w + "line", Line) +
					XML::XAttribute(ns.w + "lineRule", LineRule) 
				);
		}


		const bool Spacing::isSimple() const
		{
			return true;
			//return (!After.is_init() && !Before.is_init() && !Line.is_init() && !AfterLines.is_init() && !BeforeLines.is_init() && !LineRule.is_init()
			//	&& !BeforeAutospacing.is_init() && !AfterAutospacing.is_init());
		}

	} // namespace Logic
} // namespace OOX