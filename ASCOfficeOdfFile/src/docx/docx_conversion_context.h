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
#pragma once

#include "../odf/datatypes/stylefamily.h"
#include "../odf/datatypes/length.h"

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

#include "hyperlinks.h"
#include "mediaitems.h"
#include "headers_footers.h"
#include "docx_table_context.h"
#include "../odf/datatypes/noteclass.h"

#include "oox_conversion_context.h"
#include "oox_chart_context.h"

class CApplicationFonts;

namespace cpdoccore { 

	namespace odf_types
	{
		class style_ref;
		class length_or_percent;
	}
namespace odf_reader 
{
    class style_instance;
    class odf_document;
    class style_text_properties;
    class draw_frame;
    class draw_shape;
    typedef boost::shared_ptr<style_text_properties> style_text_properties_ptr;
    class office_element;
	class style_columns;

    
    namespace text{
        class note_citation;
    }
}

namespace oox {
    
class docx_conversion_context;
class rels;

namespace package
{
    class docx_document;
}

class streams_man
{
public:
    streams_man(std::wostream & strm) : stream_(strm)
    {
    }

    std::wostream & get() { return stream_; }

    static boost::shared_ptr<streams_man> create(std::wostream & strm)
    {
        return boost::shared_ptr<streams_man>( new streams_man(strm) );
    }

private:
    std::wostream & stream_;
};
class styles_map
{
public:
    styles_map() : count_(0) {}
    std::wstring get(const std::wstring & Name, odf_types::style_family::type Type);

    bool check(const std::wstring & Name, odf_types::style_family::type Type)
    {
        const std::wstring n = name(Name, Type);
        return map_.count(n) > 0;
    }

private:
    std::wstring name(const std::wstring & Name, odf_types::style_family::type Type);
    
    size_t count_;
    //boost::unordered_map<std::wstring, std::wstring> map_;

	std::multimap<std::wstring, std::wstring> map_;

};



class drawing_context : boost::noncopyable
{
public:
	struct _group
	{
		_group() : cx(0), cy(0), x(0x7fffffff), y(0x7fffffff) {}

		_INT32 x;
		_INT32 y;

		_INT32 cx;
		_INT32 cy;
	};
	struct _frame
	{
		odf_reader::draw_frame *ptr;
		std::wstring			text_content;
		size_t					id;
		bool					use_image_replace;
	};

    drawing_context() : objects_count_(0),  current_shape_(NULL),shape_text_content_(L""),zero_string_(L""),current_level_(0),current_shape_id_ (0){}
    
    void start_frame(odf_reader::draw_frame * drawFrame) 
    { 
        current_object_name_ = L"";
 		current_level_++;
        objects_count_++; 

		_frame fr = {drawFrame, L"", objects_count_, false};
   
		frames_.push_back(fr);
	}
    void start_shape(odf_reader::draw_shape * drawShape) 
    { 
		current_level_++;
        objects_count_++; 
		
		current_shape_id_		= objects_count_;
        current_object_name_	= L"";
        current_shape_			= drawShape;
    }
    void start_group() 
    { 
		current_level_++;
        objects_count_++; 
		
		current_shape_id_		= objects_count_;
        current_object_name_	= L"";
		
		groups_.push_back(_group());
    }
    void add_name_object(const std::wstring & name) 
    { 
        current_object_name_ = name;
    }
	void set_position_child_group(_INT32 x, _INT32 y)
	{
		if (groups_.size() < 1) return;

		if (groups_.back().x > x)	groups_.back().x = x;
		if (groups_.back().y > y)	groups_.back().y = y;
	}	
	void set_size_child_group(_INT32 cx, _INT32 cy)
	{
		if (groups_.size() < 1) return;

		if (groups_.back().cx < cx)	groups_.back().cx = cx;
		if (groups_.back().cy < cy)	groups_.back().cy = cy;
	}
	void get_position_group(_INT32 & x, _INT32 & y)
	{
		x = y = 0;
		if (groups_.size() < 1) return;

		x = groups_.back().x;
		y = groups_.back().y;
	}
	void get_size_group(_INT32 & cx, _INT32 & cy)
	{
		cx = cy = 0;
		if (groups_.size() < 1) return;

		cx = groups_.back().cx;
		cy = groups_.back().cy;
	}
	void stop_frame()
	{
		current_level_--;
		frames_.pop_back();
	}

	std::wstring & get_text_stream_frame()
	{
		if (frames_.size()>0)
			return frames_.back().text_content;
		else
			return zero_string_;
	}
	bool & get_use_image_replace()
	{
		bool res = false;
		if (frames_.size()>0)	return frames_.back().use_image_replace;
		else					return res;
	}

