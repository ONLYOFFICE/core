#ifndef IPAGEBLOCK_H
#define IPAGEBLOCK_H

#include "../Utils/XmlReader.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

namespace OFD
{
class IPageBlock
{
	unsigned int m_unID;
public:
	IPageBlock(CXmlReader& oLiteReader)
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
	virtual ~IPageBlock(){};
	virtual bool Read(CXmlReader& oLiteReader) = 0;
	virtual void Draw(IRenderer* pRenderer) const = 0;
};
}

#endif // IPAGEBLOCK_H
