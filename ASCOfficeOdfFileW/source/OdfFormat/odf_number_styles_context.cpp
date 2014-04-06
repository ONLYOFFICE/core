#include "precompiled_cpodf.h"

#include "logging.h"

#include "odf_number_styles_context.h"
#include "ods_conversion_context.h"

#include "number_style.h"


namespace cpdoccore {
namespace odf {


odf_number_styles_context::odf_number_styles_context(odf_conversion_context & Context): context_(Context)
{
}

void odf_number_styles_context::create(int oox_num_fmt, std::wstring formatCode)
{
	if (oox_num_fmt <164)return create_default(oox_num_fmt,formatCode);

	number_format_state state;

	state.oox_num_fmt = oox_num_fmt;
	state.style_name = std::wstring(L"NF1000") + boost::lexical_cast<std::wstring>( number_format_array_.size()+1);
	state.ods_type = office_value_type::Custom;
//////////////////////////////

	boost::algorithm::split(state.format_code, formatCode, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

	if (state.format_code.size()>1 && state.format_code[state.format_code.size()-1].find(L"@")>=0)
	{
		state.format_code.pop_back();
	}

	number_format_array_.push_back(state);
	named_link_map_[oox_num_fmt] = number_format_array_.size()-1;

	detect_format(number_format_array_.back());
}


void odf_number_styles_context::create_default(int oox_num_fmt, std::wstring formatCode)
{
	number_format_state state;
	
	state.oox_num_fmt = oox_num_fmt;
	state.ods_type = office_value_type::Custom;
	state.style_name = std::wstring(L"NF1000") + boost::lexical_cast<std::wstring>( number_format_array_.size()+1);

	switch (oox_num_fmt)
	{
	case 1:		formatCode = L"0";					state.ods_type =office_value_type::Float; break;
	case 2:		formatCode = L"0.00";				state.ods_type =office_value_type::Float; break;
	case 3:		formatCode = L"#,##0";				state.ods_type =office_value_type::Float; break;
	case 4:		formatCode = L"#,##0.00";			state.ods_type =office_value_type::Float; break;
	
	case 9:		formatCode = L"0%";					state.ods_type =office_value_type::Percentage; break;
	case 10:	formatCode = L"0.00%";				state.ods_type =office_value_type::Percentage; break;
	
	case 11:	formatCode = L"0.00E+00";			state.ods_type =office_value_type::Float; break;
	case 12:	formatCode = L"# ?/?";				state.ods_type =office_value_type::Float; break;
	case 13:	formatCode = L"# ??/??";			state.ods_type =office_value_type::Float; break;
	
	case 14:	formatCode = L"mm-dd-yy";			state.ods_type =office_value_type::Date; break;
	case 15:	formatCode = L"d-mmm-yy";			state.ods_type =office_value_type::Date; break;
	case 16:	formatCode = L"d-mmm";				state.ods_type =office_value_type::Date; break;
	case 17:	formatCode = L"mmm-yy";				state.ods_type =office_value_type::Date; break;

	case 18:	formatCode = L"h:mm AM/PM";			state.ods_type =office_value_type::Time; break;
	case 19:	formatCode = L"h:mm:ss AM/PM";		state.ods_type =office_value_type::Time; break;
	case 20:	formatCode = L"h:mm";				state.ods_type =office_value_type::Time; break;
	case 21:	formatCode = L"h:mm:ss";			state.ods_type =office_value_type::Time; break;
	case 22:	formatCode = L"m/d/yy h:mm";		state.ods_type =office_value_type::Time; break;

	case 37:	formatCode = L"#,##0 ;(#,##0)";		state.ods_type =office_value_type::Float; break;
	case 38:	formatCode = L"#,##0 ;[Red](#,##0)"; state.ods_type =office_value_type::Float; break;
	case 39:	formatCode = L"#,##0.00;(#,##0.00)"; state.ods_type =office_value_type::Float; break;
	case 40:	formatCode = L"#,##0.00;[Red](#,##0.00)"; state.ods_type =office_value_type::Float; break;


	case 45:	formatCode = L"mm:ss";			state.ods_type =office_value_type::Time; break;
	case 46:	formatCode = L"[h]:mm:ss";		state.ods_type =office_value_type::Time; break;
	case 47:	formatCode = L"mmss.0";			state.ods_type =office_value_type::Time; break;

	case 49:	formatCode = L"@";		state.ods_type =office_value_type::String; break;

	default:
/////////////////////////////////// с неопределенным formatCode .. он задается в файле
		if (oox_num_fmt >=5  && oox_num_fmt <=8)state.ods_type =office_value_type::Currency; 
		if (oox_num_fmt >=43 && oox_num_fmt <=44)state.ods_type =office_value_type::Currency; 
		
		if (oox_num_fmt >=27 && oox_num_fmt <=31)state.ods_type =office_value_type::Date; 
		if (oox_num_fmt >=50 && oox_num_fmt <=54)state.ods_type =office_value_type::Date; 
		if (oox_num_fmt >=57 && oox_num_fmt <=58)state.ods_type =office_value_type::Date; 
		if (oox_num_fmt ==36)state.ods_type =office_value_type::Date; 

		if (oox_num_fmt >=32 && oox_num_fmt <=35)state.ods_type =office_value_type::Time; 
		if (oox_num_fmt >=55 && oox_num_fmt <=56)state.ods_type =office_value_type::Time; 

		if (oox_num_fmt >=60 && oox_num_fmt <=62)state.ods_type =office_value_type::Float; 
		if (oox_num_fmt >=69 && oox_num_fmt <=70)state.ods_type =office_value_type::Float; 

		if (oox_num_fmt >=67 && oox_num_fmt <=68)state.ods_type =office_value_type::Percentage; 

		if (oox_num_fmt >=71 && oox_num_fmt <=74)state.ods_type =office_value_type::Date; 
		if (oox_num_fmt >=75 && oox_num_fmt <=80)state.ods_type =office_value_type::Time; 
		if (oox_num_fmt ==81)state.ods_type =office_value_type::Date; 
	}
    boost::algorithm::split(state.format_code, formatCode, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

	if (state.format_code.size()>1 && state.format_code[state.format_code.size()-1].find(L"@")>=0)
	{
		state.format_code.pop_back();
	}
////////////////////////////////////////////

	number_format_array_.push_back(state);
	named_link_map_[oox_num_fmt] = number_format_array_.size()-1;

	detect_format(number_format_array_.back());

}
number_format_state & odf_number_styles_context::add_or_find(int oox_num_fmt, std::wstring formatCode)
{
    if (named_link_map_.count(oox_num_fmt) > 0)
	{
        return number_format_array_[named_link_map_.at(oox_num_fmt)];
	}
	else
	{
		create(oox_num_fmt,formatCode);
		return number_format_array_.back();
	}
}

void odf_number_styles_context::process_styles(office_element_ptr root )
{
	for (long i=0; i< number_format_array_.size(); i++)
	{
		create_style(number_format_array_[i]);

		root->add_child_element(styles_elments.back());
	}
}	

void odf_number_styles_context::create_style(number_format_state & state)
{
	office_element_ptr elm;

	switch(state.ods_type)
	{
		case office_value_type::Float:		create_number_style		(state, elm);	break;
		case office_value_type::Currency:	create_currency_style	(state, elm);	break;
		case office_value_type::Percentage:	create_percentage_style	(state, elm);	break;
		case office_value_type::Date:		create_date_style		(state, elm);	break;
		case office_value_type::Time:		create_time_style		(state, elm);	break;
		case office_value_type::Boolean:	create_boolean_style	(state, elm);	break;
		case office_value_type::String:		create_text_style		(state, elm);	break;
	}

	number_style_base* number_style_ = dynamic_cast<number_style_base*>(elm.get());
	if (number_style_)
	{
		number_style_->set_style_name(state.style_name);
	}
	
	styles_elments.push_back(elm);		
}

void odf_number_styles_context::create_number_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"number-style", root_elm, &context_);

	if (state.format_code.size()>0)
	{
		office_element_ptr elm;
	
		create_numbers(state, elm);
		root_elm->add_child_element(elm); 
	}
}
void odf_number_styles_context::create_numbers(number_format_state & state, office_element_ptr & elm)
{
	optional< int>::Type min_digit, min_decimal;

	create_element(L"number", L"number", elm, &context_);
	styles_elments.push_back(elm);
		
	number_number* number_number_ = dynamic_cast<number_number*>(elm.get());

	if (number_number_)
	{
		if (state.format_code[0].length()>0)
		{
			std::wstring str1,str2;
			boost::wregex re1(L"([^0-9.,]+)");
			boost::wsmatch result;
			boost::wregex re2(L"([^#.,]+)");
			str1 = boost::regex_replace(state.format_code[0], re1, L"",boost::match_default | boost::format_all);
			str2 = boost::regex_replace(state.format_code[0], re2, L"",boost::match_default | boost::format_all);

			if (str1.length()<str2.length())str1=str2;

			std::vector<std::wstring> numbers;

			boost::algorithm::split(numbers, str1, boost::algorithm::is_any_of(L".,"), boost::algorithm::token_compress_on);
			int ind=1;//
			for (long i=0;i<numbers.size();i++)
			{
				if (numbers[i].length()<1)continue;
				if (ind==1)min_digit= numbers[i].length();
				if (ind==2)min_decimal= numbers[i].length();
				ind++;
			}
		}
		else
		{
			//формат не определен .. дефолтный
			min_digit =1;
		}

		number_number_->number_min_integer_digits_= min_digit;
		number_number_->number_decimal_places_= min_decimal;
		number_number_->number_grouping_ = true;
	}
}
void odf_number_styles_context::create_percentage_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"percentage-style", root_elm, &context_);
	
