#pragma once

#include <atlcoll.h>
#include <atlsimpcoll.h>

#include "../../Reader/Records.h"

//#define	___ANIMATION_TRACE_ENABLE__
// пока уберем, нужно все оттестить без анимаций пока
//#define __USE_ANIMATION__

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
	struct MotionPath
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

	// factoty method
	class CAnimationFactory
	{
	public:
		static IRecord* BuildAnimationObject ( WORD Type );
	};
	// Helpers
	class Helpers
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
	struct TimeVariant : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//

			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000002;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeVariant"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeVariant"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("TimeVariantBool") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
	};

	struct TimeVariantBool : public TimeVariant
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	
			m_Value				=	( 0x1 == StreamUtils::ReadBYTE ( pStream ) );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000002;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeVariantBool"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeVariantBool"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("Value"),		Helpers::IntToHexString ( m_Value ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeVariantBool") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
		bool					m_Value;
	};

	struct TimeVariantInt : public TimeVariant
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	
			m_Value				=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000005;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T(" TimeVariantInt "), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T(" TimeVariantInt "), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("Value"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T(" TimeVariantInt ") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
		DWORD					m_Value;
	};

	struct TimeVariantFloat : public TimeVariant
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	
			m_Value				=	StreamUtils::ReadFLOAT ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000005;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeVariantFloat"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeVariantFloat"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("Value"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeVariantFloat") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
		FLOAT					m_Value;
	};

	struct TimeVariantString : public TimeVariant
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	

			if ( WCHAR* pString = new WCHAR [ m_oHeader.RecLen / 2 ] )
			{
				if ( SUCCEEDED ( pStream->Read ( pString, ( m_oHeader.RecLen / 2 ) * 2, NULL ) ) )
				{			
					m_Value		=	CStringW ( pString );
				}

				RELEASEARRAYOBJECTS ( pString ); 
			}
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142;
			//	&&	m_oHeader.RecLen % 2		==	0x00000001;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeVariantString"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeVariantString"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("StringValue"),	CString ( CW2A ( m_Value ) ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeVariantString") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
		CStringW				m_Value;
	};

	struct TimeStringListContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

			UINT lCurLen		=	0;
			ULONG lReadLen		=	0;

			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( FAILED ( pStream->Read ( &ReadHeader, sizeof ( ReadHeader ), &lReadLen ) ) )
					break;

				if ( 0 >= lReadLen )
					break;

				TimeVariantString Element;	
				Element.ReadFromStream  ( ReadHeader, pStream );
				lCurLen += 8 + ReadHeader.RecLen;


				m_Values.Add ( Element );
			}
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecType			==	0xF13E &&
				m_oHeader.RecInstance		==	0x001;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeVariantString"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeVariantString"), TRUE, FALSE );

			for ( int i = 0; i < m_Values.GetSize(); ++i )
			{
				CDirectory::WriteValueToNode ( _T("Value"),	CString ( CW2A ( m_Values [i].m_Value ) ), &oWriter );
			}

			oWriter.WriteNodeEnd ( _T("TimeVariantString") );

			return oWriter.GetXmlString();
		}

	public:

		CSimpleArray <TimeVariantString>	m_Values;
	};
	// structures for ExtTimeNodeContainer
	struct TimeNodeAtom : public IRecord
	{
		static const DWORD RT_TimeSequenceData = 0xF141;

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition(lPos, pStream);

			StreamUtils::StreamSkip ( sizeof ( DWORD ), pStream );

			//	0x00000000  Does not restart. 
			//	0x00000001  Can restart at any time.  
			//	0x00000002  Can restart when the corresponding time node is not active.  
			//	0x00000003  Same as 0x00000000.  
			m_dwRestart				=	StreamUtils::ReadDWORD ( pStream );
			//	0x00000000 specifies that this time node is a parallel time node,
			//	which allows all of its child nodes to start at the same time. 
			m_dwType				=	(TimeNodeTypeEnum)StreamUtils::ReadDWORD ( pStream );

			//	0x00000000  The properties remain at their ending values while the parent time node is still running or holding. After which, the properties reset to  their original values.  
			//	0x00000001  The properties reset to their original values after the time node becomes inactive. 
			//	0x00000002  The properties remain at their ending values while the parent time node is still running or holding, or until another sibling time node is started under a sequential time node as  specified in the type field. After which, the  properties reset to their original values.  
			//	0x00000003  Same as 0x00000000.  
			//	0x00000004  Same as 0x00000001.  
			m_dwFill				=	StreamUtils::ReadDWORD ( pStream );

			StreamUtils::StreamSkip ( sizeof ( DWORD ), pStream );
			StreamUtils::StreamSkip ( sizeof ( DWORD ),	pStream );

			// duration: 0xFFFFFFFF specifies that the duration of the time node is infinite, 
			// and that its actual duration is determined by the durations of its child nodes. 
			m_dwDuration			=	StreamUtils::ReadDWORD ( pStream );

			DWORD Value				=	StreamUtils::ReadDWORD ( pStream );

			m_bFillProperty			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bRestartProperty		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

			m_bGroupingTypeProperty	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
			m_bDurationProperty		=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance	==	0x0	&&
				m_oHeader.RecType		==	0xF127	&&
				m_oHeader.RecLen		==	0x00000020;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeNodeAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeNodeAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Restart"),				(DWORD)m_dwRestart, &oWriter );
			CDirectory::WriteValueToNode ( _T("Type"),					Helpers::GetTimeNodeTypeEnum ( m_dwType ), &oWriter );
			CDirectory::WriteValueToNode ( _T("Fill"),					(DWORD)m_dwFill, &oWriter );
			CDirectory::WriteValueToNode ( _T("Duration"),				(DWORD)m_dwDuration, &oWriter );
			CDirectory::WriteValueToNode ( _T("FillProperty"),			(DWORD)m_bFillProperty, &oWriter );
			CDirectory::WriteValueToNode ( _T("RestartProperty"),		(DWORD)m_bRestartProperty, &oWriter );
			CDirectory::WriteValueToNode ( _T("GroupingTypeProperty"),	(DWORD)m_bGroupingTypeProperty, &oWriter );
			CDirectory::WriteValueToNode ( _T("DurationProperty"),		(DWORD)m_bDurationProperty, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeNodeAtom") );

			return oWriter.GetXmlString();
		}


	public:

		DWORD				m_dwRestart;
		TimeNodeTypeEnum	m_dwType;
		DWORD				m_dwFill;
		DWORD				m_dwDuration;

		bool				m_bFillProperty;
		bool				m_bRestartProperty;
		bool				m_bGroupingTypeProperty;
		bool				m_bDurationProperty;
	};

	struct TimeSequenceDataAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			// LONG lPos = 0;		StreamUtils::StreamPosition ( lPos, pStream );

			m_nConcurrency					=	StreamUtils::ReadDWORD ( pStream );
			m_nNextAction					=	StreamUtils::ReadDWORD ( pStream );
			m_nPreviousAction				=	StreamUtils::ReadDWORD ( pStream );

			StreamUtils::StreamSkip ( sizeof ( DWORD ),	pStream );

			DWORD Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bConcurrencyPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bNextActionPropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			m_bPreviousActionPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0	&&
				m_oHeader.RecType			==	0xF141 &&
				m_oHeader.RecLen			==	0x00000014;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeSequenceDataAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd (_T("TimeSequenceDataAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Concurrency"),			(DWORD)m_nConcurrency, &oWriter );
			CDirectory::WriteValueToNode ( _T("NextAction"),			(DWORD)m_nNextAction, &oWriter );
			CDirectory::WriteValueToNode ( _T("PreviousAction"),		(DWORD)m_nPreviousAction, &oWriter );

			CDirectory::WriteValueToNode ( _T("ConcurrencyPropertyUsed"),	(DWORD)m_bConcurrencyPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("NextActionPropertyUsed"),	(DWORD)m_bNextActionPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("PreviousActionPropertyUsed"),(DWORD)m_bPreviousActionPropertyUsed, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeSequenceDataAtom") );

			return oWriter.GetXmlString();
		}

	public:

		DWORD		m_nConcurrency;
		DWORD		m_nNextAction;
		DWORD		m_nPreviousAction;

		bool		m_bConcurrencyPropertyUsed;
		bool		m_bNextActionPropertyUsed;
		bool		m_bPreviousActionPropertyUsed;
	};
}