	std::wstring & get_text_stream_shape()
	{
		return shape_text_content_;
	}
	void clear_stream_shape()
	{
		shape_text_content_=L"";
	}
 	void clear_stream_frame()
	{
		if (frames_.size()>0)
			frames_.back().text_content=L"";
	}
	void stop_shape()
	{
		current_level_--;
		current_shape_ = NULL;
		shape_text_content_=L"";
		current_shape_id_ =0;
	}
	void stop_group()
	{
		current_level_--;
		groups_.pop_back();
	}
	
	int get_current_frame_id() const 
	{
		if (frames_.size()>0)	return frames_.back().id; 
		else					return 0;
	}
	bool	in_group()						{ return groups_.size() > 0; }
    int		get_current_level()		const	{ return current_level_; }
 	int		get_current_shape_id()	const	{ return current_shape_id_; }
	
	const std::wstring & get_current_object_name() const { return current_object_name_; }
   
	odf_reader::draw_frame * get_current_frame() const 
	{
		if (frames_.size()>0) return frames_.back().ptr; 
		else return NULL;
	}
	odf_reader::draw_shape * get_current_shape() const { return current_shape_; }
	
private:
	std::wstring			shape_text_content_;

	std::wstring			current_object_name_;
    unsigned int			objects_count_;
	
	int						current_level_;
	
	std::vector<_group>		groups_;
	std::vector<_frame>		frames_; 
   
	odf_reader::draw_shape * current_shape_; 
	size_t					current_shape_id_;

	std::wstring			zero_string_;

};

class section_context : boost::noncopyable
{
public:
	section_context() /*: after_section_(false) */{}
    
	struct _section 
	{
		_section() {is_dump_ = false;}
		
		_section(const std::wstring & SectionName, const std::wstring & Style, const std::wstring & PageProperties) 
		{ 
			is_dump_		= false;
			name_			= SectionName;
			style_			= Style;
			page_properties_.push_back(PageProperties);
		}

		std::wstring				name_; 
		std::wstring				style_; 
		_CP_OPT(odf_types::length_or_percent) margin_left_, margin_right_;
		
		std::vector<std::wstring>	page_properties_;

		bool						is_dump_;
	};
  
	void add_section(const std::wstring & SectionName, const std::wstring & Style, const std::wstring & PageProperties);
  
	bool is_empty()		
	{ 
		return sections_.empty(); 
	}
    _section & get()		
	{ 
		if (sections_.empty())
			return main_section_;
		else
			return sections_[0]; 
	}
	void remove_section()	
	{
		if (!sections_.empty()) 
		{
			sections_.erase(sections_.begin(), sections_.begin() + 1);
		}
	}
	std::wstring			dump_;
  
private:    
	_section				main_section_;
    std::vector<_section>	sections_; 
};

class header_footer_context
{
public:
    void reset()
    {
        header_ = _CP_OPT(odf_types::length)();
        footer_ = _CP_OPT(odf_types::length)();
    }

    void set_header(const _CP_OPT(odf_types::length) & Val) { header_ = Val; }
    void set_footer(const _CP_OPT(odf_types::length) & Val) { footer_ = Val; }
    const _CP_OPT(odf_types::length) & header() const { return header_; }
    const _CP_OPT(odf_types::length) & footer() const { return footer_; }

private:
    _CP_OPT(odf_types::length) header_;
    _CP_OPT(odf_types::length) footer_;
};

class drop_cap_context : boost::noncopyable
{
public:
	drop_cap_context():state_(0),Length(0),Scale(1),Space(113),FontSize(0){}//default from MS office 2010

	void state(int state)
	{
		state_=state;
		if (state_==0)
		{
			FontSize=0;
			Space=0;
			Scale=1;
			Length=0;
			Space = 113;
		}
	}
	int state(){return state_;}

	int Length;
	int Scale;
	int Space;
	int FontSize;

private:
	int state_;

};

class notes_context
{
public:
    notes_context() : note_citation_(0){}

    std::wstring next_id();
    std::wstring add(const std::wstring & Content, const std::wstring & Id);

    void set_current_note(odf_types::noteclass::type type, const odf_reader::text::note_citation * noteCitation)
    {
        type_ = type;        
        note_citation_ = noteCitation;
    }

    const odf_reader::text::note_citation * get_note_citation() const { return note_citation_; }
    const odf_types::noteclass::type		get_type() const {return type_; }

