// libxml2_test.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "../libxml2.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//std::wstring sFileInput = L"C:\\Users\\Oleg\\Desktop\\book1\\word\\document.xml";
	std::wstring sFileInput = L"D:\\123\\word\\document.xml";

#if 0
	XmlUtils::CXmlLiteReader oReader;
	oReader.FromFile(sFileInput);
	oReader.ReadNextNode();

	std::wstring strXml = oReader.GetOuterXml();
#endif

#if 1
	XmlUtils::CXmlNode oNode;
	bool bRes = oNode.FromXmlFile(sFileInput);

	XmlUtils::CXmlNode oNodeBody = oNode.GetNode(L"w:body");
	XmlUtils::CXmlNodes oNodesPar = oNodeBody.GetNodes(L"w:p");

	XmlUtils::CXmlNode oNodePar1;
	if (oNodesPar.GetAt(1, oNodePar1))
	{
		XmlUtils::CXmlNode oNode1 = oNodePar1.GetNode(L"w:pPr");
		XmlUtils::CXmlNode oNode2 = oNode1.GetNode(L"w:spacing");

		std::wstring sAttribute = oNode2.GetAttribute(L"w:after");

		XmlUtils::CXmlNode oNode3 = oNodePar1.GetNode(L"w:r");
		XmlUtils::CXmlNode oNode4 = oNode3.GetNode(L"w:t");

		std::wstring sText = oNode4.GetText();

		int y = 0;
		++y;
	}

	//std::wstring sResultXmlDOM = oNode.private_GetXml();
	std::wstring sResultXmlDOM = oNode.private_GetXmlFast();
#endif

	return 0;
}