namespace Animations
{	
	struct TimeDisplayType : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeDisplayType"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeDisplayType"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("DisplayType"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeDisplayType") );

			return oWriter.GetXmlString();
		}


	};

	struct TimeMasterRelType : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeMasterRelType"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeMasterRelType"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("MasterRel"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeMasterRelType") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeSlaveType : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeSlaveType"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeSlaveType"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("SlaveType"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeSlaveType") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeEffectID : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEffectID"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEffectID"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("EffectID"),		Helpers::IntToHexString ( m_Value ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeEffectID") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeEffectDir  : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEffectDir"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEffectDir"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("EffectDir"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeEffectDir") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeEffectType : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEffectType"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEffectType"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("EffectType"),	Helpers::GetEffectTypeOfGroup ( m_Value ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeEffectType") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeAfterEffect : public TimeVariantBool
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeAfterEffect"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeAfterEffect"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("AfterEffect"),		m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeAfterEffect") );

			return oWriter.GetXmlString();
		}
	};
	struct TimeSlideCount : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeSlideCount"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeSlideCount"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("SlideCount"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeSlideCount") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeNodeTimeFilter : public TimeVariantString
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeNodeTimeFilter"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeNodeTimeFilter"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("StringValue"),	CString ( CW2A ( m_Value ) ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeNodeTimeFilter") );

			return oWriter.GetXmlString();
		}
	};
	struct TimeEventFilter : public TimeVariantString
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEventFilter"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEventFilter"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("StringValue"),	CString ( CW2A ( m_Value ) ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeEventFilter") );

			return oWriter.GetXmlString();
		}
	};
	struct TimeHideWhenStopped : public TimeVariantBool
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeHideWhenStopped"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeHideWhenStopped"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("HideWhenStopped"),	m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeHideWhenStopped") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeGroupID : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeGroupID"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeGroupID"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("GroupID"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeGroupID") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeEffectNodeType  : public TimeVariantInt
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEffectNodeType"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEffectNodeType"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("EffectNodeType"),	(DWORD)m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeEffectNodeType") );

			return oWriter.GetXmlString();
		}
	};

	struct TimePlaceholderNode : public TimeVariantBool
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimePlaceholderNode"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimePlaceholderNode"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("PlaceholderNode"),	m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimePlaceholderNode") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeMediaVolume : public TimeVariantFloat
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeMediaVolume"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeMediaVolume"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("MediaVolume"),	Helpers::DoubleToString ( (DOUBLE)m_Value ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeMediaVolume") );

			return oWriter.GetXmlString();
		}
	};

	struct TimeMediaMute : public TimeVariantBool
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeMediaMute"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeMediaMute"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("MediaMute"),		m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeMediaMute") );

			return oWriter.GetXmlString();
		}
	};
	struct TimeZoomToFullScreen : public TimeVariantBool
	{
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeZoomToFullScreen"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeZoomToFullScreen"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
			CDirectory::WriteValueToNode ( _T("ZoomToFullScreen"),	m_Value, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeZoomToFullScreen") );

			return oWriter.GetXmlString();
		}
	};


	struct TimePropertyList4TimeNodeContainer : public IRecord
	{
	public:
		//static const DWORD RT_TimePropertyList = 0xF13D;	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  

		TimePropertyList4TimeNodeContainer ()
		{
			m_nEmtyNode	=	false;
		}

		virtual ~TimePropertyList4TimeNodeContainer ()
		{
			ClearNodes ();
		}

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;
			StreamUtils::StreamPosition ( lPos, pStream );

			UINT lCurLen		=	0;
			ULONG lReadLen		=	0;

			SRecordHeader ReadHeader;

			if ( 0 == m_oHeader.RecLen )
			{
				m_nEmtyNode = true;
			}

			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( FAILED ( pStream->Read ( &ReadHeader, sizeof ( ReadHeader ), &lReadLen ) ) )
					break;

				if ( 0 >= lReadLen )
				{
					break;
				}

				TimeVariant* pRecord = NULL;

				// ATLTRACE ("%x\n",ReadHeader.RecType);

				TimePropertyID4TimeNode VariableType = ( TimePropertyID4TimeNode ) ReadHeader.RecInstance;

				switch ( VariableType )
				{
				case TL_TPID_Display			:
					pRecord		=	new TimeDisplayType ();
				case TL_TPID_MasterPos			:
					pRecord		=	new TimeMasterRelType ();
				case TL_TPID_SlaveType			:
					pRecord		=	new TimeSlaveType ();
					break;
				case TL_TPID_EffectID			:
					pRecord		=	new TimeEffectID ();
					break;
				case TL_TPID_EffectDir			:
					pRecord		=	new TimeEffectDir ();
					break;
				case TL_TPID_EffectType			:
					pRecord		=	new TimeEffectType ();
					break;
				case TL_TPID_AfterEffect		:
					pRecord		=	new TimeAfterEffect ();
					break;
				case TL_TPID_SlideCount			:
					pRecord		=	new TimeSlideCount ();
					break;
				case TL_TPID_TimeFilter			:
					pRecord		=	new TimeNodeTimeFilter ();
					break;
				case TL_TPID_EventFilter		:
					pRecord		=	new TimeEventFilter ();
					break;
				case TL_TPID_HideWhenStopped	:
					pRecord		=	new TimeHideWhenStopped ();
					break;
				case TL_TPID_GroupID			:
					pRecord		=	new TimeGroupID ();
					break;
				case TL_TPID_EffectNodeType		:
					pRecord		=	new TimeEffectNodeType ();
					break;
				case TL_TPID_PlaceholderNode	:
					pRecord		=	new TimePlaceholderNode ();
					break;
				case TL_TPID_MediaVolume		:
					pRecord		=	new TimeMediaVolume ();
					break;
				case TL_TPID_MediaMute			:
					pRecord		=	new TimeMediaMute ();
					break;
				case TL_TPID_ZoomToFullScreen	:
					pRecord		=	new TimeZoomToFullScreen ();
					break;
#ifdef _DEBUG
				default :	assert (0);
					break;
#endif
				}

				pRecord->ReadFromStream  ( ReadHeader, pStream );
				lCurLen += 8 + ReadHeader.RecLen;

				if ( TL_TPID_EffectID == VariableType )
				{
					m_EffectID	=	*(static_cast<TimeEffectID*> ( pRecord ) );	
					//	RELEASEOBJECT ( pRecord );
					m_arrElements.Add ( pRecord );
				}
				else if ( TL_TPID_EffectType == VariableType )
				{
					m_EffectType	=	*(static_cast<TimeEffectType*> ( pRecord ) );
					//	RELEASEOBJECT ( pRecord );
					m_arrElements.Add ( pRecord );
				}
				else if  ( TL_TPID_EffectDir == VariableType )
				{
					m_EffectDir	=	*(static_cast<TimeEffectDir*> ( pRecord ) );
					//RELEASEOBJECT ( pRecord );
					m_arrElements.Add ( pRecord );
				}
				else if ( TL_TPID_EffectNodeType == VariableType )
				{
					m_EffectNodeType	=	*(static_cast<TimeEffectNodeType*> ( pRecord ) );
					RELEASEOBJECT ( pRecord );
				}
				else
				{
					m_arrElements.Add ( pRecord );
				}
			}

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13D;
		}
#endif
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

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimePropertyList4TimeNodeContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimePropertyList4TimeNodeContainer"), TRUE, FALSE );

			if ( false == m_nEmtyNode )
			{
				oWriter.WriteString ( m_EffectNodeType.ToString () );
			}

			for ( long i = 0; i < (long)m_arrElements.GetSize (); ++i )
			{
				oWriter.WriteString ( m_arrElements[i]->ToString() );
			}

			oWriter.WriteNodeEnd ( _T("TimePropertyList4TimeNodeContainer") );

			return oWriter.GetXmlString();
		}

	public:

		bool						m_nEmtyNode;

		CSimpleArray <TimeVariant*> m_arrElements;

		// 
		TimeEffectNodeType			m_EffectNodeType;
		TimeEffectID				m_EffectID;
		TimeEffectType				m_EffectType;
		TimeEffectDir				m_EffectDir;
	};

	struct TimePropertyList4TimeBehavior : public IRecord
	{
	public:
		//static const DWORD RT_TimePropertyList = 0xF13D;	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  

		TimePropertyList4TimeBehavior ()
		{
		}

		virtual ~TimePropertyList4TimeBehavior ()
		{
			ClearNodes ();
		}

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13D;
		}
#endif
		void ClearNodes ()
		{
		}

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimePropertyList4TimeBehavior"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimePropertyList4TimeBehavior"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("TimePropertyList4TimeBehavior") );

			return oWriter.GetXmlString();
		}

	public:
	};
}

