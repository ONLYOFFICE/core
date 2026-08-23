/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

	class text_format_properties;
	typedef boost::shared_ptr<text_format_properties> text_format_properties_ptr;
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
	void set_rotate			(int Val){rotate_	= Val;}

	void add_dLbl			(int ind, odf_reader::text_format_properties_ptr text_properties);
	void set_common_dLbl	( odf_reader::text_format_properties_ptr text_properties);

	void set_general_dLbls_status( bool flag );

	bool get_general_dLbls_status() const;

	void setLineChart( bool flag );

	bool getLineChart() const;

	std::map<int, odf_reader::text_format_properties_ptr> dLbls_;
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

	bool set_genereal_dLbls = false;

	int position_;
	_CP_OPT(int) rotate_;

	bool IsLineChart = false;

	odf_reader::text_format_properties_ptr textPr_;
};

}
}
