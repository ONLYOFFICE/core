
#include "BopPop.h"
#include "BopPopCustom.h"

namespace XLS
{

BopPop::BopPop()
{
}


BopPop::~BopPop()
{
}


BaseObjectPtr BopPop::clone()
{
	return BaseObjectPtr(new BopPop(*this));
}


void BopPop::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 0, fHasShadow);
	record << pst << fAutoSplit << split << iSplitPos << pcSplitPercent << pcPie2Size << pcGap << numSplitValue << flags;
}


void BopPop::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> pst >> fAutoSplit >> split >> iSplitPos >> pcSplitPercent >> pcPie2Size >> pcGap >> numSplitValue >> flags;
	fHasShadow = GETBIT(flags, 0);
}

int BopPop::serialize(std::wostream & _stream)
{
 	CP_XML_WRITER(_stream)    
	{
		CP_XML_NODE(L"c:ofPieType")
		{
			if (pst == (unsigned char)1)	CP_XML_ATTR (L"val" , L"pie");
			else							CP_XML_ATTR (L"val" , L"bar");
		}
		CP_XML_NODE(L"c:gapWidth")
		{
			CP_XML_ATTR (L"val", pcGap);
		}
		CP_XML_NODE(L"c:splitType")
		{
			switch(split)
			{
			case 0: CP_XML_ATTR (L"val" , L"pos");		break;
			case 1: CP_XML_ATTR (L"val" , L"val");		break;
			case 2: CP_XML_ATTR (L"val" , L"percent");	break;
			case 3: CP_XML_ATTR (L"val" , L"cust");		break;
			default:
					CP_XML_ATTR (L"val" , L"auto");		break;
			}
		}

		if (fAutoSplit == (unsigned char)0)
		{
			CP_XML_NODE(L"c:splitPos")
			{
				CP_XML_ATTR (L"val", iSplitPos);
			}	
		}
		//if (split == 2)
		//	CP_XML_ATTR (L"val", pcSplitPercent);
		//else
		//	CP_XML_ATTR (L"val", numSplitValue);
		
		BopPopCustom * custom = dynamic_cast<BopPopCustom *>(m_Custom.get());
		if (split == (_UINT16)3 && custom)
		{
			CP_XML_NODE(L"c:custSplit")
			{
				for (int i = 0 ; i < custom->rggrbit.pie_indices.size(); i++)
				{
					CP_XML_NODE(L"c:secondPiePt")
					{
						CP_XML_ATTR (L"val", custom->rggrbit.pie_indices[i]);
					}
				}
			}
		}
		CP_XML_NODE(L"c:secondPieSize")
		{
			CP_XML_ATTR (L"val", pcPie2Size);
		}
	}
	return 0;
}
} // namespace XLS