namespace Animations
{
	struct VisualShapeAtom : public IRecord
	{
		// Привязка анимации через этот объект к ID объекту

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			m_Type				=	(TimeVisualElementEnum) StreamUtils::ReadDWORD ( pStream );
			m_RefType			=	(ElementTypeEnum) StreamUtils::ReadDWORD ( pStream );
			m_nObjectIdRef		=	StreamUtils::ReadDWORD ( pStream );
			m_nData1			=	StreamUtils::ReadDWORD ( pStream );
			m_nData2			=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2AFB &&
				m_oHeader.RecLen			==	0x00000014;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("VisualShapeAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("VisualShapeAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),	Helpers::GetTimeVisualElementEnum (m_Type), &oWriter );
			CDirectory::WriteValueToNode ( _T("ReferenceType"),	Helpers::GetElementTypeEnum (m_RefType), &oWriter );

			if ( TL_TVET_Shape == m_RefType )
			{
				CDirectory::WriteValueToNode ( _T("ShapeID"),	m_nObjectIdRef, &oWriter );
			}
			else
			{
				CDirectory::WriteValueToNode ( _T("Type"),	m_nObjectIdRef, &oWriter );
			}

			CDirectory::WriteValueToNode ( _T("Data1"),	m_nData1, &oWriter );
			CDirectory::WriteValueToNode ( _T("Data2"),	m_nData2, &oWriter );

			oWriter.WriteNodeEnd ( _T("VisualShapeAtom") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVisualElementEnum	m_Type;	
		ElementTypeEnum			m_RefType;
		DWORD					m_nObjectIdRef;		//	ShapeIdRef || SoundIdRef
		DWORD					m_nData1;
		DWORD					m_nData2;		
	};

	struct VisualPageAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			m_Type				=	(TimeVisualElementEnum) StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2B01 &&
				m_oHeader.RecLen			==	0x00000004;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("VisualPageAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("VisualPageAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),	Helpers::GetTimeVisualElementEnum (m_Type), &oWriter );

			oWriter.WriteNodeEnd ( _T("VisualPageAtom") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVisualElementEnum	m_Type;
	};

	struct ClientVisualElementContainer : public IRecord
	{

		ClientVisualElementContainer ()
		{
			m_bVisualPageAtom	=	false;
			m_bVisualShapeAtom	=	false;
		}

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{			
				if ( RT_VisualPageAtom == ReadHeader.RecType )
				{
					m_bVisualPageAtom	=	true;
					m_oVisualPageAtom.ReadFromStream ( ReadHeader, pStream );
				}

				if ( RT_VisualShapeAtom == ReadHeader.RecType )
				{
					m_bVisualShapeAtom	=	true;
					m_oVisualShapeAtom.ReadFromStream ( ReadHeader, pStream );
				}
			}

			//	StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13C;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("ClientVisualElementContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("ClientVisualElementContainer"), TRUE, FALSE );

			if ( m_bVisualPageAtom )
				oWriter.WriteString ( m_oVisualPageAtom.ToString () );

			if ( m_bVisualShapeAtom )
				oWriter.WriteString ( m_oVisualShapeAtom.ToString () );

			oWriter.WriteNodeEnd ( _T("ClientVisualElementContainer") );

			return oWriter.GetXmlString();
		}

	public:

		VisualPageAtom	m_oVisualPageAtom;
		VisualShapeAtom	m_oVisualShapeAtom;

		bool			m_bVisualPageAtom;
		bool			m_bVisualShapeAtom;
	};

	struct TimeBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			// LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			DWORD dwFlags					=	StreamUtils::ReadDWORD ( pStream );

			m_bAdditivePropertyUsed			=	( 0x01 == ( 0x01 & ((BYTE)dwFlags) ) );
			m_bAttributeNamesPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)dwFlags) ) );

			m_nBehaviorAdditive				=	StreamUtils::ReadDWORD ( pStream );
			m_nBehaviorAccumulate			=	StreamUtils::ReadDWORD ( pStream );
			m_nBehaviorTransform			=	StreamUtils::ReadDWORD ( pStream );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF133 &&
				m_oHeader.RecLen			==	0x00000010;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeBehaviorAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("AdditivePropertyUsed"),			m_bAdditivePropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("AttributeNamesPropertyUsed"),	m_bAttributeNamesPropertyUsed, &oWriter );

			CDirectory::WriteValueToNode ( _T("BehaviorAdditive"),				m_nBehaviorAdditive, &oWriter );
			CDirectory::WriteValueToNode ( _T("BehaviorAccumulate"),			m_nBehaviorAccumulate, &oWriter );
			CDirectory::WriteValueToNode ( _T("BehaviorTransform"),				m_nBehaviorTransform, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

		bool	m_bAdditivePropertyUsed;
		bool	m_bAttributeNamesPropertyUsed;

		DWORD	m_nBehaviorAdditive;
		DWORD	m_nBehaviorAccumulate;
		DWORD	m_nBehaviorTransform;
	};

	struct TimeBehaviorContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

			m_bIsExistPropertyList	=	false;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( m_oBehaviorAtom.m_bAttributeNamesPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_oStringList.ReadFromStream ( ReadHeader, pStream );
			}

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{
				if ( RT_TimeClientVisualElement == ReadHeader.RecType )
				{
					m_oClientVisualElement.ReadFromStream ( ReadHeader, pStream );

					StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
					return;
				}

				if ( RT_TimePropertyList == ReadHeader.RecType )
				{
					m_bIsExistPropertyList	=	true;
					m_oPropertyList.ReadFromStream ( ReadHeader, pStream );
				}
			}

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oClientVisualElement.ReadFromStream ( ReadHeader, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12A;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeBehaviorContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oBehaviorAtom.ToString () );

			if ( m_oBehaviorAtom.m_bAttributeNamesPropertyUsed )
			{
				oWriter.WriteString ( m_oStringList.ToString () );
			}

			if ( m_bIsExistPropertyList )
			{
				oWriter.WriteString ( m_oPropertyList.ToString () );
			}

			oWriter.WriteString ( m_oClientVisualElement.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeBehaviorContainer") );

			return oWriter.GetXmlString();
		}

	
		long GetObjectID ()
		{
			return m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
		}

	public:

		TimeBehaviorAtom				m_oBehaviorAtom;
		TimeStringListContainer 		m_oStringList;
		bool							m_bIsExistPropertyList;
		TimePropertyList4TimeBehavior	m_oPropertyList;
		ClientVisualElementContainer	m_oClientVisualElement;
	};

	struct TimeEffectBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			// LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			DWORD dwFlags				=	StreamUtils::ReadDWORD ( pStream );

			m_bTransitionPropertyUsed	= ( 0x01 == ( 0x01 & ((BYTE)dwFlags) ) );
			m_bTypePropertyUsed			= ( 0x02 == ( 0x02 & ((BYTE)dwFlags) ) );
			m_bProgressPropertyUsed		= ( 0x04 == ( 0x04 & ((BYTE)dwFlags) ) );
			m_bRuntimeContextObsolete	= ( 0x08 == ( 0x08 & ((BYTE)dwFlags) ) );

			m_nEffectTransition			=	StreamUtils::ReadDWORD ( pStream );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF136 &&
				m_oHeader.RecLen			==	0x00000008;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEffectBehaviorAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEffectBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("TransitionPropertyUsed"),	m_bTransitionPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("TypePropertyUsed"),			m_bTypePropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("ProgressPropertyUsed"),		m_bProgressPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("RuntimeContextObsolete"),	m_bRuntimeContextObsolete, &oWriter );

			CDirectory::WriteValueToNode ( _T("EffectTransition"),			m_nEffectTransition, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeEffectBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:
		bool	m_bTransitionPropertyUsed;
		bool	m_bTypePropertyUsed;
		bool	m_bProgressPropertyUsed;
		bool	m_bRuntimeContextObsolete;

		DWORD	m_nEffectTransition;
	};

	struct TimeEffectBehaviorContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oEffectBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( m_oEffectBehaviorAtom.m_bTypePropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_varType.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oEffectBehaviorAtom.m_bProgressPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_varProgres.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oEffectBehaviorAtom.m_bRuntimeContextObsolete )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_varRuntimeContext.ReadFromStream ( ReadHeader, pStream );
			}

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oBehavior.ReadFromStream ( ReadHeader, pStream );

			//	StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12D;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeEffectBehaviorContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeEffectBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oEffectBehaviorAtom.ToString () );

			if ( m_oEffectBehaviorAtom.m_bTypePropertyUsed )
				CDirectory::WriteValueToNode ( _T("TransitionEffect"),	CString ( ( CW2A ( m_varType.m_Value ) ) ), &oWriter );
			if ( m_oEffectBehaviorAtom.m_bProgressPropertyUsed )
				CDirectory::WriteValueToNode ( _T("VarProgres"),	Helpers::DoubleToString ( (DOUBLE)m_varProgres.m_Value ), &oWriter );
			if ( m_oEffectBehaviorAtom.m_bRuntimeContextObsolete )
				CDirectory::WriteValueToNode ( _T("VarRuntimeContext"),	CString ( CW2A ( m_varRuntimeContext.m_Value ) ), &oWriter );

			oWriter.WriteString ( m_oBehavior.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeEffectBehaviorContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeEffectBehaviorAtom	m_oEffectBehaviorAtom;
		TimeVariantString		m_varType;
		TimeVariantFloat		m_varProgres;
		TimeVariantString		m_varRuntimeContext;
		TimeBehaviorContainer	m_oBehavior;

	};
	struct TimeConditionAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			m_TriggerObject		=	( TriggerObjectEnum )StreamUtils::ReadDWORD ( pStream );

			m_nTriggerEvent		=	StreamUtils::ReadDWORD ( pStream );
			m_nID				=	StreamUtils::ReadDWORD ( pStream );
			m_nTimeDelay		=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF128 &&
				m_oHeader.RecLen			==	0x00000010;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeConditionAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeConditionAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("TriggerObject"),	Helpers::GetTriggerObjectEnum (m_TriggerObject), &oWriter );
			CDirectory::WriteValueToNode ( _T("TriggerEvent"),	(DWORD)m_nTriggerEvent, &oWriter );
			CDirectory::WriteValueToNode ( _T("ID"),			(DWORD)m_nID, &oWriter );
			CDirectory::WriteValueToNode ( _T("TimeDelay"),		(DWORD)m_nTimeDelay, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeConditionAtom") );

			return oWriter.GetXmlString();
		}

	public:

		TriggerObjectEnum	m_TriggerObject;
		DWORD				m_nTriggerEvent;
		DWORD				m_nID;
		DWORD				m_nTimeDelay;
	};

	struct TimeConditionContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{			
				m_oTimeConditionAtom.ReadFromStream ( ReadHeader, pStream );

				if ( TL_TOT_VisualElement == m_oTimeConditionAtom.m_TriggerObject )
				{
					if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					{			
						m_oVisualElement.ReadFromStream ( ReadHeader, pStream );
					}
				}
			}

			//	StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		>=	0x001 && m_oHeader.RecInstance <= 0x005 &&
				m_oHeader.RecType			==	0xF125;
		}

