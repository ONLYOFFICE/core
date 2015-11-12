#pragma once

namespace NSOfficePPT
{
	enum SlideSizeType
	{
		OnScreen		 = 0,
		LetterSizedPaper = 1,
		A4Paper          = 2,
		Size35mm         = 3,
		Overhead		 = 4,
		Banner			 = 5,
		Custom           = 6
	};

	enum StyleMask
	{
		None					=	   0,

		IsBold					= 1 << 0,
		IsItalic				= 1 << 1,
		IsUnderlined			= 1 << 2,

		HasShadow				= 1 << 4,
		HasAsianSmartQuotes		= 1 << 5,

		HasHorizonNumRendering	= 1 << 7,

		IsEmbossed				= 1 << 9,

		ExtensionNibble			= 0xF << 10 
	};

	enum CharacterMask
	{
		_None						= 0,

		StyleFlagsFieldPresent		= 0xFFFF,

		TypefacePresent				= 1 << 16,
		SizePresent					= 1 << 17,
		ColorPresent				= 1 << 18,
		PositionPresent				= 1 << 19,

		FEOldTypefacePresent		= 1 << 21,
		ANSITypefacePresent			= 1 << 22,
		SymbolTypefacePresent		= 1 << 23
	};

	enum PlaceholderEnum
	{
		__None						= 0,
		MasterTitle					= 1,
		MasterBody					= 2,
		MasterCenteredTitle			= 3,
		MasterSubtitle				= 4,
		MasterNotesSlideImage		= 5,
		MasterNotesBody				= 6,
		MasterDate					= 7,
		MasterSlideNumber			= 8,
		MasterFooter				= 9,
		MasterHeader				= 10,
		NotesSlideImage				= 11,
		NotesBody					= 12,
		Title						= 13,
		Body						= 14,
		CenteredTitle				= 15,
		Subtitle					= 16,
		VerticalTextTitle			= 17,
		VerticalTextBody			= 18,
		Object						= 19,
		Graph						= 20,
		Table						= 21,
		ClipArt						= 22,
		OrganizationChart			= 23,
		MediaClip					= 24
	};

	enum ParagraphMask
	{
		___None						= 0,
		HasBullet					= 1 << 0,
		BulletHasFont				= 1 << 1,
		BulletHasColor				= 1 << 2,
		BulletHasSize				= 1 << 3,
		BulletFlagsFieldExists		= HasBullet | BulletHasFont | BulletHasColor | BulletHasSize,

		BulletFont					= 1 << 4,
		BulletColor					= 1 << 5,
		BulletSize					= 1 << 6,
		BulletChar					= 1 << 7,
		LeftMargin					= 1 << 8,
		Indent						= 1 << 10,
		Align						= 1 << 11,
		LineSpacing					= 1 << 12,
		SpaceBefore					= 1 << 13,
		SpaceAfter					= 1 << 14,
		DefaultTabSize				= 1 << 15,
		FontAlign					= 1 << 16,
		CharWrap					= 1 << 17,
		WordWrap					= 1 << 18,
		Overflow					= 1 << 19,
		WrapFlagsFieldExists		= CharWrap | WordWrap | Overflow,
		TabStops					= 1 << 20,
		TextDirection				= 1 << 21,
		BulletBlip					= 1 << 23,
		BulletScheme				= 1 << 24,
		BulletHasScheme				= 1 << 25
	};

	enum Instances
	{
		CollectionOfSlides			= 0,
		CollectionOfMasterSlides	= 1,
		CollectionOfNotesSlides		= 2
	};

	enum SlideLayoutType
	{
		TitleSlide							= 0,
		TitleAndBody						= 1,
		TitleMaster							= 2,
	    
		MasterNotes							= 4,
		NotesTitleAndBody					= 5,
		Handout								= 6,
		TitleOnly							= 7,
		TwoColumnsAndTitle					= 8,
		TwoRowsAndTitle						= 9,
		TwoColumnsRightTwoRows				= 10,
		TwoColumnsLeftTwoRows				= 11,
		TwoRowsBottomTwoColumns				= 12,
		TwoRowsTopTwoColumns				= 13,
		FourObjects							= 14,
		BigObject							= 15,
		Blank								= 16,
		VerticalTitleRightBodyLeft			= 17,
		VerticalTitleRightBodyLeftTwoRows	= 18
	};

	enum TextType
	{
		_Title		= 0,
		_Body		= 1,
		Notes		= 2,
		Outline		= 3,
		Other		= 4,
		CenterBody	= 5,
		CenterTitle	= 6,
		HalfBody	= 7,
		QuarterBody	= 8,

		NoPresent   = 0xFF
	};

	enum ActionTable
	{
		NoAction			= 0, 
		MacroAction			= 1, 
		RunProgramAction	= 2,
		JumpAction			= 3, 
		HyperlinkAction		= 4,
		OLEAction			= 5, 
		MediaAction			= 6, 
		CustomShowAction	= 7
	};

	enum JumpTable
	{
		NoJump				= 0, 
		NextSlide			= 1, 
		PreviousSlide		= 2,
		FirstSlide			= 3, 
		LastSlide			= 4,
		LastSlideViewed		= 5, 
		EndShow				= 6, 
		SlideId				= 7
	};
}

namespace NSOfficeDrawing
{
	enum ShapePath
	{
		shapeLines,        // A line of straight segments    
		shapeLinesClosed,  // A closed polygonal object    
		shapeCurves,       // A line of Bezier curve segments    
		shapeCurvesClosed, // A closed shape with curved edges    
		shapeComplex       // pSegmentInfo must be non-empty 
	};

	enum WrapMode
	{    
		wrapSquare,    
		wrapByPoints,    
		wrapNone,    
		wrapTopBottom,    
		wrapThrough    
	}; 

	enum BwMode
	{    
		bwColor,          // only used for predefined shades    
		bwAutomatic,      // depends on object type    
		bwGrayScale,      // shades of gray only    
		bwLightGrayScale, // shades of light gray only    
		bwInverseGray,    // dark gray mapped to light gray, etc.    
		bwGrayOutline,    // pure gray and white    
		bwBlackTextLine,  // black text and lines, all else grayscale    
		bwHighContrast,   // pure black and white mode (no grays)    
		bwBlack,          // solid black    
		bwWhite,          // solid white    
		bwDontShow,       // object not drawn    
		bwNumModes        // number of Black and white modes    
	};

	enum AnchorMode
	{    
		anchorTop,     
		anchorMiddle,     
		anchorBottom,     
		anchorTopCentered,     
		anchorMiddleCentered,     
		anchorBottomCentered,    
		anchorTopBaseline,    
		anchorBottomBaseline,    
		anchorTopCenteredBaseline, 
		anchorBottomCenteredBaseline    
	}; 

	enum CdirMode
	{    
		cdir0,     // Right    
		cdir90,    // Down    
		cdir180,   // Left    
		cdir270    // Up    
	};

	enum CxStyle   
	{    
		cxstyleStraight = 0,    
		cxstyleBent,    
		cxstyleCurved,    
		cxstyleNone    
	};

	enum CXKMode  
	{    
		cxkNone = 0,      // No connection sites    
		cxkSegments = 1,  // Connection sites at the segments    
		cxkCustom = 2,    // Sites defined by pidPConnectionSites    
		cxkRect = 3       // Use the connection sites for a rectangle    
	};

	enum TxflMode
	{    
		txflHorzN,           //Horizontal non-@    
		txflTtoBA,           //Top to Bottom @-font    
		txflBtoT,            //Bottom to Top non-@    
		txflTtoBN,           //Top to Bottom non-@    
		txflHorzA,           //Horizontal @-font    
		txflVertN            //Vertical, non-@    
	};

