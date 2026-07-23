#ifndef CFONTCHECKER_H
#define CFONTCHECKER_H

#include <vector>

namespace NSFonts { class IApplicationFonts; class IFontManager; }

class IFolder;

namespace OFD
{
class CFont;

class CFontChecker
{
	NSFonts::IFontManager* m_pFontManager;
	IFolder*&              m_pFolder;

	struct TUpdatedFont
	{
		bool   m_bLoadFromMemory;
		CFont* m_pFont;
	};

	std::vector<TUpdatedFont> m_arUpdatedFonts;
public:
	CFontChecker(NSFonts::IApplicationFonts* pApplicationFonts, IFolder*& pFolder);

	void Clear();

	bool UpdateFont(CFont* pFont);

	NSFonts::IApplicationFonts* GetFonts() const;
};
}

#endif // CFONTCHECKER_H
