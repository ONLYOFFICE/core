#pragma once

#include "resource.h"
#include "smartimage.h"
#include "patterns.h"

[object, oleautomation, uuid("242590FA-727C-4a8e-9784-6CB5B84FF90A"), dual, pointer_default(unique)]
__interface IImageCompose3 : IDispatch
{
	[id(100)] HRESULT Register([in] BSTR Name, [in] BSTR Password);
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
	
	[id(201), propget] HRESULT TilesWidth([out, retval] int* TileWidth);
	[id(201), propput] HRESULT TilesWidth([in] int TileWidth);
	[id(202), propget] HRESULT TilesHeight([out, retval] int* TileWidth);
	[id(202), propput] HRESULT TilesHeight([in] int TileHeight);
		
	[id(301)] HRESULT GetCategoriesCount([out, retval] long* Categories);
	[id(302)] HRESULT GetCategoryName([in] long Category, [out, retval] BSTR* Name);
	[id(303)] HRESULT GetEffectsCount([in] long Category, [out, retval] long* Count);
	[id(304)] HRESULT GetEffectName([in] long Category, [in] long Effect, [out, retval] BSTR* Name);
	[id(305)] HRESULT GetEffectName2([in] long Category, [in] long Effect, [out, retval] BSTR* Name);
	[id(306)] HRESULT GetEffectAt([in] long Category, [in] long Effect, [out, retval] long* ID);
	[id(307)] HRESULT Compose([in, satype("BYTE")] SAFEARRAY** First, [in, satype("BYTE")] SAFEARRAY** Last, [in, out, satype("BYTE")] SAFEARRAY** Result, [in] double Frame, [in] long EffectID);
	[id(308)] HRESULT Compose2([in, satype("BYTE")] SAFEARRAY** First, [in, satype("BYTE")] SAFEARRAY** Last, [in, out, satype("BYTE")] SAFEARRAY** Result, [in] double Frame, [in] long Category, [in] long Effect);

	[id(401)] HRESULT ComposeMedia([in] IUnknown* First, [in] IUnknown* Last, [in, out] IUnknown** Result, [in] double Frame, [in] long EffectID);
	[id(402)] HRESULT ComposeMedia2([in] IUnknown* First, [in] IUnknown* Last, [in, out] IUnknown** Result, [in] double Frame, [in] long Category, [in] long Effect);
};
	
[coclass, uuid("6126141F-86BA-4986-AC0D-64710B02785C"), threading(apartment), vi_progid("AVSImageCompose.ImageCompose3"), progid("AVSImageCompose.ImageCompose3.1"), version(1.0)]
class ATL_NO_VTABLE ImageCompose3 : public IImageCompose3
{
public:
	
	CSimpleArray<Category> m_categories;
	CSimpleArray<Effect> m_effects;
	Graduate m_graduate;
	Utility m_utility;
	Mask m_mask;
    MaskPinwheelCurved m_maskPinwheelCurved;
	PuzzleUtility m_puzzleUtility;
	MaskGraduate m_maskGraduate;

	int m_nSizeX;
	int m_nSizeY;
	double m_dFrame;

	int m_nTilesX;
	int m_nTilesY;
	int m_nTileSizeX;
	int m_nTileSizeY;

	Effect m_effect;

	SmartImage m_first;
	SmartImage m_last;
	SmartImage m_result;

	int m_EffectsType;
	
public:
	
	ImageCompose3()
	{
		// setup default values
		m_nSizeX = 0;
		m_nSizeY = 0;
		m_nTilesX = 0;
		m_nTilesY = 0;
		m_nTileSizeX = 0;
		m_nTileSizeY = 0;
		m_EffectsType = c_nEffectsAll;

		m_effect.ID = -1;

		// fill the list of effects
		AddEffects();
	}
	~ImageCompose3()
	{
	}
	