	enum TxDirMode
	{    
		txdirLTR,		// left-to-right text direction    
		txdirRTL,		// right-to-left text direction    
		txdirContext    // context text direction     
	}; 

	enum SpcotMode
	{    
		spcotRightAngle = 1,    
		spcotOneSegment = 2, 
		spcotTwoSegment = 3,    
		spcotThreeSegment = 4    
	};

	enum SpcoaMode
	{   
		spcoaAny,    
		spcoa30,    
		spcoa45,    
		spcoa60,    
		spcoa90,    
		spcoa0    
	};

	enum SpcodMode
	{    
		spcodTop,    
		spcodCenter,    
		spcodBottom,    
		spcodSpecified    
	};

	enum GeoTextAlign
	{   
		alignTextStretch,      /* Stretch each line of text to fit width. */    
		alignTextCenter,       /* Center text on width. */    
		alignTextLeft,         /* Left justify. */    
		alignTextRight,        /* Right justify. */    
		alignTextLetterJust,   /* Spread letters out to fit width. */    
		alignTextWordJust,     /* Spread words out to fit width. */    
		alignTextInvalid       /* Invalid */    
	};

	enum BlipFlags
	{    
		blipflagDefault = 0,    
		blipflagComment = 0,		// Blip name is a comment    
		blipflagFile,			// Blip name is a file name    
		blipflagURL,				// Blip name is a full URL    
		blipflagType = 3,		// Mask to extract type	
		/* Or the following flags with any of the above. */    
		blipflagDontSave   = 4,	// A "dont" is the depression in the metal                    
									// body work of an automobile caused when a                    
									// cyclist violently thrusts his or her nose                    
									// at it, thus a DontSave is another name for                    
									// a cycle lane.    
		blipflagDoNotSave  = 4,	// For those who prefer English    
		blipflagLinkToFile = 8    
	}; 

	enum e3DRenderMode
	{    
		FullRender,   // Generate a full rendering    
		Wireframe,    // Generate a wireframe    
		BoundingCube  // Generate a bounding cube    
	};

	enum XFormType
	{    
		xformAbsolute,   // Apply transform in absolute space centered on shape    
		xformShape,      // Apply transform to shape geometry    
		xformDrawing     // Apply transform in drawing space    
	};

	enum ShadowType
	{    
		shadowOffset,    // N pixel offset shadow    
		shadowDouble,    // Use second offset too    
		shadowRich,      // Rich perspective shadow (cast relative to shape)    
		shadowShape,     // Rich perspective shadow (cast in shape space)    
		shadowDrawing,   // Perspective shadow cast in drawing space     
		shadowEmbossOrEngrave    
	};

	enum DzType
	{    
		dztypeMin					= 0,    
		dztypeDefault				= 0,	// Default size, ignore the values    
		dztypeA						= 1,	// Values are in EMUs    
		dztypeV						= 2,	// Values are in pixels    
		dztypeShape					= 3,	// Values are 16.16 fractions of shape size    
		dztypeFixedAspect			= 4,	// Aspect ratio is fixed    
		dztypeAFixed				= 5,	// EMUs, fixed aspect ratio    
		dztypeVFixed				= 6,	// Pixels, fixed aspect ratio    
		dztypeShapeFixed			= 7,	// Proportion of shape, fixed aspect ratio    
		dztypeFixedAspectEnlarge	= 8,	// Aspect ratio is fixed, favor larger size    
		dztypeAFixedBig				= 9,	// EMUs, fixed aspect ratio    
		dztypeVFixedBig				= 10,	// Pixels, fixed aspect ratio    
		dztypeShapeFixedBig			= 11,	// Proportion of shape, fixed aspect ratio    
		dztypeMax					= 11    
	};

	enum FillType
	{    
		fillSolid,             // Fill with a solid color    
		fillPattern,           // Fill with a pattern (bitmap)    
		fillTexture,           // A texture (pattern with its own color map)    
		fillPicture,           // Center a picture in the shape    
		fillShade,             // Shade from start to end points              
		fillShadeCenter,       // Shade from bounding rectangle to end point    
		fillShadeShape,        // Shade from shape outline to end point    
		fillShadeScale,        // Similar to fillShade, but the fillAngle                    
									// is additionally scaled by the aspect ratio of                    
									// the shape. If shape is square, it is the                    
									// same as fillShade.    
		fillShadeTitle,		  // special type - shade to title ---  for PP     
		fillBackground         // Use the background fill color/pattern    
	}; 

	enum ShadeType
	{    
		shadeNone  = 0,        // Interpolate without correction between RGBs    
		shadeGamma = 1,        // Apply gamma correction to colors    
		shadeSigma = 2,        // Apply a sigma transfer function to position    
		shadeBand  = 4,        // Add a flat band at the start of the shade    
		shadeOneColor = 8,     // This is a one color shade      
		
		/* A parameter for the band or sigma function can be stored in the top      
		16 bits of the value - this is a proportion of *each* band of the      
		shade to make flat (or the approximate equal value for a sigma      
		function).  NOTE: the parameter is not used for the sigma function,      
		instead a built in value is used.  This value should not be changed      
		from the default! */    

		shadeParameterShift = 16,    
		shadeParameterMask  = 0xFFFF0000,      
		shadeDefault = (shadeGamma | shadeSigma | (16384 << shadeParameterShift))    
	};

	enum LinesStyle
	{    
		lineSimple,     // Single line (of width lineWidth)    
		lineDouble,     // Double lines of equal width    
		lineThickThin,  // Double lines, one thick, one thin    
		lineThinThick,  // Double lines, reverse order    
		lineTriple      // Three lines, thin, thick, thin    
	};

	enum LineType
	{    
		lineSolidType,         // Fill with a solid color    
		linePattern,           // Fill with a pattern (bitmap)    
		lineTexture,           // A texture (pattern with its own color map)    
		linePicture            // Center a picture in the shape    
	};

	enum LineDashing
	{
		lineSolid,				// Solid (continuous) pen    
		lineDashSys,			// PS_DASH system  dash style    
		lineDotSys,				// PS_DOT system  dash style    
		lineDashDotSys,			// PS_DASHDOT system dash style    
		lineDashDotDotSys,		// PS_DASHDOTDOT system dash style    
		lineDotGEL,				// square dot style    
		lineDashGEL,			// dash style    
		lineLongDashGEL,		// long dash style    
		lineDashDotGEL,			// dash short dash    
		lineLongDashDotGEL,		// long dash short dash    
		lineLongDashDotDotGEL	// long dash short dash short dash    
	}; 

	enum LineEnd
	{    
		lineNoEnd,   
		lineArrowEnd,    
		lineArrowStealthEnd,    
		lineArrowDiamondEnd,    
		lineArrowOvalEnd,    
		lineArrowOpenEnd    
	}; 

	enum LineEndWidth
	{    
		lineNarrowArrow,    
		lineMediumWidthArrow,    
		lineWideArrow    
	};

	enum LineEndLength
	{    
		lineShortArrow,    
		lineMediumLenArrow,    
		lineLongArrow    
	};

	enum _LineJoin
	{    
		lineJoinBevel,    // Join edges by a straight line    
		lineJoinMiter,    // Extend edges until they join    
		lineJoinRound     // Draw an arc between the two edges    
	};

	enum _LineCap
	{    
		lineEndCapRound,   // Rounded ends - the default    
		lineEndCapSquare,  // Square protrudes by half line width    
		lineEndCapFlat     // Line ends at end point              
	};

