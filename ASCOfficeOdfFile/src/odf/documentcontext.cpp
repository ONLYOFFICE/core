#include <stdio.h>

#include "documentcontext.h"

namespace cpdoccore { 
namespace odf {

class document_context::Impl
{
public:
    Impl() : last_paragraph_(NULL) {}

public:
    void set_last_paragraph(text::paragraph * Paragraph)
    {
        last_paragraph_ = Paragraph;
    }

    text::paragraph * get_last_paragraph()
    {
        return last_paragraph_;
    }

private:
    text::paragraph * last_paragraph_;

};

//////////////////////////////////////////////////////////////////////////////////////////////////

document_context::document_context() : impl_( new document_context::Impl() )
{            
}

document_context::~document_context()
{
    delete impl_;
}

void document_context::set_last_paragraph(text::paragraph * Paragraph)
{
    return impl_->set_last_paragraph(Paragraph);
}

text::paragraph * document_context::get_last_paragraph()
{
    return impl_->get_last_paragraph();
}



}
}
