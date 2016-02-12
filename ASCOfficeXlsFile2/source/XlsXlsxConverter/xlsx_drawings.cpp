
#include "xlsx_drawings.h"

#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <vector>

#include <simple_xml_writer.h>

#include "mediaitems_utils.h"
#include "oox_rels.h"

namespace oox {

class xlsx_drawings_rels::Impl
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
    void add(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
		external_items::Type type)
    {
		bool present = false;
        
		BOOST_FOREACH(rel_ const & r, rels_)
        {		
			if (r.rid_ == rid && r.target_ == target)
				present = true;
		}
		if (!present)
		{
			rels_.push_back(rel_(isInternal, rid, target, type));
		}
    }

   void dump_rels(rels & Rels)
    {
        BOOST_FOREACH(rel_ const & r, rels_)
        {
			if (r.type_ == external_items::typeChart)
			{
				Rels.add(relationship(
							r.rid_,
							utils::media::get_rel_type(r.type_),
							(r.is_internal_ ? std::wstring(L"../") + r.target_ : r.target_),
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
			else if (r.type_ == external_items::typeImage)
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
							(r.is_internal_ ? L"" : L"External"))
				);
			}
		}
    }

private:
	std::vector<rel_>	rels_;
};

xlsx_drawings_rels::xlsx_drawings_rels() : impl_( new xlsx_drawings_rels::Impl() )
{
}

xlsx_drawings_rels::~xlsx_drawings_rels()
{
}

void xlsx_drawings_rels::add(
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & target,
        external_items::Type type)
{
    impl_->add(isInternal, rid, target, type);
}

void xlsx_drawings_rels::dump_rels(rels & Rels)
{
    return impl_->dump_rels(Rels);
}

xlsx_drawings_rels_ptr xlsx_drawings_rels::create()
{
    return boost::make_shared<xlsx_drawings_rels>();
}

}