	enum SGF     
	{    
		// Arithmetic    
		sgfSum,					// guide := value + parameter1 - parameter2    
		sgfProduct,				// guide := value * parameter1 / parameter2 (MulDiv)    
		sgfMid,					// guide := (value + param1)/2      
		
		// Logical    
		sgfAbsolute,			// guide := fabs(value)    
		sgfMin,					// guide := min(value, param1)    
		sgfMax,					// guide := max(value, param1)    
		sgfIf,					// guide := vaule > 0 ? param1 : param2      
		
		// Polar arithmetic - angles are 16.16 degrees    
		sgfMod,					// guide := sqrt(value^2 + param1^2 + param2^2)    
		sgfATan2,				// guide := atan2(param1, value) [param2 ignored]    
		sgfSin,					// guide := value * sin(param1)  [param2 ignored]    
		sgfCos,					// guide := value * cos(param1)  [param2 ignored]    
		sgfCosATan2,			// guide := value * cos(atan2(param2, param1))    
		sgfSinATan2,			// guide := value * sin(atan2(param2, param1))    
		sgfSqrt,				// guide := sqrt(value) [param1 and param2 ignored]    
		sgfSumAngle,    
		sgfEllipse,    
		sgfTan				    // guide := value * tan(param1)  [param2 ignored]  
	};

	enum SGVT    
	{    
		sgvtV  = 0x2000,     // Value is adjust value or guide    
		sgvtP1 = 0x4000,     // Parameter1 is adjust value or guide    
		sgvtP2 = 0x8000      // Parameter2 is adjust value or guide    
	}; 

	enum _Effects
	{
		efFillEffect,
		efClrRepl,
		efFillOverlay,
		efClrChange,
		efGlow,
		efBlur,
		efGrayscl,
		efBlend,
		efHslEffect,
		efBiLevel,
		efInnerShdw,
		efAlphaRepl,
		efAlphaOutset,
		efLumEffect,
		efAlphaModFix,
		efOuterShdw,
		efAlphaMod,
		efPrstShdw,
		efAlphaInv,
		efReflection,
		efAlphaFloor,
		efRelOff,
		efAlphaCeiling,
		efAlphaBiLevel,
		efSoftEdge,
		efDuotone,
		efTintEffect,
		efXfrmEffect
	};

	enum _Transitions
	{
		etBlinds,
		etChecker,
		etCircle,
		etComb,
		etCover,
		etCut,
		etDiamond,
		etDissolve,
		etFade,
		etNewsflash,
		etPlus,
		etPull,
		etPush,
		etRandom,
		etRandomBar,
		etSplit,
		etStrips,
		etWedge,
		etWheel,
		etWipe,
		etZoom
	}
	///////////////////////////////////////////////////////////////
	//  а нужно ли...?
	///////////////////////////////////////////////////////////////

	/*
	
	// Value index: The first set define the value as an adjust handle property.  
	const USHORT sgvAdjust1 = pidAdjustValue;  
	const USHORT sgvAdjust2 = pidAdjust2Value; 
	
	const USHORT sgvAdjust3 = pidAdjust3Value;  
	const USHORT sgvAdjust4 = pidAdjust4Value;  
	const USHORT sgvAdjust5 = pidAdjust5Value;  
	const USHORT sgvAdjust6 = pidAdjust6Value;  
	const USHORT sgvAdjust7 = pidAdjust7Value;  
	const USHORT sgvAdjust8 = pidAdjust8Value;    
	
	// Geometry width and height  
	const USHORT sgvWidth   = pidGeoRight;  
	const USHORT sgvHeight  = pidGeoBottom;    
	
	// Other geometry properties  
	const USHORT sgvXCenter = pidGeoLeft;  
	const USHORT sgvYCenter = pidGeoTop;  
	const USHORT sgvXLimo   = pidXLimo;  
	const USHORT sgvYLimo   = pidYLimo;   
	
	// The guide properties  

	#define sgvGuide_(n) (USHORT(pidLast + 1 + (n)))    
	
	// View coordinate properties in V units (pixesl) and EMU, 
	// the EMU scaling    is the average of the width/height scaling  

	const USHORT sgvLineWidth = sgvGuide_(247);  // line width in pixels  
	const USHORT sgvDxvAnchor = sgvGuide_(248);  // width in pixels  
	const USHORT sgvDyvAnchor = sgvGuide_(249);  // height in pixels    
	const USHORT sgvDxeAnchor = sgvGuide_(252);  // width of a shape in the view in emus  
	const USHORT sgvDyeAnchor = sgvGuide_(253);  // Height of a shape in the view in emus  
	const USHORT sgvXeAnchorCenter = sgvGuide_(254);  
	const USHORT sgvYeAnchorCenter = sgvGuide_(255); 

	*/

	////////////////////////////////////////////////////////////
	// <- а нужно ли...?
	////////////////////////////////////////////////////////////

	enum BlipUsage
	{                
		blipUsageDefault,  // All non-texture fill blips get this.    
		blipUsageTexture,    
		blipUsageMax = 255 // Since this is stored in a byte    
	};
	
	enum BlipType
	{                   
		// GEL provided types...    
		blipERROR = 0,			// An error occured during loading    
		blipUNKNOWN,			// An unknown blip type    
		blipEMF,				// Windows Enhanced Metafile    
		blipWMF,				// Windows Metafile    
		blipPICT,				// Macintosh PICT    
		blipJPEG,				// JFIF   
		blipPNG,				// PNG or GIF    
		blipDIB,				// Windows DIB    
		blipTIFF = 17,			// TIFF    
		blipCMYKJPEG = 18,		// JPEG data in YCCK or CMYK color space    
		blipFirstClient = 32,   // First client defined blip type    
		blipLastClient  = 255   // Last client defined blip type    
	};    
	
	enum BlipSignature
	{    
		biUNKNOWN = 0,    
		biWMF  = 0x216,		// Metafile header then compressed WMF    
		biEMF  = 0x3D4,		// Metafile header then compressed EMF    
		biPICT = 0x542,		// Metafile header then compressed PICT    
		biPNG  = 0x6E0,		// One byte tag then PNG data    
		biJFIF = 0x46A,		// One byte tag then JFIF data    
		biJPEG = biJFIF,    
		biDIB  = 0x7A8,		// One byte tag then DIB data    
		biCMYKJPEG = 0x6E2, // One byte tag then CMYK/YCCK JPEG data    
		biTIFF = 0x6e4,		// One byte tag then TIFF data    
		biClient = 0x800,	// Clients should set this bit    
	}; 

	enum BlipFilter
    {
        filterAdaptive = 0,
        filterNone = 254,
        filterTest = 255
    };

	enum DGMNK   
	{    
		dgmnkMin = 0,    
		dgmnkNode = dgmnkMin,    
		dgmnkRoot,    
		dgmnkAssistant,    
		dgmnkCoWorker,    
		dgmnkSubordinate,    
		dgmnkAuxNode,    
		dgmnkDefault, // used for the orgchart split bar
		dgmnkMax, 
		dgmnkLast = dgmnkMax - 1,    
		dgmnkNil = 0xFFFF   
	}; 

	enum DGMLO
	{          
		dgmloFirst = 0, 
		dgmloMin = dgmloFirst, 
		dgmloMinLessOne = dgmloMin - 1,									
		
		// OrgChart layout          
		dgmloOrgChartMin,          
		dgmloOrgChartStd = 0,          
		dgmloOrgChartBothHanging,          
		dgmloOrgChartRightHanging,          
		dgmloOrgChartLeftHanging,          
		dgmloOrgChartMax, 
		dgmloOrgChartLast = dgmloOrgChartMax - 1,						
		
