#pragma once

#include "../Common/XmlTools.h"
#include "Global.h"
#include "BorderCode.h"
#include "RGBColor.h"
#include "ShadingDescriptor.h"
#include "SinglePropertyModifier.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"


#include "../Win32/ASCOfficeCriticalSection.h" //for linux make inside

namespace DocFileFormat
{
	class PropertiesMapping
	{
	public:
		PropertiesMapping( XmlUtils::CXmlWriter* writer );

	protected:
		static void init();
		void appendFlagAttribute( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* attributeName );
		virtual void appendFlagElement( XMLTools::XMLElement<wchar_t>* node, const SinglePropertyModifier& sprm, const wchar_t* elementName, bool unique );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, const wchar_t* attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, int attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, short attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, unsigned short attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<wchar_t>* node, const wchar_t* attributeName, unsigned char attributeValue );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, const wchar_t* elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, short elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, unsigned short elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, unsigned char elementValue, bool unique );
		void appendBorderAttributes( BorderCode* brc, XMLTools::XMLElement<wchar_t>* border );
		void appendShading( XMLTools::XMLElement<wchar_t>* parent, const ShadingDescriptor& desc );
		wstring getBorderType( unsigned char type );
		wstring getShadingPattern( const ShadingDescriptor& shd );
		void appendDxaElement( XMLTools::XMLElement<wchar_t>* node, const wchar_t* elementName, const wchar_t* elementValue, bool unique );
		void addOrSetBorder( XMLTools::XMLElement<wchar_t>* pBdr, const XMLTools::XMLElement<wchar_t>* border );

	protected:

		XmlUtils::CXmlWriter* m_pXmlWriter;
		static map<unsigned char, wstring> brcTypeMap;
		
		static ASCOfficeCriticalSection brcTypeMapLock;
	};
}
