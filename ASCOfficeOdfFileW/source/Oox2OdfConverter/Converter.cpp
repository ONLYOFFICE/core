#include "stdAfx.h"

#include "Converter.h"


#include <XlsxFormat\Xlsx.h>
#include <DocxFormat\Docx.h>

#include <boost/foreach.hpp>

#include "odf_conversion_context.h"

using namespace cpdoccore;

namespace Oox2Odf
{
	class Impl
	{
        friend class Converter;
	public:
		Impl(const std::wstring & path,const std::wstring & type) : output_document(type)
		{
			const OOX::CPath oox_path(CString(path.c_str()));
			
			docx_document =0;
			xlsx_document =0;
		
			if (type == L"text")
			{
				docx_document = new OOX::CDocx(oox_path);	
			}
			if (type == L"spreadsheet")
			{
				xlsx_document = new OOX::Spreadsheet::CXlsx(oox_path);	
			}

		}

	public:
		void convert(/*Odt::IOdtEvent& Event*/);
		void write(const std::wstring & path);

    private:
		OOX::CDocx					*docx_document;
		OOX::Spreadsheet::CXlsx		*xlsx_document;

		odf::package::odf_document		output_document;
	};

	void Impl::write(const std::wstring & path)
	{
		output_document.write(path);

	}

	void Impl::convert()
	{
		odf::odf_conversion_context		odf_conversion_context_(&output_document);

		if (docx_document)
		{
			//нужно само содержимое документа, окружение не важно (Core, App) 
			//стили тоже сами по себе не нужны - нужны только те которые используются в документе  
			const OOX::CDocument* document = docx_document->GetDocument();

			if (document)
			{
				//odf_context.convert(document); - формирование объектов в odf

			}
		}
		else if (xlsx_document)
		{
			const OOX::Spreadsheet::CWorkbook *document = xlsx_document->GetWorkbook();

			if (document)
			{
				odf_conversion_context_.start_document();
				document->m_oSheets;
				//odf_context.convert(document); - формирование объектов в odf
				
				
				//...
				odf_conversion_context_.end_document();
			}
		}
	}


    ///////////////

	Converter::Converter(const std::wstring & path, const std::wstring & type) 
    {
		impl_ = new Impl(path,type);
	}
	Converter::~Converter() 
    {
		if (impl_ )delete impl_ ;
	}
    void Converter::convert(/*Odt::IOdtEvent& Event*/)
    {
		if (!impl_)return;
        impl_->convert(/*Event*/);
    }
    void Converter::write(const std::wstring & path) const
    {
        return impl_->write(path);
    }

    //void Converter::app2meta(const OOX::App& app, Odt::Meta::File& meta) const
    //{
    //    return impl_->app2meta(app, meta);
    //}

} // namespace Docx2Odt