		// Cycle layout          
		dgmloCycleMin, dgmloCycleMinLessOne = dgmloCycleMin - 1,          
		dgmloCycleStd,          
		dgmloCycleMax, 
		dgmloCycleLast = dgmloCycleMax - 1,								
		
		// Radial layout          
		dgmloRadialMin, dgmloRadialMinLessOne = dgmloRadialMin - 1,          
		dgmloRadialStd,          
		dgmloRadialMax, dgmloRadialLast = dgmloRadialMax - 1,           
		
		// Stacked layout          
		dgmloStackedMin, dgmloStackedMinLessOne = dgmloStackedMin - 1,          
		dgmloStackedStd, 
		dgmloStackedMax, dgmloStackedLast = dgmloStackedMax - 1,        
		
		// Venn layout          
		dgmloVennMin, dgmloVennMinLessOne = dgmloVennMin - 1,          
		dgmloVennStd,          
		dgmloVennMax, dgmloVennLast = dgmloVennMax - 1,					
		
		// BullsEye layout          
		dgmloBullsEyeMin, dgmloBullsEyeMinLessOne = dgmloBullsEyeMin - 1,          
		dgmloBullsEyeStd,          
		dgmloBullsEyeMax, dgmloBullsEyeLast = dgmloBullsEyeMax - 1,            
		dgmloMax, dgmloLast = dgmloMax - 1,          
		dgmloNil = 0xFF
	};

	enum ODGMT
	{          
		dgmtMin                       = 0,          
		dgmtCanvas                    = dgmtMin,          
		dgmtFirstDiagramType          = 1,          
		dgmtOrgChart                  = dgmtFirstDiagramType,          
		dgmtRadial                    = 2, 
		dgmtCycle                     = 3,          
		dgmtStacked                   = 4,          
		dgmtVenn                      = 5,          
		dgmtBullsEye                  = 6,          
		dgmtMax, dgmtLast          = dgmtMax - 1,          
		dgmtNil                       = 0x0FFF
	}; 

	enum DGMST
	{          
		/***WARNING: This is written out to the file format! ***/          
		/***NOTE: If you add a new style enum, you need to update vrgdgmstdesc ***/            
		dgmstMin = 0, dgmstFirst = dgmstMin ,            
		
		// OrgChart styles          
		dgmstOrgChartFirst = dgmstFirst,          
		dgmstOrgChart2,          
		dgmstOrgChart3,          
		dgmstOrgChart4,          
		dgmstOrgChart5,          
		dgmstOrgChart6,          
		dgmstOrgChart7,          
		dgmstOrgChart8,          
		dgmstOrgChart9,          
		dgmstOrgChart10,         
		dgmstOrgChart11,          
		dgmstOrgChart12,          
		dgmstOrgChart13,          
		dgmstOrgChart14,          
		dgmstOrgChart15,          
		dgmstOrgChart16,          
		dgmstOrgChart17,          
		dgmstOrgChartMax,            
		dgmstOrgChartLast = dgmstOrgChartMax - 1,            
		
		// Radial styles          
		dgmstRadialFirst = dgmstFirst,          
		dgmstRadial2,          
		dgmstRadial3,          
		dgmstRadial4,          
		dgmstRadial5,          
		dgmstRadial6,          
		dgmstRadial7,          
		dgmstRadial8,          
		dgmstRadial9,          
		dgmstRadial10,          
		dgmstRadialMax,          
		dgmstRadialLast = dgmstRadialMax - 1,            
		
		// Cycle styles          
		dgmstCycleFirst = dgmstFirst,          
		dgmstCycle2, 
		dgmstCycle3,          
		dgmstCycle4,          
		dgmstCycle5,          
		dgmstCycle6,          
		dgmstCycle7,          
		dgmstCycle8,          
		dgmstCycle9,          
		dgmstCycle10,          
		dgmstCycle2First,          
		dgmstCycle11 = dgmstCycle2First,          
		dgmstCycle12,          
		dgmstCycle13,          
		dgmstCycle14,          
		dgmstCycle15,          
		dgmstCycle16,          
		dgmstCycle17,          
		dgmstCycle18,          
		dgmstCycle19,          
		dgmstCycle20,          
		dgmstCycle21,          
		dgmstCycleMax,         
		dgmstCycleLast = dgmstCycleMax - 1,            
		
		// Stacked styles          
		dgmstStackedFirst = dgmstFirst,          
		dgmstStacked2,          
		dgmstStacked3,          
		dgmstStacked4,          
		dgmstStacked5,          
		dgmstStacked6,          
		dgmstStacked7,          
		dgmstStacked8,          
		dgmstStacked9,          
		dgmstStacked10,          
		dgmstStackedMax,          
		dgmstStackedLast = dgmstStackedMax - 1,            
		
		// Venn styles          
		dgmstVennFirst = dgmstFirst,          
		dgmstVenn2,          
		dgmstVenn3,          
		dgmstVenn4,          
		dgmstVenn5,          
		dgmstVenn6,          
		dgmstVenn7,          
		dgmstVenn8,          
		dgmstVenn9,          
		dgmstVenn10,          
		dgmstVennMax,          
		dgmstVennLast = dgmstVennMax - 1,            
		
		// BullsEyeChart styles          
		dgmstBullsEyeFirst = dgmstFirst,          
		dgmstBullsEye2, 
		dgmstBullsEye3,          
		dgmstBullsEye4,          
		dgmstBullsEye5,          
		dgmstBullsEye6,          
		dgmstBullsEye7,          
		dgmstBullsEye8,          
		dgmstBullsEye9,          
		dgmstBullsEye10,          
		dgmstBullsEyeMax,          
		dgmstBullsEyeLast = dgmstBullsEyeMax - 1,            
		dgmstNil = 0xFFFF         
	};

