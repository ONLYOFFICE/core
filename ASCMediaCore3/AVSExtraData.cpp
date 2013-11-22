// AVSExtraData.cpp : Implementation of CAVSExtraData

#include "stdafx.h"
#include "ATLDefine.h"
#include "AVSExtraData.h"


// CAVSExtraData

STDMETHODIMP CAVSExtraData::get_Year(long *pVal)
{
	*pVal=ExtraInfo.Year;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Year(long newVal)
{
	ExtraInfo.Year=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Title(BSTR *pVal)
{
	*pVal=ExtraInfo.Title.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Title(BSTR newVal)
{
	ExtraInfo.Title=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Copyright(BSTR *pVal)
{
	*pVal=ExtraInfo.Copyright.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Copyright(BSTR newVal)
{
	ExtraInfo.Copyright=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Comments(BSTR *pVal)
{
	*pVal=ExtraInfo.Comments.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Comments(BSTR newVal)
{
	ExtraInfo.Comments=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Artist(BSTR *pVal)
{
	*pVal=ExtraInfo.Artist.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Artist(BSTR newVal)
{
	ExtraInfo.Artist=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Album(BSTR *pVal)
{
	*pVal=ExtraInfo.Album.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Album(BSTR newVal)
{
	ExtraInfo.Album=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Genre(BSTR *pVal)
{
	*pVal=ExtraInfo.Genre.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Genre(BSTR newVal)
{
	ExtraInfo.Genre=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Composer(BSTR *pVal)
{
	*pVal=ExtraInfo.Composer.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Composer(BSTR newVal)
{
	ExtraInfo.Composer=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_EncodedBy(BSTR *pVal)
{
	*pVal=ExtraInfo.EncodedBy.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_EncodedBy(BSTR newVal)
{
	ExtraInfo.EncodedBy=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Track(long *pVal)
{
	*pVal=ExtraInfo.Track;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Track(long newVal)
{
	ExtraInfo.Track=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_URL(BSTR *pVal)
{
	*pVal=ExtraInfo.URL.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_URL(BSTR newVal)
{
	ExtraInfo.URL=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_OriginalArtist(BSTR* pVal)
{
	*pVal=ExtraInfo.OriginalArtist.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_OriginalArtist(BSTR newVal)
{
	ExtraInfo.OriginalArtist=newVal;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_Picture(IPictureDisp** pVal)
{
	*pVal = ExtraInfo.Picture;
	ADDREFINTERFACE((*pVal));
	return S_OK;
}
STDMETHODIMP CAVSExtraData::put_Picture( IPictureDisp* newVal)
{
	RELEASEINTERFACE(ExtraInfo.Picture);
	ExtraInfo.Picture = newVal;
	ADDREFINTERFACE(ExtraInfo.Picture);
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_AdditionalCount( long* pVal)
{
	*pVal = (long)ExtraInfo.Additional.GetCount();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::GetAdditional(long Index, BSTR* Name,BSTR* Value)
{
	if (Index>=(long)ExtraInfo.Additional.GetCount())
		return S_OK;

	*Name=ExtraInfo.Additional[Index].Name.Copy();
	*Value=ExtraInfo.Additional[Index].Value.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::SetAdditional(long Index, BSTR Name, BSTR Value)
{
	_ExtraInfo::_Additional newAdditional;
	newAdditional.Name="";
	newAdditional.Value="";
	while (Index>=(long)ExtraInfo.Additional.GetCount())
	{
		ExtraInfo.Additional.Add(newAdditional);
	}
	ExtraInfo.Additional[Index].Name=Name;
	ExtraInfo.Additional[Index].Value=Value;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::DeleteAdditional(long Index)
{
	ExtraInfo.Additional.RemoveAt(Index);
	return S_OK;
}
STDMETHODIMP CAVSExtraData::get_MarkerCount( long* pVal)
{
	*pVal = (long)ExtraInfo.Marker.GetCount();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::GetMarker(long Index,double* TimeStamp,double* Duration,BSTR* Label,BSTR* Note)
{
	if (Index>(long)ExtraInfo.Marker.GetCount())
		return S_OK;

	*TimeStamp=ExtraInfo.Marker[Index].Position;
	*Duration=ExtraInfo.Marker[Index].Length;
	*Label=ExtraInfo.Marker[Index].Label.Copy();
	*Note=ExtraInfo.Marker[Index].Note.Copy();
	return S_OK;
}
STDMETHODIMP CAVSExtraData::SetMarker(long Index,double TimeStamp,double Duration,BSTR Label,BSTR Note)
{
	_ExtraInfo::_Marker newMarker;
	newMarker.Label="";
	newMarker.Length=0;
	newMarker.Note="";
	newMarker.Position=0;

	while (Index>=(long)ExtraInfo.Marker.GetCount())
	{
		ExtraInfo.Marker.Add(newMarker);
	}
	ExtraInfo.Marker[Index].Label=Label;
	ExtraInfo.Marker[Index].Length=Duration;
	ExtraInfo.Marker[Index].Note=Note;
	ExtraInfo.Marker[Index].Position=TimeStamp;
	return S_OK;
}
STDMETHODIMP CAVSExtraData::DeleteMarker(long Index)
{
	ExtraInfo.Marker.RemoveAt(Index);
	return S_OK;
}
