#ifndef _CPDOCCORE_DOCX_CONVERSIONSTATE_H_
#define _CPDOCCORE_DOCX_CONVERSIONSTATE_H_

namespace cpdoccore {
namespace oox {

struct conversion_state
{
    const odf::style_instance * current_processed_style_;
    std::list< const odf::style_text_properties * > text_properties_stack_;
    bool page_break_after_;
    bool in_automatic_style_;
    std::wstring current_page_properties_;
    bool next_dump_page_properties_;
    std::wstring text_list_style_name_;
    std::list<std::wstring> list_style_stack_;   
    bool first_element_list_item_;
    bool in_paragraph_;
    std::list<odf::office_element *> delayed_elements_;
    odf::office_element * section_properties_in_table_;
    std::wstring current_master_page_name_;
    bool rtl_; // right-to-left
};

}
}

#endif