	void AddEffects()
	{
		m_categories.RemoveAll();
		m_effects.RemoveAll();

		// create effects list
		AddCategory(0, "Enhanced");
			AddEffect(2, "Burning Threshold",							"Burning\nThreshold",							4201, -1, -1);
			AddEffect(2, "Burning Fade",								"Burning\nFade",								4202, -1, -1);
			AddEffect(2, "Burning Fire",								"Burning\nFire",								4203, -1, -1);
			AddEffect(1, "Gaussian Blur", 								"Gaussian\nBlur", 								4801, -1, -1);
			AddEffect(0, "Color Noise", 								"Color\nNoise", 								4901, -1, -1);
			// AddEffect(0, "Grayscale Noise", 							"Grayscale\nNoise", 							4902, -1, -1);
			AddEffect(1, "Pixelate",                                    "Pixelate",			   		                    5701, -2, -2);
			AddEffect(0, "Flash Light", 								"Flash\nLight", 								4903, -1, -1);
			AddEffect(0, "Flash Dark", 									"Flash\nDark", 									4904, -1, -1);
		AddCategory(0, "Transform");
			AddEffect(1, "Twirl Clockwise",								"Twirl\nClockwise",								3701, -1, -1);
			AddEffect(1, "Twirl Clockwise Zoom Out",					"Twirl Clockwise\n Zoom Out",					3702, -1, -1);
			AddEffect(2, "Twirl Clockwise Zoom In",						"Twirl Clockwise\nZoom In",						3703, -1, -1);
			AddEffect(1, "Twirl Reverse",								"Twirl\nReverse",								3704, -1, -1);
			AddEffect(1, "Twirl Reverse Zoom Out",						"Twirl Reverse\nZoom Out",						3705, -1, -1);
			AddEffect(2, "Twirl Revers Zoom In",						"Twirl Reverse\nZoom In",						3706, -1, -1);
			AddEffect(2, "Twirl Drain",						            "Twirl \nDrain",						        3707, -1, -1);
			AddEffect(2, "Twirl Reverse Drain",						    "Twirl Reverse\nDrain",					    	3708, -1, -1);
			AddEffect(2, "Twist Left To Right", 						"Twist Left\nTo Right", 						3901, -1, -1);
			AddEffect(2, "Twist Right To Left", 						"Twist Right\nTo Left", 						3902, -1, -1);
			AddEffect(2, "Twist Top To Bottom", 						"Twist Top\nTo Bottom", 						3903, -1, -1);
			AddEffect(2, "Twist Bottom To Top", 						"Twist Bottom\nTo Top", 						3904, -1, -1);
			AddEffect(2, "Twist Left Top",	 							"Twist\nLeft Top", 								3905, -1, -1);
			AddEffect(2, "Twist Left Bottom",	 						"Twist\nLeft Bottom", 							3906, -1, -1);
			AddEffect(2, "Twist Right Top",	 							"Twist\nRight Top", 							3907, -1, -1);
			AddEffect(2, "Twist Right Bottom",	 						"Twist\nRight Bottom", 							3908, -1, -1);
			AddEffect(2, "Ripple Outside",								"Ripple\nOutside",								4001, -1, -1);
			AddEffect(2, "Ripple Inside",								"Ripple\nInside",								4002, -1, -1);
			AddEffect(2, "Sphere Inside",								"Sphere\nInside",								3801, -1, -1);
			AddEffect(1, "Sphere Outside",								"Sphere\nOutside",								3802, -1, -1);
			AddEffect(0, "Wave Horizontal",								"Wave\nHorizontal",								4101, -1, -1);
			AddEffect(0, "Wave Vertical",								"Wave\nVertical",								4102, -1, -1);
		AddCategory(1, "Movie");
			AddEffect(1, "Movie Right",									"Movie\nRight To Left",							5501, -2, -2);
			AddEffect(1, "Movie Left",									"Movie\nLeft To Right",							5502, -2, -2);
			AddEffect(1, "Movie Top",									"Movie\nUp To Bottom",							5503, -2, -2);
			AddEffect(1, "Movie Bottom",								"Movie\nBottom To Up",							5504, -2, -2);
			AddEffect(1, "Movie Right2",								"Movie Fade\nRight To Left",					5505, -2, -2);
			AddEffect(1, "Movie Left2",									"Movie Fade\nLeft To Right",					5506, -2, -2);
			AddEffect(1, "Movie Top 2",									"Movie Fade\nUp To Bottom",						5507, -2, -2);
			AddEffect(1, "Movie Bottom 2",								"Movie Fade\nBottom To Up",						5508, -2, -2);
			AddEffect(1, "Movie Right no Zoom",							"Shot change Grey\nRight To Left",				5509, -2, -2);
			AddEffect(1, "Movie Left no Zoom",							"Shot change Grey\nLeft To Right",				5510, -2, -2);
			AddEffect(1, "Movie Top no Zoom",							"Shot change Grey\nUp To Bottom",				5511, -2, -2);
			AddEffect(1, "Movie Bottom no Zoom",						"Shot change Grey\nBottom To Up",				5512, -2, -2);
			AddEffect(1, "Movie Right no Zoom 2",						"Shot change Black\nRight To Left",				5513, -2, -2);
			AddEffect(1, "Movie Left no Zoom 2",						"Shot change Black\nLeft To Right",				5514, -2, -2);
			AddEffect(1, "Movie Top no Zoom 2",							"Shot change Black\nUp To Bottom",				5515, -2, -2);
			AddEffect(1, "Movie Bottom no Zoom 2",						"Shot change Black\nBottom To Up",				5516, -2, -2);
		AddCategory(1, "Games");
			AddEffect(1, "Game Tertis",									"Game\nTertis",									5401, -2, -2);
			AddEffect(1, "Game Tertis Collapse",						"Game\nTertis Collapse",						5402, -2, -2);
		AddCategory(1, "Puzzle");
			AddEffect(1, "Game Puzzle   4x4",						    "Game\nPuzzle   4x4",						    6001, -2, -2);
			AddEffect(1, "Game Puzzle   7x7",						    "Game\nPuzzle   7x7",						    6002, -2, -2);
			AddEffect(1, "Game Puzzle 10x10",						    "Game\nPuzzle 10x10",						    6003, -2, -2);
			AddEffect(1, "Game Puzzle   4x4 Collapse",  			    "Game\nPuzzle   4x4 Collapse",					6004, -2, -2);
			AddEffect(1, "Game Puzzle   7x7 Collapse",					"Game\nPuzzle   7x7 Collapse",					6005, -2, -2);
			AddEffect(1, "Game Puzzle 10x10 Collapse",					"Game\nPuzzle 10x10 Collapse",					6006, -2, -2);
			AddEffect(1, "Game Tile 4x4 Consecutive",					"Game\nGame Tile 4x4 Consecutive",				6007, -2, -2);
			AddEffect(1, "Game Tile 4x4 Randomly",						"Game\nGame Tile 4x4 Randomly",					6008, -2, -2);
			AddEffect(1, "Game Tile 7x7 Consecutive",					"Game\nGame Tile 7x7 Consecutive",				6009, -2, -2);
			AddEffect(1, "Game Tile 7x7 Randomly",						"Game\nGame Tile 7x7 Randomly",					6010, -2, -2);
			AddEffect(1, "Game Tile 10x10 Consecutive",					"Game\nGame Tile 10x10 Consecutive",			6011, -2, -2);
			AddEffect(1, "Game Tile 10x10 Randomly",					"Game\nGame Tile 10x10 Randomly",				6012, -2, -2);
			AddEffect(1, "Game Tile 4x4 Consecutive Collapse",			"Game\nGame Tile 4x4 Consecutive Collapse",		6013, -2, -2);
			AddEffect(1, "Game Tile 4x4 Randomly Collapse",				"Game\nGame Tile 4x4 Randomly Collapse",		6014, -2, -2);
			AddEffect(1, "Game Tile 7x7 Consecutive Collapse",			"Game\nGame Tile 7x7 Consecutive Collapse",		6015, -2, -2);
			AddEffect(1, "Game Tile 7x7 Randomly Collapse",				"Game\nGame Tile 7x7 Randomly Collapse",		6016, -2, -2);
			AddEffect(1, "Game Tile 10x10 Consecutive Collapse",		"Game\nGame Tile 10x10 Consecutive Collapse",	6017, -2, -2);
			AddEffect(1, "Game Tile 10x10 Randomly Collapse",			"Game\nGame Tile 10x10 Randomly Collapse",		6018, -2, -2);
			AddEffect(2, "Small Hexagons",							    "Small Hexagons",							    6201, -1, -1);
			AddEffect(2, "Large Hexagons",							    "Large Hexagons",							    6202, -1, -1);
			AddEffect(2, "Small Hexagons, Collapse",					"Small Hexagons, Collapse",						6203, -1, -1);
			AddEffect(2, "Large Hexagons, Collapse",					"Large Hexagons, Collapse",						6204, -1, -1);
			AddEffect(2, "Small Rhombuses",							    "Small Rhombuses",							    6205, -1, -1);
			AddEffect(2, "Large Rhombuses",							    "Large Rhombuses",							    6206, -1, -1);
			AddEffect(2, "Small Rhombuses, Collapse",					"Small Rhombuses, Collapse",					6207, -1, -1);
			AddEffect(2, "Large Rhombuses, Collapse",					"Large Rhombuses, Collapse",					6208, -1, -1);
			AddEffect(2, "Small Bricks",							    "Small Bricks",							        6209, -1, -1);
			AddEffect(2, "Large Bricks",							    "Large Bricks",							        6210, -1, -1);
			AddEffect(2, "Small Bricks 2",							    "Small Bricks 2",							    6211, -1, -1);
			AddEffect(2, "Large Bricks 2",							    "Large Bricks 2",							    6212, -1, -1);
			AddEffect(2, "Small Bricks 3",							    "Small Bricks 3",							    6213, -1, -1);
			AddEffect(2, "Large Bricks 3",							    "Large Bricks 3",							    6214, -1, -1);
			AddEffect(2, "Small Bricks, Collapse",						"Small Bricks, Collapse",						6215, -1, -1);
			AddEffect(2, "Large Bricks, Collapse",						"Large Bricks, Collapse",						6216, -1, -1);
			AddEffect(2, "Small Triangles",							    "Small Triangles",							    6217, -1, -1);
			AddEffect(2, "Large Triangles",							    "Large Triangles",							    6218, -1, -1);
			AddEffect(2, "Small Triangles 2",							"Small Triangles 2",							6219, -1, -1);
			AddEffect(2, "Large Triangles 2",							"Large Triangles 2",							6220, -1, -1);
			AddEffect(2, "Small Triangles 3",							"Small Triangles 3",							6221, -1, -1);
			AddEffect(2, "Large Triangles 3",							"Large Triangles 3",							6222, -1, -1);
			AddEffect(2, "Small Triangles, Collapse",					"Small Triangles, Collapse",					6223, -1, -1);
			AddEffect(2, "Large Triangles, Collapse",					"Large Triangles, Collapse",					6224, -1, -1);
			AddEffect(2, "Small Inclined Bricks",						"Small Inclined Bricks",						6225, -1, -1);
			AddEffect(2, "Large Inclined Bricks",						"Large Inclined Bricks",						6226, -1, -1);
			AddEffect(2, "Small Inclined Bricks 2",						"Small Inclined Bricks 2",						6227, -1, -1);
			AddEffect(2, "Large Inclined Bricks 2",						"Large Inclined Bricks 2",						6228, -1, -1);
			AddEffect(2, "Small Inclined Bricks, Collapse",				"Small Inclined Bricks, Collapse",				6229, -1, -1);
			AddEffect(2, "Large Inclined Bricks, Collapse",				"Large Inclined Bricks, Collapse",				6230, -1, -1);
		AddCategory(1, "Effects");
			AddEffect(1, "Effect Zebra",								"Effect Zebra",									5601, -2, -2);
			AddEffect(1, "Effect Zebra Inverse",						"Effect Zebra Inverse",							5602, -2, -2);
		AddCategory(0, "Rotate");
			AddEffect(1, "Roll Right",                                  "Roll\nRight",			   		                5702, -2, -2);
			AddEffect(1, "Roll Left",                                   "Roll\nLeft",			   		                5703, -2, -2);
			AddEffect(0, "Rotate Single In",							"Rotate\nSingle In",							301, -1, -1);
			AddEffect(0, "Rotate Twice In",								"Rotate\nTwice In",								302, -1, -1);
			AddEffect(0, "Rotate Single Out",							"Rotate\nSingle Out",							303, -1, -1);
			AddEffect(0, "Rotate Twice Out",							"Rotate\nTwice Out",							304, -1, -1);
			AddEffect(0, "Rotate Half In",				                "Rotate \nHalf In",				                305, -1, -1);
			AddEffect(0, "Rotate Half Out",				                "Rotate \nHalf Out",				            306, -1, -1);
			AddEffect(0, "Rotate Fade Single In",						"Rotate Fade\nSingle In",						307, -1, -1);
			AddEffect(0, "Rotate Fade Twice In",						"Rotate Fade\nTwice In",						308, -1, -1);
			AddEffect(0, "Rotate Fade Single Out",						"Rotate Fade\nSingle Out",						309, -1, -1);
			AddEffect(0, "Rotate Fade Twice Out",						"Rotate Fade\nTwice Out",						310, -1, -1);
			AddEffect(0, "Rotate Fade Half In",							"Rotate Fade\nHalf In",							311, -1, -1);
			AddEffect(0, "Rotate Fade Half Out",					    "Rotate Fade\nHalf Out",					    312, -1, -1);
			AddEffect(0, "Rotate Spiral Top Left",				        "Rotate Spiral\nTop Left",			            5801, -1, -1);
			AddEffect(0, "Rotate Spiral Top Right",				        "Rotate Spiral\nTop Right",				        5802, -1, -1);
			AddEffect(0, "Rotate Spiral Bottom Left",			        "Rotate Spiral\nBottom Left",			        5803, -1, -1);
			AddEffect(0, "Rotate Spiral Bottom Right",				    "Rotate Spiral\nBottom Right",			        5804, -1, -1);
			AddEffect(0, "Rotate Spiral Center In",						"Rotate Spiral\nCenter In",		  	            5805, -1, -1);
			AddEffect(0, "Rotate Spiral Reverse Center In",				"Rotate Spiral\nReverse Center In",				5806, -1, -1);
			AddEffect(0, "Rotate Spiral Center Out",					"Rotate Spiral\nCenter Out",		  	        5807, -1, -1);
			AddEffect(0, "Rotate Spiral Reverse Center Out",			"Rotate Spiral\nReverse Center Out",			5808, -1, -1);
		AddCategory(0, "Flip");
			AddEffect(1, "Flip Over Vertical Back",						"Flip Over\nVertical Back",						2503, -1, -1);
			AddEffect(1, "Flip Over Vertical",							"Flip Over\nVertical",							2501, -1, -1);
			AddEffect(1, "Flip Over Horizontal Back",					"Flip Over\nHorizontal Back",					2507, -1, -1);
			AddEffect(1, "Flip Over Horizontal",						"Flip Over\nHorizontal",						2505, -1, -1);
			AddEffect(1, "Flip Away Vertical In Back",					"Flip Away\nVertical In Back",					2601, -1, -1);
			AddEffect(1, "Flip Away Vertical Out Back",					"Flip Away\nVertical Out Back", 				2602, -1, -1);
			AddEffect(1, "Flip Away Vertical In",						"Flip Away\nVertical In",						2603, -1, -1);
			AddEffect(1, "Flip Away Vertical Out",						"Flip Away\nVertical Out",						2604, -1, -1);
			AddEffect(1, "Flip Away Horizontal In Back",				"Flip Away\nHorizontal In Back",				2605, -1, -1);
			AddEffect(1, "Flip Away Horizontal Out Back",				"Flip Away\nHorizontal Out Back",				2606, -1, -1);
			AddEffect(1, "Flip Away Horizontal In",						"Flip Away\nHorizontal In",						2607, -1, -1);
			AddEffect(1, "Flip Away Horizontal Out",					"Flip Away\nHorizontal Out",					2608, -1, -1);
			AddEffect(2, "Flip Fly Horizontal In",						"Flip Fly\nHorizontal In",						4605, -1, -1);
			AddEffect(2, "Flip Fly Horizontal Out",						"Flip Fly\nHorizontal Out",						4607, -1, -1);
			AddEffect(2, "Flip Fly Vertical In",						"Flip Fly\nVertical In",						4601, -1, -1);
			AddEffect(2, "Flip Fly Vertical Out",						"Flip Fly\nVertical Out",						4603, -1, -1);
			AddEffect(2, "Flip Boxes Left To Right",					"Flip Boxes\nLeft To Right",					5001, -8, -8);
			AddEffect(2, "Flip Boxes Right To Left",					"Flip Boxes\nRight To Left",					5002, -8, -8);
			AddEffect(2, "Flip Boxes Top To Bottom",					"Flip Boxes\nTop To Bottom",					5003, -8, -8);
			AddEffect(2, "Flip Boxes Bottom To Top",					"Flip Boxes\nBottom To Top",					5004, -8, -8);
			AddEffect(1, "Flip Out Boxes Vertical",						"Flip Out Boxes\nVertical",						5101, -4, -4);
			AddEffect(1, "Flip Out Boxes Vertical Back",				"Flip Out Boxes\nVertical Back",				5103, -4, -4);
			AddEffect(1, "Flip Out Boxes Horizontal",					"Flip Out Boxes\nHorizontal",					5105, -4, -4);
			AddEffect(1, "Flip Out Boxes Horizontal Back",				"Flip Out Boxes\nHorizontal Back",				5107, -4, -4);
		AddCategory(1, "Swing");
			AddEffect(1, "Swing Side Left In",							"Swing Side\nLeft In",							2001, -1, -1);
			AddEffect(1, "Swing Side Left Out",							"Swing Side\nLeft Out",							2002, -1, -1);
			AddEffect(1, "Swing Side Right In",							"Swing Side\nRight In",							2003, -1, -1);
			AddEffect(1, "Swing Side Right Out",						"Swing Side\nRight Out",						2004, -1, -1);
			AddEffect(1, "Swing Side Top In",							"Swing Side\nTop In",							2007, -1, -1);
			AddEffect(1, "Swing Side Top Out",							"Swing Side\nTop Out",							2008, -1, -1);
			AddEffect(1, "Swing Side Bottom In",						"Swing Side\nBottom In",						2005, -1, -1);
			AddEffect(1, "Swing Side Bottom Out",						"Swing Side\nBottom Out",						2006, -1, -1);
			AddEffect(1, "Swing Side Change Left", 						"Swing Side\nChange Left", 						2101, -1, -1);
			AddEffect(1, "Swing Side Change Right", 					"Swing Side\nChange Right", 					2102, -1, -1);
			AddEffect(1, "Swing Side Change Top", 						"Swing Side\nChange Top", 						2103, -1, -1);
			AddEffect(1, "Swing Side Change Bottom", 					"Swing Side\nChange Bottom", 					2104, -1, -1);
			AddEffect(1, "Swing Doors Vertical In",						"Swing Doors\nVertical In",						2201, -1, -1);
			AddEffect(1, "Swing Doors Vertical Out",					"Swing Doors\nVertical Out",					2202, -1, -1);
			AddEffect(1, "Swing Doors Horizontal In",					"Swing Doors\nHorizontal In",					2203, -1, -1);
			AddEffect(1, "Swing Doors Horizontal Out",					"Swing Doors\nHorizontal Out",					2204, -1, -1);
			AddEffect(1, "Swing Doors Change Vertical",					"Swing Doors\nChange Vertical",					2302, -1, -1);
			AddEffect(1, "Swing Doors Change Horizontal",				"Swing Doors\nChange Horizontal",				2304, -1, -1);
		AddCategory(1, "Shatter");
			AddEffect(1, "Shatter Whirl Wind",						    "Shatter\nWhirl Wind",	     			        3305, -1, -1);
			AddEffect(1, "Shatter Corner Right Bottom", 				"Shatter Corner\nRight Bottom", 				3205, -1, -1);
			AddEffect(1, "Shatter Corner Right Top", 					"Shatter Corner\nRight Top", 					3206, -1, -1);
			AddEffect(1, "Shatter Corner Left Top", 					"Shatter Corner\nLeft Top", 					3207, -1, -1);
			AddEffect(1, "Shatter Corner Left Bottom", 					"Shatter Corner\nLeft Bottom", 					3208, -1, -1);
			AddEffect(1, "Shatter Side Left To Right", 					"Shatter Side\nLeft To Right",					3301, -1, -1);
			AddEffect(1, "Shatter Side Right To Left", 					"Shatter Side\nRight To Left",					3302, -1, -1);
			AddEffect(1, "Shatter Side Top To Bottom", 					"Shatter Side\nTop To Bottom",					3303, -1, -1);
			AddEffect(1, "Shatter Side Bottom To Top", 					"Shatter Side\nBottom To Top",					3304, -1, -1);
			AddEffect(2, "Shatter Center Inside High",					"Shatter Center\nInside High",					3401, -1, -1);
			AddEffect(2, "Shatter Center Inside Low",					"Shatter Center\nInside Low",					3402, -1, -1);
		AddCategory(0, "Diffuse");
			AddEffect(0, "Diffuse Left To Right", 						"Diffuse\nLeft To Right", 						201, -1, -1);
			AddEffect(0, "Diffuse Right To Left", 						"Diffuse\nRight To Left", 						202, -1, -1);
			AddEffect(0, "Diffuse Top To Bottom", 						"Diffuse\nTop To Bottom", 						203, -1, -1);
			AddEffect(0, "Diffuse Bottom To Top", 						"Diffuse\nBottom To Top", 						204, -1, -1);
		AddCategory(0, "Page");
			AddEffect(1, "Page Turn 3D Left Top", 						"Page Turn 3D\nLeft Top",						2409, -1, -1);
			AddEffect(1, "Page Turn 3D Left Bottom", 					"Page Turn 3D\nLeft Bottom",					2410, -1, -1);
			AddEffect(1, "Page Turn 3D Right Bottom", 					"Page Turn 3D\nRight Bottom",					2411, -1, -1);
			AddEffect(1, "Page Turn 3D Right Top", 						"Page Turn 3D\nRight Top",						2412, -1, -1);
			AddEffect(1, "Page Turn 3D Left Top 2", 					"Page Turn 3D\nLeft Top 2",						2420, -1, -1);
			AddEffect(1, "Page Turn 3D Left Bottom 2",	 				"Page Turn 3D\nLeft Bottom 2",					2421, -1, -1);
			AddEffect(1, "Page Turn 3D Right Bottom 2",					"Page Turn 3D\nRight Bottom 2",					2422, -1, -1);
			AddEffect(1, "Page Turn 3D Right Top 2", 					"Page Turn 3D\nRight Top 2",					2423, -1, -1);
			AddEffect(1, "Page Turn Album Left Top", 					"Page Turn\nAlbum Left Top",					2424, -1, -1);
			AddEffect(1, "Page Turn Album Right Top", 					"Page Turn\nAlbum Right Top",					2425, -1, -1);
			AddEffect(1, "Page Turn Album Right Bottom", 				"Page Turn\nAlbum Right Bottom",				2426, -1, -1);
			AddEffect(1, "Page Turn Album Left Bottom", 				"Page Turn\nAlbum Left Bottom",					2427, -1, -1);
			AddEffect(1, "Page Turn Album Left Top Corners1", 			"Page Turn\nAlbum Left Top Corners1",			2428, -1, -1);
			AddEffect(1, "Page Turn Album Right Top Corners1", 			"Page Turn\nAlbum Right Top Corners1",			2429, -1, -1);
			AddEffect(1, "Page Turn Album Right Bottom Corners1", 		"Page Turn\nAlbum Right Bottom Corners1",		2430, -1, -1);
			AddEffect(1, "Page Turn Album Left Bottom Corners1", 		"Page Turn\nAlbum Left Bottom Corners1",		2431, -1, -1);
			AddEffect(1, "Page Turn Album Left Top Corners2", 			"Page Turn\nAlbum Left Top Corners2",			2432, -1, -1);
			AddEffect(1, "Page Turn Album Right Top Corners2", 			"Page Turn\nAlbum Right Top Corners2",			2433, -1, -1);
			AddEffect(1, "Page Turn Album Right Bottom Corners2", 		"Page Turn\nAlbum Right Bottom Corners2",		2434, -1, -1);
			AddEffect(1, "Page Turn Album Left Bottom Corners2", 		"Page Turn\nAlbum Left Bottom Corners2",		2435, -1, -1);
			AddEffect(1, "Page Turn Album Left Top Corners3", 			"Page Turn\nAlbum Left Top Corners3",			2436, -1, -1);
			AddEffect(1, "Page Turn Album Right Top Corners3", 			"Page Turn\nAlbum Right Top Corners3",			2437, -1, -1);
			AddEffect(1, "Page Turn Album Right Bottom Corners3", 		"Page Turn\nAlbum Right Bottom Corners3",		2438, -1, -1);
			AddEffect(1, "Page Turn Album Left Bottom Corners3", 		"Page Turn\nAlbum Left Bottom Corners3",		2439, -1, -1);
			AddEffect(1, "Page Turn Album Left Top Corners4", 			"Page Turn\nAlbum Left Top Corners4",			2440, -1, -1);
			AddEffect(1, "Page Turn Album Right Top Corners4", 			"Page Turn\nAlbum Right Top Corners4",			2441, -1, -1);
			AddEffect(1, "Page Turn Album Right Bottom Corners4", 		"Page Turn\nAlbum Right Bottom Corners4",		2442, -1, -1);
			AddEffect(1, "Page Turn Album Left Bottom Corners4", 		"Page Turn\nAlbum Left Bottom Corners4",		2443, -1, -1);
		AddCategory(0, "Fade");
			AddEffect(0, "Fade Solid",									"Fade\nSolid",									1, -1, -1);
			AddEffect(1, "Fade Zoom In",								"Fade\nZoom In",								14, -1, -1);
			AddEffect(1, "Fade Zoom Out",								"Fade\nZoom Out",								15, -1, -1);
			AddEffect(1, "Fade Zoom Both",								"Fade\nZoom Both",								16, -1, -1);
			AddEffect(0, "Fade Dots Small",								"Fade\nDots Small",								2, 8, 8);
			AddEffect(0, "Fade Dots Dense Small",						"Fade\nDots Dense Small",						3, 8, 8);
			AddEffect(0, "Fade Grid Small",								"Fade\nGrid Small",								4, 8, 8);
			AddEffect(0, "Fade Grid Rotated Small",						"Fade Grid\nRotated Small",						5, 8, 8);
			AddEffect(0, "Fade Line Vertical Small",					"Fade Line\nVertical Small",					6, 8, 8);
			AddEffect(0, "Fade Line Horizontal Small",					"Fade Line\nHorizontal Small",					7, 8, 8);
			AddEffect(0, "Fade Line Diagonal Small",					"Fade Line\nDiagonal Small",					8, 8, 8);
			AddEffect(0, "Fade Line Diagonal Rotated Small",			"Fade Line\nDiagonal Rotated Small",			9, 8, 8);
			AddEffect(0, "Fade Chess Small",							"Fade\nChess Small",							10, 8, 8);
			AddEffect(0, "Fade Dashes Small",							"Fade\nDashes Small",							11, 8, 8);
			AddEffect(0, "Fade Spiner Small",							"Fade\nSpiner Small",							12, 8, 8);
			AddEffect(0, "Fade Thatches Small",							"Fade\nThatches Small",							13, 8, 8);
			AddEffect(0, "Fade Heart Small",							"Fade\nHeart Small",							17, 8, 8);
			AddEffect(0, "Fade Cross Small",							"Fade\nCross Small",							18, 8, 8);
			AddEffect(0, "Fade Dots",									"Fade\nDots",									32, 12, 12);
			AddEffect(0, "Fade Dots Dense",								"Fade\nDots Dense",								33, 12, 12);
			AddEffect(0, "Fade Grid",									"Fade\nGrid",									34, 12, 12);
			AddEffect(0, "Fade Grid Rotated",							"Fade Grid\nRotated",							35, 12, 12);
			AddEffect(0, "Fade Line Vertical",							"Fade Line\nVertical",							36, 12, 12);
			AddEffect(0, "Fade Line Horizontal",						"Fade Line\nHorizontal",						37, 12, 12);
			AddEffect(0, "Fade Line Diagonal",							"Fade Line\nDiagonal",							38, 12, 12);
			AddEffect(0, "Fade Line Diagonal Rotated",					"Fade Line\nDiagonal Rotated",					39, 12, 12);
			AddEffect(0, "Fade Chess",									"Fade\nChess",									40, 12, 12);
			AddEffect(0, "Fade Dashes",									"Fade\nDashes",									41, 12, 12);
			AddEffect(0, "Fade Spiner",									"Fade\nSpiner",									42, 12, 12);
			AddEffect(0, "Fade Thatches",								"Fade\nThatches",								43, 12, 12);
			AddEffect(0, "Fade Heart",									"Fade\nHeart",									47, 12, 12);
			AddEffect(0, "Fade Cross",									"Fade\nCross",									48, 12, 12);
			AddEffect(0, "Fade Dots Large",								"Fade\nDots Large",								62, 16, 16);
			AddEffect(0, "Fade Dots Dense Large",						"Fade\nDots Dense Large",						63, 16, 16);
			AddEffect(0, "Fade Grid Large",								"Fade\nGrid Large",								64, 16, 16);
			AddEffect(0, "Fade Grid Rotated Large",						"Fade Grid\nRotated Large",						65, 16, 16);
			AddEffect(0, "Fade Line Vertical Large",					"Fade Line\nVertical Large",					66, 16, 16);
			AddEffect(0, "Fade Line Horizontal Large",					"Fade Line\nHorizontal Large",					67, 16, 16);
			AddEffect(0, "Fade Line Diagonal Large",					"Fade Line\nDiagonal Large",					68, 16, 16);
			AddEffect(0, "Fade Line Diagonal Rotated Large",			"Fade Line\nDiagonal Rotated Large",			69, 16, 16);
			AddEffect(0, "Fade Chess Large",							"Fade\nChess Large",							70, 16, 16);
			AddEffect(0, "Fade Dashes Large",							"Fade\nDashes Large",							71, 16, 16);
			AddEffect(0, "Fade Spiner Large",							"Fade\nSpiner Large",							72, 16, 16);
			AddEffect(0, "Fade Thatches Large",							"Fade\nThatches Large",							73, 16, 16);
			AddEffect(0, "Fade Heart Large",							"Fade\nHeart Large",							77, 16, 16);
			AddEffect(0, "Fade Cross Large",							"Fade\nCross Large",							78, 16, 16);
		AddCategory(0, "Mosaic");
			AddEffect(0, "Mosaic Spiral In",							"Mosaic\nSpiral In",							101, -1, -1);
			AddEffect(0, "Mosaic Spiral Out",							"Mosaic\nSpiral Out",							102, -1, -1);
			AddEffect(0, "Mosaic Strips",								"Mosaic\nStrips",								103, -1, -1);
			AddEffect(0, "Mosaic Dissolve",								"Mosaic\nDissolve",								104, 8, 8);
			AddEffect(0, "Mosaic Clockwise",							"Mosaic\nClockwise",							105, 8, 8);
			AddEffect(0, "Mosaic Counterclockwise",						"Mosaic Counter \nClockwise",					106, 8, 8);
			AddEffect(0, "Mosaic Random",								"Mosaic\nRandom",								107, 8, 8);
			AddEffect(0, "Mosaic Left Top", 							"Mosaic\nLeft Top",								108, -1, -1);
			AddEffect(0, "Mosaic Right Top", 							"Mosaic\nRight Top", 							109, -1, -1);
			AddEffect(0, "Mosaic Right Bottom", 						"Mosaic\nRight Bottom", 						110, -1, -1);
			AddEffect(0, "Mosaic Left Bottom", 							"Mosaic\nLeft Bottom", 							111, -1, -1);
			AddEffect(0, "Mosaic Wall Left To Right", 					"Mosaic Wall\nLeft To Right", 					112, -1, -1);
			AddEffect(0, "Mosaic Wall Right To Left", 					"Mosaic Wall\nRight To Left", 					113, -1, -1);
			AddEffect(0, "Mosaic Wall Top To Bottom", 					"Mosaic Wall\nTop To Bottom", 					114, -1, -1);
			AddEffect(0, "Mosaic Wall Bottom To Top", 					"Mosaic Wall\nBottom To Top", 					115, -1, -1);
			AddEffect(0, "Mosaic Chess Left To Right", 					"Mosaic Chess\nLeft To Right", 					116, -1, -1);
			AddEffect(0, "Mosaic Chess Right To Left", 					"Mosaic Chess\nRight To Left", 					117, -1, -1);
			AddEffect(0, "Mosaic Chess Top To Bottom", 					"Mosaic Chess\nTop To Bottom", 					118, -1, -1);
			AddEffect(0, "Mosaic Chess Bottom To Top", 					"Mosaic Chess\nBottom To Top", 					119, -1, -1);
		AddCategory(0, "Clock");
			AddEffect(0, "Clock Corner Left Top", 						"Clock Corner\nLeft Top", 						4501, -1, -1);
			AddEffect(0, "Clock Corner Left Bottom", 					"Clock Corner\nLeft Bottom", 					4502, -1, -1);
			AddEffect(0, "Clock Corner Right Top", 						"Clock Corner\nRight Top", 						4503, -1, -1);
			AddEffect(0, "Clock Corner Right Bottom", 					"Clock Corner\nRight Bottom", 					4504, -1, -1);
			AddEffect(0, "Clock Corner Reverse Left Top", 				"Clock Corner\nReverse Left Top", 				4505, -1, -1);
			AddEffect(0, "Clock Corner Reverse Left Bottom", 			"Clock Corner\nReverse Left Bottom", 			4506, -1, -1);
			AddEffect(0, "Clock Corner Reverse Right Top", 				"Clock Corner\nReverse Right Top", 				4507, -1, -1);
			AddEffect(0, "Clock Corner Reverse Right Bottom", 			"Clock Corner\nReverse Right Bottom", 			4508, -1, -1);
			AddEffect(0, "Clock Bottom Clockwise",						"Clock\nBottom Clockwise",						3001, -1, -1);
			AddEffect(0, "Clock Bottom Reverse",						"Clock\nBottom Reverse",						3002, -1, -1);
			AddEffect(0, "Clock Bottom Opposite",						"Clock\nBottom Opposite",						3003, -1, -1);
			AddEffect(0, "Clock Top Clockwise",							"Clock\nTop Clockwise",							3004, -1, -1);
			AddEffect(0, "Clock Top Reverse",							"Clock\nTop Reverse",							3005, -1, -1);
			AddEffect(0, "Clock Top Opposite",							"Clock\nTop Opposite",							3006, -1, -1);
			AddEffect(0, "Clock Left Clockwise",						"Clock\nLeft Clockwise",						3007, -1, -1);
			AddEffect(0, "Clock Left Reverse",							"Clock\nLeft Reverse",							3008, -1, -1);
			AddEffect(0, "Clock Left Opposite",							"Clock\nLeft Opposite",							3009, -1, -1);
			AddEffect(0, "Clock Right Clockwise",						"Clock\nRight Clockwise",						3010, -1, -1);
			AddEffect(0, "Clock Right Reverse",							"Clock\nRight Reverse",							3011, -1, -1);
			AddEffect(0, "Clock Right Opposite",						"Clock\nRight Opposite",						3012, -1, -1);
			AddEffect(1, "Clock 2 Angles Clockwise",					"Clock 2\nAngles Clockwise",					3013, -1, -1);
			AddEffect(1, "Clock 2 Angles Reverse",						"Clock 2\nAngles Reverse",						3014, -1, -1);
			AddEffect(1, "Clock 3 Angles Clockwise",					"Clock 3\nAngles Clockwise",					3015, -1, -1);
			AddEffect(1, "Clock 3 Angles Reverse",						"Clock 3\nAngles Reverse",						3016, -1, -1);
			AddEffect(1, "Clock 4 Angles Clockwise",					"Clock 4\nAngles Clockwise",					3017, -1, -1);
			AddEffect(1, "Clock 4 Angles Reverse",						"Clock 4\nAngles Reverse",						3018, -1, -1);
			AddEffect(1, "Clock 6 Angles Clockwise",					"Clock 6\nAngles Clockwise",					3019, -1, -1);
			AddEffect(1, "Clock 6 Angles Reverse",						"Clock 6\nAngles Reverse",						3020, -1, -1);
			AddEffect(1, "Clock 8 Angles Clockwise",					"Clock 8\nAngles Clockwise",					3021, -1, -1);
			AddEffect(1, "Clock 8 Angles Reverse",						"Clock 8\nAngles Reverse",						3022, -1, -1);
			AddEffect(1, "Clock 12 Angles Clockwise",					"Clock 12\nAngles Clockwise",					3023, -1, -1);
			AddEffect(1, "Clock 12 Angles Reverse",						"Clock 12\nAngles Reverse",						3024, -1, -1);
		AddCategory(0, "Wipe");
			AddEffect(0, "Wipe Door Vertical In",						"Wipe Door\nVertical In",						401, -4, -4);
			AddEffect(0, "Wipe Door Vertical Out",						"Wipe Door\nVertical Out",						402, -4, -4);
			AddEffect(0, "Wipe Door Horizontal In",						"Wipe Door\nHorizontal In",						403, -4, -4);
			AddEffect(0, "Wipe Door Horizontal Out",					"Wipe Door\nHorizontal Out",					404, -4, -4);
			AddEffect(0, "Wipe Door Diagonal",							"Wipe Door\nDiagonal",							405, -4, -4);
			AddEffect(0, "Wipe Door Diagonal Rotated", 					"Wipe Door\nDiagonal Rotated",					406, -4, -4);
			AddEffect(0, "Wipe Bars Vertical",							"Wipe Bars\nVertical",						501, -4, -4);
			AddEffect(0, "Wipe Bars Horizontal",						"Wipe Bars\nHorizontal",						502, -4, -4);
			AddEffect(0, "Wipe Side Left To Right", 					"Wipe Side\nLeft To Right", 					601, -4, -4);
			AddEffect(0, "Wipe Side Right To Left", 					"Wipe Side\nRight To Left", 					602, -4, -4);
			AddEffect(0, "Wipe Side Top To Bottom", 					"Wipe Side\nTop To Bottom", 					603, -4, -4);
			AddEffect(0, "Wipe Side Bottom To Top", 					"Wipe Side\nBottom To Top", 					604, -4, -4);
			AddEffect(0, "Wipe Corner Left Top", 						"Wipe Corner\nLeft Top", 						605, -4, -4);
			AddEffect(0, "Wipe Corner Left Bottom", 					"Wipe Corner\nLeft Bottom", 					606, -4, -4);
			AddEffect(0, "Wipe Corner Right Bottom", 					"Wipe Corner\nRight Bottom", 					607, -4, -4);
			AddEffect(0, "Wipe Corner Right Top", 						"Wipe Corner\nRight Top", 						608, -4, -4);
			AddEffect(1, "Wipe Strips Horizontal",					    "Wipe Strips\nHorizontal",	     		        5331, -2, -2);
			AddEffect(1, "Wipe Strips Vertical",					    "Wipe Strips\nVertical",	     			    5332, -2, -2);
			AddEffect(1, "Wipe Checker Left To Right", 					"Wipe Checker\nLeft To Right", 					2901, -4, -4);
			AddEffect(1, "Wipe Checker Right To Left", 					"Wipe Checker\nRight To Left", 					2902, -4, -4);
			AddEffect(1, "Wipe Checker Top To Bottom", 					"Wipe Checker\nTop To Bottom", 					2903, -4, -4);
			AddEffect(1, "Wipe Checker Bottom To Top", 					"Wipe Checker\nBottom To Top", 					2904, -4, -4);
			AddEffect(1, "Wipe Random Strips Vertical",					"Wipe Random\nStrips Vertical",					3101, -1, -1);
			AddEffect(1, "Wipe Random Strips Horizontal",				"Wipe Random\nStrips Horizontal",				3102, -1, -1);
		AddCategory(0, "Wipe Extra");
			AddEffect(1, "Diagonal, Down Right",                        "Diagonal, Down Right",			   		        5305, -2, -2);
			AddEffect(1, "Diagonal, Down Left",                         "Diagonal, Down Left",			   		        5306, -2, -2);
			AddEffect(1, "Diagonal, Up Right",                          "Diagonal, Up Right",			   		        5307, -2, -2);
			AddEffect(1, "Diagonal, Up Left",                           "Diagonal, Up Left",			   		        5308, -2, -2);
			AddEffect(1, "Fun, In",                                     "Fun, In",			   		                    5309, -2, -2);
			AddEffect(1, "Fun, Out",                                    "Fun, Out",			   		                    5310, -2, -2);
			AddEffect(1, "Fun, Up",                                     "Fun, Up",			   		                    5311, -2, -2);
			AddEffect(1, "Filled V, Down",                              "Filled V, Down",	     		   		        5312, -2, -2);
			AddEffect(1, "Filled V, Left",                              "Filled V, Left",	     		   		        5313, -2, -2);
			AddEffect(1, "Filled V, Right",                             "Filled V, Right",	     		   		        5314, -2, -2);
			AddEffect(1, "Filled V, Up",                                "Filled V, Up",	     		   	     	        5315, -2, -2);
			AddEffect(1, "Insert, Down Left",                           "Insert, Down Left",	     		   		    5316, -2, -2);
			AddEffect(1, "Insert, Down Right",                          "Insert, Down Right",	     		   		    5317, -2, -2);
			AddEffect(1, "Insert, Up Left",                             "Insert, Up Left",	     		   		        5318, -2, -2);
			AddEffect(1, "Insert, Up Right",                            "Insert, Up Right",	     		   	     	    5319, -2, -2);
			AddEffect(1, "Corner Circle, Left Top", 					"Corner Circle\nLeft Top", 				        5381, -1, -1);
			AddEffect(1, "Corner Circle, Left Bottom", 					"Corner Circle\nLeft Bottom", 				    5382, -1, -1);
			AddEffect(1, "Corner Circle, Right Top", 					"Corner Circle\nRight Top", 				    5383, -1, -1);
			AddEffect(1, "Corner Circle, Right Bottom", 				"Corner Circle\nRight Bottom", 			        5384, -1, -1);
			AddEffect(1, "Reveal, Right",                               "Reveal, Right",	     		   		        5321, -2, -2);
			AddEffect(1, "Reveal, Left",                                "Reveal, Left",	     		   		            5322, -2, -2);
			AddEffect(1, "Reveal, Bottom",                              "Reveal, Down",	     		   		            5320, -2, -2);
			AddEffect(1, "Reveal, Top",                                 "Reveal, Up",	     		   	     	        5323, -2, -2);
			AddEffect(1, "Split, Horizontal",                           "Split, Horizontal",	     		   		    5324, -2, -2);
			AddEffect(1, "Split, Vertical",                             "Split, Vertical",	     		   	     	    5325, -2, -2);
			AddEffect(1, "Sweep, In",                                   "Sweep, In",			   		                5326, -2, -2);
			AddEffect(1, "Sweep, Out",                                  "Sweep, Out",			   		                5327, -2, -2);
			AddEffect(1, "Sweep, Up",                                   "Sweep, Up",			   		                5328, -2, -2);
		AddCategory(0, "Wipe Extra Smooth");
			AddEffect(1, "Diagonal Smooth, Down Right",                 "Diagonal Smooth, Down Right",			   		5337, -2, -2);
			AddEffect(1, "Diagonal Smooth, Down Left",                  "Diagonal Smooth, Down Left",			   		5338, -2, -2);
			AddEffect(1, "Diagonal Smooth, Up Right",                   "Diagonal Smooth, Up Right",			   		5339, -2, -2);
			AddEffect(1, "Diagonal Smooth, Up Left",                    "Diagonal Smooth, Up Left",			   		    5340, -2, -2);
			AddEffect(1, "Fun Smooth In",						        "Fun Smooth, In",			   		            5341, -2, -2);
			AddEffect(1, "Fun Smooth Out",						        "Fun Smooth, Out",			   		            5342, -2, -2);
			AddEffect(1, "Fun Smooth Up",						        "Fun Smooth, Up",			   		            5343, -2, -2);
			AddEffect(1, "Filled V Smooth, Down",                       "Filled V Smooth, Down",	     		   		5344, -2, -2);
			AddEffect(1, "Filled V Smooth, Left",                       "Filled V Smooth, Left",	     		   		5345, -2, -2);
			AddEffect(1, "Filled V Smooth, Right",                      "Filled V Smooth, Right",	     		   		5346, -2, -2);
			AddEffect(1, "Filled V Smooth, Up",                         "Filled V Smooth, Up",	     		   	     	5347, -2, -2);
			AddEffect(1, "Insert Smooth, Down Left",                    "Insert Smooth, Down Left",	     		   		5348, -2, -2);
			AddEffect(1, "Insert Smooth, Down Right",                   "Insert Smooth, Down Right",	     		   	5349, -2, -2);
			AddEffect(1, "Insert Smooth, Up Left",                      "Insert Smooth, Up Left",	     		   		5350, -2, -2);
			AddEffect(1, "Insert Smooth, Up Right",                     "Insert Smooth, Up Right",	     		   	    5351, -2, -2);
			AddEffect(1, "Corner Circle Smooth, Left Top", 				"Corner Circle Smooth\nLeft Top", 				5385, -1, -1);
			AddEffect(1, "Corner Circle Smooth, Left Bottom", 			"Corner Circle Smooth\nLeft Bottom", 			5386, -1, -1);
			AddEffect(1, "Corner Circle Smooth, Right Top", 			"Corner Circle Smooth\nRight Top", 				5387, -1, -1);
			AddEffect(1, "Corner Circle Smooth, Right Bottom", 			"Corner Circle Smooth\nRight Bottom", 			5388, -1, -1);
			AddEffect(2, "Reveal Smooth Right",							"Fade Side\nLeft To Right",						4301, -1, -1);
			AddEffect(2, "Reveal Smooth Left",							"Fade Side\nRight To Left",						4302, -1, -1);
			AddEffect(2, "Reveal Smooth Bottom",						"Fade Side\nTop To Bottom",						4303, -1, -1);
			AddEffect(2, "Reveal Smooth Top",							"Fade Side\nBottom To Top",						4304, -1, -1);
			AddEffect(1, "Split Smooth, Horizontal",                    "Split Smooth, Horizontal",	     		   		5352, -2, -2);
			AddEffect(1, "Split Smooth, Vertical",                      "Split Smooth, Vertical",	     		   	    5353, -2, -2);
			AddEffect(1, "Sweep Smooth, In",                            "Sweep Smooth, In",			   		            5354, -2, -2);
			AddEffect(1, "Sweep Smooth, Out",                           "Sweep Smooth, Out",			   		        5355, -2, -2);
			AddEffect(1, "Sweep Smooth, Up",                            "Sweep Smooth, Up",			   		            5356, -2, -2);
		AddCategory(0, "Slide");
			AddEffect(2, "Slide Lines Left To Right",					"Slide Lines\nLeft To Right",					2801, -1, -1);
			AddEffect(2, "Slide Lines Right To Left",					"Slide Lines\nRight To Left",					2802, -1, -1);
			AddEffect(2, "Slide Lines Top To Bottom",					"Slide Lines\nTop To Bottom",					2803, -1, -1);
			AddEffect(2, "Slide Lines Bottom To Top",					"Slide Lines\nBottom To Top",					2804, -1, -1);
			AddEffect(1, "Slide, Up",                                   "Slide\nUp",			   		                5704, -2, -2);
			AddEffect(1, "Slide, Down",                                 "Slide\nDown",			   		                5705, -2, -2);
			AddEffect(1, "Slide, Right",                                "Slide\nRight",			   		                5706, -2, -2);
			AddEffect(1, "Slide, Left",                                 "Slide\nLeft",			   		                5707, -2, -2);
			AddEffect(1, "Slide, Up Center",                            "Slide\nUp Center",			   		            5708, -2, -2);
			AddEffect(1, "Slide, Down Center",                          "Slide\nDown Center",			   		        5709, -2, -2);
			AddEffect(1, "Slide, Right Center",                         "Slide\nRight Center",			   		        5710, -2, -2);
			AddEffect(1, "Slide, Left Center",                          "Slide\nLeft Center",			   		        5711, -2, -2);
			AddEffect(1, "Slide Change Left To Right", 					"Slide Change\nLeft To Right", 					2701, -1, -1);
			AddEffect(1, "Slide Change Right To Left", 					"Slide Change\nRight To Left", 					2702, -1, -1);
			AddEffect(1, "Slide Change Top To Bottom", 					"Slide Change\nTop To Bottom", 					2703, -1, -1);
			AddEffect(1, "Slide Change Bottom To Top",					"Slide Change\nBottom To Top",					2704, -1, -1);
			AddEffect(0, "Slide Door Vertical In",						"Slide Door\nVertical In",						801, -1, -1);
			AddEffect(0, "Slide Door Vertical Out",						"Slide Door\nVertical Out",						802, -1, -1);
			AddEffect(0, "Slide Door Horizontal In",					"Slide Door\nHorizontal In",					803, -1, -1);
			AddEffect(0, "Slide Door Horizontal Out",					"Slide Door\nHorizontal Out",					804, -1, -1);
			AddEffect(0, "Slide Door Diagonal",							"Slide Door\nDiagonal",							805, -1, -1);
			AddEffect(0, "Slide Door Diagonal Rotated",					"Slide Door\nDiagonal Rotated", 				806, -1, -1);
			AddEffect(0, "Slide Strips Vertical",						"Slide Strips\nVertical",						901, -2, -2);
			AddEffect(0, "Slide Strips Horizontal",						"Slide Strips\nHorizontal",						902, -2, -2);
			AddEffect(0, "Slide Side Left To Right", 					"Slide Side\nLeft To Right",					1001, -8, -8);
			AddEffect(0, "Slide Side Right To Left", 					"Slide Side\nRight To Left",					1002, -8, -8);
			AddEffect(0, "Slide Side Top To Bottom", 					"Slide Side\nTop To Bottom",					1003, -8, -8);
			AddEffect(0, "Slide Side Bottom To Top", 					"Slide Side\nBottom To Top",					1004, -8, -8);
			AddEffect(0, "Slide Corner Left Top", 						"Slide Corner\nLeft Top", 						1005, -8, -8);
			AddEffect(0, "Slide Corner Left Bottom", 					"Slide Corner\nLeft Bottom", 					1006, -8, -8);
			AddEffect(0, "Slide Corner Right Bottom", 					"Slide Corner\nRight Bottom", 					1007, -8, -8);
			AddEffect(0, "Slide Corner Right Top", 						"Slide Corner\nRight Top", 						1008, -8, -8);
			AddEffect(0, "Slide Center Cross In",						"Slide Center\nCross In",						1107, -1, -1);
			AddEffect(0, "Slide Center Cross Out",						"Slide Center\nCross Out",						1108, -1, -1);
			AddEffect(0, "Slide Page Turn Left Top", 					"Page Turn\nLeft Top",							2405, -1, -1);
			AddEffect(0, "Slide Page Turn Left Bottom", 				"Page Turn\nLeft Bottom",						2406, -1, -1);
			AddEffect(0, "Slide Page Turn Right Bottom", 				"Page Turn\nRight Bottom",						2407, -1, -1);
			AddEffect(0, "Slide Page Turn Right Top", 					"Page Turn\nRight Top",							2408, -1, -1);
		AddCategory(1, "Stencil");
			AddEffect(0, "Stencil Circle In",							"Stencil\nCircle In",							705, -1, -1);
			AddEffect(0, "Stencil Circle Out",							"Stencil\nCircle Out",							706, -1, -1);
			AddEffect(1, "Stencil Circles",						        "Stencil\nCircles",						        5390, -5, -5);
			AddEffect(0, "Stencil Box In",								"Stencil\nBox In",								701, -1, -1);
			AddEffect(0, "Stencil Box Out",								"Stencil\nBox Out",								702, -1, -1);
			AddEffect(0, "Stencil Diamond In",							"Stencil\nDiamond In",							703, -1, -1);
			AddEffect(0, "Stencil Diamond Out",							"Stencil\nDiamond Out", 						704, -1, -1);
			AddEffect(1, "Stencil Diamonds",						    "Stencil\nDiamonds",						    5394, -5, -5);
			//AddEffect(0, "Stencil Cross In",							"Stencil\nCross In",							707, -1, -1);
			//AddEffect(0, "Stencil Cross Out",							"Stencil\nCross Out",							708, -1, -1);
			AddEffect(1, "Stencil Eye",						            "Stencil Eye",			   			            5714, -2, -2);
			AddEffect(1, "Stencil Heart",						        "Stencil Heart",	     			            5715, -2, -2);
			AddEffect(1, "Stencil Wave",						        "Stencil Wave",			   			            5716, -2, -2);
			AddEffect(1, "Stencil Star",								"Stencil Smooth Narrow\nCenter Star",			5369, -1, -1);
			AddEffect(1, "Stencil Stars",								"Stencil Smooth Narrow\nStars",					5370, -5, -5);
			AddEffect(1, "Stencil KeyHole",								"Stencil Smooth Narrow\nCenter KeyHole",		5371, -1, -1);
			AddEffect(1, "Stencil KeyHoles",							"Stencil Smooth Narrow\nKeyHoles",				5372, -5, -5);
			AddEffect(1, "Stencil Flower",								"Stencil Smooth Narrow\nCenter Flower",			5373, -1, -1);
			AddEffect(1, "Stencil Flowers",								"Stencil Smooth Narrow\nFlowers",				5374, -5, -5);
			AddEffect(1, "Stencil Box Diagonal",                        "Stencil Box\nDiagonal",			   		    5303, -2, -2);
			AddEffect(1, "Stencil Cross Diagonal",                      "Stencil Cross\nDiagonal",			   		    5304, -2, -2);
			AddEffect(1, "Stencil Bow Horizontal",				        "Stencil Butterfly\nHorizontal",		        5301, -2, -2);
			AddEffect(1, "Stencil Bow Vertical",			            "Stencil Butterfly\nVertical",			        5302, -2, -2);
			AddEffect(1, "Stencil Zigzag Horizontal",                   "Stencil ZigZag\nHorizontal",			        5329, -2, -2);
			AddEffect(1, "Stencil Zigzag Vertical",                     "Stencil ZigZag\nVertical",			   	        5330, -2, -2);
		AddCategory(1, "Stencil Smooth");

			AddEffect(1, "Stencil Smooth Circle In",					"Stencil Smooth\nCircle In",				    5389, -1, -1);
			AddEffect(1, "Stencil Smooth Circle Out",					"Stencil Smooth\nCircle Out",				    5359, -1, -1);
			AddEffect(1, "Stencil Smooth Circles",						"Stencil Smooth\nCircles",						5360, -5, -5);
			AddEffect(0, "Stencil Smooth Box In",						"Stencil Smooth\nBox In",						5391, -1, -1);
			AddEffect(0, "Stencil Smooth Box Out",						"Stencil Smooth\nBox Out",						5392, -1, -1);
			AddEffect(1, "Stencil Smooth Diamond In",				    "Stencil Smooth\nDiamond In",					5393, -1, -1);
			AddEffect(1, "Stencil Smooth Diamond Out",				    "Stencil Smooth\nDiamond Out",					5361, -1, -1);
			AddEffect(1, "Stencil Smooth Diamonds",						"Stencil Smooth\nDiamonds",						5362, -5, -5);
			AddEffect(1, "Stencil Smooth Eye",						    "Stencil Smooth\nEye",			   			    5717, -2, -2);
			AddEffect(1, "Stencil Smooth Heart",						"Stencil Smooth\nHeart",	     			    5718, -2, -2);
			AddEffect(1, "Stencil Smooth Wave",						    "Stencil Smooth\nWave",			   			    5719, -2, -2);
			AddEffect(1, "Stencil Smooth Wide Center Star",				"Stencil Smooth Wide\nCenter Star",				5363, -1, -1);
			AddEffect(1, "Stencil Smooth Wide Stars",					"Stencil Smooth Wide\nStars",					5364, -5, -5);
			AddEffect(1, "Stencil Smooth Wide Center Flower",			"Stencil Smooth Wide\nCenter Flower",			5365, -1, -1);
			AddEffect(1, "Stencil Smooth Wide Flowers",					"Stencil Smooth Wide\nFlowers",					5366, -5, -5);
			AddEffect(1, "Stencil Smooth Wide Center KeyHole",			"Stencil Smooth Wide\nCenter KeyHole",			5367, -1, -1);
			AddEffect(1, "Stencil Smooth Wide KeyHoles",				"Stencil Smooth Wide\nKeyHoles",				5368, -5, -5);

			AddEffect(1, "Stencil Smooth Diagonal Box Out",             "Diagonal Smooth, Box Out",			   		    5335, -2, -2);
			AddEffect(1, "Stencil Smooth Diagonal Cross Out",           "Diagonal Smooth, Cross Out",			   		5336, -2, -2);
			AddEffect(1, "Bow Tile Smooth, Horizontal",                 "Bow Tile Smooth, Horizontal",			   		5333, -2, -2);
			AddEffect(1, "Bow Tile Smooth, Vertical",                   "Bow Tile Smooth, Vertical",			   		5334, -2, -2);
			AddEffect(1, "Stencil Smooth ZigZag Horizontal",            "ZigZag Smooth, Horizontal",			   		5357, -2, -2);
			AddEffect(1, "Stencil Smooth ZigZag Vertical",              "ZigZag Smooth, Vertical",			   		    5358, -2, -2);
		
		AddCategory(0, "Push");
			AddEffect(0, "Push Door Vertical In",						"Push Door\nVertical In",						1201, -1, -1);
			AddEffect(0, "Push Door Vertical Out",						"Push Door\nVertical Out",						1202, -1, -1);
			AddEffect(0, "Push Door Horizontal In",						"Push Door\nHorizontal In",						1203, -1, -1);
			AddEffect(0, "Push Door Horizontal Out",					"Push Door\nHorizontal Out",					1204, -1, -1);
			AddEffect(0, "Push Strips Vertical",						"Push Strips\nVertical",						1301, -1, -1);
			AddEffect(0, "Push Strips Horizontal",						"Push Strips\nHorizontal",						1302, -1, -1);
			AddEffect(0, "Push Side Left To Right",						"Push Side\nLeft To Right",						1401, -1, -1);
			AddEffect(0, "Push Side Right To Left",						"Push Side\nRight To Left",						1402, -1, -1);
			AddEffect(0, "Push Side Top To Bottom",						"Push Side\nTop To Bottom",						1403, -1, -1);
			AddEffect(0, "Push Side Bottom To Top",						"Push Side\nBottom To Top",						1404, -1, -1);
		AddCategory(0, "Stretch");
			AddEffect(2, "Stretch Fold Left To Right",					"Stretch Fold\nLeft To Right",					4701, -4, -4);
			AddEffect(2, "Stretch Fold Right To Left",					"Stretch Fold\nRight To Left",					4702, -4, -4);
			AddEffect(2, "Stretch Fold Top To Bottom",					"Stretch Fold\nTop To Bottom",					4703, -4, -4);
			AddEffect(2, "Stretch Fold Bottom To Top",					"Stretch Fold\nBottom To Top",					4704, -4, -4);
			AddEffect(1, "Stretch Shrink In Vertical",				    "Stretch Shrink\nIn Vertical",			   	    5712, -2, -2);
			AddEffect(1, "Stretch Shrink In Horizontal",				"Stretch Shrink\nIn Horizontal",			   	5713, -2, -2);
			AddEffect(0, "Stretch Door Vertical In",					"Stretch Door\nVertical In",					1501, -1, -1);
			AddEffect(0, "Stretch Door Vertical Out",					"Stretch Door\nVertical Out",					1502, -1, -1);
			AddEffect(0, "Stretch Door Horizontal In",					"Stretch Door\nHorizontal In",					1503, -1, -1);
			AddEffect(0, "Stretch Door Horizontal Out",					"Stretch Door\nHorizontal Out",					1504, -1, -1);
			AddEffect(0, "Stretch Strips Vertical",						"Stretch Strips\nVertical",						1601, -1, -1);
			AddEffect(0, "Stretch Strips Horizontal",					"Stretch Strips\nHorizontal",					1602, -1, -1);
			AddEffect(0, "Stretch Side Left To Right",					"Stretch Side\nLeft To Right",					1701, -1, -1);
			AddEffect(0, "Stretch Side Right To Left",					"Stretch Side\nRight To Left",					1702, -1, -1);
			AddEffect(0, "Stretch Side Top To Bottom",					"Stretch Side\nTop To Bottom",					1703, -1, -1);
			AddEffect(0, "Stretch Side Bottom To Top",					"Stretch Side\nBottom To Top",					1704, -1, -1);
			AddEffect(0, "Stretch Corner Left Top", 					"Stretch Corner\nLeft Top", 					1705, -1, -1);
			AddEffect(0, "Stretch Corner Left Bottom", 					"Stretch Corner\nLeft Bottom", 					1706, -1, -1);
			AddEffect(0, "Stretch Corner Right Bottom", 				"Stretch Corner\nRight Bottom", 				1707, -1, -1);
			AddEffect(0, "Stretch Corner Right Top", 					"Stretch Corner\nRight Top", 					1708, -1, -1);
			AddEffect(0, "Stretch Center Box In",						"Stretch Center\nBox In",						1801, -1, -1);
			AddEffect(0, "Stretch Center Box Out",						"Stretch Center\nBox Out",						1802, -1, -1);
			AddEffect(0, "Stretch Wipe Side Left To Right In",			"Stretch Wipe Side\nLeft To Right In",			1901, -1, -1);
			AddEffect(0, "Stretch Wipe Side Left To Right Out",			"Stretch Wipe Side\nLeft To Right Out",			1902, -1, -1);
			AddEffect(0, "Stretch Wipe Side Right To Left In",			"Stretch Wipe Side\nRight To Left In",			1903, -1, -1);
			AddEffect(0, "Stretch Wipe Side Right To Left Out",			"Stretch Wipe Side\nRight To Left Out",			1904, -1, -1);
			AddEffect(0, "Stretch Wipe Side Top To Bottom In",			"Stretch Wipe Side\nTop To Bottom In",			1905, -1, -1);
			AddEffect(0, "Stretch Wipe Side Top To Bottom Out",			"Stretch Wipe Side\nTop To Bottom Out",			1906, -1, -1);
			AddEffect(0, "Stretch Wipe Side Bottom To Top In",			"Stretch Wipe Side\nBottom To Top In",			1907, -1, -1);
			AddEffect(0, "Stretch Wipe Side Bottom To Top Out",			"Stretch Wipe Side\nBottom To Top Out",			1908, -1, -1);
		AddCategory(1, "Graduate");
			AddEffect(1, "Graduate Side Left To Right", 				"Graduate Side\nLeft To Right",					3501, -4, -4);
			AddEffect(1, "Graduate Side Right To Left", 				"Graduate Side\nRight To Left",					3502, -4, -4);
			AddEffect(1, "Graduate Side Top To Bottom", 				"Graduate Side\nTop To Bottom",					3503, -4, -4);
			AddEffect(1, "Graduate Side Bottom To Top", 				"Graduate Side\nBottom To Top",					3504, -4, -4);
			AddEffect(1, "Graduate Corner Left Top", 					"Graduate Corner\nLeft Top", 					3505, -4, -4);
			AddEffect(1, "Graduate Corner Left Bottom", 				"Graduate Corner\nLeft Bottom", 				3506, -4, -4);
			AddEffect(1, "Graduate Corner Right Bottom", 				"Graduate Corner\nRight Bottom", 				3507, -4, -4);
			AddEffect(1, "Graduate Corner Right Top", 					"Graduate Corner\nRight Top", 					3508, -4, -4);
			AddEffect(1, "Graduate Clock Bottom Clockwise",				"Graduate Clock\nBottom Clockwise",				3601, -2, -2);
			AddEffect(1, "Graduate Clock Bottom Reverse",				"Graduate Clock\nBottom Reverse",				3602, -2, -2);
			AddEffect(1, "Graduate Clock Bottom Opposite",				"Graduate Clock\nBottom Opposite",				3603, -2, -2);
			AddEffect(1, "Graduate Clock Top Clockwise",				"Graduate Clock\nTop Clockwise",				3604, -2, -2);
			AddEffect(1, "Graduate Clock Top Reverse",					"Graduate Clock\nTop Reverse",					3605, -2, -2);
			AddEffect(1, "Graduate Clock Top Opposite",					"Graduate Clock\nTop Opposite",					3606, -2, -2);
			AddEffect(1, "Graduate Clock Left Clockwise",				"Graduate Clock\nLeft Clockwise",				3607, -2, -2);
			AddEffect(1, "Graduate Clock Left Reverse",					"Graduate Clock\nLeft Reverse",					3608, -2, -2);
			AddEffect(1, "Graduate Clock Left Opposite",				"Graduate Clock\nLeft Opposite",				3609, -2, -2);
			AddEffect(1, "Graduate Clock Right Clockwise",				"Graduate Clock\nRight Clockwise",				3610, -2, -2);
			AddEffect(1, "Graduate Clock Right Reverse",				"Graduate Clock\nRight Reverse",				3611, -2, -2);
			AddEffect(1, "Graduate Clock Right Opposite",				"Graduate Clock\nRight Opposite",				3612, -2, -2);
			AddEffect(1, "Graduate Clock 2 Angles Clockwise",			"Graduate Clock\n2 Angles Clockwise",			3613, -2, -2);
			AddEffect(1, "Graduate Clock 2 Angles Reverse",				"Graduate Clock\n2 Angles Reverse",				3614, -2, -2);
			AddEffect(1, "Graduate Clock 3 Angles Clockwise",			"Graduate Clock\n3 Angles Clockwise",			3615, -2, -2);
			AddEffect(1, "Graduate Clock 3 Angles Reverse",				"Graduate Clock\n3 Angles Reverse",				3616, -2, -2);
		AddCategory(1, "Graduate Smooth");
			AddEffect(1, "Graduate Smooth Side Left To Right", 			"Graduate Smooth Side\nLeft To Right",			3617, -4, -4);
			AddEffect(1, "Graduate Smooth Side Right To Left", 			"Graduate Smooth Side\nRight To Left",			3618, -4, -4);
			AddEffect(1, "Graduate Smooth Side Top To Bottom", 			"Graduate Smooth Side\nTop To Bottom",			3619, -4, -4);
			AddEffect(1, "Graduate Smooth Side Bottom To Top", 			"Graduate Smooth Side\nBottom To Top",			3620, -4, -4);
			AddEffect(1, "Graduate Smooth Corner Left Top", 			"Graduate Smooth Corner\nLeft Top", 			3621, -4, -4);
			AddEffect(1, "Graduate Smooth Corner Left Bottom", 			"Graduate Smooth Corner\nLeft Bottom", 			3622, -4, -4);
			AddEffect(1, "Graduate Smooth Corner Right Bottom", 		"Graduate Smooth Corner\nRight Bottom", 		3623, -4, -4);
			AddEffect(1, "Graduate Smooth Corner Right Top", 			"Graduate Smooth Corner\nRight Top", 			3624, -4, -4);
			AddEffect(1, "Graduate Smooth Clock Bottom Clockwise",		"Graduate Smooth Clock\nBottom Clockwise",		3625, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Bottom Reverse",		"Graduate Smooth Clock\nBottom Reverse",		3626, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Bottom Opposite",		"Graduate Smooth Clock\nBottom Opposite",		3627, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Top Clockwise",			"Graduate Smooth Clock\nTop Clockwise",			3628, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Top Reverse",			"Graduate Smooth Clock\nTop Reverse",			3629, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Top Opposite",			"Graduate Smooth Clock\nTop Opposite",			3630, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Left Clockwise",		"Graduate Smooth Clock\nLeft Clockwise",		3631, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Left Reverse",			"Graduate Smooth Clock\nLeft Reverse",			3632, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Left Opposite",			"Graduate Smooth Clock\nLeft Opposite",			3633, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Right Clockwise",		"Graduate Smooth Clock\nRight Clockwise",		3634, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Right Reverse",			"Graduate Smooth Clock\nRight Reverse",			3635, -2, -2);
			AddEffect(1, "Graduate Smooth Clock Right Opposite",		"Graduate Smooth Clock\nRight Opposite",		3636, -2, -2);
			AddEffect(1, "Graduate Smooth Clock 2 Angles Clockwise",	"Graduate Smooth Clock\n2 Angles Clockwise",	3637, -2, -2);
			AddEffect(1, "Graduate Smooth Clock 2 Angles Reverse",		"Graduate Smooth Clock\n2 Angles Reverse",		3638, -2, -2);
			AddEffect(1, "Graduate Smooth Clock 3 Angles Clockwise",	"Graduate Smooth Clock\n3 Angles Clockwise",	3639, -2, -2);
			AddEffect(1, "Graduate Smooth Clock 3 Angles Reverse",		"Graduate Smooth Clock\n3 Angles Reverse",		3640, -2, -2);
		AddCategory(0, "Pinwheel Curved");
			AddEffect(1, "Pinwheel Small Narrow Curve, 5 Angles",		"Pinwheel Small Narrow Curve, 5 Angles",		5901, -1, -1);
			AddEffect(1, "Pinwheel Small Narrow Curve, 7 Angles",		"Pinwheel Small Narrow Curve, 7 Angles",		5902, -1, -1);
			AddEffect(1, "Pinwheel Small Narrow Curve, 12 Angles",		"Pinwheel Small Narrow Curve, 12 Angles",		5903, -1, -1);
			AddEffect(1, "Pinwheel Small Wide Curve, 5 Angles",		    "Pinwheel Small Wide Curve, 5 Angles",		    5904, -1, -1);
			AddEffect(1, "Pinwheel Small Wide Curve, 7 Angles",		    "Pinwheel Small Wide Curve, 7 Angles",		    5905, -1, -1);
			AddEffect(1, "Pinwheel Small Wide Curve, 12 Angles",		"Pinwheel Small Wide Curve, 12 Angles",		    5906, -1, -1);
			AddEffect(1, "Pinwheel Large Curve, 5 Angles",		        "Pinwheel Large Curve, 5 Angles",		        5907, -1, -1);
			AddEffect(1, "Pinwheel Large Curve, 7 Angles",		        "Pinwheel Large Curve, 7 Angles",		        5908, -1, -1);
			AddEffect(1, "Pinwheel Large Curve, 12 Angles",		        "Pinwheel Large Curve, 12 Angles",		        5909, -1, -1);
			AddEffect(1, "Propeller, 4 Angles",		                    "Propeller, 4 Angles",		                    5910, -1, -1);
			AddEffect(1, "Propeller, 7 Angles",		                    "Propeller, 7 Angles",		                    5911, -1, -1);
			AddEffect(1, "Propeller, 12 Angles",		                "Propeller, 12 Angles",		                    5912, -1, -1);

			AddEffect(1, "Pinwheel Small Narrow Curve Smooth, 5 Angles", "Pinwheel Small Narrow Curve Smooth, 5 Angles", 5913, -1, -1);
			AddEffect(1, "Pinwheel Small Narrow Curve Smooth, 7 Angles", "Pinwheel Small Narrow Curve Smooth, 7 Angles", 5914, -1, -1);
			AddEffect(1, "Pinwheel Small Narrow Curve Smooth, 12 Angles","Pinwheel Small Narrow Curve Smooth, 12 Angles",5915, -1, -1);
			AddEffect(1, "Pinwheel Small Wide Curve Smooth, 5 Angles",	 "Pinwheel Small Wide Curve Smooth, 5 Angles",	 5916, -1, -1);
			AddEffect(1, "Pinwheel Small Wide Curve Smooth, 7 Angles",	 "Pinwheel Small Wide Curve Smooth, 7 Angles",	 5917, -1, -1);
			AddEffect(1, "Pinwheel Small Wide Curve Smooth, 12 Angles",	 "Pinwheel Small Wide Curve Smooth, 12 Angles",	 5918, -1, -1);
			AddEffect(1, "Pinwheel Large Curve Smooth, 5 Angles",		 "Pinwheel Large Curve Smooth, 5 Angles",		 5919, -1, -1);
			AddEffect(1, "Pinwheel Large Curve Smooth, 7 Angles",		 "Pinwheel Large Curve Smooth, 7 Angles",		 5920, -1, -1);
			AddEffect(1, "Pinwheel Large Curve Smooth, 12 Angles",		 "Pinwheel Large Curve Smooth, 12 Angles",		 5921, -1, -1);
			AddEffect(1, "Propeller Smooth, 4 Angles",		             "Propeller Smooth, 4 Angles",		             5922, -1, -1);
			AddEffect(1, "Propeller Smooth, 7 Angles",		             "Propeller Smooth, 7 Angles",		             5923, -1, -1);
			AddEffect(1, "Propeller Smooth, 12 Angles",		             "Propeller Smooth, 12 Angles",		             5924, -1, -1);
	}
	void AddCategory(int type, const CString& name)
	{ 
		if (type > m_EffectsType)
			return;

		int count = m_categories.GetSize(); 
		
		Category category(name); 
		
		m_categories.Add(category); 
	}
	void AddEffect(int type, const CString& name, const CString& name2, int id, int tileX = -1, int tileY = -1)
	{ 
		if (type > m_EffectsType)
			return;

		int count = m_effects.GetSize(); 
		
		m_categories[m_categories.GetSize() - 1].Effects.Add(count); 
		
		Effect effect(name, name2, id); 

		effect.TileX = tileX;
		effect.TileY = tileY;
		effect.Type = type;
		
		m_effects.Add(effect); 
	}
	void ApplyEffect(int ID)
	{
		// find the effect information
		if (m_effect.ID != ID)
		{
			m_effect.ID = -1;

			for (int index = 0; index < m_effects.GetSize(); ++index)
			{
				if (m_effects[index].ID == ID)
				{
					m_effect = m_effects[index];
					break;
				}
			}
		}

		// check for valid effect
		if (m_effect.ID < 0)
			return;

		// check for effect type
		if (m_effect.Type > m_EffectsType)
			return;

		// compose effect category and effect sub-id
		int effectCategory = ID / 100;
		int effectType = ID % 100;

		// compute default tile size
		m_nTileSizeX = (m_nSizeX < 2) ? m_first.Width : m_nSizeX; m_nTileSizeX = max(2, m_nTileSizeX);
		m_nTileSizeY = (m_nSizeY < 2) ? m_first.Height : m_nSizeY; m_nTileSizeY = max(2, m_nTileSizeY);

		// retrieve best tile sizes for the effect
		if (m_nSizeX <= 0 || m_nSizeY <= 0)
		{
			if (m_effect.TileX > 1)
				m_nTileSizeX = m_effect.TileX;
			else if (m_effect.TileX < 0)
				m_nTileSizeX = m_result.Width / abs(m_effect.TileX);

			if (m_effect.TileY > 1)
				m_nTileSizeY = m_effect.TileY;
			else if (m_effect.TileY < 0)
				m_nTileSizeY = m_result.Height / abs(m_effect.TileY);
		}

		// compute tiles count
		m_nTilesX = 1 + m_result.Width / m_nTileSizeX;
		m_nTilesY = 1 + m_result.Height / m_nTileSizeY;

		if (m_first.Width % m_nTileSizeX <= 1)
			m_nTilesX -= 1; 
		if (m_first.Height % m_nTileSizeY <= 1)
			m_nTilesY -= 1; 

		// create graduator if necessary
		if (effectCategory == 35 || effectCategory == 36)
		{
			if (!m_graduate.Create(m_nTilesX*m_nTilesY))
				return;
		}

		switch (effectCategory)
		{
            default:  Fade             (effectType); break;
            case 1:   Mosaic           (effectType); break;
            case 2:   Diffuse          (effectType); break;
            case 3:   Rotate           (effectType); break;
            case 4:   WipeDoor         (effectType); break;
            case 5:   WipeStrips       (effectType); break;
            case 6:   WipeSide         (effectType); break;
            case 7:   WipeCenter       (effectType); break;
            case 8:   SlideDoor        (effectType); break;
            case 9:   SlideStrips      (effectType); break;
            case 10:  SlideSide        (effectType); break;
            case 11:  SlideCenter      (effectType); break;
            case 12:  PushDoor         (effectType); break;
            case 13:  PushStrips       (effectType); break;
            case 14:  PushSide         (effectType); break;
            case 15:  StretchDoor      (effectType); break;
            case 16:  StretchStrips    (effectType); break;
            case 17:  StretchSide      (effectType); break;
            case 18:  StretchCenter    (effectType); break;
            case 19:  StretchWipeSide  (effectType); break;
            case 20:  SwingSide        (effectType); break;    
            case 21:  SwingSideChange  (effectType); break;
            case 22:  SwingDoors       (effectType); break;
            case 23:  SwingDoorsChange (effectType); break;
            case 24:  Page             (effectType); break;
            case 25:  FlipOver         (effectType); break;
            case 26:  FlipAway         (effectType); break;
            case 27:  SlideChange      (effectType); break;
            case 28:  SlideBoxes       (effectType); break;   
            case 29:  WipeChecker      (effectType); break;
            case 30:  WipeClock        (effectType); break;
            case 31:  WipeStripsRandom (effectType); break;
            case 32:  Shatter          (effectType); break;
            case 33:  ShatterRotate    (effectType); break;
            case 34:  ShatterIn        (effectType); break;
			case 35:  GraduateWipeSide (effectType); break;
			case 36:  GraduateWipeClock(effectType); break;
			case 37:  Twirl		       (effectType); break;
			case 38:  Sphere	       (effectType); break;
			case 39:  Twist			   (effectType); break;
			case 40:  Ripple		   (effectType); break;
			case 41:  Wave			   (effectType); break;
			case 42:  Burning		   (effectType); break;
			case 43:  WipeFadeSide	   (effectType); break;
//			case 44:  WipeCircleCorner (effectType); break;
			case 45:  WipeClockCorner  (effectType); break;
			case 46:  FlipOutIn        (effectType); break;
			case 47:  Fold			   (effectType); break;
			case 48:  GaussianBlur	   (effectType); break;
			case 49:  Noise			   (effectType); break;
			case 50:  FlipBoxes		   (effectType); break;
			case 51:  FlipOutBoxes	   (effectType); break;
			case 52:  WipeSmooth	   (effectType); break;
			case 53:  Mask             (effectType); break;
			case 54:  Tetris           (effectType); break;
			case 55:  ShotChange       (effectType); break;
			case 56:  BlackWhiteLines  (effectType); break;
			case 57:  MovieMaker       (effectType); break;
			case 58:  Spiral           (effectType); break;
			case 59:  PinwheelCurved   (effectType); break; 
			case 60:  Puzzle           (effectType); break;
			case 62:  Tile             (effectType); break;
		}
	}
	