#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeConditionContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeConditionContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oTimeConditionAtom.ToString () );

			if ( TL_TOT_VisualElement == m_oTimeConditionAtom.m_TriggerObject )
			{
				oWriter.WriteString ( m_oVisualElement.ToString () );
			}

			oWriter.WriteNodeEnd ( _T("TimeConditionContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeConditionAtom				m_oTimeConditionAtom;
		ClientVisualElementContainer	m_oVisualElement;
	};
}

namespace Animations
{
	struct TimeMotionBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			m_bToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
			m_bOriginPropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
			m_bPathPropertyUsed				=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

			m_bEditRotationPropertyUsed		=	( 0x40 == ( 0x40 & ((BYTE)Value) ) );
			m_bPointsTypesPropertyUsed		=	( 0x80 == ( 0x80 & ((BYTE)Value) ) );

			m_nXBY							=	StreamUtils::ReadFLOAT ( pStream );
			m_nYBY							=	StreamUtils::ReadFLOAT ( pStream );
			m_nXFROM						=	StreamUtils::ReadFLOAT ( pStream );
			m_nYFROM						=	StreamUtils::ReadFLOAT ( pStream );
			m_nXTO							=	StreamUtils::ReadFLOAT ( pStream );
			m_nYTO							=	StreamUtils::ReadFLOAT ( pStream );
			m_nBehaviorOrigin				=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF137 &&
				m_oHeader.RecLen			==	0x00000020;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeMotionBehaviorAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeMotionBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("ByPropertyUsed"),			(DWORD)m_bByPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("FromPropertyUsed"),			(DWORD)m_bFromPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("ToPropertyUsed"),			(DWORD)m_bToPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("OriginPropertyUsed"),		(DWORD)m_bOriginPropertyUsed			, &oWriter );
			CDirectory::WriteValueToNode ( _T("PathPropertyUsed"),			(DWORD)m_bPathPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("EditRotationPropertyUsed"),	(DWORD)m_bEditRotationPropertyUsed		, &oWriter );
			CDirectory::WriteValueToNode ( _T("PointsTypesPropertyUsed"),	(DWORD)m_bPointsTypesPropertyUsed		, &oWriter );

			CDirectory::WriteValueToNode ( _T("XBY"),						Helpers::DoubleToString ( (DOUBLE)m_nXBY ), &oWriter );
			CDirectory::WriteValueToNode ( _T("YBY"),						Helpers::DoubleToString ( (DOUBLE)m_nYBY ), &oWriter );
			CDirectory::WriteValueToNode ( _T("XFROM"),						Helpers::DoubleToString ( (DOUBLE)m_nXFROM ), &oWriter );
			CDirectory::WriteValueToNode ( _T("YFROM"),						Helpers::DoubleToString ( (DOUBLE)m_nYFROM ), &oWriter );
			CDirectory::WriteValueToNode ( _T("XTO"),						Helpers::DoubleToString ( (DOUBLE)m_nXTO ), &oWriter );
			CDirectory::WriteValueToNode ( _T("YTO"),						Helpers::DoubleToString ( (DOUBLE)m_nYTO ), &oWriter );
			CDirectory::WriteValueToNode ( _T("BehaviorOrigin"),			m_nBehaviorOrigin, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeMotionBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

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
	};

	struct TimeMotionBehaviorContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oMotionBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{
				m_VarPath.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oMotionBehaviorAtom.m_bPathPropertyUsed )
			{
				//TimeVariantInt Skip;
				//if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				//	Skip.ReadFromStream ( ReadHeader, pStream );

				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_oBehavior.ReadFromStream ( ReadHeader, pStream );
			}

			//StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12E;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeMotionBehaviorContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeMotionBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oMotionBehaviorAtom.ToString () );

			CDirectory::WriteValueToNode ( _T("VarPath"), CString ( CW2A ( m_VarPath.m_Value ) ), &oWriter );

			if ( m_oMotionBehaviorAtom.m_bPathPropertyUsed )
			{
				oWriter.WriteString ( m_oBehavior.ToString () );
			}

			oWriter.WriteNodeEnd ( _T("TimeMotionBehaviorContainer") );

			return oWriter.GetXmlString();
		}


	public:

		TimeMotionBehaviorAtom		m_oMotionBehaviorAtom;
		TimeVariantString			m_VarPath;
		TimeBehaviorContainer		m_oBehavior;
	};
}

namespace Animations
{
	struct TimeSetBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bToPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bValueTypePropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			
			m_ValueType						=	(TimeAnimateBehaviorValueTypeEnum)StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13A &&
				m_oHeader.RecLen			==	0x00000008;
		}
#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeSetBehaviorAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("ToPropertyUsed"),			(DWORD)m_bToPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("ValueTypePropertyUsed"),		(DWORD)m_bValueTypePropertyUsed			, &oWriter );

			CDirectory::WriteValueToNode ( _T("ValueType"),					Helpers::GetTimeAnimateBehaviorValueTypeEnum ( m_ValueType ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

		bool								m_bToPropertyUsed;
		bool								m_bValueTypePropertyUsed;

		TimeAnimateBehaviorValueTypeEnum	m_ValueType;
	};

	struct TimeSetBehaviorContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			SRecordHeader ReadHeader;
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oSetBehaviorAtom.ReadFromStream ( ReadHeader, pStream );
		
			if ( m_oSetBehaviorAtom.m_bToPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_VarTO.ReadFromStream ( ReadHeader, pStream );
			}

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oBehavior.ReadFromStream ( ReadHeader, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF131;
		}
#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeSetBehaviorContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oSetBehaviorAtom.ToString () );
			
			if ( m_oSetBehaviorAtom.m_bToPropertyUsed )
			{
				oWriter.WriteString ( m_VarTO.ToString() );
			}
			oWriter.WriteString ( m_oSetBehaviorAtom.ToString () );

			oWriter.WriteString ( m_oBehavior.ToString() );

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorContainer") );

			return oWriter.GetXmlString();
		}


	public:

		TimeSetBehaviorAtom		m_oSetBehaviorAtom;
		TimeVariantString		m_VarTO;
		TimeBehaviorContainer	m_oBehavior;
	};
}

