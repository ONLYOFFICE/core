
#include "xlsx_defined_names.h"

#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "../formulasconvert/formulasconvert.h"

namespace cpdoccore {
namespace oox {

class xlsx_defined_names::Impl
{
public:
    void add(std::wstring const & name, std::wstring const & ref, bool formula, int tableId)
    {
		int is_file_link = 0;

		if (!formula)
		{
			if ((is_file_link = ref.find(L"\\")) >=0) return;
			if ((is_file_link = ref.find(L"/")) >=0) return;
		}
        formulasconvert::odf2oox_converter converter;
        std::wstring oox_ref;
		
		if (formula)
		{
			oox_ref = converter.convert_named_expr(ref);
		}
		else
		{
			oox_ref = converter.convert_named_ref(ref);
		}
        content_.push_back(name_and_ref());
		
		content_.back().name	= name;
		content_.back().ref		= oox_ref;
		content_.back().tableId	= tableId;
    }

    void xlsx_serialize(std::wostream & _Wostream)
    {
        if (content_.size() > 0)
        {
            CP_XML_WRITER(_Wostream)
            {
                CP_XML_NODE(L"definedNames")
                {
                    for (int i = 0 ; i < content_.size(); i++)
                    {
                        CP_XML_NODE(L"definedName")
                        {
                            CP_XML_ATTR(L"name", content_[i].name);
							
							if (content_[i].tableId >= 0)
							{
								CP_XML_ATTR(L"localSheetId", content_[i].tableId);
							}	
							
							int pos;
							if ( (pos = content_[i].ref.find(L"#REF!")) >= 0 )
							{
								CP_XML_ATTR(L"comment", content_[i].ref);
								CP_XML_CONTENT(L"#REF!");
							}
							else
								CP_XML_CONTENT(content_[i].ref);


                        }
                    }
                }
            }
        }
    }

private:
	struct name_and_ref
	{
		name_and_ref() : tableId(-1) {}

		std::wstring	name;
		std::wstring	ref;
		int				tableId;
	};
	std::vector<name_and_ref> content_;
};

xlsx_defined_names::xlsx_defined_names() : impl_(new xlsx_defined_names::Impl())
{
}

xlsx_defined_names::~xlsx_defined_names()
{
}

void xlsx_defined_names::add(std::wstring const & name, std::wstring const & ref, bool formula, int tableId)
{
    return impl_->add(name, ref, formula, tableId);        
}

void xlsx_defined_names::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

}
}
