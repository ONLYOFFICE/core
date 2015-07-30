
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

void xlsx_drawing_context::start_drawing(std::wstring const & name, int type)
{
	count_object++;

	bool isIternal = false;
	std::wstring target;
	std::wstring rId = handle_.impl_->get_mediaitems().find_image(type, target, isIternal);


	if (!rId.empty())
	{
		xlsx_drawings_->add(stream_.str(), isIternal, rId , target, external_items::typeImage);
	}
}

void xlsx_drawing_context::end_drawing()
{
	bool isMediaInternal = true;

	//xlsx_drawings_->add(stream_.str(), isMediaInternal, rId , ref, external_items::typeImage);
	stream_.clear();

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

