#ifndef TEMPLATEPAGE_H
#define TEMPLATEPAGE_H

#include "../IOFDElement.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace OFD
{
enum class EZOrder
{
	Body,
	Background
} ;

EZOrder GetZOrderFromString(const std::string& sValue);

class CPage;
class CCommonData;
class CTemplatePage : public IOFDElement
{
	EZOrder m_eZOrder;

	const CPage* m_pPage;
public:
	CTemplatePage(CXmlReader& oXmlReader, const CCommonData& oCommonData, NSFonts::IFontManager* pFontManager);
	~CTemplatePage();

	EZOrder GetZOrder() const;
	const CPage* GetPage() const;
};
}

#endif // TEMPLATEPAGE_H