	enum SPT
	{    
		sptMin = 0,    
		sptNotPrimitive = sptMin,    
		sptRectangle = 1,    
		sptRoundRectangle = 2,    
		sptEllipse = 3,    
		sptDiamond = 4,    
		sptIsocelesTriangle = 5,    
		sptRightTriangle = 6,    
		sptParallelogram = 7,    
		sptTrapezoid = 8,    
		sptHexagon = 9,    
		sptOctagon = 10,    
		sptPlus = 11,    
		sptStar = 12,    
		sptArrow = 13,    
		sptThickArrow = 14,   
		sptHomePlate = 15,    
		sptCube = 16,    
		sptBalloon = 17,    
		sptSeal = 18,    
		sptArc = 19,    
		sptLine = 20,    
		sptPlaque = 21,   
		sptCan = 22,    
		sptDonut = 23,    
		sptTextSimple = 24,    
		sptTextOctagon = 25,    
		sptTextHexagon = 26,    
		sptTextCurve = 27,    
		sptTextWave = 28,    
		sptTextRing = 29,    
		sptTextOnCurve = 30,    
		sptTextOnRing = 31,    
		sptStraightConnector1 = 32,    
		sptBentConnector2 = 33,    
		sptBentConnector3 = 34,    
		sptBentConnector4 = 35,    
		sptBentConnector5 = 36,    
		sptCurvedConnector2 = 37,    
		sptCurvedConnector3 = 38,    
		sptCurvedConnector4 = 39,    
		sptCurvedConnector5 = 40,    
		sptCallout1 = 41,    
		sptCallout2 = 42,    
		sptCallout3 = 43,   
		sptAccentCallout1 = 44,    
		sptAccentCallout2 = 45,    
		sptAccentCallout3 = 46,    
		sptBorderCallout1 = 47,    
		sptBorderCallout2 = 48,    
		sptBorderCallout3 = 49,    
		sptAccentBorderCallout1 = 50,    
		sptAccentBorderCallout2 = 51,    
		sptAccentBorderCallout3 = 52,    
		sptRibbon = 53,    
		sptRibbon2 = 54,    
		sptChevron = 55,    
		sptPentagon = 56,  
		sptNoSmoking = 57,    
		sptSeal8 = 58,    
		sptSeal16 = 59,    
		sptSeal32 = 60,    
		sptWedgeRectCallout = 61,    
		sptWedgeRRectCallout = 62,    
		sptWedgeEllipseCallout = 63,    
		sptWave = 64,    
		sptFoldedCorner = 65,    
		sptLeftArrow = 66,    
		sptDownArrow = 67,    
		sptUpArrow = 68,    
		sptLeftRightArrow = 69,   
		sptUpDownArrow = 70,    
		sptIrregularSeal1 = 71,    
		sptIrregularSeal2 = 72,    
		sptLightningBolt = 73,    
		sptHeart = 74,   
		sptPictureFrame = 75,    
		sptQuadArrow = 76,    
		sptLeftArrowCallout = 77,    
		sptRightArrowCallout = 78,    
		sptUpArrowCallout = 79,    
		sptDownArrowCallout = 80,    
		sptLeftRightArrowCallout = 81,    
		sptUpDownArrowCallout = 82,    
		sptQuadArrowCallout = 83,    
		sptBevel = 84,    
		sptLeftBracket = 85,    
		sptRightBracket = 86,    
		sptLeftBrace = 87,    
		sptRightBrace = 88,    
		sptLeftUpArrow = 89,    
		sptBentUpArrow = 90,    
		sptBentArrow = 91,    
		sptSeal24 = 92,    
		sptStripedRightArrow = 93,    
		sptNotchedRightArrow = 94,    
		sptBlockArc = 95,    
		sptSmileyFace = 96,    
		sptVerticalScroll = 97,    
		sptHorizontalScroll = 98,    
		sptCircularArrow = 99,    
		sptNotchedCircularArrow = 100,    
		sptUturnArrow = 101,    
		sptCurvedRightArrow = 102,    
		sptCurvedLeftArrow = 103,    
		sptCurvedUpArrow = 104,    
		sptCurvedDownArrow = 105,   
		sptCloudCallout = 106,    
		sptEllipseRibbon = 107,    
		sptEllipseRibbon2 = 108,    
		sptFlowChartProcess = 109,    
		sptFlowChartDecision = 110,    
		sptFlowChartInputOutput = 111,    
		sptFlowChartPredefinedProcess = 112,    
		sptFlowChartInternalStorage = 113,    
		sptFlowChartDocument = 114,    
		sptFlowChartMultidocument = 115,    
		sptFlowChartTerminator = 116,    
		sptFlowChartPreparation = 117,    
		sptFlowChartManualInput = 118,   
		sptFlowChartManualOperation = 119,   
		sptFlowChartConnector = 120,    
		sptFlowChartPunchedCard = 121,    
		sptFlowChartPunchedTape = 122,    
		sptFlowChartSummingJunction = 123,   
		sptFlowChartOr = 124,    
		sptFlowChartCollate = 125,    
		sptFlowChartSort = 126,    
		sptFlowChartExtract = 127,    
		sptFlowChartMerge = 128,    
		sptFlowChartOfflineStorage = 129,    
		sptFlowChartOnlineStorage = 130,    
		sptFlowChartMagneticTape = 131,    
		sptFlowChartMagneticDisk = 132,    
		sptFlowChartMagneticDrum = 133,    
		sptFlowChartDisplay = 134,    
		sptFlowChartDelay = 135,   
		sptTextPlainText = 136,    
		sptTextStop = 137,    
		sptTextTriangle = 138,    
		sptTextTriangleInverted = 139,    
		sptTextChevron = 140,    
		sptTextChevronInverted = 141,    
		sptTextRingInside = 142,    
		sptTextRingOutside = 143,    
		sptTextArchUpCurve = 144,    
		sptTextArchDownCurve = 145,    
		sptTextCircleCurve = 146,    
		sptTextButtonCurve = 147,    
		sptTextArchUpPour = 148,    
		sptTextArchDownPour = 149,    
		sptTextCirclePour = 150,    
		sptTextButtonPour = 151,    
		sptTextCurveUp = 152,    
		sptTextCurveDown = 153,   
		sptTextCascadeUp = 154,   
		sptTextCascadeDown = 155,   
		sptTextWave1 = 156,    
		sptTextWave2 = 157,    
		sptTextWave3 = 158,    
		sptTextWave4 = 159,    
		sptTextInflate = 160,    
		sptTextDeflate = 161,    
		sptTextInflateBottom = 162,    
		sptTextDeflateBottom = 163,    
		sptTextInflateTop = 164, 
		sptTextDeflateTop = 165,    
		sptTextDeflateInflate = 166,    
		sptTextDeflateInflateDeflate = 167,    
		sptTextFadeRight = 168,    
		sptTextFadeLeft = 169,    
		sptTextFadeUp = 170,    
		sptTextFadeDown = 171,    
		sptTextSlantUp = 172,    
		sptTextSlantDown = 173,    
		sptTextCanUp = 174,    
		sptTextCanDown = 175,    
		sptFlowChartAlternateProcess = 176,    
		sptFlowChartOffpageConnector = 177,    
		sptCallout90 = 178,    
		sptAccentCallout90 = 179,    
		sptBorderCallout90 = 180,    
		sptAccentBorderCallout90 = 181,   
		sptLeftRightUpArrow = 182,    
		sptSun = 183,    
		sptMoon = 184,    
		sptBracketPair = 185,    
		sptBracePair = 186,    
		sptSeal4 = 187,    
		sptDoubleWave = 188,    
		sptActionButtonBlank = 189,    
		sptActionButtonHome = 190,    
		sptActionButtonHelp = 191,    
		sptActionButtonInformation = 192,    
		sptActionButtonForwardNext = 193,    
		sptActionButtonBackPrevious = 194,   
		sptActionButtonEnd = 195,    
		sptActionButtonBeginning = 196,    
		sptActionButtonReturn = 197,    
		sptActionButtonDocument = 198,    
		sptActionButtonSound = 199,    
		sptActionButtonMovie = 200,   
		sptHostControl = 201,    
		sptTextBox = 202,   
		sptMax,    
		sptNil = 0x0FFF
	};

	enum PropertyId
    {
        //Transform
        left=0,
        top=1,
        right=2,
        bottom=3,
        rotation=4,
        gvPage=5,
        fChangePage=61,
        fFlipV=62,
        fFlipH=63,

        //Protection
        fLockAgainstUngrouping=118,
        fLockRotation=119,
        fLockAspectRatio=120,
        fLockPosition=121,
        fLockAgainstSelect=122,
        fLockCropping=123,
        fLockVertices=124,
        fLockText=125,
        fLockAdjustHandles=126,
        fLockAgainstGrouping=127,

        //Text
        lTxid=128,
        dxTextLeft=129,
        dyTextTop=130,
        dxTextRight=131,
        dyTextBottom=132,
        WrapText=133,
        scaleText=134,
        anchorText=135,
        txflTextFlow=136,
        cdirFont=137,
        hspNext=138,
        txdir=139,
        ccol=140,
        dzColMargin=141,
        fSelectText=187,
        fAutoTextMargin=188,
        fRotateText=189,
        fFitShapeToText=190,
        fFitTextToShape=191,

