
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

//#include <cpdoccore/xml/utils.h>
//
//#include "mediaitems_utils.h"
//
//#include "xlsx_utils.h"
#include "xlsx_drawing_context.h"



namespace oox {


class xlsx_drawing_context_handle::Impl
{
public:
    Impl(external_items & items)
        : items_(items), next_rId_(1), next_drawing_id_(1) 
    {
    }  

    external_items & get_mediaitems() { return items_; }

    size_t next_rId()
    {
        return next_rId_++;
    }

    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings)
    {
        const std::wstring id = boost::lexical_cast<std::wstring>(next_drawing_id_++);
        const std::wstring fileName = std::wstring(L"drawing") + id + L".xml";
       
		drawings_.push_back(drawing_elm(fileName, content, drawings));
       
		const std::wstring rId = std::wstring(L"rId") + id;//rDrId
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }
    const std::vector<drawing_elm> & content() const
    {
        return drawings_;
    }

	xlsx_drawings_ptr get_drawings();
private:
    external_items & items_;
    size_t next_rId_;
    
    std::vector<drawing_elm> drawings_;

    size_t next_drawing_id_;
};

xlsx_drawing_context_handle::xlsx_drawing_context_handle(external_items & items)
: impl_(new xlsx_drawing_context_handle::Impl(items))
{
}

xlsx_drawing_context_handle::~xlsx_drawing_context_handle()
{
}
std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings )
{
    return impl_->add_drawing_xml(content, drawings);
}

const std::vector<drawing_elm> & xlsx_drawing_context_handle::content() const
{
    return impl_->content();
}

//---------------------------------------------------------------------------------------

xlsx_drawing_context::xlsx_drawing_context(xlsx_drawing_context_handle & h)
 : handle_(h), xlsx_drawings_(xlsx_drawings::create()), count_object(0)
{    
}

bool xlsx_drawing_context::start_drawing(int type)
{
	switch(type)
	{
	case 0x0000: // Group
	case 0x0001: // Line
	case 0x0002: // Rectangle
	case 0x0003: // Oval
	case 0x0004: // Arc
		start_shape(type); return true;
	case 0x0005: // Chart  
	case 0x0006: // Text
	case 0x0007: // Button
		break;
	case 0x0008: // Picture
		start_image(); return true;
	case 0x0009: // Polygon:
	case 0x000B: // Checkbox
	case 0x000C: // Radio button
	case 0x000D: // Edit box
	case 0x000E: // Label
	case 0x000F: // Dialog box
	case 0x0010: // Spin control
	case 0x0011: // Scrollbar
	case 0x0012: // List
	case 0x0013: // Group box
	case 0x0014: // Dropdown list
	case 0x0019: // Note
	case 0x001E: // OfficeArt object
		break;
	}
	return false;
	//count_object++;

	//bool isIternal = false;
	//std::wstring target;
	//std::wstring rId = handle_.impl_->get_mediaitems().find_image(type, target, isIternal);


	//if (!rId.empty())
	//{
	//	xlsx_drawings_->add(stream_.str(), isIternal, rId , target, external_items::typeImage);
	//}
}

void xlsx_drawing_context::start_image()
{
	_drawing_state st;
	drawing_state.push_back(st);

	drawing_state.back().type = external_items::typeImage;
}

void xlsx_drawing_context::start_shape(int type)
{
	_drawing_state st;
	drawing_state.push_back(st);

	drawing_state.back().type = external_items::typeShape;
}
void xlsx_drawing_context::end_drawing()
{
	if (drawing_state.size() < 1 )return;

	std::wstringstream strm;

	//serialize

	xlsx_drawings_->add(strm.str(), drawing_state.back().isMediaInternal, 
						drawing_state.back().rId , drawing_state.back().target, drawing_state.back().type);

}

bool xlsx_drawing_context::empty()
{
	return xlsx_drawings_->empty();
}

xlsx_drawings_ptr xlsx_drawing_context::get_drawings()
{
    return xlsx_drawings_;
}

}

