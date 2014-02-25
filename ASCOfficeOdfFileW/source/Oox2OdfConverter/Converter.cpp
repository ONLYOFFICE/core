#include "stdAfx.h"

#include "Converter.h"


#include <XlsxFormat\Xlsx.h>
#include <DocxFormat\Docx.h>

#include <boost/foreach.hpp>

#include "odf_conversion_context.h"

namespace Oox2Odf
{
	class Impl
	{
        friend class Converter;
	public:
		Impl(const CString & path)
		{
			const OOX::CPath oox_path(path);
			m_input_docx =0;
			m_input_xlsx =0;


			int type = 1;	//get type oox ????
		
			if (type == 1)
			{
				docx_document = new OOX::CDocx(oox_path);	
			}
			if (type == 2)
			{
				xlsx_document = new OOX::Spreadsheet::CXlsx(oox_path);	
			}

		}

	public:
		void convert(/*Odt::IOdtEvent& Event*/);
		void write(const CString & path);

    private:
		OOX::CDocx					*docx_document;
		OOX::Spreadsheet::CXlsx		*xlsx_document;

		package::odf_document		outputDocument;

		
		
		//Odf::Folder m_output; //odf_document 
	};

	void Impl::write(const CString & path)
	{
		outputDocument.write(path);

	}

	void Impl::convert()
	{
		odf_conversion_context		odf_conversion_context_(&outputDocument);

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
				odf_conversion_context_.start_documet();
				document->m_oSheets;
				//odf_context.convert(document); - формирование объектов в odf
				
				
				//...
				odf_conversion_context_.end_documet();
			}
		}
	}


    ///////////////

    Converter::Converter(const CString & path) 
    {
		impl_ = new Impl(path);
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
    void Converter::write(const CString & path) const
    {
        return impl_->write(path);
    }

    //void Converter::app2meta(const OOX::App& app, Odt::Meta::File& meta) const
    //{
    //    return impl_->app2meta(app, meta);
    //}

} // namespace Docx2Odt