	BOOL Initialize(SAFEARRAY** First, SAFEARRAY** Last, SAFEARRAY** Result, double Frame)
	{
		// initialize images
		if (!m_first.Extract(First))
			return FALSE;
		if (!m_last.Extract(Last))
			return FALSE;
		if (!m_result.Extract(Result))
			return FALSE;

		// check for valid input parameters
		m_dFrame = Clamp(Frame);

		// check for simpliest case
		if (m_dFrame < 0.0001)
		{
			m_result.CopyFrom(m_first);
			return FALSE;
		}
		if (m_dFrame > 0.9999)
		{
			m_graduate.Clear();
			m_result.CopyFrom(m_last);
			return FALSE;
		}

		// check whether image have the same sizes
		if (m_first.Width != m_last.Width || m_first.Width != m_result.Width || m_first.Height != m_last.Height || m_first.Height != m_result.Height)
			return FALSE;

		// all ok
		return TRUE;
	}

	BOOL Initialize(IUnknown* First, IUnknown* Last, IUnknown** Result, double Frame)
	{
		// initialize images
		if (!m_first.Extract(&First))
			return FALSE;
		if (!m_last.Extract(&Last))
			return FALSE;
		if (!m_result.Extract(Result))
		{
			IUnknown* pImage = NULL;
			if (!ImageStudioUtils::ByteArrayToMediaData(0, m_last.Width, m_last.Height, &pImage))
				return FALSE;
			if (!m_result.Extract(&pImage))
				return FALSE;

			*Result = m_result.GetMediaData().GetMediaData(TRUE);

			pImage->Release();

			// здесь не надо сохранять указатели, т.к. на выходе должен быть инферфейс с счетчиком +1 от входного.
			// при нулевом входном интерфейсе на выходе будет интерфейс со счетчиком 1 (на выходе 2, но после удаления одного счетчика в ~MediaData будет 1)
			// а при ненулевом входном - с счетчиком 2 (опять же реально на выходе будет 3, но после ~MediaData будет 2)
		}
		else
			*Result = m_result.GetMediaData().GetMediaData(TRUE);

		// check for valid input parameters
		m_dFrame = Clamp(Frame);

		// check for simpliest case
		if (m_dFrame < 0.0001)
		{
			m_result.CopyFrom(m_first);
			return FALSE;
		}
		if (m_dFrame > 0.9999)
		{
			m_graduate.Clear();
			m_result.CopyFrom(m_last);
			return FALSE;
		}

		// check whether image have the same sizes
		if (m_first.Width != m_last.Width || m_first.Width != m_result.Width || m_first.Height != m_last.Height || m_first.Height != m_result.Height)
			return FALSE;

		// all ok
		return TRUE;
	}
	void GetRadiusAndAngle(double x, double y, double cx, double cy, double& r, double& a)
	{
		double dx = x - cx;
		double dy = y - cy;
		r = _hypot(dx, dy);

		double sdy = (dy >= 0 ? 1.0 : -1.0);
		a = (r ? acos(dx/r)*sdy: 0);
	}

