#pragma once

#include <atlcoll.h>
#include <atlsimpcoll.h>

//#include "../../Reader/Records.h"
//#include "../../Reader/PPTFileDefines.h"

#include "File.h"

#define	___ANIMATION_TRACE_ENABLE__
// пока уберем, нужно все оттестить без анимаций пока
#define __USE_ANIMATION__

namespace Animations
{
	enum AnimationsClassType : WORD
	{
		RT_TimeNode							=	0xF127,
		RT_TimeVariant						=	0xF142,
		RT_TimePropertyList					=	0xF13D,		//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  
		RT_TimeExtTimeNodeContainer			=	0xF144,
		RT_SlideTime10Atom					=	0x2EEB,
		RT_SlideFlags10Atom					=	0x2EEA,
		RT_HashCodeAtom						=	0x2B00,
		RT_TimeSequenceData					=	0xF141,
		RT_TimeConditionContainer			=	0xF125,
		RT_TimeCondition					=	0xF128,
		RT_TimeClientVisualElement			=	0xF13C,
		RT_VisualPageAtom					=	0x2B01,
		RT_VisualShapeAtom					=	0x2AFB,
		RT_TimeEffectBehaviorContainer		=	0xF12D,
		RT_TimeEffectBehavior				=	0xF136,
		RT_TimeBehavior						=	0xF133,
		RT_TimeBehaviorContainer			=	0xF12A,
		RT_TimeColorBehaviorContainer		=	0xF12C,
		RT_TimeMotionBehaviorContainer		=	0xF12E,
		RT_TimeRotationBehaviorContainer	=	0xF12F,
		RT_TimeScaleBehaviorContainer		=	0xF130,
		RT_TimeColorBehavior				=	0xF135,
		RT_TimeMotionBehavior				=	0xF137,
		RT_TimeRotationBehavior				=	0xF138,
		RT_TimeScaleBehavior				=	0xF139,
		RT_TimeSetBehavior					=	0xF13A,
		RT_TimeSetBehaviorContainer			=	0xF131,
		RT_TimeAnimateBehavior				=	0xF134,
		RT_TimeAnimateBehaviorContainer		=	0xF12B,
		RT_TimeAnimationValueList			=	0xF13F,
		RT_TimeAnimationValue				=	0xF143 
	};

	enum TimeNodeTypeEnum : DWORD
	{
		TL_TNT_Parallel				=	0x00000000,	//  Parallel time node whose child nodes can start simultaneously. 
		TL_TNT_Sequential			=	0x00000001,	//	Sequential time node whose child nodes can only start sequentially and each child can only start after its previous sibling has started.  
		TL_TNT_Behavior				=	0x00000003,	//	Behavior time node that contains a behavior. 
		TL_TNT_Media				=	0x00000004	//  Media time node that contains a media object. 
	};

	enum TimeVariantTypeEnum : BYTE
	{
		TL_TVT_Bool					=	0x00,		//	A Boolean value.  
		TL_TVT_Int					=	0x01,		//	A signed integer. 
		TL_TVT_Float				=	0x02,		//	A floating-point number
		TL_TVT_String				=	0x03		//  A Unicode string.
	};

	enum TimePropertyID4TimeNode : WORD
	{
		TL_TPID_Display				=	0x00000002,	//  Display type in UI.  
		TL_TPID_MasterPos			=	0x00000005,	//  Relationship to the master time node. 
		TL_TPID_SlaveType			=	0x00000006,	//  Type of the slave time node. 
		TL_TPID_EffectID			=	0x00000009,	//  ID of an animation effect. 
		TL_TPID_EffectDir			=	0x0000000A,	//  Direction of an animation effect. 
		TL_TPID_EffectType			=	0x0000000B,	//  Type of an animation effect. 
		TL_TPID_AfterEffect			=	0x0000000D,	//  Whether the time node is an after effect. 
		TL_TPID_SlideCount			=	0x0000000F,	//  The number of slides that a media will play across. 
		TL_TPID_TimeFilter			=	0x00000010,	//  Time filtering for the time node. 
		TL_TPID_EventFilter			=	0x00000011,	//  Event filtering for the time node. 
		TL_TPID_HideWhenStopped		=	0x00000012,	//  Whether to display the media when it is stopped. 
		TL_TPID_GroupID				=	0x00000013,	//  Build identifier. 
		TL_TPID_EffectNodeType		=	0x00000014,	//  The role of the time node in the timing structure. 
		TL_TPID_PlaceholderNode		=	0x00000015,	//  Whether the time node is a placeholder. 
		TL_TPID_MediaVolume			=	0x00000016,	//  The volume of a media. 
		TL_TPID_MediaMute			=	0x00000017,	//  Whether a media object is mute. 
		TL_TPID_ZoomToFullScreen	=	0x0000001A	//  Whether to zoom a media object to full screen. 
	};

	enum TriggerObjectEnum : DWORD
	{
		TL_TOT_None					=	0x00000000,	//	None.  
		TL_TOT_VisualElement		=	0x00000001,	//	An animatable object. 
		TL_TOT_TimeNode				=	0x00000002,	//	A time node.  
		TL_TOT_RuntimeNodeRef		=	0x00000003	//	Runtime child time nodes. 
	};

	enum TimeVisualElementEnum : DWORD 
	{
		TL_TVET_Shape				=	0x00000000,	//  Applies to the shape and all its text. 
		TL_TVET_Page				=	0x00000001,	//  Applies to the slide.  
		TL_TVET_TextRange			=	0x00000002,	//  Applies to a specified range of text of the shape. 
		TL_TVET_Audio				=	0x00000003, //	Applies to the audio of the shape. 
		TL_TVET_Video				=	0x00000004,	//  Applies to the video of the shape. 
		TL_TVET_ChartElement		=	0x00000005,	//  Applies to the elements of the chart. 
		TL_TVET_ShapeOnly			=	0x00000006, //	Applies to the shape but not its text. 
		TL_TVET_AllTextRange		=	0x00000008	//	Applies to all text of the shape
	};

	enum TimeAnimateBehaviorValueTypeEnum : DWORD
	{
		TL_TABVT_String				=	0x00000000, //	Animate text content. 
		TL_TABVT_Number				=	0x00000001,	//	Animate a numeric property. 
		TL_TABVT_Color				=	0x00000002	//	Animate a color property. 
	};
	enum ElementTypeEnum : DWORD
	{
		TL_ET_ShapeType				=	0x00000001,	//	The animation targets a shape or some part of a shape. 
		TL_ET_SoundType				=	0x00000002	//	The animation targets a sound file that does not correspond to a shape.  
	};

}

