
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FontFaceDecls.h"
#include <algorithm>
#include <boost/bind.hpp>


namespace Odt
{
	namespace Logic
	{

		FontFaceDecls::FontFaceDecls()
		{
		}


		FontFaceDecls::~FontFaceDecls()
		{
		}
	

		FontFaceDecls::FontFaceDecls(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FontFaceDecls& FontFaceDecls::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FontFaceDecls::fromXML(const XML::XNode& node)
		{
			XML::Fill(Fonts, node);
		}


		const XML::XNode FontFaceDecls::toXML() const
		{
			return XML::XElement(ns.office + "font-face-decls", XML::Write(Fonts));
		}


		void FontFaceDecls::Add(const FontFace& font)
		{
			if (std::find_if(Fonts->begin(), Fonts->end(), boost::bind(&FontFace::Name, _1) == *font.Name) == Fonts->end())
				Fonts->push_back(font);
		}

	} // namespace Logic
} // namespace Odt