	void ApplyRegister()
	{

	}
	double Clamp(double value)
	{
		return min(max(0, value), 1);
	}
	
	// f/x
	BOOL Fade(int Type);
	BOOL Mosaic(int Type);
	BOOL Diffuse(int Type);
	BOOL Rotate(int Type);
	BOOL Twirl(int Type);
	BOOL Sphere(int Type);
	BOOL Twist(int Type);
	BOOL Ripple(int Type);
	BOOL Wave(int Type);
	BOOL Burning(int Type);
	BOOL Fold(int Type);
	BOOL GaussianBlur(int Type);
	BOOL Noise(int Type);

	// wipe
	BOOL WipeDoor(int Type);
	BOOL WipeStrips(int Type);
	BOOL WipeSide(int Type);
	BOOL WipeCenter(int Type);
	BOOL WipeChecker(int Type);
	BOOL WipeClock(int Type);
	BOOL WipeStripsRandom(int Type);
	//BOOL WipeCircleCorner(int Type);
	BOOL WipeClockCorner(int Type);
	BOOL WipeFadeSide(int Type);	
	BOOL WipeSmooth(int Type);

	// slide
	BOOL SlideDoor(int Type);
	BOOL SlideStrips(int Type);
	BOOL SlideSide(int Type);
	BOOL SlideCenter(int Type);
	BOOL SlideChange(int Type);
	BOOL SlideBoxes(int Type);

