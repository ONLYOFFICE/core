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
#pragma once

#include "../../DataTypes/stylefamily.h"
#include "../../DataTypes/length.h"
#include "../../DataTypes/noteclass.h"

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <list>

#include "docx_table_context.h"
#include "oox_conversion_context.h"
#include "oox_chart_context.h"
#include "xlsx_drawing_context.h"

#include "headers_footers.h"
#include "hyperlinks.h"
#include "mediaitems.h"

namespace cpdoccore { 

	namespace odf_types
	{
		class length_or_percent;
	}
	namespace odf_reader 
	{
		class style_instance;
		class odf_document;
		class style_text_properties;
		class draw_frame;
		class draw_shape;
		class draw_control;
		class office_element;
		class style_columns;
		class form_element;
		class text_linenumbering_configuration;

		namespace text
		{
			class note_citation;
			class paragraph_attrs;
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
typedef boost::shared_ptr<streams_man> StreamsManPtr;
//------------------------------------------------------------------------------------------------------

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
    void start_control(odf_reader::draw_control * drawControl) 
    { 
		current_level_++;
        objects_count_++; 
		
		current_shape_id_		= objects_count_;
        current_object_name_	= L"";
        current_control_		= drawControl;
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
		if (groups_.empty()) return;

		if (groups_.back().x > x)	groups_.back().x = x;
		if (groups_.back().y > y)	groups_.back().y = y;
	}	
	void set_size_child_group(_INT32 cx, _INT32 cy)
	{
		if (groups_.empty()) return;

		if (groups_.back().cx < cx)	groups_.back().cx = cx;
		if (groups_.back().cy < cy)	groups_.back().cy = cy;
	}
	void get_position_group(_INT32 & x, _INT32 & y)
	{
		x = y = 0;
		if (groups_.empty()) return;

		x = groups_.back().x;
		y = groups_.back().y;
	}
	void get_size_group(_INT32 & cx, _INT32 & cy)
	{
		cx = cy = 0;
		if (groups_.empty()) return;

		cx = groups_.back().cx;
		cy = groups_.back().cy;
	}
	void stop_frame()
	{
		current_level_--;
		frames_.pop_back();
		caption_.clear();
	}

	std::wstring & get_text_stream_frame()
	{
		if (false == frames_.empty())
			return frames_.back().text_content;
		else
			return zero_string_;
	}
	bool & get_use_image_replace()
	{
		bool res = false;
		if (false == frames_.empty())	return frames_.back().use_image_replace;
		else
			throw;
	}
	std::wstring & get_text_stream_shape()
	{
		return shape_text_content_;
	}
	void clear_stream_shape()
	{
		shape_text_content_ = L"";
	}
 	void clear_stream_frame()
	{
		if (false == frames_.empty())
			frames_.back().text_content.clear();
	}
	void stop_shape()
	{
		current_level_--;
		current_shape_ = NULL;
		shape_text_content_.clear();
		current_shape_id_ = 0;
		caption_.clear();
	}
	void stop_control()
	{
		current_level_--;
		current_control_ = NULL;
		shape_text_content_.clear();
		current_shape_id_ = 0;
		caption_.clear();
	}
	void stop_group()
	{
		current_level_--;
		groups_.pop_back();
	}
	
	int get_current_frame_id() const 
	{
		if (false == frames_.empty())	return frames_.back().id;
		else					return 0;
	}
	bool	in_group()						{ return !groups_.empty(); }
    int		get_current_level()		const	{ return current_level_; }
 	int		get_current_shape_id()	const	{ return current_shape_id_; }
	
	const std::wstring & get_current_object_name() const { return current_object_name_; }
   
	odf_reader::draw_frame * get_current_frame() const 
	{
		if (false == frames_.empty()) return frames_.back().ptr; 
		else return NULL;
	}
	odf_reader::draw_shape * get_current_shape() const { return current_shape_; }

	void set_next_object_caption(const std::wstring & value)
	{
		caption_ = value;
	}
	
private:
	std::wstring			shape_text_content_;

	std::wstring			current_object_name_;
    unsigned int			objects_count_;
	
