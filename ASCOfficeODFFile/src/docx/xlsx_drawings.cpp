#include "precompiled_cpodf.h"
#include "xlsx_drawings.h"
#include "xlsx_drawing.h"
#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "mediaitems_utils.h"
#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawings::Impl
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
    void add(_xlsx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		mediaitems::Type type)
    {
        xlsx_drawings_.push_back(d);
		
		bool present = false;
        BOOST_FOREACH(rel_ const & r, xlsx_drawing_rels_)
        {		
			if (r.rid_ == rid && r.ref_ == ref)
				present = true;
		}
		if (!present)
		{
			xlsx_drawing_rels_.push_back(rel_(isInternal, rid, ref, type));
		}
        BOOST_FOREACH(_hlink_desc h, d.hlinks)
        {
			xlsx_drawing_rels_.push_back(rel_(false, h.hId, h.hRef, mediaitems::typeHyperlink));
		}
    }
    void add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		mediaitems::Type type)
    {
		bool present = false;
        BOOST_FOREACH(rel_ const & r, xlsx_drawing_rels_)
        {		
			if (r.rid_ == rid && r.ref_ == ref)
				present = true;
		}
		if (!present)
		{
			xlsx_drawing_rels_.push_back(rel_(isInternal, rid, ref, type));
		}
    }


	void serialize(std::wostream & strm) const
    {
        CP_XML_WRITER(strm)
        {
            CP_XML_NODE(L"xdr:wsDr")
            {
                CP_XML_ATTR(L"xmlns:xdr", L"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing");
                CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
                CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

				BOOST_FOREACH(_xlsx_drawing const & d, xlsx_drawings_)
                {
                    xlsx_serialize(CP_XML_STREAM(), d);
                }
			}
        }
    }
    
    bool empty() const
    {
        return (xlsx_drawings_.empty());
    }

    void dump_rels(rels & Rels)
    {
        BOOST_FOREACH(rel_ const & r, xlsx_drawing_rels_)
        {
            const bool valid = utils::media::check_file_type(r.ref_);
			
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
							/*valid ? (*/r.is_internal_ ? std::wstring(L"../") + r.ref_ : r.ref_/*) : L"NULL"*/,
							(r.is_internal_ ? L"" : L"External")
							) 
					);
			}
			//typeShape внутренний рисованый объект - релсов нет
 			else if (r.type_ == mediaitems::typeHyperlink)//заместо гипрелинка пользуем неизвестный ... поменять ... временно .. сделать красиво
			{
				Rels.add(relationship(
							r.rid_,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
							r.ref_,
							L"External")
				);
			}
		}
    }

private:

	std::vector<_xlsx_drawing> xlsx_drawings_;
	
	std::vector<rel_> xlsx_drawing_rels_;
};

xlsx_drawings::xlsx_drawings() : impl_( new xlsx_drawings::Impl() )
{
}

xlsx_drawings::~xlsx_drawings()
{
}

void xlsx_drawings::add(_xlsx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
{
    impl_->add(d, isInternal, rid, ref, type);
}
void xlsx_drawings::add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
{
    impl_->add(isInternal, rid, ref, type);
}
void xlsx_serialize(std::wostream & _Wostream, xlsx_drawings const & val)
{
    val.impl_->serialize(_Wostream);
}

bool xlsx_drawings::empty() const
{
    return impl_->empty();
}

void xlsx_drawings::dump_rels(rels & Rels)
{
    return impl_->dump_rels(Rels);
}

xlsx_drawings_ptr xlsx_drawings::create()
{
    return boost::make_shared<xlsx_drawings>();
}

}
}

