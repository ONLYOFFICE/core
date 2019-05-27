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

#include <vector>
#include <map>

#include "../../Reader/Records.h"
#include "../../../../ASCOfficePPTXFile/Editor/Drawing/Document.h"

#define MIN_SLIDE_TIME	5000.0

//inline int sort (const long* a, const long* b) { return *a > *b ? 1 : -1; }

#if !defined(_WIN32) && !defined (_WIN64)

    typedef struct _FILETIME {
        _UINT32 dwLowDateTime;
        _UINT32 dwHighDateTime;
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

	enum TimeNodeTypeEnum : _UINT32
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

	enum TriggerObjectEnum : _UINT32
	{
		TL_TOT_None					=	0x00000000,	//	None.  
		TL_TOT_VisualElement		=	0x00000001,	//	An animatable object. 
		TL_TOT_TimeNode				=	0x00000002,	//	A time node.  
		TL_TOT_RuntimeNodeRef		=	0x00000003	//	Runtime child time nodes. 
	};

	enum TimeVisualElementEnum : _UINT32 
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

	enum TimeAnimateBehaviorValueTypeEnum : _UINT32
	{
		TL_TABVT_String				=	0x00000000, //	Animate text content. 
		TL_TABVT_Number				=	0x00000001,	//	Animate a numeric property. 
		TL_TABVT_Color				=	0x00000002	//	Animate a color property. 
	};
	enum ElementTypeEnum : _UINT32
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
            wchar_t	TYPE;	// // M = move to // L = line to // C = curve to // Z = close loop // E = end
		};

	public:

        inline bool Create ( std::wstring MovePath )
		{
			m_Points.clear ();

            std::vector<std::wstring> arMovePath;
            boost::algorithm::split(arMovePath, MovePath, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

            for (size_t i = 0 ; i < arMovePath.size(); i++)
			{
				ActionPoint	aPoint;
                aPoint.TYPE		=	arMovePath[i++][0];

				if ( L'm' == aPoint.TYPE )	aPoint.TYPE =	MOVE_TO;
				if ( L'l' == aPoint.TYPE )	aPoint.TYPE =	LINE_TO;
				if ( L'c' == aPoint.TYPE )	aPoint.TYPE =	CURVE_TO;
				if ( L'z' == aPoint.TYPE )	aPoint.TYPE =	CLOSE_LOOP;	//	This action requires no points.
				if ( L'e' == aPoint.TYPE )	aPoint.TYPE =	END;		//	This action requires no points.

				if ( MOVE_TO == aPoint.TYPE || LINE_TO == aPoint.TYPE )
				{
                    aPoint.X[0]	=	_wtof (	arMovePath[i++].c_str() );
                    aPoint.Y[0]	=	_wtof (	arMovePath[i++].c_str() );
				}

				if ( CURVE_TO == aPoint.TYPE )
				{
                    aPoint.X[0]	=	_wtof (	arMovePath[i++].c_str() );
                    aPoint.Y[0]	=	_wtof (	arMovePath[i++].c_str() );

                    aPoint.X[1]	=	_wtof (	arMovePath[i++].c_str() );
                    aPoint.Y[1]	=	_wtof (	arMovePath[i++].c_str() );

                    aPoint.X[2]	=	_wtof (	arMovePath[i++].c_str() );
                    aPoint.Y[2]	=	_wtof (	arMovePath[i++].c_str() );
				}

				m_Points.push_back ( aPoint ); 
			}

			return ( m_Points.size() >= 2 );
		}

        inline std::wstring Recalculate ( double ScaleX, double ScaleY )
		{
            std::wstring	MovePath;

			for ( size_t i = 0; i < m_Points.size(); ++i )
			{
                std::wstring NextPoint;

                if ( MOVE_TO ==	m_Points[i].TYPE )      NextPoint = L"M";
                if ( LINE_TO ==	m_Points[i].TYPE )      NextPoint = L"L";
                if ( CURVE_TO   == m_Points[i].TYPE )   NextPoint = L"C";
                if ( CLOSE_LOOP == m_Points[i].TYPE )	NextPoint = L"Z";
                if ( END        == m_Points[i].TYPE )   NextPoint = L"E";

                if ( CURVE_TO   == m_Points[i].TYPE ||
                     MOVE_TO    ==	m_Points[i].TYPE ||
                     LINE_TO    ==	m_Points[i].TYPE)
                {
                    NextPoint += L" " + std::to_wstring(m_Points[i].X[0] * ScaleX) + L" " + std::to_wstring(m_Points[i].Y[0] * ScaleY);
                }

                if ( CURVE_TO == m_Points[i].TYPE )
                {
                    NextPoint += L" " + std::to_wstring(m_Points[i].X[1] * ScaleX) + L" " + std::to_wstring(m_Points[i].Y[1] * ScaleY);
                    NextPoint += L" " + std::to_wstring(m_Points[i].X[2] * ScaleX) + L" " + std::to_wstring(m_Points[i].Y[2] * ScaleY);
                }
                MovePath += NextPoint;


				if ( i != m_Points.size() - 1 ) 
                    MovePath += std::wstring ( L" ");
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
        static std::wstring GetTimePropertyID4TimeNode			( TimePropertyID4TimeNode Value );
        static std::wstring GetTimeVariantTypeEnum				( TimeVariantTypeEnum Value );
        static std::wstring GetTimeNodeTypeEnum					( TimeNodeTypeEnum Value );
        static std::wstring GetTriggerObjectEnum				( TriggerObjectEnum Value );
        static std::wstring GetTimeVisualElementEnum			( TimeVisualElementEnum Value );
        static std::wstring GetElementTypeEnum					( ElementTypeEnum Value );
        static std::wstring GetTimeAnimateBehaviorValueTypeEnum	( TimeAnimateBehaviorValueTypeEnum Value );
        static std::wstring GetAnimationClassName				( AnimationsClassType Value );

        static std::wstring GetEffectTypeOfGroup				( _UINT32 Value );
		//	the corresponding effect type is an entrance or an exit effect
        static std::wstring GetEffectEntranceOrExitNameByID		( _UINT32 EffectID );
		//	the corresponding effect type is an emphasis effect
        static std::wstring GetEffectEmphasisNameByID			( _UINT32 EffectID );
		//	the corresponding effect type is a motion path effect
        static std::wstring GetEffectMotionPathNameByID			( _UINT32 EffectID );

        static std::wstring GetEffectNameByID					( _UINT32 EffectType, _UINT32 EffectID );
	};
}

namespace Animations
{
	struct BuildAtom : public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			buildType		=	StreamUtils::ReadDWORD ( pStream );
			buildId			=	StreamUtils::ReadDWORD ( pStream );
			shapeIdRef		=	StreamUtils::ReadDWORD ( pStream );

			_UINT32 Value		=	StreamUtils::ReadDWORD ( pStream );

			fExpanded		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			fUIExpanded		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
		}

		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_BuildAtom && m_oHeader.RecLen == 0x00000010; }

	public:

		_UINT32	buildType;			//	1	-	Paragraph build type,	2	-	Chart build type,	3	-	Diagram build type
		_UINT32	buildId;
		_UINT32	shapeIdRef;

        bool	fExpanded;
        bool	fUIExpanded;
	};
	struct ParaBuildAtom : public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			paraBuild					=	StreamUtils::ReadDWORD ( pStream );
			buildLevel					=	StreamUtils::ReadDWORD ( pStream );

			_UINT32 Value					=	StreamUtils::ReadDWORD ( pStream );

			fAnimBackground				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			fReverse					=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			fUserSetAnimBackground		=	( 0x03 == ( 0x03 & ((BYTE)Value) ) ); 
			fAutomatic					=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );

			delayTime					=	StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect () 
		{ 
			return	m_oHeader.RecVersion == 0x1 && 
					m_oHeader.RecInstance == 0x0 && 
					m_oHeader.RecType == RT_ParaBuildAtom && 
					m_oHeader.RecLen == 0x00000010; 
		}

	public:

		_UINT32	paraBuild;			//	0	-	All paragraphs in the shape animate at the same time. 
		//	1	-	Paragraph levels 1 to n – 1 in the shape animate separately.
		//			All paragraph levels n or greater animate at the same time. 
		//	2	-	Applies a custom animation paragraph build type to the 
		//			paragraphs of the shape. 
		//	3	-	The shape and all paragraphs within the shape animate as one 
		//			graphical object. 
		_UINT32	buildLevel;

        bool	fAnimBackground;
        bool	fReverse;
        bool	fUserSetAnimBackground;
        bool	fAutomatic;

		_UINT32	delayTime;
	};	
	struct LevelInfoAtom : public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader	=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			level		=	StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_LevelInfoAtom && m_oHeader.RecLen == 0x00000004; }

		_UINT32	level;			
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

	struct ParaBuildContainer : public CUnknownRecord
	{
	public:

		ParaBuildContainer()
		{

		}

		virtual ~ParaBuildContainer()
		{
			for ( size_t i = 0; i < rgParaBuildLevel.size(); ++i )
				RELEASEOBJECT (rgParaBuildLevel[i]);
		}

		virtual void ReadFromStream ( SRecordHeader & thisHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	thisHeader;

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
                if (ReadHeader.ReadFromStream(pStream) == false)
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
	public:
		BuildAtom		buildAtom;
		ParaBuildAtom	paraBuildAtom;

		std::vector <ParaBuildLevel*>	rgParaBuildLevel;
	};

	struct BuildListContainer : public CUnknownRecord
	{
	public:

		BuildListContainer ()
		{

		}

		virtual ~BuildListContainer()
		{
			for ( size_t i = 0; i < rgChildRec.size(); ++i )
				RELEASEOBJECT (rgChildRec[i]);
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );

			//SRecordHeader header;
			//if ( SUCCEEDED ( pStream->read ( &header, sizeof ( SRecordHeader ), NULL ) ) )
			//{
			//	if (RT_ParaBuild == header.RecType)
			//	{
			//		rgChildRec = new ParaBuildContainer ();
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
                if (header.ReadFromStream(pStream) == false)
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
	public:
		std::vector <ParaBuildContainer*>	rgChildRec;
		//ParaBuildContainer*	rgChildRec;
	};
}

namespace Animations
{
	struct TimeVariant : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//

			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000002;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000002;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000005;
		}

	public:

		TimeVariantTypeEnum		m_Type;
		_UINT32					m_Value;
	};

	struct TimeVariantFloat : public TimeVariant
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	
			m_Value				=	StreamUtils::ReadFLOAT ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142 &&
				m_oHeader.RecLen			==	0x00000005;
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

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif

			m_Type				=	( TimeVariantTypeEnum )StreamUtils::ReadBYTE ( pStream );	//	MUST be TL_TVT_Bool	

			if ( WCHAR* pString = new WCHAR [ m_oHeader.RecLen / 2 ] )
			{
				POLE::uint64 res = pStream->read ((unsigned char*) pString, ( m_oHeader.RecLen / 2 ) * 2) ;
				
				if (res > 0)
				{		
					if (sizeof(wchar_t) == 4)
					{
						//todoooo
					}
					else
                        stringValue		=	std::wstring ( pString );
				}

				RELEASEARRAYOBJECTS ( pString ); 
			}
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecType			==	0xF142;
			//	&&	m_oHeader.RecLen % 2		==	0x00000001;
		}

	public:

		TimeVariantTypeEnum		m_Type;
        std::wstring				stringValue;
	};

	struct TimeStringListContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

			UINT lCurLen		=	0;
			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
                if (ReadHeader.ReadFromStream(pStream) == false)
					break;

				TimeVariantString Element;	
				Element.ReadFromStream  ( ReadHeader, pStream );
				lCurLen += 8 + ReadHeader.RecLen;


				m_Values.push_back ( Element );
			}
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecType			==	0xF13E &&
				m_oHeader.RecInstance		==	0x001;
		}

	public:

		std::vector <TimeVariantString>	m_Values;
	};
	// structures for ExtTimeNodeContainer
	struct TimeNodeAtom : public CUnknownRecord
	{
		static const _UINT32 RT_TimeSequenceData = 0xF141;

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			//	LONG lPos = 0;	StreamUtils::StreamPosition(lPos, pStream);

			StreamUtils::StreamSkip ( sizeof ( _UINT32 ), pStream );

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

			StreamUtils::StreamSkip ( sizeof ( _UINT32 ), pStream );
			StreamUtils::StreamSkip ( sizeof ( _UINT32 ),	pStream );

			// duration: 0xFFFFFFFF specifies that the duration of the time node is infinite, 
			// and that its actual duration is determined by the durations of its child nodes. 
			m_nDuration				=	StreamUtils::ReadLONG ( pStream );

			_UINT32 Value				=	StreamUtils::ReadDWORD ( pStream );

			m_bFillProperty			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bRestartProperty		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

			m_bGroupingTypeProperty	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
			m_bDurationProperty		=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

        virtual bool IsCorrect () { return m_oHeader.RecVersion ==	0x0	&& m_oHeader.RecInstance ==	0x0	&& m_oHeader.RecType == 0xF127 && m_oHeader.RecLen == 0x00000020; }

	public:

		_UINT32				m_dwRestart;
		TimeNodeTypeEnum	m_dwType;
		_UINT32				m_dwFill;
		long				m_nDuration;

		bool				m_bFillProperty;
		bool				m_bRestartProperty;
		bool				m_bGroupingTypeProperty;
		bool				m_bDurationProperty;
	};

	struct TimeSequenceDataAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			// LONG lPos = 0;		StreamUtils::StreamPosition ( lPos, pStream );

			m_nConcurrency					=	StreamUtils::ReadDWORD ( pStream );
			m_nNextAction					=	StreamUtils::ReadDWORD ( pStream );
			m_nPreviousAction				=	StreamUtils::ReadDWORD ( pStream );

			StreamUtils::StreamSkip ( sizeof ( _UINT32 ),	pStream );

			_UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bConcurrencyPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bNextActionPropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			m_bPreviousActionPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0	&&
				m_oHeader.RecType			==	0xF141 &&
				m_oHeader.RecLen			==	0x00000014;
		}

	public:

		_UINT32		m_nConcurrency;
		_UINT32		m_nNextAction;
		_UINT32		m_nPreviousAction;

		bool		m_bConcurrencyPropertyUsed;
		bool		m_bNextActionPropertyUsed;
		bool		m_bPreviousActionPropertyUsed;
	};
}

namespace Animations
{	
	struct TimeDisplayType : public TimeVariantInt
	{

	};

	struct TimeMasterRelType : public TimeVariantInt
	{

	};

	struct TimeSlaveType : public TimeVariantInt
	{

	};

	struct TimeEffectID : public TimeVariantInt
	{

	};

	struct TimeEffectDir  : public TimeVariantInt
	{

	};

	struct TimeEffectType : public TimeVariantInt
	{

	};

	struct TimeAfterEffect : public TimeVariantBool
	{

	};
	struct TimeSlideCount : public TimeVariantInt
	{

	};

	struct TimeNodeTimeFilter : public TimeVariantString
	{

	};
	struct TimeEventFilter : public TimeVariantString
	{

	};
	struct TimeHideWhenStopped : public TimeVariantBool
	{

	};

	struct TimeGroupID : public TimeVariantInt
	{

	};

	struct TimeEffectNodeType  : public TimeVariantInt
	{

	};

	struct TimePlaceholderNode : public TimeVariantBool
	{

	};

	struct TimeMediaVolume : public TimeVariantFloat
	{

	};

	struct TimeMediaMute : public TimeVariantBool
	{

	};
	struct TimeZoomToFullScreen : public TimeVariantBool
	{

	};


	struct TimePropertyList4TimeNodeContainer : public CUnknownRecord
	{
	public:
		//static const _UINT32 RT_TimePropertyList = 0xF13D;	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  

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

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			LONG lPos = 0;
			StreamUtils::StreamPosition ( lPos, pStream );

			UINT lCurLen		=	0;

			SRecordHeader ReadHeader;

			if ( 0 == m_oHeader.RecLen )
				m_nEmtyNode = true;

			while ( lCurLen < m_oHeader.RecLen )
			{
                if (ReadHeader.ReadFromStream(pStream) == false)
					break;

				TimeVariant* pRecord = NULL;

				TimePropertyID4TimeNode VariableType = ( TimePropertyID4TimeNode ) ReadHeader.RecInstance;

				switch ( VariableType )
				{
				case TL_TPID_Display:			pRecord = new TimeDisplayType ();		break;
				case TL_TPID_MasterPos:			pRecord = new TimeMasterRelType ();		break;
				case TL_TPID_SlaveType:			pRecord = new TimeSlaveType ();			break;
				case TL_TPID_EffectID:			pRecord = new TimeEffectID ();			break;
				case TL_TPID_EffectDir:			pRecord = new TimeEffectDir ();			break;
				case TL_TPID_EffectType:		pRecord = new TimeEffectType ();		break;
				case TL_TPID_AfterEffect:		pRecord = new TimeAfterEffect ();		break;
				case TL_TPID_SlideCount:		pRecord = new TimeSlideCount ();		break;
				case TL_TPID_TimeFilter:		pRecord = new TimeNodeTimeFilter ();	break;
				case TL_TPID_EventFilter:		pRecord = new TimeEventFilter ();		break;
				case TL_TPID_HideWhenStopped:	pRecord = new TimeHideWhenStopped ();	break;
				case TL_TPID_GroupID:			pRecord = new TimeGroupID ();			break;
				case TL_TPID_EffectNodeType:	pRecord = new TimeEffectNodeType ();	break;
				case TL_TPID_PlaceholderNode:	pRecord = new TimePlaceholderNode ();	break;
				case TL_TPID_MediaVolume:		pRecord = new TimeMediaVolume ();		break;
				case TL_TPID_MediaMute:			pRecord = new TimeMediaMute ();			break;
				case TL_TPID_ZoomToFullScreen:	pRecord = new TimeZoomToFullScreen ();	break;
				default :
					break;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13D;
		}

        inline bool IsEmpty ()
		{
			return m_nEmtyNode;
		}
		void ClearNodes ()
		{
			for ( size_t i = 0; i < m_arrElements.size(); ++i )
			{
				RELEASEOBJECT ( m_arrElements[i] );
			}
			m_arrElements.clear ();
		}

		// helper

		inline const _UINT32& GetEffectNodeType () const
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

	struct TimePropertyList4TimeBehavior : public CUnknownRecord
	{
	public:
		//static const _UINT32 RT_TimePropertyList = 0xF13D;	//	Specifies a TimePropertyList4TimeNodeContainer or TimePropertyList4TimeBehavior.  

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

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13D;
		}
		void ClearNodes ()
		{
		}


	public:
	};
}

namespace Animations
{
	struct VisualShapeAtom : public CUnknownRecord
	{
		// Привязка анимации через этот объект к ID объекту

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			m_Type				=	(TimeVisualElementEnum) StreamUtils::ReadDWORD ( pStream );
			m_RefType			=	(ElementTypeEnum) StreamUtils::ReadDWORD ( pStream );
			m_nObjectIdRef		=	StreamUtils::ReadDWORD ( pStream );
			m_nData1			=	StreamUtils::ReadDWORD ( pStream );
			m_nData2			=	StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2AFB &&
				m_oHeader.RecLen			==	0x00000014;
		}

	public:

		TimeVisualElementEnum	m_Type;	
		ElementTypeEnum			m_RefType;
		_UINT32					m_nObjectIdRef;		//	ShapeIdRef || SoundIdRef
		_UINT32					m_nData1;
		_UINT32					m_nData2;		
	};

	struct VisualPageAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			m_Type				=	(TimeVisualElementEnum) StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2B01 &&
				m_oHeader.RecLen			==	0x00000004;
		}

	public:

		TimeVisualElementEnum	m_Type;
	};

	struct ClientVisualElementContainer : public CUnknownRecord
	{

		ClientVisualElementContainer ()
		{
			m_bVisualPageAtom	=	false;
			m_bVisualShapeAtom	=	false;
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13C;
		}

	public:

		VisualPageAtom	m_oVisualPageAtom;
		VisualShapeAtom	m_oVisualShapeAtom;

		bool			m_bVisualPageAtom;
		bool			m_bVisualShapeAtom;
	};

	struct TimeBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			// LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			_UINT32 dwFlags					=	StreamUtils::ReadDWORD ( pStream );

			m_bAdditivePropertyUsed			=	( 0x01 == ( 0x01 & ((BYTE)dwFlags) ) );
			m_bAttributeNamesPropertyUsed	=	( 0x04 == ( 0x04 & ((BYTE)dwFlags) ) );

			m_nBehaviorAdditive				=	StreamUtils::ReadDWORD ( pStream );
			m_nBehaviorAccumulate			=	StreamUtils::ReadDWORD ( pStream );
			m_nBehaviorTransform			=	StreamUtils::ReadDWORD ( pStream );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF133 &&
				m_oHeader.RecLen			==	0x00000010;
		}

	public:

		bool	m_bAdditivePropertyUsed;
		bool	m_bAttributeNamesPropertyUsed;

		_UINT32	m_nBehaviorAdditive;
		_UINT32	m_nBehaviorAccumulate;
		_UINT32	m_nBehaviorTransform;
	};

	struct TimeBehaviorContainer : public CUnknownRecord
	{
		TimeBehaviorContainer ()
		{
            havePropertyList	=	false;
			propertyList		=	NULL;

            haveStringList		=	false;
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

            havePropertyList	=	false;
            haveStringList		=	false;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader ReadHeader;
			
			if ( ReadHeader.ReadFromStream(pStream) )
				behaviorAtom.ReadFromStream ( ReadHeader, pStream );

			SRecordHeader header;

			UINT lCurLen	=	0;

			while ( lCurLen < m_oHeader.RecLen )
			{
                if (header.ReadFromStream(pStream) == false)
					break;

				lCurLen			+=	8 + header.RecLen;

				WORD nRecord	=	header.RecType;

				if ( RT_TimePropertyList == header.RecType )
				{
					propertyList = new TimePropertyList4TimeBehavior();
					if (propertyList)
					{
						propertyList->ReadFromStream (header, pStream);
                        havePropertyList	=	true;

						continue;
					}
				}

				if ( RT_TimeVariantList == header.RecType )
				{
					stringList = new TimeStringListContainer();
					if (stringList)
					{
						stringList->ReadFromStream (header, pStream);
                        haveStringList	=	true;

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
            havePropertyList	=	true;
			propertyList.ReadFromStream ( ReadHeader, pStream );
			}

			if ( RT_TimeVariantList == ReadHeader.RecType )
			{
            haveStringList = true;
			stringList.ReadFromStream ( ReadHeader, pStream );
			}
			}

			if ( SUCCEEDED ( pStream->read ( &ReadHeader, sizeof ( SRecordHeader ), NULL ) ) )
			clientVisualElement.ReadFromStream ( ReadHeader, pStream );
			*/

			//StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF12A; }

 		inline _UINT32 GetObjectID ()
		{
			return clientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
		}

	public:

		TimeBehaviorAtom				behaviorAtom;

		TimePropertyList4TimeBehavior*	propertyList;
        bool							havePropertyList;

		TimeStringListContainer* 		stringList;
        bool							haveStringList;

		ClientVisualElementContainer	clientVisualElement;
	};

	struct TimeEffectBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			// LONG lPos = 0;	StreamUtils::StreamPosition ( lPos, pStream );

			_UINT32 dwFlags				=	StreamUtils::ReadDWORD ( pStream );

			m_bTransitionPropertyUsed	= ( 0x01 == ( 0x01 & ((BYTE)dwFlags) ) );
			m_bTypePropertyUsed			= ( 0x02 == ( 0x02 & ((BYTE)dwFlags) ) );
			m_bProgressPropertyUsed		= ( 0x04 == ( 0x04 & ((BYTE)dwFlags) ) );
			m_bRuntimeContextObsolete	= ( 0x08 == ( 0x08 & ((BYTE)dwFlags) ) );

			m_nEffectTransition			=	StreamUtils::ReadDWORD ( pStream );

			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF136 &&
				m_oHeader.RecLen			==	0x00000008;
		}

	public:
		bool	m_bTransitionPropertyUsed;
		bool	m_bTypePropertyUsed;
		bool	m_bProgressPropertyUsed;
		bool	m_bRuntimeContextObsolete;

		_UINT32	m_nEffectTransition;
	};

	struct TimeEffectBehaviorContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12D;
		}


	public:

		TimeEffectBehaviorAtom	effectBehaviorAtom;

		TimeVariantString		m_varType;
		TimeVariantFloat		m_varProgres;
		TimeVariantString		m_varRuntimeContext;
		TimeBehaviorContainer	m_oBehavior;

	};
	struct TimeConditionAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			m_TriggerObject		=	( TriggerObjectEnum )StreamUtils::ReadDWORD ( pStream );

			m_nTriggerEvent		=	StreamUtils::ReadDWORD ( pStream );
			m_nID				=	StreamUtils::ReadDWORD ( pStream );
			m_nTimeDelay		=	StreamUtils::ReadLONG ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF128 &&
				m_oHeader.RecLen			==	0x00000010;
		}
	public:

		TriggerObjectEnum	m_TriggerObject;
		_UINT32				m_nTriggerEvent;
		_UINT32				m_nID;
		LONG				m_nTimeDelay;
	};

	struct TimeConditionContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
            if( IsCorrect () == false ) return;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		>=	0x001 && m_oHeader.RecInstance <= 0x005 &&
				m_oHeader.RecType			==	0xF125;
		}
	public:

		TimeConditionAtom				m_oTimeConditionAtom;
		ClientVisualElementContainer	m_oVisualElement;
	};

	struct TimeIterateDataAtom: public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			iterateInterval						=	StreamUtils::ReadDWORD ( pStream );
			iterateType							=	StreamUtils::ReadDWORD ( pStream );
			iterateDirection					=	StreamUtils::ReadDWORD ( pStream );
			iterateIntervalType					=	StreamUtils::ReadDWORD ( pStream );

			_UINT32 Value							=	StreamUtils::ReadDWORD ( pStream );

			fIterateDirectionPropertyUsed		=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			fIterateTypePropertyUsed			=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			fIterateIntervalPropertyUsed		=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
			fIterateIntervalTypePropertyUsed	=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
		}

		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeIterateData && m_oHeader.RecLen == 0x00000014; }

  	public:

		unsigned long iterateInterval;
		unsigned long iterateType;
		unsigned long iterateDirection;
		unsigned long iterateIntervalType;

        bool fIterateDirectionPropertyUsed;
        bool fIterateTypePropertyUsed;
        bool fIterateIntervalPropertyUsed;
        bool fIterateIntervalTypePropertyUsed;
	};
}

namespace Animations
{
	struct TimeMotionBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;
#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			_UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF137 &&
				m_oHeader.RecLen			==	0x00000020;
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
		_UINT32	m_nBehaviorOrigin;
	};

	struct TimeMotionBehaviorContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12E;
		}

	public:

		TimeMotionBehaviorAtom		m_oMotionBehaviorAtom;
		TimeVariantString			m_VarPath;
		TimeBehaviorContainer		m_oBehavior;
	};
}

namespace Animations
{
	struct TimeSetBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			_UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bToPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bValueTypePropertyUsed		=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );

			m_ValueType						=	(TimeAnimateBehaviorValueTypeEnum)StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13A &&
				m_oHeader.RecLen			==	0x00000008;
		}

	public:

		bool								m_bToPropertyUsed;
		bool								m_bValueTypePropertyUsed;

		TimeAnimateBehaviorValueTypeEnum	m_ValueType;
	};

	struct TimeSetBehaviorContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
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


		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0xF	&& m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeSetBehaviorContainer; }

 	public:

		TimeSetBehaviorAtom		setBehaviorAtom;
		TimeVariantString		varTo;
		TimeBehaviorContainer	behavior;
	};
}

namespace Animations
{
	struct TimeAnimateBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			m_nCalcMode						=	StreamUtils::ReadDWORD ( pStream );

			_UINT32 Value						=	StreamUtils::ReadDWORD ( pStream );

			m_bByPropertyUsed				=	( 0x01 == ( 0x01 & ((BYTE)Value) ) ); 
			m_bFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
			m_bToPropertyUsed				=	( 0x04 == ( 0x04 & ((BYTE)Value) ) );
			m_bCalcModePropertyUsed			=	( 0x08 == ( 0x08 & ((BYTE)Value) ) );
			m_bAnimationValuesPropertyUsed	=	( 0x10 == ( 0x10 & ((BYTE)Value) ) );
			m_bValueTypePropertyUsed		=	( 0x20 == ( 0x20 & ((BYTE)Value) ) );

			m_ValueType						=	(TimeAnimateBehaviorValueTypeEnum)StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF134 &&
				m_oHeader.RecLen			==	0x0000000C;
		}

	public:

		_UINT32								m_nCalcMode;

		bool								m_bByPropertyUsed;
		bool								m_bFromPropertyUsed;
		bool								m_bToPropertyUsed;
		bool								m_bCalcModePropertyUsed;
		bool								m_bAnimationValuesPropertyUsed;
		bool								m_bValueTypePropertyUsed;

		TimeAnimateBehaviorValueTypeEnum	m_ValueType;
	};

	struct TimeAnimationValueAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			m_nTime				=	StreamUtils::ReadDWORD ( pStream );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF143 &&
				m_oHeader.RecLen			==	0x00000004;
		}

	public:

		long	m_nTime;

	};

	struct TimeAnimationEntry 
	{
		virtual void ReadFromStream ( UINT& CurLen, SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oTimeAnimationValueAtom.ReadFromStream ( oHeader, pStream );

			UINT res = 0;
			SRecordHeader ReadHeader;

			if ( ReadHeader.ReadFromStream(pStream) )	
			{
				m_VarValue.ReadFromStream ( ReadHeader, pStream );
			}

			CurLen += 8 + ReadHeader.RecLen;

			if ( ReadHeader.ReadFromStream(pStream) )	
			{
				m_VarFormula.ReadFromStream ( ReadHeader, pStream );
			}

			CurLen += 8 + ReadHeader.RecLen;
		}		

		TimeAnimationValueAtom	m_oTimeAnimationValueAtom;
		TimeVariantString		m_VarValue;
		TimeVariantString		m_VarFormula;
	};

	struct TimeAnimationValueListContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif		
			LONG lPos		=	0;
			StreamUtils::StreamPosition ( lPos, pStream );

			UINT lCurLen	=	0;

			SRecordHeader ReadHeader;
			while ( lCurLen < m_oHeader.RecLen )
			{
                if ( ReadHeader.ReadFromStream(pStream) == false )
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

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF13F;
		}

	public:

		std::vector<TimeAnimationEntry>	m_arrEntry;
	};

	struct TimeAnimateBehaviorContainer : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
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


		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0xF	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0xF12B;
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
	struct TimeRotationBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			_UINT32 src						=	StreamUtils::ReadDWORD ( pStream );

			fByPropertyUsed					=	( 0x01 == ( 0x01 & ((BYTE)src) ) ); 
			fFromPropertyUsed				=	( 0x02 == ( 0x02 & ((BYTE)src) ) );
			fToPropertyUsed					=	( 0x03 == ( 0x03 & ((BYTE)src) ) );
			fDirectionPropertyUsed			=	( 0x04 == ( 0x04 & ((BYTE)src) ) );

			fBy								=	StreamUtils::ReadFLOAT ( pStream );
			fFrom							=	StreamUtils::ReadFLOAT ( pStream );
			fTo								=	StreamUtils::ReadFLOAT ( pStream );

			rotationDirection				=	StreamUtils::ReadDWORD ( pStream );
		}


		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF138 && m_oHeader.RecLen == 0x00000014; }

	public:

        bool	fByPropertyUsed;
        bool	fFromPropertyUsed;
        bool	fToPropertyUsed;
        bool	fDirectionPropertyUsed;
		float	fBy;
		float	fFrom;
		float	fTo;
		_UINT32	rotationDirection;		//	0	-	rotate clockwise,	1	-	rotate counter clockwise
	};
	struct TimeRotationBehaviorContainer  : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			SRecordHeader ReadHeader;
			
			if ( ReadHeader.ReadFromStream(pStream) )	
				rotationBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream ( ReadHeader, pStream );
		}

        virtual bool IsCorrect () {	return	m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF12F;	}


	public:

		TimeRotationBehaviorAtom		rotationBehaviorAtom;
		TimeBehaviorContainer			behavior;
	};
}

namespace Animations
{	
	struct TimeScaleBehaviorAtom : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			_UINT32 src						=	StreamUtils::ReadDWORD ( pStream );

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


		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF139 && m_oHeader.RecLen == 0x00000020; }

	public:

        bool	fByPropertyUsed;
        bool	fFromPropertyUsed;
        bool	fToPropertyUsed;
        bool	fZoomContentsUsed;
		float	fXBy;
		float	fYBy;
		float	fXFrom;
		float	fYFrom;
		float	fXTo;
		float	fYTo;
        bool	fZoomContents;
	};

	struct TimeScaleBehaviorContainer  : public CUnknownRecord
	{
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			SRecordHeader ReadHeader;
	
			if ( ReadHeader.ReadFromStream(pStream) )	
				scaleBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if ( ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream ( ReadHeader, pStream );
		}


		virtual bool IsCorrect () {	return	m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == 0xF130;	}

	public:

		TimeScaleBehaviorAtom			scaleBehaviorAtom;
		TimeBehaviorContainer			behavior;
	};
}

namespace Animations
{
	struct TimeAnimateColorBy
	{
		_UINT32 model;			//	0	-	RGB,	1	-	HSL,	2	-	IndexScheme
		_UINT32 component0;
		_UINT32 component1;
		_UINT32 component2;

		inline unsigned long FRGB (BYTE alpha = 0xFF)	//	
		{
			return ((component2 <<  16) |	(component1 << 8) | (component0 <<   0) | (alpha << 24));
		}
	};

	struct TimeAnimateColor
	{
		_UINT32 model;			//	0	-	RGB,	2	-	IndexScheme
		_UINT32 red;
		_UINT32 green;
		_UINT32 blue;

		inline unsigned long FRGB (BYTE alpha = 0xFF)
		{
			return ((blue <<  16) |	(green << 8) | (red <<   0) | (alpha << 24));
		}
	};

	struct TimeColorBehaviorAtom: public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
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


		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeColorBehavior && m_oHeader.RecLen == 0x00000034; }

	public:

		_UINT32	flag;

        bool	fByPropertyUsed;
        bool	fFromPropertyUsed;
        bool	fToPropertyUsed;
        bool	fColorSpacePropertyUsed;
        bool	fDirectionPropertyUsed;

		TimeAnimateColorBy	colorBy;
		TimeAnimateColor	colorFrom;
		TimeAnimateColor	colorTo;
	};

	struct TimeColorBehaviorContainer : public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader			=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			SRecordHeader ReadHeader;
			if (ReadHeader.ReadFromStream(pStream) )	
				colorBehaviorAtom.ReadFromStream ( ReadHeader, pStream );

			if (ReadHeader.ReadFromStream(pStream) )	
				behavior.ReadFromStream (ReadHeader, pStream );
		}


		virtual bool IsCorrect () {	return	m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x0 && m_oHeader.RecType == RT_TimeColorBehaviorContainer;	}

	public:

		TimeColorBehaviorAtom	colorBehaviorAtom;
		TimeBehaviorContainer	behavior;
	};
}

namespace Animations
{	
	struct TimeModifierAtom : public CUnknownRecord
	{
	public:
		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader	=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			type		=	StreamUtils::ReadDWORD(pStream);
			value		=	StreamUtils::ReadFLOAT(pStream);
		}


		virtual bool IsCorrect () { return m_oHeader.RecVersion == 0x0 && m_oHeader.RecType == RT_TimeModifierAtom && m_oHeader.RecLen == 0x00000008; }

	public:

		unsigned long type;		//	0x00000000  Repeat count. 
		//	0x00000001  Repeat duration. 
		//	0x00000002  Speed. 
		//	0x00000003  Accelerate. 
		//	0x00000004  Decelerate. 
		float value;
	};

	struct SlaveContainer : public CUnknownRecord
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

            haveTimePropertyList	=	false;
            haveColorBehavior		=	false;
            haveSetBehavior			=	false;
            haveTimeContainer		=	false;
            haveClientVisualElement	=	false;
            haveExtTimeContainer	=	false;
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

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			SRecordHeader	rHeader;

			if ( rHeader.ReadFromStream(pStream) )	
				timeNodeAtom.ReadFromStream ( rHeader, pStream );

			UINT lCurLen	=	0;
			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
                if ( ReadHeader.ReadFromStream(pStream) == false)
					break;

				lCurLen			+=	8 + ReadHeader.RecLen;

				WORD nRecord	=	ReadHeader.RecType;

				if ( RT_TimePropertyList	== nRecord )
				{
                    haveTimePropertyList	 = true;
					timePropertyList	 = new TimePropertyList4TimeNodeContainer ();
					timePropertyList->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}

				if ( RT_TimeSetBehaviorContainer == nRecord )
				{
                    haveSetBehavior	 = true;
					timeSetBehavior		 = new TimeSetBehaviorContainer();
					timeSetBehavior->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}				

				if ( RT_TimeColorBehaviorContainer == nRecord )
				{
                    haveColorBehavior = true;
					timeColorBehavior	 = new TimeColorBehaviorContainer();
					timeColorBehavior->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}				

				if ( RT_TimeClientVisualElement == nRecord )
				{
                    haveClientVisualElement	 = true;
					clientVisualElement	 = new ClientVisualElementContainer ();
					clientVisualElement->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}

				if (RT_TimeExtTimeNodeContainer == nRecord)
				{
					extTimeContainer				=	CAnimationFactory::BuildAnimationObject (nRecord); // ExtTimeNodeContainer
					if (extTimeContainer)
					{
                        haveExtTimeContainer = true;
						extTimeContainer->ReadFromStream  ( ReadHeader, pStream );
						continue;
					}
				}
				StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
			}
		}

		virtual bool IsCorrect() { return m_oHeader.RecVersion == 0xF && m_oHeader.RecInstance == 0x01 && m_oHeader.RecType == 0xF145; }

	public:

		TimeNodeAtom							timeNodeAtom;	 
		TimePropertyList4TimeNodeContainer*		timePropertyList;				
		TimeColorBehaviorContainer*				timeColorBehavior;
		TimeSetBehaviorContainer*				timeSetBehavior;
		//TimeCommandBehaviorContainer*			timeCommandBehavior;
		ClientVisualElementContainer*			clientVisualElement;
		TimeConditionContainer*					rgBeginTimeCondition;

		IRecord*								extTimeContainer;	//	в спецификации такого элемента не должно быть

        bool									haveTimePropertyList;
        bool									haveColorBehavior;
        bool									haveSetBehavior;
        bool									haveTimeContainer;
        bool									haveClientVisualElement;
        bool									haveExtTimeContainer;
	};

	struct ExtTimeNodeContainer	: public CUnknownRecord
	{
	public:

		ExtTimeNodeContainer()
		{
            haveTimePropertyList	=	false;
            haveAnimateBehavior		=	false;
            haveSetBehavior			=	false;
            haveEffectBehavior		=	false;
            haveColorBehavior		=	false;
            haveMotionBehavior		=	false;
            haveRotationBehavior	=	false;
            haveScaleBehavior		=	false;
            haveSlaveContainer		=	false;
            haveClientVisualElement	=	false;
            haveSequenceAtom		=	false;
            haveIterateDataAtom		=	false;
            haveBuildList			=	false;

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

			for ( size_t i = 0; i < rgExtTimeNodeChildren.size(); ++i )
				RELEASEOBJECT ( rgExtTimeNodeChildren[i] );

			for ( size_t i = 0; i < timeCondition.size(); ++i )
				RELEASEOBJECT ( timeCondition[i] );

			for ( size_t i = 0; i < rgTimeModifierAtom.size(); ++i )
				RELEASEOBJECT ( rgTimeModifierAtom[i] );
		}

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

			LONG lPos = 0;
			StreamUtils::StreamPosition ( lPos, pStream );

			SRecordHeader	rHeader;

			if ( rHeader.ReadFromStream(pStream) )	
				timeNodeAtom.ReadFromStream ( rHeader, pStream );

			UINT lCurLen	=	0;

			SRecordHeader ReadHeader;

			while ( lCurLen < m_oHeader.RecLen )
			{
                if ( ReadHeader.ReadFromStream(pStream) == false)
					break;

				lCurLen +=	8 + ReadHeader.RecLen;

				//AnimationsClassType nRecord	=	(AnimationsClassType)ReadHeader.RecType;
				WORD nRecord	=	ReadHeader.RecType;

				if ( RT_TimeExtTimeNodeContainer	==	nRecord )
				{
					ExtTimeNodeContainer* pContainer = new ExtTimeNodeContainer ();
					if (pContainer)
					{
						pContainer->ReadFromStream  ( ReadHeader, pStream );
						rgExtTimeNodeChildren.push_back ( pContainer );

						continue;
					}
				}

				if ( RT_BuildList	==	nRecord )
				{
					buildList = new BuildListContainer ();
					if (buildList)
					{
						buildList->ReadFromStream  ( ReadHeader, pStream );
                        haveBuildList = true;

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
					timeSequenceDataAtom = new TimeSequenceDataAtom ();
					if (timeSequenceDataAtom)
					{
						timeSequenceDataAtom->ReadFromStream  ( ReadHeader, pStream );
                        haveSequenceAtom = true;

						continue;
					}
				}

				if ( RT_TimeIterateData == nRecord )
				{
					timeIterateDataAtom = new TimeIterateDataAtom ();
					if (timeIterateDataAtom)
					{
						timeIterateDataAtom->ReadFromStream  ( ReadHeader, pStream );
                        haveIterateDataAtom = true;

						continue;
					}
				}

				if ( RT_TimePropertyList == nRecord )
				{
					timePropertyList = new TimePropertyList4TimeNodeContainer ();
					if (timePropertyList)
					{
						timePropertyList->ReadFromStream  ( ReadHeader, pStream );
                        haveTimePropertyList	=	true;

						continue;
					}
				}

				if ( RT_TimeEffectBehaviorContainer == nRecord )
				{
					timeEffectBehavior = new TimeEffectBehaviorContainer();
					if (timeEffectBehavior)
					{
						timeEffectBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveEffectBehavior = true;
						continue;
					}
				}

				if ( RT_TimeColorBehaviorContainer == nRecord )
				{
					timeColorBehavior = new TimeColorBehaviorContainer();
					if (timeColorBehavior)
					{
						timeColorBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveColorBehavior = true;

						continue;
					}
				}

				if ( RT_TimeMotionBehaviorContainer == nRecord )
				{
					timeMotionBehavior = new TimeMotionBehaviorContainer ();
					if (timeMotionBehavior)
					{
						timeMotionBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveMotionBehavior = true;

						continue;
					}
				}

				if ( RT_TimeSetBehaviorContainer == nRecord )
				{
					timeSetBehavior	 = new TimeSetBehaviorContainer ();
					if (timeSetBehavior)
					{
						timeSetBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveSetBehavior	 = true;

						continue;
					}
				}

				if ( RT_TimeAnimateBehaviorContainer == nRecord )
				{
					timeAnimateBehavior = new TimeAnimateBehaviorContainer ();
					if (timeAnimateBehavior)
					{
						timeAnimateBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveAnimateBehavior = true;

						continue;
					}
				}

				if ( RT_TimeRotationBehaviorContainer == nRecord )
				{
					timeRotationBehavior = new TimeRotationBehaviorContainer ();
					if (timeRotationBehavior)
					{
						timeRotationBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveRotationBehavior	=	true;

						continue;
					}
				}

				if ( RT_TimeScaleBehaviorContainer == nRecord )
				{
					timeScaleBehavior = new TimeScaleBehaviorContainer ();
					if (timeScaleBehavior)
					{
						timeScaleBehavior->ReadFromStream  ( ReadHeader, pStream );
                        haveScaleBehavior = true;

						continue;
					}
				}

				if (RT_TimeClientVisualElement == nRecord)
				{
                    haveClientVisualElement	 = true;
					clientVisualElement	 = new ClientVisualElementContainer ();
					clientVisualElement->ReadFromStream  ( ReadHeader, pStream );

					continue;
				}

				if ( RT_TimeSlaveContainer == nRecord )
				{
					rgSlave			 = new SlaveContainer ();
					if (rgSlave)
					{
						rgSlave->ReadFromStream  ( ReadHeader, pStream );
                        haveSlaveContainer = true;

						continue;
					}
				}

				if (RT_TimeModifierAtom == nRecord)
				{
					TimeModifierAtom* atom = new TimeModifierAtom ();
					if (atom)
					{
						atom->ReadFromStream  ( ReadHeader, pStream );
						rgTimeModifierAtom.push_back (atom);
						continue;
					}
				}
				StreamUtils::StreamSkip ( ReadHeader.RecLen, pStream );
			}


			// StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
		}


		//
		inline TimeModifierAtom* GetModifier (long Type)
		{
			for ( size_t i = 0; i < rgTimeModifierAtom.size(); ++i )
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

        bool									haveTimePropertyList;
        bool									haveSequenceAtom;
        bool									haveIterateDataAtom;

        bool									haveEffectBehavior;
        bool									haveMotionBehavior;
        bool									haveAnimateBehavior;
        bool									haveColorBehavior;
        bool									haveRotationBehavior;
        bool									haveScaleBehavior;
        bool									haveSetBehavior;
        bool									haveClientVisualElement;
        bool									haveSlaveContainer;

        bool									haveBuildList;
	};

	struct SlideTime10Atom : public CUnknownRecord
	{
		static const _UINT32 RT_SlideTime10Atom	=	0x2EEB;

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			pStream->read ((unsigned char*) &m_FileTime, sizeof ( FILETIME ) );

#if defined(_WIN32) || defined (_WIN64)
            FileTimeToSystemTime ( &m_FileTime, &m_SystemTime );
#else
            //todooo
#endif
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2EEB &&
				m_oHeader.RecLen			==	0x00000008;
		}

	public:

		SRecordHeader	m_oHeader;
		FILETIME		m_FileTime;
		SYSTEMTIME		m_SystemTime;
	};

	struct SlideFlags10Atom : public CUnknownRecord
	{
		static const _UINT32 RT_SlideFlags10Atom = 0x2EEA;

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader = oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			_UINT32 Value = 0L;
			pStream->read ((unsigned char*) &Value, sizeof ( Value ));

			m_bPreserveMaster			=	( 0x01 == ( 0x01 & ((BYTE)Value) ) );
			m_bOverrideMasterAnimation	=	( 0x02 == ( 0x02 & ((BYTE)Value) ) );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2EEA &&
				m_oHeader.RecLen			==	0x00000004;
		}

	public:

		bool			m_bPreserveMaster;					//	A bit that specifies whether to preserve the main master slide or title 
		bool			m_bOverrideMasterAnimation;			//	A bit that specifies whether the slide does not follow the animations on the main master slide or title master slide.  
	};

	struct HashCode10Atom : public CUnknownRecord
	{
		//static const _UINT32 RT_HashCodeAtom	=	0x2B00;

		virtual void ReadFromStream ( SRecordHeader & oHeader, POLE::Stream* pStream )
		{
			m_oHeader	=	oHeader;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
			if( IsCorrect () == false ) return;
#endif
			_UINT32 Value; 
			pStream->read ( (unsigned char*) &Value, sizeof ( _UINT32 ) );
		}

		virtual bool IsCorrect ( )
		{
			return	m_oHeader.RecVersion	==	0x0	&&
				m_oHeader.RecInstance		==	0x0 &&
				m_oHeader.RecType			==	0x2B00 &&
				m_oHeader.RecLen			==	0x00000004;
		}

	};
}

namespace Animations
{
	typedef PPT_FORMAT::CAnimationSimple Effect;
	typedef std::vector <Effect> Effects;
	typedef std::map<_UINT32,Effects*> EffectsMap;
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
		}

		inline void Pop ()
		{
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

        bool Create (ExtTimeNodeContainer* pTimeNode)
		{
			return FindBuildList (pTimeNode);
		}

		inline long GetBuild (long nShapeID)
		{
			for ( size_t i = 0; i < m_arrBuilds.size(); ++i )
			{
				if (m_arrBuilds[i].m_nShapeID == nShapeID)
					return m_arrBuilds[i].m_nBuildType;
			}

			return -1;
		}

	private:

        inline bool FindBuildList ( ExtTimeNodeContainer* pContainer )
		{
			if ( pContainer->buildList )
			{
				for ( size_t i = 0; i < pContainer->buildList->rgChildRec.size(); ++i )
					m_arrBuilds.push_back(EffectBuild(pContainer->buildList->rgChildRec[i]->buildAtom.shapeIdRef,	pContainer->buildList->rgChildRec[i]->paraBuildAtom.buildLevel) );

                return true;
			}

			for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if (FindBuildList ( pContainer->rgExtTimeNodeChildren [i]) )
                    return true;
			}

            return true;
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

        inline bool				Build (ExtTimeNodeContainer* pContainer)
		{
			if (pContainer)
			{	
				Clear ();

                m_bSaveEffect		=	false;
                m_HaveAfterEffect	=	false;

				m_oParagraphBuilds.Create(pContainer);

				ExploreTree ( pContainer );

				UpdateParagraph ();

				return (0 != m_oAnimation.size());
			}

            return false;
		}

		inline EffectsMap& GetAnimation ()
		{
			return m_oAnimation;
		}

		inline double GetTime ()
		{
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
			unsigned long nNodeType =	pContainer->GetEffectNodeType ();
			if (nNodeType)
			{
				if (CNodeTiming::MainSequenceNode != nNodeType && CNodeTiming::TimingRootNode != nNodeType)
				{
					m_oTopEffect =	CreateEffectFromNode ( pContainer->timePropertyList );

					m_ComposeEffectMothionPath	=	_T("");

					FindEffectMothionPath ( pContainer );

					// время старта анимации
					if ( pContainer->timeCondition.size() )
					{
						double dTime	=	static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay;
						m_oNodeTiming.SetTimeDelay (dTime);	

						//m_oTopEffect.m_nBeginTime	=	static_cast<TimeConditionContainer*> ( pContainer->timeCondition[0] )->m_oTimeConditionAtom.m_nTimeDelay;

                        m_bSaveEffect	=	false;
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
                            m_HaveAfterEffect	 = true;

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

			int nID =	GetShapeID ( pContainer );
			if ( -1 != nID )
			{
				//	TODO : 
				//	нужно из всех объектов (*Behavior) для конкретного эффекта,
				//	который заранее прочитан в m_oTopEffect дополнять параметры из потомков

                if ( false == m_bSaveEffect )
				{
					AddAnimation ( pContainer, nID );

                    m_bSaveEffect	=	true;
				}
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
		inline void	AddAnimation (ExtTimeNodeContainer* pTimeNode, int nID)
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
                    oEffect.m_dTransparency		=	_tstof (pTimeNode->timeSetBehavior->varTo.stringValue.c_str());
			}

            if (m_ComposeEffectMothionPath.length())
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
            m_oTopEffect.m_bIgnoreShape		=	false;
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
                m_HaveAfterEffect			=	false;
			}
		}
		inline void	AddEffectTopMap (Effect& oEffect, long nID)
		{
            oEffect.m_bRemoveEmptyBlocks	=	true;	//	ALWAYS

			EffectToMap (oEffect);
		}
		inline void EffectToMap (const Effect& oEffect)
		{
			EffectsMap::const_iterator mSearch	=	m_oAnimation.find(oEffect.m_nRefID);
			Effects* pEffects = NULL;
			if (mSearch == m_oAnimation.end())
			{
				pEffects = new Effects ();
				m_oAnimation.insert(std::pair<_UINT32,Effects*>( oEffect.m_nRefID, pEffects));	
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
				if (!pContainer->rgExtTimeNodeChildren.empty())
				{
					ExtTimeNodeContainer* pChild = pContainer->rgExtTimeNodeChildren[0];
					if (pChild)
					{
						if (pChild->timeNodeAtom.m_bDurationProperty)		
						{
							m_dEffectDuration =	pChild->timeNodeAtom.m_nDuration;
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
				if (!m_arDurations.empty() && !m_arBeginTimes.empty() )
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

        inline bool MediumEffectDuration (ExtTimeNodeContainer* pContainer)
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

            return true;
		}

        inline bool MediumEffectTimeDelay (ExtTimeNodeContainer* pContainer)
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

            return true;
		}
        inline bool FindEffectMothionPath (ExtTimeNodeContainer* pContainer)
		{
			// сложные эффект может содержать в себе перемещение по траектории
			if (52 == m_oTopEffect.m_nEffectID)			//	ArcUp
			{
				if ( pContainer->haveMotionBehavior )
				{
					m_ComposeEffectMothionPath	=	pContainer->timeMotionBehavior->m_VarPath.stringValue;
					// debug::trace (m_ComposeEffectMothionPath);
                    return true;
				}

				for ( int i = 0; i < (int)pContainer->rgExtTimeNodeChildren.size(); ++i )
					FindEffectMothionPath ( pContainer->rgExtTimeNodeChildren [i] );

                return true;
			}

            return false;
		}

		inline void ReadPropertyIgnoreShape (const TimeBehaviorContainer& container)
		{
			if (2 == container.clientVisualElement.m_oVisualShapeAtom.m_Type)										//	указывает на то что анимация применяется к тексту
			{
                m_oTopEffect.m_bIgnoreShape	 = true;
				m_oTopEffect.m_nTextSequence		=	container.clientVisualElement.m_oVisualShapeAtom.m_nData1;	//	номер параграфа - не нормальзован
                m_oTopEffect.m_bRemoveEmptyBlocks	=	true;

				for (size_t i = 0; i < m_arParIndexer.size(); ++i)
				{
					if (m_oTopEffect.m_nTextSequence == m_arParIndexer.at(i))
						return;
				}

				m_arParIndexer.push_back (m_oTopEffect.m_nTextSequence);
			}
		}

        inline bool UpdateParagraph ()
		{
			if (!m_arParIndexer.empty())
			{
				//ATLTRACE ( _T("====================PARAGRAPH==========================================\n"));
				//qsort ( m_arParIndexer.GetData(), m_arParIndexer.size(), sizeof (long), (int(*)(const void*, const void*))sort );

				sort(m_arParIndexer.begin(), m_arParIndexer.end());

				for (std::map<_UINT32, Effects*>::iterator pPair = m_oAnimation.begin(); pPair != m_oAnimation.end(); ++pPair)
				{
					Effects* arEffects = pPair->second;
					if (arEffects)
					{
						for (size_t i = 0; i < arEffects->size(); ++i)
						{
							Effect& oEffect = arEffects->at(i);
							if (oEffect.m_nTextSequence >= 0)
							{
								for (size_t ind = 0; ind < m_arParIndexer.size(); ++ind)
								{
									if (oEffect.m_nTextSequence == m_arParIndexer.at(ind))
									{
										oEffect.m_nTextSequence = ind;

										break;
									}
								}
							}
						}
					}
				}

                return true;
			}

            return false;
		}

		//
		inline void ProcessMediaCall (ExtTimeNodeContainer* pContainer)
		{			
			if (MediaCallEffect == m_oTopEffect.m_nEffectType)		//	если анимация применена к VIDEO или AUDIO элементу
			{
				m_nMediaPush		=	GetAttachedShapeToVideo (pContainer);	//	если к видео добавлена картинка, надо учитывать смещение при поиск ID

				if (GetMediaID (pContainer))
				{
					AddAnimation (pContainer, m_nMediaID);
                    m_bSaveEffect	=	true;
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
                        return true;

					--m_nMediaPush;
				}
			}

			for (size_t i = 0; i < pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if (GetMediaID(pContainer->rgExtTimeNodeChildren [i]))
                    return true;
			}

            return false;
		}

        inline bool GetAttachedShapeToVideo (ExtTimeNodeContainer* pContainer)
		{
			if (TL_TNT_Media == pContainer->GetNodeType ())																//	нод типа Media
			{
				if (pContainer->clientVisualElement)
				{
					m_arrMedia.push_back (pContainer->clientVisualElement->m_oVisualShapeAtom.m_nObjectIdRef);

					if (2 == (int)m_arrMedia.size())																//	у видео есть статическое изображение
                        return true;
				}
			}

			for (size_t i = 0; i < pContainer->rgExtTimeNodeChildren.size(); ++i )
			{
				if (CNodeTiming::AfterPreviousNode == pContainer->rgExtTimeNodeChildren [i]->GetEffectNodeType ())		//	нод с описанием эффекта
                    return false;

				if (GetAttachedShapeToVideo(pContainer->rgExtTimeNodeChildren [i]))
                    return true;
			}

            return false;
		}

		//
		inline void Clear ()
		{
			for (std::map<_UINT32,Effects*>::iterator pPair = m_oAnimation.begin(); pPair != m_oAnimation.end(); ++pPair)
			{
				RELEASEOBJECT(pPair->second);
			}

			m_oAnimation.clear();
		}


	private: 

		Effect				m_oAfterEffect;

        bool				m_bSaveEffect;
		Effect				m_oTopEffect;
		CNodeTiming			m_oNodeTiming;

		double				m_dEffectDuration;
		TimeArray			m_arDurations;
		TimeArray			m_arBeginTimes;

        std::wstring		m_ComposeEffectMothionPath;
        bool				m_HaveAfterEffect;

		EffectsMap			m_oAnimation;
		CParagraphBuilds	m_oParagraphBuilds;

		std::vector <_INT32>	m_arParIndexer;
		
		// media
		int					m_nMediaID;
		int					m_nMediaShapeID;
		int					m_nMediaPush;					//	глубина поиска

		std::vector <_INT32> m_arrMedia;
	};
}

struct PP10SlideBinaryTagExtension : public CUnknownRecord
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
            if ( ReadHeader.ReadFromStream(pStream) == false)
				break;

			lCurLen += 8 + ReadHeader.RecLen;

			if (Animations::RT_TimeExtTimeNodeContainer == ReadHeader.RecType)
			{
				extTimeNodeContainer = new Animations::ExtTimeNodeContainer ();
				if (extTimeNodeContainer)
				{
					extTimeNodeContainer->ReadFromStream  ( ReadHeader, pStream );
					continue;
				}
			}

			if (Animations::RT_BuildList == ReadHeader.RecType)
			{
				buildListContainer = new Animations::BuildListContainer ();
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
public:

	//Animations::SlideTime10Atom			m_oSlideTime10Atom;
	//Animations::HashCode10Atom			m_oHashCode10Atom;

	Animations::ExtTimeNodeContainer*	extTimeNodeContainer;	//	ROOT
	Animations::BuildListContainer*		buildListContainer;		//	OPTIONAL
};

struct SlideProgTagsContainer : public CUnknownRecord
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

            tagName	=	std::wstring ( Name );

            if ( std::wstring ( L"___PPT10" ) == tagName )
			{
				SRecordHeader rhData;
				rhData.ReadFromStream(pStream) ;	

				if (rhData.RecType == 0x138B && rhData.RecVersion == 0x0 && rhData.RecInstance == 0x000)	//	RT_BinaryTagDataBlob - 0x138B
				{
					m_PP10SlideBinaryTagExtension.ReadFromStream (rhData, pStream);

				}
			}
		}

		StreamUtils::StreamSeek ( lPos + m_oHeader.RecLen, pStream );
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

    std::wstring					tagName;

	PP10SlideBinaryTagExtension	m_PP10SlideBinaryTagExtension;

	SRecordHeader				m_oHeaderChild;
};