	office_element_ptr elm;

	create_numbers(state, elm);
	root_elm->add_child_element(elm); 

	create_element(L"number", L"text", elm, &context_);
	number_text* number_text_ = dynamic_cast<number_text*>(elm.get());
	if (number_text_)number_text_->add_text(L"%"); 	

	root_elm->add_child_element(elm);
	styles_elments.push_back(elm);
}

void odf_number_styles_context::create_currency_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"currency-style", root_elm, &context_);
	{
//1 ??? - определить где ставить значок todooo
		office_element_ptr elm;
		create_element(L"number", L"currency-symbol", elm, &context_);
		styles_elments.push_back(elm);

		number_currency_symbol* number_currency_symbol_ = dynamic_cast<number_currency_symbol*>(elm.get());
		if (number_currency_symbol_)
		{
			number_currency_symbol_->add_text(state.currency_str);
		}
		root_elm->add_child_element(elm);
//2 ???
		create_numbers(state, elm);
		root_elm->add_child_element(elm);  
	}
}

void odf_number_styles_context::create_date_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"date-style", root_elm, &context_);

	//state.language_code == L"F800" System long date format
	
	std::wstring s = state.format_code[0];
	boost::algorithm::to_lower(s);
	
	boost::wregex re(L"([a-zA-Z]+)(\\W+)");//(L"(\\w+)");
	std::list<std::wstring> result;
	bool b = boost::regex_split(std::back_inserter(result),s, re);
	if (b)result.push_back(s);//последний ..выносится - так уж работает boost.regex_split
	int res;
	int sz=0;
	for (std::list<std::wstring>::iterator i=result.begin(); i!=result.end(); ++i)
	{
		office_element_ptr elm;
		s = *i;
		sz = s.length();
		if ((res=s.find(L"m")) >=0) 
		{
			create_element(L"number", L"month", elm, &context_);
			number_month* number_month_ = dynamic_cast<number_month*>(elm.get());
		
			if (number_month_ && sz > 2)number_month_->number_textual_ = true;
			if (sz == 1 || sz == 3) number_month_->number_style_ = L"short";
			if (sz == 2 || sz == 4) number_month_->number_style_ = L"long";
		}
		else if ((res=s.find(L"d")) >=0) 
		{
			if (sz < 3)
			{
				create_element(L"number", L"day", elm, &context_);
				number_day* number_day_ = dynamic_cast<number_day*>(elm.get());
				if (number_day_)
				{
					if (sz == 1) number_day_->number_style_ = L"short";
					if (sz == 2) number_day_->number_style_ = L"long";
				}
			}
			else
			{
				create_element(L"number", L"day-of-week", elm, &context_);
				number_day_of_week* number_day_of_week_ = dynamic_cast<number_day_of_week*>(elm.get());
				if (number_day_of_week_)
				{
					if (sz == 3) number_day_of_week_->number_style_ = L"short";
					if (sz == 4) number_day_of_week_->number_style_ = L"long";
				}
			}
		}
		else if ((res=s.find(L"y")) >=0) 
		{
			create_element(L"number", L"year", elm, &context_);
			number_year* number_year_ = dynamic_cast<number_year*>(elm.get());
			if (number_year_)
			{
				if (sz<3)	number_year_->number_style_ = L"short";
				else		number_year_->number_style_ = L"long";
			}
		}
		else
		{	//////////////////// делитель ////////////////////	
			if(sz>1) 
			{
				//выкинем "лишние" слэши
				boost::algorithm::replace_all(s, L"\\", L"");
			}
			create_element(L"number", L"text", elm, &context_);
			number_text* number_text_ = dynamic_cast<number_text*>(elm.get());
			if (number_text_)number_text_->add_text(s); 	
		}
		if (elm)
		{
			root_elm->add_child_element(elm);
			styles_elments.push_back(elm);

		}
	}
}

