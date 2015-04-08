#include "../odf/precompiled_cpodf.h"

#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "mediaitems_utils.h"
#include "docx_rels.h"

#include "pptx_drawings.h"
#include "pptx_drawing.h"

namespace cpdoccore {
namespace oox {

class pptx_drawings::Impl
{
    struct rel_
    { 
        rel_(bool is_internal, std::wstring const & rid, std::wstring const & ref,  mediaitems::Type type) :
        is_internal_(is_internal),
        rid_(rid),
        ref_(ref),
        type_(type)
        {}

        bool is_internal_;
        std::wstring rid_;
        std::wstring ref_;
        mediaitems::Type type_;
    };

public:
    void add(_pptx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		mediaitems::Type type)
    {
        pptx_drawings_.push_back(d);
		
		bool present = false;
        BOOST_FOREACH(rel_ const & r, pptx_drawing_rels_)
        {		
			if (r.rid_ == rid && r.ref_ == ref)
				present = true;
		}
		if (!present)
		{
			pptx_drawing_rels_.push_back(rel_(isInternal, rid, ref, type));
		}
        BOOST_FOREACH(_hlink_desc h, d.hlinks)
        {
			pptx_drawing_rels_.push_back(rel_(false, h.hId, h.hRef, mediaitems::typeHyperlink));
		}
    }

    void add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		mediaitems::Type type)
    {
	
		bool present = false;
        BOOST_FOREACH(rel_ const & r, pptx_drawing_rels_)
        {		
			if (r.rid_ == rid && r.ref_ == ref)
				present = true;
		}
		if (!present)
		{
			pptx_drawing_rels_.push_back(rel_(isInternal, rid, ref, type));
		}
    }
	void serialize(std::wostream & strm)
	{
		BOOST_FOREACH(_pptx_drawing const & d, pptx_drawings_)
		{
			pptx_serialize(strm,d);
		}
	}

    bool empty() const
    {
        return (pptx_drawings_.empty());
    }

    void dump_rels(rels & Rels)
    {
        BOOST_FOREACH(rel_ const & r, pptx_drawing_rels_)
        {
			if (r.type_ == mediaitems::typeChart)//временно - нужно потом все загнать в релс
			{
				Rels.add(relationship(
							r.rid_,
							utils::media::get_rel_type(r.type_),
							(r.is_internal_ ? std::wstring(L"../") + r.ref_ : r.ref_),
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
			else if (r.type_ == mediaitems::typeImage)
			{
				Rels.add(relationship(
							r.rid_,
							utils::media::get_rel_type(r.type_),
							r.is_internal_ ? std::wstring(L"../") + r.ref_ : r.ref_,
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
 			else if (r.type_ == mediaitems::typeHyperlink)
			{
				Rels.add(relationship(
							r.rid_,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
							r.ref_,
							L"External")
				);
			}
			else if (r.type_ == mediaitems::typeComment)
			{
				Rels.add(relationship(
							r.rid_,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments",
							r.ref_)
				);
			}
		}
    }

private:

	std::vector<_pptx_drawing> pptx_drawings_;
	
	std::vector<rel_> pptx_drawing_rels_;
};

pptx_drawings::pptx_drawings() : impl_( new pptx_drawings::Impl() )
{
}

pptx_drawings::~pptx_drawings()
{
}

void pptx_drawings::add(_pptx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
{
    impl_->add(d, isInternal, rid, ref, type);
}
void pptx_drawings::add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
{
    impl_->add(isInternal, rid, ref, type);
}
void pptx_drawings::serialize(std::wostream & _Wostream)
{
	impl_->serialize(_Wostream);
}

bool pptx_drawings::empty() const
{
    return impl_->empty();
}

void pptx_drawings::dump_rels(rels & Rels)
{
    return impl_->dump_rels(Rels);
}

pptx_drawings_ptr pptx_drawings::create()
{
    return boost::make_shared<pptx_drawings>();
}

}
}

