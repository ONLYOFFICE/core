/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "logging.h"

#include "odf_lists_styles_context.h"

#include "styles_list.h"
#include "style_text_properties.h"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
 
namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer {

odf_lists_styles_context::odf_lists_styles_context()
{
}
void odf_lists_styles_context::set_odf_context(odf_conversion_context * Context)
{
	odf_context_ = Context;
}

void odf_lists_styles_context::process_styles(office_element_ptr root, bool automatic)
{
	for (size_t i = 0; i < lists_format_array_.size(); i++)
	{
		if (lists_format_array_[i].elements.size() < 1)		continue;
		if (lists_format_array_[i].automatic != automatic)	continue;

		root->add_child_element(lists_format_array_[i].elements[0]);
	}
}	

std::wstring odf_lists_styles_context::get_style_name(int oox_style_num)
{
	if (oox_style_num < 0)
	{
		if (false == lists_format_array_.empty())
			return lists_format_array_.back().odf_list_style_name;
		else
			return L"";
	}
	else
	{
		std::map<int, size_t>::iterator pFind = lists_format_map_.find(oox_style_num);
		
		if (pFind != lists_format_map_.end())
			return lists_format_array_[pFind->second].odf_list_style_name;
		else
			return L"";
	}
}
void odf_lists_styles_context::start_style(bool bMaster, int based_number)
{
	list_format_state state;

	office_element_ptr elm;	
	create_element(L"text", L"list-style", elm, odf_context_);
	state.elements.push_back(elm);
	
	if (based_number < 0) 
	{
		state.oox_based_number		= lists_format_array_.size();
		state.odf_list_style_name	= std::wstring(L"L") + boost::lexical_cast<std::wstring>(state.oox_based_number + 1);
	}
	else
	{
		state.oox_based_number		= based_number;
		state.odf_list_style_name	= std::wstring(L"WWNum") + boost::lexical_cast<std::wstring>(state.oox_based_number);
	}

	lists_format_map_.insert(std::make_pair(state.oox_based_number, lists_format_array_.size()));

	state.automatic	= !bMaster; 

	text_list_style *style = dynamic_cast<text_list_style *>(elm.get());
	if (style == NULL)return;

	style->text_list_style_attr_.style_name_ = state.odf_list_style_name;
	lists_format_array_.push_back(state); //перенести в end??
}

style_list_level_properties * odf_lists_styles_context::get_list_level_properties()
{
	if (lists_format_array_.empty()) return NULL;
	if (lists_format_array_.back().elements.empty()) return NULL;
	
	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_image  *style_image_  = dynamic_cast<text_list_level_style_image  *>(lists_format_array_.back().elements.back().get());

	style_list_level_properties *props = NULL;
	if (style_number_)
	{
 		if (!style_number_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties",style_number_->style_list_level_properties_,odf_context_);

		props = dynamic_cast<style_list_level_properties *>(style_number_->style_list_level_properties_.get());    
	}
	if (style_bullet_)
	{
 		if (!style_bullet_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties",style_bullet_->style_list_level_properties_,odf_context_);

		props =  dynamic_cast<style_list_level_properties *>(style_bullet_->style_list_level_properties_.get());    
	}
	if (style_image_)
	{
 		if (!style_image_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties",style_image_->style_list_level_properties_,odf_context_);

		props =  dynamic_cast<style_list_level_properties *>(style_image_->style_list_level_properties_.get());    
	}

	return props;
}
style_text_properties * odf_lists_styles_context::get_text_properties()
{
	if (lists_format_array_.empty()) return NULL;
	if (lists_format_array_.back().elements.empty()) return NULL;
	
	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_image  *style_image_  = dynamic_cast<text_list_level_style_image  *>(lists_format_array_.back().elements.back().get());

	style_text_properties *props = NULL;
	if (style_number_)
	{
 		if (!style_number_->style_text_properties_)
			create_element(L"style", L"text-properties", style_number_->style_text_properties_, odf_context_);

		props = dynamic_cast<style_text_properties *>(style_number_->style_text_properties_.get());    
	}
	if (style_bullet_)
	{
 		if (!style_bullet_->style_text_properties_)
			create_element(L"style", L"text-properties", style_bullet_->style_text_properties_, odf_context_);

		props =  dynamic_cast<style_text_properties *>(style_bullet_->style_text_properties_.get());    
	}
	if (style_image_)
	{
 		if (!style_image_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties",style_image_->style_list_level_properties_,odf_context_);

		props =  dynamic_cast<style_text_properties *>(style_image_->style_text_properties_.get());    
	}
	return props;
}

style_list_level_label_alignment * odf_lists_styles_context::get_list_level_alignment_properties()
{
	if (lists_format_array_.empty()) return NULL;
	if (lists_format_array_.back().elements.empty()) return NULL;
	
	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_image  *style_image_  = dynamic_cast<text_list_level_style_image  *>(lists_format_array_.back().elements.back().get());

	style_list_level_properties *props = NULL;
	if (style_number_)
	{
 		if (!style_number_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties", style_number_->style_list_level_properties_, odf_context_);

		props = dynamic_cast<style_list_level_properties *>(style_number_->style_list_level_properties_.get());    
	}
	if (style_bullet_)
	{
 		if (!style_bullet_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties", style_bullet_->style_list_level_properties_, odf_context_);

		props =  dynamic_cast<style_list_level_properties *>(style_bullet_->style_list_level_properties_.get());    
	}
	if (style_image_)
	{
 		if (!style_image_->style_list_level_properties_)
			create_element(L"style", L"list-level-properties", style_image_->style_list_level_properties_, odf_context_);

		props =  dynamic_cast<style_list_level_properties *>(style_image_->style_list_level_properties_.get());    
	}
	if (!props)return NULL;

	if (!props->style_list_level_label_alignment_)
	{
		create_element(L"style", L"list-level-label-alignment" , props->style_list_level_label_alignment_, odf_context_);
	}
	return dynamic_cast<style_list_level_label_alignment *>(props->style_list_level_label_alignment_.get());
}

int odf_lists_styles_context::start_style_level(int level, int type)
{
	if (lists_format_array_.empty()) return -1;
	
	int odf_type = 1;
	int format_type = -1;

	style_numformat num_format;
	bool sync_letter = false;
	switch(type)
	{
		case 0: //numberformatAiueo                        : 
		case 1: //numberformatAiueoFullWidth               :
			num_format = style_numformat(style_numformat::aiueo);
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
			num_format = style_numformat(style_numformat::chineseCounting);
			break;
		case 9: //numberformatChineseCountingThousand      : 
			break;
		case 10: //numberformatChineseLegalSimplified       :
			num_format = style_numformat(style_numformat::chineseLegal);
			break;
		case 11: //numberformatChosung                      : 
			break;
		case 12: //numberformatCustom                       : 
			break;
		case 13: //numberformatDecimal                      : 
			num_format = style_numformat(style_numformat::arabic);
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
			//01 02 03 ...
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
			num_format = style_numformat(style_numformat::ideographLegal);
			break;
		case 34: //numberformatIdeographTraditional         :
			num_format = style_numformat(style_numformat::ideographTraditional);
			break;
		case 35: //numberformatIdeographZodiac              : 
			num_format = style_numformat(style_numformat::ideographZodiac);
			break;
		case 36: //numberformatIdeographZodiacTraditional   : 
			num_format = style_numformat(style_numformat::ideographZodiacTraditional);
			break;
		case 37: //numberformatIroha                        : 
			num_format = style_numformat(style_numformat::iroha);
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
		case 43: //numberformatKoreanDigital                : 
			num_format = style_numformat(style_numformat::koreanDigital);
			break;
		case 44: //numberformatKoreanDigital2               : 
			break;
		case 45: //numberformatKoreanLegal                  : 
			break;
		case 46: //numberformatLowerLetter 
			num_format = style_numformat(style_numformat::alphaLc);
			sync_letter = true; 
			break;
		case 47: //numberformatLowerRoman                   : 
			num_format = style_numformat(style_numformat::romanLc);
			break;
		case 48: //numberformatNone                         : 
			//num_format = L"";
			break;
		case 49: //numberformatNumberInDash                 : //??
			//suffix -
			//preffix -
			break;
		case 50: //numberformatOrdinal                      : 
			break;
		case 51: //numberformatOrdinalText                  : 
			break;
		case 52: //numberformatRussianLower                 : 
			num_format = style_numformat(style_numformat::russianLo);
			sync_letter = true; 
			break;
		case 53: //numberformatRussianUpper                 : 
			num_format = style_numformat(style_numformat::russianUp);
			sync_letter = true; 
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
			num_format = style_numformat(style_numformat::alphaUc);
			sync_letter = true; 
			break;
		case 61: //numberformatUpperRoman                   : 
			num_format = style_numformat(style_numformat::romanUc);
			break;
		case 62: //numberformatVietnameseCounting           : 
			break;
		case 1000:
			odf_type = 3;
			break;
		default :
			odf_type = 1;
			break;
	}

	office_element_ptr elm;	
	
	if (odf_type == 2)
		create_element(L"text", L"list-level-style-bullet", elm, odf_context_);
	else if (odf_type == 3)
		create_element(L"text", L"list-level-style-image", elm, odf_context_);
	else
		create_element(L"text", L"list-level-style-number", elm, odf_context_);

	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(elm.get());
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(elm.get());
	text_list_level_style_image  *style_image_  = dynamic_cast<text_list_level_style_image  *>(elm.get());

	if (style_number_)
	{
		style_number_->text_list_level_style_attr_.text_level_ = level + 1;
		style_number_->text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_ = num_format;

		if (sync_letter)style_number_->text_list_level_style_number_attr_.common_num_format_attlist_.style_num_letter_sync_ = true;
	}

	if (style_image_)
	{
		style_image_->text_list_level_style_attr_.text_level_ = level + 1;
	}
	if (style_bullet_)
	{
		style_bullet_->text_list_level_style_attr_.text_level_ = level + 1;
	}
	lists_format_array_.back().elements.push_back(elm);

	lists_format_array_.back().elements[0]->add_child_element(elm);
	
	return odf_type;
}
wchar_t convert_bullet_char(wchar_t c)
{
    wchar_t res = c;
    switch(c)
    {
    case L'\xf075': res = L'\xE00C';; break; 
    //case L'\xf0A7': res = L'\xE00A'; break; 
    case L'\xf0FE': res = L'\x2611'; break; 
    case L'\xf0B7': res = L'\x25CF'; break; 
    case L'\xf0D8': res = L'\x27A2'; break; 
    case L'\xf0FC': res = L'\x2714'; break; 
    case L'\xf06F': res = L'\x25CB'; break; 
    case L'\xf0DA': res = L'\x2794'; break; 
    case L'\xf04F': res = L'\x2717'; break; 
    default:break;
    }

    return res;
}
void odf_lists_styles_context::set_numeric_format(std::wstring val)
{
	if ( lists_format_array_.empty() ) return;
	if ( lists_format_array_.back().elements.empty() ) return;

	if ( val.empty() ) return;

	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(lists_format_array_.back().elements.back().get());
	if (style_number_)
	{
		//style_number_->text_list_level_style_number_attr_.common_num_format_attlist_.style_num_format_ = val;

		std::wstring s = val;

		int r1 = s.find(L"%");
		if (r1 >0)
		{
			std::wstring prefix = s.substr(0, r1);
			style_number_->text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_prefix_ = prefix;

			s = s.substr(r1, s.length() - r1);
			r1 = 0;
		}
		if (r1 < 0) return;//??

        r1 = (int)s.find(L"%", 1);
		if (r1 >0)
		{
			int level = 0;
			//find count % in s
			int r2=0;
			while (r2 < s.length())
			{
                r2 = (int)s.find(L"%", r2);
				if (r2 >=0) level++;
				else break;
				r2++;
			}
			style_number_->text_list_level_style_number_attr_.text_display_levels_ = level;

			s = s.substr(0,r1);
		}
		
		boost::wregex r(L"(\\%[0-9]+)(\\S+)");
		boost::match_results<std::wstring::const_iterator> res;
		if ( boost::regex_match(s, res, r))
		{
			std::wstring suffix = res[2].str();
			if (!suffix.empty()) 
				style_number_->text_list_level_style_number_attr_.common_num_format_prefix_suffix_attlist_.style_num_suffix_ = suffix;
		
			std::wstring numb = res[2].str();
			//if (!numb.empty())
			//{
			//	int format = 0;
			//	try
			//	{
			//		format = boost::lexical_cast<int>(numb.substr(1, numb.length() - 1));
			//	}catch(...)
			//	{
			//	}
			//	if (format > 0)
			//	{
			//		//style_number_->text_list_level_style_number_attr_.common_num_format_attlist_.text_display_levels_ = display_level;
			//	}
			//}

		}

	}
}
void odf_lists_styles_context::set_bullet_char(std::wstring val)
{
	if ( lists_format_array_.empty() ) return;
	if ( lists_format_array_.back().elements.empty() ) return ;
	
	if ( val.empty() ) return;
	
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(lists_format_array_.back().elements.back().get());

	if (style_bullet_ == NULL) return;
	
	wchar_t char_ = convert_bullet_char(val.at(0));
	
	style_bullet_->text_list_level_style_bullet_attr_.text_bullet_char_ = val;//char_;
}
void odf_lists_styles_context::set_bullet_image_size(double size)
{
	if (size < 0.1) return;
	if (lists_format_array_.empty()) return;
	if (lists_format_array_.back().elements.empty()) return ;

	style_list_level_properties *props = get_list_level_properties();

	if(!props) return;
	
	props->fo_width_  = length(length(size, length::pt).get_value_unit(length::cm), length::cm);
	props->fo_height_ = length(length(size, length::pt).get_value_unit(length::cm), length::cm);
}
void odf_lists_styles_context::set_bullet_image	(std::wstring ref)
{
	if (lists_format_array_.empty()) return;
	if (lists_format_array_.back().elements.empty()) return ;
	
	text_list_level_style_image *style_image_ = dynamic_cast<text_list_level_style_image *>(lists_format_array_.back().elements.back().get());

	if (style_image_ == NULL) return;

	style_image_->text_list_level_style_image_attr_.common_xlink_attlist_.href_ = ref;
	
	style_image_->text_list_level_style_image_attr_.common_xlink_attlist_.type_		= xlink_type::Simple;
	style_image_->text_list_level_style_image_attr_.common_xlink_attlist_.show_		= xlink_show::Embed;
	style_image_->text_list_level_style_image_attr_.common_xlink_attlist_.actuate_	= xlink_actuate::OnLoad;
}
void odf_lists_styles_context::set_restart_number(int val)
{
	if (lists_format_array_.empty()) return;

	lists_format_array_.back().restart_number = val;
}
void odf_lists_styles_context::set_start_number(int val)
{
	if (lists_format_array_.empty()) return;
	if (lists_format_array_.back().elements.empty()) return ;

	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(lists_format_array_.back().elements.back().get());

	if (style_number_ == NULL) return;

	style_number_->text_list_level_style_number_attr_.text_start_value_ = val;
}
void odf_lists_styles_context::set_text_style_name(std::wstring name)
{
	if (lists_format_array_.empty()) return;
	if (lists_format_array_.back().elements.empty()) return ;
	
	text_list_level_style_number *style_number_ = dynamic_cast<text_list_level_style_number *>(lists_format_array_.back().elements.back().get());
	text_list_level_style_bullet *style_bullet_ = dynamic_cast<text_list_level_style_bullet *>(lists_format_array_.back().elements.back().get());

	if (style_number_) 	style_number_->text_list_level_style_number_attr_.text_style_name_ = name;
	if (style_bullet_) 	style_bullet_->text_list_level_style_bullet_attr_.text_style_name_ = name;

}
void odf_lists_styles_context::end_style_level()
{
}
void odf_lists_styles_context::end_style()
{
}
}
}
