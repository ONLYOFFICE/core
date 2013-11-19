#include "SlideLayoutType.h"

namespace PPTX
{
	namespace Limit
	{
		SlideLayoutType::SlideLayoutType()
		{
			add("blank");
			add("chart");
			add("chartAndTx");
			add("clipArtAndTx");
			add("clipArtAndVertTx");
			add("cust");
			add("dgm");
			add("fourObj");
			add("mediaAndTx");
			add("obj");
			add("objAndTwoObj");
			add("objAndTx");
			add("objOnly");
			add("objOverTx");
			add("objTx");
			add("picTx");
			add("secHead");
			add("tbl");
			add("title");
			add("titleOnly");
			add("twoColTx");
			add("twoObj");
			add("twoObjAndObj");
			add("twoObjAndTx");
			add("twoObjOverTx");
			add("twoTxTwoObj");
			add("tx");
			add("txAndChart");
			add("txAndClipArt");
			add("txAndMedia");
			add("txAndObj");
			add("txAndTwoObj");
			add("txOverObj");
			add("vertTitleAndTx");
			add("vertTitleAndTxOverChart");
			add("vertTx");
		}

		const std::string SlideLayoutType::no_find() const
		{
			return "blank";
		}
	} // namespace Limit
} // namespace PPTX