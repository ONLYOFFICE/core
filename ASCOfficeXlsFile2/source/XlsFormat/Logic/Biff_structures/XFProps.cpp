
#include "XFProps.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

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
	arXFPropBorder.is_present = false;

	record.skipNunBytes(2); // reserved
	record >> cprops;
	
	for(int i = 0; i < cprops; ++i)
	{
		XFProp prop;
		record >> prop;

		if	(prop.xfPropType >=  0 && prop.xfPropType <=  3)
		{
			arXFPropFill.push_back(prop);
		}
		else if (prop.xfPropType == 4)		
		{
			arXFPropGradient.push_back(prop);
		}
		else if (prop.xfPropType >=  6 && prop.xfPropType <= 14)
		{
			arXFPropBorder.is_present = true;
			switch(prop.xfPropType)
			{
				case 6: arXFPropBorder.top		= prop; break;
				case 7: arXFPropBorder.bottom	= prop; break;
				case 8: arXFPropBorder.left		= prop; break;
				case 9: arXFPropBorder.right	= prop; break;
				default:
					arXFPropBorder.other.push_back(prop);
			}
		}
		else if (prop.xfPropType >= 15 && prop.xfPropType <= 22 || prop.xfPropType == 42)	
		{
			arXFPropAlignment.push_back(prop);
		}
		else if	(prop.xfPropType >= 24 && prop.xfPropType <= 37 || prop.xfPropType ==  5)	
		{
			arXFPropFont.push_back(prop);
		}
		else if (prop.xfPropType >= 38 && prop.xfPropType <= 41)
		{
			bool skip_codes = false;
			if (prop.xfPropType == 0x0029)
			{
				BIFF_WORD* word = dynamic_cast<BIFF_WORD*>(prop.xfPropDataBlob.get());
				if ((word) && (word->value() == (_UINT16)30))
				{
					skip_codes = true;
				}

			}
			if (!skip_codes)
				arXFPropNumFmt.push_back(prop);	
		}
		else if (prop.xfPropType >= 43 && prop.xfPropType <= 44)	
		{
			arXFPropProtection.push_back(prop);		
		}
		else
		{
		}
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
		if (arXFPropBorder.is_present)
		{
			CP_XML_NODE(L"border")
			{	
				//порядок важен - xfPropType - 8, 9, 6, 7 ( - DataVal_Headings.xls
				if (arXFPropBorder.left)	arXFPropBorder.left->serialize(CP_XML_STREAM());
				if (arXFPropBorder.right)	arXFPropBorder.right->serialize(CP_XML_STREAM());
				if (arXFPropBorder.top)		arXFPropBorder.top->serialize(CP_XML_STREAM());
				if (arXFPropBorder.bottom)	arXFPropBorder.bottom->serialize(CP_XML_STREAM());

				//----------------------------------------
				for (int i = 0; i < arXFPropBorder.other.size(); i++)
				{
					arXFPropBorder.other[i].serialize(CP_XML_STREAM());
				}
			}
		}	
	}
	return 0;
}
} // namespace XLS