	// push
	BOOL PushDoor(int Type);
	BOOL PushStrips(int Type);
	BOOL PushSide(int Type);

	// stretch
	BOOL StretchDoor(int Type);
	BOOL StretchStrips(int Type);
	BOOL StretchSide(int Type);
	BOOL StretchCenter(int Type);
	BOOL StretchWipeSide(int Type);

	// swing
	BOOL SwingSideChange(int Type);
	BOOL SwingSide(int Type);
	BOOL SwingDoors(int Type);
	BOOL SwingDoorsChange(int Type);

	// page
	BOOL Page(int Type);

	// flip
	BOOL FlipOver(int Type);
	BOOL FlipAway(int Type);
	BOOL FlipOutIn(int Type);
	BOOL FlipBoxes(int Type);
	BOOL FlipOutBoxes(int Type);

	// shatter
	BOOL Shatter(int Type);
	BOOL ShatterRotate(int Type);
	BOOL ShatterIn(int Type);

	// graduate
	BOOL GraduateWipeSide(int Type);
	BOOL GraduateWipeClock(int Type);
	BOOL Graduate(int Type, BOOL bWipeSide);

	//test
	BOOL Mask(int Type);
	BOOL Tetris(int Type);
	BOOL ShotChange(int Type);
	BOOL BlackWhiteLines(int Type);
	BOOL MovieMaker(int Type);
	BOOL Spiral(int Type);
	BOOL PinwheelCurved(int Type);
	BOOL Puzzle(int Type);
	BOOL Tile(int Type);

protected:
	BOOL WipePathCommon(int nTileCountX, int nTileCountY, GraphicsPath* path);

public:
	