namespace Animations
{
	struct TimeAnimateBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			m_nCalcMode						=	StreamUtils::ReadDWORD ( pStream );

			DWORD Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			m_bToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
			m_bCalcModePropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
			m_bAnimationValuesPropertyUsed	=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );
			m_bValueTypePropertyUsed		=	( 0x20 == ( 0x20 & ((BYTE)Value) ) );
			
			m_ValueType						=	(TimeAnimateBehaviorValueTypeEnum)StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF134 &&
				m_oHeader.RecLen			==	0x0000000C;
		}
#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeAnimateBehaviorAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("ByPropertyUsed"),				(DWORD)m_bByPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("FromPropertyUsed"),				(DWORD)m_bFromPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("ToPropertyUsed"),				(DWORD)m_bToPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("CalcModePropertyUsed"),			(DWORD)m_bCalcModePropertyUsed			, &oWriter );
			CDirectory::WriteValueToNode ( _T("AnimationValuesPropertyUsed"),	(DWORD)m_bAnimationValuesPropertyUsed	, &oWriter );
			CDirectory::WriteValueToNode ( _T("ValueTypePropertyUsed"),			(DWORD)m_bValueTypePropertyUsed			, &oWriter );

			CDirectory::WriteValueToNode ( _T("ValueType"),						Helpers::GetTimeAnimateBehaviorValueTypeEnum ( m_ValueType ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeAnimateBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

		DWORD								m_nCalcMode;

		bool								m_bByPropertyUsed;
		bool								m_bFromPropertyUsed;
		bool								m_bToPropertyUsed;
		bool								m_bCalcModePropertyUsed;
		bool								m_bAnimationValuesPropertyUsed;
		bool								m_bValueTypePropertyUsed;

		TimeAnimateBehaviorValueTypeEnum	m_ValueType;
	};

	struct TimeAnimationValueAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			m_nTime				=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF143 &&
				m_oHeader.RecLen			==	0x00000004;
		}
#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeAnimationValueAtom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeAnimationValueAtom"), TRUE, FALSE );
			
			CDirectory::WriteValueToNode ( _T("Time"), (DWORD)m_nTime, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeAnimationValueAtom") );

			return oWriter.GetXmlString();
		}

	public:

		long	m_nTime;

	};

	struct TimeAnimationEntry 
	{
		virtual void ReadFromStream ( UINT& CurLen, SRecordHeader oHeader, IStream* pStream )
		{
			m_oTimeAnimationValueAtom.ReadFromStream ( oHeader, pStream );
		//	ATLTRACE(_T("Time : %d\n"), m_oTimeAnimationValueAtom.m_nTime );

			SRecordHeader ReadHeader;
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{
				m_VarValue.ReadFromStream ( ReadHeader, pStream );
				//if ( m_VarValue.m_Value.GetLength () > 1 )
				//{

				//	ATLTRACE(_T("VarValue : ") );
				//	ATLTRACE(CW2A (m_VarValue.m_Value));
				//	ATLTRACE(_T("\n"));
				//}
			}

			CurLen += 8 + ReadHeader.RecLen;

			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{
				m_VarFormula.ReadFromStream ( ReadHeader, pStream );
				//if ( m_VarFormula.m_Value.GetLength () > 1 )
				//{
				//	ATLTRACE(_T("Formula : ") );
				//	ATLTRACE(CW2A (m_VarFormula.m_Value));
				//	ATLTRACE(_T("\n"));
				//}
			}
			
			CurLen += 8 + ReadHeader.RecLen;
		}		
			
		TimeAnimationValueAtom	m_oTimeAnimationValueAtom;
		TimeVariantString		m_VarValue;
		TimeVariantString		m_VarFormula;
	};

	struct TimeAnimationValueListContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif		
			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );
		
			UINT lCurLen	=	0;
			ULONG lReadLen	=	0;

			SRecordHeader ReadHeader;
			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( FAILED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), &lReadLen ) ) )
					break;

				if ( 0 >= lReadLen )
				{
					break;
				}

				lCurLen += 8 + ReadHeader.RecLen;

				TimeAnimationEntry	Entry;
				Entry.ReadFromStream ( lCurLen, ReadHeader, pStream );

				m_arrEntry.Add ( Entry );
			
			}
			
			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13F;
		}
#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeAnimationValueListContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeAnimationValueListContainer"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("TimeAnimationValueListContainer") );

			return oWriter.GetXmlString();
		}

	public:
		
		CSimpleArray<TimeAnimationEntry>	m_arrEntry;
	};

	struct TimeAnimateBehaviorContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif		
			SRecordHeader ReadHeader;
			
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oAnimateBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( m_oAnimateBehaviorAtom.m_bAnimationValuesPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_oAnimateValueList.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oAnimateBehaviorAtom.m_bByPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_VarBy.ReadFromStream ( ReadHeader, pStream );
			}
			
			if ( m_oAnimateBehaviorAtom.m_bFromPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_VarFrom.ReadFromStream ( ReadHeader, pStream );
			}
		
			if ( m_oAnimateBehaviorAtom.m_bToPropertyUsed )
			{
				if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
					m_VarTo.ReadFromStream ( ReadHeader, pStream );
			}
		
			if ( SUCCEEDED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				m_oBehavior.ReadFromStream ( ReadHeader, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12B;
		}
#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeAnimateBehaviorContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("TimeAnimateBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oAnimateBehaviorAtom.ToString () );

			if ( m_oAnimateBehaviorAtom.m_bCalcModePropertyUsed )
			{
				oWriter.WriteString ( m_oAnimateValueList.ToString () );
			}

			if ( m_oAnimateBehaviorAtom.m_bByPropertyUsed )
			{
				oWriter.WriteString ( m_VarBy.ToString () );
			}
			
			if ( m_oAnimateBehaviorAtom.m_bFromPropertyUsed )
			{
				oWriter.WriteString ( m_VarFrom.ToString () );
			}
		
			if ( m_oAnimateBehaviorAtom.m_bToPropertyUsed )
			{
				oWriter.WriteString ( m_VarTo.ToString () );
			}

			oWriter.WriteString ( m_oBehavior.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeAnimateBehaviorContainer") );

			return oWriter.GetXmlString();
		}
	public:
		
		TimeAnimateBehaviorAtom			m_oAnimateBehaviorAtom;
		TimeAnimationValueListContainer	m_oAnimateValueList;
		
		TimeVariantString				m_VarBy;
		TimeVariantString				m_VarFrom;
		TimeVariantString				m_VarTo;

		TimeBehaviorContainer			m_oBehavior;
	};
}

namespace Animations
{
	// structures for PP10SlideBinaryTagExtension
	struct ExtTimeNodeContainer	: public IRecord
	{
	public:

		ExtTimeNodeContainer()
		{
			m_bIsExistsTimeList						=	false;
			m_bIsExistsTimeSequence					=	false;
			m_bIsExistsTimeEffectBehavior			=	false;
			m_bIsExistsTimeMotionBehavior			=	false;
			m_bIsExistsTimeSetBehaviorContainer		=	false;
			m_bIsExistsTimeAnimateBehaviorContainer	=	false;
		}

		virtual ~ExtTimeNodeContainer()
		{
			ClearNodes ();
		}

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;
			StreamUtils::StreamPosition ( lPos, pStream );

			// ATLTRACE ("ExtTimeNodeContainer : Pos : %d, Length : %d\n"), lPos, oHeader.RecLen );

			SRecordHeader	rHeader;

			pStream->Read ( &rHeader, sizeof ( SRecordHeader ), NULL );
			m_oTimeNodeAtom.ReadFromStream ( rHeader, pStream );

			UINT lCurLen	=	0;
			ULONG lReadLen	=	0;

			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( FAILED ( pStream->Read ( &ReadHeader, sizeof ( SRecordHeader ), &lReadLen ) ) )
					break;

				if ( 0 >= lReadLen )
				{
					break;
				}

				IRecord* pRecord = CAnimationFactory::BuildAnimationObject ( ReadHeader.RecType );
				lCurLen += 8 + ReadHeader.RecLen;

				if ( pRecord == NULL )
				{
					// ATLTRACE ( "%x\n"), ReadHeader.RecType );

					StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
					continue;
				}

				pRecord->ReadFromStream  ( ReadHeader, pStream );

				// StreamUtils::StreamSeek ( lCurLen, pStream );

