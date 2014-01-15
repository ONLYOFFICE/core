
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"


namespace OOX
{
	namespace Settings
	{

		File::File()
		{
		}


		File::File(const boost::filesystem::wpath& filename)
		{
			read(filename);
		}


		File::~File()
		{
		}


		void File::read(const boost::filesystem::wpath& filename)
		{
			const XML::XDocument document(filename);

			m_zoom							= document.Root.element("zoom");
			m_proofState				= document.Root.element("proofState");
			m_defaultTabStop		= document.Root.element("defaultTabStop").attribute("val").value();
			m_characterSpacingControl = document.Root.element("characterSpacingControl").attribute("val").value(); 
			m_compat						= document.Root.element("compat").attribute("val").value();
			m_mathPr						= document.Root.element("mathPr");			
			m_footnotePr				= document.Root.element("footnotePr");
			m_endnotePr					= document.Root.element("endnotePr");
			m_themeFontLang			= document.Root.element("themeFontLang").attribute("val").value();
			m_clrSchemeMapping	= document.Root.element("clrSchemeMapping");
			m_shapeDefaults			= document.Root.element("shapeDefaults");
			m_decimalSymbol			= document.Root.element("decimalSymbol").attribute("val").value();
			m_listSeparator		  = document.Root.element("listSeparator").attribute("val").value();	
			EvenAndOddHeaders = document.Root.element("evenAndOddHeaders").exist();
			m_hdrShapeDefaults	= document.Root.element("hdrShapeDefaults");
			m_drawingGridHorizontalSpacing			= document.Root.element("drawingGridHorizontalSpacing").attribute("val").value();;
			m_displayHorizontalDrawingGridEvery = document.Root.element("displayHorizontalDrawingGridEvery").attribute("val").value();
		}


		void File::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
		{
			XML::XElement(ns.w + "settings",
				XML::Write(m_zoom) +
				XML::Write(m_proofState) +
				XML::XElement(ns.w + "defaultTabStop", XML::XAttribute(ns.w + "val", m_defaultTabStop)) +
				XML::XElement(ns.w + "drawingGridHorizontalSpacing", XML::XAttribute(ns.w + "val", m_drawingGridHorizontalSpacing)) +
				XML::XElement(ns.w + "displayHorizontalDrawingGridEvery", XML::XAttribute(ns.w + "val", m_displayHorizontalDrawingGridEvery)) +
				XML::XElement(ns.w + "characterSpacingControl", XML::XAttribute(ns.w + "val", m_characterSpacingControl)) +
				XML::WriteIf(XML::XElement(ns.w + "evenAndOddHeaders"), EvenAndOddHeaders) +
				XML::Write(m_hdrShapeDefaults) +
				XML::Write(m_footnotePr) +
				XML::Write(m_endnotePr) +
				XML::XElement(ns.w + "compat", XML::XAttribute(ns.w + "val", m_compat)) +
				XML::Write(m_mathPr) +				
				XML::XElement(ns.w + "themeFontLang", XML::XAttribute(ns.w + "val", m_themeFontLang)) +
				XML::Write(m_clrSchemeMapping) +
				XML::Write(m_shapeDefaults) +
				XML::XElement(ns.w + "decimalSymbol", XML::XAttribute(ns.w + "val", m_decimalSymbol)) +
				XML::XElement(ns.w + "listSeparator", XML::XAttribute(ns.w + "val", m_listSeparator))			
			).Save(filename);

			content.registration(type().OverrideType(), directory, filename);
		}


		const FileType File::type() const
		{
			return FileTypes::Setting;
		}


		const boost::filesystem::wpath File::DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}


		const boost::filesystem::wpath File::DefaultFileName() const
		{
			return type().DefaultFileName();
		}

	} // namespace Settings
} // namespace OOXFile