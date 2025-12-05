#ifndef PAGEAREA_H
#define PAGEAREA_H

#include "../Utils/XmlReader.h"
#include "../Utils/Types.h"

namespace OFD
{
class CPageArea
{
	TBox m_oPhysicalBox;
	TBox m_oApplicationBox;
	TBox m_oContentBox;
	TBox m_oBleedBox;
public:
	CPageArea();

	bool Read(CXmlReader& oLiteReader);

	TBox GetPhysicalBox() const;
};
}
#endif // PAGEAREA_H
