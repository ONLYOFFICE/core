/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Open Exif Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2001 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Creation Date: 06/05/2005
 *
 * Original Author: 
 * Sam Fryer samuel.fryer@kodak.com 
 *
 * Contributor(s): 
 * 
 */ 

#ifndef _EXIF_COM_MARKER_H_
#define _EXIF_COM_MARKER_H_
namespace Exif 
{

class ExifComMarkerList;

class ExifComMarker
{
    public:
            
        ExifComMarker() : next(NULL), mSize(0), mData(NULL) {};
    
        ~ExifComMarker()
        {
            if (mData != NULL) delete[] mData;
            if (next != NULL) delete next;
        };
        
        const uint8 * getData(tsize_t &size)
        {
            size = mSize;
            return mData;
        };
        
        tsize_t setData(uint8 * data, tsize_t size)
        {
            mSize = size;
            mData = new uint8[size];
            memcpy(mData,data,size);
            return mSize;
        };
        
        friend class ExifComMarkerList;
        
    private:
            
        ExifComMarker * next;
        tsize_t mSize;
        uint8 * mData;
};


class ExifComMarkerList
{
    public:

        ExifComMarkerList() : mCount(0),mMarkers(NULL) {};
    
        ~ExifComMarkerList()
        {
            if (mMarkers != NULL) delete mMarkers;
        };
        
        const unsigned int size()
        {
            return mCount;
        }
        
        ExifComMarker * getComMarker (unsigned int index)
        {
            unsigned int i = 0;
            ExifComMarker * res = NULL;
            ExifComMarker * marker = mMarkers;
            if (mCount > 0)
            {
                while ((i < mCount-1) && (i<index) && (marker != NULL))
                { marker = marker->next; i++; }
                if (i == index) res = marker;
            }
            return res;
        }
        
        unsigned int addData(uint8 * data, tsize_t dataSize)
        {
            ExifComMarker * newMarker = new ExifComMarker();
            newMarker->setData(data,dataSize);
            if (mMarkers == NULL)
            {
                mMarkers = newMarker;
            }
            else
            {
               ExifComMarker * marker = mMarkers;
               while ( marker->next != NULL) marker = marker->next;
               marker->next = newMarker;
            }
            mCount++;
            return mCount;
        }
        
    private:
            
        unsigned int mCount;
        ExifComMarker * mMarkers;
};

}  
#endif