        //GeoText
        gtextUNICODE=192,
        gtextRTF=193,
        gtextAlign=194,
        gtextSize=195,
        gtextSpacing=196,
        gtextFont=197,
        gtextCSSFont=198,
        gtextFReverseRows=240,
        fGtext=241,
        gtextFVertical=242,
        gtextFKern=243,
        gtextFTight=244,
        gtextFStretch=245,
        gtextFShrinkFit=246,
        gtextFBestFit=247,
        gtextFNormalize=248,
        gtextFDxMeasure=249,
        gtextFBold=250,
        gtextFItalic=251,
        gtextFUnderline=252,
        gtextFShadow=253,
        gtextFSmallcaps=254,
        gtextFStrikethrough=255,

        //BLIP
        cropFromTop=256,
        cropFromBottom=257,
        cropFromLeft=258,
        cropFromRight=259,
        Pib=260,
        pibName=261,
        pibFlags=262,
        pictureTransparent=263,
        pictureContrast=264,
        pictureBrightness=265,
        pictureGamma=266,
        pictureId=267,
        pictureDblCrMod=268,
        pictureFillCrMod=269,
        pictureLineCrMod=270,
        pibPrint=271,
        pibPrintName=272,
        pibPrintFlags=273,
        movie=274,
        pictureRecolor=282,
        picturePreserveGrays=313,
        fRewind=314,
        fLooping=315,
        pictureGray=317,
        pictureBiLevel=318,
        pictureActive=319,

        //Geometry
        geoLeft=320,
        geoTop=321,
        geoRight=322,
        geoBottom=323,
        shapePath=324,
        pVertices=325,
        pSegmentInfo=326,
        adjustValue=327,
        adjust2Value=328,
        adjust3Value=329,
        adjust4Value=330,
        adjust5Value=331,
        adjust6Value=332,
        adjust7Value=333,
        adjust8Value=334,
        adjust9Value=335,
        adjust10Value=336,
        pConnectionSites=337,
        pConnectionSitesDir=338,
        xLimo=339,
        yLimo=340,
        pAdjustHandles=341,
        pGuides=342,
        pInscribe=343,
        cxk=344,
        pFragments=345,
        fColumnLineOK=377,
        fShadowOK=378,
        f3DOK=379,
        fLineOK=380,
        fGtextOK=381,
        fFillShadeShapeOK=382,
        fFillOK=383,

        //Fill Style
        fillType=384,
        fillColor=385,
        fillOpacity=386,
        fillBackColor=387,
        fillBackOpacity=388,
        fillCrMod=389,
        fillBlip=390,
        fillBlipName=391,
        fillBlipFlags=392,
        fillWidth=393,
        fillHeight=394,
        fillAngle=395,
        fillFocus=396,
        fillToLeft=397,
        fillToTop=398,
        fillToRight=399,
        fillToBottom=400,
        fillRectLeft=401,
        fillRectTop=402,
        fillRectRight=403,
        fillRectBottom=404,
        fillDztype=405,
        fillShadePreset=406,
        fillShadeColors=407,
        fillOriginX=408,
        fillOriginY=409,
        fillShapeOriginX=410,
        fillShapeOriginY=411,
        fillShadeType=412,
        fRecolorFillAsPicture=441,
        fUseShapeAnchor=442,
        fFilled=443,
        fHitTestFill=444,
        fillShape=445,
        fillUseRect=446,
        fNoFillHitTest=447,

        //Line Style
        lineColor=448,
        lineOpacity=449,
        lineBackColor=450,
        lineCrMod=451,
        lineType=452,
        lineFillBlip=453,
        lineFillBlipName=454,
        lineFillBlipFlags=455,
        lineFillWidth=456,
        lineFillHeight=457,
        lineFillDztype=458,
        lineWidth=459,
        lineMiterLimit=460,
        lineStyle=461,
        lineDashing=462,
        lineDashStyle=463,
        lineStartArrowhead=464,
        lineEndArrowhead=465,
        lineStartArrowWidth=466,
        lineStartArrowLength=467,
        lineEndArrowWidth=468,
        lineEndArrowLength=469,
        lineJoinStyle=470,
        lineEndCapStyle=471,
        fInsetPen=505,
        fInsetPenOK=506,
        fArrowheadsOK=507,
        fLine=508,
        fHitTestLine=509,
        lineFillShape=510,
        fNoLineDrawDash=511,

        //Shadow Style
        shadowType=512,
        shadowColor=513,
        shadowHighlight=514,
        shadowCrMod=515,
        shadowOpacity=516,
        shadowOffsetX=517,
        shadowOffsetY=518,
        shadowSecondOffsetX=519,
        shadowSecondOffsetY=520,
        shadowScaleXToX=521,
        shadowScaleYToX=522,
        shadowScaleXToY=523,
        shadowScaleYToY=524,
        shadowPerspectiveX=525,
        shadowPerspectiveY=526,
        shadowWeight=527,
        shadowOriginX=528,
        shadowOriginY=529,
        fShadow=574,
        fshadowObscured=575,

        //Perspective Style
        perspectiveType=576,
        perspectiveOffsetX=577,
        perspectiveOffsetY=578,
        perspectiveScaleXToX=579,
        perspectiveScaleYToX=580,
        perspectiveScaleXToY=581,
        perspectiveScaleYToY=582,
        perspectivePerspectiveX=583,
        perspectivePerspectiveY=584,
        perspectiveWeight=585,
        perspectiveOriginX=586,
        perspectiveOriginY=587,
        fPerspective=639,

        //3D Object
        c3DSpecularAmt=640,
        c3DDiffuseAmt=641,
        c3DShininess=642,
        c3DEdgeThickness=643,
        C3DExtrudeForward=644,
        c3DExtrudeBackward=645,
        c3DExtrudePlane=646,
        c3DExtrusionColor=647,
        c3DCrMod=648,
        f3D=700,
        fc3DMetallic=701,
        fc3DUseExtrusionColor=702,
        fc3DLightFace=703,

        //3D Style
        c3DYRotationAngle=704,
        c3DXRotationAngle=705,
        c3DRotationAxisX=706,
        c3DRotationAxisY=707,
        c3DRotationAxisZ=708,
        c3DRotationAngle=709,
        c3DRotationCenterX=710,
        c3DRotationCenterY=711,
        c3DRotationCenterZ=712,
        c3DRenderMode=713,
        c3DTolerance=714,
        c3DXViewpoint=715,
        c3DYViewpoint=716,
        c3DZViewpoint=717,
        c3DOriginX=718,
        c3DOriginY=719,
        c3DSkewAngle=720,
        c3DSkewAmount=721,
        c3DAmbientIntensity=722,
        c3DKeyX=723,
        c3DKeyY=724,
        c3DKeyZ=725,
        c3DKeyIntensity=726,
        c3DFillX=727,
        c3DFillY=728,
        c3DFillZ=729,
        c3DFillIntensity=730,
        fc3DConstrainRotation=763,
        fc3DRotationCenterAuto=764,
        fc3DParallel=765,
        fc3DKeyHarsh=766,
        fc3DFillHarsh=767,

        //Shape
        hspMaster=769,
        cxstyle=771,
        bWMode=772,
        bWModePureBW=773,
        bWModeBW=774,
        idDiscussAnchor=775,
        dgmLayout=777,
        dgmNodeKind=778,
        dgmLayoutMRU=779,
        wzEquationXML=780,
        fPolicyLabel=822,
        fPolicyBarcode=823,
        fFlipHQFE5152=824,
        fFlipVQFE5152=825,
        fPreferRelativeResize=827,
        fLockShapeType=828,
        fInitiator=829,
        fDeleteAttachedObject=830,
        fBackground=831,

