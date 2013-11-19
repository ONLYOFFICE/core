#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPScopedPtr.h>

            
namespace cpdoccore {
namespace oox {

class oox_data_labels: boost::noncopyable
{
public:
	oox_data_labels();
	~oox_data_labels(){}
 
    void oox_serialize(std::wostream & _Wostream);

	void set_showBubbleSize(bool Val){showBubbleSize_ = Val;}
	void set_showCatName(bool Val){showCatName_ = Val;}
	void set_showLeaderLines(bool Val){showLeaderLines_ = Val;}
	void set_showLegendKey(bool Val){showLegendKey_ = Val;}
	void set_showPercent(bool Val){showPercent_ = Val;}
	void set_showSerName(bool Val){showSerName_ = Val;}
	void set_showVal(bool Val){showVal_ = Val;}

   
private:

	bool showBubbleSize_;// (Show Bubble Size) §21.2.2.178
	bool showCatName_;// (Show Category Name) §21.2.2.179
	bool showLeaderLines_;// (Show Leader Lines) §21.2.2.183
	bool showLegendKey_;// (Show Legend Key) §21.2.2.184
	bool showPercent_;// (Show Percent) §21.2.2.187
	bool showSerName_;// (Show Series Name) §21.2.2.188
	bool showVal_;// (Show Value) §21.2.2.189


//delete (Delete) §21.2.2.40
//dLbl (Data Label) §21.2.2.47 - точно такая же еще раз вложенная хрень
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