namespace Animations
{
	struct ODMotionPath
	{
	public:
		struct ActionPoint
		{
#define MOVE_TO		L'M'
#define LINE_TO		L'L'
#define CURVE_TO	L'C'
#define CLOSE_LOOP	L'Z'
#define END			L'E'

			double	X[3];
			double	Y[3];
			WCHAR	TYPE;	// // M = move to // L = line to // C = curve to // Z = close loop // E = end 
		};

	public:

		inline bool Create ( CStringW MovePath )
		{
			m_Points.RemoveAll ();

			int Pos = 0;
			while ( Pos < MovePath.GetLength () )
			{
				ActionPoint	aPoint;
				aPoint.TYPE		=	MovePath.Tokenize ( L" ", Pos )[0];

				if ( L'm' == aPoint.TYPE )	aPoint.TYPE =	MOVE_TO;
				if ( L'l' == aPoint.TYPE )	aPoint.TYPE =	LINE_TO;
				if ( L'c' == aPoint.TYPE )	aPoint.TYPE =	CURVE_TO;
				if ( L'z' == aPoint.TYPE )	aPoint.TYPE =	CLOSE_LOOP;	//	This action requires no points.
				if ( L'e' == aPoint.TYPE )	aPoint.TYPE =	END;		//	This action requires no points.

				if ( MOVE_TO == aPoint.TYPE || LINE_TO == aPoint.TYPE )
				{
					aPoint.X[0]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );
					aPoint.Y[0]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );
				}

				if ( CURVE_TO == aPoint.TYPE )
				{
					aPoint.X[0]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );
					aPoint.Y[0]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );

					aPoint.X[1]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );
					aPoint.Y[1]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );

					aPoint.X[2]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );
					aPoint.Y[2]	=	_wtof (	MovePath.Tokenize ( L" ", Pos ) );
				}

				m_Points.Add ( aPoint ); 
			}

			return ( m_Points.GetSize () >= 2 );
		}

		inline CStringW Recalculate ( double ScaleX, double ScaleY )
		{
			CStringW	MovePath;

			for ( int i = 0; i < m_Points.GetSize(); ++i )
			{
				CStringW NextPoint;

				if ( MOVE_TO ==	m_Points[i].TYPE )
				{
					NextPoint.Format ( L"M %f %f", 
						m_Points[i].X[0] * ScaleX, m_Points[i].Y[0] * ScaleY );

					MovePath += NextPoint;
				}

				if ( LINE_TO ==	m_Points[i].TYPE )
				{
					NextPoint.Format ( L"L %f %f", 
						m_Points[i].X[0] * ScaleX, m_Points[i].Y[0] * ScaleY );

					MovePath += NextPoint;
				}

				if ( CURVE_TO == m_Points[i].TYPE )
				{
					NextPoint.Format ( L"C %f %f %f %f %f %f", 
						m_Points[i].X[0] * ScaleX, m_Points[i].Y[0] * ScaleY,
						m_Points[i].X[1] * ScaleX, m_Points[i].Y[1] * ScaleY,
						m_Points[i].X[2] * ScaleX, m_Points[i].Y[2] * ScaleY );

					MovePath += NextPoint;
				}

				if ( CLOSE_LOOP == m_Points[i].TYPE )
				{
					MovePath	+=	CStringW ( L"Z" );
				}

				if ( END == m_Points[i].TYPE )
				{
					MovePath	+=	CStringW ( L"E" );
				}

				if ( i != m_Points.GetSize() - 1 ) 
					MovePath += CStringW ( L" ");
			}

			return MovePath;
		}

	public:

		CSimpleArray < ActionPoint >	m_Points;
	};

/*
	// factoty method
	class CAnimationFactory
	{
	public:
		static IRecord* BuildAnimationObject ( WORD Type );
	};
*/
	// Helpers
	class ODHelpers
	{
	public:
		static CString GetTimePropertyID4TimeNode			( TimePropertyID4TimeNode Value );
		static CString GetTimeVariantTypeEnum				( TimeVariantTypeEnum Value );
		static CString GetTimeNodeTypeEnum					( TimeNodeTypeEnum Value );
		static CString GetTriggerObjectEnum					( TriggerObjectEnum Value );
		static CString GetTimeVisualElementEnum				( TimeVisualElementEnum Value );
		static CString GetElementTypeEnum					( ElementTypeEnum Value );
		static CString GetTimeAnimateBehaviorValueTypeEnum	( TimeAnimateBehaviorValueTypeEnum Value );
		static CString IntToHexString						( DWORD Value );
		static CString DoubleToString						( DOUBLE Value );
		static CString IntToString							( int Value );
		static CString GetAnimationClassName				( AnimationsClassType Value );

		static CString GetEffectTypeOfGroup					( DWORD Value );
		//	the corresponding effect type is an entrance or an exit effect
		static CString GetEffectEntranceOrExitNameByID		( DWORD EffectID );
		//	the corresponding effect type is an emphasis effect
		static CString GetEffectEmphasisNameByID			( DWORD EffectID );
		//	the corresponding effect type is a motion path effect
		static CString GetEffectMotionPathNameByID			( DWORD EffectID );

		static CString GetEffectNameByID					( DWORD EffectType, DWORD EffectID );
	};
}

namespace Animations
{
	struct ODTimeVariant// : public IRecord
	{
		TimeVariantTypeEnum		m_Type;

		ODTimeVariant()
		{
			m_Type = TL_TVT_Int;
		}
	};

	struct ODTimeVariantBool : public ODTimeVariant
	{
		bool					m_Value;

		ODTimeVariantBool()
		{
			m_Type = TL_TVT_Bool;
			m_Value = false;
		}
	};

	struct ODTimeVariantInt : public ODTimeVariant
	{
		DWORD					m_Value;

		ODTimeVariantInt()
		{
			m_Type = TL_TVT_Int;
			m_Value = 0;
		}
	};

	struct ODTimeVariantFloat : public ODTimeVariant
	{
		FLOAT					m_Value;

		ODTimeVariantFloat()
		{
			m_Type = TL_TVT_Float;
			m_Value = 0.0;
		}
	};

