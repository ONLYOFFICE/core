#pragma once

#include "IMapping.h"
#include "PropertiesMapping.h"
#include "CharacterPropertyExceptions.h"
#include "StyleSheetMapping.h"
#include "SectionPropertiesMapping.h"
#include "ConversionContext.h"
#include "TabDescriptor.h"
#include "LineSpacingDescriptor.h"
#include "WordDocument.h"

namespace DocFileFormat
{
	class ParagraphPropertiesMapping: public PropertiesMapping, public IMapping
	{
	public: 
		ParagraphPropertiesMapping( XmlUtils::CXmlWriter* writer, ConversionContext* ctx, WordDocument* document, CharacterPropertyExceptions* paraEndChpx, bool isParagraphStyleNeeded = true );
		ParagraphPropertiesMapping( XmlUtils::CXmlWriter* writer, ConversionContext* ctx, WordDocument* document, CharacterPropertyExceptions* paraEndChpx, SectionPropertyExceptions* sepx, int sectionNr, bool isParagraphStyleNeeded = true );
		virtual ~ParagraphPropertiesMapping();
		void Apply(IVisitable* visited);

	private:
		WordDocument*		m_document;
		ConversionContext*	m_context;
	
		XMLTools::XMLElement<wchar_t>* _pPr;
		XMLTools::XMLElement<wchar_t>* _framePr;
		SectionPropertyExceptions* _sepx;
		CharacterPropertyExceptions* _paraEndChpx;
		int _sectionNr;

		bool _isParagraphStyleNeeded;
	};
}
