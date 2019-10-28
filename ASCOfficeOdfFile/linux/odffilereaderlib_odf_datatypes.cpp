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
#include "../src/odf/datatypes/anchortype.cpp"
#include "../src/odf/datatypes/backgroundcolor.cpp"
#include "../src/odf/datatypes/bool.cpp"
#include "../src/odf/datatypes/bordermodel.cpp"
#include "../src/odf/datatypes/borderstyle.cpp"
#include "../src/odf/datatypes/borderwidths.cpp"
#include "../src/odf/datatypes/calcext_type.cpp"
#include "../src/odf/datatypes/chartdatalabelnumber.cpp"
#include "../src/odf/datatypes/charterrorcategory.cpp"
#include "../src/odf/datatypes/chartinterpolation.cpp"
#include "../src/odf/datatypes/chartlabelarrangement.cpp"
#include "../src/odf/datatypes/chartregressiontype.cpp"
#include "../src/odf/datatypes/chartseriessource.cpp"
#include "../src/odf/datatypes/chartsolidtype.cpp"
#include "../src/odf/datatypes/chartsymbol.cpp"
#include "../src/odf/datatypes/clockvalue.cpp"
#include "../src/odf/datatypes/color.cpp"
#include "../src/odf/datatypes/common_attlists.cpp"
#include "../src/odf/datatypes/direction.cpp"
#include "../src/odf/datatypes/drawfill.cpp"
#include "../src/odf/datatypes/dropcaplength.cpp"
#include "../src/odf/datatypes/fillimagerefpoint.cpp"
#include "../src/odf/datatypes/fobreak.cpp"
#include "../src/odf/datatypes/fontfamilygeneric.cpp"
#include "../src/odf/datatypes/fontpitch.cpp"
#include "../src/odf/datatypes/fontrelief.cpp"
#include "../src/odf/datatypes/fontsize.cpp"
#include "../src/odf/datatypes/fontstretch.cpp"
#include "../src/odf/datatypes/fontstyle.cpp"
#include "../src/odf/datatypes/fontvariant.cpp"
#include "../src/odf/datatypes/fontweight.cpp"
#include "../src/odf/datatypes/gradientstyle.cpp"
#include "../src/odf/datatypes/hatchstyle.cpp"
#include "../src/odf/datatypes/hyphenationkeep.cpp"
#include "../src/odf/datatypes/hyphenationladdercount.cpp"
#include "../src/odf/datatypes/iconset_type.cpp"
#include "../src/odf/datatypes/keeptogether.cpp"
#include "../src/odf/datatypes/layoutgridmode.cpp"
#include "../src/odf/datatypes/length.cpp"
#include "../src/odf/datatypes/lengthorpercent.cpp"
#include "../src/odf/datatypes/letterspacing.cpp"
#include "../src/odf/datatypes/linebreak.cpp"
#include "../src/odf/datatypes/linemode.cpp"
#include "../src/odf/datatypes/linestyle.cpp"
#include "../src/odf/datatypes/linetype.cpp"
#include "../src/odf/datatypes/linewidth.cpp"
#include "../src/odf/datatypes/markerstyle.cpp"
#include "../src/odf/datatypes/noteclass.cpp"
#include "../src/odf/datatypes/officevaluetype.cpp"
#include "../src/odf/datatypes/pageusage.cpp"
#include "../src/odf/datatypes/percent.cpp"
#include "../src/odf/datatypes/percentorscale.cpp"
#include "../src/odf/datatypes/presentationclass.cpp"
#include "../src/odf/datatypes/punctuationwrap.cpp"
#include "../src/odf/datatypes/rotationalign.cpp"
#include "../src/odf/datatypes/runthrough.cpp"
#include "../src/odf/datatypes/scripttype.cpp"
#include "../src/odf/datatypes/shadowtype.cpp"
#include "../src/odf/datatypes/smil_transitiontype.cpp"
#include "../src/odf/datatypes/stylefamily.cpp"
#include "../src/odf/datatypes/stylehorizontalpos.cpp"
#include "../src/odf/datatypes/stylehorizontalrel.cpp"
#include "../src/odf/datatypes/styleleadercolor.cpp"
#include "../src/odf/datatypes/styleposition.cpp"
#include "../src/odf/datatypes/style_ref.cpp"
#include "../src/odf/datatypes/stylerepeat.cpp"
#include "../src/odf/datatypes/styletype.cpp"
#include "../src/odf/datatypes/styleverticalpos.cpp"
#include "../src/odf/datatypes/styleverticalrel.cpp"
#include "../src/odf/datatypes/stylewrap.cpp"
#include "../src/odf/datatypes/stylewrapcontourmode.cpp"
#include "../src/odf/datatypes/tablealign.cpp"
#include "../src/odf/datatypes/tablecentering.cpp"
#include "../src/odf/datatypes/tablemode.cpp"
#include "../src/odf/datatypes/tablevisibility.cpp"
#include "../src/odf/datatypes/targetframename.cpp"
#include "../src/odf/datatypes/textalign.cpp"
#include "../src/odf/datatypes/textalignsource.cpp"
#include "../src/odf/datatypes/textautospace.cpp"
#include "../src/odf/datatypes/textcombine.cpp"
#include "../src/odf/datatypes/textdisplay.cpp"
#include "../src/odf/datatypes/textemphasize.cpp"
#include "../src/odf/datatypes/textposition.cpp"
#include "../src/odf/datatypes/textrotationscale.cpp"
#include "../src/odf/datatypes/texttransform.cpp"
#include "../src/odf/datatypes/underlinecolor.cpp"
#include "../src/odf/datatypes/verticalalign.cpp"
#include "../src/odf/datatypes/wrapoption.cpp"
#include "../src/odf/datatypes/writingmode.cpp"
#include "../src/odf/datatypes/xlink.cpp"
#include "../src/odf/datatypes/chartlabelposition.cpp"
#include "../src/odf/datatypes/grandtotal.cpp"
#include "../src/odf/datatypes/membertype.cpp"
#include "../src/odf/datatypes/tabletype.cpp"
#include "../src/odf/datatypes/tableorientation.cpp"
#include "../src/odf/datatypes/tablefunction.cpp"
#include "../src/odf/datatypes/tableorder.cpp"
#include "../src/odf/datatypes/dategroup.cpp"
#include "../src/odf/datatypes/commandtype.cpp"
#include "../src/odf/datatypes/stylenumformat.cpp"
#include "../src/odf/datatypes/bibliography.cpp"
#include "../src/odf/datatypes/styleprint.cpp"