	struct ODTimeVariantString : public ODTimeVariant
	{
		CStringW				m_Value;

		ODTimeVariantString()
		{
			m_Type = TL_TVT_String;
			m_Value = L"";
		}
	};

	struct ODTimeStringListContainer// : public IRecord
	{
		CSimpleArray <ODTimeVariantString>	m_Values;
	};
	// structures for ExtTimeNodeContainer
	struct ODTimeNodeAtom// : public IRecord
	{
		static const DWORD RT_TimeSequenceData = 0xF141;

		DWORD				m_dwRestart;
		TimeNodeTypeEnum	m_dwType;
		DWORD				m_dwFill;
		DWORD				m_dwDuration;

		bool				m_bFillProperty;
		bool				m_bRestartProperty;
		bool				m_bGroupingTypeProperty;
		bool				m_bDurationProperty;

		ODTimeNodeAtom()
		{
			m_bFillProperty = false;
			m_bRestartProperty = false;
			m_bGroupingTypeProperty = false;
			m_bDurationProperty = false;
		}
	};

	struct ODTimeSequenceDataAtom //: public IRecord
	{
		DWORD		m_nConcurrency;
		DWORD		m_nNextAction;
		DWORD		m_nPreviousAction;

		bool		m_bConcurrencyPropertyUsed;
		bool		m_bNextActionPropertyUsed;
		bool		m_bPreviousActionPropertyUsed;

		ODTimeSequenceDataAtom()
		{
			m_bConcurrencyPropertyUsed = false;
			m_bNextActionPropertyUsed = false;
			m_bPreviousActionPropertyUsed = false;
		}
	};
}

namespace Animations
{	
	struct ODTimeDisplayType : public ODTimeVariantInt
	{
	};

	struct ODTimeMasterRelType : public ODTimeVariantInt
	{
	};

	struct ODTimeSlaveType : public ODTimeVariantInt
	{
	};

	struct ODTimeEffectID : public ODTimeVariantInt
	{
	};

	struct ODTimeEffectDir  : public ODTimeVariantInt
	{
	};

	struct ODTimeEffectType : public ODTimeVariantInt
	{
	};

	struct ODTimeAfterEffect : public ODTimeVariantBool
	{
	};
	struct ODTimeSlideCount : public ODTimeVariantInt
	{
	};

	struct ODTimeNodeTimeFilter : public ODTimeVariantString
	{
	};
	struct ODTimeEventFilter : public ODTimeVariantString
	{
	};
	struct ODTimeHideWhenStopped : public ODTimeVariantBool
	{
	};

	struct ODTimeGroupID : public ODTimeVariantInt
	{
	};

	struct ODTimeEffectNodeType  : public ODTimeVariantInt
	{
	};

	struct ODTimePlaceholderNode : public ODTimeVariantBool
	{
	};

	struct ODTimeMediaVolume : public ODTimeVariantFloat
	{
	};

	struct ODTimeMediaMute : public ODTimeVariantBool
	{
	};
	struct ODTimeZoomToFullScreen : public ODTimeVariantBool
	{
	};


	struct ODTimePropertyList4TimeNodeContainer// : public IRecord
	{
	public:
		//static const DWORD RT_TimePropertyList = 0xF13D;	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  

		ODTimePropertyList4TimeNodeContainer ()
		{
			m_nEmtyNode	=	false;

			m_EffectNodeType.m_Value = 0;
			m_EffectID.m_Value = 0;
			m_EffectType.m_Value = 0;
			m_EffectDir.m_Value = 0;
		}

		virtual ~ODTimePropertyList4TimeNodeContainer ()
		{
			ClearNodes ();
		}

		inline bool IsEmpty ()
		{
			return m_nEmtyNode;
		}
		void ClearNodes ()
		{
			for ( long i = 0; i < (long)m_arrElements.GetSize (); ++i )
			{
				RELEASEOBJECT ( m_arrElements[i] );
			}
			m_arrElements.RemoveAll ();
		}

		bool						m_nEmtyNode;

		CSimpleArray <ODTimeVariant*> m_arrElements;

		// 
		ODTimeEffectNodeType			m_EffectNodeType;
		ODTimeEffectID				m_EffectID;
		ODTimeEffectType				m_EffectType;
		ODTimeEffectDir				m_EffectDir;
	};

	struct ODTimePropertyList4TimeBehavior// : public IRecord
	{
	public:
		//static const DWORD RT_TimePropertyList = 0xF13D;	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  

		ODTimePropertyList4TimeBehavior ()
		{
		}

		virtual ~ODTimePropertyList4TimeBehavior ()
		{
			ClearNodes ();
		}

		void ClearNodes ()
		{
		}
	};
}

namespace Animations
{
	struct ODVisualShapeAtom// : public IRecord
	{
		// Привязка анимации через этот объект к ID объекту
		TimeVisualElementEnum	m_Type;	
		ElementTypeEnum			m_RefType;
		DWORD					m_nObjectIdRef;		//	ShapeIdRef || SoundIdRef
		DWORD					m_nData1;
		DWORD					m_nData2;	

		ODVisualShapeAtom()
		{
			m_nObjectIdRef = 0;
			m_nData1 = 0;
			m_nData2 = 0;
		}
	};

	struct ODVisualPageAtom// : public IRecord
	{
		TimeVisualElementEnum	m_Type;

		ODVisualPageAtom()
		{
			m_Type = TL_TVET_Page;
		}
	};

	struct ODClientVisualElementContainer// : public IRecord
	{

		ODClientVisualElementContainer ()
		{
			m_bVisualPageAtom	=	false;
			m_bVisualShapeAtom	=	false;
		}

		ODVisualPageAtom	m_oVisualPageAtom;
		ODVisualShapeAtom	m_oVisualShapeAtom;

		bool			m_bVisualPageAtom;
		bool			m_bVisualShapeAtom;
	};

	struct ODTimeBehaviorAtom// : public IRecord
	{
		bool	m_bAdditivePropertyUsed;
		bool	m_bAttributeNamesPropertyUsed;

		DWORD	m_nBehaviorAdditive;
		DWORD	m_nBehaviorAccumulate;
		DWORD	m_nBehaviorTransform;

		ODTimeBehaviorAtom()
		{
			m_bAdditivePropertyUsed = false;
			m_bAttributeNamesPropertyUsed = false;

			m_nBehaviorAdditive = 0;
			m_nBehaviorAccumulate = 0;
			m_nBehaviorTransform = 0;
		}
	};