	int						current_level_;
	
	std::vector<_group>		groups_;
	std::vector<_frame>		frames_; 
   
	odf_reader::draw_shape *current_shape_; 
	size_t					current_shape_id_;

	odf_reader::draw_control*current_control_; 

	std::wstring			zero_string_;
	std::wstring			caption_;

};

class section_context : boost::noncopyable
{
public:
	section_context() /*: after_section_(false) */{}
    
	struct _section 
	{
		_section() : is_dump_(false){}
		
		_section(const std::wstring & SectionName, const std::wstring & Style, const std::wstring & PageProperties) : is_dump_(false)
		{ 
			name_			= SectionName;
			style_			= Style;
			page_properties_.push_back(PageProperties);
		}

		std::wstring				name_; 
		std::wstring				style_; 
		_CP_OPT(odf_types::length_or_percent) margin_left_, margin_right_;
		
		std::vector<std::wstring>	page_properties_;

		bool						is_dump_ = false;
	};
  
	void add_section(const std::wstring & SectionName, const std::wstring & Style, const std::wstring & PageProperties);
  
	bool is_empty()		
	{ 
		return sections_.empty(); 
	}
    _section & get_first();
    _section & get_last();

	void remove_section();

	std::wstring dump_;
  
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
    std::wstring add	(const std::wstring & Content, const std::wstring & Id);

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
	comments_context(){}
	
	struct _comment_desc
	{
		_comment_desc(const std::wstring & c, int _id, const std::wstring & a, const std::wstring & d) :
		content(c), id(_id), author(a), date(d)
		{}
		std::wstring content;
		int id;
		std::wstring date;
		std::wstring author;
		std::wstring initials;
	};
	void end_comment(const std::wstring & name)
	{
		std::map<std::wstring, int>::iterator pFind = comments_map_.find(name);
		if (pFind != comments_map_.end())
		{
			ref_end_.push_back(pFind->second);		
		}
	}
	void start_comment(const std::wstring & content, const std::wstring & author, const std::wstring & date, _CP_OPT(std::wstring) name)
	{
		int id = comments_.size() + 1;
		_comment_desc new_comment(content, id, author, date);
		
		if ((name) && (false == name->empty()))
		{
			comments_map_.insert(std::make_pair(*name, id));
			ref_start_.push_back(id);
		}
		else
		{
			ref_.push_back(id);
		}
		comments_.push_back(new_comment);
	}
	
	rels& get_rels(){return internal_rels_;}

	std::vector<_comment_desc>	comments_;
	std::vector<int>			ref_start_;
	std::vector<int>			ref_end_;
	std::vector<int>			ref_;

private:
	rels internal_rels_;//это для гиперлинков или медиа в комментариях
	std::map<std::wstring, int> comments_map_;
};

class text_tracked_context
{
public:
	struct _state
	{
		std::wstring				id;
		std::wstring				author;
		std::wstring				date;
		int							type = 0;
		std::wstring				style_name;
		std::vector<std::wstring>	content;	//delete elements		
		
		bool						active = false;
		bool						in_drawing = false;
		bool						out_active = false;
		_UINT32						oox_id = 0;

		void clear()
		{
			out_active = false;
			type = 0;
			id.clear();
			author.clear();
			date.clear();
			content.clear();
			active = false;
			in_drawing = false;
			oox_id = 0;
		}
	};
	std::wstring dumpPPr_;
	std::wstring dumpRPr_;
	std::wstring dumpRPrInsDel_;
	std::wstring dumpTblPr_;
	std::wstring dumpTcPr_;

	text_tracked_context(docx_conversion_context & context);

	void start_changes_content		();
	void end_changes_content		();

	void start_change				(std::wstring id);
	void end_change					();

	void set_user_info				(std::wstring &author, std::wstring &date);
	void set_type					(int type);
	void set_style_name				(std::wstring style_name);

	_state & get_tracked_change		(std::wstring id);

private:

	docx_conversion_context		&	docx_context_;
	
	bool							bParaStateDocx_;
	bool							bRunStateDocx_;

