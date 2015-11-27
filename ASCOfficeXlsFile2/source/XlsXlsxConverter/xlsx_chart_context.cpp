#include "xlsx_conversion_context.h"
#include "oox_rels.h"
#include "mediaitems_utils.h"

//#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

#include <simple_xml_writer.h>

namespace oox {

class oox_chart_context::Impl
{
    struct rel_
    { 
        rel_(bool is_internal, std::wstring const & rid, std::wstring const & target,  external_items::Type type) :
        is_internal_(is_internal),
        rid_(rid),
		target_(target),
        type_(type)
        {}

        bool is_internal_;
        std::wstring rid_;
        std::wstring target_;
        external_items::Type type_;
    };

public:
	Impl() {}
    
    std::wstringstream  chartData_;
	std::vector<rel_>	chartRels_;
   
	void dump_rels(rels & Rels)
    {
        BOOST_FOREACH(rel_ const & r, chartRels_)
        {
			if (r.type_ == external_items::typeImage)
			{
				Rels.add(relationship(
							r.rid_,
							utils::media::get_rel_type(r.type_),
							r.is_internal_ ? std::wstring(L"../") + r.target_ : r.target_,
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
 			else if (r.type_ == external_items::typeHyperlink)
			{
				Rels.add(relationship(
							r.rid_,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
							r.target_,
							L"External")
				);
			}
		}
    }


    void add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type)
    {
		bool present = false;
        
		BOOST_FOREACH(rel_ const & r, chartRels_)
        {		
			if (r.rid_ == rid && r.target_ == target)
				present = true;
		}
		if (!present)
		{
			chartRels_.push_back(rel_(isInternal, rid, target, type));
		}
    }

};

oox_chart_context_ptr oox_chart_context::create()
{
    return boost::make_shared<oox_chart_context>();
}

oox_chart_context::oox_chart_context() : impl_(new oox_chart_context::Impl())
{
}
std::wostream & oox_chart_context::chartData()
{
    return impl_->chartData_;
}
void oox_chart_context::add_rels(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
        external_items::Type type)
{
    impl_->add_rels(isInternal, rid, target, type);
}
void oox_chart_context::dump_rels(rels & Rels)
{
	impl_->dump_rels(Rels);
}
void oox_chart_context::write_to(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"c:chartSpace")
		{          
            CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
			CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
            CP_XML_ATTR(L"xmlns:c", L"http://schemas.openxmlformats.org/drawingml/2006/chart");
		
			CP_XML_NODE(L"c:lang")
			{
				CP_XML_ATTR(L"val",L"en-US");
			}
			CP_XML_STREAM() << impl_->chartData_.str();
		}
	}

}

oox_chart_context::~oox_chart_context()
{
}


}

