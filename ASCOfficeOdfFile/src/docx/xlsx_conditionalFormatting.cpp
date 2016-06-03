
#include "xlsx_conditionalFormatting.h"
#include "xlsx_utils.h"

#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../formulasconvert/formulasconvert.h"

namespace cpdoccore {
namespace oox {

	struct _cfvo
	{
		int						type;
		_CP_OPT(bool)			gte;
		_CP_OPT(std::wstring)	val;
		
		void serialize(std::wostream & _Wostream)
		{
			CP_XML_WRITER(_Wostream)
			{
				CP_XML_NODE(L"cfvo")
				{
					switch(type)
					{
						case 0: CP_XML_ATTR(L"type", L"percent");	break;
						case 1: CP_XML_ATTR(L"type", L"num");		break;
 						case 2: CP_XML_ATTR(L"type", L"max");		break;
  						case 3: CP_XML_ATTR(L"type", L"min");		break;
 						case 4: CP_XML_ATTR(L"type", L"max");		break; //todooo ext - autoMax
 						case 5: CP_XML_ATTR(L"type", L"min");		break;
					}
					if (val)	
						CP_XML_ATTR(L"val", *val);
				}
			}
		}
	};

	struct rule
    {
		int							type;
		_CP_OPT(int)				dxfId;
		_CP_OPT(bool)				percent;
		_CP_OPT(bool)				stopIfTrue;
		_CP_OPT(std::wstring)		operator_;
//expr		
		_CP_OPT(std::wstring)		formula;
//color scale icon set data_bar
		std::vector<_cfvo>			cfvo;
//color scale data_bar(1 element)
		std::vector<std::wstring>	color;
//data_bar	icon_set	
		_CP_OPT(bool)				showValue;
//data_bar
		_CP_OPT(int)				minLength;
		_CP_OPT(int)				maxLength;
//icon set
		_CP_OPT(bool)				reverse;
		_CP_OPT(bool)				iconset_percent;
		_CP_OPT(int)				iconset_type;
	};
	struct conditionalFormatting
    {
		std::wstring		ref;
		std::vector<rule>	rules;
    };

class xlsx_conditionalFormatting_context::Impl
{
public:

    void serialize(std::wostream & _Wostream)
    {
        if (!conditionalFormattings_.empty())
        {
			int priority = 1;
            CP_XML_WRITER(_Wostream)
            {
				for (int i = 0 ; i < conditionalFormattings_.size(); i++)
                {
					conditionalFormatting & c = conditionalFormattings_[i];
                    CP_XML_NODE(L"conditionalFormatting")
                    {
 						CP_XML_ATTR(L"sqref", c.ref);

						for (int j = 0 ; j < c.rules.size(); j++)
						{
							if (c.rules[j].type < 1 || c.rules[j].type > 4) continue;

							CP_XML_NODE(L"cfRule")
							{	
								CP_XML_ATTR(L"priority", priority++);
							
								if (c.rules[j].dxfId)		CP_XML_ATTR(L"dxfId", *c.rules[j].dxfId);
								if (c.rules[j].percent)		CP_XML_ATTR(L"percent", *c.rules[j].percent);
								if (c.rules[j].operator_)	CP_XML_ATTR(L"operator", *c.rules[j].operator_);
								if (c.rules[j].stopIfTrue)	CP_XML_ATTR(L"stopIfTrue", *c.rules[j].stopIfTrue);
								
								//CP_XML_ATTR(L"text"			, L"");
								//CP_XML_ATTR(L"rank"			, 0);
								//CP_XML_ATTR(L"bottom"			, 0);
								//CP_XML_ATTR(L"equalAverage"	, 0);
								//CP_XML_ATTR(L"aboveAverage"	, 0);							
								if (c.rules[j].type == 1)
								{
									CP_XML_ATTR(L"type", L"expression");									
									if (c.rules[j].formula)
									{
										CP_XML_NODE(L"formula")
										{
											CP_XML_CONTENT(*c.rules[j].formula);
										}
									}
								}
								else if (c.rules[j].type == 2)
								{
									CP_XML_ATTR(L"type", L"dataBar");
									CP_XML_NODE(L"dataBar")
									{
										if (c.rules[j].showValue)	CP_XML_ATTR(L"showValue", *c.rules[j].showValue);
										if (c.rules[j].minLength)	CP_XML_ATTR(L"minLength", *c.rules[j].minLength);
										if (c.rules[j].maxLength)	CP_XML_ATTR(L"maxLength", *c.rules[j].maxLength);

										for (int k = 0; k < c.rules[j].cfvo.size(); k++)
										{
											c.rules[j].cfvo[k].serialize(CP_XML_STREAM());
										}
										if (c.rules[j].color.size() > 0)
										{
											CP_XML_NODE(L"color")
											{
												CP_XML_ATTR(L"rgb", c.rules[j].color[0]);
											}
										}										
									}
								}
								else if (c.rules[j].type == 3)
								{
									CP_XML_ATTR(L"type", L"colorScale");
									CP_XML_NODE(L"colorScale")
									{
										for (int k = 0; k < c.rules[j].cfvo.size(); k++)
										{
											c.rules[j].cfvo[k].serialize(CP_XML_STREAM());
										}	
										for (int k = 0; k < c.rules[j].color.size(); k++)
										{
											CP_XML_NODE(L"color")
											{
												CP_XML_ATTR(L"rgb", c.rules[j].color[k]);
											}
										}	
									}
								}
								else if (c.rules[j].type == 4)
								{
									CP_XML_ATTR(L"type", L"iconSet");
									CP_XML_NODE(L"iconSet")
									{
										if (c.rules[j].showValue)	CP_XML_ATTR(L"showValue", *c.rules[j].showValue);
										
										for (int k = 0; k < c.rules[j].cfvo.size(); k++)
										{
											c.rules[j].cfvo[k].serialize(CP_XML_STREAM());
										}										
									}
								}
							}
						}
                    } 
                }        
            }
        }
    }