	_state							current_state_;

	StreamsManPtr					docx_stream_;
	std::wstringstream				changes_stream_;
	
	std::map<std::wstring, _state>	mapChanges_;
};

class table_content_context
{
public:
	table_content_context()
	{
		clear_all();
	}
	enum template_type
	{
		TableContent	= 1,
		Illustrations	= 2,
		Bibliography	= 3,
		Tables			= 4
	};
	enum level_type
	{
		Span = 1,
		Text = 2,
		LinkStart = 3,
		LinkEnd = 4,
		TabStop = 5,
		PageNumber = 6,
		Chapter = 7
	};

	struct _state
	{
		std::wstring name;
		int outline_level = -1;
		std::vector<int> content;

		void clear()
		{
			name.clear();
			content.clear();
			outline_level = -1;
		}
	};
	void start_template(int type)
	{
		current_template.clear();
		type_table_content = type;
	}
	void end_template()
	{
	}

	void start_level(const std::wstring& style_name)
	{
		current_state.name = style_name;
	}
	void add_level_content(int type)
	{
		current_state.content.push_back(type);
	}	
	void add_outline_level_style(const std::wstring& style_name, int level)
	{
		if (min_outline_level == -1 || min_outline_level > level)
			min_outline_level = level;
		
		if (max_outline_level == -1 || max_outline_level < level)
			max_outline_level = level;

		if (!style_name.empty())
			outline_level_styles.insert(std::make_pair(level, style_name));

		current_state.outline_level = level;
	}
	void end_level()
	{
		current_template.push_back(current_state);

		map_current_template.insert(std::make_pair(current_state.name, current_state));
		current_state.clear();
	}
	void set_current_level(const std::wstring &name)
	{
		std::map<std::wstring, _state>::iterator pFind = map_current_template.find(name);
		if (pFind == map_current_template.end())
		{
			current_content_template_.clear();
		}
		else 
			current_content_template_ = pFind->second.content;
		current_content_template_index_ = 0;
	}

	void next_level_index()
	{
		current_content_template_index_++;
	}
	int get_type_current_content_template_index()
	{
		if (current_content_template_index_ < (int)current_content_template_.size() && current_content_template_index_ >= 0)
			return current_content_template_[current_content_template_index_];

		return 0;
	}

	void clear_current_content_template_index()
	{
		current_content_template_index_ = 0;
		current_content_template_.clear();
	}

	bool empty_current_table_content_level_index()
	{
		return current_content_template_.empty();
	}
	void clear_all()
	{
		type_table_content = 0;
		current_content_template_index_ = 0;
		current_content_template_.clear();
		current_template.clear();
		map_current_template.clear();
		current_state.clear();
		caption_sequence_name.clear();
		min_outline_level = -1;
		max_outline_level = -1;
		outline_level_styles.clear();
		bSeparators = false;
	}
	void add_sequence(const std::wstring & name, int outline_level)
	{
		//sequences.insert(std::make_pair(name, outline_level));
		sequences.push_back(name);
	}

	std::wstring get_sequence (const std::wstring & ref)
	{
		for (size_t i = 0; i < sequences.size(); i++)
		{
			if (std::wstring:: npos != ref.find(sequences[i]))
			{
				return sequences[i];
			}
		}
		return L"";
	}

	std::wstring					caption_sequence_name;
	int								type_table_content;
	int								min_outline_level;
	int								max_outline_level;
	std::map<int, std::wstring>		outline_level_styles;
	bool							bSeparators;
	std::vector<_state>				current_template;

	std::map<std::wstring, std::wstring> mapReferences;

private:
	std::vector<int>				current_content_template_;
	int								current_content_template_index_;

	std::map<std::wstring, _state>	map_current_template;
	_state							current_state;
	//std::map<std::wstring, int>	sequences;
	std::vector<std::wstring>		sequences;
};
//---------------------------------------------------------------------------------------------------------
class docx_conversion_context : boost::noncopyable
{
public:
	enum NoteType { noNote, footNote, footNoteRefSet, endNote, endNoteRefSet };
	
