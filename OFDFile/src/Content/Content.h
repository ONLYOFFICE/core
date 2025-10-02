#ifndef CONTENT_H
#define CONTENT_H

#include "Layer.h"
#include "../Types/CommonData.h"

namespace OFD
{
class CContent
{
	std::vector<CLayer*> m_arLayers;
public:
	CContent();
	~CContent();

	bool Read(CXmlReader& oLiteReader);
	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
};
}

#endif // CONTENT_H
