#ifndef REGION_H
#define REGION_H

#include "../Utils/XmlReader.h"
#include "../Utils/Types.h"

namespace OFD
{
class IPathElement;

class CArea
{
	TPos m_oStart;
	std::vector<IPathElement*> m_arElements;
public:
	CArea(CXmlReader& oReader);
	~CArea();
private:
	void AddElement(IPathElement* pElement);
};

class CRegion
{
	std::vector<CArea*> m_arAreas;
public:
	CRegion(CXmlReader& oReader);
	~CRegion();
};
}

#endif // REGION_H