	docx_conversion_context(odf_reader::odf_document * _odf_document);
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

	void add_bibliography_item	(const std::wstring & item);
	void add_user_field			(const std::wstring & name, const std::wstring & value);	
	std::wstring get_user_field	(const std::wstring & name);

	void add_element_to_run		(std::wstring parenStyleId = _T(""));
    void finish_run				();
	void add_new_run			(std::wstring parentStyleId = _T(""));    
  
    void start_paragraph		(bool is_header = false);
    void finish_paragraph		();

	bool is_alphabetical_index	()					{ return false == mapAlphabeticals.empty();}
	bool is_table_content		()					{ return in_table_content_; }
	bool is_paragraph_header	()					{ return in_header_;		}
	
	void reset_context_state();
	void back_context_state();
	
	bool get_run_state			()					{ return state_.in_run_; }
    void set_run_state			(bool Val)			{ state_.in_run_ = Val; }
	
	bool get_paragraph_state	()					{ return state_.in_paragraph_;		}
    void set_paragraph_state	(bool val)			{ state_.in_paragraph_	= val;		}
	
	bool get_paragraph_keep		()					{ return state_.is_paragraph_keep_;}
	void set_paragraph_keep		(bool val)			{ state_.is_paragraph_keep_ = val;	}

	bool get_delete_text_state	()					{ return is_delete_text_;	}
	void set_delete_text_state	(bool Val)			{ is_delete_text_ = Val;	}

	bool get_drawing_state_content()			{ return in_drawing_content_;	}
	void set_drawing_state_content(bool val)	{ in_drawing_content_ = val;	}

	std::wstring		add_hyperlink	(const std::wstring & href, bool drawing);
    hyperlinks::_ref	last_hyperlink	();
    void				dump_hyperlinks	(rels & Rels, _rels_type_place type);

    void dump_headers_footers	(rels & Rels) const;
    void dump_notes				(rels & Rels) const;

	_rels_type_place get_type_place();
	
	void dump_bibliography();
	
	std::wstring  dump_settings_document();
	std::wstring  dump_settings_app();
	std::wstring  dump_settings_core();

 	bool next_dump_page_properties_;
	bool next_dump_section_;
	bool last_dump_page_properties_;

	int nFormFieldId_ = 0;

    odf_reader::odf_document *root()
    {
        return odf_document_;
    }
    void start_document	();
    void end_document	();

    void start_body		();
    void end_body		();
    
	void start_office_text	();
    void end_office_text	();

	void start_sdt	(int type);
	void end_sdt	();

	void start_index_content();
	void end_index_content();

	void start_index_element();
	void end_index_element();

	void start_field(const std::wstring & sInstrText, const std::wstring & sName);
	void end_field();

	void process_styles			();
    void process_fonts			();
    
    void process_list_styles	();
    void process_headers_footers();
    void process_comments		();
    bool process_page_properties(std::wostream & strm);
	void process_section		(std::wostream & strm, odf_reader::style_columns * columns = NULL);
	
	int process_paragraph_style (_CP_OPT(std::wstring) style_name_ptr);
	int process_paragraph_attr	(odf_reader::text::paragraph_attrs *attr);
	int process_text_attr		(odf_reader::text::paragraph_attrs *Attr);
	void process_page_break_after(const odf_reader::style_instance *styleInst);

	std::vector<odf_reader::_property> & get_settings_properties ();
    void set_settings_property		(const odf_reader::_property & prop);

    void start_process_style_content();
    void end_process_style_content	(bool in_styles = false);

    void start_automatic_style	(const std::wstring & ParentId);
    void end_automatic_style	();
    bool in_automatic_style		();

    void push_text_properties	(const odf_reader::style_text_properties * TextProperties);
    void pop_text_properties	();
   
	odf_reader::style_text_properties_ptr current_text_properties();

    void set_page_break_after(int val);
    int get_page_break_after();

    void set_page_break_before(int val);
    int get_page_break_before();

	void set_page_break		(bool val);
    bool get_page_break		();
	
	void			add_page_properties		(const std::wstring & StyleName);
    std::wstring	get_page_properties		();
	void			remove_page_properties	();
    
