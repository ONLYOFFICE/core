#pragma once

#include "WordDocument.h"
#include "StyleSheet.h"
#include "AbstractOpenXmlMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CharacterPropertiesMapping.h"
#include "TablePropertiesMapping.h"
#include "ContentTypes.h"
#include "ConversionContext.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class StyleSheetMapping: public AbstractOpenXmlMapping, public IMapping
	{
	private: 
		ConversionContext*	_ctx;
		WordDocument*		m_document;

	public: 
		StyleSheetMapping( ConversionContext* ctx );
		void Apply( IVisitable* visited );
		/// Generates a style id for custom style names or returns the build-in identifier for build-in styles.
		static wstring MakeStyleId( StyleSheetDescription* std );
		static map<std::wstring, std::wstring> m_mapStyleId;
		static AVSOfficeCriticalSection m_mapStyleIdLock;
		virtual ~StyleSheetMapping();

	private:
		void writeRunDefaults( StyleSheet* sheet );
		void writeParagraphDefaults( StyleSheet* sheet );
		/// Chooses the correct style name.
		/// Word 2007 needs the identifier instead of the stylename for translating it into the UI language.
		wstring getStyleName( StyleSheetDescription* std );
		/// Writes the "NormalTable" default style
		void writeNormalTableStyle();
	};
}