	STDMETHOD(Register)(BSTR User, BSTR Password)
	{

		
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"EffectsType")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			m_EffectsType = ParamVal.lVal;

			AddEffects();
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"EffectsType")
			(*ParamValue).lVal = (LONG)m_EffectsType;

		return S_OK;
	}
	
	STDMETHOD(get_TilesWidth)(int* TilesWidth)
	{
		*TilesWidth = m_nSizeX;
		
		return S_OK;
	}
	STDMETHOD(put_TilesWidth)(int TilesWidth)
	{
		m_nSizeX = TilesWidth;
		
		return S_OK;
	}
	STDMETHOD(get_TilesHeight)(int* TilesHeight)
	{
		*TilesHeight = m_nSizeY;
		
		return S_OK;
	}
	STDMETHOD(put_TilesHeight)(int TilesHeight)
	{
		m_nSizeY = TilesHeight;
		
		return S_OK;
	}
	
	STDMETHOD(GetCategoriesCount)(long* Categories)
	{
		*Categories = m_categories.GetSize();
		
		return S_OK;
	}
	STDMETHOD(GetCategoryName)(long Category, BSTR* Name)
	{ 
		CString _name = "";

		if (Category >= 0 && Category < m_categories.GetSize()) 
			_name = m_categories[Category].Name;

		*Name = _name.AllocSysString();

		return S_OK; 
	}
	STDMETHOD(GetEffectsCount)(long Category, long* Count)
	{ 
		if (Category < 0)
			*Count = m_effects.GetSize();
		else if (Category >=0 && Category < m_categories.GetSize())
			*Count = m_categories[Category].Effects.GetSize();
		else
			*Count = 0;

		return S_OK; 
	}
	STDMETHOD(GetEffectName)(long Category, long Effect, BSTR* Name)
	{ 
		CString _name = "";

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_name = m_effects[Effect].Name;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_name = m_effects[m_categories[Category].Effects[Effect]].Name;
		}

		*Name = _name.AllocSysString();

		return S_OK; 
	}
	STDMETHOD(GetEffectName2)(long Category, long Effect, BSTR* Name)
	{ 
		CString _name = "";

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_name = m_effects[Effect].Name2;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_name = m_effects[m_categories[Category].Effects[Effect]].Name2;
		}

		*Name = _name.AllocSysString();

		return S_OK; 
	}
	STDMETHOD(GetEffectAt)(long Category, long Effect, long* ID)
	{ 
		int _id = -1;

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_id = m_effects[Effect].ID;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_id = m_effects[m_categories[Category].Effects[Effect]].ID;
		}

		*ID = _id;

		return S_OK; 
	}
	STDMETHOD(Compose)(SAFEARRAY** First, SAFEARRAY** Last, SAFEARRAY** Result, double Frame, long EffectID)
	{ 
		int EffectCategory = (EffectID / 100);

		if (Initialize(First, Last, Result, Frame)) 
			ApplyEffect(EffectID);
		
		ApplyRegister();
		
		return S_OK; 
	}
	STDMETHOD(Compose2)(SAFEARRAY** First, SAFEARRAY** Last, SAFEARRAY** Result, double Frame, long Category, long Effect)
	{
		int _id = -1;

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_id = m_effects[Effect].ID;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_id = m_effects[m_categories[Category].Effects[Effect]].ID;
		}

		return Compose(First, Last, Result, Frame, _id);
	}
	
	STDMETHOD(ComposeMedia)(IUnknown* First, IUnknown* Last, IUnknown** Result, double Frame, long EffectID)
	{
		int EffectCategory = (EffectID / 100);

		if (Initialize(First, Last, Result, Frame))
			ApplyEffect(EffectID);
		
		ApplyRegister();
		
		return S_OK;
	}
	STDMETHOD(ComposeMedia2)(IUnknown* First, IUnknown* Last, IUnknown** Result, double Frame, long Category, long Effect)
	{
		int _id = -1;

		if (Category < 0)
		{
			if (Effect >= 0 && Effect < m_effects.GetSize())
				_id = m_effects[Effect].ID;
		}
		else if (Category >=0 && Category < m_categories.GetSize())
		{
			if (Effect >= 0 && Effect < m_categories[Category].Effects.GetSize())
				_id = m_effects[m_categories[Category].Effects[Effect]].ID;
		}

		return ComposeMedia(First, Last, Result, Frame, _id);
	}
};
