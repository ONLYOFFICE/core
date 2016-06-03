#include "xlsx_fill.h"
#include "xlsx_fills.h"

#include <cpdoccore/xml/simple_xml_writer.h>

#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/unordered_set.hpp>

namespace cpdoccore {
namespace oox {

class xlsx_fills::Impl
{
public:

    typedef boost::unordered_set<xlsx_fill, boost::hash<xlsx_fill> > xlsx_fill_array;

    xlsx_fill_array fills_;
};

xlsx_fills::xlsx_fills(): impl_( new xlsx_fills::Impl() )
{
//defaults fill
     {
        xlsx_patternFill patternFill;
        patternFill.patternType = L"none";

        xlsx_fill fill;
        fill.patternFill = patternFill;
        fill.index = 0;
		fill.bDefault = true;

        impl_->fills_.insert(fill);
    }

    {
        xlsx_patternFill patternFill;
        patternFill.patternType = L"gray125";

        xlsx_fill fill;
        fill.patternFill = patternFill;
        fill.index = 1;
		fill.bDefault = false;

        impl_->fills_.insert(fill);
    }

}

xlsx_fills::~xlsx_fills()
{
}

size_t xlsx_fills::size() const
{
    return impl_->fills_.size();
}

size_t xlsx_fills::fillId(	const odf_reader::text_format_properties_content		* textProp,
							const odf_reader::paragraph_format_properties			* parProp,
							const odf_reader::style_table_cell_properties_attlist	* cellProp, bool default_set)
{
    bool is_default;
    return fillId(textProp, parProp, cellProp,default_set, is_default);
}

size_t xlsx_fills::fillId(	const odf_reader::text_format_properties_content		* textProp,
							const odf_reader::paragraph_format_properties			* parProp,
							const odf_reader::style_table_cell_properties_attlist	* cellProp, 
							bool default_set, bool & is_default)
{
	is_default = true;
	xlsx_fill fill(NULL, cellProp);
	
	if (fill.bEnabled)
	{
		fill.bDefault = default_set;

        Impl::xlsx_fill_array::const_iterator i = impl_->fills_.find(fill);
       
		if (i != impl_->fills_.end())
        {
            const std::size_t dbgId = i->index;
			if (default_set && i->bDefault != default_set)
			{
				fill.index = i->index;
				impl_->fills_.insert(i,fill);
			
				is_default = default_set;
				return fill.index;
			}

			is_default = i->bDefault;
            return dbgId;
			
        }
        else
        {
            fill.index = impl_->fills_.size();
            impl_->fills_.insert(fill);
		
			is_default = default_set;
            return fill.index;
       }
	}

    return 0;
}

namespace 
{

struct compare_xlsx_fills
{
    bool operator() (xlsx_fill const & x1, xlsx_fill const & x2)
    {
        return x1.index < x2.index;            
    }
};

}

void xlsx_fills::serialize(std::wostream & _Wostream) const
{
    std::vector<xlsx_fill> inst_array;
            
    BOOST_FOREACH(const xlsx_fill & inst, impl_->fills_)
    {
        inst_array.push_back(inst);
    }

    std::sort(inst_array.begin(), inst_array.end(), compare_xlsx_fills());

    CP_XML_WRITER(_Wostream)
    {
        CP_XML_NODE(L"fills")
        {
            CP_XML_ATTR(L"count", inst_array.size());
			
			for (int i = 0; i < inst_array.size(); i++)
            {
                xlsx_serialize(CP_XML_STREAM(), inst_array[i]);
            }
        }
    }

    //_Wostream << L"<fills count=\"" << inst_array.size() << L"\" >";
    //BOOST_FOREACH(const xlsx_fill & f, inst_array)
    //{
    //    ::cpdoccore::oox::xlsx_serialize(_Wostream, f);
    //}
    //_Wostream << L"</fills>";    
}

}
}
