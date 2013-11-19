#include "Utils.h"

#include "../XML/XmlUtils.h"

void Common::readAllShapeTypes(const OOX::CPath& oPath, CSimpleArray<CString>& aShapetypes)
{
	XmlUtils::CXmlLiteReader oReader;

	if ( !oReader.FromFile( oPath.GetPath() ) )
		return;

	while ( FALSE != oReader.ReadNextNode() )
	{
		CString sName = oReader.GetName();
		if(_T("v:shapetype") == sName)
		{
			CString sXml = oReader.GetOuterXml();
			if(false == sXml.IsEmpty())
				aShapetypes.Add(sXml);
		}
	}
}