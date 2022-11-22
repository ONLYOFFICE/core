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


#include "ShapeTypeFactory.h"

#include "Shapetypes/PictureFrameType.h"
#include "Shapetypes/LineType.h"
#include "Shapetypes/OvalType.h"
#include "Shapetypes/RectangleType.h"
#include "Shapetypes/RoundedRectangleType.h"
#include "Shapetypes/TextboxType.h"
#include "Shapetypes/DiamondType.h"
#include "Shapetypes/IsoscelesTriangleType.h"
#include "Shapetypes/RightTriangleType.h"
#include "Shapetypes/ParallelogramType.h"
#include "Shapetypes/TrapezoidType.h"
#include "Shapetypes/HexagonType.h"
#include "Shapetypes/OctagonType.h"
#include "Shapetypes/PlusType.h"
#include "Shapetypes/StarType.h"
#include "Shapetypes/CubeType.h"
#include "Shapetypes/ArcType.h"
#include "Shapetypes/PlaqueType.h"
#include "Shapetypes/CanType.h"
#include "Shapetypes/DonutType.h"
#include "Shapetypes/BevelType.h"
#include "Shapetypes/PentagonType.h"
#include "Shapetypes/FoldedCornerlType.h"
#include "Shapetypes/SmileyFaceType.h"
#include "Shapetypes/NoSmokingType.h"
#include "Shapetypes/BlockArcType.h"
#include "Shapetypes/HeartType.h"
#include "Shapetypes/LightningBoltType.h"
#include "Shapetypes/SunType.h"
#include "Shapetypes/MoonType.h"
#include "Shapetypes/ArrowType.h"
#include "Shapetypes/FlowChartType.h"
#include "Shapetypes/BracketsType.h"
#include "Shapetypes/CalloutsType.h"
#include "Shapetypes/Connector.h"
#include "Shapetypes/WordArtText.h"

