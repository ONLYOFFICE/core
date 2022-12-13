#ifndef CSVGPARSER_H
#define CSVGPARSER_H

#include "CSvgStorage.h"

#include "../../../common/Directory.h"
#include "../../../xml/include/xmlutils.h"

namespace SVG
{
	class CSvgParser
	{
	public:
		CSvgParser();
		~CSvgParser();

		bool LoadFromFile(const std::wstring& wsFile, CSvgStorage* pStorage);
		bool LoadFromString(const std::wstring& wsContente, CSvgStorage* pStorage);

		void Clear();
	private:

		bool ReadElement(XmlUtils::CXmlNode& oElement, CObjectBase *pParent = NULL);

		CSvgStorage *m_pStorage;

		double m_dX;
		double m_dY;
		double m_dWidth;
		double m_dHeigh;
	};
}

#endif // CSVGPARSER_H
