#ifndef FONT_H
#define FONT_H

#include "../IOFDElement.h"

namespace OFD
{
class CFont : public IOFDElement
{
public:
	CFont(CXmlReader& oXmlReader);
private:
	std::wstring m_wsFontName;
	std::wstring m_wsFamilyName;
	std::wstring m_wsCharset;
	bool         m_bItalic;
	bool         m_bBold;
	bool         m_bSerif;
	bool         m_bFixedWidth;

	std::wstring m_wsFileName;
};
}

#endif // FONT_H
