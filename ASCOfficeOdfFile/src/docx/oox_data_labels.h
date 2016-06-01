#pragma once 

#include <iosfwd>
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/xml/attributes.h>
            
namespace cpdoccore {
namespace oox {

class oox_data_labels
{
public:
	oox_data_labels();
	~oox_data_labels(){}
 
    void oox_serialize(std::wostream & _Wostream);

	void set_showBubbleSize	(bool Val){showBubbleSize_	= Val;}
	void set_showCatName	(bool Val){showCatName_		= Val;}
	void set_showLeaderLines(bool Val){showLeaderLines_ = Val;}
	void set_showLegendKey	(bool Val){showLegendKey_	= Val;}
	void set_showPercent	(bool Val){showPercent_		= Val;}
	void set_showSerName	(bool Val){showSerName_		= Val;}
	void set_showVal		(bool Val){showVal_			= Val;}

	void add_dLbl(int ind, std::vector<odf_reader::_property> & text_properties);
	void set_common_dLbl ( std::vector<odf_reader::_property> & text_properties);
   
private:

	bool showBubbleSize_;		// (Show Bubble Size) §21.2.2.178
	bool showCatName_;			// (Show Category Name) §21.2.2.179
	bool showLeaderLines_;		// (Show Leader Lines) §21.2.2.183
	bool showLegendKey_;		// (Show Legend Key) §21.2.2.184
	bool showPercent_;			// (Show Percent) §21.2.2.187
	bool showSerName_;			// (Show Series Name) §21.2.2.188
	bool showVal_;				// (Show Value) §21.2.2.189

	std::vector<odf_reader::_property>					textPr_;
	std::map<int, std::vector<odf_reader::_property>>	dLbls_;


//delete (Delete) §21.2.2.40
//dLblPos (Data Label Position) §21.2.2.48
//extLst (Chart Extensibility) §21.2.2.64
//leaderLines (Leader Lines) §21.2.2.92
//numFmt (Number Format) §21.2.2.121
//separator (Separator) §21.2.2.166
//spPr (Shape Properties) §21.2.2.197
//txPr (Text Properties) §21.2.2.216
};

}
}