        //Callout
        spcot=832,
        dxyCalloutGap=833,
        spcoa=834,
        spcod=835,
        dxyCalloutDropSpecified=836,
        dxyCalloutLengthSpecified=837,
        fCallout=889,
        fCalloutAccentBar=890,
        fCalloutTextBorder=891,
        fCalloutMinusX=892,
        fCalloutMinusY=893,
        fCalloutDropAuto=894,
        fCalloutLengthSpecified=895,

        //Groupe Shape
        wzName=896,
        wzDescription=897,
        pihlShape=898,
        pWrapPolygonVertices=899,
        dxWrapDistLeft=900,
        dyWrapDistTop=901,
        dxWrapDistRight=902,
        dyWrapDistBottom=903,
        lidRegroup=904,
        groupLeft=905,
        groupTop=906,
        groupRight=907,
        groupBottom=908,
        wzTooltip=909,
        wzScript=910,
        posh=911,
        posrelh=912,
        posv=913,
        posrelv=914,
        pctHR=915,
        alignHR=916,
        dxHeightHR=917,
        dxWidthHR=918,
        wzScriptExtAttr=919,
        scriptLang=920,
        wzScriptIdAttr=921,
        wzScriptLangAttr=922,
        borderTopColor=923,
        borderLeftColor=924,
        borderBottomColor=925,
        borderRightColor=926,
        tableProperties=927,
        tableRowProperties=928,
        scriptHtmlLocation=929,
        wzApplet=930,
        wzFrameTrgtUnused=932,
        wzWebBot=933,
        wzAppletArg=934,
        wzAccessBlob=936,
        metroBlob=937,
        dhgt=938,
        fLayoutInCell=944,
        fIsBullet=945,
        fStandardHR=946,
        fNoshadeHR=947,
        fHorizRule=948,
        fUserDrawn=949,
        fAllowOverlap=950,
        fReallyHidden=951,
        fScriptAnchor=952,

        //Unknown HTML
        wzLineId=1026,
        wzFillId=1027,
        wzPictureId=1028,
        wzPathId=1029,
        wzShadowId=1030,
        wzPerspectiveId=1031,
        wzGtextId=1032,
        wzFormulaeId=1033,
        wzHandlesId=1034,
        wzCalloutId=1035,
        wzLockId=1036,
        wzTextId=1037,
        wzThreeDId=1038,
        FakeShapeType=1039,
        fFakeMaster=1086,

        //Diagramm
        dgmt=1280,
        dgmStyle=1281,
        pRelationTbl=1284,
        dgmScaleX=1285,
        dgmScaleY=1286,
        dgmDefaultFontSize=1287,
        dgmConstrainBounds=1288,
        dgmBaseTextScale=1289,
        fBorderlessCanvas=1338,
        fNonStickyInkCanvas=1339,
        fDoFormat=1340,
        fReverse=1341,
        fDoLayout=1342,
        fPseudoInline=1343,

		// dashing
		lineLeftDashStyle=1359,
		lineTopDashStyle=1423,
		lineRightDashStyle=1487,
		lineBottomDashStyle=1551,

        //Web Component
        webComponentWzHtml=1664,
        webComponentWzName=1665,
        webComponentWzUrl=1666,
        webComponentWzProperties=1667,
        fIsWebComponent=1727,

        //Clip
        pVerticesClip=1728,
        pSegmentInfoClip=1729,
        shapePathClip=1730,
        fClipToWrap=1790,
        fClippedOK=1791,

        //Ink
        pInkData=1792,
        fInkAnnotation=1852,
        fHitTestInk=1853,
        fRenderShape=1854,
        fRenderInk=1855,

        //Signature
        wzSigSetupId=1921,
        wzSigSetupProvId=192,
        wzSigSetupSuggSigner=1923,
        wzSigSetupSuggSigner2=1924,
        wzSigSetupSuggSignerEmail=1925,
        wzSigSetupSignInst=1926,
        wzSigSetupAddlXml=1927,
        wzSigSetupProvUrl=1928,
        fSigSetupShowSignDate=1980,
        fSigSetupAllowComments=1981,
        fSigSetupSignInstSet=1982,
        fIsSignatureLine=1983,

        //Groupe Shape 2
        pctHoriz=1984,
        pctVert=1985,
        pctHorizPos=1986,
        pctVertPos=1987,
        sizerelh=1988,
        sizerelv=1989,
        colStart=1990,
        colSpan=1991
    };

	namespace NSAnimation
	{
		enum ConditionType     
		{        
			_NA,    // obsolete identifier; not used        
			ctBegin,        
			ctEnd,        
			Next,        
			Previous,        
			EndSync,        
			ConditionType_MaxEnumIDs     
		};

		enum TLTimeNodeType  
		{     
			TLTimeNodeTypeParallel = 0,     
			TLTimeNodeTypeSequential,     
			TLTimeNodeTypeExclusive,     
			TLTimeNodeTypeBehaviorType,     
			TLTimeNodeTypeMediaType,     
			TLTimeNodeType_MaxEnumIDs  
		};

		enum TLTimeNodeRestartType  
		{     
			TLR_NoRestartType = 0,     
			TLR_AlwaysRestart = 1,     
			TLR_RestartWhenOff = 2,     
			TLR_NeverRestart = 3,     
			TLTimeNodeRestartType_MaxEnumIDs = 4  
		}; 

		enum TLTimeNodeFillType 
		{     
			TLF_NoFillType = 0,     
			TLF_FillRemove = 1,     
			TLF_FillFreeze = 2, 
			TLF_FillHold = 3,     
			TLF_FillTransition = 4,     
			TLTimeNodeFillType_MaxEnumIDs = 5  
		}; 

		enum TLTimeNodeSyncType  
		{     
			TLS_NoSyncType = 0,     
			TLS_CanSlipSyncType = 1,     
			TLS_LockedSyncType = 2,     
			TLTimeNodeSyncType_MaxEnumIDs = 3  
		};    
		
		// property bit flag for propertiesSet  
		const int fillProperty         = (1 << 0);  
		const int restartProperty      = (1 << 1);  
		const int syncBehaviorProperty = (1 << 2);  
		const int groupingTypeProperty = (1 << 3);  
		const int durationProperty     = (1 << 4);

		enum TotEventType
		{     
			totNone,     
			totVisualElement,     
			totTimeNode,     
			totRuntimeNodeRef,     
			TriggerObjectType_MaxEnumIDs  
		}; 

		enum TriggerEventType  
		{     
			tetNone = 0,     
			tetOnBegin,     
			tetOnEnd,     
			tetBegin,     
			tetEnd,  
			tetOnClick,     
			tetOnDoubleClick,    
			tetOnMouseOver,     
			tetOnMouseOut,     
			tetOnNext,            // PPT-specific     
			tetOnPrev,            // PPT-specific     
			tetOnStopAudio,     
			TriggerEventType_MaxEnumIDs 
		}; 

		enum RuntimeNodeReferenceType  
		{     
			rnrtWithFirstChild,     
			rnrtWithLastChild,     
			rnrtAllChildren,     
			RuntimeNodeReferenceType_MaxEnumIDs  
		}; 

		enum Type     
		{        
			RepeatCount = 0,  // The number of times to repeat        
			RepeatDur,        // Duration of repeats        
			Speed,            // Fraction indicating how much faster                          
								// or slower to go than the normal duration.        
			Accelerate,       // Fraction between 0 and 1 indicating                          
								// portion of the duration over which to accelerate.        
								
			Decelerate,       // Fraction between 0 and 1 indicating portion of the  
								// duration over which to decelerate.        
			AutoReverse,      // The node should play forward then reverse.        
			Type_MaxEnumIDs     
		};

