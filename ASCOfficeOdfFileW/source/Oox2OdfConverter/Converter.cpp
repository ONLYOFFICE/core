#include "stdAfx.h"

#include "Converter.h"


#include <XlsxFormat\Xlsx.h>
#include <DocxFormat\Docx.h>

#include <boost/foreach.hpp>

#include "ods_conversion_context.h"
//#include "odt_conversion_context.h"

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
		odf::ods_conversion_context		ods_conversion_context_(&output_document);

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
			const OOX::Spreadsheet::CWorkbook *Workbook= xlsx_document->GetWorkbook();
			CAtlMap<CString, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = xlsx_document->GetWorksheets();

			//convert(xlsx_document->GetWorkbook(),odf_conversion_context_);
			
			if (Workbook)
			{
				ods_conversion_context_.start_document();

				if(Workbook->m_oSheets.IsInit())
				{
					CSimpleArray<OOX::Spreadsheet::CSheet*>& aWs = Workbook->m_oSheets->m_arrItems;
					
					for(int i = 0, length = aWs.GetSize(); i < length; ++i)
					{
						OOX::Spreadsheet::CSheet* pSheet = aWs[i];
							
						if(pSheet->m_oRid.IsInit())
						{
							CString sSheetRId = pSheet->m_oName.get2();
							CAtlMap<CString, OOX::Spreadsheet::CWorksheet*>::CPair* pPair = arrWorksheets.Lookup(sSheetRId);
							if (NULL != pPair)
							{
								OOX::Spreadsheet::CWorksheet *pWorksheet = pPair->m_value;
								if (NULL != pWorksheet && pWorksheet->m_oSheetData.IsInit())
								{
									std::wstring name = L"";/*pSheet->m_oName.get2()*/
									ods_conversion_context_.start_sheet(name);
									//CAtlMap<CString, OOX::Spreadsheet::CWorksheet*>::CPair* pair = aWorksheets.Lookup(pSheet->m_oRid->GetValue());
									//if(NULL != pair)
									//{
									//	nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Worksheet);
									//	WriteWorksheet(*pSheet, *pair->m_value);
									//	m_oBcw.WriteItemWithLengthEnd(nCurPos);
									//}
									ods_conversion_context_.end_sheet();						
								}
							}

						}
					}
				}
				ods_conversion_context_.end_document();
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

} // namespace Docx2Odt