namespace DocFileFormat
{
	ShapeType* ShapeTypeFactory::NewShapeType (MSOSPT TypeCode)
	{
		ShapeType* pShape = NULL;

		switch (TypeCode)
		{
		case msosptRectangle					:	{	pShape	=	new RectangleType();					}	break;
		case msosptRoundRectangle				:	{	pShape	=	new RoundedRectangleType();				}	break;
		case msosptEllipse						:	{	pShape	=	new OvalType();							}	break;
		case msosptDiamond						:	{	pShape	=	new DiamondType();						}	break;
		case msosptIsocelesTriangle				:	{	pShape	=	new IsoscelesTriangleType();			}	break;
		case msosptRightTriangle				:	{	pShape	=	new RightTriangleType();				}	break;
		case msosptParallelogram				:	{	pShape	=	new ParallelogramType();				}	break;
		case msosptTrapezoid					:	{	pShape	=	new TrapezoidType();					}	break;
		case msosptHexagon						:	{	pShape	=	new HexagonType();						}	break;
		case msosptOctagon						:	{	pShape	=	new OctagonType();						}	break;
		case msosptPlus							:	{	pShape	=	new PlusType();							}	break;
		case msosptStar							:	{	pShape	=	new StarType();							}	break;
		case msosptIrregularSeal1				:	{	pShape	=	new IrregularSeal1();					}	break;
		case msosptIrregularSeal2				:	{	pShape	=	new IrregularSeal2();					}	break;
		case msosptSeal4						:	{	pShape	=	new Seal4();							}	break;
		case msosptSeal8						:	{	pShape	=	new Seal8();							}	break;
		case msosptSeal16						:	{	pShape	=	new Seal16();							}	break;
		case msosptSeal24						:	{	pShape	=	new Seal24();							}	break;
		case msosptSeal32						:	{	pShape	=	new Seal32();							}	break;
		case msosptRibbon2						:	{	pShape	=	new Ribbon2();							}	break;
		case msosptRibbon						:	{	pShape	=	new Ribbon();							}	break;
		case msosptEllipseRibbon2				:	{	pShape	=	new EllipseRibbon2();					}	break;
		case msosptEllipseRibbon				:	{	pShape	=	new EllipseRibbon();					}	break;
		case msosptVerticalScroll				:	{	pShape	=	new VerticalScroll();					}	break;
		case msosptHorizontalScroll				:	{	pShape	=	new HorizontalScroll();					}	break;
		case msosptWave							:	{	pShape	=	new Wave();								}	break;
		case msosptDoubleWave					:	{	pShape	=	new DoubleWave();						}	break;			
		case msosptCube							:	{	pShape	=	new CubeType();							}	break;
		case msosptArc							:	{	pShape	=	new ArcType();							}	break;
		case msosptLine							:	{	pShape	=	new LineType();							}	break;
		case msosptPlaque						:	{	pShape	=	new PlaqueType();						}	break;
		case msosptCan							:	{	pShape	=	new CanType();							}	break;
		case msosptDonut						:	{	pShape	=	new DonutType();						}	break;
		case msosptPictureFrame 				:	{	pShape	=	new PictureFrameType();					}	break;
		case msosptTextBox						:	{	pShape	=	new TextboxType();						}	break;
		case msosptBevel						:	{	pShape	=	new BevelType();						}	break;			
		case msosptPentagon						:	{	pShape	=	new PentagonType();						}	break;
		case msosptFoldedCorner 				:	{	pShape	=	new FoldedCornerlType();				}	break;
		case msosptSmileyFace					:	{	pShape	=	new SmileyFaceType ();					}	break;
		case msosptNoSmoking					:	{	pShape	=	new NoSmokingType ();					}	break;
		case msosptHeart						:	{	pShape	=	new HeartType ();						}	break;
		case msosptLightningBolt				:	{	pShape	=	new LightningBoltType ();				}	break;
		case msosptSun							:	{	pShape	=	new SunType ();							}	break;
		case msosptMoon							:	{	pShape	=	new MoonType ();						}	break;
		case msosptDownArrow					:	{	pShape	=	new DownArrowType ();					}	break;
		case msosptUpArrow						:	{	pShape	=	new UpArrowType ();						}	break;
		case msosptLeftArrow					:	{	pShape	=	new LeftArrowType ();					}	break;
		case msosptArrow						:	{	pShape	=	new ArrowType ();						}	break;
		case msosptUpDownArrow					:	{	pShape	=	new UpDownArrow ();						}	break;
		case msosptLeftRightArrow				:	{	pShape	=	new LeftRightArrow ();					}	break;
		case msosptQuadArrow					 :	{	pShape	=	new QuadArrow ();						}	break;
		case msosptLeftRightUpArrow				:	{	pShape	=	new LeftRightUpArrow ();				}	break;
		case msosptBentArrow					:	{	pShape	=	new BentArrow ();						}	break;
		case msosptUturnArrow					:	{	pShape	=	new UturnArrow ();						}	break;
		case msosptLeftUpArrow					:	{	pShape	=	new LeftUpArrow ();						}	break;
		case msosptBentUpArrow					:	{	pShape	=	new BentUpArrow ();						}	break;
		case msosptCurvedRightArrow				:	{	pShape	=	new CurvedRightArrow ();				}	break;
		case msosptCurvedLeftArrow				:	{	pShape	=	new CurvedLeftArrow ();					}	break;
		case msosptCurvedUpArrow				:	{	pShape	=	new CurvedUpArrow ();					}	break;
		case msosptCurvedDownArrow				:	{	pShape	=	new CurvedDownArrow ();					}	break;
		case msosptStripedRightArrow			:	{	pShape	=	new StripedRightArrow ();				}	break;
		case msosptNotchedRightArrow			:	{	pShape	=	new NotchedRightArrow ();				}	break;
		case msosptHomePlate					:	{	pShape	=	new HomePlate ();						}	break;
		case msosptChevron						:	{	pShape	=	new Chevron ();							}	break;
		case msosptRightArrowCallout			:	{	pShape	=	new RightArrowCallout ();				}	break;
		case msosptLeftArrowCallout				:	{	pShape	=	new LeftArrowCallout ();				}	break;
		case msosptUpArrowCallout				:	{	pShape	=	new UpArrowCallout ();					}	break;
		case msosptDownArrowCallout				:	{	pShape	=	new DownArrowCallout ();				}	break;
		case msosptLeftRightArrowCallout		:	{	pShape	=	new LeftRightArrowCallout ();			}	break;
		case msosptUpDownArrowCallout			:	{	pShape	=	new UpDownArrowCallout ();				}	break;
		case msosptQuadArrowCallout				:	{	pShape	=	new QuadArrowCallout ();				}	break;
		case msosptCircularArrow				:	{	pShape	=	new CircularArrow ();					}	break;
		case msosptRightBracket					:	{	pShape	=	new RightBracketType ();				}	break;
		case msosptLeftBracket					:	{	pShape	=	new LeftBracketType ();					}	break;
		case msosptRightBrace					:	{	pShape	=	new RightBraceType ();					}	break;
		case msosptLeftBrace					:	{	pShape	=	new LeftBraceType ();					}	break;
		case msosptBracePair					:	{	pShape	=	new BracePairType ();					}	break;	
		case msosptBracketPair					:	{	pShape	=	new BracketPairType ();					}	break;		
		case msosptBlockArc						:	{	pShape	=	new BlockArcType ();					}	break;
		case msosptFlowChartProcess				:	{	pShape	=	new FlowChartProcess ();				}	break;
		case msosptFlowChartAlternateProcess	:	{	pShape	=	new FlowChartAlternateProcess ();		}	break;
		case msosptFlowChartDecision			:	{	pShape	=	new FlowChartDecision ();				}	break;
		case msosptFlowChartInputOutput			:	{	pShape	=	new FlowChartInputOutput ();			}	break;
		case msosptFlowChartPredefinedProcess	:	{	pShape	=	new FlowChartPredefinedProcess ();		}	break;
		case msosptFlowChartInternalStorage		:	{	pShape	=	new FlowChartInternalStorage ();		}	break;
		case msosptFlowChartDocument			:	{	pShape	=	new FlowChartDocument ();				}	break;
		case msosptFlowChartMultidocument		:	{	pShape	=	new FlowChartMultidocument ();			}	break;
		case msosptFlowChartTerminator			:	{	pShape	=	new FlowChartTerminator ();				}	break;
		case msosptFlowChartPreparation			:	{	pShape	=	new FlowChartPreparation();				}	break;
		case msosptFlowChartManualInput			:	{	pShape	=	new FlowChartManualInput();				}	break;
		case msosptFlowChartManualOperation		:	{	pShape	=	new FlowChartManualOperation();			}	break;
		case msosptFlowChartConnector			:	{	pShape	=	new FlowChartConnector();				}	break;
		case msosptFlowChartOffpageConnector	:	{	pShape	=	new FlowChartOffpageConnector();		}	break;
		case msosptFlowChartPunchedCard			:	{	pShape	=	new FlowChartPunchedCard();				}	break;
		case msosptFlowChartPunchedTape			:	{	pShape	=	new FlowChartPunchedTape();				}	break;
		case msosptFlowChartSummingJunction		:	{	pShape	=	new	FlowChartSummingJunction();			}	break;
		case msosptFlowChartOr					:	{	pShape	=	new FlowChartOr();						}	break;
		case msosptFlowChartCollate				:	{	pShape	=	new FlowChartCollate();					}	break;
		case msosptFlowChartSort				:	{	pShape	=	new FlowChartSort();					}	break;
		case msosptFlowChartExtract				:	{	pShape	=	new FlowChartExtract();					}	break;
		case msosptFlowChartMerge				:	{	pShape	=	new FlowChartMerge ();					}	break;
		case msosptFlowChartOnlineStorage		:	{	pShape	=	new FlowChartOnlineStorage();			}	break;
		case msosptFlowChartDelay				:	{	pShape	=	new FlowChartDelay ();					}	break;
		case msosptFlowChartMagneticTape		:	{	pShape	=	new FlowChartMagneticTape();			}	break;
		case msosptFlowChartMagneticDisk		:	{	pShape	=	new FlowChartMagneticDisk();			}	break;
		case msosptFlowChartMagneticDrum		:	{	pShape	=	new FlowChartMagneticDrum();			}	break;
		case msosptFlowChartDisplay				:	{	pShape	=	new FlowChartDisplay();					}	break;
		case msosptWedgeRectCallout				:	{	pShape	=	new WedgeRectCallout();					}	break;
		case msosptWedgeRRectCallout			:	{	pShape	=	new WedgeRRectCallout();				}	break;
		case msosptWedgeEllipseCallout			:	{	pShape	=	new WedgeEllipseCallout();				}	break;
		case msosptCloudCallout					:	{	pShape	=	new CloudCallout();						}	break;		
		case msosptBorderCallout90				:	{	pShape	=	new BorderCallout90();					}	break;
		case msosptBorderCallout1				:	{	pShape	=	new BorderCallout1();					}	break;
		case msosptBorderCallout2				:	{	pShape	=	new BorderCallout2();					}	break;
		case msosptBorderCallout3				:	{	pShape	=	new BorderCallout3();					}	break;
		case msosptAccentCallout90				:	{	pShape	=	new AccentCallout90();					}	break;
		case msosptAccentCallout1				:	{	pShape	=	new AccentCallout1();					}	break;
		case msosptAccentCallout2				:	{	pShape	=	new AccentCallout2();					}	break;
		case msosptAccentCallout3				:	{	pShape	=	new AccentCallout3();					}	break;
		case msosptCallout90					:	{	pShape	=	new Callout90();						}	break;
		case msosptCallout1						:	{	pShape	=	new Callout1();							}	break;
		case msosptCallout2						:	{	pShape	=	new Callout2();							}	break;
		case msosptCallout3						:	{	pShape	=	new Callout3();							}	break;
		case msosptAccentBorderCallout90		:	{	pShape	=	new AccentBorderCallout90();			}	break;
		case msosptAccentBorderCallout1			:	{	pShape	=	new AccentBorderCallout1();				}	break;
		case msosptAccentBorderCallout2			:	{	pShape	=	new AccentBorderCallout2();				}	break;
		case msosptAccentBorderCallout3			:	{	pShape	=	new AccentBorderCallout3();				}	break;	
		case msosptBentConnector2				:	{	pShape	=	new BentConnector2();					}	break;		
		case msosptBentConnector3				:	{	pShape	=	new BentConnector3();					}	break;		
		case msosptBentConnector4				:	{	pShape	=	new BentConnector4();					}	break;		
		case msosptBentConnector5				:	{	pShape	=	new BentConnector5();					}	break;	
		case msosptCurvedConnector2				:	{	pShape	=	new CurvedConnector2();					}	break;		
		case msosptCurvedConnector3				:	{	pShape	=	new CurvedConnector3();					}	break;		
		case msosptCurvedConnector4				:	{	pShape	=	new CurvedConnector4();					}	break;		
		case msosptCurvedConnector5				:	{	pShape	=	new CurvedConnector5();					}	break;		
//wordArt		
		
		case msosptTextStop						:	{	pShape	=	new WordArtTextStop();					}	break;
		case msosptTextArchUpCurve				:	{	pShape	=	new WordArtTextArchUpCurve();			}	break;
		case msosptTextSlantUp					:	{	pShape	=	new WordArtTextSlantUp();				}	break;
		case msosptTextDeflate					:	{	pShape	=	new WordArtTextDeflate();				}	break;
		case msosptTextCanDown					:	{	pShape	=	new WordArtTextCanDown();				}	break;
		case msosptTextWave1					:	{	pShape	=	new WordArtTextWave1();					}	break;
		case msosptTextWave3					:	{	pShape	=	new WordArtTextWave3();					}	break;
		case msosptTextWave4					:	{	pShape	=	new WordArtTextWave4();					}	break;
		case msosptTextCurveUp					:	{	pShape	=	new WordArtTextCurveUp();				}	break;
		case msosptTextFadeUp					:	{	pShape	=	new WordArtTextFadeUp();				}	break;
		case msosptTextTriangle					:	{	pShape	=	new WordArtTextTriangle();				}	break;
		case msosptTextCascadeUp				:	{	pShape	=	new WordArtTextCascadeUp();				}	break;
		case msosptTextDeflateBottom			:	{	pShape	=	new WordArtTextDeflateBottom();			}	break;

		case msosptTextTriangleInverted			:
		case msosptTextChevron					:
		case msosptTextChevronInverted			:
		case msosptTextRingInside				:
		case msosptTextRingOutside				:
		case msosptTextArchDownCurve			:
		case msosptTextCircleCurve				:
		case msosptTextButtonCurve				:
		case msosptTextArchUpPour				:
		case msosptTextArchDownPour				:
		case msosptTextCirclePour				:
		case msosptTextButtonPour				:
		case msosptTextCurveDown				:
		case msosptTextCascadeDown				:
		case msosptTextWave2					:
		case msosptTextInflate					:
		case msosptTextInflateBottom			:
		case msosptTextInflateTop				:
		case msosptTextDeflateTop				:
		case msosptTextDeflateInflate			:
		case msosptTextDeflateInflateDeflate	:
		case msosptTextFadeRight				:
		case msosptTextFadeLeft					:
		case msosptTextFadeDown					:
		case msosptTextSlantDown				:
		case msosptTextCanUp					:
		case msosptTextPlainText				:	
													{	pShape	=	new WordArtTextType(TypeCode);			}	break;

		default									:	{	pShape	=	NULL;									}	break;
		}

		return pShape;
	}
}