void odf_number_styles_context::create_time_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"time-style", root_elm, &context_);
	
	// state.language_code == L"F400")//System time format
	std::wstring s = state.format_code[0];
	boost::algorithm::to_lower(s);
	
	boost::wregex re(L"([a-zA-Z]+)(\\W+)");//(L"(\\w+)");
	std::list<std::wstring> result;
	bool b = boost::regex_split(std::back_inserter(result),s, re);
	if (b)result.push_back(s);//последний ..выносится - так уж работает boost.regex_split

	int res;
	int sz=0;
	for (std::list<std::wstring>::iterator i=result.begin(); i!=result.end(); ++i)
	{
		office_element_ptr elm;
		s = *i;
		sz = s.length();
		if ((res=s.find(L"h")) >=0) 
		{
			create_element(L"number", L"hours", elm, &context_);
			number_hours* number_hours_ = dynamic_cast<number_hours*>(elm.get());		
			if (number_hours_)
			{
				if (sz == 1)number_hours_->number_style_ = L"short";
				if (sz == 2)number_hours_->number_style_ = L"long";
			}
		}
		else if ((res=s.find(L"am")) >=0/* || (res=s.find(L"pm")) >=0*/)  //излишнее .. 
		{
			create_element(L"number", L"am-pm", elm, &context_);
		}
		else if ((res=s.find(L"m")) >=0 && (res=s.find(L"am")) <0 && (res=s.find(L"pm")) <0 ) 
		{
			create_element(L"number", L"minutes", elm, &context_);
			number_minutes* number_minutes_ = dynamic_cast<number_minutes*>(elm.get());
			if (number_minutes_)
			{
				if (sz == 1)number_minutes_->number_style_ = L"short";
				if (sz == 2)number_minutes_->number_style_ = L"long";
			}
		}
		else if ((res=s.find(L"s")) >=0) 
		{
			create_element(L"number", L"seconds", elm, &context_);
			number_seconds* number_seconds_ = dynamic_cast<number_seconds*>(elm.get());
			if (number_seconds_)
			{
				if (sz == 1)	number_seconds_->number_style_ = L"short";
				if (sz == 2)	number_seconds_->number_style_ = L"long";
				//number_decimal_places_
			}
		}
		else if((res=s.find(L"pm")) <0)//так уж формат делится .. а этот текст нам не нужен
		{
		//////////////////// делитель ////////////////////	
			if(sz>1) 
			{
				//выкинем "лишние" слэши
				boost::algorithm::replace_all(s, L"\\", L"");
			}
			create_element(L"number", L"text", elm, &context_);
			number_text* number_text_ = dynamic_cast<number_text*>(elm.get());
			if (number_text_)number_text_->add_text(s); 
		}
		if (elm)
		{
			root_elm->add_child_element(elm);
			styles_elments.push_back(elm);
		}
	}
}