	struct ODTimeBehaviorContainer// : public IRecord
	{
		long GetObjectID ()
		{
			return m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
		}

		ODTimeBehaviorAtom				m_oBehaviorAtom;
		ODTimeStringListContainer 		m_oStringList;
		bool							m_bIsExistPropertyList;
		ODTimePropertyList4TimeBehavior	m_oPropertyList;
		ODClientVisualElementContainer	m_oClientVisualElement;
	};

	struct ODTimeEffectBehaviorAtom// : public IRecord
	{
		bool	m_bTransitionPropertyUsed;
		bool	m_bTypePropertyUsed;
		bool	m_bProgressPropertyUsed;
		bool	m_bRuntimeContextObsolete;

		DWORD	m_nEffectTransition;

		ODTimeEffectBehaviorAtom()
		{
			m_bTransitionPropertyUsed = false;
			m_bTypePropertyUsed = false;
			m_bProgressPropertyUsed = false;
			m_bRuntimeContextObsolete = false;

			m_nEffectTransition = 0;
		}
	};

	struct ODTimeEffectBehaviorContainer// : public IRecord
	{
		ODTimeEffectBehaviorAtom	m_oEffectBehaviorAtom;
		ODTimeVariantString		m_varType;
		ODTimeVariantFloat		m_varProgres;
		ODTimeVariantString		m_varRuntimeContext;
		ODTimeBehaviorContainer	m_oBehavior;

	};
	struct ODTimeConditionAtom// : public IRecord
	{
		TriggerObjectEnum	m_TriggerObject;
		DWORD				m_nTriggerEvent;
		DWORD				m_nID;
		DWORD				m_nTimeDelay;

		ODTimeConditionAtom()
		{
			m_TriggerObject = TL_TOT_None;
			m_nTriggerEvent = 0;
			m_nID = 0;
			m_nTimeDelay = 0;
		}
	};

	struct ODTimeConditionContainer// : public IRecord
	{
		ODTimeConditionAtom				m_oTimeConditionAtom;
		ODClientVisualElementContainer	m_oVisualElement;
	};
}

namespace Animations
{
	struct ODTimeMotionBehaviorAtom// : public IRecord
	{
		bool	m_bByPropertyUsed;
		bool	m_bFromPropertyUsed;
		bool	m_bToPropertyUsed;
		bool	m_bOriginPropertyUsed;
		bool	m_bPathPropertyUsed;
		bool	m_bEditRotationPropertyUsed;
		bool	m_bPointsTypesPropertyUsed;

		FLOAT	m_nXBY;
		FLOAT	m_nYBY;
		FLOAT	m_nXFROM;
		FLOAT	m_nYFROM;
		FLOAT	m_nXTO;
		FLOAT	m_nYTO;
		DWORD	m_nBehaviorOrigin;

		ODTimeMotionBehaviorAtom()
		{
			m_bByPropertyUsed = false;
			m_bFromPropertyUsed = false;
			m_bToPropertyUsed = false;
			m_bOriginPropertyUsed = false;
			m_bPathPropertyUsed = false;
			m_bEditRotationPropertyUsed = false;
			m_bPointsTypesPropertyUsed = false;

			m_nXBY = 0.0;
			m_nYBY = 0.0;
			m_nXFROM = 0.0;
			m_nYFROM = 0.0;
			m_nXTO = 0.0;
			m_nYTO = 0.0;
			m_nBehaviorOrigin = 0;
		}
	};

	struct ODTimeMotionBehaviorContainer// : public IRecord
	{
		ODTimeMotionBehaviorAtom		m_oMotionBehaviorAtom;
		ODTimeVariantString			m_VarPath;
		ODTimeBehaviorContainer		m_oBehavior;
	};
}

namespace Animations
{
	struct ODTimeSetBehaviorAtom// : public IRecord
	{
		bool								m_bToPropertyUsed;
		bool								m_bValueTypePropertyUsed;

		TimeAnimateBehaviorValueTypeEnum	m_ValueType;
	};

	struct ODTimeSetBehaviorContainer// : public IRecord
	{
		ODTimeSetBehaviorAtom		m_oSetBehaviorAtom;
		ODTimeVariantString		m_VarTO;
		ODTimeBehaviorContainer	m_oBehavior;
	};
}

namespace Animations
{
	struct ODTimeAnimateBehaviorAtom// : public IRecord
	{
		DWORD								m_nCalcMode;

		bool								m_bByPropertyUsed;
		bool								m_bFromPropertyUsed;
		bool								m_bToPropertyUsed;
		bool								m_bCalcModePropertyUsed;
		bool								m_bAnimationValuesPropertyUsed;
		bool								m_bValueTypePropertyUsed;

		TimeAnimateBehaviorValueTypeEnum	m_ValueType;

		ODTimeAnimateBehaviorAtom()
		{
			m_nCalcMode = 0;

			m_bByPropertyUsed = false;
			m_bFromPropertyUsed = false;
			m_bToPropertyUsed = false;
			m_bCalcModePropertyUsed = false;
			m_bAnimationValuesPropertyUsed = false;
			m_bValueTypePropertyUsed = false;
		}
	};

	struct ODTimeAnimationValueAtom// : public IRecord
	{
		long	m_nTime;
	};

	struct ODTimeAnimationEntry 
	{
		ODTimeAnimationValueAtom	m_oTimeAnimationValueAtom;
		ODTimeVariantString		m_VarValue;
		ODTimeVariantString		m_VarFormula;
	};

	struct ODTimeAnimationValueListContainer// : public IRecord
	{
		CSimpleArray<ODTimeAnimationEntry>	m_arrEntry;
	};

	struct ODTimeAnimateBehaviorContainer// : public IRecord
	{
		ODTimeAnimateBehaviorAtom			m_oAnimateBehaviorAtom;
		ODTimeAnimationValueListContainer	m_oAnimateValueList;
		
		ODTimeVariantString				m_VarBy;
		ODTimeVariantString				m_VarFrom;
		ODTimeVariantString				m_VarTo;

		ODTimeBehaviorContainer			m_oBehavior;
	};
}

namespace Animations
{
	// structures for PP10SlideBinaryTagExtension
	struct ODExtTimeNodeContainer//	: public IRecord
	{
	public:

