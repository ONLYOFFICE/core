/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "STYLES.h"
#include <Logic/Biff_records/Style.h>
#include <Logic/Biff_records/StyleExt.h>

namespace XLS
{

class Parenthesis_STYLES_1 : public CompositeObject //: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_STYLES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_STYLES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{	
		if(proc.mandatory<Style>())
		{
			m_Style = elements_.back();
			elements_.pop_back();
		}else return false;

	
		if (proc.optional<StyleExt>())
		{
			m_StyleEx = elements_.back();
			elements_.pop_back();
		}
		return true;
	};

	BaseObjectPtr m_StyleEx;
	BaseObjectPtr m_Style;

};

BaseObjectPtr STYLES::clone()
{
	return BaseObjectPtr(new STYLES(*this));
}


// STYLES = 1*(Style [StyleExt])
const bool STYLES::loadContent(BinProcessor& proc)
{
	styles_count = proc.repeated<Parenthesis_STYLES_1>(0, 0);
	
	return true;
}

int STYLES::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cellStyles")
		{
			if (styles_count > 0)
			{
				CP_XML_ATTR(L"count", styles_count);

				for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); it++)
				{
					Parenthesis_STYLES_1 * style_1 = dynamic_cast<Parenthesis_STYLES_1*>(it->get());

					if (style_1)
					{
						CP_XML_NODE(L"cellStyle")
						{
							XLS::Style		* style		= dynamic_cast<Style*>	(style_1->m_Style.get());
							XLS::StyleExt	* styleExt	= dynamic_cast<StyleExt*>(style_1->m_StyleEx.get());
							
							if (styleExt)
							{
								CP_XML_ATTR(L"name", styleExt->stName.value());
							
								if (styleExt->fBuiltIn)
								{
									CP_XML_ATTR(L"builtinId", styleExt->builtInData.istyBuiltIn);
									
									if (styleExt->builtInData.iLevel > 0 && styleExt->builtInData.iLevel < 255)
									{
										CP_XML_ATTR(L"iLevel", styleExt->builtInData.iLevel);
									}
								}

        //                        for (int i = 0; i < styleExt->xfProps.xfPropArray.size(); i++)
								//{
								//}
							}
							else
							{
								CP_XML_ATTR(L"name", style->user.value());
									
								if (style->fBuiltIn)
								{
									CP_XML_ATTR(L"builtinId", style->builtInData.istyBuiltIn);
									if (style->builtInData.iLevel > 0 && style->builtInData.iLevel < 255)
									{
										CP_XML_ATTR(L"iLevel", style->builtInData.iLevel);
									}
								}
							}
							int xfId = style->ixfe - 1;
							if (xfId < 0) xfId = 0;
								
							CP_XML_ATTR(L"xfId", xfId);
						}
					}
				}
			}
			else
			{
				CP_XML_ATTR(L"count",1);
				CP_XML_NODE(L"cellStyle")
				{
					CP_XML_ATTR(L"xfId", 0);
					CP_XML_ATTR(L"builtinId", 0);
                    CP_XML_ATTR(L"name", L"Common");
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

