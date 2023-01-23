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

#include "../ShapeType.h"

namespace DocFileFormat
{
	class FlowChartProcess : public ShapeType
	{
	public:
		FlowChartProcess () : ShapeType(msosptFlowChartProcess)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m,l,21600r21600,l21600,xe";

            ConnectorLocations	=	L"Rectangle";
		}
	};

	class FlowChartAlternateProcess : public ShapeType
	{
	public:
		FlowChartAlternateProcess () : ShapeType(msosptFlowChartAlternateProcess)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m@0,qx0@0l0@2qy@0,21600l@1,21600qx21600@2l21600@0qy@1,xe";

            Formulas.push_back(L"val #0");
            Formulas.push_back(L"sum width 0 #0");
            Formulas.push_back(L"sum height 0 #0");
            Formulas.push_back(L"prod @0 2929 10000");
            Formulas.push_back(L"sum width 0 @3");
            Formulas.push_back(L"sum height 0 @3");
            Formulas.push_back(L"val width");
            Formulas.push_back(L"val height");
            Formulas.push_back(L"prod width 1 2");
            Formulas.push_back(L"prod height 1 2");

            AdjustmentValues	=	L"2700";
            ConnectorLocations	=	L"@8,0;0,@9;@8,@7;@6,@9";
            TextBoxRectangle	=	L"@3,@3,@4,@5";
            Limo				=	L"10800,10800";
		}
	};

	class FlowChartDecision : public ShapeType
	{
	public:
		FlowChartDecision () : ShapeType(msosptFlowChartDecision)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m10800,l,10800,10800,21600,21600,10800xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"5400,5400,16200,16200";
		}
	};

	class FlowChartInputOutput : public ShapeType
	{
	public:
		FlowChartInputOutput () : ShapeType(msosptFlowChartInputOutput)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m4321,l21600,,17204,21600,,21600xe";

            ConnectorLocations	=	L"12961,0;10800,0;2161,10800;8602,21600;10800,21600;19402,10800";
            TextBoxRectangle	=	L"4321,0,17204,21600";
		}
	};

	class FlowChartPredefinedProcess : public ShapeType
	{
	public:
		FlowChartPredefinedProcess () : ShapeType(msosptFlowChartPredefinedProcess)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m,l,21600r21600,l21600,xem2610,nfl2610,21600em18990,nfl18990,21600e";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"2610,0,18990,21600";
		}
	};

	class FlowChartInternalStorage : public ShapeType
	{
	public:
		FlowChartInternalStorage () : ShapeType(msosptFlowChartInternalStorage)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;

            Path				=	L"m,l,21600r21600,l21600,xem4236,nfl4236,21600em,4236nfl21600,4236e";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"4236,4236,21600,21600";
		}
	};

	class FlowChartDocument : public ShapeType
	{
	public:
		FlowChartDocument () : ShapeType(msosptFlowChartDocument)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,20172v945,400,1887,628,2795,913c3587,21312,4342,21370,5060,21597v2037,,2567,-227,3095,-285c8722,21197,9325,20970,9855,20800v490,-228,945,-400,1472,-740c11817,19887,12347,19660,12875,19375v567,-228,1095,-513,1700,-740c15177,18462,15782,18122,16537,17950v718,-113,1398,-398,2228,-513c19635,17437,20577,17322,21597,17322l21597,,,xe";

            AdjustmentValues	=	L"21600,21600";
            ConnectorLocations	=	L"10800,0;0,10800;10800,20400;21600,10800";
            TextBoxRectangle	=	L"0,0,21600,17322";
		}
	};

	class FlowChartMultidocument : public ShapeType
	{
	public:
		FlowChartMultidocument () : ShapeType(msosptFlowChartMultidocument)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,20465v810,317,1620,452,2397,725c3077,21325,3790,21417,4405,21597v1620,,2202,-180,2657,-272c7580,21280,8002,21010,8455,20917v422,-135,810,-405,1327,-542c10205,20150,10657,19967,11080,19742v517,-182,970,-407,1425,-590c13087,19017,13605,18745,14255,18610v615,-180,1262,-318,1942,-408c16975,18202,17785,18022,18595,18022r,-1670l19192,16252r808,l20000,14467r722,-75l21597,14392,21597,,2972,r,1815l1532,1815r,1860l,3675,,20465xem1532,3675nfl18595,3675r,12677em2972,1815nfl20000,1815r,12652e";

            AdjustmentValues	=	L"21600,21600";
            ConnectorLocations	=	L"10800,0;0,10800;10800,19890;21600,10800";
            TextBoxRectangle	=	L"0,3675,18595,18022";
		}
	};

	class FlowChartTerminator : public ShapeType
	{
	public:
		FlowChartTerminator () : ShapeType(msosptFlowChartTerminator)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m3475,qx,10800,3475,21600l18125,21600qx21600,10800,18125,xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"1018,3163,20582,18437";
		}
	};

	class FlowChartPreparation : public ShapeType
	{
	public:
		FlowChartPreparation () : ShapeType(msosptFlowChartPreparation)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m4353,l17214,r4386,10800l17214,21600r-12861,l,10800xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"4353,0,17214,21600";
		}
	};

	class FlowChartManualInput : public ShapeType
	{
	public:
		FlowChartManualInput () : ShapeType(msosptFlowChartManualInput)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,4292l21600,r,21600l,21600xe";

            ConnectorLocations	=	L"10800,2146;0,10800;10800,21600;21600,10800";
            TextBoxRectangle	=	L"0,4291,21600,21600";
		}
	};

	class FlowChartManualOperation : public ShapeType
	{
	public:
		FlowChartManualOperation () : ShapeType(msosptFlowChartManualOperation)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,l21600,,17240,21600r-12880,xe";

            ConnectorLocations	=	L"10800,0;2180,10800;10800,21600;19420,10800";
            TextBoxRectangle	=	L"4321,0,17204,21600";
		}
	};

	class FlowChartConnector : public ShapeType
	{
	public:
		FlowChartConnector () : ShapeType(msosptFlowChartConnector)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,qx,10800,10800,21600,21600,10800,10800,xe";

            ConnectorLocations	=	L"10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163";
            TextBoxRectangle	=	L"3163,3163,18437,18437";
		}
	};

	class FlowChartOffpageConnector : public ShapeType
	{
	public:
		FlowChartOffpageConnector () : ShapeType(msosptFlowChartOffpageConnector)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,l21600,r,17255l10800,21600,,17255xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"0,0,21600,17255";
		}
	};

	class FlowChartPunchedCard : public ShapeType
	{
	public:
		FlowChartPunchedCard () : ShapeType(msosptFlowChartPunchedCard)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m4321,l21600,r,21600l,21600,,4338xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"0,4321,21600,21600";
		}
	};

	class FlowChartPunchedTape : public ShapeType
	{
	public:
		FlowChartPunchedTape () : ShapeType(msosptFlowChartPunchedTape)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m21597,19450v-225,-558,-750,-1073,-1650,-1545c18897,17605,17585,17347,16197,17260v-1500,87,-2700,345,-3787,645c11472,18377,10910,18892,10800,19450v-188,515,-750,1075,-1613,1460c8100,21210,6825,21425,5400,21597,3937,21425,2700,21210,1612,20910,675,20525,150,19965,,19450l,2147v150,558,675,1073,1612,1460c2700,3950,3937,4165,5400,4337,6825,4165,8100,3950,9187,3607v863,-387,1425,-902,1613,-1460c10910,1632,11472,1072,12410,600,13497,300,14697,85,16197,v1388,85,2700,300,3750,600c20847,1072,21372,1632,21597,2147xe";

            ConnectorLocations	=	L"10800,2147;0,10800;10800,19450;21600,10800";
            TextBoxRectangle	=	L"0,4337,21600,17260";
		}
	};

	class FlowChartSummingJunction : public ShapeType
	{
	public:
		FlowChartSummingJunction () : ShapeType(msosptFlowChartSummingJunction)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,qx,10800,10800,21600,21600,10800,10800,xem3163,3163nfl18437,18437em3163,18437nfl18437,3163e";

            ConnectorLocations	=	L"10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163";
            TextBoxRectangle	=	L"3163,3163,18437,18437";
		}
	};

	class FlowChartOr : public ShapeType
	{
	public:
		FlowChartOr () : ShapeType(msosptFlowChartOr)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,qx,10800,10800,21600,21600,10800,10800,xem,10800nfl21600,10800em10800,nfl10800,21600e";

            ConnectorLocations	=	L"10800,0;3163,3163;0,10800;3163,18437;10800,21600;18437,18437;21600,10800;18437,3163";
            TextBoxRectangle	=	L"3163,3163,18437,18437";
		}
	};

	class FlowChartCollate : public ShapeType
	{
	public:
		FlowChartCollate () : ShapeType(msosptFlowChartCollate)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m21600,21600l,21600,21600,,,xe";

            ConnectorLocations	=	L"10800,0;10800,10800;10800,21600";
            TextBoxRectangle	=	L"5400,5400,16200,16200";
		}
	};

	class FlowChartSort : public ShapeType
	{
	public:
		FlowChartSort () : ShapeType(msosptFlowChartSort)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,l,10800,10800,21600,21600,10800xem,10800nfl21600,10800e";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"5400,5400,16200,16200";
		}
	};

	class FlowChartExtract : public ShapeType
	{
	public:
		FlowChartExtract () : ShapeType(msosptFlowChartExtract)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,l21600,21600,,21600xe";

            ConnectorLocations	=	L"10800,0;5400,10800;10800,21600;16200,10800";
            TextBoxRectangle	=	L"5400,10800,16200,21600";
		}
	};

	class FlowChartMerge : public ShapeType
	{
	public:
		FlowChartMerge () : ShapeType(msosptFlowChartMerge)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m,l21600,,10800,21600xe";

            ConnectorLocations	=	L"10800,0;5400,10800;10800,21600;16200,10800";
            TextBoxRectangle	=	L"5400,0,16200,10800";
		}
	};

	class FlowChartOnlineStorage : public ShapeType
	{
	public:
		FlowChartOnlineStorage () : ShapeType(msosptFlowChartOnlineStorage)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m3600,21597c2662,21202,1837,20075,1087,18440,487,16240,75,13590,,10770,75,8007,487,5412,1087,3045,1837,1465,2662,337,3600,l21597,v-937,337,-1687,1465,-2512,3045c18485,5412,18072,8007,17997,10770v75,2820,488,5470,1088,7670c19910,20075,20660,21202,21597,21597xe";

            ConnectorLocations	=	L"10800,0;0,10800;10800,21600;17997,10800";
            TextBoxRectangle	=	L"3600,0,17997,21600";
		}
	};

	class FlowChartDelay : public ShapeType
	{
	public:
		FlowChartDelay () : ShapeType(msosptFlowChartDelay)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,qx21600,10800,10800,21600l,21600,,xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"0,3163,18437,18437";
		}
	};

	class FlowChartMagneticTape : public ShapeType
	{
	public:
		FlowChartMagneticTape () : ShapeType(msosptFlowChartMagneticTape)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"ar,,21600,21600,18685,18165,10677,21597l20990,21597r,-3432xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"3163,3163,18437,18437";
		}
	};

	class FlowChartMagneticDisk : public ShapeType
	{
	public:
		FlowChartMagneticDisk () : ShapeType(msosptFlowChartMagneticDisk)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m10800,qx,3391l,18209qy10800,21600,21600,18209l21600,3391qy10800,xem,3391nfqy10800,6782,21600,3391e";

            ConnectorLocations	=	L"10800,6782;10800,0;0,10800;10800,21600;21600,10800";
            ConnectorAngles		=	L"270,270,180,90,0";
            TextBoxRectangle	=	L"0,6782,21600,18209";
		}
	};

	class FlowChartMagneticDrum : public ShapeType
	{
	public:
		FlowChartMagneticDrum () : ShapeType(msosptFlowChartMagneticDrum)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m21600,10800qy18019,21600l3581,21600qx,10800,3581,l18019,qx21600,10800xem18019,21600nfqx14438,10800,18019,e";

            ConnectorLocations	=	L"10800,0;0,10800;10800,21600;14438,10800;21600,10800";
            ConnectorAngles		=	L"270,180,90,0,0";
            TextBoxRectangle	=	L"3581,0,14438,21600";
		}
	};

	class FlowChartDisplay : public ShapeType
	{
	public:
		FlowChartDisplay () : ShapeType(msosptFlowChartDisplay)
		{
			ShapeConcentricFill	=	true;
			Joins				=	miter;
            Path				=	L"m17955,v862,282,1877,1410,2477,3045c21035,5357,21372,7895,21597,10827v-225,2763,-562,5300,-1165,7613c19832,20132,18817,21260,17955,21597r-14388,l,10827,3567,xe";

            ConnectorLocations	=	L"Rectangle";
            TextBoxRectangle	=	L"3567,0,17955,21600";
		}
	};
}