				if ( (WORD)RT_TimeExtTimeNodeContainer	==	ReadHeader.RecType )
				{
					m_arrTimeNodes.Add ( pRecord );
				}

				if ( (WORD)RT_TimeConditionContainer	==	ReadHeader.RecType )
				{
					m_arrTimeConditionRF.Add ( pRecord );
					//m_arrTimeNodes.Add ( pRecord );
				}

				if ( (WORD) RT_TimeSequenceData == ReadHeader.RecType )
				{
					m_bIsExistsTimeSequence	=	true;
					m_oTimeSequenceDataAtom	=	*( static_cast<TimeSequenceDataAtom*> ( pRecord ) );
				}

				if ( (WORD) RT_TimePropertyList	== ReadHeader.RecType )
				{
					m_bIsExistsTimeList		=	true;
					m_oTimePropertyList		=	*( static_cast<TimePropertyList4TimeNodeContainer*> ( pRecord ) );
				}

				if ( (WORD) RT_TimeEffectBehaviorContainer == ReadHeader.RecType )
				{
					m_bIsExistsTimeEffectBehavior	=	true;
					m_oTimeEffectBehavior			=	*( static_cast<TimeEffectBehaviorContainer*> ( pRecord ) );
				}

				if ( (WORD) RT_TimeMotionBehaviorContainer == ReadHeader.RecType )
				{
					m_bIsExistsTimeMotionBehavior	=	true;
					m_oTimeMotionBehavior			=	*( static_cast<TimeMotionBehaviorContainer*> ( pRecord ) );
				}

				if ( (WORD) RT_TimeSetBehaviorContainer == ReadHeader.RecType )
				{
					m_bIsExistsTimeSetBehaviorContainer	=	true;
					m_oTimeSetBehaviorContainer			=	*( static_cast<TimeSetBehaviorContainer*> ( pRecord ) );
				}

				if ( (WORD) RT_TimeAnimateBehaviorContainer == ReadHeader.RecType )
				{
					m_bIsExistsTimeAnimateBehaviorContainer	=	true;
					m_oTimeAnimateBehaviorContainer			=	*( static_cast<TimeAnimateBehaviorContainer*> ( pRecord ) );

				}
			}
			
			//ATLTRACE ("ExtTimeNodeContainer : %d\n"), m_arrTimeNodes.GetCount () );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x001 &&
				m_oHeader.RecType			==	0xF144;
		}

#endif
		void ClearNodes ()
		{
			for ( long i = 0; i < (long)m_arrTimeNodes.GetCount (); ++i )
			{
				RELEASEOBJECT ( m_arrTimeNodes[i] );
			}

			m_arrTimeNodes.RemoveAll ();
		}

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("ExtTimeNodeContainer"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("ExtTimeNodeContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oTimeNodeAtom.ToString() );

			if ( m_bIsExistsTimeList )
				oWriter.WriteString ( m_oTimePropertyList.ToString() );
			if ( m_bIsExistsTimeEffectBehavior )
				oWriter.WriteString ( m_oTimeEffectBehavior.ToString () );
			if ( m_bIsExistsTimeSequence )
				oWriter.WriteString ( m_oTimeSequenceDataAtom.ToString() );
			if ( m_bIsExistsTimeMotionBehavior )
				oWriter.WriteString ( m_oTimeMotionBehavior.ToString() );
			if ( m_bIsExistsTimeSetBehaviorContainer )
				oWriter.WriteString ( m_oTimeSetBehaviorContainer.ToString () );
			if ( m_bIsExistsTimeAnimateBehaviorContainer )
				oWriter.WriteString ( m_oTimeAnimateBehaviorContainer.ToString () );

			for ( long i = 0; i < (long)m_arrTimeNodes.GetCount (); ++i )
			{
				oWriter.WriteString ( m_arrTimeNodes[i]->ToString() );
			}

			oWriter.WriteNodeEnd ( _T("ExtTimeNodeContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeNodeAtom							m_oTimeNodeAtom;					// 
		TimePropertyList4TimeNodeContainer		m_oTimePropertyList;				//	OPTIONAL
		TimeEffectBehaviorContainer				m_oTimeEffectBehavior;				//	OPTIONAL
		TimeSequenceDataAtom					m_oTimeSequenceDataAtom;			//	OPTIONAL
		TimeMotionBehaviorContainer				m_oTimeMotionBehavior;				//	OPTIONAL
		TimeSetBehaviorContainer				m_oTimeSetBehaviorContainer;		//	OPTIONAL
		TimeAnimateBehaviorContainer			m_oTimeAnimateBehaviorContainer;	//	OPTIONAL

		bool									m_bIsExistsTimeList;
		bool									m_bIsExistsTimeEffectBehavior;
		bool									m_bIsExistsTimeSequence;
		bool									m_bIsExistsTimeMotionBehavior;
		bool									m_bIsExistsTimeSetBehaviorContainer;
		bool									m_bIsExistsTimeAnimateBehaviorContainer;

		CAtlArray <IRecord*>					m_arrTimeConditionRF;
		CAtlArray <IRecord*>					m_arrTimeNodes;
	};

	struct SlideTime10Atom : public IRecord
	{
		static const DWORD RT_SlideTime10Atom	=	0x2EEB;

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			pStream->Read ( &m_FileTime, sizeof ( FILETIME ), NULL );

			FileTimeToSystemTime ( &m_FileTime, &m_SystemTime );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2EEB &&
				m_oHeader.RecLen			==	0x00000008;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("SlideTime10Atom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("SlideTime10Atom"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("SlideTime10Atom") );

			return oWriter.GetXmlString();
		}

	public:

		SRecordHeader	m_oHeader;
		FILETIME		m_FileTime;
		SYSTEMTIME		m_SystemTime;
	};

	struct SlideFlags10Atom : public IRecord
	{
		static const DWORD RT_SlideFlags10Atom = 0x2EEA;

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD Value = 0L;
			pStream->Read ( &Value, sizeof ( Value ), NULL );

			m_bPreserveMaster			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
			m_bOverrideMasterAnimation	=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2EEA &&
				m_oHeader.RecLen			==	0x00000004;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("SlideFlags10Atom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("SlideFlags10Atom"), TRUE, FALSE );


			oWriter.WriteNodeEnd ( _T("SlideFlags10Atom") );

			return oWriter.GetXmlString();
		}

	public:

		bool			m_bPreserveMaster;					//	A bit that specifies whether to preserve the main master slide or title 
		bool			m_bOverrideMasterAnimation;			//	A bit that specifies whether the slide does not follow the animations on the main master slide or title master slide.  
	};

	struct HashCode10Atom : public IRecord
	{
		//static const DWORD RT_HashCodeAtom	=	0x2B00;

		virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
		{
			m_oHeader	=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD Value; 
			pStream->Read ( &Value, sizeof ( DWORD ), NULL );
		}

#ifdef _DEBUG
		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2B00 &&
				m_oHeader.RecLen			==	0x00000004;
		}

#endif

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("HashCode10Atom"), TRUE );

			oWriter.WriteAttribute ( _T("Length"),		Helpers::IntToHexString ( m_oHeader.RecLen ) );
			oWriter.WriteAttribute ( _T("Type"),		Helpers::IntToHexString ( m_oHeader.RecType ) );
			oWriter.WriteAttribute ( _T("Instance"),	Helpers::IntToHexString ( m_oHeader.RecInstance) );
#ifdef _DEBUG
			oWriter.WriteAttribute ( _T("Status"),		(IsCorrect ()) ? CString(_T("Valid")) : CString (_T("INVALID")) );
#endif

			oWriter.WriteNodeEnd ( _T("HashCode10Atom"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("HashCode10Atom") );

			return oWriter.GetXmlString();
		}
	};
}

namespace Animations
{
	struct TimeLineElement
	{
	public:
		bool operator == ( const TimeLineElement& Copy )
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

	struct AnimationNode
	{
		AnimationNode () :
							m_pArray ( NULL ), m_nTimeMax ( 0.0 ), m_nTimeOffSet ( 0.0 ), m_nFullTimeMax ( 0.0 )
		{
			m_pArray	=	new CAtlArray<TimeLineElement> ();
		}
		
		~AnimationNode ()
		{
			if ( NULL != m_pArray )
			{
				delete m_pArray;
			}
		}

		inline void Add ( TimeLineElement element )
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
		CAtlArray<TimeLineElement>*	m_pArray;
	};

	// достаточно сформировать map анимаций по индексам для объектов на слайде
	class CSlideTimeLine
	{
	public:
		typedef CAtlArray<TimeLineElement>*					AnimationListPtr;
		typedef CAtlArray<TimeLineElement>					AnimationList;
		typedef CSimpleMap < DWORD, AnimationListPtr >		AnimationMap;

