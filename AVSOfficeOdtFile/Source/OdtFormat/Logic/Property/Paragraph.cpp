
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Paragraph.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{

			Paragraph::Paragraph()
			{
			}


			Paragraph::~Paragraph()
			{
			}


			Paragraph::Paragraph(const XML::XNode& node)
			{
				fromXML(node);
			}


			const Paragraph& Paragraph::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}


			void Paragraph::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);
				Align						= element.attribute("text-align").value();
				JustifySingleWord			= element.attribute("justify-single-word").value();
				HyphenationLadderCount		= element.attribute("hyphenation-ladder-count").value();
				TextAutospace				= element.attribute("text-autospace").value();
				PunctuationWrap				= element.attribute("punctuation-wrap").value();

				LineBreak					= element.attribute("line-break").value();
				BreakBefore					= element.attribute("break-before").value();
                BreakAfter					= element.attribute("break-after").value();
				TabStopDistance				= element.attribute("tab-stop-distance").value();
				WritingMode					= element.attribute("writing-mode").value();
				FontIndependentLineSpacing	= element.attribute("font-independent-line-spacing").value();
				DropCap						= element.element("drop-cap");
				
				KeepWithNext	= element.attribute("keep-with-next").value();
				KeepTogether = element.attribute("keep-together").value();
				AutoTextIndent	= element.attribute("auto-text-indent").value();
				
				NumberLines		= element.attribute("number-lines").value();
				LineNumber		= element.attribute("line-number").value();
				Background		= element.attribute("background-color").value();

				if (element.attribute("line-height").exist())
				{
					LineHeight = element.attribute("line-height").value().ToString();					
					//UniversalUnit p = *LineHeight;
					//if (LineHeight->Type == UniversalUnit::Percent)
					//{
					//	UniversalUnit value(5, UniversalUnit::Mm);
					//	LineHeight->apply(value);
					//}
				}

				if (element.attribute("line-height-at-least").exist())
					LineHeightAtLeast = element.attribute("line-height-at-least").value().ToString();
				if (element.attribute("text-indent").exist())
					TextIndent = element.attribute("text-indent").value().ToString();
				if (element.attribute("margin-left").exist())
					MarginLeft = element.attribute("margin-left").value().ToString();
				if (element.attribute("margin-right").exist())
					MarginRight = element.attribute("margin-right").value().ToString();
				if (element.attribute("margin-top").exist())
					MarginTop = element.attribute("margin-top").value().ToString();
				if (element.attribute("margin-bottom").exist())
					MarginBottom = element.attribute("margin-bottom").value().ToString();

				PageNumber		 = element.attribute("page-namber").value();
				TabStops		 = element.element("tab-stops");
				Border			 = element.attribute("border").value();
				BorderTop		 = element.attribute("border-top").value();
				BorderLeft		 = element.attribute("border-left").value();
				BorderRight		 = element.attribute("border-right").value();
				BorderBottom	 = element.attribute("border-bottom").value();
				SnapToLayoutGrid = element.attribute("snap-to-layout-grid").value();
			}

			
			const XML::XNode Paragraph::toXML() const
			{
				return 
					XML::XElement(ns.style + "paragraph-properties",
						XML::XAttribute(ns.fo + "text-align", Align) +
						XML::XAttribute(ns.style + "justify-single-word", JustifySingleWord) +
						XML::XAttribute(ns.fo + "hyphenation-ladder-count", HyphenationLadderCount) +
						XML::XAttribute(ns.style + "text-autospace", TextAutospace) +
						XML::XAttribute(ns.style + "punctuation-wrap", PunctuationWrap) +
						XML::XAttribute(ns.style + "line-break", LineBreak) +
						XML::XAttribute(ns.fo + "break-before", BreakBefore) +
                        XML::XAttribute(ns.fo + "break-after", BreakAfter) +
						XML::XAttribute(ns.style + "tab-stop-distance", TabStopDistance) +
						XML::XAttribute(ns.style + "writing-mode", WritingMode) +
						XML::XAttribute(ns.style + "font-independent-line-spacing", FontIndependentLineSpacing) +
						XML::XAttribute(ns.fo + "margin-top", MarginTop) +
						XML::XAttribute(ns.fo + "margin-bottom", MarginBottom) +
						XML::XAttribute(ns.fo + "margin-left", MarginLeft) +
						XML::XAttribute(ns.fo + "margin-right", MarginRight) +
						XML::XAttribute(ns.fo + "keep-with-next", KeepWithNext) +
						XML::XAttribute(ns.fo + "keep-together", KeepTogether) +
						XML::XAttribute(ns.fo + "auto-text-indent", AutoTextIndent) +
						XML::XAttribute(ns.fo + "text-indent", TextIndent) +
						XML::XAttribute(ns.text + "number-lines", NumberLines) +
						XML::XAttribute(ns.text + "line-number", LineNumber) +
						XML::XAttribute(ns.fo + "background-color", Background) +
						XML::XAttribute(ns.fo + "line-height", LineHeight) +
						XML::XAttribute(ns.style + "line-height-at-least", LineHeightAtLeast) +
						XML::XAttribute(ns.style + "page-number", PageNumber) +
						XML::XAttribute(ns.fo + "border", Border) +
						XML::XAttribute(ns.fo + "border-left", BorderLeft) +
						XML::XAttribute(ns.fo + "border-right", BorderRight) +
						XML::XAttribute(ns.fo + "border-top", BorderTop) +
						XML::XAttribute(ns.fo + "border-bottom", BorderBottom) +
						XML::XAttribute(ns.style + "snap-to-layout-grid", SnapToLayoutGrid) +
						XML::Write(TabStops) +
						XML::Write(DropCap)
				);
			}


			const bool Paragraph::isSimple() const
			{
				if (Align.is_init() || MarginLeft.is_init() || LineHeight.is_init() || LineHeightAtLeast.is_init() || BreakBefore.is_init() || BreakAfter.is_init() || TabStops.is_init()
					|| KeepTogether.is_init() || KeepWithNext.is_init() || NumberLines.is_init())
					return false;
				return true;
			}


			const bool Paragraph::onlyBorder() const
			{
				return !(BorderLeft.is_init() || BorderRight.is_init() || BorderTop.is_init() || BorderBottom.is_init());
			}


			const bool Paragraph::hasBorders() const
			{
				return (BorderLeft.is_init() || BorderRight.is_init() || BorderTop.is_init() || BorderBottom.is_init() || Border.is_init());
			}


			const Paragraph Paragraph::merge(const Paragraph& prev, const Paragraph& current)
			{
				Paragraph properties;
				properties.Align				= ::merge(prev.Align, current.Align);
				properties.JustifySingleWord	= ::merge(prev.JustifySingleWord, current.JustifySingleWord);
				properties.HyphenationLadderCount = ::merge(prev.HyphenationLadderCount, current.HyphenationLadderCount);
				properties.TextAutospace		= ::merge(prev.TextAutospace, current.TextAutospace);
				properties.PunctuationWrap		= ::merge(prev.PunctuationWrap, current.PunctuationWrap);
				properties.LineBreak			= ::merge(prev.LineBreak, current.LineBreak);
				properties.BreakBefore			= ::merge(prev.BreakBefore, current.BreakBefore);
                properties.BreakAfter			= ::merge(prev.BreakAfter, current.BreakAfter);
				properties.TabStopDistance		= ::merge(prev.TabStopDistance, current.TabStopDistance);
				properties.WritingMode			= ::merge(prev.WritingMode, current.WritingMode);
				properties.FontIndependentLineSpacing = ::merge(prev.FontIndependentLineSpacing, current.FontIndependentLineSpacing);
				properties.MarginTop			= ::merge(prev.MarginTop, current.MarginTop);
				properties.MarginBottom			= ::merge(prev.MarginBottom, current.MarginBottom);
				properties.MarginLeft			= ::merge(prev.MarginLeft, current.MarginLeft);
				properties.MarginRight			= ::merge(prev.MarginRight, current.MarginRight);
				properties.KeepWithNext			= ::merge(prev.KeepWithNext, current.KeepWithNext);
				properties.KeepTogether			= ::merge(prev.KeepTogether, current.KeepTogether);
				properties.TextIndent			= ::merge(prev.TextIndent, current.TextIndent);
				properties.AutoTextIndent		= ::merge(prev.AutoTextIndent, current.AutoTextIndent);
				properties.NumberLines			= ::merge(prev.NumberLines, current.NumberLines);
				properties.LineNumber			= ::merge(prev.LineNumber, current.LineNumber);
				properties.Background			= ::merge(prev.Background, current.Background);
				properties.TabStops				= ::merge(prev.TabStops, current.TabStops);
				properties.DropCap				= ::merge(prev.DropCap, current.DropCap);
				properties.LineHeight			= ::merge(prev.LineHeight, current.LineHeight);
				properties.LineHeightAtLeast	= ::merge(prev.LineHeightAtLeast, current.LineHeightAtLeast);
				properties.PageNumber			= ::merge(prev.PageNumber, current.PageNumber);
				properties.Border				= ::merge(prev.Border, current.Border);
				properties.BorderTop			= ::merge(prev.BorderTop, current.BorderTop);
				properties.BorderLeft			= ::merge(prev.BorderLeft, current.BorderLeft);
				properties.BorderRight			= ::merge(prev.BorderRight, current.BorderRight);
				properties.BorderBottom			= ::merge(prev.BorderBottom, current.BorderBottom);
				properties.SnapToLayoutGrid		= ::merge(prev.SnapToLayoutGrid, current.SnapToLayoutGrid);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt