#pragma once

#include "..\Common\XmlTools.h"
#include "Global.h"
#include "BorderCode.h"
#include "RGBColor.h"
#include "ShadingDescriptor.h"
#include "SinglePropertyModifier.h"

#include <atlstr.h>
#include "..\..\..\..\..\Common\xmlutils.h"

namespace DocFileFormat
{
	class PropertiesMapping
	{
	public:
		PropertiesMapping( XmlUtils::CXmlWriter* writer );

	protected:
		static void init();
		void appendFlagAttribute( XMLTools::XMLElement<WCHAR>* node, const SinglePropertyModifier& sprm, const WCHAR* attributeName );
		virtual void appendFlagElement( XMLTools::XMLElement<WCHAR>* node, const SinglePropertyModifier& sprm, const WCHAR* elementName, bool unique );
		void appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, const WCHAR* attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, int attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, short attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, unsigned short attributeValue );
		void appendValueAttribute( XMLTools::XMLElement<WCHAR>* node, const WCHAR* attributeName, byte attributeValue );
		void appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, const WCHAR* elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, short elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, unsigned short elementValue, bool unique );
		void appendValueElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, byte elementValue, bool unique );
		void appendBorderAttributes( BorderCode* brc, XMLTools::XMLElement<WCHAR>* border );
		void appendShading( XMLTools::XMLElement<WCHAR>* parent, const ShadingDescriptor& desc );
		wstring getBorderType( byte type );
		wstring getShadingPattern( const ShadingDescriptor& shd );
		void appendDxaElement( XMLTools::XMLElement<WCHAR>* node, const WCHAR* elementName, const WCHAR* elementValue, bool unique );
		void addOrSetBorder( XMLTools::XMLElement<WCHAR>* pBdr, const XMLTools::XMLElement<WCHAR>* border );

	protected:

		XmlUtils::CXmlWriter* m_pXmlWriter;
		static map<byte, wstring> brcTypeMap;
		static AVSOfficeCriticalSection brcTypeMapLock;
	};
}