	public:

		CSlideTimeLine ()
		{
			m_nTimeLineDuration = PPT_DEFAULT_SLIDE_DURATION;
		}

		virtual ~CSlideTimeLine()
		{
			Clear ();
		}

		inline bool				Process ( Animations::ExtTimeNodeContainer* pTimeNodeContainer )
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

		inline void				ExploreNodes ( Animations::ExtTimeNodeContainer* pTimeNode )
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
						m_oReadAnimation.m_nTimeDelay	=	
							static_cast<TimeConditionContainer*> ( pTimeNode->m_arrTimeConditionRF[0] )->m_oTimeConditionAtom.m_nTimeDelay;
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
				if ( (WORD)RT_TimeExtTimeNodeContainer	==	pTimeNode->m_arrTimeNodes [i]->m_oHeader.RecType )
				{
					if ( Animations::ExtTimeNodeContainer* pNextNode =
						static_cast<Animations::ExtTimeNodeContainer*> ( pTimeNode->m_arrTimeNodes [i] ) )
					{
						ExploreNodes ( pNextNode );
					}
				}
			}
		}

		inline AnimationMap&	GetAnimation ()
		{
			return m_oAnimation;
		}

		inline double			GetTime ()
		{
			return m_nTimeLineDuration;
		}

	private:
		
		inline void				AddAnimation ( Animations::ExtTimeNodeContainer* pTimeNode, long ObjectID )
		{
			TimeLineElement	element;
			
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
				AnimationNode* pNode	=	new AnimationNode ();
				if ( NULL != pNode )
				{
					pNode->m_nTimeMax		=	element.m_nTimeDelay + element.m_nDuration;
					pNode->m_nFullTimeMax	=	element.m_nTimeDelay + element.m_nDuration;

					pNode->Add ( element );

					m_NodesTL.Add ( pNode );
#ifdef _DEBUG
#ifdef ___ANIMATION_TRACE_ENABLE__
					ATLTRACE ( _T("ShapeID : %d, EffectName : %s,\t\tEffectNode : %d, TimeBegin : %f, TimeEnd : %f, TimeMax : %f\n"), 
						ObjectID, Helpers::GetEffectNameByID ( element.m_nEffectType, element.m_nEffectID ), element.m_nEffectNodeType, 
						element.m_nTimeDelay, element.m_nDuration + element.m_nTimeDelay, pNode->m_nTimeMax );
#endif
#endif
				}
			}
			else
			{
				AnimationNode* pNode	=	m_NodesTL.GetAt ( m_NodesTL.GetCount() - 1 );
				if ( pNode )
				{
					UINT Count = pNode->GetCount ();
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
						ObjectID, Helpers::GetEffectNameByID ( element.m_nEffectType, element.m_nEffectID ), element.m_nEffectNodeType, 
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
				CAtlArray<TimeLineElement>* pNode = m_NodesTL [ iNode ]->m_pArray;
				if ( NULL != pNode )
				{
					for ( UINT j = 0; j < pNode->GetCount (); ++j )
					{
						TimeLineElement element	=	pNode->GetAt ( j );
						element.m_nTimeDelay	+=	NodeOffSet;

						CAtlArray<TimeLineElement>* pObjectAnimations = m_oAnimation.Lookup ( element.m_nObjectID );
						if ( NULL == pObjectAnimations )
						{
							pObjectAnimations	=	new CAtlArray<TimeLineElement> ();
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

		AnimationMap					m_oAnimation;
		TimeLineElement					m_oReadAnimation;

		double							m_nTimeLineDuration;				//	время всего таймлайна с учетом всех событий и смещений

		bool							m_bSelectSetNode;					//	защита от дубликатов
		bool							m_bSelectAnimNode;

		long							m_nSelectObjectID;

		CAtlArray < AnimationNode* >	m_NodesTL;
	};
}

namespace Animations
{
	namespace Serialize
	{
		static inline CString CreateAnimationSource ( CString& XmlSource, Animations::TimeLineElement& Element )
		{
			XmlUtils::CXmlWriter oWriter;

			// обычный эффект
			CString ImageAnimationEffect = CString ( _T("ImageAnimation-Effect") ) + Helpers::GetEffectNameByID ( Element.m_nEffectType, Element.m_nEffectID );

			oWriter.WriteNodeBegin	( ImageAnimationEffect, TRUE );
			oWriter.WriteAttribute	( _T("type"), Helpers::IntToString ( Element.m_nEffectType ) );
			oWriter.WriteAttribute	( _T("direction"), Helpers::IntToString ( Element.m_nEffectDir ) );
			oWriter.WriteNodeEnd	( ImageAnimationEffect, TRUE, FALSE );

			oWriter.WriteString		( XmlSource );

			oWriter.WriteNodeBegin	( _T("timeline"), TRUE );
			oWriter.WriteAttribute	( _T("begin"), Helpers::DoubleToString ( (double) Element.m_nTimeDelay ) );
			oWriter.WriteAttribute	( _T("end"), Helpers::DoubleToString ( (double) ( Element.m_nDuration + Element.m_nTimeDelay ) ) );
			oWriter.WriteNodeEnd	( _T("timeline"), TRUE, TRUE );

			oWriter.WriteNodeEnd	( ImageAnimationEffect );

			return oWriter.GetXmlString ();
		}

		static inline CString CreateAnimationSourceMP ( CString& XmlSource, CString& MotionPath, Animations::TimeLineElement& Element )
		{
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin	( _T("ImageAnimation-EffectMotion"), TRUE );
			oWriter.WriteAttribute	( _T("type"), Helpers::IntToString ( Element.m_nEffectType ) );
			oWriter.WriteAttribute	( _T("direction"), Helpers::IntToString ( Element.m_nEffectDir ) );
			oWriter.WriteNodeEnd	( _T("ImageAnimation-EffectMotion"), TRUE, FALSE );

			oWriter.WriteNodeBegin	( _T("path") );
			oWriter.WriteString		( MotionPath );
			oWriter.WriteNodeEnd	( _T("path") );

			oWriter.WriteString		( XmlSource );

			oWriter.WriteNodeBegin	( _T("timeline"), TRUE );
			oWriter.WriteAttribute	( _T("begin"), Helpers::DoubleToString ( (double) Element.m_nTimeDelay ) );
			oWriter.WriteAttribute	( _T("end"), Helpers::DoubleToString ( (double) ( Element.m_nDuration + Element.m_nTimeDelay ) ) );
			oWriter.WriteNodeEnd	( _T("timeline"), TRUE, TRUE );

			oWriter.WriteNodeEnd	( _T("ImageAnimation-EffectMotion") );

			return oWriter.GetXmlString ();
		}
		static inline CString CreateTransformTimeLine ( Animations::TimeLineElement& Element )
		{
			CString TimeLine;
			TimeLine.Format ( _T("<timeline type=\"%d\" begin=\"%lf\" end=\"%lf\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\" />"), 
				1, Element.m_nTimeDelay, Element.m_nDuration + Element.m_nTimeDelay );
			return TimeLine;
		}

		static inline CString CreateMotionPath ( const Animations::MotionPath& path, double ScaleX, double ScaleY )
		{
			XmlUtils::CXmlWriter oWriter;
			
			oWriter.WriteNodeBegin	( _T("motion_path"), TRUE );
			oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[0].X[0] * ScaleX ) );
			oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[0].Y[0] * ScaleY ) );
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
								oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i-1].X[2] * ScaleX ) );
								oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i-1].Y[2] * ScaleY ) );
								oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
							}
							else
							{
								oWriter.WriteNodeBegin	( _T("point"), TRUE );
								oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i-1].X[0] * ScaleX ) );
								oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i-1].Y[0] * ScaleY ) );
								oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
							}
						}
						else
						{
							oWriter.WriteNodeBegin	( _T("point"), TRUE );
							oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i-1].X[0] * ScaleX ) );
							oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i-1].Y[0] * ScaleY ) );
							oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
						}

						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i].X[0] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i].Y[0] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );
					}
					oWriter.WriteNodeEnd	( _T("move") );
				}

				if ( CURVE_TO == path.m_Points[i].TYPE )
				{
					oWriter.WriteNodeBegin	( _T("move"), FALSE );
					{
						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i].X[0] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i].Y[0] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );

						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i].X[1] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i].Y[1] * ScaleY ) );
						oWriter.WriteNodeEnd	( _T("point"), TRUE, TRUE );

						oWriter.WriteNodeBegin	( _T("point"), TRUE );
						oWriter.WriteAttribute	( _T("x"), Helpers::DoubleToString ( path.m_Points[i].X[2] * ScaleX ) );
						oWriter.WriteAttribute	( _T("y"), Helpers::DoubleToString ( path.m_Points[i].Y[2] * ScaleY ) );
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