	void next_dump_page_properties			(bool val);
    bool is_next_dump_page_properties		();
	void last_dump_page_properties			(bool val);
	bool is_last_dump_page_properties		();

    void set_master_page_name(const std::wstring & MasterPageName);
    const std::wstring & get_master_page_name() const;

    void start_text_list_style	(const std::wstring & StyleName);
    void end_text_list_style	();
    
	std::wstring	get_text_list_style_name();
	std::wstring	current_list_style();
 
	void start_list				(const std::wstring & StyleName, bool Continue = false);
    void end_list				();
    void start_list_item		(bool restart = false);
    void end_list_item	();
    
	void serialize_list_properties(std::wostream & strm);
	void serialize_paragraph_style(std::wostream & strm, const std::wstring & ParentId, bool in_styles = false);
   
	std::wstring find_list_rename(const std::wstring & ListStyleName) const;

    styles_map			* get_style_map()			{ return &styles_map_; }

	mediaitems_ptr		& get_mediaitems()			{return mediaitems_;}
    styles_context		& get_styles_context()		{ return styles_context_; }
    drawing_context		& get_drawing_context()		{ return drawing_context_; } 	
	comments_context	& get_comments_context()	{ return comments_context_; }
	math_context		& get_math_context()		{ return math_context_; }
	section_context		& get_section_context()		{ return section_context_; }
	notes_context		& get_notes_context()		{ return notes_context_; }
	text_tracked_context& get_text_tracked_context(){ return text_tracked_context_; }
	forms_context		& get_forms_context()		{ return forms_context_; }
	tabs_context		& get_tabs_context()		{ return tabs_context_;}
	table_content_context & get_table_content_context()	{ return table_content_context_;}
	
	xlsx_drawing_context_handle_ptr & get_chart_drawing_handle() { return chart_drawing_handle_;} 

	void set_drawing_text_props (const std::wstring &props);

    void docx_convert_delayed	();
    void add_delayed_element	(odf_reader::office_element * Elm);

    docx_table_context & get_table_context() { return table_context_; }

    StreamsManPtr	get_stream_man() const				{ return streams_man_; }
    void			set_stream_man(StreamsManPtr Sm)	{ streams_man_ = Sm; }

    void set_rtl(bool val)			{ is_rtl_ = val; }
    bool get_rtl() const			{return is_rtl_;}
   
	double get_current_fontSize()	{return current_fontSize.empty() ? 0 : current_fontSize.back();}
	void pop_current_fontSize()		{if (!current_fontSize.empty()) current_fontSize.pop_back();}
	
	void set_outline_level(int val)	{current_outline_level_ = val;}
	int get_outline_level()			{return current_outline_level_;}

	void set_process_note		(NoteType Val) { current_process_note_ = Val; }
	NoteType get_process_note	() const		{ return current_process_note_; }
	void add_note_reference		();

	void start_paragraph_style(const std::wstring& style_name) {paragraph_style_stack_.push_back(style_name);}
	void end_paragraph_style() { if (!paragraph_style_stack_.empty()) paragraph_style_stack_.pop_back();}
	
	_CP_OPT(std::wstring) get_current_paragraph_style()	
	{
		_CP_OPT(std::wstring) result;
		if (false == paragraph_style_stack_.empty())

			result = paragraph_style_stack_.back();
		
		return result;
	}
	
	oox_chart_context & current_chart();
    void start_chart(std::wstring name);
    void end_chart	();

	void start_comment_content	()	{current_process_comment_ = true;}
	void end_comment_content	()	{current_process_comment_ = false;}
   
	void start_comment(const std::wstring & content, const std::wstring & author, const std::wstring & date, _CP_OPT(std::wstring) name);
	void end_comment(const std::wstring & name);

	void start_math_formula	();
	void end_math_formula	();

	void start_text_changes	(const std::wstring &id);
	void end_text_changes	(const std::wstring &id);
	
	void start_bookmark	(const std::wstring &name);
	void end_bookmark	(const std::wstring &name);

