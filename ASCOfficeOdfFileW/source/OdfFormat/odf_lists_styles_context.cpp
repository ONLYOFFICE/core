#include "precompiled_cpodf.h"

#include "logging.h"

#include "odf_lists_styles_context.h"

#include "styles_list.h"


namespace cpdoccore {
namespace odf {

odf_lists_styles_context::odf_lists_styles_context()
{
}
void odf_lists_styles_context::set_odf_context(odf_conversion_context * Context)
{
	odf_context_ = Context;
}

void odf_lists_styles_context::process_styles(office_element_ptr root )
{
	for (long i=0; i< lists_format_array_.size(); i++)
	{
		if (lists_format_array_[i].elements.size() < 1) continue;

		root->add_child_element(lists_format_array_[i].elements[0]);
	}
}	

void odf_lists_styles_context::add_style(int style_num, int based_num)
{
	//find index array for based_num

	std::wstring style_name;

	for (long i=0 ; i < lists_format_array_.size(); i++)
	{
		if (lists_format_array_[i].oox_based_number == based_num)
		{
			link_format_map_[based_num] = lists_format_array_[i].odf_list_style_name;
			break;
		}
	}
}
std::wstring odf_lists_styles_context::get_style_name(int oox_style_num)
{
    if (link_format_map_.count(oox_style_num) > 0)
	{
        return link_format_map_.at(oox_style_num);
	}
	else
	{
		return L"";
	}
}
void odf_lists_styles_context::start_style(int based_number)
{
	list_format_state state;

	office_element_ptr elm;	
	create_element(L"text", L"list-style", elm, odf_context_);
	state.elements.push_back(elm);
	
	state.oox_based_number = based_number;
	state.odf_list_style_name = std::wstring(L"WWNum") + boost::lexical_cast<std::wstring>(based_number + 1);

	text_list_style *style = dynamic_cast<text_list_style *>(elm.get());
	if (style == NULL)return;

	style->text_list_style_attr_.style_name_ = state.odf_list_style_name;

	lists_format_array_.push_back(state); //перенести в end??
}
void odf_lists_styles_context::start_style_level(int level, int type)
{
	if (lists_format_array_.size() < 1) return;
	int odf_type =-1;
	int format_type = -1;

	switch(type)
	{
		case 0: //numberformatAiueo                        : 
			break;
		case 1: //numberformatAiueoFullWidth               :
			break;
		case 2: //numberformatArabicAbjad                  : 
			break;
		case 3: //numberformatArabicAlpha                  : 
			break;
		case 4: //numberformatBahtText                     : 
			break;
		case 5: //numberformatBullet                       :
			odf_type = 2;
			break;
		case 6: //numberformatCardinalText                 : 
			break;
		case 7: //numberformatChicago                      : 
			break;
		case 8: //numberformatChineseCounting              : 
			break;
		case 9: //numberformatChineseCountingThousand      : 
			break;
		case 10: //numberformatChineseLegalSimplified       : 
			break;
		case 11: //numberformatChosung                      : 
			break;
		case 12: //numberformatCustom                       : 
			break;
		case 13: //numberformatDecimal                      : 
			break;
		case 14: //numberformatDecimalEnclosedCircle        : 
			break;
		case 15: //numberformatDecimalEnclosedCircleChinese : 
			break;
		case 16: //numberformatDecimalEnclosedFullstop      : 
			break;
		case 17: //numberformatDecimalEnclosedParen         : 
			break;
		case 18: //numberformatDecimalFullWidth             : 
			break;
		case 19: //numberformatDecimalFullWidth2            : 
			break;
		case 20: //numberformatDecimalHalfWidth             : 
			break;
		case 21: //numberformatDecimalZero                  : 
			break;
		case 22: //numberformatDollarText                   : 
			break;
		case 23: //numberformatGanada                       : 
			break;
		case 24: //numberformatHebrew1                      : 
			break;
		case 25: //numberformatHebrew2                      : 
			break;
		case 26: //numberformatHex                          : 
			break;
		case 27: //numberformatHindiConsonants              : 
			break;
		case 28: //numberformatHindiCounting                : 
			break;
		case 29: //numberformatHindiNumbers                 : 
			break;
		case 30: //numberformatHindiVowels                  :
			break;
		case 31: //numberformatIdeographDigital             : 
			break;
		case 32: //numberformatIdeographEnclosedCircle      : 
			break;
		case 33: //numberformatIdeographLegalTraditional    : 
			break;
		case 34: //numberformatIdeographTraditional         :
			break;
		case 35: //numberformatIdeographZodiac              : 
			break;
		case 36: //numberformatIdeographZodiacTraditional   : 
			break;
		case 37: //numberformatIroha                        : 
			break;
		case 38: //numberformatIrohaFullWidth               : 
			break;
		case 39: //numberformatJapaneseCounting             : 
			break;
		case 40: //numberformatJapaneseDigitalTenThousand   : 
			break;
		case 41: //numberformatJapaneseLegal                : 
			break;
		case 42: //numberformatKoreanCounting               : 
			break;
		case 43: //numberformatKoreanDigital                : 
			break;
		case 44: //numberformatKoreanDigital2               : 
			break;
		case 45: //numberformatKoreanLegal                  : 
			break;
		case 46: //numberformatLowerLetter                  : 
			break;
		case 47: //numberformatLowerRoman                   : 
			break;
		case 48: //numberformatNone                         : 
			break;
		case 49: //numberformatNumberInDash                 : 
			break;
		case 50: //numberformatOrdinal                      : 
			break;
		case 51: //numberformatOrdinalText                  : 
			break;
		case 52: //numberformatRussianLower                 : 
			break;
		case 53: //numberformatRussianUpper                 : 
			break;
		case 54: //numberformatTaiwaneseCounting            : 
			break;
		case 55: //numberformatTaiwaneseCountingThousand    : 
			break;
		case 56: //numberformatTaiwaneseDigital             : 
			break;
		case 57: //numberformatThaiCounting                 : 
			break;
		case 58: //numberformatThaiLetters                  : 
			break;
		case 59: //numberformatThaiNumbers                  : 
			break;
		case 60: //numberformatUpperLetter                  : 
			break;
		case 61: //numberformatUpperRoman                   : 
			break;
		case 62: //numberformatVietnameseCounting           : 
			break;
		default :
			odf_type = 1;
			break;
	}

	office_element_ptr elm;	
	if (odf_type == 2)
		create_element(L"text", L"list-level-style-bullet", elm, odf_context_);
	else
		create_element(L"text", L"list-level-style-number", elm, odf_context_);

	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(elm.get());
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(elm.get());

	if (style_number_)
	{
		style_number_->text_list_level_style_attr_.text_level_ = level;
		style_number_->text_list_level_style_number_attr_.text_display_levels_ = level;
		style_number_->text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_ = L"1";
	}

	if (style_bullet_)
	{
		style_bullet_->text_list_level_style_attr_.text_level_ = level;
	}
	lists_format_array_.back().elements.push_back(elm);

	lists_format_array_.back().elements[0]->add_child_element(elm);

}
void odf_lists_styles_context::end_style_level()
{
}
void odf_lists_styles_context::end_style()
{
}
}
}