struct PP10SlideBinaryTagExtension : public IRecord
{
public:

	PP10SlideBinaryTagExtension ()
	{
		m_bIsExistsExtTimeNodeContainer	=	true;
		m_pExtTimeNodeContainer			=	NULL;
	}

	virtual ~PP10SlideBinaryTagExtension ()
	{
		if ( NULL != m_pExtTimeNodeContainer )
		{
			delete m_pExtTimeNodeContainer;
			m_pExtTimeNodeContainer = NULL;
		}
	}

	virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
	{
		m_oHeader	=	oHeader;

		LONG lPos = 0;
		StreamUtils::StreamPosition ( lPos, pStream );

		UINT lCurLen		=	0;
		ULONG lReadLen		=	0;

		SRecordHeader ReadHeader;

		while ( lCurLen < m_oHeader.RecLen )
		{
			if ( FAILED ( pStream->Read ( &ReadHeader, sizeof ( ReadHeader ), &lReadLen ) ) )
				break;

			if ( 0 >= lReadLen )
			{
				break;
			}

			IRecord* pRecord = Animations::CAnimationFactory::BuildAnimationObject ( ReadHeader.RecType );
			lCurLen += 8 + ReadHeader.RecLen;

			if ( pRecord == NULL )
			{
				//ATLTRACE ( "%x\n"), ReadHeader.RecType );
				StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
				continue;
			}

			pRecord->ReadFromStream  ( ReadHeader, pStream );

			if ( (WORD) Animations::RT_TimeExtTimeNodeContainer == ReadHeader.RecType )
			{
				m_bIsExistsExtTimeNodeContainer	=	true;
				m_pExtTimeNodeContainer			=	static_cast<Animations::ExtTimeNodeContainer*> ( pRecord );
			}
		}


		//SRecordHeader	rHeader;

		//pStream->Read ( &rHeader, sizeof ( SRecordHeader ), NULL );
		//m_oSlideTime10Atom.ReadFromStream ( rHeader, pStream );

		//pStream->Read ( &rHeader, sizeof ( SRecordHeader ), NULL );
		//m_oHashCode10Atom.ReadFromStream ( rHeader, pStream );

		//pStream->Read ( &rHeader, sizeof ( SRecordHeader ), NULL );
		//m_oExtTimeNodeContainer.ReadFromStream ( rHeader, pStream );

		StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
	}

	virtual CString ToString()
	{	
		XmlUtils::CXmlWriter oWriter;

		oWriter.WriteNodeBegin ( _T("PP10SlideBinaryTagExtension"), TRUE );

		oWriter.WriteAttribute ( _T("Length"),		Animations::Helpers::IntToHexString ( m_oHeader.RecLen ) );
		oWriter.WriteAttribute ( _T("Type"),		Animations::Helpers::IntToHexString ( m_oHeader.RecType ) );
		oWriter.WriteAttribute ( _T("Instance"),	Animations::Helpers::IntToHexString ( m_oHeader.RecInstance) );

		oWriter.WriteNodeEnd ( _T("PP10SlideBinaryTagExtension"), TRUE, FALSE );

		oWriter.WriteString ( m_oSlideTime10Atom.ToString () );
		oWriter.WriteString ( m_oHashCode10Atom.ToString () );
		
		if ( m_bIsExistsExtTimeNodeContainer )
		{
			if ( NULL != m_pExtTimeNodeContainer )
			{
				oWriter.WriteString ( m_pExtTimeNodeContainer->ToString () );
			}
		}

		oWriter.WriteNodeEnd ( _T("PP10SlideBinaryTagExtension") );

		return oWriter.GetXmlString();
	}


public:

	Animations::SlideTime10Atom			m_oSlideTime10Atom;
	Animations::HashCode10Atom			m_oHashCode10Atom;

	Animations::ExtTimeNodeContainer*	m_pExtTimeNodeContainer;	// ROOT

	bool								m_bIsExistsExtTimeNodeContainer;
};

struct SlideProgTagsContainer : public IRecord
{
public:

	SlideProgTagsContainer ()
	{
		m_bExistsPP10SlideBinaryTag	=	false;
	}

	virtual ~SlideProgTagsContainer ()
	{

	}

	virtual void ReadFromStream ( SRecordHeader oHeader, IStream* pStream )
	{
		m_oHeader	=	oHeader;

		LONG lPos = 0;
		StreamUtils::StreamPosition ( lPos, pStream );

		pStream->Read ( &m_oHeaderChild, sizeof(SRecordHeader), NULL );

		if ( RECORD_PROG_BINARY_TAG == m_oHeaderChild.RecType )
		{
#ifdef __USE_ANIMATION__

			SRecordHeader rgSubRec;
			pStream->Read ( &rgSubRec, sizeof(SRecordHeader), NULL );

			WCHAR Name[9];
			pStream->Read ( Name, 16, NULL );
			Name[8] = L'\0';

			m_wsTagName = CStringW ( Name );

			if ( CStringW ( L"___PPT10" ) == m_wsTagName )
			{
				SRecordHeader	oHeader;

				pStream->Read ( &oHeader, sizeof ( SRecordHeader ), NULL );
				m_PP10SlideBinaryTagExtension.ReadFromStream ( oHeader, pStream );

				m_bExistsPP10SlideBinaryTag		=	true;

#ifdef _DEBUG
				if ( NULL != m_PP10SlideBinaryTagExtension.m_pExtTimeNodeContainer )
				{
					XmlUtils::CXmlWriter oWriter;

					oWriter.WriteString ( m_PP10SlideBinaryTagExtension.m_pExtTimeNodeContainer->ToString() );
					oWriter.SaveToFile(_T("C:\\ppt_animations.xml"));
				}
#endif
			}

			if ( CStringW ( L"___PPT12" ) == m_wsTagName )
			{
			}

			if ( CStringW ( L"___PPT9" ) == m_wsTagName )
			{
			}

#endif
		}

		StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
	}


	virtual CString ToString()
	{	
		XmlUtils::CXmlWriter oWriter;

		oWriter.WriteNodeBegin ( _T("SlideProgTagsContainer"), TRUE );

		oWriter.WriteAttribute ( _T("Length"),		Animations::Helpers::IntToHexString ( m_oHeader.RecLen ) );
		oWriter.WriteAttribute ( _T("Type"),		Animations::Helpers::IntToHexString ( m_oHeader.RecType ) );
		oWriter.WriteAttribute ( _T("Instance"),	Animations::Helpers::IntToHexString ( m_oHeader.RecInstance) );

		oWriter.WriteNodeEnd ( _T("SlideProgTagsContainer"), TRUE, FALSE );

		if ( CStringW ( L"___PPT10" ) == m_wsTagName )
		{
			CDirectory::WriteValueToNode ( _T("TagName"),	CString(_T("___PPT10")), &oWriter );
		}

		if ( CStringW ( L"___PPT9" ) == m_wsTagName )
		{
			CDirectory::WriteValueToNode ( _T("TagName"),	CString(_T("___PPT9")), &oWriter );
		}

		if ( CStringW ( L"___PPT12" ) == m_wsTagName )
		{
			CDirectory::WriteValueToNode ( _T("TagName"),	CString(_T("___PPT12")), &oWriter );
		}

		oWriter.WriteString ( m_PP10SlideBinaryTagExtension.ToString () );

		oWriter.WriteNodeEnd ( _T("SlideProgTagsContainer") );

		return oWriter.GetXmlString();
	}


	inline Animations::CSlideTimeLine* CreateTimeLine ()
	{
		Animations::CSlideTimeLine* pSlideTimeLine = NULL;

		if ( m_bExistsPP10SlideBinaryTag )
		{
			pSlideTimeLine = new Animations::CSlideTimeLine ();
			if ( NULL != m_PP10SlideBinaryTagExtension.m_pExtTimeNodeContainer )
			{
				pSlideTimeLine->Process ( m_PP10SlideBinaryTagExtension.m_pExtTimeNodeContainer );
			}
		}

		return pSlideTimeLine;
	}

public:

	bool						m_bExistsPP10SlideBinaryTag;
	PP10SlideBinaryTagExtension	m_PP10SlideBinaryTagExtension;

	SRecordHeader				m_oHeaderChild;

	CStringW					m_wsTagName;
};