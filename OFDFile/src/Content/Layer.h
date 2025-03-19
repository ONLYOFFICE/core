#ifndef LAYER_H
#define LAYER_H

#include "IPageBlock.h"

namespace OFD
{
class CLayer
{
	enum class EType
	{
		Body,
		Foreground,
		Background
	} m_eType;

	unsigned int m_unID;
	std::vector<IPageBlock*> m_arPageBlocks;
public:
	CLayer(CXmlReader& oLiteReader);
	~CLayer();

	bool Read(CXmlReader& oLiteReader);
};
}

#endif // LAYER_H