		enum PropertyUsedFlag  
		{     
			PUF_NONE             = 0,     
			PUF_Additive         = 1 << 0,     
			PUF_Accumulate       = 1 << 1,     
			PUF_AttributeNames   = 1 << 2,     
			PUF_TransformType    = 1 << 3,     
			PUF_FromFormula      = 1 << 4,     
			PUF_ToFormula        = 1 << 5,     
			PUF_ByFormula        = 1 << 6  
		}; 

		enum Additive
		{     
			BaseAdditive,     
			SumAdditive,     
			ReplaceAdditive,     
			MultiplyAdditive,     
			NoAdditive,     
			Additive_MaxEnumIDs  
		};

		enum Accumulate  
		{     
			NoAccumulate,     
			AlwaysAccumulate,     
			Accumulate_MaxEnumIDs  
		};    
		
		enum TransformType  
		{     
			PropertyTransformType,     
			ImageTransformType,     
			TransformType_MaxEnumIDs  
		}; 

		enum CalcMode  
		{     
			DiscreteMode,     
			LinearMode,     
			FormulaMode,     
			CalcMode_MaxEnumIDs  
		};    
		
		enum AnimatePropertyUsedFlag  
		{     
			APUF_NONE                = 0,     
			APUF_By                  = 1 << 0,     
			APUF_From                = 1 << 1,     
			APUF_To                  = 1 << 2,     
			APUF_CalcMode            = 1 << 3,  
			APUF_AnimationValues     = 1 << 4,     
			APUF_ValueType           = 1 << 5  
		}; 

		enum ValueType  
		{    
			StringType,     
			NumberType,     
			ColorType,     
			ValueType_MaxEnumIDs  
		}; 

		enum AnimateColorPropertyUsedFlag  
		{     
			ACPUF_NONE                = 0,     
			ACPUF_By                  = 1 << 0,     
			ACPUF_From                = 1 << 1,     
			ACPUF_To                  = 1 << 2,     
			ACPUF_ColorSpace          = 1 << 3,     
			ACPUF_Direction           = 1 << 4  
		};    
			
		enum ColorModel  
		{     
			RGBColorModel   = 0,     
			HSLColorModel   = 1,     
			IndexColorModel = 2,     
			ColorModel_MaxEnumIDs  
		}; 

		enum AnimateEffectPropertyUsedFlag  
		{     
			AEPUF_NONE                    = 0,     
			AEPUF_Transition              = 1 << 0,     
			AEPUF_Type                    = 1 << 1,     
			AEPUF_Progress                = 1 << 2,     
			AEPUF_RuntimeContext_Obsolete = 1 << 3 // Keep this for backward binary persistence  
		};   
		
		enum Transition  
		{     
			TransitionIn,     
			TransitionOut,     
			TransitionNone,     
			Transition_MaxEnumIDs  
		}; 

		enum AnimateMotionPropertyUsedFlag
		{     
			AMPUF_NONE                = 0,     
			AMPUF_By                  = 1 << 0,     
			AMPUF_From                = 1 << 1,     
			AMPUF_To                  = 1 << 2,     
			AMPUF_Origin              = 1 << 3,     
			AMPUF_Path                = 1 << 4,     
			AMPUF_Rotation            = 1 << 5,     
			AMPUF_EditRotation        = 1 << 6,     
			AMPUF_PointsTypes         = 1 << 7  
		};    
		
		enum Origin  
		{     
			DefaultOrigin,     
			ParentOrigin,     
			LayoutOrigin,     
			Origin_MaxEnumIDs  
		}; 

		enum AnimateRotationPropertyUsedFlag  
		{     
			ARPUF_NONE                = 0,     
			ARPUF_By                  = 1 << 0,     
			ARPUF_From                = 1 << 1,     
			ARPUF_To                  = 1 << 2,     
			ARPUF_Direction           = 1 << 3  
		};    
		
		enum Direction  
		{     
			Clockwise,     
			CounterClockwise,     
			Direction_MaxEnumIDs  
		};

		enum AnimateScalePropertyUsedFlag  
		{     
			ASPUF_NONE                = 0,     
			ASPUF_By                  = 1 << 0,     
			ASPUF_From                = 1 << 1,     
			ASPUF_To                  = 1 << 2,     
			ASPUF_ZoomContents        = 1 << 3  
		};

		enum SetPropertyUsedFlag  
		{     
			SPUF_NONE                = 0,     
			SPUF_To                  = 1 << 0,     
			SPUF_ValueType           = 1 << 1  
		};    

		enum CommandPropertyUsedFlag  
		{     
			CPUF_NONE                = 0,     
			CPUF_Type                = 1 << 0,     
			CPUF_Command             = 1 << 1  
		};    
		
		enum CommandType  
		{     
			EventType,     
			CallType,     
			OleVerbType,     
			CommandType_MaxEnumIDs  
		};

		enum TLTimePropertyID  
		{     
			tpidUnknown                  = 0,     
			tpidID                       = 1, // string     
			tpidDisplay                  = 2,     
			tpidMasterPos                = 5,     
			tpidSubNodeType              = 6,     
			tpidParagraphLevel           = 7,     
			tpidGraphLevel               = 8, 
			tpidEffectID                 = 9,     
			tpidEffectDir                = 10,     
			tpidEffectType               = 11,     
			tpidAfterEffect              = 13,     
			tpidDiagramLevel             = 14,     
			tpidSlideCount               = 15, // integer     
			tpidTimeFilter               = 16, // string     
			tpidEventFilter              = 17, // string     
			tpidHideWhenStopped          = 18, // boolean     
			tpidGroupID                  = 19,     
			tpidPPTType                  = 20,     
			tpidPlaceholderNode          = 21,    
			tpidMediaVolume              = 22,     
			tpidMediaMute                = 23,     
			tpidXMLUnknownAttribs        = 24,     
			tpidXMLAttribsUnknownValues  = 25,     
			tpidZoomToFullScreen         = 26,     
			tpidShowControls             = 27,     
			tpidDVDTitle                 = 28,     
			tpidDVDStartTime             = 29,     
			tpidDVDEndTime               = 30  
		}; 

		enum IterationType  
		{     
			AllAtOnce,     
			ByWord,
			ByLetter,     
			IterationType_MaxEnumIDs  
		}; 

		enum IntervalType  
		{     
			Seconds,     
			Percentage,     
			IntervalType_MaxEnumIDs  
		};    
		
		enum _Direction  
		{     
			Backwards,     
			Forwards,     
			_Direction_MaxEnumIDs  
		};    
		
		enum PropertyUsed 
		{     
			DirectionProperty      = 1 << 0,     
			IterationTypeProperty  = 1 << 1,     
			IntervalProperty       = 1 << 2,     
			IntervalTypeProperty   = 1 << 3,  
		};

		enum ConcurrencyType  
		{     
			Disabled,     
			Enabled,     
			ConcurrencyType_MaxEnumIDs  
		};    
		
		enum NextActionType  
		{     
			NoNextActionType,     
			Seek,
			NextActionType_MaxEnumIDs  
		}; 

		enum PreviousActionType  
		{     
			NoPreviousActionType,     
			SkipTimed,     
			PreviousActionType_MaxEnumIDs  
		};    
		
		enum EnableNextType  
		{     
			End,     
			Begin,     
			EnableNextType_MaxEnumIDs  
		};    
		
		// TLTimeSequenceData flags:  
		const int SF_Concurrency    = (1 << 0);  
		const int SF_NextAction     = (1 << 1);  
		const int SF_PreviousAction = (1 << 2);  
		const int SF_EnableNext     = (1 << 3);

		enum _Type  
		{     
			None     = -1,     
			Bool     = 0,     
			Int      = 1,     
			Float    = 2,     
			String   = 3,  
		};
	}
}
