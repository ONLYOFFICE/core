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

	bool Read(CXmlReader& oLiteReader, const CRes* pDocumentRes);
	void Draw(IRenderer* pRenderer, const CRes* pPublicRes) const;
};
}

#endif // CONTENT_H
