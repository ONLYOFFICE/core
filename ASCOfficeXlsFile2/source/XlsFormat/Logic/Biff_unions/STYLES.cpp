
#include "STYLES.h"
#include <Logic/Biff_records/Style.h>
#include <Logic/Biff_records/StyleExt.h>

#include <simple_xml_writer.h>

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
							
								if ((styleExt->fBuiltIn.value()) && (*styleExt->fBuiltIn.value()))
								{
									CP_XML_ATTR(L"builtinId", styleExt->builtInData.istyBuiltIn);
									if (styleExt->builtInData.iLevel > 0 && styleExt->builtInData.iLevel < 255)
									{
										CP_XML_ATTR(L"iLevel", styleExt->builtInData.iLevel);
									}
								}

								for (long i = 0; i < styleExt->xfProps.xfPropArray.size(); i++)
								{
								}
							}
							else
							{
								CP_XML_ATTR(L"name", style->user.value());
									
								if ((style->fBuiltIn.value()) && (*style->fBuiltIn.value()))
								{
									CP_XML_ATTR(L"builtinId", style->builtInData.istyBuiltIn);
									if (style->builtInData.iLevel > 0 && style->builtInData.iLevel < 255)
									{
										CP_XML_ATTR(L"iLevel", style->builtInData.iLevel);
									}
								}
							}

							if (style->ixfe.value())
							{
								int xfId = *style->ixfe.value() - 1;
								if (xfId < 0) xfId = 0;
								
								CP_XML_ATTR(L"xfId", xfId);
							}
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