    struct instance
    {
        instance(const std::wstring & Id, const std::wstring & Content) : id(Id), content(Content) {}
        std::wstring id;
        std::wstring content;
    };
    typedef boost::shared_ptr<instance>						 instance_ptr;
    typedef boost::unordered_map<std::wstring, instance_ptr> instances_map; 

    instances_map & footnotes() { return instances_footnotes_; }
    instances_map & endnotes()	{ return instances_endnotes_; }    

	rels & endnotesRels()		{ return endnoteRels; }
	rels & footnotesRels()		{ return footnoteRels; }

	void dump_rels(rels & Rels)				const;

private:
    instances_map instances_footnotes_;
    instances_map instances_endnotes_;

	rels endnoteRels;
	rels footnoteRels;

    const odf_reader::text::note_citation * note_citation_;

    odf_types::noteclass::type type_;

};

class comments_context : boost::noncopyable
{
public:
	comments_context() : state_(0) {}
	
	struct _comment_desc
	{
		std::wstring content;
		int id;
		std::wstring date;
		std::wstring author;
		std::wstring initials;
	};
    void start_comment(const std::wstring & content, const std::wstring & author, const std::wstring & date)
	{
		int id = comments_.size()+1;
		_comment_desc new_comment={content,id,date,author};
		
		comments_.push_back(new_comment);

		state_ = 1;
	}
	int current_id()
	{
		if (comments_.size()>0 && state_ >0 )return comments_.back().id;
		else return -1; //not set
	}
	void state(int state) {state_ = state;}
	int state(){return state_;}
	rels& get_rels(){return internal_rels_;}

	std::vector<_comment_desc> comments_;
private:
	int state_;
	rels internal_rels_;//это для гиперлинков или медиа в комментариях
};

class docx_conversion_context : boost::noncopyable
{
public:
    docx_conversion_context(odf_reader::odf_document * OdfDocument);

	~docx_conversion_context();

	void set_output_document	(package::docx_document * document);
    void set_font_directory		(std::wstring pathFonts);
	
	std::wostream & output_stream()
    {
        if ( streams_man_)
            return streams_man_->get();
        else
            return temp_stream_;
    }

    void add_element_to_run(std::wstring parenStyleId = _T(""));
    void finish_run();
	void add_new_run(std::wstring parentStyleId = _T(""));
    bool get_run_state() { return current_run_; }
    void set_run_state(bool Val) { current_run_ = Val; }
  
    void start_paragraph();
    void finish_paragraph();
    bool get_paragraph_state()  { return in_paragraph_; }
    void set_paragraph_state(bool Val)  {in_paragraph_= Val; }

	 std::wstring		add_hyperlink	(const std::wstring & href, bool drawing);
    hyperlinks::_ref	last_hyperlink	();
    void				dump_hyperlinks	(rels & Rels, hyperlinks::_type_place type);

    std::wstring add_mediaitem(const std::wstring & uri, mediaitems::Type type, bool & isInternal, std::wstring & ref);
    
	void dump_mediaitems		(rels & Rels);
    void dump_headers_footers	(rels & Rels) const;
    void dump_notes				(rels & Rels) const;

 	bool next_dump_page_properties_;
	bool next_dump_section_;

    odf_reader::odf_document * root()
    {
        return odf_document_;
    }

    void start_document();
    void end_document();

	std::wstring  dump_settings_document();

    void start_body	();
    void end_body	();
    
	void start_office_text	();
    void end_office_text	();

    void process_styles			();
    void process_fonts			();
    
    void process_list_styles	();
    bool process_page_properties(std::wostream & strm);
    void process_headers_footers();
    void process_comments		();
	void process_section		(std::wostream & strm, odf_reader::style_columns * columns = NULL);


    void set_settings_property(const odf_reader::_property & prop);
	std::vector<odf_reader::_property> & get_settings_properties();

    void start_process_style_content();
    void end_process_style_content(bool in_styles = false);

    void start_automatic_style(const std::wstring & ParentId);
    void end_automatic_style();
    bool in_automatic_style();

    styles_context	& get_styles_context()	{ return styles_context_; }
    styles_map		* get_style_map()		{ return &styles_map_; }

    void push_text_properties(const odf_reader::style_text_properties * TextProperties);
    void pop_text_properties();
    odf_reader::style_text_properties_ptr current_text_properties();

    void set_page_break_after(bool val);
    bool get_page_break_after();

    void set_page_break_before(bool val);
    bool get_page_break_before();

	void set_page_break		(bool val);
    bool get_page_break		();
	
	void			add_page_properties		(const std::wstring & StyleName);
    std::wstring	get_page_properties		();
	void			remove_page_properties	();
    