		ODExtTimeNodeContainer()
		{
			m_bIsExistsTimeList						=	false;
			m_bIsExistsTimeSequence					=	false;
			m_bIsExistsTimeEffectBehavior			=	false;
			m_bIsExistsTimeMotionBehavior			=	false;
			m_bIsExistsTimeSetBehaviorContainer		=	false;
			m_bIsExistsTimeAnimateBehaviorContainer	=	false;
		}

		virtual ~ODExtTimeNodeContainer()
		{
			ClearNodes ();
		}

		void ClearNodes ()
		{
			for ( long i = 0; i < (long)m_arrTimeNodes.GetCount (); ++i )
			{
				RELEASEOBJECT ( m_arrTimeNodes[i] );
			}

			m_arrTimeNodes.RemoveAll ();
		}

		ODTimeNodeAtom							m_oTimeNodeAtom;					// 
		ODTimePropertyList4TimeNodeContainer		m_oTimePropertyList;				//	OPTIONAL
		ODTimeEffectBehaviorContainer				m_oTimeEffectBehavior;				//	OPTIONAL
		ODTimeSequenceDataAtom					m_oTimeSequenceDataAtom;			//	OPTIONAL
		ODTimeMotionBehaviorContainer				m_oTimeMotionBehavior;				//	OPTIONAL
		ODTimeSetBehaviorContainer				m_oTimeSetBehaviorContainer;		//	OPTIONAL
		ODTimeAnimateBehaviorContainer			m_oTimeAnimateBehaviorContainer;	//	OPTIONAL

		bool									m_bIsExistsTimeList;
		bool									m_bIsExistsTimeEffectBehavior;
		bool									m_bIsExistsTimeSequence;
		bool									m_bIsExistsTimeMotionBehavior;
		bool									m_bIsExistsTimeSetBehaviorContainer;
		bool									m_bIsExistsTimeAnimateBehaviorContainer;

		CAtlArray <ODTimeConditionContainer*>	m_arrTimeConditionRF;
		CAtlArray <ODExtTimeNodeContainer*>		m_arrTimeNodes;
	};

	struct ODSlideTime10Atom// : public IRecord
	{
		static const DWORD RT_SlideTime10Atom	=	0x2EEB;

		FILETIME		m_FileTime;
		SYSTEMTIME		m_SystemTime;
	};

	struct ODSlideFlags10Atom// : public IRecord
	{
		static const DWORD RT_SlideFlags10Atom = 0x2EEA;

		bool			m_bPreserveMaster;					//	A bit that specifies whether to preserve the main master slide or title 
		bool			m_bOverrideMasterAnimation;			//	A bit that specifies whether the slide does not follow the animations on the main master slide or title master slide.  
	};

	struct ODHashCode10Atom// : public IRecord
	{
	};
}

namespace Animations
{
	struct ODTimeLineElement
	{
	public:
		bool operator == ( const ODTimeLineElement& Copy )
		{
			if ( m_nDuration		== Copy.m_nDuration && 
				m_nTimeDelay		== Copy.m_nTimeDelay &&
				m_nEffectID			== Copy.m_nEffectID && 
				m_nEffectNodeType	== Copy.m_nEffectNodeType &&
				m_nEffectType		== Copy.m_nEffectType && 
				m_MotionPath		== Copy.m_MotionPath &&
				m_nObjectID			== Copy.m_nObjectID )
			{
				return true;
			}

			return false;
		}
	public:

		double		m_nTimeDelay;
		double		m_nDuration;	
		DWORD		m_nEffectID;
		DWORD		m_nEffectDir;
		DWORD		m_nEffectType;
		DWORD		m_nEffectNodeType;
		DWORD		m_nObjectID;

		CStringW	m_MotionPath;
	};

	struct ODAnimationNode
	{
		ODAnimationNode () :
							m_pArray ( NULL ), m_nTimeMax ( 0.0 ), m_nTimeOffSet ( 0.0 ), m_nFullTimeMax ( 0.0 )
		{
			m_pArray	=	new CAtlArray<ODTimeLineElement> ();
		}
		
		~ODAnimationNode ()
		{
			if ( NULL != m_pArray )
			{
				delete m_pArray;
			}
		}

		inline void Add ( ODTimeLineElement element )
		{
			if ( NULL != m_pArray )
			{
				m_pArray->Add ( element );
			}
		}

		inline size_t GetCount ()
		{
			return m_pArray->GetCount ();
		}

		double						m_nTimeOffSet;
		double						m_nTimeMax;
		double						m_nFullTimeMax;
		CAtlArray<ODTimeLineElement>*	m_pArray;
	};

	// достаточно сформировать map анимаций по индексам для объектов на слайде
	class ODCSlideTimeLine
	{
	public:
		typedef CAtlArray<ODTimeLineElement>*					ODAnimationListPtr;
		typedef CAtlArray<ODTimeLineElement>					ODAnimationList;
		typedef CSimpleMap < DWORD, ODAnimationListPtr >		ODAnimationMap;

	public:

		ODCSlideTimeLine ()
		{
//			m_nTimeLineDuration = PPT_DEFAULT_SLIDE_DURATION;
		}

		virtual ~ODCSlideTimeLine()
		{
			Clear ();
		}

		inline bool				Process ( Animations::ODExtTimeNodeContainer* pTimeNodeContainer )
		{
			if ( NULL == pTimeNodeContainer )
				return false;

			Clear ();

#ifdef _DEBUG
#ifdef ___ANIMATION_TRACE_ENABLE__
			ATLTRACE(_T("//------------------------------------Animations----------------------------------------------// \n"));
#endif
#endif
			m_nTimeLineDuration			=	0.0;

			m_bSelectSetNode				=	false;
			m_bSelectAnimNode				=	false;
			m_nSelectObjectID				=	-1;

			ExploreNodes ( pTimeNodeContainer );
			CreateMapAnimations ( );

#ifdef _DEBUG
#ifdef ___ANIMATION_TRACE_ENABLE__

			ATLTRACE(_T("Animations - duration : %f,\n"), m_nTimeLineDuration );
			ATLTRACE(_T("//--------------------------------------------------------------------------------------------// \n"));
#endif
#endif
			return true;
		}