    std::vector<conditionalFormatting> conditionalFormattings_;
};

xlsx_conditionalFormatting_context::xlsx_conditionalFormatting_context() :
			impl_(new xlsx_conditionalFormatting_context::Impl())
{}

xlsx_conditionalFormatting_context::~xlsx_conditionalFormatting_context()
{
}
void xlsx_conditionalFormatting_context::serialize(std::wostream & _Wostream)
{
    return impl_->serialize(_Wostream);
}

void xlsx_conditionalFormatting_context::add(std::wstring ref)
{
	formulasconvert::odf2oox_converter converter;
	impl_->conditionalFormattings_.push_back(conditionalFormatting());
	
	ref = converter.convert_named_ref(ref);

//'Fitness Plan'!N20:'Fitness Plan'!P22; K25 -> N20:P22
	int pos_cells = ref.find(L":");
	std::wstring ref2;
	if (pos_cells >0)
	{
		ref2	= ref.substr(pos_cells + 1);
		ref		= ref.substr(0, pos_cells);
		pos_cells	= ref2.find(L"!");
		if (pos_cells > 0)
			ref2	= ref2.substr(pos_cells + 1);
	}
	pos_cells	= ref.find(L"!");
	if (pos_cells > 0)
		ref	= ref.substr(pos_cells + 1);

	if (!ref2.empty()) ref += L":" + ref2;

	impl_->conditionalFormattings_.back().ref = ref;
}

void xlsx_conditionalFormatting_context::add_rule(int type)
{
	impl_->conditionalFormattings_.back().rules.push_back(rule());
	impl_->conditionalFormattings_.back().rules.back().type = type;

}
void xlsx_conditionalFormatting_context::set_formula(std::wstring f)
{
	int pos = f.find(L"formula-is(");
	if ( pos >= 0 )
	{
		f = f.substr(11, f.size() - 12);
	}
	formulasconvert::odf2oox_converter converter;
	impl_->conditionalFormattings_.back().rules.back().formula = converter.convert_named_expr(f);
}
void xlsx_conditionalFormatting_context::set_dataBar(_CP_OPT(int) min, _CP_OPT(int) max)
{
	impl_->conditionalFormattings_.back().rules.back().minLength = min;
	impl_->conditionalFormattings_.back().rules.back().maxLength = max;
}
void xlsx_conditionalFormatting_context::set_dxf(int dxfId)
{
	impl_->conditionalFormattings_.back().rules.back().dxfId = dxfId;
}
void xlsx_conditionalFormatting_context::add_sfv(int type, std::wstring value)
{
	_cfvo cfvo;
	cfvo.type = type;
	if (!value.empty()) cfvo.val = value;

	impl_->conditionalFormattings_.back().rules.back().cfvo.push_back(cfvo);
}
void xlsx_conditionalFormatting_context::add_color(std::wstring col)
{
	impl_->conditionalFormattings_.back().rules.back().color.push_back(col);
}
void xlsx_conditionalFormatting_context::set_showVal(bool val)
{
	impl_->conditionalFormattings_.back().rules.back().showValue = val;
}


}
}
