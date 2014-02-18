#include "stdAfx.h"

#include "Converter.h"


#include <XlsxFormat\Xlsx.h>
#include <DocxFormat\Docx.h>

#include <boost/foreach.hpp>

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
				m_input_docx = new OOX::CDocx(oox_path);	
			}
			if (type == 2)
			{
				m_input_xlsx = new OOX::Spreadsheet::CXlsx(oox_path);	
			}
		}

	public:
		void convert(/*Odt::IOdtEvent& Event*/);
		void write(const CString & path);

    private:
		OOX::CDocx					*m_input_docx;
		OOX::Spreadsheet::CXlsx		*m_input_xlsx;
		
		
		//Odf::Folder m_output; //odf_document 
	};

	void Impl::write(const CString & path)
	{
		//m_output.write(path);
	}

	void Impl::convert()
	{
		bool cancelled = false;
		int percent = 150000;

		if (m_input_docx)
		{
			const OOX::CApp *app = m_input_docx->GetApp();
			if (app)
			{
				percent += 250000;
				if(cancelled)
					return;
			}
			
			const OOX::CCore *core = m_input_docx->GetCore();
			if (core)
			{
				percent += 250000;
				if(cancelled)
					return;
			}				
			const OOX::CDocument* document = m_input_docx->GetDocument();

			if (document)
			{
				//odf_context.convert(document);

				percent += 250000;
				if(cancelled)
					return;
			}
		}
		else if (m_input_xlsx)
		{
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