		inline void				ExploreNodes ( Animations::ODExtTimeNodeContainer* pTimeNode )
		{
			if ( false == pTimeNode->m_oTimePropertyList.IsEmpty () )
			{
				if ( 9 !=  pTimeNode->m_oTimePropertyList.m_EffectNodeType.m_Value && 4 != pTimeNode->m_oTimePropertyList.m_EffectNodeType.m_Value )
				{
					// эффект анимации
					m_oReadAnimation.m_nEffectID		=	pTimeNode->m_oTimePropertyList.m_EffectID.m_Value;
					m_oReadAnimation.m_nEffectType		=	pTimeNode->m_oTimePropertyList.m_EffectType.m_Value;
					m_oReadAnimation.m_nEffectDir		=	pTimeNode->m_oTimePropertyList.m_EffectDir.m_Value;
					m_oReadAnimation.m_nEffectNodeType	=	pTimeNode->m_oTimePropertyList.m_EffectNodeType.m_Value;
					
					if ( pTimeNode->m_arrTimeConditionRF.GetCount () )
					{
						m_oReadAnimation.m_nTimeDelay	=	pTimeNode->m_arrTimeConditionRF[0]->m_oTimeConditionAtom.m_nTimeDelay;
					}
				}
			}

			if ( TL_TNT_Behavior == pTimeNode->m_oTimeNodeAtom.m_dwType )
			{
				//ATLTRACE ( _T("EffectName : %s \n"), Helpers::GetEffectNameByID ( m_oReadAnimation.m_nEffectType, m_oReadAnimation.m_nEffectID ) );

				// MOTION PATH
				if ( pTimeNode->m_oTimeMotionBehavior.m_oBehavior.m_oClientVisualElement.m_bVisualShapeAtom )
				{
					AddAnimation ( pTimeNode, pTimeNode->m_oTimeMotionBehavior.m_oBehavior.GetObjectID () ); //AddMotionAnimation ( pTimeNode, );
					
					m_bSelectSetNode	=	false;
					m_bSelectAnimNode	=	false;
				}
				else
				// Fade, Wheel, Box, Plus, etc
				if ( pTimeNode->m_oTimeEffectBehavior.m_oBehavior.m_oClientVisualElement.m_bVisualShapeAtom )
				{
					AddAnimation ( pTimeNode, pTimeNode->m_oTimeEffectBehavior.m_oBehavior.GetObjectID () ); //AddEffectAnimation ( pTimeNode );
					
					m_bSelectSetNode	=	false;
					m_bSelectAnimNode	=	false;
				}
				else
				// Spiral, FlyIn, etc.
				if ( pTimeNode->m_oTimeAnimateBehaviorContainer.m_oBehavior.m_oClientVisualElement.m_bVisualShapeAtom )
				{
					//ATLTRACE (_T("ShapeID : %d\n"), pTimeNode->m_oTimeAnimateBehaviorContainer.m_oBehavior.GetObjectID () );
					if ( false == m_bSelectAnimNode && m_bSelectSetNode || m_nSelectObjectID != pTimeNode->m_oTimeAnimateBehaviorContainer.m_oBehavior.GetObjectID () )
					{
						m_nSelectObjectID	=	pTimeNode->m_oTimeAnimateBehaviorContainer.m_oBehavior.GetObjectID ();
						AddAnimation ( pTimeNode, pTimeNode->m_oTimeAnimateBehaviorContainer.m_oBehavior.GetObjectID () ); //AddAnimation ( pTimeNode, ); //AddAnimateAnimation ( pTimeNode );
					}

					m_bSelectAnimNode	=	true;
				}
				else
				// Spiral, FlyIn, etc.
				if ( pTimeNode->m_oTimeSetBehaviorContainer.m_oBehavior.m_oClientVisualElement.m_bVisualShapeAtom )
				{
					m_nSelectObjectID	=	-1;
					m_bSelectAnimNode	=	false;
					m_bSelectSetNode	=	true;
				//	AddAnimation ( pTimeNode, pTimeNode->m_oTimeAnimateBehaviorContainer.m_oBehavior.GetObjectID () ); //AddAnimation ( pTimeNode, ); //AddAnimateAnimation ( pTimeNode );
				}
			}

			for ( size_t i = 0; i < pTimeNode->m_arrTimeNodes.GetCount (); ++i )
			{
//				if ( (WORD)RT_TimeExtTimeNodeContainer	==	pTimeNode->m_arrTimeNodes [i]->m_oHeader.RecType )
//				{
					if ( Animations::ODExtTimeNodeContainer* pNextNode = pTimeNode->m_arrTimeNodes [i] )
					{
						ExploreNodes ( pNextNode );
					}
//				}
			}
		}

		inline ODAnimationMap&	GetAnimation ()
		{
			return m_oAnimation;
		}

		inline double			GetTime ()
		{
			return m_nTimeLineDuration;
		}

	private:
		
