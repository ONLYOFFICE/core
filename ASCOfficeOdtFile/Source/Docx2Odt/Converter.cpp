
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Converter.h"
#include "AbstractConverter.h"
//#include "Docx.h"
//#include "Odt.h"
#include "DocxFormat/Folder.h"
#include "OdtFormat/Folder.h"

#include "IFileContainer.h"
#include <boost/foreach.hpp>
#include "Document.h"
#include "OdtEvent.h"

namespace Docx2Odt
{

	class Converter_Impl : public AbstractConverter<Docx::Folder, Odt::Folder>
	{
        friend class Converter;
	public:
		Converter_Impl(const boost::filesystem::wpath& originPath);

	public:
		void convert(Odt::IOdtEvent& Event);

    private:
		void app2meta(const OOX::App& app, Odt::Meta::File& meta) const;
	};

	Converter_Impl::Converter_Impl(const boost::filesystem::wpath& originPath)
		: base(originPath)
	{
	}


	void Converter_Impl::convert(Odt::IOdtEvent& Event)
	{
		m_output = m_origin;
		bool cancelled = false;
		int percent = 150000;

		BOOST_FOREACH(const OOX::IFileContainer::RIdFilePair& pair, m_input)
		{
			if (dynamic_cast<const OOX::App*>(pair.second.get()) != 0)
			{
				const OOX::App& app = static_cast<const OOX::App&>(*pair.second);
				app2meta(app, *m_output.Meta);
				cancelled = Event.Progress(0, percent + 250000);
				percent += 250000;
				if(cancelled)
					return;
			}
			else if (dynamic_cast<const OOX::Core*>(pair.second.get()) != 0)
			{
				const OOX::Core& core = static_cast<const OOX::Core&>(*pair.second);
				cancelled = Event.Progress(0, percent + 250000);
				percent += 250000;
				if(cancelled)
					return;
			}
			else if (dynamic_cast<const OOX::Document*>(pair.second.get()) != 0)
			{
				const OOX::Document& document = static_cast<const OOX::Document&>(*pair.second);
				Document::convert(document, m_output);
				cancelled = Event.Progress(0, percent + 250000);
				percent += 250000;
				if(cancelled)
					return;
			}
		}
	}

	
	void Converter_Impl::app2meta(const OOX::App& app, Odt::Meta::File& meta) const
	{
		meta.Statistic->PageCount		=	app.Pages;
		meta.Statistic->ParagraphCount	=	app.Paragraphs;
		meta.Statistic->WordCount		=	app.Words;
		meta.Statistic->CharacterCount	=	app.CharactersWithSpaces;
	}

    ///////////////

    Converter::Converter(const boost::filesystem::wpath& originPath) : converter_( new Converter_Impl(originPath) )
    {}

    Converter::~Converter()
    {
        delete converter_;
    }

    void Converter::convert(Odt::IOdtEvent& Event)
    {
        converter_->convert(Event);
    }

    void Converter::read(const boost::filesystem::wpath& path)
    {
        return converter_->read(path);
    }

    void Converter::write(const boost::filesystem::wpath& path) const
    {
        return converter_->write(path);
    }

    void Converter::app2meta(const OOX::App& app, Odt::Meta::File& meta) const
    {
        return converter_->app2meta(app, meta);
    }

} // namespace Docx2Odt