	void start_alphabetical_index (const std::wstring &id);
	void end_alphabetical_index	(const std::wstring &id);
	void add_alphabetical_index_text (odf_reader::office_element_ptr & elem);

	void set_process_headers_footers(bool Val)				{ process_headers_footers_ = Val; }
   
	headers_footers			& get_headers_footers()			{ return headers_footers_; }
	header_footer_context	& get_header_footer_context()	{ return header_footer_context_; }
	drop_cap_context		& get_drop_cap_context()		{ return drop_cap_context_; }
	
	styles_map	styles_map_;
	bool		process_headers_footers_;
	bool		delayed_converting_;
	bool		convert_delayed_enabled_;

	void		start_changes();
	void		end_changes();

	void		add_jsaProject(const std::string &content);

private:

	struct _context_state
	{
		bool in_paragraph_ = false;
		bool in_run_ = false;
		bool is_paragraph_keep_ = false; 

		std::wstring drawing_text_props_;

		std::vector< const odf_reader::style_text_properties*> text_properties_stack_;
	}state_;
	std::vector<_context_state> keep_state_;
//---------------------------------------------------------	
	std::wstringstream		document_xml_;
    std::wstringstream		styles_xml_;
    std::wstringstream		fontTable_xml_;
    std::wstringstream		numbering_xml_;
    std::wstringstream		temp_stream_;
	std::wstringstream		mimetype_xml_;
    
    std::wstringstream		footer_xml_;
    std::wstringstream		header_xml_;
    std::wstringstream		settings_xml_;
    std::wstringstream		meta_xml_;
//--------------------------------------------------------------
	xlsx_drawing_context_handle_ptr chart_drawing_handle_; 

	styles_context			styles_context_;
	math_context			math_context_;
    drawing_context			drawing_context_;
	comments_context		comments_context_;
    section_context			section_context_;
	drop_cap_context		drop_cap_context_;
    docx_table_context		table_context_;
    header_footer_context	header_footer_context_;
    notes_context			notes_context_;
	text_tracked_context	text_tracked_context_;
	forms_context			forms_context_;
	tabs_context			tabs_context_;
	table_content_context	table_content_context_;
       
    boost::shared_ptr<streams_man> streams_man_;

    package::docx_document		* output_document_;
    odf_reader::odf_document	* odf_document_;

	std::vector<odf_reader::_property>		settings_properties_;

	hyperlinks								hyperlinks_;
    mediaitems_ptr							mediaitems_;     
	std::vector<oox_chart_context_ptr>		charts_;
    headers_footers							headers_footers_;

    std::wstring				automatic_parent_style_; 
    std::wstring				current_master_page_name_;
	std::wstring				text_list_style_name_;	
	std::vector<std::wstring>	paragraph_style_stack_;
	std::vector<std::wstring>	list_style_stack_;
	std::vector<std::wstring>	fields_names_stack_;
    
	bool first_element_list_item_;
    
	int page_break_after_; // 0 = false, 1 - column, 2 -page
    int page_break_before_;
	bool page_break_;

	bool in_automatic_style_; 
	bool in_drawing_content_;
	bool in_table_content_;

	bool in_header_;
	bool is_delete_text_;
    bool is_rtl_; // right-to-left
 
	NoteType			current_process_note_;
	bool				current_process_comment_;
	std::vector<double> current_fontSize;
	std::wstring		current_alphabetic_index_;
	int					current_margin_left_;
	int					current_outline_level_;
    int					new_list_style_number_;	// счетчик для нумерации имен созданных в процессе конвертации стилей
    
	std::vector<odf_reader::office_element*>							delayed_elements_;

	std::map<std::wstring, text_tracked_context::_state>				map_current_changes_;    
    boost::unordered_map<std::wstring, std::wstring>					list_style_renames_;// цепочки переименований нумераций
	
	std::map<std::wstring, std::wstring>								map_user_fields;
	std::map<std::wstring, int>											mapBookmarks;
	std::map<std::wstring, std::vector<odf_reader::office_element_ptr>> mapAlphabeticals;

	std::vector<std::wstring>											arBibliography;
};

}
}
