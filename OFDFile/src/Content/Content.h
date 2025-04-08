#ifndef CONTENT_H
#define CONTENT_H

#include "Layer.h"

namespace OFD
{
class CContent
{
	std::vector<CLayer*> m_arLayers;
public:
	CContent();
	~CContent();

	bool Read(CXmlReader& oLiteReader, const CRes* pDocumentRes, const CRes* pPublicRes, NSFonts::IFontManager* pFontManager);
	void Draw(IRenderer* pRenderer) const;
};
}

#endif // CONTENT_H
