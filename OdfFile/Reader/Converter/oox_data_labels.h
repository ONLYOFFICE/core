/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once 

#include <iosfwd>
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>

#include <CPOptional.h>
#include <CPScopedPtr.h>
#include <xml/attributes.h>
            
namespace cpdoccore {

namespace odf_reader {

	class text_format_properties_content;
	typedef boost::shared_ptr<text_format_properties_content> text_format_properties_content_ptr;
}

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
	
	void set_formatCode		(std::wstring & formatCode, bool linkData)
	{
		formatCode_ = formatCode;
		linkData_ = linkData;
	}

	void set_position		(int Val){position_	= Val;}

	void add_dLbl			(int ind, odf_reader::text_format_properties_content_ptr text_properties);
	void set_common_dLbl	( odf_reader::text_format_properties_content_ptr text_properties);
   
private:

	std::wstring formatCode_;
	bool linkData_;
	bool showBubbleSize_;		// (Show Bubble Size) §21.2.2.178
	bool showCatName_;			// (Show Category Name) §21.2.2.179
	bool showLeaderLines_;		// (Show Leader Lines) §21.2.2.183
	bool showLegendKey_;		// (Show Legend Key) §21.2.2.184
	bool showPercent_;			// (Show Percent) §21.2.2.187
	bool showSerName_;			// (Show Series Name) §21.2.2.188
	bool showVal_;				// (Show Value) §21.2.2.189

	int position_;

	odf_reader::text_format_properties_content_ptr					textPr_;
	std::map<int, odf_reader::text_format_properties_content_ptr>	dLbls_;


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
