#pragma once
#include "../../../AVSOfficeStudio/Common/OfficeDrawing/Structures.h"
#include "SlidePersist.h"

using namespace NSAttributes;

/**************************************************************
теперь все структуры...
которые участвуют в записях
**************************************************************/
struct SRatioAtom 
{ 
	LONG Number; 
	LONG Denom; 

	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("<Ratio info='(%d,%d)' />"), Number, Denom);
		return str;
	}

	void FromStream(IStream* pStream)
	{
		Number = StreamUtils::ReadLONG(pStream);
		Denom = StreamUtils::ReadLONG(pStream);
	}
}; 
 
struct SScalingAtom 
{ 
	SRatioAtom X; 
	SRatioAtom Y; 

	CString ToString()
	{
		return X.ToString() + Y.ToString();
	}

	void FromStream(IStream* pStream)
	{
		X.FromStream(pStream);
		Y.FromStream(pStream);
	}
};

struct SSlideLayoutAtom
{
	INT m_nGeom;
	BYTE m_pPlaceHolderID[8];

	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("<SSlideLayoutAtom Geom='%d' PlaceHolderIDs='(%d,%d,%d,%d,%d,%d,%d,%d)' />"),
			m_nGeom, m_pPlaceHolderID[0], m_pPlaceHolderID[1], m_pPlaceHolderID[2], m_pPlaceHolderID[3], 
			m_pPlaceHolderID[4], m_pPlaceHolderID[5], m_pPlaceHolderID[6], m_pPlaceHolderID[7]);
		return str;
	}
};



struct SFileIdCluster
{
	UINT DrawingGroupID;
	UINT CurrentShapeID;

	void ReadFromStream(IStream* pStream)
	{
		DrawingGroupID = (UINT)StreamUtils::ReadDWORD(pStream);
		CurrentShapeID = (UINT)StreamUtils::ReadDWORD(pStream);
	}
	
	CString ToString()
	{
		CString str = _T("");
		str.Format(_T("<FileIdCluster drawinggroupID='%d' currentshapeID='%d' />"), DrawingGroupID, CurrentShapeID);
		return str;
	}
};

