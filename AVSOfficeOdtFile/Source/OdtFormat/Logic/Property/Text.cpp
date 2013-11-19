
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Text.h"
#include <boost/lexical_cast.hpp>


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{

			Text::Text()
			{
			}


			Text::~Text()
			{
			}


			Text::Text(const XML::XNode& node)
			{
				fromXML(node);
			}


			const Text& Text::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}


			void Text::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);

				if(element.attribute("font-size").exist())
				{				
					const std::string strSize = element.attribute("font-size").value();
				
					FontSize = sizeFromXML(strSize);					
				}			

				FontName			= element.attribute("font-name").value();
				FontNameAsian		= element.attribute("font-name-asian").value();
				FontNameComplex		= element.attribute("font-name-complex").value();
				Language			= element.attribute("language").value();
				Country				= element.attribute("country").value();
				FontColor			= element.attribute("color").value();
				Background			= element.attribute("background-color").value();				
				FontFamily			= element.attribute("font-family").value();
				FontFamilyGeneric	= element.attribute("font-family-generic").value();
				FontPitch			= element.attribute("font-pitch").value();

				LineThroughType		= element.attribute("text-line-through-type").value();
				LineThroughStyle	= element.attribute("text-line-through-style").value();
				LineThroughWidth	= element.attribute("text-line-through-width").value();
				LineThroughColor	= element.attribute("text-line-through-color").value();

				UnderlineType		= element.attribute("text-underline-type").value();
				UnderlineStyle		= element.attribute("text-underline-style").value();
				UnderlineWidth		= element.attribute("text-underline-width").value();
				UnderlineColor		= element.attribute("text-underline-color").value();

				UseWindowFontColor	= element.attribute("use-window-font-color").value();
				Index				= element.attribute("text-position").value();
				FontVariant			= element.attribute("font-variant").value();
				TextTransform		= element.attribute("text-transform").value();
				Outline				= element.attribute("text-outline").value();
				Shadow				= element.attribute("text-shadow").value();
				Display				= element.attribute("display").value();
				Relief				= element.attribute("font-relief").value();
				
				if (element.attribute("text-scale").exist())
					TextScale = element.attribute("text-scale").value();

				if (element.attribute("letter-spacing").exist())
					if (element.attribute("letter-spacing").value() != "normal")
						LetterSpacing = element.attribute("letter-spacing").value();

				//if (element.attribute("text-line-through-style").exist()) 
				//	LineThroughStyle = element.attribute("text-line-through-style").value().ToString() == "solid";
				
				if (element.attribute("font-style").exist()) 
					Italic = element.attribute("font-style").value().ToString() == "italic";

				if (element.attribute("font-weight").exist())
					Bold = element.attribute("font-weight").value().ToString() == "bold";

				//if (element.attribute("text-underline-style").exist())
				//	Under = element.attribute("text-underline-style").value().ToString() == "solid";

				if (element.attribute("hyphenate").exist())
				{
					Hyphenate = Odt::Logic::Hyphenate();
					Hyphenate->Use = element.attribute("hyphenate").value();
					Hyphenate->Remain = element.attribute("hyphenation-remain-char-count").value();
					Hyphenate->Push = element.attribute("hyphenation-push-char-count").value();
				}
			}


			const XML::XNode Text::toXML() const
			{
				XML::XElement textProperties = 
					XML::XElement(ns.style + "text-properties", 
						XML::XAttribute(ns.style + "use-window-font-color", UseWindowFontColor) +
						XML::XAttribute(ns.fo + "font-family", FontFamily) +
						XML::XAttribute(ns.style + "font-name", FontName) + 
						XML::XAttribute(ns.style + "font-family-generic", FontFamilyGeneric) +
						XML::XAttribute(ns.style + "font-pitch", FontPitch) +
						XML::XAttribute(ns.style + "font-name-asian", FontNameAsian) + 
						XML::XAttribute(ns.style + "font-name-complex", FontNameComplex) +
						XML::XAttribute(ns.fo + "language", Language) +
						XML::XAttribute(ns.fo + "country", Country) + 
						XML::XAttribute(ns.fo + "color", FontColor) +
						XML::XAttribute(ns.fo + "background-color", Background) +
						XML::XAttribute(ns.style + "text-position", Index) +
						XML::XAttribute(ns.fo + "letter-spacing", LetterSpacing) +
						XML::XAttribute(ns.style + "text-scale", TextScale) +

						XML::XAttribute(ns.style + "text-line-through-style",  LineThroughStyle) +
						XML::XAttribute(ns.style + "text-line-through-type",  LineThroughType) +
						XML::XAttribute(ns.style + "text-line-through-width",  LineThroughWidth) +
						XML::XAttribute(ns.style + "text-line-through-color",  LineThroughColor) +

						XML::XAttribute(ns.style + "text-underline-style",  UnderlineStyle) +
						XML::XAttribute(ns.style + "text-underline-type",  UnderlineType) +
						XML::XAttribute(ns.style + "text-underline-width",  UnderlineWidth) +
						XML::XAttribute(ns.style + "text-underline-color",  UnderlineColor) +

						XML::XAttribute(ns.fo + "font-variant", FontVariant) +
						XML::XAttribute(ns.fo + "text-transform", TextTransform) +
						XML::XAttribute(ns.style + "text-outline", Outline) +
						XML::XAttribute(ns.fo + "text-shadow", Shadow) +
						XML::XAttribute(ns.text + "display", Display) +
						XML::XAttribute(ns.style + "font-relief", Relief)
						);

				//if (LineThroughStyle.is_init() && LineThroughStyle)
				//	textProperties.Add(	XML::XAttribute(ns.style + "text-line-through-style", "solid"));

				if (Italic.is_init() && Italic)
					textProperties.Add(	XML::XAttribute(ns.fo + "font-style", "italic") +
															XML::XAttribute(ns.style + "font-style-asian", "italic") +
															XML::XAttribute(ns.style + "font-style-complex", "italic"));
				if (Italic.is_init() && Italic == false)
					textProperties.Add(	XML::XAttribute(ns.fo + "font-style", "normal") +
															XML::XAttribute(ns.style + "font-style-asian", "normal") +
															XML::XAttribute(ns.style + "font-style-complex", "normal"));

				if (Bold.is_init() && Bold)
					textProperties.Add(	XML::XAttribute(ns.fo + "font-weight", "bold") +
															XML::XAttribute(ns.style + "font-weight-asian", "bold") +
															XML::XAttribute(ns.style + "font-weight-complex", "bold"));
				if (Bold.is_init() && Bold == false)
					textProperties.Add(	XML::XAttribute(ns.fo + "font-weight", "normal") +
															XML::XAttribute(ns.style + "font-weight-asian", "normal") +
															XML::XAttribute(ns.style + "font-weight-complex", "normal"));

				//if (Under.is_init() && Under)
				//	textProperties.Add(	XML::XAttribute(ns.style + "text-underline-style", "solid") +
				//											XML::XAttribute(ns.style + "text-underline-width", "auto") +
				//											XML::XAttribute(ns.style + "text-underline-color", "font-color"));
				//if (Under.is_init() && Under == false)
				//	textProperties.Add(	XML::XAttribute(ns.style + "text-underline-style", "none"));

				if (FontSize.is_init())
					textProperties.Add(	XML::XAttribute(ns.fo + "font-size", FontSize) +
															XML::XAttribute(ns.style + "font-size-asian", FontSize) +
															XML::XAttribute(ns.style + "font-size-complex", FontSize));

				if (Hyphenate.is_init())
					textProperties.Add(	XML::XAttribute(ns.fo + "hyphenate", Hyphenate->Use) +
															XML::XAttribute(ns.fo + "hyphenation-remain-char-count", Hyphenate->Remain) + 
															XML::XAttribute(ns.fo + "hyphenation-push-char-count", Hyphenate->Push));

				return textProperties;
			}


			const bool Text::isSimple() const
			{
				if (Bold.is_init() && Bold == true)
					return false;
				if (Italic.is_init() && Italic == true)
					return false;
				if ((UnderlineStyle.get_value_or("none") != "none") && (UnderlineType.get_value_or("sigle") != "none"))
					return false;
				if (FontName.is_init())
					return false;
				if (FontNameAsian.is_init())
					return false;
				if (FontNameComplex.is_init())
					return false;
				if (FontColor.is_init())
					return false;
				if (Background.is_init())
					return false;
				if (FontSize.is_init())
					return false;
				if (FontVariant.is_init())
					return false;
				if (TextTransform.is_init())
					return false;
				if (Index.is_init())
					return false;
				if (LetterSpacing.is_init())
					return false;
				if ((LineThroughStyle.get_value_or("none") != "none") && (LineThroughType.get_value_or("sigle") != "none"))
					return false;
				if(Outline.is_init())
					return false;
				if(Shadow.is_init())
					return false;
				if(Display.is_init())
					return false;
				if(Relief.is_init())
					return false;
				return true;
			}


			const UnitPt Text::sizeFromXML(const std::string& strSize)
			{
				const size_t pos = strSize.find_first_not_of("-.0123456789");
				const std::string ext = strSize.substr(pos, strSize.size() - pos);
				if (ext == "%")
				{
					const int value = boost::lexical_cast<int>(strSize.substr(0, pos));
					UnitPt unitPt = (14 * value / 100);	
					return unitPt;					
				}
				else
				{
					return strSize;
				}
			}


			const Text Text::merge(const Text& prev, const Text& current)
			{
				Text properties;
				properties.FontName			= ::merge(prev.FontName, current.FontName);
				properties.FontNameAsian	= ::merge(prev.FontNameAsian, current.FontNameAsian);
				properties.FontNameComplex	= ::merge(prev.FontNameComplex, current.FontNameComplex);
				properties.Language			= ::merge(prev.Language, current.Language);
				properties.Country			= ::merge(prev.Country, current.Country);
				properties.FontColor		= ::merge(prev.FontColor, current.FontColor);
				properties.Background		= ::merge(prev.Background, current.Background);
				properties.Bold				= ::merge(prev.Bold, current.Bold);
				properties.Italic			= ::merge(prev.Italic, current.Italic);
				//properties.Under			= ::merge(prev.Under, current.Under);
				properties.FontSize			= ::merge(prev.FontSize, current.FontSize);
				properties.UseWindowFontColor	= ::merge(prev.UseWindowFontColor, current.UseWindowFontColor);
				properties.Hyphenate		= ::merge(prev.Hyphenate, current.Hyphenate);
				properties.Index			= ::merge(prev.Index, current.Index);
				properties.LetterSpacing	= ::merge(prev.LetterSpacing, current.LetterSpacing);
				properties.FontFamily		= ::merge(prev.FontFamily, current.FontFamily);
				properties.FontFamilyGeneric	= ::merge(prev.FontFamilyGeneric, current.FontFamilyGeneric);
				properties.FontPitch			= ::merge(prev.FontPitch, current.FontPitch);

				properties.LineThroughStyle		= ::merge(prev.LineThroughStyle, current.LineThroughStyle);
				properties.LineThroughType		= ::merge(prev.LineThroughType, current.LineThroughType);
				properties.LineThroughWidth		= ::merge(prev.LineThroughWidth, current.LineThroughWidth);
				properties.LineThroughColor		= ::merge(prev.LineThroughColor, current.LineThroughColor);

				properties.UnderlineStyle		= ::merge(prev.UnderlineStyle, current.UnderlineStyle);
				properties.UnderlineType		= ::merge(prev.UnderlineType, current.UnderlineType);
				properties.UnderlineWidth		= ::merge(prev.UnderlineWidth, current.UnderlineWidth);
				properties.UnderlineColor		= ::merge(prev.UnderlineColor, current.UnderlineColor);

				properties.FontVariant			= ::merge(prev.FontVariant, current.FontVariant);
				properties.TextTransform		= ::merge(prev.TextTransform, current.TextTransform);
				properties.Outline		= ::merge(prev.Outline, current.Outline);
				properties.Shadow		= ::merge(prev.Shadow, current.Shadow);
				properties.Display		= ::merge(prev.Display, current.Display);
				properties.Relief		= ::merge(prev.Relief, current.Relief);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt