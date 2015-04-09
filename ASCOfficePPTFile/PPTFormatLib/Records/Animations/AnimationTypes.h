#pragma once

#include <vector>
#include <map>

#include "../../Reader/Records.h"
#include "../../../../ASCPresentationEditor/OfficeDrawing/Document.h"

#ifdef _DEBUG
#define	_DEBUG_LOG		//	_DEBUG_LOG
#endif

#define __USE_ANIMATION__

#define CHECK_RECORD_3X(VER,INSTANCE,TYPE) ATLASSERT(m_oHeader.RecVersion==##VER&&m_oHeader.RecInstance==##INSTANCE&&m_oHeader.RecType==##TYPE);

inline int sort (const long* a, const long* b) { return *a > *b ? 1 : -1; }

namespace debug
{
	inline static void tracearrf (std::vector<double>& arr)
	{
#if defined(_WIN32) || defined (_WIN64)
        for ( int i = 0; i < (int)arr.size(); ++i )
			ATLTRACE ( _T("%f, "), arr [i] );

        ATLTRACE ( _T("\n") );
#endif
	}
	inline static void trace (CString& str)
	{
#if defined(_WIN32) || defined (_WIN64)
        ATLTRACE (_T("%s\n"), str);
#endif
	}
};

#if !defined(_WIN32) && !defined (_WIN64)

    typedef struct _FILETIME {
        DWORD dwLowDateTime;
        DWORD dwHighDateTime;
    } FILETIME;

    typedef struct _SYSTEMTIME {
        WORD wYear;
        WORD wMonth;
        WORD wDayOfWeek;
        WORD wDay;
        WORD wHour;
        WORD wMinute;
        WORD wSecond;
        WORD wMilliseconds;
    } SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

#endif

namespace Animations
{
	enum AnimationsClassType : WORD
	{
		RT_BuildList						=	0x2B02,
		RT_BuildAtom						=	0x2B03,
		RT_ParaBuild						=	0x2B08,
		RT_ParaBuildAtom					=	0x2B09,
		RT_LevelInfoAtom					=	0x2B0A,

		RT_TimeNode							=	0xF127,
		RT_TimeVariant						=	0xF142,
		RT_TimePropertyList					=	0xF13D,		//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  
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
		RT_TimeModifierAtom					=	0xF129,
		RT_TimeScaleBehaviorContainer		=	0xF130,
		RT_TimeColorBehavior				=	0xF135,
		RT_TimeMotionBehavior				=	0xF137,
		RT_TimeRotationBehavior				=	0xF138,
		RT_TimeScaleBehavior				=	0xF139,
		RT_TimeSetBehavior					=	0xF13A,
		RT_TimeSetBehaviorContainer			=	0xF131,
		RT_TimeAnimateBehavior				=	0xF134,
		RT_TimeAnimateBehaviorContainer		=	0xF12B,
		RT_TimeVariantList					=	0xF13E,
		RT_TimeAnimationValueList			=	0xF13F,
		RT_TimeIterateData					=	0xF140,
		RT_TimeAnimationValue				=	0xF143,
		RT_TimeExtTimeNodeContainer			=	0xF144,
		RT_TimeSlaveContainer				=	0xF145
	};

	enum TimeNodeTypeEnum : DWORD
	{
		TL_TNT_Parallel				=	0x00000000,	//  Parallel time node whose child nodes can start simultaneously. 
		TL_TNT_Sequential			=	0x00000001,	//	Sequential time node whose child nodes can only start sequentially and each child can only start after its previous sibling has started.  
		TL_TNT_Behavior				=	0x00000003,	//	Behavior time node that contains a behavior. 
		TL_TNT_Media				=	0x00000004	//	Media time node that contains a media object. 
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
			m_Points.clear ();

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

				m_Points.push_back ( aPoint ); 
			}

			return ( m_Points.size() >= 2 );
		}

		inline CStringW Recalculate ( double ScaleX, double ScaleY )
		{
			CStringW	MovePath;

			for ( int i = 0; i < m_Points.size(); ++i )
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

				if ( i != m_Points.size() - 1 ) 
					MovePath += CStringW ( L" ");
			}

			return MovePath;
		}

	public:

		std::vector < ActionPoint >	m_Points;
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
        static CString DoubleToString						( double Value );
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
	struct BuildAtom : public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			buildType		=	StreamUtils::ReadDWORD ( pStream );
			buildId			=	StreamUtils::ReadDWORD ( pStream );
			shapeIdRef		=	StreamUtils::ReadDWORD ( pStream );

			DWORD Value		=	StreamUtils::ReadDWORD ( pStream );

			fExpanded		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			fUIExpanded		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_BuildAtom && m_oHeader.RecLen == 0x00000010; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("BuildAtom"), TRUE );

			oWriter.WriteNodeEnd ( _T("BuildAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("buildType"),		(DWORD)buildType, &oWriter );
			CDirectory::WriteValueToNode ( _T("buildId"),		(DWORD)buildId, &oWriter );
			CDirectory::WriteValueToNode ( _T("shapeIdRef"),	(DWORD)shapeIdRef, &oWriter );

			CDirectory::WriteValueToNode ( _T("fExpanded"),		(DWORD)fExpanded, &oWriter );
			CDirectory::WriteValueToNode ( _T("fUIExpanded"),	(DWORD)fUIExpanded, &oWriter );

			oWriter.WriteNodeEnd ( _T("BuildAtom") );

			return oWriter.GetXmlString();
		}

	public:

		DWORD	buildType;			//	1	-	Paragraph build type,	2	-	Chart build type,	3	-	Diagram build type
		DWORD	buildId;
		DWORD	shapeIdRef;

		BOOL	fExpanded;
		BOOL	fUIExpanded;
	};
	struct ParaBuildAtom : public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			paraBuild					=	StreamUtils::ReadDWORD ( pStream );
			buildLevel					=	StreamUtils::ReadDWORD ( pStream );

			DWORD Value					=	StreamUtils::ReadDWORD ( pStream );

			fAnimBackground				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			fReverse					=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			fUserSetAnimBackground		=	( 0x03 == ( 0x03 & ((BYTE)Value) ) ); 
			fAutomatic					=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );

			delayTime					=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x1 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_ParaBuildAtom && m_oHeader.RecLen == 0x00000010; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("ParaBuildAtom"), TRUE );

			oWriter.WriteNodeEnd ( _T("ParaBuildAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("paraBuild"),					(DWORD)paraBuild, &oWriter );
			CDirectory::WriteValueToNode ( _T("buildLevel"),				(DWORD)buildLevel, &oWriter );

			CDirectory::WriteValueToNode ( _T("fAnimBackground"),			(DWORD)fAnimBackground, &oWriter );
			CDirectory::WriteValueToNode ( _T("fReverse"),					(DWORD)fReverse, &oWriter );
			CDirectory::WriteValueToNode ( _T("fUserSetAnimBackground"),	(DWORD)fUserSetAnimBackground, &oWriter );
			CDirectory::WriteValueToNode ( _T("fAutomatic"),				(DWORD)fAutomatic, &oWriter );

			CDirectory::WriteValueToNode ( _T("delayTime"),					(DWORD)delayTime, &oWriter );

			oWriter.WriteNodeEnd ( _T("ParaBuildAtom") );

			return oWriter.GetXmlString();
		}

	public:

		DWORD	paraBuild;			//	0	-	All paragraphs in the shape animate at the same time. 
		//	1	-	Paragraph levels 1 to n – 1 in the shape animate separately.
		//			All paragraph levels n or greater animate at the same time. 
		//	2	-	Applies a custom animation paragraph build type to the 
		//			paragraphs of the shape. 
		//	3	-	The shape and all paragraphs within the shape animate as one 
		//			graphical object. 
		DWORD	buildLevel;

		BOOL	fAnimBackground;
		BOOL	fReverse;
		BOOL	fUserSetAnimBackground;
		BOOL	fAutomatic;

		DWORD	delayTime;
	};	
	struct LevelInfoAtom : public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader	=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			level		=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_LevelInfoAtom && m_oHeader.RecLen == 0x00000004; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("LevelInfoAtom"), TRUE );
			oWriter.WriteNodeEnd ( _T("LevelInfoAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("level"),		(DWORD)level, &oWriter );

			oWriter.WriteNodeEnd ( _T("LevelInfoAtom") );

			return oWriter.GetXmlString();
		}

	public:

		DWORD	level;			
	};

	struct ParaBuildLevel
	{
		ParaBuildLevel ()
		{
			timeNode	=	NULL;
		}

		~ParaBuildLevel()
		{
			RELEASEOBJECT (timeNode);
		}

		LevelInfoAtom	levelInfoAtom;
		IRecord*		timeNode;	// ExtTimeNodeContainer 
	};

	struct ParaBuildContainer : public IRecord
	{
	public:

		ParaBuildContainer()
		{

		}

		virtual ~ParaBuildContainer()
		{
			for ( long i = 0; i < (long)rgParaBuildLevel.size(); ++i )
				RELEASEOBJECT (rgParaBuildLevel[i]);
		}

		virtual void ReadFromStream ( SRecordHeader & thisHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	thisHeader;

#ifdef _DEBUG
			CHECK_RECORD_3X(0xF,0x0,RT_ParaBuild);
#endif
			SRecordHeader oHeader;
			UINT res = 0;
			
			if (oHeader.ReadFromStream(pStream))
				buildAtom.ReadFromStream ( oHeader, pStream );

			if (oHeader.ReadFromStream(pStream))
				paraBuildAtom.ReadFromStream ( oHeader, pStream );

			UINT lCurLen	=	0;

			while ( lCurLen < m_oHeader.RecLen )
			{
				SRecordHeader ReadHeader;
				if (ReadHeader.ReadFromStream(pStream) == FALSE)
					break;

				lCurLen			+=	8 + ReadHeader.RecLen;

				WORD nRecord	=	ReadHeader.RecType;

				if ( RT_LevelInfoAtom	== nRecord )
				{
					ParaBuildLevel* pLevel = new ParaBuildLevel ();
					if (pLevel)
					{
						pLevel->levelInfoAtom.ReadFromStream  ( ReadHeader, pStream );

						if (ReadHeader.ReadFromStream(pStream) )	
						{
							pLevel->timeNode	=	CAnimationFactory::BuildAnimationObject ( ReadHeader.RecType );
							if (pLevel->timeNode)
							{
								pLevel->timeNode->ReadFromStream  ( ReadHeader, pStream );
								rgParaBuildLevel.push_back ( pLevel );

								continue;
							}
						}
					}
				}
				StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
			}
		}
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("ParaBuildContainer"), TRUE );
			oWriter.WriteNodeEnd ( _T("ParaBuildContainer"), TRUE, FALSE );

			oWriter.WriteString ( buildAtom.ToString() );
			oWriter.WriteString ( paraBuildAtom.ToString() );
			//	rgParaBuildLevel

			oWriter.WriteNodeEnd ( _T("ParaBuildContainer") );

			return oWriter.GetXmlString();
		}

	public:

		BuildAtom		buildAtom;
		ParaBuildAtom	paraBuildAtom;

		std::vector <ParaBuildLevel*>	rgParaBuildLevel;
	};

	struct BuildListContainer : public IRecord
	{
	public:

		BuildListContainer ()
		{

		}

		virtual ~BuildListContainer()
		{
			for ( long i = 0; i < (long)rgChildRec.size(); ++i )
				RELEASEOBJECT (rgChildRec[i]);
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			CHECK_RECORD_3X(0xF,0x0,RT_BuildList);
#endif
			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );

			//SRecordHeader header;
			//if ( SUCCEEDED ( pStream->read ( &header, sizeof ( SRecordHeader ), NULL ) ) )
			//{
			//	if (RT_ParaBuild == header.RecType)
			//	{
			//		rgChildRec	=	new ParaBuildContainer ();
			//		if (rgChildRec)
			//			rgChildRec->ReadFromStream (header, pStream);
			//	}

			//	pStream->read ( &header, sizeof ( SRecordHeader ), NULL );
			//}

			//StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );

			SRecordHeader header;

			UINT lCurLen	=	0;
			ULONG lReadLen	=	0;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if (header.ReadFromStream(pStream) == FALSE)
					break;

				lCurLen			+=	8 + header.RecLen;

				WORD nRecord	=	header.RecType;

				if (RT_ParaBuild == header.RecType)
				{
					ParaBuildContainer* pContainer = new ParaBuildContainer ();
					if (pContainer)
					{
						pContainer->ReadFromStream(header, pStream);
						rgChildRec.push_back (pContainer);

						continue;
					}
				}

				StreamUtils::StreamSkip ( header.RecLen, pStream );
			}

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual CString ToString()
		{	
			if ((long)rgChildRec.size())
			{
				XmlUtils::CXmlWriter oWriter;

				oWriter.WriteNodeBegin ( _T("BuildListContainer"), TRUE );
				oWriter.WriteNodeEnd ( _T("BuildListContainer"), TRUE, FALSE );

				for ( long i = 0; i < (long)rgChildRec.size(); ++i )
					oWriter.WriteString ( rgChildRec[i]->ToString() );

				oWriter.WriteNodeEnd ( _T("BuildListContainer") );

				return oWriter.GetXmlString();
			}

			return _T("");
		}

	public:

		std::vector <ParaBuildContainer*>	rgChildRec;
		//ParaBuildContainer*	rgChildRec;
	};
}

namespace Animations
{
	struct TimeVariant : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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

			oWriter.WriteNodeEnd ( _T("TimeVariant"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("TimeVariantBool") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
	};

	struct TimeVariantBool : public TimeVariant
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	

			if ( WCHAR* pString = new WCHAR [ m_oHeader.RecLen / 2 ] )
			{
				UINT res = pStream->read ((unsigned char*) pString, ( m_oHeader.RecLen / 2 ) * 2) ;
				
				if (res >0)
				{		
					if (sizeof(wchar_t) == 4)
					{
						//todoooo
					}
					else
						stringValue		=	CStringW ( pString );
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
			oWriter.WriteNodeEnd ( _T("TimeVariantString"), TRUE, FALSE );

            CDirectory::WriteValueToNode ( _T("Type"),          Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
            CDirectory::WriteValueToNode ( _T("StringValue"),	stringValue, &oWriter ); //todooo проверить - нужен ли ansi

			oWriter.WriteNodeEnd ( _T("TimeVariantString") );

			return oWriter.GetXmlString();
		}

	public:

		TimeVariantTypeEnum		m_Type;
		CStringW				stringValue;
	};

	struct TimeStringListContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

			UINT lCurLen		=	0;
			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if (ReadHeader.ReadFromStream(pStream) == FALSE)
					break;

				TimeVariantString Element;	
				Element.ReadFromStream  ( ReadHeader, pStream );
				lCurLen += 8 + ReadHeader.RecLen;


				m_Values.push_back ( Element );
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
			oWriter.WriteNodeEnd ( _T("TimeVariantString"), TRUE, FALSE );

			for ( int i = 0; i < m_Values.size(); ++i )
			{
                CDirectory::WriteValueToNode ( _T("Value"),	 m_Values [i].stringValue, &oWriter ); //ansi ????
			}

			oWriter.WriteNodeEnd ( _T("TimeVariantString") );

			return oWriter.GetXmlString();
		}

	public:

		std::vector <TimeVariantString>	m_Values;
	};
	// structures for ExtTimeNodeContainer
	struct TimeNodeAtom : public IRecord
	{
		static const DWORD RT_TimeSequenceData = 0xF141;

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
			m_nDuration				=	StreamUtils::ReadLONG ( pStream );

			DWORD Value				=	StreamUtils::ReadDWORD ( pStream );

			m_bFillProperty			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bRestartProperty		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

			m_bGroupingTypeProperty	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
			m_bDurationProperty		=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion ==	0x0	&& m_oHeader.RecInstance ==	0x0	&& m_oHeader.RecType == 0xF127 && m_oHeader.RecLen == 0x00000020; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeNodeAtom"), TRUE );
			oWriter.WriteNodeEnd ( _T("TimeNodeAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Restart"),				(DWORD)m_dwRestart, &oWriter );
			CDirectory::WriteValueToNode ( _T("Type"),					Helpers::GetTimeNodeTypeEnum ( m_dwType ), &oWriter );
			CDirectory::WriteValueToNode ( _T("Fill"),					(DWORD)m_dwFill, &oWriter );
			CDirectory::WriteValueToNode ( _T("Duration"),				(DWORD)m_nDuration, &oWriter );
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
		long				m_nDuration;

		bool				m_bFillProperty;
		bool				m_bRestartProperty;
		bool				m_bGroupingTypeProperty;
		bool				m_bDurationProperty;
	};

	struct TimeSequenceDataAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
			oWriter.WriteNodeEnd ( _T("TimeEffectID"), TRUE, FALSE );

			// CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
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
			oWriter.WriteNodeEnd ( _T("TimeEffectDir"), TRUE, FALSE );

			//CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
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
			oWriter.WriteNodeEnd ( _T("TimeEffectType"), TRUE, FALSE );

			//CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
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
			oWriter.WriteNodeEnd ( _T("TimeNodeTimeFilter"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
            CDirectory::WriteValueToNode ( _T("StringValue"),	stringValue , &oWriter );       // ansi ????

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

			oWriter.WriteNodeEnd ( _T("TimeEventFilter"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),		Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
            CDirectory::WriteValueToNode ( _T("StringValue"),	 stringValue , &oWriter ); //ansi ????

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
			oWriter.WriteNodeEnd ( _T("TimeEffectNodeType"), TRUE, FALSE );

			//CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
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
			oWriter.WriteNodeEnd ( _T("TimeMediaVolume"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("Type"),			Helpers::GetTimeVariantTypeEnum ( m_Type ), &oWriter );
            CDirectory::WriteValueToNode ( _T("MediaVolume"),	Helpers::DoubleToString ( (double)m_Value ), &oWriter );

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
			m_nEmtyNode					=	false;
			m_EffectNodeType.m_Value	=	0;
		}

		virtual ~TimePropertyList4TimeNodeContainer ()
		{
			ClearNodes ();
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;
			StreamUtils::StreamPosition ( lPos, pStream );

			UINT lCurLen		=	0;

			SRecordHeader ReadHeader;

			if ( 0 == m_oHeader.RecLen )
				m_nEmtyNode = true;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if (ReadHeader.ReadFromStream(pStream) == FALSE)
					break;

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
					m_arrElements.push_back ( pRecord );
				}
				else if ( TL_TPID_EffectType == VariableType )
				{
					m_EffectType	=	*(static_cast<TimeEffectType*> ( pRecord ) );
					//	RELEASEOBJECT ( pRecord );
					m_arrElements.push_back ( pRecord );
				}
				else if  ( TL_TPID_EffectDir == VariableType )
				{
					m_EffectDir	=	*(static_cast<TimeEffectDir*> ( pRecord ) );
					//RELEASEOBJECT ( pRecord );
					m_arrElements.push_back ( pRecord );
				}
				else if ( TL_TPID_EffectNodeType == VariableType )
				{
					m_EffectNodeType	=	*(static_cast<TimeEffectNodeType*> ( pRecord ) );
					RELEASEOBJECT ( pRecord );
				}
				else
				{
					m_arrElements.push_back ( pRecord );
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
			for ( long i = 0; i < (long)m_arrElements.size(); ++i )
			{
				RELEASEOBJECT ( m_arrElements[i] );
			}
			m_arrElements.clear ();
		}

		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimePropertyList4TimeNodeContainer"), TRUE );


			oWriter.WriteNodeEnd ( _T("TimePropertyList4TimeNodeContainer"), TRUE, FALSE );

			if ( false == m_nEmtyNode )
			{
				oWriter.WriteString ( m_EffectNodeType.ToString () );
			}

			for ( long i = 0; i < (long)m_arrElements.size(); ++i )
			{
				oWriter.WriteString ( m_arrElements[i]->ToString() );
			}

			oWriter.WriteNodeEnd ( _T("TimePropertyList4TimeNodeContainer") );

			return oWriter.GetXmlString();
		}


		// helper

		inline const DWORD& GetEffectNodeType () const
		{
			return m_EffectNodeType.m_Value;
		}

	public:

		bool						m_nEmtyNode;

		std::vector <TimeVariant*> m_arrElements;

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

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			if (ReadHeader.ReadFromStream(pStream) )	
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		TimeBehaviorContainer ()
		{
			havePropertyList	=	FALSE;
			propertyList		=	NULL;

			haveStringList		=	FALSE;
			stringList			=	NULL;
		}

		virtual ~TimeBehaviorContainer ()
		{
			RELEASEOBJECT (propertyList);
			RELEASEOBJECT (stringList);
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

			havePropertyList	=	FALSE;
			haveStringList		=	FALSE;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			
			if ( ReadHeader.ReadFromStream(pStream) )
				behaviorAtom.ReadFromStream ( ReadHeader, pStream );

			SRecordHeader header;

			UINT lCurLen	=	0;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if (header.ReadFromStream(pStream) == FALSE)			
					break;

				lCurLen			+=	8 + header.RecLen;

				WORD nRecord	=	header.RecType;

				if ( RT_TimePropertyList == header.RecType )
				{
					propertyList		=	new TimePropertyList4TimeBehavior();
					if (propertyList)
					{
						propertyList->ReadFromStream (header, pStream);
						havePropertyList	=	TRUE;

						continue;
					}
				}

				if ( RT_TimeVariantList == header.RecType )
				{
					stringList		=	new TimeStringListContainer();
					if (stringList)
					{
						stringList->ReadFromStream (header, pStream);
						haveStringList	=	TRUE;

						continue;
					}
				}

				if ( RT_TimeClientVisualElement == header.RecType )
				{
					clientVisualElement.ReadFromStream (header, pStream);

					continue;
				}

				StreamUtils::StreamSkip ( header.RecLen, pStream );
			}

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );

			/*

			if ( behaviorAtom.m_bAttributeNamesPropertyUsed )
			{
			if ( SUCCEEDED ( pStream->read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			stringList.ReadFromStream ( ReadHeader, pStream );
			}

			if ( SUCCEEDED ( pStream->read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			{
			if ( RT_TimeClientVisualElement == ReadHeader.RecType )
			{
			clientVisualElement.ReadFromStream ( ReadHeader, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );

			return;
			}

			if ( RT_TimePropertyList == ReadHeader.RecType )
			{
			havePropertyList	=	TRUE;
			propertyList.ReadFromStream ( ReadHeader, pStream );
			}

			if ( RT_TimeVariantList == ReadHeader.RecType )
			{
			haveStringList		=	TRUE;
			stringList.ReadFromStream ( ReadHeader, pStream );
			}
			}

			if ( SUCCEEDED ( pStream->read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			clientVisualElement.ReadFromStream ( ReadHeader, pStream );
			*/

			//StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF12A; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeBehaviorContainer"), TRUE );


			oWriter.WriteNodeEnd ( _T("TimeBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString (behaviorAtom.ToString());

			if (stringList)
				oWriter.WriteString (stringList->ToString());

			if (propertyList)
				oWriter.WriteString (propertyList->ToString());

			oWriter.WriteString ( clientVisualElement.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeBehaviorContainer") );

			return oWriter.GetXmlString();
		}


		inline long GetObjectID ()
		{
			return clientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
		}

	public:

		TimeBehaviorAtom				behaviorAtom;

		TimePropertyList4TimeBehavior*	propertyList;
		BOOL							havePropertyList;

		TimeStringListContainer* 		stringList;
		BOOL							haveStringList;

		ClientVisualElementContainer	clientVisualElement;
	};

	struct TimeEffectBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader header;
			if ( header.ReadFromStream(pStream) )	
				effectBehaviorAtom.ReadFromStream ( header, pStream );

			if ( effectBehaviorAtom.m_bTypePropertyUsed )
			{
				if ( header.ReadFromStream(pStream) )	
					m_varType.ReadFromStream ( header, pStream );
			}

			if ( effectBehaviorAtom.m_bProgressPropertyUsed )
			{
				if ( header.ReadFromStream(pStream) )	
					m_varProgres.ReadFromStream ( header, pStream );
			}

			if ( effectBehaviorAtom.m_bRuntimeContextObsolete )
			{
				if ( header.ReadFromStream(pStream) )	
					m_varRuntimeContext.ReadFromStream ( header, pStream );
			}

			if ( header.ReadFromStream(pStream) )	
			{
				if (header.RecType == 0xF12A)
				{
					m_oBehavior.ReadFromStream (header, pStream);
				}
				else
				{
					StreamUtils::StreamSkip (header.RecLen, pStream);
					if ( header.ReadFromStream(pStream) )	
					{
						m_oBehavior.ReadFromStream ( header, pStream );
					}
				}
			}

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

			oWriter.WriteNodeEnd ( _T("TimeEffectBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( effectBehaviorAtom.ToString () );

			if ( effectBehaviorAtom.m_bTypePropertyUsed )
                CDirectory::WriteValueToNode ( _T("TransitionEffect"),	m_varType.stringValue , &oWriter ); //ansi ???
			if ( effectBehaviorAtom.m_bProgressPropertyUsed )
                CDirectory::WriteValueToNode ( _T("VarProgres"),	Helpers::DoubleToString ( (double)m_varProgres.m_Value ), &oWriter );
			if ( effectBehaviorAtom.m_bRuntimeContextObsolete )
                CDirectory::WriteValueToNode ( _T("VarRuntimeContext"),	m_varRuntimeContext.stringValue, &oWriter );

			oWriter.WriteString ( m_oBehavior.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeEffectBehaviorContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeEffectBehaviorAtom	effectBehaviorAtom;

		TimeVariantString		m_varType;
		TimeVariantFloat		m_varProgres;
		TimeVariantString		m_varRuntimeContext;
		TimeBehaviorContainer	m_oBehavior;

	};
	struct TimeConditionAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			m_TriggerObject		=	( TriggerObjectEnum )StreamUtils::ReadDWORD ( pStream );

			m_nTriggerEvent		=	StreamUtils::ReadDWORD ( pStream );
			m_nID				=	StreamUtils::ReadDWORD ( pStream );
			m_nTimeDelay		=	StreamUtils::ReadLONG ( pStream );
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
		LONG				m_nTimeDelay;
	};

	struct TimeConditionContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader header;
			
			if ( header.ReadFromStream(pStream) )	
			{			
				m_oTimeConditionAtom.ReadFromStream ( header, pStream );

				if ( TL_TOT_VisualElement == m_oTimeConditionAtom.m_TriggerObject )
				{
					if ( header.ReadFromStream(pStream) )	
					{			
						m_oVisualElement.ReadFromStream ( header, pStream );
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

	struct TimeIterateDataAtom: public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			iterateInterval						=	StreamUtils::ReadDWORD ( pStream );
			iterateType							=	StreamUtils::ReadDWORD ( pStream );
			iterateDirection					=	StreamUtils::ReadDWORD ( pStream );
			iterateIntervalType					=	StreamUtils::ReadDWORD ( pStream );

			DWORD Value							=	StreamUtils::ReadDWORD ( pStream );

			fIterateDirectionPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			fIterateTypePropertyUsed			=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			fIterateIntervalPropertyUsed		=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
			fIterateIntervalTypePropertyUsed	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeIterateData && m_oHeader.RecLen == 0x00000014; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeIterateDataAtom"), TRUE );

			oWriter.WriteNodeEnd ( _T("TimeIterateDataAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("iterateInterval"),		(DWORD)iterateInterval, &oWriter );
			CDirectory::WriteValueToNode ( _T("iterateType"),			(DWORD)iterateType, &oWriter );
			CDirectory::WriteValueToNode ( _T("iterateDirection"),		(DWORD)iterateDirection, &oWriter );
			CDirectory::WriteValueToNode ( _T("iterateIntervalType"),	(DWORD)iterateIntervalType, &oWriter );

			CDirectory::WriteValueToNode ( _T("fIterateDirectionPropertyUsed"),			(DWORD)fIterateDirectionPropertyUsed		, &oWriter );
			CDirectory::WriteValueToNode ( _T("fIterateTypePropertyUsed"),				(DWORD)fIterateTypePropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("fIterateIntervalPropertyUsed"),			(DWORD)fIterateIntervalPropertyUsed			, &oWriter );
			CDirectory::WriteValueToNode ( _T("fIterateIntervalTypePropertyUsed"),		(DWORD)fIterateIntervalTypePropertyUsed		, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeIterateDataAtom") );

			return oWriter.GetXmlString();
		}

	public:

		unsigned long iterateInterval;
		unsigned long iterateType;
		unsigned long iterateDirection;
		unsigned long iterateIntervalType;

		BOOL fIterateDirectionPropertyUsed;		
		BOOL fIterateTypePropertyUsed;		
		BOOL fIterateIntervalPropertyUsed;	
		BOOL fIterateIntervalTypePropertyUsed;
	};
}

namespace Animations
{
	struct TimeMotionBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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


			oWriter.WriteNodeEnd ( _T("TimeMotionBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("ByPropertyUsed"),			(DWORD)m_bByPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("FromPropertyUsed"),			(DWORD)m_bFromPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("ToPropertyUsed"),			(DWORD)m_bToPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("OriginPropertyUsed"),		(DWORD)m_bOriginPropertyUsed			, &oWriter );
			CDirectory::WriteValueToNode ( _T("PathPropertyUsed"),			(DWORD)m_bPathPropertyUsed				, &oWriter );
			CDirectory::WriteValueToNode ( _T("EditRotationPropertyUsed"),	(DWORD)m_bEditRotationPropertyUsed		, &oWriter );
			CDirectory::WriteValueToNode ( _T("PointsTypesPropertyUsed"),	(DWORD)m_bPointsTypesPropertyUsed		, &oWriter );

            CDirectory::WriteValueToNode ( _T("XBY"),						Helpers::DoubleToString ( (double)m_nXBY ), &oWriter );
            CDirectory::WriteValueToNode ( _T("YBY"),						Helpers::DoubleToString ( (double)m_nYBY ), &oWriter );
            CDirectory::WriteValueToNode ( _T("XFROM"),						Helpers::DoubleToString ( (double)m_nXFROM ), &oWriter );
            CDirectory::WriteValueToNode ( _T("YFROM"),						Helpers::DoubleToString ( (double)m_nYFROM ), &oWriter );
            CDirectory::WriteValueToNode ( _T("XTO"),						Helpers::DoubleToString ( (double)m_nXTO ), &oWriter );
            CDirectory::WriteValueToNode ( _T("YTO"),						Helpers::DoubleToString ( (double)m_nYTO ), &oWriter );
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			UINT res = 0;

			SRecordHeader header;
			if ( header.ReadFromStream(pStream) )	
			{
				m_oMotionBehaviorAtom.ReadFromStream ( header, pStream );
			}

			if ( header.ReadFromStream(pStream) )	
			{
				m_VarPath.ReadFromStream ( header, pStream );
			}

			if ( m_oMotionBehaviorAtom.m_bPathPropertyUsed )
			{
				//TimeVariantInt Skip;
				//if ( SUCCEEDED ( pStream->read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
				//	Skip.ReadFromStream ( ReadHeader, pStream );

				if ( header.ReadFromStream(pStream) )
					m_oBehavior.ReadFromStream ( header, pStream );
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

			oWriter.WriteNodeEnd ( _T("TimeMotionBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( m_oMotionBehaviorAtom.ToString () );

            CDirectory::WriteValueToNode ( _T("VarPath"), m_VarPath.stringValue , &oWriter ); ///ansi ???

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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			UINT res = 0;
			SRecordHeader ReadHeader;
			if ( ReadHeader.ReadFromStream(pStream) )	
				setBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			//if ( m_oSetBehaviorAtom.m_bToPropertyUsed )
			//{
			if ( ReadHeader.ReadFromStream(pStream) )	
				varTo.ReadFromStream ( ReadHeader, pStream );
			//}

			if ( ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream ( ReadHeader, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0xF	&& m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeSetBehaviorContainer; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeSetBehaviorContainer"), TRUE );

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( setBehaviorAtom.ToString () );

			if ( setBehaviorAtom.m_bToPropertyUsed )
				oWriter.WriteString ( varTo.ToString() );
			oWriter.WriteString ( setBehaviorAtom.ToString () );

			oWriter.WriteString ( behavior.ToString() );

			oWriter.WriteNodeEnd ( _T("TimeSetBehaviorContainer") );

			return oWriter.GetXmlString();
		}


	public:

		TimeSetBehaviorAtom		setBehaviorAtom;
		TimeVariantString		varTo;
		TimeBehaviorContainer	behavior;
	};
}

namespace Animations
{
	struct TimeAnimateBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
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
		virtual void ReadFromStream ( UINT& CurLen, SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oTimeAnimationValueAtom.ReadFromStream ( oHeader, pStream );
			//	ATLTRACE(_T("Time : %d\n"), m_oTimeAnimationValueAtom.m_nTime );

			UINT res = 0;
			SRecordHeader ReadHeader;

			if ( ReadHeader.ReadFromStream(pStream) )	
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

			if ( ReadHeader.ReadFromStream(pStream) )	
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
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif		
			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );

			UINT lCurLen	=	0;

			SRecordHeader ReadHeader;
			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( ReadHeader.ReadFromStream(pStream) == FALSE )	
				{
					break;
				}

				lCurLen += 8 + ReadHeader.RecLen;

				TimeAnimationEntry	Entry;
				Entry.ReadFromStream ( lCurLen, ReadHeader, pStream );

				m_arrEntry.push_back ( Entry );

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
			oWriter.WriteString (_T(">") );

			for ( int i = 0; i < m_arrEntry.size(); ++i )
			{
                CDirectory::WriteValueToNode ( _T("value"),	m_arrEntry[i].m_VarValue.stringValue , &oWriter );      //ansi ???
                CDirectory::WriteValueToNode ( _T("formula"), m_arrEntry[i].m_VarFormula.stringValue , &oWriter ); //ansi ???
			}

			oWriter.WriteNodeEnd ( _T("TimeAnimationValueListContainer") );

			return oWriter.GetXmlString();
		}

	public:

		std::vector<TimeAnimationEntry>	m_arrEntry;
	};

	struct TimeAnimateBehaviorContainer : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif		
			SRecordHeader ReadHeader;

			if ( ReadHeader.ReadFromStream(pStream) )	
				m_oAnimateBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( m_oAnimateBehaviorAtom.m_bAnimationValuesPropertyUsed )
			{
				if ( ReadHeader.ReadFromStream(pStream) )	
					m_oAnimateValueList.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oAnimateBehaviorAtom.m_bByPropertyUsed )
			{
				if ( ReadHeader.ReadFromStream(pStream) )	
					m_VarBy.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oAnimateBehaviorAtom.m_bFromPropertyUsed )
			{
				if ( ReadHeader.ReadFromStream(pStream) )	
					m_VarFrom.ReadFromStream ( ReadHeader, pStream );
			}

			if ( m_oAnimateBehaviorAtom.m_bToPropertyUsed )
			{
				if ( ReadHeader.ReadFromStream(pStream) )	
					m_VarTo.ReadFromStream ( ReadHeader, pStream );
			}

			if ( ReadHeader.ReadFromStream(pStream) )	
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
	struct TimeRotationBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD src						=	StreamUtils::ReadDWORD ( pStream );

			fByPropertyUsed					=	( 0x01 == ( 0x01 & ((BYTE)src) ) ); 
			fFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)src) ) );
			fToPropertyUsed					=	( 0x03 == ( 0x03 & ((BYTE)src) ) );
			fDirectionPropertyUsed			=	( 0x04 == ( 0x04 & ((BYTE)src) ) );

			fBy								=	StreamUtils::ReadFLOAT ( pStream );
			fFrom							=	StreamUtils::ReadFLOAT ( pStream );
			fTo								=	StreamUtils::ReadFLOAT ( pStream );

			rotationDirection				=	StreamUtils::ReadDWORD ( pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF138 && m_oHeader.RecLen == 0x00000014; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeRotationBehaviorAtom"), TRUE );


			oWriter.WriteNodeEnd ( _T("TimeRotationBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("fByPropertyUsed"),		(DWORD)fByPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fFromPropertyUsed"),		(DWORD)fFromPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fToPropertyUsed"),		(DWORD)fToPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fDirectionPropertyUsed"),(DWORD)fDirectionPropertyUsed, &oWriter );

            CDirectory::WriteValueToNode ( _T("fBy"),					Helpers::DoubleToString ( (double)fBy ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fFrom"),					Helpers::DoubleToString ( (double)fFrom ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fTo"),					Helpers::DoubleToString ( (double)fTo ), &oWriter );
			CDirectory::WriteValueToNode ( _T("rotationDirection"),		Helpers::IntToHexString ( rotationDirection ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeRotationBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

		BOOL	fByPropertyUsed;
		BOOL	fFromPropertyUsed;
		BOOL	fToPropertyUsed;
		BOOL	fDirectionPropertyUsed;
		float	fBy;
		float	fFrom;
		float	fTo;
		DWORD	rotationDirection;		//	0	-	rotate clockwise,	1	-	rotate counter clockwise
	};
	struct TimeRotationBehaviorContainer  : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			SRecordHeader ReadHeader;
			
			if ( ReadHeader.ReadFromStream(pStream) )	
				rotationBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream ( ReadHeader, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () {	return	m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF12F;	}
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeRotationBehaviorContainer"), TRUE );
			oWriter.WriteNodeEnd ( _T("TimeRotationBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( rotationBehaviorAtom.ToString () );
			oWriter.WriteString ( behavior.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeRotationBehaviorContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeRotationBehaviorAtom		rotationBehaviorAtom;
		TimeBehaviorContainer			behavior;
	};
}

namespace Animations
{	
	struct TimeScaleBehaviorAtom : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD src						=	StreamUtils::ReadDWORD ( pStream );

			fByPropertyUsed					=	( 0x01 == ( 0x01 & ((BYTE)src) ) ); 
			fFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)src) ) );
			fToPropertyUsed					=	( 0x03 == ( 0x03 & ((BYTE)src) ) );
			fZoomContentsUsed				=	( 0x04 == ( 0x04 & ((BYTE)src) ) );

			fXBy							=	StreamUtils::ReadFLOAT ( pStream );
			fYBy							=	StreamUtils::ReadFLOAT ( pStream );
			fXFrom							=	StreamUtils::ReadFLOAT ( pStream );
			fYFrom							=	StreamUtils::ReadFLOAT ( pStream );
			fXTo							=	StreamUtils::ReadFLOAT ( pStream );
			fYTo							=	StreamUtils::ReadFLOAT ( pStream );

			src								=	StreamUtils::ReadDWORD ( pStream );
			fZoomContents					=	( 0x01 == ( 0x01 & ((BYTE)src) ) ); 
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF139 && m_oHeader.RecLen == 0x00000020; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeScaleBehaviorAtom"), TRUE );


			oWriter.WriteNodeEnd ( _T("TimeScaleBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("fByPropertyUsed"),		(DWORD)fByPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fFromPropertyUsed"),		(DWORD)fFromPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fToPropertyUsed"),		(DWORD)fToPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fZoomContentsUsed"),		(DWORD)fZoomContentsUsed, &oWriter );

            CDirectory::WriteValueToNode ( _T("fXBy"),					Helpers::DoubleToString ( (double)fXBy ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fYBy"),					Helpers::DoubleToString ( (double)fYBy ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fXFrom"),				Helpers::DoubleToString ( (double)fXFrom ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fYFrom"),				Helpers::DoubleToString ( (double)fYFrom ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fXTo"),					Helpers::DoubleToString ( (double)fXTo ), &oWriter );
            CDirectory::WriteValueToNode ( _T("fYTo"),					Helpers::DoubleToString ( (double)fYTo ), &oWriter );
			CDirectory::WriteValueToNode ( _T("fZoomContents"),			(DWORD)fZoomContents, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeScaleBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

		BOOL	fByPropertyUsed;
		BOOL	fFromPropertyUsed;
		BOOL	fToPropertyUsed;
		BOOL	fZoomContentsUsed;
		float	fXBy;
		float	fYBy;
		float	fXFrom;
		float	fYFrom;
		float	fXTo;
		float	fYTo;
		BOOL	fZoomContents;
	};

	struct TimeScaleBehaviorContainer  : public IRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			SRecordHeader ReadHeader;
	
			if ( ReadHeader.ReadFromStream(pStream) )	
				scaleBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream ( ReadHeader, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () {	return	m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF130;	}
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeScaleBehaviorContainer"), TRUE );
			oWriter.WriteNodeEnd ( _T("TimeScaleBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( scaleBehaviorAtom.ToString () );
			oWriter.WriteString ( behavior.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeScaleBehaviorContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeScaleBehaviorAtom			scaleBehaviorAtom;
		TimeBehaviorContainer			behavior;
	};
}

namespace Animations
{
	struct TimeAnimateColorBy
	{
		DWORD model;			//	0	-	RGB,	1	-	HSL,	2	-	IndexScheme
		DWORD component0;
		DWORD component1;
		DWORD component2;

		inline unsigned long FRGB (BYTE alpha = 0xFF)	//	
		{
			return ((component2 <<  16) |	(component1 << 8) | (component0 <<   0) | (alpha << 24));
		}
	};

	struct TimeAnimateColor
	{
		DWORD model;			//	0	-	RGB,	2	-	IndexScheme
		DWORD red;
		DWORD green;
		DWORD blue;

		inline unsigned long FRGB (BYTE alpha = 0xFF)
		{
			return ((blue <<  16) |	(green << 8) | (red <<   0) | (alpha << 24));
		}
	};

	struct TimeColorBehaviorAtom: public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			flag					=	StreamUtils::ReadDWORD ( pStream );

			fByPropertyUsed			=	( 0x01 == ( 0x01 & ((BYTE)flag) ) ); 
			fFromPropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)flag) ) );
			fToPropertyUsed			=	( 0x03 == ( 0x03 & ((BYTE)flag) ) );
			fColorSpacePropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)flag) ) );
			fDirectionPropertyUsed	=	( 0x05 == ( 0x05 & ((BYTE)flag) ) );

			pStream->read ((unsigned char*) &colorBy	, sizeof ( TimeAnimateColorBy ) );
			pStream->read ((unsigned char*) &colorFrom	, sizeof ( TimeAnimateColor ) );
			pStream->read ((unsigned char*) &colorTo	, sizeof ( TimeAnimateColor ) );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeColorBehavior && m_oHeader.RecLen == 0x00000034; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeColorBehaviorAtom"), TRUE );
			oWriter.WriteNodeEnd ( _T("TimeColorBehaviorAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("fByPropertyUsed"),		 (DWORD)fByPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fFromPropertyUsed"),		 (DWORD)fFromPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fToPropertyUsed"),		 (DWORD)fToPropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fColorSpacePropertyUsed"),(DWORD)fColorSpacePropertyUsed, &oWriter );
			CDirectory::WriteValueToNode ( _T("fColorSpacePropertyUsed"),(DWORD)fDirectionPropertyUsed, &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeColorBehaviorAtom") );

			return oWriter.GetXmlString();
		}

	public:

		DWORD	flag;

		BOOL	fByPropertyUsed;
		BOOL	fFromPropertyUsed;
		BOOL	fToPropertyUsed;
		BOOL	fColorSpacePropertyUsed;
		BOOL	fDirectionPropertyUsed;

		TimeAnimateColorBy	colorBy;
		TimeAnimateColor	colorFrom;
		TimeAnimateColor	colorTo;
	};

	struct TimeColorBehaviorContainer : public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			SRecordHeader ReadHeader;
			if (ReadHeader.ReadFromStream(pStream) )	
				colorBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if (ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream (ReadHeader, pStream );
		}

#ifdef _DEBUG
		virtual bool IsCorrect () {	return	m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeColorBehaviorContainer;	}
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeColorBehaviorContainer"), TRUE );
			oWriter.WriteNodeEnd ( _T("TimeColorBehaviorContainer"), TRUE, FALSE );

			oWriter.WriteString ( colorBehaviorAtom.ToString () );
			oWriter.WriteString ( behavior.ToString () );

			oWriter.WriteNodeEnd ( _T("TimeColorBehaviorContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeColorBehaviorAtom	colorBehaviorAtom;
		TimeBehaviorContainer	behavior;
	};
}

namespace Animations
{	
	struct TimeModifierAtom : public IRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader	=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			type		=	StreamUtils::ReadDWORD(pStream);
			value		=	StreamUtils::ReadFLOAT(pStream);
		}

#ifdef _DEBUG
		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecType == RT_TimeModifierAtom && m_oHeader.RecLen == 0x00000008; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("TimeModifierAtom"), TRUE );


			oWriter.WriteNodeEnd ( _T("TimeModifierAtom"), TRUE, FALSE );

			CDirectory::WriteValueToNode ( _T("type"),	type, &oWriter );
            CDirectory::WriteValueToNode ( _T("value"),	Helpers::DoubleToString ( (double)value ), &oWriter );

			oWriter.WriteNodeEnd ( _T("TimeModifierAtom") );

			return oWriter.GetXmlString();
		}

	public:

		unsigned long type;		//	0x00000000  Repeat count. 
		//	0x00000001  Repeat duration. 
		//	0x00000002  Speed. 
		//	0x00000003  Accelerate. 
		//	0x00000004  Decelerate. 
		float value;
	};

	struct SlaveContainer : public IRecord
	{
	public:

		SlaveContainer ()
		{
			timePropertyList		=	NULL;
			timeColorBehavior		=	NULL;
			timeSetBehavior			=	NULL;
			//timeCommandBehavior	=	NULL;
			clientVisualElement		=	NULL;
			rgBeginTimeCondition	=	NULL;
			extTimeContainer		=	NULL;

			haveTimePropertyList	=	FALSE;
			haveColorBehavior		=	FALSE;
			haveSetBehavior			=	FALSE;
			haveTimeContainer		=	FALSE;
			haveClientVisualElement	=	FALSE;
			haveExtTimeContainer	=	FALSE;
		}

		virtual ~SlaveContainer ()
		{
			RELEASEOBJECT(timePropertyList);				
			RELEASEOBJECT(timeColorBehavior);
			RELEASEOBJECT(timeSetBehavior);
			//RELEASEOBJECT(timeCommandBehavior);
			RELEASEOBJECT(clientVisualElement);
			RELEASEOBJECT(rgBeginTimeCondition);
			RELEASEOBJECT(extTimeContainer);
		}

		virtual void ReadFromStream (SRecordHeader & oHeader, POLE::Stream* pStream)
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			SRecordHeader	rHeader;

			if ( rHeader.ReadFromStream(pStream) )	
				timeNodeAtom.ReadFromStream ( rHeader, pStream );

			UINT lCurLen	=	0;
			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( ReadHeader.ReadFromStream(pStream) == FALSE)	
					break;

				lCurLen			+=	8 + ReadHeader.RecLen;

				WORD nRecord	=	ReadHeader.RecType;

				if ( RT_TimePropertyList	== nRecord )
				{
					haveTimePropertyList			=	TRUE;
					timePropertyList				=	new TimePropertyList4TimeNodeContainer ();
					timePropertyList->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}

				if ( RT_TimeSetBehaviorContainer == nRecord )
				{
					haveSetBehavior					=	TRUE;
					timeSetBehavior					=	new TimeSetBehaviorContainer();
					timeSetBehavior->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}				

				if ( RT_TimeColorBehaviorContainer == nRecord )
				{
					haveColorBehavior				=	TRUE;
					timeColorBehavior				=	new TimeColorBehaviorContainer();
					timeColorBehavior->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}				

				if ( RT_TimeClientVisualElement == nRecord )
				{
					haveClientVisualElement			=	TRUE;
					clientVisualElement				=	new ClientVisualElementContainer ();
					clientVisualElement->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}

				if (RT_TimeExtTimeNodeContainer == nRecord)
				{
					extTimeContainer				=	CAnimationFactory::BuildAnimationObject (nRecord); // ExtTimeNodeContainer
					if (extTimeContainer)
					{
						haveExtTimeContainer		=	TRUE;
						extTimeContainer->ReadFromStream  ( ReadHeader, pStream );
						continue;
					}
				}

#ifdef _DEBUG
				ATLTRACE ( _T("SlaveContainer : UNKNOW RECORD : 0x%x\n"), nRecord );
#endif
				StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
			}
		}

#ifdef _DEBUG
		virtual bool IsCorrect() { return m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x01 && m_oHeader.RecType == 0xF145; }
#endif
		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("SlaveContainer"), TRUE );
			oWriter.WriteNodeEnd ( _T("SlaveContainer"), TRUE, FALSE );

			oWriter.WriteString (timeNodeAtom.ToString());

			if (timePropertyList)
				oWriter.WriteString ( timePropertyList->ToString() );
			if (timeSetBehavior)
				oWriter.WriteString ( timeSetBehavior->ToString () );
			if (clientVisualElement)
				oWriter.WriteString ( clientVisualElement->ToString () );
			if (extTimeContainer)
				oWriter.WriteString (extTimeContainer->ToString ());

			oWriter.WriteNodeEnd ( _T("SlaveContainer") );

			return oWriter.GetXmlString();
		}

	public:

		TimeNodeAtom							timeNodeAtom;	 
		TimePropertyList4TimeNodeContainer*		timePropertyList;				
		TimeColorBehaviorContainer*				timeColorBehavior;
		TimeSetBehaviorContainer*				timeSetBehavior;
		//TimeCommandBehaviorContainer*			timeCommandBehavior;
		ClientVisualElementContainer*			clientVisualElement;
		TimeConditionContainer*					rgBeginTimeCondition;

		IRecord*								extTimeContainer;	//	в спецификации такого элемента не должно быть

		BOOL									haveTimePropertyList;
		BOOL									haveColorBehavior;
		BOOL									haveSetBehavior;
		BOOL									haveTimeContainer;
		BOOL									haveClientVisualElement;
		BOOL									haveExtTimeContainer;
	};

	struct ExtTimeNodeContainer	: public IRecord
	{
	public:

		ExtTimeNodeContainer()
		{
			haveTimePropertyList	=	FALSE;
			haveAnimateBehavior		=	FALSE;			
			haveSetBehavior			=	FALSE;
			haveEffectBehavior		=	FALSE;
			haveColorBehavior		=	FALSE;
			haveMotionBehavior		=	FALSE;
			haveRotationBehavior	=	FALSE;
			haveScaleBehavior		=	FALSE;
			haveSlaveContainer		=	FALSE;
			haveClientVisualElement	=	FALSE;
			haveSequenceAtom		=	FALSE;
			haveIterateDataAtom		=	FALSE;
			haveBuildList			=	FALSE;

			timePropertyList		=	NULL;
			timeAnimateBehavior		=	NULL;
			timeColorBehavior		=	NULL;
			timeEffectBehavior		=	NULL;
			timeMotionBehavior		=	NULL;
			timeRotationBehavior	=	NULL;
			timeScaleBehavior		=	NULL;
			timeSetBehavior			=	NULL;
			clientVisualElement		=	NULL;
			timeSequenceDataAtom	=	NULL;
			timeIterateDataAtom		=	NULL;
			rgSlave					=	NULL;
			buildList				=	NULL;
		}

		virtual ~ExtTimeNodeContainer()
		{
			RELEASEOBJECT(timePropertyList);		
			RELEASEOBJECT(timeAnimateBehavior);	
			RELEASEOBJECT(timeColorBehavior);		
			RELEASEOBJECT(timeEffectBehavior);	
			RELEASEOBJECT(timeMotionBehavior);	
			RELEASEOBJECT(timeRotationBehavior);	
			RELEASEOBJECT(timeScaleBehavior);		
			RELEASEOBJECT(timeSetBehavior);
			RELEASEOBJECT(clientVisualElement);
			RELEASEOBJECT(timeSequenceDataAtom);
			RELEASEOBJECT(timeIterateDataAtom);
			RELEASEOBJECT(buildList);

			RELEASEOBJECT(rgSlave);

			for ( long i = 0; i < (long)rgExtTimeNodeChildren.size(); ++i )
				RELEASEOBJECT ( rgExtTimeNodeChildren[i] );

			for ( long i = 0; i < (long)timeCondition.size(); ++i )
				RELEASEOBJECT ( timeCondition[i] );

			for ( long i = 0; i < (long)rgTimeModifierAtom.size(); ++i )
				RELEASEOBJECT ( rgTimeModifierAtom[i] );
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			CHECK_RECORD_3X (0xF,0x01,RT_TimeExtTimeNodeContainer);
#endif
			LONG lPos = 0;
			StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader	rHeader;

			if ( rHeader.ReadFromStream(pStream) )	
				timeNodeAtom.ReadFromStream ( rHeader, pStream );

			UINT lCurLen	=	0;

			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
				if ( ReadHeader.ReadFromStream(pStream) == FALSE)	
					break;

				lCurLen			+=	8 + ReadHeader.RecLen;

				//AnimationsClassType nRecord	=	(AnimationsClassType)ReadHeader.RecType;
				WORD nRecord	=	ReadHeader.RecType;

				if ( RT_TimeExtTimeNodeContainer	==	nRecord )
				{
					ExtTimeNodeContainer* pContainer	=	new ExtTimeNodeContainer ();
					if (pContainer)
					{
						pContainer->ReadFromStream  ( ReadHeader, pStream );
						rgExtTimeNodeChildren.push_back ( pContainer );

						continue;
					}
				}

				if ( RT_BuildList	==	nRecord )
				{
					buildList	=	new BuildListContainer ();
					if (buildList)
					{
						buildList->ReadFromStream  ( ReadHeader, pStream );
						haveBuildList				=	TRUE;

						continue;
					}
				}	

				if ( RT_TimeConditionContainer	==	nRecord )
				{
					IRecord* pRecord	=	CAnimationFactory::BuildAnimationObject ( ReadHeader.RecType );
					if (pRecord)
					{
						pRecord->ReadFromStream  ( ReadHeader, pStream );
						timeCondition.push_back ( pRecord );

						continue;
					}
				}

				if ( RT_TimeSequenceData == nRecord )
				{
					timeSequenceDataAtom		=	new TimeSequenceDataAtom ();
					if (timeSequenceDataAtom)
					{
						timeSequenceDataAtom->ReadFromStream  ( ReadHeader, pStream );
						haveSequenceAtom		=	TRUE;

						continue;
					}
				}

				if ( RT_TimeIterateData == nRecord )
				{
					timeIterateDataAtom		=	new TimeIterateDataAtom ();
					if (timeIterateDataAtom)
					{
						timeIterateDataAtom->ReadFromStream  ( ReadHeader, pStream );
						haveIterateDataAtom		=	TRUE;

						continue;
					}
				}

				if ( RT_TimePropertyList == nRecord )
				{
					timePropertyList			=	new TimePropertyList4TimeNodeContainer ();
					if (timePropertyList)
					{
						timePropertyList->ReadFromStream  ( ReadHeader, pStream );
						haveTimePropertyList	=	TRUE;

						continue;
					}
				}

				if ( RT_TimeEffectBehaviorContainer == nRecord )
				{
					timeEffectBehavior			=	new TimeEffectBehaviorContainer();
					if (timeEffectBehavior)
					{
						timeEffectBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveEffectBehavior		=	TRUE;
						continue;
					}
				}

				if ( RT_TimeColorBehaviorContainer == nRecord )
				{
					timeColorBehavior			=	new TimeColorBehaviorContainer();
					if (timeColorBehavior)
					{
						timeColorBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveColorBehavior		=	TRUE;

						continue;
					}
				}

				if ( RT_TimeMotionBehaviorContainer == nRecord )
				{
					timeMotionBehavior			=	new TimeMotionBehaviorContainer ();
					if (timeMotionBehavior)
					{
						timeMotionBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveMotionBehavior		=	TRUE;

						continue;
					}
				}

				if ( RT_TimeSetBehaviorContainer == nRecord )
				{
					timeSetBehavior				=	new TimeSetBehaviorContainer ();
					if (timeSetBehavior)
					{
						timeSetBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveSetBehavior			=	TRUE;

						continue;
					}
				}

				if ( RT_TimeAnimateBehaviorContainer == nRecord )
				{
					timeAnimateBehavior			=	new TimeAnimateBehaviorContainer ();
					if (timeAnimateBehavior)
					{
						timeAnimateBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveAnimateBehavior		=	TRUE;

						continue;
					}
				}

				if ( RT_TimeRotationBehaviorContainer == nRecord )
				{
					timeRotationBehavior		=	new TimeRotationBehaviorContainer ();
					if (timeRotationBehavior)
					{
						timeRotationBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveRotationBehavior	=	TRUE;

						continue;
					}
				}

				if ( RT_TimeScaleBehaviorContainer == nRecord )
				{
					timeScaleBehavior			=	new TimeScaleBehaviorContainer ();
					if (timeScaleBehavior)
					{
						timeScaleBehavior->ReadFromStream  ( ReadHeader, pStream );
						haveScaleBehavior		=	TRUE;

						continue;
					}
				}

				if (RT_TimeClientVisualElement == nRecord)
				{
					haveClientVisualElement			=	TRUE;
					clientVisualElement				=	new ClientVisualElementContainer ();
					clientVisualElement->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}

				if ( RT_TimeSlaveContainer == nRecord )
				{
					rgSlave						=	new SlaveContainer ();
					if (rgSlave)
					{
						rgSlave->ReadFromStream  ( ReadHeader, pStream );
						haveSlaveContainer		=	TRUE;

						continue;
					}
				}

				if (RT_TimeModifierAtom == nRecord)
				{
					TimeModifierAtom* atom	=	new TimeModifierAtom ();
					if (atom)
					{
						atom->ReadFromStream  ( ReadHeader, pStream );
						rgTimeModifierAtom.push_back (atom);
						continue;
					}
				}
#ifdef _DEBUG
				ATLTRACE ( _T("ExtTimeNodeContainer : UNKNOW RECORD : 0x%x\n"), nRecord );
#endif
				StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
			}

			//ATLTRACE ("ExtTimeNodeContainer : %d\n"), rgExtTimeNodeChildren.size() );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}


		virtual CString ToString()
		{	
			XmlUtils::CXmlWriter oWriter;

			oWriter.WriteNodeBegin ( _T("ExtTimeNodeContainer"), TRUE );
			oWriter.WriteNodeEnd ( _T("ExtTimeNodeContainer"), TRUE, FALSE );

			oWriter.WriteString ( timeNodeAtom.ToString() );

			if (timePropertyList)
				oWriter.WriteString ( timePropertyList->ToString() );
			if (timeAnimateBehavior)
				oWriter.WriteString ( timeAnimateBehavior->ToString () );
			if (timeColorBehavior)
				oWriter.WriteString ( timeColorBehavior->ToString () );
			if (timeEffectBehavior)
				oWriter.WriteString ( timeEffectBehavior->ToString () );
			if (timeMotionBehavior)
				oWriter.WriteString ( timeMotionBehavior->ToString() );
			if (timeRotationBehavior)
				oWriter.WriteString ( timeRotationBehavior->ToString() );
			if (timeScaleBehavior)
				oWriter.WriteString ( timeScaleBehavior->ToString() );

			if (clientVisualElement)
				oWriter.WriteString ( clientVisualElement->ToString() );

			if (timeSequenceDataAtom)
				oWriter.WriteString ( timeSequenceDataAtom->ToString() );
			if (timeIterateDataAtom)
				oWriter.WriteString ( timeIterateDataAtom->ToString() );

			if (timeSetBehavior)
				oWriter.WriteString ( timeSetBehavior->ToString () );

			for ( long i = 0; i < (long)timeCondition.size(); ++i )
				oWriter.WriteString ( timeCondition[i]->ToString() );

			for ( long i = 0; i < (long)rgTimeModifierAtom.size(); ++i )
				oWriter.WriteString ( rgTimeModifierAtom[i]->ToString() );

			if (rgSlave)
				oWriter.WriteString (rgSlave->ToString());
			if (buildList)
				oWriter.WriteString (buildList->ToString());

			for ( long i = 0; i < (long)rgExtTimeNodeChildren.size(); ++i )
				oWriter.WriteString ( rgExtTimeNodeChildren[i]->ToString() );

			oWriter.WriteNodeEnd ( _T("ExtTimeNodeContainer") );

			return oWriter.GetXmlString();
		}

		//
		inline TimeModifierAtom* GetModifier (long Type)
		{
			for ( long i = 0; i < (long)rgTimeModifierAtom.size(); ++i )
				if (rgTimeModifierAtom[i]->type == Type)
					return rgTimeModifierAtom[i];

			return NULL;
		}

		inline int GetNodeType ()
		{
			return timeNodeAtom.m_dwType;
		}

		inline unsigned long GetEffectNodeType ()
		{
			if (timePropertyList)
				return timePropertyList->GetEffectNodeType ();

			return 0;
		}

	public:

		/*-------------------------------------- record --------------------------------- */

		TimeNodeAtom							timeNodeAtom;	
		// 
		TimePropertyList4TimeNodeContainer*		timePropertyList;				//	OPTIONAL
		TimeAnimateBehaviorContainer*			timeAnimateBehavior;			//	OPTIONAL
		TimeColorBehaviorContainer*				timeColorBehavior;
		TimeEffectBehaviorContainer*			timeEffectBehavior;				//	OPTIONAL
		TimeMotionBehaviorContainer*			timeMotionBehavior;				//	OPTIONAL
		TimeRotationBehaviorContainer*			timeRotationBehavior;
		TimeScaleBehaviorContainer*				timeScaleBehavior;
		TimeSetBehaviorContainer*				timeSetBehavior;				//	OPTIONAL
		//	timeCommandBehavior
		//	clientVisualElement
		TimeIterateDataAtom*					timeIterateDataAtom;			//	OPTIONAL
		TimeSequenceDataAtom*					timeSequenceDataAtom;			//	OPTIONAL

		ClientVisualElementContainer*			clientVisualElement;			//	OPTIONAL	-	It MUST exist only if timeNodeAtom.type is TL_TNT_Media 

		std::vector <IRecord*>					timeCondition;
		//	rgBeginTimeCondition
		//	rgEndTimeCondition
		//	timeEndSyncTimeCondition
		std::vector<TimeModifierAtom*>			rgTimeModifierAtom;
		SlaveContainer*							rgSlave;						//	OPTIONAL
		std::vector <ExtTimeNodeContainer*>		rgExtTimeNodeChildren;
		BuildListContainer*						buildList;

		/*-------------------------------------- record --------------------------------- */

		BOOL									haveTimePropertyList;
		BOOL									haveSequenceAtom;
		BOOL									haveIterateDataAtom;

		BOOL									haveEffectBehavior;
		BOOL									haveMotionBehavior;
		BOOL									haveAnimateBehavior;
		BOOL									haveColorBehavior;
		BOOL									haveRotationBehavior;
		BOOL									haveScaleBehavior;
		BOOL									haveSetBehavior;
		BOOL									haveClientVisualElement;
		BOOL									haveSlaveContainer;

		BOOL									haveBuildList;
	};

	struct SlideTime10Atom : public IRecord
	{
		static const DWORD RT_SlideTime10Atom	=	0x2EEB;

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			pStream->read ((unsigned char*) &m_FileTime, sizeof ( FILETIME ) );

#if defined(_WIN32) || defined (_WIN64)
            FileTimeToSystemTime ( &m_FileTime, &m_SystemTime );
#else
            //todooo
#endif
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

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD Value = 0L;
			pStream->read ((unsigned char*) &Value, sizeof ( Value ));

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

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader	=	oHeader;

#ifdef _DEBUG
			assert ( IsCorrect () );
#endif
			DWORD Value; 
			pStream->read ( (unsigned char*) &Value, sizeof ( DWORD ) );
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

			oWriter.WriteNodeEnd ( _T("HashCode10Atom"), TRUE, FALSE );

			oWriter.WriteNodeEnd ( _T("HashCode10Atom") );

			return oWriter.GetXmlString();
		}
	};
}

namespace Animations
{
	typedef NSPresentationEditor::CAnimationSimple Effect;
	typedef std::vector <Effect> Effects;
	typedef std::map<DWORD,Effects*> EffectsMap;
	typedef std::vector <double> TimeArray;
}

namespace Animations
{	
	class CNodeTiming
	{
	public:

		static const int ClickEffectNode			=	1;
		static const int WithPreviousNode			=	2;
		static const int AfterPreviousNode			=	3;
		static const int MainSequenceNode			=	4;
		static const int InteractiveSequenceNode	=	5; 
		static const int ClickParallelNode			=	6; 
		static const int WithGroupNode				=	7; 
		static const int AfterGroupNode				=	8; 
		static const int TimingRootNode				=	9; 

		struct EffectTime
		{
			double	dTime;
			double	dDuration;
			int		nType;			// задает тип последовательности запуска эффект ( после, параллельно, сброс и т.д. )
		};

	public:

		CNodeTiming ()
		{
			m_dTimeLine			=	0.0;
			m_dTimeDelay		=	0.0;

			m_nDuration			=	0.0;
			m_nStageTime		=	0.0;
		}

		inline void Push ( double dTime, int nType )
		{
			m_oTop.nType		=	nType;
			m_oTop.dTime		=	0.0;
			m_oTop.dDuration	=	dTime;

			if (ClickEffectNode == m_oTop.nType || AfterPreviousNode == m_oTop.nType)
				m_nStageTime	=	m_nDuration;

			m_oTop.dTime		=	m_nStageTime + m_dTimeDelay;

			m_arrEffects.push_back ( m_oTop );

            m_nDuration			=	(std::max) ( m_nDuration, m_oTop.dTime + m_oTop.dDuration );

#ifdef _DEBUG_LOG_
			ATLTRACE ( _T("Push : %d, begin : %d, duration : %d\n"), nType, (int)m_oTop.dTime, (int)m_oTop.dDuration );
#endif
		}

		inline void Pop ()
		{
#ifdef _DEBUG_LOG_
			ATLTRACE ( _T("Pop \n") );	
#endif
			if ( (int)m_arrEffects.size() )
				m_arrEffects.pop_back ();						

			if ( (int)m_arrEffects.size() )
			{
				m_oTop				=	m_arrEffects.at ( m_arrEffects.size() - 1 );
			}
			else
			{
				m_oTop.dDuration	=	0.0;
				m_oTop.dTime		=	0.0;
			}
		}

		inline const EffectTime& GetTop () const
		{
			return m_oTop;
		}


		inline void SetTimeDelay ( double dTimeDelay )
		{
			m_dTimeDelay	=	dTimeDelay;
		}


		inline double GetTimeLine () const
		{
			return m_nDuration;
		}

	private:

		double					m_dTimeLine;
		double					m_dTimeDelay;
		std::vector <EffectTime>	m_arrEffects;

		EffectTime				m_oTop;

		double					m_nDuration;

		double					m_nStageTime;
	};

	class CParagraphBuilds
	{
	public:
		struct EffectBuild
		{
			EffectBuild (long nShapeID, long nBuild) : m_nShapeID(nShapeID), m_nBuildType(nBuild)
			{

			}

			long m_nShapeID;
			long m_nBuildType;		// 1	-	Paragraph levels 1 to n – 1 in the shape animate separately,	3	-	as one graphical object
		};

	public:

		CParagraphBuilds()
		{

		}

		BOOL Create (ExtTimeNodeContainer* pTimeNode)
		{
			return FindBuildList (pTimeNode);
		}

		inline long GetBuild (long nShapeID)
		{
			for ( long i = 0; i < (long)m_arrBuilds.size(); ++i )
			{
				if (m_arrBuilds[i].m_nShapeID == nShapeID)
					return m_arrBuilds[i].m_nBuildType;
			}

			return -1;
		}

	private:

		inline BOOL FindBuildList ( ExtTimeNodeContainer* pContainer )
		{
			if ( pContainer->buildList )
			{
				for ( long i = 0; i < (long)pContainer->buildList->rgChildRec.size(); ++i )
					m_arrBuilds.push_back(EffectBuild(pContainer->buildList->rgChildRec[i]->buildAtom.shapeIdRef,	pContainer->buildList->rgChildRec[i]->paraBuildAtom.buildLevel) );

				return TRUE;
			}

			for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if (FindBuildList ( pContainer->rgExtTimeNodeChildren [i]) )
					return TRUE;
			}

			return TRUE;
		}


	private:

		std::vector<EffectBuild>	m_arrBuilds;
	};

	class CSlideTimeLine
	{
	public:

		CSlideTimeLine ()
		{
			m_dEffectDuration		=	0.0;

			m_nMediaID				=	-1;
			m_nMediaShapeID			=	-1;
			m_nMediaPush			=	0;
		}

		~CSlideTimeLine()
		{
			Clear ();
		}

		inline BOOL				Build (ExtTimeNodeContainer* pContainer)
		{
			if (pContainer)
			{	
				Clear ();

#ifdef _DEBUG_LOG
				ATLTRACE(_T("======================================== Slide ========================================\n"));
#endif
				m_bSaveEffect		=	FALSE;
				m_HaveAfterEffect	=	FALSE;

				m_oParagraphBuilds.Create(pContainer);

				ExploreTree ( pContainer );

				UpdateParagraph ();

#ifdef _DEBUG_LOG
				ATLTRACE(_T("duration : %f,\n"), GetTime () );
				ATLTRACE(_T("=======================================================================================\n"));
#endif
				return (0 != m_oAnimation.size());
			}

			return FALSE;
		}

		inline EffectsMap&		GetAnimation ()
		{
			return m_oAnimation;
		}

		inline double			GetTime ()
		{
#define MIN_SLIDE_TIME	5000.0

			double dTime	=	m_oNodeTiming.GetTimeLine ();
			if (dTime < MIN_SLIDE_TIME)
				return MIN_SLIDE_TIME;

			if (((long)dTime) % 1000)
				return ( ((long)((long)(dTime) / 1000) + 1 ) * 1000.0 );

			return dTime;
		}

	private:
		inline void	ExploreTree (ExtTimeNodeContainer* pContainer)
		{	
			unsigned long nNodeType				=	pContainer->GetEffectNodeType ();
			if (nNodeType)
			{
				if (CNodeTiming::MainSequenceNode != nNodeType && CNodeTiming::TimingRootNode != nNodeType)
				{
					m_oTopEffect				=	CreateEffectFromNode ( pContainer->timePropertyList );

					m_ComposeEffectMothionPath	=	_T("");

					FindEffectMothionPath ( pContainer );

					// время старта анимации
					if ( pContainer->timeCondition.size() )
					{
						double dTime	=	static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay;
						m_oNodeTiming.SetTimeDelay (dTime);	

						//m_oTopEffect.m_nBeginTime	=	static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay;

						m_bSaveEffect	=	FALSE;
					}

					// setup accelerate - decelerate

					if (pContainer->rgTimeModifierAtom.size())
					{
						if (pContainer->GetModifier(3))
                            m_oTopEffect.m_dTimeAccel			=	(std::max)((std::min)(pContainer->GetModifier(3)->value,1.0f),0.0f);

						if (pContainer->GetModifier(4))
                            m_oTopEffect.m_dTimeDecel			=	(std::max)((std::min)(pContainer->GetModifier(4)->value,1.0f),0.0f);
					}

					if (pContainer->haveSlaveContainer)
					{
						SlaveContainer* pSlave	= pContainer->rgSlave;
						if(pSlave->haveSetBehavior)				// после анимации к объекту может быть применена дополнительная анимация 
						{
							m_HaveAfterEffect					=	TRUE;

							// эффект исчезновения элемента (Appear) 
							m_oAfterEffect.m_nDuration			=	1.0;
							m_oAfterEffect.m_nEffectID			=	1;									
							m_oAfterEffect.m_nEffectNodeType	=	1;
							m_oAfterEffect.m_nEffectType		=	2;

							if (pSlave->timeSetBehavior)
							{
								if (pSlave->timeSetBehavior->behavior.clientVisualElement.m_bVisualShapeAtom)	
									m_oAfterEffect.m_nRefID		=	pSlave->timeSetBehavior->behavior.GetObjectID ();
							}

							if (pSlave->extTimeContainer)
							{
								ExploreTree (static_cast<ExtTimeNodeContainer*>(pSlave->extTimeContainer));
							}
						}
					}

					ProcessMediaCall (pContainer);
				}
			}

			int nID					=	GetShapeID ( pContainer );
			if ( -1 != nID )
			{
				//	TODO : 
				//	нужно из всех объектов (*Behavior) для конкретного эффекта,
				//	который заранее прочитан в m_oTopEffect дополнять параметры из потомков

				if ( FALSE == m_bSaveEffect )
				{
					AddAnimation ( pContainer, nID );

					m_bSaveEffect	=	TRUE;
				}

#ifdef _DEBUG_LOG
				// ATLTRACE ( _T("idObj : %d, effect : %s\t\t"), nID, Helpers::GetEffectNameByID ( m_oTopEffect.m_nEffectType, m_oTopEffect.m_nEffectID ) );				
				// ATLTRACE ( _T("dbg - NodeType : %d, count : %d\n"), nNodeType, (int)pNode->rgExtTimeNodeChildren.size() );
#endif
			}				

			for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if ( (WORD)RT_TimeExtTimeNodeContainer	==	pContainer->rgExtTimeNodeChildren [i]->m_oHeader.RecType )
				{
					Animations::ExtTimeNodeContainer* pChild = static_cast<Animations::ExtTimeNodeContainer*> ( pContainer->rgExtTimeNodeChildren [i] );
					if ( pChild )
						ExploreTree ( pChild );
				}
			}
		}

		//
		inline void	AddAnimation (ExtTimeNodeContainer* pTimeNode, long nID)
		{
			CalculateTimeEffect (pTimeNode);

			Effect oEffect						=	m_oTopEffect;

			oEffect.m_nRefID					=	nID;
			oEffect.m_nBeginTime				=	m_oNodeTiming.GetTop ().dTime;
			oEffect.m_nDuration					=	m_oNodeTiming.GetTop ().dDuration;

			if (pTimeNode->timeMotionBehavior)
			{
				oEffect.m_MotionPath			=	pTimeNode->timeMotionBehavior->m_VarPath.stringValue;
			}

			if (pTimeNode->haveScaleBehavior)		//	GrowAndShrinkEffect	=	6	//	простое увеличение	
			{
				oEffect.m_dSX					=	pTimeNode->timeScaleBehavior->scaleBehaviorAtom.fXBy * 0.01;
				oEffect.m_dSY					=	pTimeNode->timeScaleBehavior->scaleBehaviorAtom.fYBy * 0.01;
			}

			if (pTimeNode->haveRotationBehavior)	//	SpinEffect			=	8	//	вращение
			{
				oEffect.m_dRotateAngle			=	pTimeNode->timeRotationBehavior->rotationBehaviorAtom.fBy;
				//oEffect.m_nRotateDirection	=	pTimeNode->timeRotationBehavior->rotationBehaviorAtom.rotationDirection;
			}

			if (pTimeNode->haveSetBehavior)
			{
				if (9 == oEffect.m_nEffectID)		//	TransparencyEffect	=	9,	// временная прозрачность
					oEffect.m_dTransparency		=	_tstof (pTimeNode->timeSetBehavior->varTo.stringValue);
			}

			if (m_ComposeEffectMothionPath.GetLength())
			{
				oEffect.m_MotionPath			=	m_ComposeEffectMothionPath;		//	составной эффект может иметь траекторию для движения
			}

			if(pTimeNode->haveColorBehavior)
			{
				oEffect.m_nSchemeColor			=	pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.model;

				if (0 == pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.model)
				{
					oEffect.m_nColorTo			=	pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.FRGB();
				}

				// TODO : HSL
				// TODO : ColorDirection

				if (2 == pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.model)
				{
					oEffect.m_nColorTo			=	pTimeNode->timeColorBehavior->colorBehaviorAtom.colorTo.red;	//	index from table
				}
			}

			if (MediaCallEffect == oEffect.m_nEffectType)
			{
				if (1 == oEffect.m_nEffectID)	//	PLAY
					oEffect.m_nMediaCMD				=	1;
				
				if (2 == oEffect.m_nEffectID)	//	PAUSE
					oEffect.m_nMediaCMD				=	2;
				
				if (3 == oEffect.m_nEffectID)	//	STOP
					oEffect.m_nMediaCMD				=	0;				
			}

			// oEffect.m_nTextSequence			=	m_oParagraphBuilds.GetBuild(nID);

			AddEffectTopMap (oEffect, nID);
			AddAfterEffect	(nID);

			m_oTopEffect.m_dTimeAccel		=	0.0;
			m_oTopEffect.m_dTimeDecel		=	0.0;
			m_oTopEffect.m_bIgnoreShape		=	FALSE;
		}

		inline void	AddAfterEffect (long nID)
		{
			if (m_HaveAfterEffect)
			{
				if (-1 == m_oAfterEffect.m_nRefID)
				{
					m_oAfterEffect.m_nRefID	=	nID;
				}

				m_oAfterEffect.m_nDuration	=	1.0;
				m_oAfterEffect.m_nBeginTime	=	m_oNodeTiming.GetTop ().dTime + m_oNodeTiming.GetTop ().dDuration;

				AddEffectTopMap (m_oAfterEffect,nID);

				m_oAfterEffect.m_nRefID		=	-1;
				m_HaveAfterEffect			=	FALSE;
			}
		}
		inline void	AddEffectTopMap (Effect& oEffect, long nID)
		{
			oEffect.m_bRemoveEmptyBlocks	=	TRUE;	//	ALWAYS

			EffectToMap (oEffect);
#ifdef _DEBUG
			//ATLTRACE ( _T("REF : %d, EffectType : %d, EffectID : %d, EffectDir : %d, Group : %d, Begin : %f, Dur : %f, ShapeIgnore : %d, TextBlock : %d\n"), 
			//	oEffect.m_nRefID, oEffect.m_nEffectType, oEffect.m_nEffectID, oEffect.m_nEffectDir, oEffect.m_nEffectNodeType, oEffect.m_nBeginTime, oEffect.m_nDuration, oEffect.m_bIgnoreShape, oEffect.m_nTextSequence);
			ATLTRACE (_T("REF : %d, [ %f, %f ]\n"), oEffect.m_nRefID, oEffect.m_nBeginTime, oEffect.m_nDuration);
#endif
		}
		inline void EffectToMap (const Effect& oEffect)
		{
			EffectsMap::const_iterator mSearch	=	m_oAnimation.find(oEffect.m_nRefID);
			Effects* pEffects = NULL;
			if (mSearch == m_oAnimation.end())
			{
				pEffects		=	new Effects ();
				m_oAnimation.insert(std::pair<DWORD,Effects*>( oEffect.m_nRefID, pEffects));	
			}else 
				pEffects = mSearch->second;

			if (pEffects)
				pEffects->push_back ( oEffect );
		}

		//
		inline Effect CreateEffectFromNode (TimePropertyList4TimeNodeContainer* pContainer)
		{
			Effect oEffect;

			if (pContainer)
			{
				oEffect.m_nEffectID			=	pContainer->m_EffectID.m_Value;
				oEffect.m_nEffectType		=	pContainer->m_EffectType.m_Value;
				oEffect.m_nEffectDir		=	pContainer->m_EffectDir.m_Value;
				oEffect.m_nEffectNodeType	=	pContainer->m_EffectNodeType.m_Value;
			}

			return oEffect;
		}

		inline int GetShapeID (ExtTimeNodeContainer* pContainer)
		{	
			if (TL_TNT_Behavior == pContainer->timeNodeAtom.m_dwType)
			{				
				if (pContainer->timeMotionBehavior)
				{
					if (pContainer->timeMotionBehavior->m_oBehavior.clientVisualElement.m_bVisualShapeAtom)
					{
						ReadPropertyIgnoreShape (pContainer->timeMotionBehavior->m_oBehavior);
						return pContainer->timeMotionBehavior->m_oBehavior.GetObjectID ();
					}
				}

				if (pContainer->timeEffectBehavior)
				{
					if (pContainer->timeEffectBehavior->m_oBehavior.clientVisualElement.m_bVisualShapeAtom)				
					{
						ReadPropertyIgnoreShape (pContainer->timeEffectBehavior->m_oBehavior);
						return pContainer->timeEffectBehavior->m_oBehavior.GetObjectID ();
					}
				}

				if (pContainer->timeAnimateBehavior)
				{
					if (pContainer->timeAnimateBehavior->m_oBehavior.clientVisualElement.m_bVisualShapeAtom)	
					{
						ReadPropertyIgnoreShape (pContainer->timeAnimateBehavior->m_oBehavior);
						return pContainer->timeAnimateBehavior->m_oBehavior.GetObjectID ();
					}
				}

				if (pContainer->timeSetBehavior)
				{
					if (pContainer->timeSetBehavior->behavior.clientVisualElement.m_bVisualShapeAtom)	
					{
						ReadPropertyIgnoreShape (pContainer->timeSetBehavior->behavior);
						return pContainer->timeSetBehavior->behavior.GetObjectID ();
					}
				}

				if (pContainer->timeScaleBehavior)
				{
					if ( pContainer->timeScaleBehavior->behavior.clientVisualElement.m_bVisualShapeAtom )
					{
						ReadPropertyIgnoreShape (pContainer->timeScaleBehavior->behavior);
						return pContainer->timeScaleBehavior->behavior.GetObjectID ();
					}
				}

				if (pContainer->timeRotationBehavior)
				{
					if ( pContainer->timeRotationBehavior->behavior.clientVisualElement.m_bVisualShapeAtom )
					{
						ReadPropertyIgnoreShape (pContainer->timeRotationBehavior->behavior);
						return pContainer->timeRotationBehavior->behavior.GetObjectID ();
					}
				}

				if (pContainer->timeColorBehavior)
				{
					if ( pContainer->timeColorBehavior->behavior.clientVisualElement.m_bVisualShapeAtom )
					{
						ReadPropertyIgnoreShape (pContainer->timeColorBehavior->behavior);
						return pContainer->timeColorBehavior->behavior.GetObjectID ();					
					}
				}
			}

			return -1;
		}


		inline void CalculateTimeEffect (ExtTimeNodeContainer* pContainer)
		{
			m_dEffectDuration			=	pContainer->timeNodeAtom.m_nDuration;

			MediumEffectDuration ( pContainer );		//	получаем для все временные интервалы данного эффекта
			MediumEffectTimeDelay ( pContainer );		//	получаем все внутренние метки для данного эффткта

			if (MediaCallEffect == m_oTopEffect.m_nEffectType)
			{
				if ((long)pContainer->rgExtTimeNodeChildren.size())
				{
					ExtTimeNodeContainer* pChild	=	pContainer->rgExtTimeNodeChildren[0];
					if (pChild)
					{
						if (pChild->timeNodeAtom.m_bDurationProperty)		
						{
							m_dEffectDuration			=	pChild->timeNodeAtom.m_nDuration;
						}
					}
				}
			}

			if ((int)m_arDurations.size() > 1)
			{
				m_dEffectDuration		=	m_arDurations[1];

				if (FadeExitType == m_oTopEffect.m_nEffectType)
					m_dEffectDuration	=	m_arDurations[0];
			}

			// TODO : для составных эффектов посчитаем время с помощью заглушек

			if (/*	BlindsEffect		*/( m_oTopEffect.m_nEffectID ==	3	||
				/*	BoxEffect			*/	m_oTopEffect.m_nEffectID ==	4	||
				/*	CheckBoardEffect	*/	m_oTopEffect.m_nEffectID ==	5	||
				/*	CircleEffect		*/	m_oTopEffect.m_nEffectID ==	6	||
				/*	DiamondEffect		*/	m_oTopEffect.m_nEffectID ==	8	||
				/*	DissolveEffect		*/	m_oTopEffect.m_nEffectID ==	9	||
				/*	PeekEffect			*/	m_oTopEffect.m_nEffectID ==	12	||
				/*	PlusEffect			*/	m_oTopEffect.m_nEffectID ==	13	||
				/*	RandomBarsEffect	*/	m_oTopEffect.m_nEffectID ==	14	||
				/*	SplitEffect			*/	m_oTopEffect.m_nEffectID ==	16	||
				/*	StripsEffect		*/	m_oTopEffect.m_nEffectID ==	18	||
				/*	WedgeEffect			*/	m_oTopEffect.m_nEffectID ==	20	||
				/*	WheelEffect			*/	m_oTopEffect.m_nEffectID ==	21	||
				/*	WipeEffect			*/	m_oTopEffect.m_nEffectID ==	22 ) && 2 == m_oTopEffect.m_nEffectType )
			{
				m_dEffectDuration	=	 m_arDurations[0];
			}

			if (43 == m_oTopEffect.m_nEffectID)			//	CenterRevolveEffect
			{
				if (5 == m_arDurations.size())
					m_dEffectDuration	=	m_arDurations[0]	+ m_arDurations[1] + m_arDurations[3];
			}

			if (45 == m_oTopEffect.m_nEffectID)			//	FadedSwivel
			{
			}

			if (55 == m_oTopEffect.m_nEffectID)			//	Expand
			{
			}

			if (52 == m_oTopEffect.m_nEffectID)			//	RiseUp
			{
			}

			if (30 == m_oTopEffect.m_nEffectID)			//	Float
			{
				if ((int)m_arDurations.size() > 5)
					m_dEffectDuration	=	 m_arDurations[1]	+ m_arDurations[5];
			}

			if (26 == m_oTopEffect.m_nEffectID)
			{
				if ((long)m_arDurations.size() && (long)m_arBeginTimes.size() )
					m_dEffectDuration	=	m_arDurations.at(m_arDurations.size()-1) + m_arBeginTimes.at(m_arBeginTimes.size()-1);
			}

			if ( 32 == m_oTopEffect.m_nEffectID && 3 == m_oTopEffect.m_nEffectType )		//	Teeter Effect
			{
				if ((int)m_arDurations.size())
					m_dEffectDuration	=	 m_arDurations[0] * 10.0;

			}

			if ( 26 == m_oTopEffect.m_nEffectID && 3 == m_oTopEffect.m_nEffectType )		//	FlashBulb Effect
			{
				if ((int)m_arDurations.size() >= 2)
					m_dEffectDuration	=	 m_arDurations[0];
			}

			//debug::tracearrf (m_arDurations); 
			//debug::tracearrf (m_arBeginTimes); 

			m_arDurations.clear();
			m_arBeginTimes.clear();

			m_oNodeTiming.Push ( m_dEffectDuration, m_oTopEffect.m_nEffectNodeType );
		}

		inline BOOL MediumEffectDuration (ExtTimeNodeContainer* pContainer)
		{
			if ( pContainer->timeNodeAtom.m_bDurationProperty )
			{
				m_arDurations.push_back ( pContainer->timeNodeAtom.m_nDuration );

				for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
				{
					if ( (WORD)RT_TimeExtTimeNodeContainer	==	pContainer->rgExtTimeNodeChildren [i]->m_oHeader.RecType )
						MediumEffectDuration ( pContainer->rgExtTimeNodeChildren [i] );
				}
			}

			return TRUE;
		}

		inline BOOL MediumEffectTimeDelay (ExtTimeNodeContainer* pContainer)
		{
			if ( pContainer->timeNodeAtom.m_bDurationProperty )
			{
				if ( pContainer->timeCondition.size() )
				{
					m_arBeginTimes.push_back( static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay );	
				}

				for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
				{
					if ( (WORD)RT_TimeExtTimeNodeContainer	==	pContainer->rgExtTimeNodeChildren [i]->m_oHeader.RecType )
						MediumEffectTimeDelay ( pContainer->rgExtTimeNodeChildren [i] );
				}
			}

			return TRUE;
		}
		inline BOOL FindEffectMothionPath (ExtTimeNodeContainer* pContainer)
		{
			// сложные эффект может содержать в себе перемещение по траектории
			if (52 == m_oTopEffect.m_nEffectID)			//	ArcUp
			{
				if ( pContainer->haveMotionBehavior )
				{
					m_ComposeEffectMothionPath	=	pContainer->timeMotionBehavior->m_VarPath.stringValue;
					// debug::trace (m_ComposeEffectMothionPath);
					return TRUE;
				}

				for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
					FindEffectMothionPath ( pContainer->rgExtTimeNodeChildren [i] );

				return TRUE;
			}

			return FALSE;
		}

		inline void ReadPropertyIgnoreShape (const TimeBehaviorContainer& container)
		{
			if (2 == container.clientVisualElement.m_oVisualShapeAtom.m_Type)										//	указывает на то что анимация применяется к тексту
			{
				m_oTopEffect.m_bIgnoreShape			=	TRUE;
				m_oTopEffect.m_nTextSequence		=	container.clientVisualElement.m_oVisualShapeAtom.m_nData1;	//	номер параграфа - не нормальзован
				m_oTopEffect.m_bRemoveEmptyBlocks	=	TRUE;

				for (long i = 0; i < (long)m_arParIndexer.size(); ++i)
				{
					if (m_oTopEffect.m_nTextSequence == m_arParIndexer.at(i))
						return;
				}

				m_arParIndexer.push_back (m_oTopEffect.m_nTextSequence);
			}
		}

		inline BOOL UpdateParagraph ()
		{
			if ((long)m_arParIndexer.size())
			{
				//ATLTRACE ( _T("====================PARAGRAPH==========================================\n"));
				//qsort ( m_arParIndexer.GetData(), m_arParIndexer.size(), sizeof (long), (int(*)(const void*, const void*))sort );

				sort(m_arParIndexer.begin(), m_arParIndexer.end());

				for (std::map<DWORD, Effects*>::iterator pPair = m_oAnimation.begin(); pPair != m_oAnimation.end(); ++pPair)
				{
					Effects* arEffects = pPair->second;
					if (arEffects)
					{
						for (long i = 0; i < (long)arEffects->size(); ++i)
						{
							Effect& oEffect = arEffects->at(i);
							if (oEffect.m_nTextSequence >= 0)
							{
								for (long ind = 0; ind < (long)m_arParIndexer.size(); ++ind)
								{
									if (oEffect.m_nTextSequence == m_arParIndexer.at(ind))
									{
										oEffect.m_nTextSequence = ind;
#ifdef _DEBUG
										//ATLTRACE ( _T("REF : %d, EffectType : %d, EffectID : %d, EffectDir : %d, Group : %d, Begin : %f, Dur : %f, ShapeIgnore : %d, TextBlock : %d\n"), 
										//	oEffect.m_nRefID, oEffect.m_nEffectType, oEffect.m_nEffectID, oEffect.m_nEffectDir, oEffect.m_nEffectNodeType, oEffect.m_nBeginTime, oEffect.m_nDuration, oEffect.m_bIgnoreShape, oEffect.m_nTextSequence);
#endif
										break;
									}
								}
							}
						}
					}
				}

				return TRUE;
			}

			return FALSE;
		}

		//
		inline void ProcessMediaCall (ExtTimeNodeContainer* pContainer)
		{			
			if (MediaCallEffect == m_oTopEffect.m_nEffectType)															//	если анимация применена к VIDEO или AUDIO элементу
			{
				m_nMediaPush		=	GetAttachedShapeToVideo (pContainer);											//	если к видео добавлена картинка, надо учитывать смещение при поиск ID

				if (GetMediaID (pContainer))
				{
					AddAnimation (pContainer, m_nMediaID);
					m_bSaveEffect	=	TRUE;
				}

				m_arrMedia.clear ();
			}
		}

		inline int GetMediaID (ExtTimeNodeContainer* pContainer)
		{
			m_nMediaID			=	-1;

			if (TL_TNT_Media == pContainer->GetNodeType ())	
			{
				if (pContainer->clientVisualElement)
				{
					m_nMediaID	=	pContainer->clientVisualElement->m_oVisualShapeAtom.m_nObjectIdRef;	

					if (0 == m_nMediaPush)
						return TRUE;

					--m_nMediaPush;
				}
			}

			for (long i = 0; i < (long)pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if (GetMediaID(pContainer->rgExtTimeNodeChildren [i]))
					return TRUE;
			}

			return FALSE;
		}

		inline BOOL GetAttachedShapeToVideo (ExtTimeNodeContainer* pContainer)
		{
			if (TL_TNT_Media == pContainer->GetNodeType ())																//	нод типа Media
			{
				if (pContainer->clientVisualElement)
				{
					m_arrMedia.push_back (pContainer->clientVisualElement->m_oVisualShapeAtom.m_nObjectIdRef);

					if (2 == (int)m_arrMedia.size())																//	у видео есть статическое изображение
						return TRUE;
				}
			}

			for (long i = 0; i < (long)pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if (CNodeTiming::AfterPreviousNode == pContainer->rgExtTimeNodeChildren [i]->GetEffectNodeType ())		//	нод с описанием эффекта
					return FALSE;

				if (GetAttachedShapeToVideo(pContainer->rgExtTimeNodeChildren [i]))
					return TRUE;
			}

			return FALSE;
		}

		//
		inline void Clear ()
		{
			for (std::map<DWORD,Effects*>::iterator pPair = m_oAnimation.begin(); pPair != m_oAnimation.end(); ++pPair)
			{
				RELEASEOBJECT(pPair->second);
			}

			m_oAnimation.clear();
		}


	private: 

		Effect				m_oAfterEffect;

		BOOL				m_bSaveEffect;
		Effect				m_oTopEffect;
		CNodeTiming			m_oNodeTiming;

		double				m_dEffectDuration;
		TimeArray			m_arDurations;
		TimeArray			m_arBeginTimes;

		CString				m_ComposeEffectMothionPath;
		BOOL				m_HaveAfterEffect;

		EffectsMap			m_oAnimation;
		CParagraphBuilds	m_oParagraphBuilds;

		std::vector <long>	m_arParIndexer;
		
		// media
		int					m_nMediaID;
		int					m_nMediaShapeID;
		int					m_nMediaPush;					//	глубина поиска

		std::vector <int>		m_arrMedia;
	};
}

struct PP10SlideBinaryTagExtension : public IRecord
{
public:

	PP10SlideBinaryTagExtension ()
	{
		extTimeNodeContainer	=	NULL;
		buildListContainer		=	NULL;
	}

	virtual ~PP10SlideBinaryTagExtension ()
	{
		RELEASEOBJECT (extTimeNodeContainer);
		RELEASEOBJECT (buildListContainer);
	}

	virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
	{
		m_oHeader			=	oHeader;

		LONG lPos			=	0;
		StreamUtils::StreamPosition ( lPos, pStream );

		UINT lCurLen		=	0;

		SRecordHeader ReadHeader;

		while ( lCurLen < m_oHeader.RecLen )
		{
			if ( ReadHeader.ReadFromStream(pStream) == FALSE)	
				break;

			lCurLen += 8 + ReadHeader.RecLen;

			if (Animations::RT_TimeExtTimeNodeContainer == ReadHeader.RecType)
			{
				extTimeNodeContainer	=	new Animations::ExtTimeNodeContainer ();
				if (extTimeNodeContainer)
				{
					extTimeNodeContainer->ReadFromStream  ( ReadHeader, pStream );
					continue;
				}
			}

			if (Animations::RT_BuildList == ReadHeader.RecType)
			{
				buildListContainer		=	new Animations::BuildListContainer ();
				if (buildListContainer)
				{
					buildListContainer->ReadFromStream  ( ReadHeader, pStream );
					continue;
				}
			}

			StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
		}

		StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
	}

	virtual CString ToString()
	{	
		XmlUtils::CXmlWriter oWriter;

		oWriter.WriteNodeBegin ( _T("PP10SlideBinaryTagExtension"), TRUE );


		oWriter.WriteNodeEnd ( _T("PP10SlideBinaryTagExtension"), TRUE, FALSE );

		// oWriter.WriteString ( m_oSlideTime10Atom.ToString () );
		// oWriter.WriteString ( m_oHashCode10Atom.ToString () );


		if (extTimeNodeContainer)
			oWriter.WriteString ( extTimeNodeContainer->ToString () );

		if (buildListContainer)
			oWriter.WriteString ( buildListContainer->ToString () );

		oWriter.WriteNodeEnd ( _T("PP10SlideBinaryTagExtension") );

		return oWriter.GetXmlString();
	}


public:

	//Animations::SlideTime10Atom			m_oSlideTime10Atom;
	//Animations::HashCode10Atom			m_oHashCode10Atom;

	Animations::ExtTimeNodeContainer*	extTimeNodeContainer;	//	ROOT
	Animations::BuildListContainer*		buildListContainer;		//	OPTIONAL
};

struct SlideProgTagsContainer : public IRecord
{
public:

	SlideProgTagsContainer ()
	{

	}


	virtual void ReadFromStream (SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader	=	oHeader;

		LONG lPos = 0;
		StreamUtils::StreamPosition ( lPos, pStream );

		m_oHeaderChild.ReadFromStream(pStream) ;

		if ( RECORD_PROG_BINARY_TAG == m_oHeaderChild.RecType )
		{
#ifdef __USE_ANIMATION__

			SRecordHeader rgSubRec;
			
			rgSubRec.ReadFromStream(pStream) ;	

			WCHAR Name[9];
			if (sizeof (wchar_t) == 4)
			{
				//todoooo
			}
			else
			{
			}
			pStream->read ((unsigned char*) Name, 16 );
			Name[8]	=	L'\0';

			tagName	=	CStringW ( Name );

			if ( CStringW ( L"___PPT10" ) == tagName )
			{
				SRecordHeader rhData;
				rhData.ReadFromStream(pStream) ;	

				if (rhData.RecType == 0x138B && rhData.RecVersion == 0x0 && rhData.RecInstance == 0x000)	//	RT_BinaryTagDataBlob - 0x138B
				{
					m_PP10SlideBinaryTagExtension.ReadFromStream (rhData, pStream);

#if defined(_DEBUG) && (defined(_WIN32) || defined (_WIN64))
					if (m_PP10SlideBinaryTagExtension.extTimeNodeContainer)
					{
						XmlUtils::CXmlWriter oWriter;
						oWriter.WriteString (m_PP10SlideBinaryTagExtension.extTimeNodeContainer->ToString());
						oWriter.SaveToFile(_T("C:\\ppt_animations.xml"));
					}
#endif
				}
			}
#endif
		}

		StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
	}

	virtual CString ToString()
	{	
		return CString (_T(""));
	}


	inline Animations::CSlideTimeLine* GetTimeLine ()
	{
		if (m_PP10SlideBinaryTagExtension.extTimeNodeContainer)
		{
			Animations::CSlideTimeLine* pTimeLine = new Animations::CSlideTimeLine ();
			if (pTimeLine)
			{
				if (pTimeLine->Build(m_PP10SlideBinaryTagExtension.extTimeNodeContainer))
					return pTimeLine;

				RELEASEOBJECT (pTimeLine);
			}
		}

		return NULL;
	}

public:

	CStringW					tagName;

	PP10SlideBinaryTagExtension	m_PP10SlideBinaryTagExtension;

	SRecordHeader				m_oHeaderChild;
};
