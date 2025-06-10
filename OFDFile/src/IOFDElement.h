#ifndef IOFDELEMENT_H
#define IOFDELEMENT_H

#include "Utils/XmlReader.h"

namespace OFD
{
class IOFDElement
{
	unsigned int m_unID;
public:
	IOFDElement(CXmlReader& oLiteReader)
		: m_unID(0)
	{
		if (0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
		{
			do
			{
				if (L"ID" == oLiteReader.GetName())
					m_unID = oLiteReader.GetUInteger(true);
			} while (oLiteReader.MoveToNextAttribute());

			oLiteReader.MoveToElement();
		}
	};
	virtual ~IOFDElement(){};

	unsigned int GetID() const
	{
		return m_unID;
	}
};
}

#endif // IOFDELEMENT_H