void odf_number_styles_context::create_boolean_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"boolean-style", root_elm, &context_);
}

void odf_number_styles_context::create_text_style(number_format_state & state, office_element_ptr & root_elm)
{
	create_element(L"number", L"text-style", root_elm, &context_);
}

void odf_number_styles_context::detect_format(number_format_state & state)
{
	if (state.ods_type != office_value_type::Custom)return;
	if (state.format_code.size()<1)return;

 	//find [$<Currency String>-<language info>].
	boost::wregex re(L"(?:\\[)(?:\\$)(\\S+)?\-(\\S+)(?:\\])");
	boost::wsmatch result;
	bool b = boost::regex_search(state.format_code[0], result, re);
	if (b && result.size()==3)
	{
		state.currency_str=result[1];
		state.language_code=result[2];

		 state.format_code[0] = boost::regex_replace( state.format_code[0],re,L"");
	}
	if (state.currency_str.length()>0)
	{
		state.ods_type = office_value_type::Currency;
		return;
	}
	std::wstring tmp = state.format_code[0]; 
	boost::algorithm::to_lower(tmp);
	if (state.format_code.size() == 1)//any
	{
		int res=0;
		if ((res=tmp.find(L"at"))>=0 || (res=tmp.find(L"pm"))>=0 ||
			 (res=tmp.find(L"h"))>=0 || (res=tmp.find(L"s"))>=0  || state.language_code == L"F400")
		{
			state.ods_type = office_value_type::Time;
			return;
		}
		if ((res=tmp.find(L"y"))>=0 || (res=tmp.find(L"d"))>=0 || (res=tmp.find(L"m"))>=0)//minutes отсеялись выше
		{
			state.ods_type = office_value_type::Date;
			return;
		}		
		if ((res=tmp.find(L"%"))>=0)
		{
			state.ods_type = office_value_type::Percentage;
			return;
		}

		state.ods_type = office_value_type::Float;
		return;
	}
	else if (state.format_code.size() == 2)//>0, <0
	{

	}
	else if (state.format_code.size() == 3)//>0, <0, ==0
	{
	}
///////////////////////////////
}
}
}