		inline void				AddAnimation ( Animations::ODExtTimeNodeContainer* pTimeNode, long ObjectID )
		{
			ODTimeLineElement	element;
			
			element.m_MotionPath		=	pTimeNode->m_oTimeMotionBehavior.m_VarPath.m_Value;
			element.m_nTimeDelay		=	m_oReadAnimation.m_nTimeDelay;
			element.m_nDuration			=	pTimeNode->m_oTimeNodeAtom.m_dwDuration;
			element.m_nEffectDir		=	m_oReadAnimation.m_nEffectDir;
			element.m_nEffectID			=	m_oReadAnimation.m_nEffectID;
			element.m_nEffectNodeType	=	m_oReadAnimation.m_nEffectNodeType;
			element.m_nEffectType		=	m_oReadAnimation.m_nEffectType;
			element.m_nObjectID			=	ObjectID;

			if ( 0x00000001 == element.m_nEffectNodeType || 0 == m_NodesTL.GetCount () )
			{
				ODAnimationNode* pNode	=	new ODAnimationNode ();
				if ( NULL != pNode )
				{
					pNode->m_nTimeMax		=	element.m_nTimeDelay + element.m_nDuration;
					pNode->m_nFullTimeMax	=	element.m_nTimeDelay + element.m_nDuration;

					pNode->Add ( element );

					m_NodesTL.Add ( pNode );
#ifdef _DEBUG
#ifdef ___ANIMATION_TRACE_ENABLE__
					ATLTRACE ( _T("ShapeID : %d, EffectName : %s,\t\tEffectNode : %d, TimeBegin : %f, TimeEnd : %f, TimeMax : %f\n"), 
						ObjectID, ODHelpers::GetEffectNameByID ( element.m_nEffectType, element.m_nEffectID ), element.m_nEffectNodeType, 
						element.m_nTimeDelay, element.m_nDuration + element.m_nTimeDelay, pNode->m_nTimeMax );
#endif
#endif
				}
			}
			else
			{
				ODAnimationNode* pNode	=	m_NodesTL.GetAt ( m_NodesTL.GetCount() - 1 );
				if ( pNode )
				{
					UINT Count = (UINT)pNode->GetCount ();
					for ( UINT i = 0; i < Count; ++i )
					{
						if ( pNode->m_pArray->GetAt ( i ) == element )
							return;
					}
					
					if ( Count > 0 )
					{
						if ( 0x00000003 == element.m_nEffectNodeType )
						{
							pNode->m_nTimeOffSet	+=	pNode->m_nTimeMax;
							pNode->m_nTimeMax		=	0.0;
						}
					}
					
					if ( element.m_nTimeDelay + element.m_nDuration > pNode->m_nTimeMax )
					{
						pNode->m_nTimeMax			=	element.m_nTimeDelay + element.m_nDuration;
					}

					if ( element.m_nTimeDelay + element.m_nDuration + pNode->m_nTimeOffSet > pNode->m_nFullTimeMax )
					{
						pNode->m_nFullTimeMax		=	element.m_nTimeDelay + element.m_nDuration + pNode->m_nTimeOffSet;
					}
				
					element.m_nTimeDelay			+=	pNode->m_nTimeOffSet;
					pNode->Add ( element );

#ifdef _DEBUG
#ifdef ___ANIMATION_TRACE_ENABLE__
					ATLTRACE ( _T("ShapeID : %d, EffectName : %s,\t\tEffectNode : %d, TimeBegin : %f, TimeEnd : %f, TimeMax : %f\n"), 
						ObjectID, ODHelpers::GetEffectNameByID ( element.m_nEffectType, element.m_nEffectID ), element.m_nEffectNodeType, 
						element.m_nTimeDelay, element.m_nDuration + element.m_nTimeDelay, pNode->m_nTimeMax );
#endif
#endif
				}
			}
		}

		inline void				CreateMapAnimations ()
		{
			double NodeOffSet	=	0.0;

			for ( UINT iNode = 0; iNode < m_NodesTL.GetCount (); ++iNode )
			{
				CAtlArray<ODTimeLineElement>* pNode = m_NodesTL [ iNode ]->m_pArray;
				if ( NULL != pNode )
				{
					for ( UINT j = 0; j < pNode->GetCount (); ++j )
					{
						ODTimeLineElement element	=	pNode->GetAt ( j );
						element.m_nTimeDelay	+=	NodeOffSet;

						CAtlArray<ODTimeLineElement>* pObjectAnimations = m_oAnimation.Lookup ( element.m_nObjectID );
						if ( NULL == pObjectAnimations )
						{
							pObjectAnimations	=	new CAtlArray<ODTimeLineElement> ();
							pObjectAnimations->Add ( element );

							m_oAnimation.Add ( element.m_nObjectID, pObjectAnimations );	
						}
						else
						{
							pObjectAnimations->Add ( element );
						}

						if ( m_nTimeLineDuration < element.m_nTimeDelay + element.m_nDuration )
							m_nTimeLineDuration	=	element.m_nTimeDelay + element.m_nDuration;
					}

					NodeOffSet	+=	m_NodesTL [ iNode ]->m_nFullTimeMax;
				}
			}
		}

		inline void				Clear ()
		{
			for ( UINT i = 0; i < m_NodesTL.GetCount (); ++i )
			{
				RELEASEOBJECT ( m_NodesTL.GetAt ( i ) );
			}

			m_NodesTL.RemoveAll ();

			for ( int i = 0; i < m_oAnimation.GetSize (); ++i )
			{
				RELEASEOBJECT ( m_oAnimation.GetValueAt ( i ) );
			}

			m_oAnimation.RemoveAll ();
		}


	private:

		ODAnimationMap					m_oAnimation;
		ODTimeLineElement					m_oReadAnimation;

		double							m_nTimeLineDuration;				//	время всего таймлайна с учетом всех событий и смещений

		bool							m_bSelectSetNode;					//	защита от дубликатов
		bool							m_bSelectAnimNode;

		long							m_nSelectObjectID;

		CAtlArray < ODAnimationNode* >	m_NodesTL;
	};
}

namespace Animations
{
	namespace Serialize
	{
		static inline CString CreateAnimationSource ( CString& XmlSource, Animations::ODTimeLineElement& Element )
		{
			XmlUtils::CXmlWriter oWriter;

			// обычный эффект
			CString ImageAnimationEffect = CString ( _T("ImageAnimation-Effect") ) + ODHelpers::GetEffectNameByID ( Element.m_nEffectType, Element.m_nEffectID );

			oWriter.WriteNodeBegin	( ImageAnimationEffect, TRUE );
			oWriter.WriteAttribute	( _T("type"), ODHelpers::IntToString ( Element.m_nEffectType ) );
			oWriter.WriteAttribute	( _T("direction"), ODHelpers::IntToString ( Element.m_nEffectDir ) );
			oWriter.WriteNodeEnd	( ImageAnimationEffect, TRUE, FALSE );

			oWriter.WriteString		( XmlSource );

			oWriter.WriteNodeBegin	( _T("timeline"), TRUE );
			oWriter.WriteAttribute	( _T("begin"), ODHelpers::DoubleToString ( (double) Element.m_nTimeDelay ) );
			oWriter.WriteAttribute	( _T("end"), ODHelpers::DoubleToString ( (double) ( Element.m_nDuration + Element.m_nTimeDelay ) ) );
			oWriter.WriteNodeEnd	( _T("timeline"), TRUE, TRUE );

			oWriter.WriteNodeEnd	( ImageAnimationEffect );

			return oWriter.GetXmlString ();
		}

