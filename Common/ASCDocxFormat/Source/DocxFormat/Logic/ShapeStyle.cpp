
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ShapeStyle.h"
#include "ASCStlUtils.h"

namespace OOX
{
	namespace Logic
	{
		ShapeStyle::ShapeStyle()
		{

		}

		ShapeStyle::ShapeStyle(const std::string& value)
		{
			FromString(value);
		}

		const ShapeStyle& ShapeStyle::operator =(const std::string& value)
		{
			FromString(value);
			return *this;
		}

		const std::string ShapeStyle::ToString () const
		{
			return std::string("");
		}

		void ShapeStyle::FromString(const std::string& value)
		{			
			std::vector<std::string> container;
			StlUtils::SplitStrings(container, value, ';');

			distanceLeftTop.init();
			distanceLeftTop->X = 0;
			distanceLeftTop->X = 0;

			distanceRightBottom.init();
			distanceRightBottom->X = 0;
			distanceRightBottom->Y = 0;

			leftTop.init();
			leftTop->X = 0;
			leftTop->Y = 0;

			msoWidthPercent					=	std::string("");
			msoHeightPercent				=	std::string("");
			msoPositionHorizontal			=	std::string("");
			msoPositionVertical				=	std::string("");
			msoPositionHorizontalRelative	=	std::string("");
			msoPositionVerticalRelative		=	std::string("");
			textAnchor						=	std::string("");

			isLocalUnit = false;
		
			for (std::vector<std::string>::const_iterator iter = container.begin(); iter != container.end(); ++iter)
			{
				std::vector<std::string> pair;
				StlUtils::SplitStrings(pair, (*iter), ':');

				std::string name = pair.front();

				std::string::size_type start = name.find_first_not_of(" ");
				std::string::size_type end = name.find_last_not_of(" ");

				if (start != std::string::npos && end != std::string::npos)
					name = name.substr(start, end - start + 1);

				if (name == "position")
				{
					Position	=	pair.back();
				}
				else if (name == "visibility")
				{
					visibility	=	pair.back();
				}
				else if (name == "margin-left")
				{
					Point.init();
					Point->X = pair.back();
				}
				else if (name == "margin-top")
				{
					Point.init();
					Point->Y = pair.back();
				}
				else if (name == "width")
				{
					Size->Width = pair.back();

					if ((std::string::npos == pair.back().find("pt")) &&
						(std::string::npos == pair.back().find("mm")) && 
						(std::string::npos == pair.back().find("cm")) && 
						(std::string::npos == pair.back().find("px")) &&
						(std::string::npos == pair.back().find("in")) &&
						(std::string::npos == pair.back().find("%")) &&
						(std::string::npos == pair.back().find("*")) )
					{
						isLocalUnit = true;
					}
				}
				else if (name == "height")
				{
					Size->Height = pair.back();
				
					if ((std::string::npos == pair.back().find("pt")) &&
						(std::string::npos == pair.back().find("mm")) && 
						(std::string::npos == pair.back().find("cm")) && 
						(std::string::npos == pair.back().find("px")) &&
						(std::string::npos == pair.back().find("in")) &&
						(std::string::npos == pair.back().find("%")) &&
						(std::string::npos == pair.back().find("*")) )
					{
						isLocalUnit = true;
					}
				}
				else if (name == "z-index")
				{
					ZIndex = pair.back();
				}
				else if (name == "mso-width-relative")
				{
					WidthRelative = pair.back();
				}
				else if (name == "mso-height-relative")
				{
					HeightRelative = pair.back();
				}
				else if (name == "rotation")
				{
					rotation = pair.back();
				}
				// MSO-HW
				else if (name == "mso-width-percent")
					msoWidthPercent					=	pair.back();
				else if (name == "mso-height-percent")
					msoHeightPercent				=	pair.back();
				// MSO - POSITION
				else if (name == "mso-position-horizontal")
					msoPositionHorizontal			=	pair.back();
				else if (name == "mso-position-vertical")
					msoPositionVertical				=	pair.back();
				else if (name == "mso-position-horizontal-relative")
					msoPositionHorizontalRelative	=	pair.back();
				else if (name == "mso-position-vertical-relative")
					msoPositionVerticalRelative		=	pair.back();
				// TEXT
				else if (name == "v-text-anchor")
					textAnchor						=	pair.back();

				else if ("mso-wrap-distance-left" == name)
				{
					distanceLeftTop->X		=	pair.back();
				}
				else if ("mso-wrap-distance-top" == name)
				{
					distanceLeftTop->Y		=	pair.back();
				}
				else if ("mso-wrap-distance-right" == name)
				{
					distanceRightBottom->X	=	pair.back();
				}
				else if ("mso-wrap-distance-bottom" == name)
				{
					distanceRightBottom->Y	=	pair.back();
				}
				else if (name == "flip")
				{
					flip = pair.back();
				}
				else if (name == "left")
				{
					leftTop->X	=	pair.back();

					if ((std::string::npos == pair.back().find("pt")) &&
						(std::string::npos == pair.back().find("mm")) && 
						(std::string::npos == pair.back().find("cm")) && 
						(std::string::npos == pair.back().find("px")) &&
						(std::string::npos == pair.back().find("in")) &&
						(std::string::npos == pair.back().find("%")) &&
						(std::string::npos == pair.back().find("*")) )
					{
						isLocalUnit = true;
					}
				}
				else if (name == "top")
				{
					leftTop->Y	=	pair.back();

					if ((std::string::npos == pair.back().find("pt")) &&
						(std::string::npos == pair.back().find("mm")) && 
						(std::string::npos == pair.back().find("cm")) && 
						(std::string::npos == pair.back().find("px")) &&
						(std::string::npos == pair.back().find("in")) &&
						(std::string::npos == pair.back().find("%")) &&
						(std::string::npos == pair.back().find("*")) )
					{
						isLocalUnit = true;
					}
				}
			}
		}

	} // namespace Logic
} // namespace OOX