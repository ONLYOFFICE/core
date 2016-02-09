
#include "XFProps.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr XFProps::clone()
{
	return BiffStructurePtr(new XFProps(*this));
}



void XFProps::store(CFRecord& record)
{
#pragma message("####################### XFProps record has no BiffStructure::store() implemented")
	Log::error(" Error!!! XFProps record has no BiffStructure::store() implemented.");
	//record << something;
}


void XFProps::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	record >> cprops;
	
	for(int i = 0; i < cprops; ++i)
	{
		XFProp prop;
		record >> prop;

		if		(prop.xfPropType >=  0 && prop.xfPropType <=  3)							arXFPropFill.push_back(prop);
		else if (prop.xfPropType == 4)														arXFPropGradient.push_back(prop);
		else if (prop.xfPropType >=  6 && prop.xfPropType <= 14)							arXFPropBorder.push_back(prop);
		else if (prop.xfPropType >= 15 && prop.xfPropType <= 22 || prop.xfPropType == 42)	arXFPropAlignment.push_back(prop);
		else if	(prop.xfPropType >= 24 && prop.xfPropType <= 37 || prop.xfPropType ==  5)	arXFPropFont.push_back(prop);
		else if (prop.xfPropType >= 38 && prop.xfPropType <= 41)							arXFPropNumFmt.push_back(prop);	
		else if (prop.xfPropType >= 43 && prop.xfPropType <= 44)							arXFPropProtection.push_back(prop);		
	}
}
int XFProps::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		if (arXFPropFont.size() > 0)
		{
			CP_XML_NODE(L"font")
			{	
				for (int i = 0; i < arXFPropFont.size(); i++)
				{
					arXFPropFont[i].serialize(CP_XML_STREAM());
				}
			}
		}
		if (arXFPropNumFmt.size() > 0)
		{
			CP_XML_NODE(L"numFmt")
			{	
				for (int i = 0; i < arXFPropNumFmt.size(); i++)
				{
					arXFPropNumFmt[i].serialize(CP_XML_STREAM());
				}
			}
		}
		if (arXFPropFill.size() > 0)
		{
			XFProp pPatternType;
			XFPropGradient *pGradient = NULL;

			for (int i = 0; i < arXFPropFill.size(); i++)
			{
				switch(arXFPropFill[i].xfPropType)
				{
				case 0: pPatternType	= arXFPropFill[i];	break;
				case 3: pGradient		= dynamic_cast<XFPropGradient*>(arXFPropFill[i].xfPropDataBlob.get());		break;
				}
			}
			CP_XML_NODE(L"fill")
			{	
				if (pGradient || arXFPropGradient.size()>0)
				{
					CP_XML_NODE(L"gradientFill")
					{
						if (pGradient) 
							pGradient->serialize_attr(CP_GET_XML_NODE());

						for (int i = 0 ; i < arXFPropGradient.size(); i++)
						{
							if (arXFPropGradient[i].xfPropDataBlob == NULL) continue;
							arXFPropGradient[i].xfPropDataBlob->serialize(CP_XML_STREAM());
						}
					}
				}
				else
				{
					CP_XML_NODE(L"patternFill")
					{
						pPatternType.serialize_attr(CP_GET_XML_NODE());
						
						for (int i = 0; i < arXFPropFill.size(); i++)
						{
							arXFPropFill[i].serialize(CP_XML_STREAM());
						}
					}
				}
			}
		}
		if (arXFPropAlignment.size() > 0)
		{
			CP_XML_NODE(L"alignment")
			{	
				for (int i = 0; i < arXFPropAlignment.size(); i++)
				{
					arXFPropAlignment[i].serialize(CP_XML_STREAM());
				}
			}
		}
		if (arXFPropBorder.size() > 0)
		{
			CP_XML_NODE(L"border")
			{	
				for (int i = 0; i < arXFPropBorder.size(); i++)
				{
					arXFPropBorder[i].serialize(CP_XML_STREAM());
				}
			}
		}	
	}
	return 0;
}
} // namespace XLS
