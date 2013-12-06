#pragma once

#include <atlstr.h>

#include "..\..\Common\DocxFormat\Source\XML\XmlUtils.h"

namespace DocFileFormat
{
	class AbstractOpenXmlMapping
	{
	public: 
		AbstractOpenXmlMapping (XmlUtils::CXmlWriter* pWriter) : m_pXmlWriter(pWriter)
		{

		}

		inline XmlUtils::CXmlWriter* GetXMLWriter()
		{
			return m_pXmlWriter;
		}

		virtual ~AbstractOpenXmlMapping()
		{

		}

	protected:

		XmlUtils::CXmlWriter* m_pXmlWriter;
	};
}