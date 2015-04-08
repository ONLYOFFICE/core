#include "../odf/precompiled_cpodf.h"
#include "xlsx_merge_cells.h"
#include "xlsx_utils.h"
#include <ostream>
#include <vector>
#include <boost/foreach.hpp>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore {
namespace oox {

class xlsx_merge_cells::Impl
{
    struct merge
    {
        merge(size_t _c, size_t _r, size_t _cols, size_t _rows): 
            c(_c), r(_r), cols(_cols), rows(_rows)
            {}
        size_t c, r, cols, rows;
    };

public:
    void add_merge(size_t column, size_t row, size_t colsMerged, size_t rowsMerged)
    {
        merges_.push_back(merge(column, row, colsMerged, rowsMerged));
    }

    void xlsx_serialize(std::wostream & _Wostream)
    {
        if (!merges_.empty())
        {
            CP_XML_WRITER(_Wostream)
            {
                CP_XML_NODE(L"mergeCells")
                {
                    CP_XML_ATTR(L"count", merges_.size());

                    BOOST_FOREACH(const merge & m, merges_)
                    {
                        CP_XML_NODE(L"mergeCell")
                        {
                            CP_XML_ATTR(L"ref", getCellAddress(m.c, m.r) + L":" + getCellAddress(m.c + m.cols, m.r + m.rows));
                        } // L"mergeCell"
                    }
                } // L"mergeCells"            
            }

            //_Wostream << L"<mergeCells count=\"" << merges_.size() << L"\">";
            //BOOST_FOREACH(const merge & m, merges_)
            //{
            //    _Wostream << L"<mergeCell ref=\"" 
            //        << getCellAddress(m.c, m.r) << L":" 
            //        << getCellAddress(m.c + m.cols, m.r + m.rows) << L"\" />";
            //}
            //_Wostream << L"</mergeCells>";
        }
    }

private:
    std::vector<merge> merges_;
};

xlsx_merge_cells::xlsx_merge_cells(): impl_(new xlsx_merge_cells::Impl())
{}

xlsx_merge_cells::xlsx_merge_cells(const xlsx_merge_cells & m): impl_(new xlsx_merge_cells::Impl(*m.impl_))
{
}

const xlsx_merge_cells & xlsx_merge_cells::operator=(const xlsx_merge_cells & m)
{
    _CP_SCOPED_PTR(Impl) newImpl(new xlsx_merge_cells::Impl(*m.impl_));
    newImpl.swap(impl_);        
    return *this;
}

xlsx_merge_cells::~xlsx_merge_cells()
{}

void xlsx_merge_cells::add_merge(size_t column, size_t row, size_t colsMerged, size_t rowsMerged)
{
    return impl_->add_merge(column, row, colsMerged, rowsMerged);
}

void xlsx_merge_cells::xlsx_serialize(std::wostream & _Wostream)
{
    return impl_->xlsx_serialize(_Wostream);
}

}
}