		static inline CString CreateAnimationSourceMP ( CString& XmlSource, CString& MotionPath, Animations::ODTimeLineElement& Element )
		{
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin	( _T("ImageAnimation-EffectMotion"), TRUE );
			oWriter.WriteAttribute	( _T("type"), ODHelpers::IntToString ( Element.m_nEffectType ) );
			oWriter.WriteAttribute	( _T("direction"), ODHelpers::IntToString ( Element.m_nEffectDir ) );
			oWriter.WriteNodeEnd	( _T("ImageAnimation-EffectMotion"), TRUE, FALSE );

			oWriter.WriteNodeBegin	( _T("path") );
			oWriter.WriteString		( MotionPath );
			oWriter.WriteNodeEnd	( _T("path") );

			oWriter.WriteString		( XmlSource );

			oWriter.WriteNodeBegin	( _T("timeline"), TRUE );
			oWriter.WriteAttribute	( _T("begin"), ODHelpers::DoubleToString ( (double) Element.m_nTimeDelay ) );
			oWriter.WriteAttribute	( _T("end"), ODHelpers::DoubleToString ( (double) ( Element.m_nDuration + Element.m_nTimeDelay ) ) );
			oWriter.WriteNodeEnd	( _T("timeline"), TRUE, TRUE );

			oWriter.WriteNodeEnd	( _T("ImageAnimation-EffectMotion") );

			return oWriter.GetXmlString ();
		}
		static inline CString CreateTransformTimeLine ( Animations::ODTimeLineElement& Element )
		{
			CString TimeLine;
			TimeLine.Format ( _T("<timeline type=\"%d\" begin=\"%lf\" end=\"%lf\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\" />"), 
				1, Element.m_nTimeDelay, Element.m_nDuration + Element.m_nTimeDelay );
			return TimeLine;
		}

		static inline CString CreateMotionPath ( const Animations::ODMotionPath& path, double ScaleX, double ScaleY )
		{
			XmlUtils::CXmlWriter oWriter;
			
			oWriter.WriteNodeBegin	( _T("motion_path"), TRUE );
			oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[0].X[0] * ScaleX ) );
			oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[0].Y[0] * ScaleY ) );
			oWriter.WriteNodeEnd	( _T("motion_path"), TRUE, FALSE );

			for ( int i = 1; i < path.m_Points.GetSize(); ++i )
			{
				if ( LINE_TO == path.m_Points[i].TYPE )
				{
					oWriter.WriteNodeBegin	( _T("move"), FALSE );
					{
						if ( i > 0 )
						{
							if ( CURVE_TO == path.m_Points[i-1].TYPE )
							{
								oWriter.WriteNodeBegin	( _T("point"), TRUE );
								oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i-1].X[2] * ScaleX ) );
								oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i-1].Y[2] * ScaleY ) );
								oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
							}
							else
							{
								oWriter.WriteNodeBegin	( _T("point"), TRUE );
								oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i-1].X[0] * ScaleX ) );
								oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i-1].Y[0] * ScaleY ) );
								oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
							}
						}
						else
						{
							oWriter.WriteNodeBegin	( _T("point"), TRUE );
							oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i-1].X[0] * ScaleX ) );
							oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i-1].Y[0] * ScaleY ) );
							oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
						}

						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i].X[0] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i].Y[0] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
					}
					oWriter.WriteNodeEnd	( _T("move") );
				}

				if ( CURVE_TO == path.m_Points[i].TYPE )
				{
					oWriter.WriteNodeBegin	( _T("move"), FALSE );
					{
						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i].X[0] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i].Y[0] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );

						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i].X[1] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i].Y[1] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );

						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), ODHelpers::DoubleToString ( path.m_Points[i].X[2] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), ODHelpers::DoubleToString ( path.m_Points[i].Y[2] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
					}
					oWriter.WriteNodeEnd	( _T("move") );
				}
			}

			oWriter.WriteNodeEnd	( _T("motion_path") );

			return oWriter.GetXmlString ();
		}
	}
}

struct ODPP10SlideBinaryTagExtension //: public IRecord
{
public:

	ODPP10SlideBinaryTagExtension ()
	{
		m_bIsExistsExtTimeNodeContainer	=	true;
		m_pExtTimeNodeContainer			=	NULL;
	}

	virtual ~ODPP10SlideBinaryTagExtension ()
	{
		if ( NULL != m_pExtTimeNodeContainer )
		{
			delete m_pExtTimeNodeContainer;
			m_pExtTimeNodeContainer = NULL;
		}
	}

	Animations::ODSlideTime10Atom			m_oSlideTime10Atom;
	Animations::ODHashCode10Atom			m_oHashCode10Atom;

	Animations::ODExtTimeNodeContainer*	m_pExtTimeNodeContainer;	// ROOT

	bool								m_bIsExistsExtTimeNodeContainer;
};

struct ODSlideProgTagsContainer// : public IRecord
{
public:

	ODSlideProgTagsContainer ()
	{
		m_bExistsPP10SlideBinaryTag	=	false;
	}

	virtual ~ODSlideProgTagsContainer ()
	{

	}

	inline Animations::ODCSlideTimeLine* CreateTimeLine ()
	{
		Animations::ODCSlideTimeLine* pSlideTimeLine = NULL;

		if ( m_bExistsPP10SlideBinaryTag )
		{
			pSlideTimeLine = new Animations::ODCSlideTimeLine ();
			if ( NULL != m_PP10SlideBinaryTagExtension.m_pExtTimeNodeContainer )
			{
				pSlideTimeLine->Process ( m_PP10SlideBinaryTagExtension.m_pExtTimeNodeContainer );
			}
		}

		return pSlideTimeLine;
	}

public:

	bool						m_bExistsPP10SlideBinaryTag;
	ODPP10SlideBinaryTagExtension	m_PP10SlideBinaryTagExtension;

//	SRecordHeader				m_oHeaderChild;

	CStringW					m_wsTagName;
};

class ODCSlideShowSlideInfoAtom
{
public:

	INT		m_nSlideTime;
	DWORD	m_nSoundRef;

	BYTE	m_nEffectDirection;
	BYTE	m_nEffectType;

	bool	m_bManualAdvance;
	//  reserved1 (1 bit)
	bool	m_bHidden;
	//  reserved1 (1 bit)
	bool	m_bSound;
	//  reserved1 (1 bit)
	bool	m_bLoopSound;
	//  reserved1 (1 bit)
	bool	m_bStopSound;
	//  reserved1 (1 bit)
	bool	m_bAutoAdvance;
	//  reserved1 (1 bit)
	bool	m_bCursorVisible;
	//  reserved1 (3 bit)

	//BYTE	m_nSpeed;
	DWORD m_nTime;

public:
	
	ODCSlideShowSlideInfoAtom()
	{
	}

	~ODCSlideShowSlideInfoAtom()
	{
	}
};