	void next_dump_page_properties(bool val);
    bool is_next_dump_page_properties();

    void set_master_page_name(const std::wstring & MasterPageName);
    const std::wstring & get_master_page_name() const;

    void start_text_list_style(const std::wstring & StyleName);
    void end_text_list_style();
    const std::wstring & get_text_list_style_name();
    void start_list(const std::wstring & StyleName, bool Continue = false);
    void end_list();
    const std::wstring current_list_style() const;
    void start_list_item(bool restart = false);
    void end_list_item();
    
	void docx_serialize_list_properties(std::wostream & strm);
	void docx_serialize_paragraph_style(std::wostream & strm, const std::wstring & ParentId, bool in_styles = false);
   
	std::wstring find_list_rename(const std::wstring & ListStyleName) const;

    drawing_context & get_drawing_context() { return drawing_context_; } 
	
	comments_context & get_comments_context() {return comments_context_;}

    void docx_convert_delayed();
    void add_delayed_element(odf_reader::office_element * Elm);
	bool delayed_converting_;
	bool convert_delayed_enabled_;

    docx_table_context & get_table_context() { return table_context_; }

	section_context				& get_section_context() { return section_context_; }
    odf_reader::office_element	* get_section_properties_in_table();
	void section_properties_in_table(odf_reader::office_element * Elm);

    typedef boost::shared_ptr<streams_man> StreamsManPtr;
    void set_stream_man(StreamsManPtr Sm) { streams_man_ = Sm; }
    StreamsManPtr get_stream_man() const { return streams_man_; }

    void set_rtl(bool Val) { rtl_ = Val; }
    bool rtl() const {return rtl_;}

	notes_context & get_notes_context() { return notes_context_; }
   
	enum NoteType { noNote, footNote, footNoteRefSet, endNote, endNoteRefSet };

	void set_process_note(NoteType Val) { process_note_ = Val; }
	NoteType get_process_note() const { return process_note_; }
	void add_note_reference();

	oox_chart_context & current_chart();
    void start_chart(std::wstring const & name);
    void end_chart();

	void start_comment()	{process_comment_ = true;}
	void end_comment()		{process_comment_ = false;}
	bool process_comment_;
   
	void start_math_formula();
	void end_math_formula();
	bool process_math_formula_;

	void set_process_headers_footers(bool Val) { process_headers_footers_ = Val; }
    headers_footers			& get_headers_footers() { return headers_footers_; }
	header_footer_context	& get_header_footer_context() { return header_footer_context_; }
	bool process_headers_footers_;

	drop_cap_context & get_drop_cap_context(){return drop_cap_context_;}
	
	styles_map styles_map_;
private:
    std::wstringstream document_xml_;
    std::wstringstream styles_xml_;
    std::wstringstream fontTable_xml_;
    std::wstringstream numbering_xml_;
    std::wstringstream temp_stream_;
    
    std::wstringstream footer_xml_;
    std::wstringstream header_xml_;
    std::wstringstream settings_xml_;

        
    boost::shared_ptr<streams_man> streams_man_;

    package::docx_document		* output_document_;
    odf_reader::odf_document	* odf_document_;
    CApplicationFonts			* applicationFonts_;

	std::vector<odf_reader::_property> settings_properties_;

	bool current_run_;
  
	hyperlinks hyperlinks_;
    mediaitems mediaitems_;
     
    styles_context styles_context_;

    std::wstring automatic_parent_style_; 

    std::list< const odf_reader::style_text_properties * > text_properties_stack_;
    
	bool page_break_after_;
    bool page_break_before_;
	bool page_break_;
   
	bool in_automatic_style_;
    

   
	std::wstring text_list_style_name_;
    std::list<std::wstring> list_style_stack_;

    drawing_context drawing_context_;

	comments_context comments_context_;

    bool first_element_list_item_;
    bool in_paragraph_;

    std::list<odf_reader::office_element *> delayed_elements_;

	std::vector<oox_chart_context_ptr> charts_;

    section_context section_context_;

	drop_cap_context drop_cap_context_;

    docx_table_context table_context_;
    
    odf_reader::office_element * section_properties_in_table_;

    headers_footers headers_footers_;
    std::wstring current_master_page_name_;

    // счетчик для нумерации имен созданных в процессе конвертации стилей
    size_t new_list_style_number_;

    // цепочки переименований нумераций
    boost::unordered_map<std::wstring, std::wstring> list_style_renames_;

    bool rtl_; // right-to-left

    header_footer_context header_footer_context_;

    notes_context notes_context_;
    NoteType process_note_;


};

}
}
