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
 * Portions of the Original Code are
 * Copyright (c) 1988-1996 Sam Leffler
 * Copyright (c) 1991-1996 Silicon Graphics, Inc.
 *
 * Creation Date: 07/14/2001
 *
 * Original Author: 
 * George Sotak george.sotak@kodak.com 
 *
 * Contributor(s): 
 * Ricardo Rosario ricardo.rosario@kodak.com
 * Sam Fryer       samuel.fryer@kodak.com
 */ 


#include <stdlib.h>
#include <cmath>
#include <climits>

#include "ExifJpegImage.h"
#include "ExifTypeDefs.h"
#include "ExifTags.h"
#include "ExifIO.h"
#include "ExifStripImage.h"
#include "ExifIFD.h"
#include "ExifImageDescUtils.h"
namespace Exif{
template < class Iter, class A >
void copy_vec_to_array(Iter start, Iter end, A result, int max)
{
	int i = 0;
	for(i=0;(start != end) && (i < max);i++,start++)
		*(result+i) = *start;
}
#define EXIF_COPY copy_vec_to_array

ExifIFD::SubIFDVec    ExifIFD::msSubDirTags;

struct ExifImportantValuesCache
{
    public:
        ExifImportantValuesCache()
            : mImageWidth(0), mImageLength(0), mPhotoMetric(0),
              mPlanarConfig(0), mSamplesPerPixel(0), mRowsPerStrip(0),
              mCompression(0), mJpegInterchange(0), mJpegInterchangeLength(0),
              mYccSubSampling(0), mBitsPerSample(0), mStripOffsets(0),
              mStripByteCounts(0)
            {}

        ExifTagEntryT<exif_uint32>* mImageWidth ;
        ExifTagEntryT<exif_uint32>* mImageLength ;
        ExifTagEntryT<uint16>* mPhotoMetric ;
        ExifTagEntryT<uint16>* mPlanarConfig ;
        ExifTagEntryT<uint16>* mSamplesPerPixel ;
        ExifTagEntryT<exif_uint32>* mRowsPerStrip ;
        ExifTagEntryT<uint16>* mCompression ;
        ExifTagEntryT<exif_uint32>* mJpegInterchange ;
        ExifTagEntryT<exif_uint32>* mJpegInterchangeLength ;
        ExifTagEntryT<std::vector<uint16> >* mYccSubSampling ;
        ExifTagEntryT<std::vector<uint16> >* mBitsPerSample ;
        ExifTagEntry* mStripOffsets ;
        ExifTagEntry* mStripByteCounts ;
};


ExifIFD::ExifIFD(exiftag_t tagNum, exifoff_t _offset, 
                 exifoff_t _exifOffset, int16 idx,
                 ExifIFDMap * _ifdmap, ExifIFD * parent)
  : mImage(NULL), mCompressedThumb(NULL), mCompressedThumbLength(0),
    mEntryOffset(0), mDataOffset(0), mEndOfData(0),
    mOffset(_offset), mExifOffset(_exifOffset), mNextDirOffset( 0 ),
    mWriteOffset(0), mParentDataOffset(0), mOffsetToStripOffsets(0),
    mOffsetToStripByteCounts(0), mOffsetToJpegInterop(0),
    mOffsetToJpegByteCount(0), mTag( tagNum ), mIndex(idx),
	mIFDmap(_ifdmap),mParent(parent), bLoadImage(true)
{  
    mCache= new ExifImportantValuesCache ;
}


ExifIFD::ExifIFD( exiftag_t tagNum, int16 idx, 
                 ExifIFDMap * _ifdmap, ExifIFD * parent)
  : mImage(NULL), mCompressedThumb(NULL), mCompressedThumbLength(0),
    mEntryOffset(0), mDataOffset(0), mEndOfData(0),
    mOffset(0), mExifOffset(0), mNextDirOffset(0), mWriteOffset(0),
    mParentDataOffset(0), mOffsetToStripOffsets(0),
    mOffsetToStripByteCounts(0), mOffsetToJpegInterop(0),
    mOffsetToJpegByteCount(0), mTag( tagNum ), mIndex(idx),
    mIFDmap(_ifdmap),mParent(parent), bLoadImage(true)
{ 
    mCache= new ExifImportantValuesCache ;
}

// Copy constructor (deep copy)
ExifIFD::ExifIFD( const ExifIFD& ifd )
  : mImage(NULL), mCompressedThumb(NULL), mCompressedThumbLength(0),
    mEntryOffset(0), mDataOffset(0), mEndOfData(0),
    mOffset(0), mExifOffset(0), mNextDirOffset(0), mWriteOffset(0),
    mParentDataOffset(0), mOffsetToStripOffsets(0),
    mOffsetToStripByteCounts(0), mOffsetToJpegInterop(0),
    mOffsetToJpegByteCount(0), mTag(ifd.mTag), mIndex(ifd.mIndex),
    mIFDmap(ifd.mIFDmap),mParent(ifd.mParent), bLoadImage(ifd.bLoadImage)
{
    mCache = new ExifImportantValuesCache ;
    
    this->copy( ifd ) ;
}

// deep copy
void ExifIFD::copy( const ExifIFD& ifd )
{
    const EntryMap& entryMap = ifd.getTagEntries();
    const SubIFDMap& dirMap = ifd.getSubIFDs();

    // copy tag entries
    ConstEntryIter s = entryMap.begin();
    while (s != entryMap.end())
    {
        if (!selfGenerated((*s).first))
        {
            setGenericTag(*((*s).second));
        }

        s++;
    }

    // Copy thumbnail, if any
    if ( ifd.mImage )
    {
        ExifImageInfo imgInfo ;
        imgInfo.width = ifd.mImage->numberOfColumns ;
        imgInfo.height = ifd.mImage->numberOfRows ;
        imgInfo.numChannels = ifd.mImage->numberOfComponents ;
        exifAllocImageDesc( imgInfo, *mImage ) ;
        
        memcpy_s( mImage->components[0].theData, imgInfo.width*imgInfo.height*imgInfo.numChannels,
            ifd.mImage->components[0].theData,
            imgInfo.width*imgInfo.height*imgInfo.numChannels ) ;
    }
    if ( ifd.mCompressedThumb )
    {
        mCompressedThumbLength = ifd.mCompressedThumbLength ;
        mCompressedThumb = new uint8[mCompressedThumbLength] ;
        memcpy_s( mCompressedThumb, mCompressedThumbLength, ifd.mCompressedThumb,
            mCompressedThumbLength ) ;
    }
    

    // copy sub dirs
    ConstSubIFDIter ss = dirMap.begin();
    while (ss != dirMap.end())
    {
        ExifIFDVec dirVec1 = (*ss).second;
        ExifIFDVec dirVec2;
        getSubIFDVec((*ss).first, dirVec2);
        for (unsigned int i=0; i<dirVec1.size(); i++)
        {
            ExifIFD* dir1 = dirVec1[i];
            ExifIFD* dir2 = dirVec2[i];
            dir2->copy(*dir1);
        }
        ss ++;
    }
}
    
exifoff_t ExifIFD::writeDirTree( ExifIO* exifio, exifoff_t& tiffHeaderOffset )
{
    mWriteOffset = tiffHeaderOffset ;

    // Make sure some image specific metadata is valid before writing
    // out the IFDs
    uint16 comp ;
    if(mImage!=NULL || mCompressedThumb!=NULL)
    {
        comp = compression() ;
        
        if (mImage==NULL && comp==EXIF_COMPRESSION_NONE)
        {
            // Ae want an uncompressed thumbnail but we have a pointer to
            // a compressed one, so force uncompression.
            getImage();
        }
        
        if (mImage!=NULL && comp==EXIF_COMPRESSION_NONE)
        {
            exif_uint32 numStrips = numOfStrips() ;
            if( numStrips == (exif_uint32)(-1) )
                return EXIF_ERROR ;

            std::vector<exif_uint32> stripInit(numStrips, 0) ;
            
            ExifTagEntry* off ;
            ExifTagEntry* cnt ;

            if ( numStrips > 1 )
            {
                off = new ExifTagEntryT<std::vector<exif_uint32> >(
                                    EXIFTAG_STRIPOFFSETS,
                                    EXIF_LONG, stripInit.size(), stripInit );
                cnt = new ExifTagEntryT<std::vector<exif_uint32> >
                    ( EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, stripInit.size(),
                    stripInit );
            }
            else
            {
                off = new ExifTagEntryT<exif_uint32>
                    ( EXIFTAG_STRIPOFFSETS, EXIF_LONG, 1, 0 );
                cnt = new ExifTagEntryT<exif_uint32>
                    ( EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, 1, 0 );
            }
            setGenericTag( off ) ;
            setGenericTag( cnt ) ;

            // remove tags that are only for compressed Thumbnails
            removeGenericTag(EXIFTAG_JPEGIFOFFSET);
            removeGenericTag(EXIFTAG_JPEGIFBYTECOUNT);
        }
        else if (comp==EXIF_COMPRESSION_JPEG)
        {
            removeGenericTag(EXIFTAG_IMAGEWIDTH);
            removeGenericTag(EXIFTAG_IMAGELENGTH);
            removeGenericTag(EXIFTAG_BITSPERSAMPLE);
            removeGenericTag(EXIFTAG_PHOTOMETRIC);
            removeGenericTag(EXIFTAG_SAMPLESPERPIXEL);
            removeGenericTag(EXIFTAG_ROWSPERSTRIP);
            removeGenericTag(EXIFTAG_PLANARCONFIG);
            removeGenericTag(EXIFTAG_STRIPOFFSETS);
            removeGenericTag(EXIFTAG_STRIPBYTECOUNTS);
            
            // Make sure we do have some necessary tags, if not create them
            ExifTagEntry* jpegLengthTag=getGenericTag(EXIFTAG_JPEGIFBYTECOUNT);
            if (jpegLengthTag==NULL)
            {
                jpegLengthTag = new ExifTagEntryT<exif_uint32>
                    (EXIFTAG_JPEGIFBYTECOUNT, EXIF_LONG, 1, 0);
                setGenericTag( jpegLengthTag ) ;
            }
            
            ExifTagEntry* jpegTag = getGenericTag(EXIFTAG_JPEGIFOFFSET);
            if (jpegTag==NULL)
            {    
                jpegTag = new ExifTagEntryT<exif_uint32>
                    (EXIFTAG_JPEGIFOFFSET, EXIF_LONG, 1, 0);
                setGenericTag( jpegTag ) ;
            }
        }
    }
    else
    {
        // we have no thumbnail, so we shouldn't have any of the following tags
        removeGenericTag(EXIFTAG_JPEGIFOFFSET);
        removeGenericTag(EXIFTAG_JPEGIFBYTECOUNT);
        removeGenericTag(EXIFTAG_IMAGEWIDTH);
        removeGenericTag(EXIFTAG_IMAGELENGTH);
        removeGenericTag(EXIFTAG_BITSPERSAMPLE);
        removeGenericTag(EXIFTAG_PHOTOMETRIC);
        removeGenericTag(EXIFTAG_SAMPLESPERPIXEL);
        removeGenericTag(EXIFTAG_ROWSPERSTRIP);
        removeGenericTag(EXIFTAG_PLANARCONFIG);
    }
   

    exifoff_t linkDirOffset = writeDir( exifio );
    
    SubIFDIter subDir, endSubIFD;
    subDir = mSubDirs.begin();
    endSubIFD = mSubDirs.end();
    while ( subDir != endSubIFD )
    {
        ExifIFDVec dirVec = (*subDir).second;
        for (unsigned int i=0; i<dirVec.size(); i++)
        {
			dirVec[i]->writeDirTree( exifio, tiffHeaderOffset );
        }
        subDir++ ;
    }

    if( mImage != NULL || mCompressedThumb != NULL )
    {
	    comp = compression();
        if( comp == EXIF_COMPRESSION_NONE && mImage != NULL)
        {
            // do something with ExifStripImage
            ExifStatus status ;
            ExifStripImage stripImage( status, this, exifio ) ;
            stripImage.writeImage( *mImage ) ;
            
            writeStripOffsetByteCount( exifio ) ;
        }
        else if( comp == EXIF_COMPRESSION_JPEG )
        {
            // do something with ExifJpegImage
            ExifStatus status = EXIF_OK;
            ExifTagEntryT<exif_uint32>* jpegTag = 
                dynamic_cast<ExifTagEntryT<exif_uint32>*>
                (getGenericTag(EXIFTAG_JPEGIFOFFSET));

            exifoff_t offsetToJpeg = exifio->seek( 0, SEEK_END ) ;

            jpegTag->setValue( offsetToJpeg - mWriteOffset ) ;
            
            if( mCompressedThumb == NULL )
            {
                // A thumbnail can't be over 64K in size, so we just 
                // compress it in memory!
                void * tmpBuf = malloc(0x0000FFFF);
                memset(tmpBuf,0,0x0000FFFF);
                ExifJpegImage jpegImage ;
                status = jpegImage.open( tmpBuf,0x0000FFFF, "w" ) ;
                if( status != EXIF_OK )
                {
                    // HERE HERE CLEAN UP
                    exit(1);
                    free(tmpBuf);
                    return status ;
                }

                ExifImageInfo imgInfo ;
                imgInfo.width = mImage->numberOfColumns ;
                imgInfo.height = mImage->numberOfRows ;
                imgInfo.numChannels = (uint16)mImage->numberOfComponents ;

                jpegImage.setImageInfo( imgInfo ) ;
                jpegImage.set422Sampling();

                exif_uint32 nextLine ;
                exif_uint32 linesDone ;
            
                status = jpegImage.writeScanLines( *mImage,
                    mImage->numberOfRows, nextLine, linesDone ) ;
                jpegImage.close();
                
                unsigned char * backEnd = ((unsigned char *)tmpBuf)+0x0000FFFE;
                while ((*backEnd == 0) && (backEnd != tmpBuf)) backEnd--;
                mCompressedThumbLength = backEnd - ((unsigned char *)tmpBuf)
                                           + sizeof(unsigned char);
                mCompressedThumb = new uint8[mCompressedThumbLength] ;
                memcpy(mCompressedThumb,tmpBuf,mCompressedThumbLength);
                free(tmpBuf);
            }
            exifio->seek( 0, SEEK_END ) ;
            exifio->write( mCompressedThumb, mCompressedThumbLength ) ;
            delete [] mCompressedThumb ;
            mCompressedThumb = NULL ;

            exif_uint32 length = exifio->seek( 0, SEEK_END ) - offsetToJpeg ;

            ExifTagEntryT<exif_uint32>* jpegLengthTag = 
                dynamic_cast<ExifTagEntryT<exif_uint32>*>
                (getGenericTag(EXIFTAG_JPEGIFBYTECOUNT));
            
            jpegLengthTag->setValue( length ) ;

            writeJPEGOffsetByteCount( exifio ) ;   
        }
        else if ( mCompressedThumb != NULL )
        {
            // What if I have a compressed thumb but want to write it out
            // uncompressed ??? Should we add this ability???
            delete [] mCompressedThumb ;
            mCompressedThumb = NULL ;
        }
    }
    
    mEndOfData = exifio->seek( 0, SEEK_END ) ;

    return linkDirOffset ;
}

void ExifIFD::writeStripOffsetByteCount( ExifIO* exifio ) 
{
    std::vector<exif_uint32> sOffsets = stripOffsets() ;
    exif_int32* exif_int32Vec = new exif_int32[sOffsets.size()];

    for( unsigned int i=0; i<sOffsets.size(); i++ )
        *(exif_int32Vec+i) = sOffsets[i] - mWriteOffset ;
    
    exifio->seek( mOffsetToStripOffsets, SEEK_SET ) ;
    exifio->swabArrayOfLong((exif_uint32*)exif_int32Vec, sOffsets.size() );
    exifio->write(exif_int32Vec, sOffsets.size()*sizeof(exif_int32));

    std::vector<exif_uint32> byteCounts = stripByteCounts() ;
    EXIF_COPY( byteCounts.begin(), byteCounts.end(), exif_int32Vec, sOffsets.size() );
    exifio->seek( mOffsetToStripByteCounts, SEEK_SET ) ;
    exifio->swabArrayOfLong( (exif_uint32*)exif_int32Vec, byteCounts.size() );
    exifio->write( exif_int32Vec, byteCounts.size()*sizeof(exif_int32) );

    delete [] exif_int32Vec ;
    
    return ;
}

void ExifIFD::writeJPEGOffsetByteCount( ExifIO* exifio ) 
{
    exif_uint32 jpegOffset ;
    jpegOffset = jpegInterchange() ;
    
    exifio->seek( mOffsetToJpegInterop, SEEK_SET ) ;
    exifio->swabArrayOfLong((exif_uint32*)&jpegOffset, 1 );
    exifio->write(&jpegOffset, sizeof(exif_int32));

    exif_uint32 jpegLength ;
    jpegLength = jpegInterchangeLength( ) ;
    
    exifio->seek( mOffsetToJpegByteCount, SEEK_SET ) ;
    exifio->swabArrayOfLong((exif_uint32*)&jpegLength, 1 );
    exifio->write(&jpegLength, sizeof(exif_int32));

    return ;
}

// When assigning multiple subdirs of the same type to the same directory,
// we need to make sure that only one ExifTagEntry is created. This tag
// is then assigned the multiple offsets and written to the file...
exifoff_t ExifIFD::writeDir( ExifIO* exifio )
{
 
    uint16 nEntries = numEntries();

    mOffset = exifio->seek( 0, SEEK_END );
    // make sure offset is on word boundary
    if (mOffset & 0x00000001) 
    {
        mOffset += 0x00000001;
        uint8 tmpNul = 0;
        exifio->write(&tmpNul,sizeof(uint8));
    }
    
    exifio->seek( mParentDataOffset, SEEK_SET ) ;
    exifoff_t tmpoff = mOffset - mWriteOffset ;
    exifio->swabLong((exif_uint32*)&tmpoff);
    exifio->write(&tmpoff, sizeof(exifoff_t));

    mEntryOffset = mOffset;
    
    mDataOffset = mOffset + nEntries*sizeof(ExifDirEntry) + sizeof(uint16) +
        sizeof(exifoff_t);
    // make sure data offset is on word boundary with respect to the Tiff header
    if ( ((mDataOffset-mWriteOffset)%2) == 1 )
        mDataOffset++;
    
    exifio->seek( mEntryOffset, SEEK_SET ) ;
    uint16 num = nEntries;
    exifio->swabShort(&num);
    exifio->write( &num, sizeof(uint16) );
    mEntryOffset += sizeof(uint16);
    
    EntryIter crntEntry, endEntry;
    crntEntry = mTagEntries.begin();
    endEntry = mTagEntries.end();
    
    while ( crntEntry != endEntry )
    {
        // set subIFD data write offsets if any
        ExifIFDVec dirVec;
        if (getSubIFDVec((*crntEntry).second->getTagNum(), dirVec))
        {
            writeSubIFDArray( exifio, (*crntEntry).second, dirVec ) ;
        }
        else
            writeNormalTag( exifio, (*crntEntry).second );
        
        crntEntry++ ;
    }

    // link the cur dir to the exif dir structure if it is a main dir
    exifoff_t linkOffset = mOffset + sizeof(uint16) + 
        nEntries*sizeof(ExifDirEntry);
    // Write the place keeper for the offset to the next directory
    exif_uint32 nulVal = 0;
    exifio->seek(linkOffset, SEEK_SET);
    exifio->write( &nulVal, sizeof(exif_uint32) );

    return linkOffset ;
}

// Need to track two different offsets when writing:
// the offset to where the current directory entry should be
// written; second, offset to where the data (that does not
// fit in the offset field of the tag) is written. That is,
//
//  start dirEntries  -----------------------
//                      |                  |
//                      |                  |
//  dir Entry Offset -> |                  |
//                      |                  |
//                      |                  |
// end of dirEntries  -----------------------  start dir data
//                      |                  |
//                      |                  |
//  data offset ->      |                  |
//
// the start of the data segment is at the end of the
// entry segment. The size of the entry segment can be
// pre-computed and added to the offset of the start
// of the entry segment to get the offset to the start 
// of the data segment. Use the seek() method on ExifIO
// to set the respective offset for writing. The seek()
// method allows the offset to be set past the end of
// file... see the seek() implemetation in ExifIO.cpp.

void ExifIFD::writeNormalTag( ExifIO* exifio, ExifTagEntry* tagEntry )
{
    ExifDirEntry dEntry;
    dEntry.tag = tagEntry->getTagNum();
    dEntry.type = tagEntry->getType();
    dEntry.count = tagEntry->getCount();
    dEntry.offset = 0;

    switch( dEntry.type ) 
    {
        case EXIF_BYTE:
        {
            uint8* uint8Vec = new uint8[dEntry.count];
            if (dEntry.count == 1)
                *uint8Vec = ((ExifTagEntryT<uint8>*)tagEntry)->getValue();
            else
            {
                std::vector<uint8>& tagVec =
                    ((ExifTagEntryT<std::vector<uint8> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), uint8Vec, dEntry.count );
            }

            writeByteArray( exifio, dEntry, (int8*)uint8Vec);
            delete [] uint8Vec;

            break;
        }

        case EXIF_SBYTE:
        case EXIF_UNDEFINED:
        {
            int8* int8Vec = new int8[dEntry.count];
            if (dEntry.count == 1)
                *int8Vec = ((ExifTagEntryT<int8>*)tagEntry)->getValue();
            else
            {
                std::vector<int8>& tagVec =
                   ((ExifTagEntryT<std::vector<int8> >*)
                   tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), int8Vec, dEntry.count );
            }

            writeByteArray( exifio, dEntry, int8Vec );
            delete [] int8Vec;

            break;
        }
        
        case EXIF_SHORT:
        {
            uint16* uint16Vec = new uint16[dEntry.count];
            if (dEntry.count == 1)
                *uint16Vec = ((ExifTagEntryT<uint16>*)tagEntry)->getValue();
            else
            {
                std::vector<uint16>& tagVec =
                    ((ExifTagEntryT<std::vector<uint16> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), uint16Vec, dEntry.count );
            }

            writeShortArray( exifio, dEntry, (int16*)uint16Vec );
            delete [] uint16Vec;

            break;
        }
        
        case EXIF_SSHORT:
        {
            int16* int16Vec = new int16[dEntry.count];
            if (dEntry.count == 1)
                *int16Vec = ((ExifTagEntryT<int16>*)tagEntry)->getValue();
            else
            {
                std::vector<int16>& tagVec =
                    ((ExifTagEntryT<std::vector<int16> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), int16Vec, dEntry.count );
            }
            writeShortArray( exifio, dEntry, int16Vec );
            delete [] int16Vec;

            break;
        }
        
        case EXIF_LONG:
        {
            exif_uint32* exif_uint32Vec = new exif_uint32[dEntry.count];
            if (dEntry.count == 1)
               *exif_uint32Vec = ((ExifTagEntryT<exif_uint32>*)
               tagEntry)->getValue();
            else
            {
                std::vector<exif_uint32>& tagVec =
                    ((ExifTagEntryT<std::vector<exif_uint32> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), exif_uint32Vec, dEntry.count );
            }

            writeLongArray( exifio, dEntry, (exif_int32*)exif_uint32Vec );
            delete [] exif_uint32Vec;

            break;
        }
        
        case EXIF_SLONG:
        {
            exif_int32* exif_int32Vec = new exif_int32[dEntry.count];
            if (dEntry.count == 1)
                *exif_int32Vec = ((ExifTagEntryT<exif_int32>*)
                tagEntry)->getValue();
            else
            {
                std::vector<exif_int32>& tagVec =
                    ((ExifTagEntryT<std::vector<exif_int32> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), exif_int32Vec, dEntry.count );
            }
            writeLongArray( exifio, dEntry, exif_int32Vec );
            delete [] exif_int32Vec;
            break;
        }
        
        case EXIF_RATIONAL:
        {
            exif_uint32* rat = new exif_uint32[2*dEntry.count];

            if ( dEntry.count == 1 )
            {
                float fval = ((ExifTagEntryT<float>*)tagEntry)->getValue();
                rationalize( fval, *(rat), *(rat+1) );
            }          
            else
            {
                std::vector<float>& tagVec =
                    ((ExifTagEntryT<std::vector<float> >*)
                    tagEntry)->getValue();
                std::vector<float>::iterator crnt, end;
                crnt = tagVec.begin();
                end = tagVec.end();
                int j = 0;
                while( crnt != end )
                {
                    rationalize( *crnt, *(rat+j), *(rat+j+1) );
                    j += 2;
                    crnt++;
                }
            }

            writeRationalArray( exifio, dEntry, 
                reinterpret_cast<exif_int32*>(rat) );

            delete [] rat;

            break;
        }

        case EXIF_SRATIONAL:
        {
            exif_int32* rat = new exif_int32[2*dEntry.count];
            if ( dEntry.count == 1 )
            {
                bool setSign = false;
                float fval = ((ExifTagEntryT<float>*)tagEntry)->getValue();
                if ( fval < 0 )
                {
                    setSign = true;
                    fval = -fval;
                }

                rationalize( fval, *reinterpret_cast<exif_uint32*>((rat)), 
                             *reinterpret_cast<exif_uint32*>((rat+1)) );
                if( setSign )
                {
                    *rat = -*rat;
                }
            }          
            else
            {
                std::vector<float>& tagVec =
                    ((ExifTagEntryT<std::vector<float> >*)
                    tagEntry)->getValue();
                std::vector<float>::iterator crnt, end;
                crnt = tagVec.begin();
                end = tagVec.end();
                int j = 0;
                bool setSign;
                while( crnt != end )
                {
                    if ( *crnt < 0.0 )
                    {
                        setSign = true;
                        *crnt = -*crnt;
                    }
                    else
                        setSign = false;
                 
                    rationalize( *crnt, *reinterpret_cast<exif_uint32*>((rat+j)),
                                 *reinterpret_cast<exif_uint32*>((rat+j+1)) );

                    if ( setSign )
                        *(rat+j) = -*(rat+j);
                  
                    j += 2;
                    crnt++;
                }
            }

            writeRationalArray( exifio, dEntry, rat );
            delete [] rat;
            break;
        }

        case EXIF_FLOAT:
        {
            float* floatVec = new float[dEntry.count];
            if (dEntry.count == 1)
                *floatVec = ((ExifTagEntryT<float>*)tagEntry)->getValue();
            else
            {
                std::vector<float>& tagVec =
                    ((ExifTagEntryT<std::vector<float> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), floatVec, dEntry.count );
            }
            writeFloatArray( exifio, dEntry, floatVec);
            delete [] floatVec;
            break;
        }
        
        case EXIF_DOUBLE:
        {
            double* doubleVec = new double[dEntry.count];
          
            if ( dEntry.count == 1 )
                *doubleVec = ((ExifTagEntryT<double>*)tagEntry)->getValue();
            else
            {
                std::vector<double>& tagVec =
                    ((ExifTagEntryT<std::vector<double> >*)
                    tagEntry)->getValue();
                EXIF_COPY( tagVec.begin(), tagVec.end(), doubleVec, dEntry.count );
            }

            writeDoubleArray( exifio, dEntry, doubleVec );
            delete [] doubleVec;
            break;
        }
        
        case EXIF_ASCII:
        {
            int8* int8Vec = new int8[dEntry.count];
            std::string stng = ((ExifTagEntryT<std::string>*)
                tagEntry)->getValue();
            EXIF_COPY( stng.begin(), stng.end(), int8Vec, dEntry.count );
            int8Vec[dEntry.count - 1] = '\0';
            writeByteArray( exifio, dEntry, int8Vec );
            delete [] int8Vec;
            break;
        }
    }

    // write dir entry
    exifio->swabShort(&dEntry.tag);
    exifio->swabShort(&dEntry.type);
    exifio->swabLong(&dEntry.count);
    //dEntry.offset is already swabbed (if necessary) at this point
    exifio->seek( mEntryOffset, SEEK_SET );
    exifio->write( &dEntry, sizeof(ExifDirEntry) );
    mEntryOffset += sizeof(ExifDirEntry);

    //Need to swab back the tag and count values
    exifio->swabShort(&dEntry.tag);
    exifio->swabLong(&dEntry.count);
    if ( dEntry.tag == EXIFTAG_STRIPOFFSETS  )
    {
        if( dEntry.count == 1 )
            mOffsetToStripOffsets = mEntryOffset - sizeof(exif_int32) ;
        else
            mOffsetToStripOffsets = dEntry.offset + mWriteOffset ;
    }

    if ( dEntry.tag == EXIFTAG_STRIPBYTECOUNTS )
    {
        if( dEntry.count == 1 )
            mOffsetToStripByteCounts = mEntryOffset - sizeof(exif_int32) ;
        else
            mOffsetToStripByteCounts = dEntry.offset + mWriteOffset ;
    }  

    if ( dEntry.tag == EXIFTAG_JPEGIFOFFSET )
    {
        if( dEntry.count == 1 )
            mOffsetToJpegInterop = mEntryOffset - sizeof(exif_int32) ;
        else
            mOffsetToJpegInterop = dEntry.offset + mWriteOffset ;
    }

    if ( dEntry.tag == EXIFTAG_JPEGIFBYTECOUNT )
    {
        if( dEntry.count == 1 )
            mOffsetToJpegByteCount = mEntryOffset - sizeof(exif_int32) ;
        else
            mOffsetToJpegByteCount = dEntry.offset + mWriteOffset ;
    }


    return;
}




void ExifIFD::writeSubIFDArray( ExifIO* exifio, ExifTagEntry *tagEntry,
                               ExifIFDVec &dirVec )
{
    ExifDirEntry dirEntry;
    dirEntry.tag = tagEntry->getTagNum();
    dirEntry.type = tagEntry->getType();
    dirEntry.count = tagEntry->getCount();
    dirEntry.offset = 0;

    exifoff_t multiIFDDataOffsetLoc = 0 ;
    if (dirEntry.count > 1)
    {
        exif_int32* exif_int32Vec = new exif_int32[dirEntry.count];
    
        multiIFDDataOffsetLoc = mDataOffset ;
        
        // Offset to data is relative to the TiffHeader, not the
        // beginning of the file, so subtract off the extra offset
        // due to the exif header.
        dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset;
        // No reason to swab, these aren't real numbers        
        // exifio->swabArrayOfLong( exif_int32Vec, dirEntry.count );
        exifio->write(exif_int32Vec, dirEntry.count*sizeof(exif_int32));
        mDataOffset = exifio->seek(0, SEEK_END);
        // make sure data offset is on word boundary with respect to the Tiff
        // header
        if ( ((mDataOffset-mWriteOffset)%2) == 1 )
            mDataOffset++;
    }
    else
    {
        // The real value will be written by the sub ifd when it's being
        // written
        dirEntry.offset = 0 ;
    }


    // write dir entry
    exifio->swabShort(&dirEntry.tag);
    exifio->swabShort(&dirEntry.type);
    exifio->swabLong(&dirEntry.count);
    exifio->swabLong(&dirEntry.offset);
    exifio->seek( mEntryOffset, SEEK_SET );
    exifio->write( &dirEntry, sizeof(ExifDirEntry) );

    mEntryOffset += sizeof(ExifDirEntry);

    //dirEntry.count was just swabbed, need to swab back to check its value
    exifio->swabLong(&dirEntry.count);
    if (dirEntry.count == 1)
        dirVec[0]->setDataOffset(  mEntryOffset - sizeof(exif_uint32) ) ;
    else
    {
        for (unsigned int i=0; i<dirVec.size(); i++)
        {
            dirVec[i]->setDataOffset( multiIFDDataOffsetLoc ) ;
            multiIFDDataOffsetLoc += sizeof(exif_int32) ;
        }
    }

    return ;
}





void ExifIFD::writeByteArray(ExifIO* exifio, ExifDirEntry& dirEntry, int8* dp)
{
    if (dirEntry.count > 4)
    {
        // Size of value is too large to fit in the 4 byte
        // length of the entry's offset. So, write it out
        // somewhere else, and store the offset to the value
        // in ExifDirEntry's offset element.
        dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset;
        exifio->write(dp, dirEntry.count);
        mDataOffset = exifio->seek(0, SEEK_END);
        // make sure data offset is on word boundary with respect to the Tiff
        // header
        if ( ((mDataOffset-mWriteOffset)%2) == 1 )
            mDataOffset++;

        exifio->swabLong(&dirEntry.offset) ;
    }
    else
    {
        // Copy directly into the offset element of
        // the ExifDirEntry structure since it will
        // fit.
        memcpy(&dirEntry.offset, dp, dirEntry.count);
    }
}


void ExifIFD::writeShortArray( ExifIO* exifio, ExifDirEntry& dirEntry, 
                              int16* dp )
{
    if (dirEntry.count > 2)
    {
        dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset;
        exifio->swabArrayOfShort((uint16*)dp, dirEntry.count);
        exifio->write(dp, dirEntry.count*sizeof(int16));
        mDataOffset = exifio->seek(0, SEEK_END);
        // make sure data offset is on word boundary with respect to the Tiff
        // header
        if ( ((mDataOffset-mWriteOffset)%2) == 1 )
            mDataOffset++;

        exifio->swabLong(&dirEntry.offset) ;
    }
    else
    {
        if ( HOST_BIGENDIAN )
        {
            dirEntry.offset = (exif_uint32) ((int) dp[0] << 16);
            if (dirEntry.count == 2)
                dirEntry.offset |= dp[1] & 0xffff;
        }
        else
        {
            dirEntry.offset =  dp[0] & 0xffff;
            if (dirEntry.count == 2)
                dirEntry.offset |= (int) dp[1] << 16;
        }

        exifio->swabArrayOfShort((unsigned short *)&dirEntry.offset,
            dirEntry.count) ;
    }
}

void ExifIFD::writeLongArray( ExifIO* exifio, ExifDirEntry& dirEntry,
                             exif_int32* dp )
{
    if (dirEntry.count > 1)
    {
        dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset;
        exifio->swabArrayOfLong((exif_uint32*)dp, dirEntry.count);
        exifio->write(dp, dirEntry.count*sizeof(exif_int32));
        mDataOffset = exifio->seek(0, SEEK_END);
        // make sure data offset is on word boundary with respect to the Tiff
        // header
        if ( ((mDataOffset-mWriteOffset)%2) == 1 )
            mDataOffset++;
    }
    else
    {
        dirEntry.offset = dp[0];
    }

    exifio->swabLong(&dirEntry.offset);
}

void ExifIFD::writeRationalArray( ExifIO* exifio, ExifDirEntry& dirEntry,
                                 exif_int32* dp )
{
    dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset ;
    exifio->swabArrayOfLong((exif_uint32*)dp, 2*dirEntry.count);
    exifio->write(dp, 2*dirEntry.count*sizeof(exif_int32));
    mDataOffset = exifio->seek(0, SEEK_END);
    // make sure data offset is on word boundary with respect to the Tiff header
    if ( ((mDataOffset-mWriteOffset)%2) == 1 )
        mDataOffset++;

    exifio->swabLong(&dirEntry.offset);
}

void ExifIFD::writeFloatArray( ExifIO* exifio, ExifDirEntry& dirEntry,
                              float* dp )
{
    if (dirEntry.count > 1)
    {
        dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset;
        exifio->swabArrayOfLong((exif_uint32*)dp, dirEntry.count);
        exifio->write(dp, dirEntry.count*sizeof(float));
        mDataOffset = exifio->seek(0, SEEK_END);
        // make sure data offset is on word boundary with respect to the Tiff header
        if ( ((mDataOffset-mWriteOffset)%2) == 1 )
            mDataOffset++;
    }
    else
    {
        dirEntry.offset = *(exif_uint32*)&dp[0];
    }

    exifio->swabLong(&dirEntry.offset);
}

void ExifIFD::writeDoubleArray( ExifIO* exifio, ExifDirEntry& dirEntry,
                               double* dp )
{
    dirEntry.offset = exifio->seek( mDataOffset, SEEK_SET ) - mWriteOffset ;
//  EXIFCvtNativeToIEEEDouble(tif, n, v);
    exifio->swabArrayOfDouble(dp, dirEntry.count);
    exifio->write(dp, dirEntry.count*sizeof(double));
    mDataOffset = exifio->seek(0, SEEK_END);
    // make sure data offset is on word boundary with respect to the Tiff header
    if ( ((mDataOffset-mWriteOffset)%2) == 1 )
        mDataOffset++;

    exifio->swabLong(&dirEntry.offset);
}

ExifStatus ExifIFD::rationalize( const double& fpNum, exif_uint32& numer,
                                exif_uint32& denom, const double error )
{
    // This algorithm will not work with negative floating point
    // numbers - so record the sign and set the calculated numerator's
    // sign at end.
    if( fpNum != 0.0 )
    {
        
        if ( fpNum < 0.0 || fpNum < 3.0e-10 || fpNum > 4.0e+10 || error < 0.0 )
            return EXIF_CANNOT_RATIONALIZE_NUMBER ;
    }
    
  
    exif_uint32 D, N;
    double epsilon = 0.0;

    denom = D = 1;
    numer = static_cast<int>(fpNum);
    N = numer + 1;

    while( true )
    {
        double r = 0.0;
        if ( fpNum * (denom) != static_cast<double>(numer) )
        {
            r = (N - fpNum * D)/( (fpNum * denom) - numer);
            if (r <= 1.0)
            {
                unsigned int t = N;
                N = numer;
                numer = t;
                t = D;
                D = denom;
                denom = t;
            }
        }
        else
            break;
      
        epsilon = fabs( 1.0 - ( numer / ( fpNum * denom )) );
        if (epsilon <= error)
            break;

        double m = 10.0;
        while( m * epsilon < 1.0)
            m *= 10.0;

        epsilon = 1.0/m * ((int)(0.5 + m*epsilon));

        if (epsilon <= error)
            break;
 
        if ( r < 1.0 )
            r = 1.0/r;
      
        N += numer * static_cast<unsigned int>(r);
        D += denom * static_cast<unsigned int>(r);
        numer += N;
        denom += D;
    }
    return EXIF_OK ;
}

int ExifIFD::readDir( ExifIO* exifio, exifoff_t offset, ExifDirEntry*& dirEntries,
                     ExifStatus& errRtn )
{
    errRtn = EXIF_OK ;
    
    uint16 dircount;

    exifio->seek( offset, SEEK_SET );
    exifio->read( &dircount, sizeof(uint16) );
    exifio->swabShort( &dircount );
	
    // if there are too many entries, they can't fit all in an AppSegment,
    // so this is a bad IFD. (note: 0x1FFF > 64k/12, really we're allowing
    // a little too much here...
    if (dircount > 0x1FFF)
    {
        errRtn = EXIF_FILE_READ_ERROR ;
        return 0 ;
    }
  
	if (dircount > 0)
	{
	    dirEntries = new ExifDirEntry[dircount] ;

	    if ( (unsigned)(exifio->read(dirEntries, dircount*sizeof(ExifDirEntry))) !=
	         dircount*sizeof(ExifDirEntry) )
	    {
	        delete [] dirEntries;
	        dirEntries = NULL;
	        errRtn = EXIF_FILE_READ_ERROR ;
	        return 0 ;
	    }

	    for (int i=0; i<dircount; i++)
	    {
	        exifio->swabShort(&(dirEntries[i].tag));
	        exifio->swabShort(&(dirEntries[i].type));
	        exifio->swabLong(&(dirEntries[i].count));
	        exifio->swabLong(&(dirEntries[i].offset));
	    }
	}

    // Read offset to next directory for sequential scans.
    exifio->read( &mNextDirOffset, sizeof(exif_uint32) );
    exifio->swabLong(reinterpret_cast<exif_uint32*>(&mNextDirOffset));

    return dircount;
}


/*
 * Read the next EXIF directory from a file
 * and convert it to the internal format.
 * We read directories sequentially.
 */
ExifStatus ExifIFD::readDirTree( ExifIO* exifio )
{
    exifio->flags() &= ~EXIF_BEENWRITING;   /* reset before new dir */

    if (mOffset == 0)
        return EXIF_OK ;

    ExifDirEntry* dirEntries;
    ExifStatus errRtn ;

    int dircount = readDir( exifio, mOffset, dirEntries, errRtn );

    if( errRtn != EXIF_OK)// || dircount == 0 )
        return errRtn ;

	if (0 == dircount)
		return EXIF_OK;

  
    ExifDirEntry* dp;
    int n;
    ExifTagEntry* tagEntry;
    for( dp = dirEntries, n = dircount; n > 0; n--, dp++ ) 
    {
        tagEntry = fetchNormalTag( exifio, dp );
        if ( tagEntry != NULL )
        {           
            setGenericTag( *tagEntry );
            delete tagEntry;
        }
    }


    // Check if there is an image present
    if ( mCache->mCompression != NULL && bLoadImage) 
	{
        // Got an Image - is it compressed?
        if(( compression( ) == EXIF_COMPRESSION_NONE ) && 
                ( mCache->mImageWidth != NULL ) &&
                ( mCache->mImageLength != NULL ) )
        {
            ExifStatus status ;
                
            ExifStripImage unCompImg(status, this, exifio);

            ExifImageInfo imgInfo ;

            mImage = new ExifImageDesc ;

            imgInfo.width = imageWidth() ;
            imgInfo.height = imageLength() ;
            imgInfo.numChannels = samplesPerPixel() ;
            if ( imgInfo.width*imgInfo.height*imgInfo.numChannels <= 0xFFF0 )
            {
                exifAllocImageDesc( imgInfo, *mImage ) ;
                if (unCompImg.readImage(*mImage)!=EXIF_OK)
                {
                    // Error reading in image
                    exifDeleteImageDesc( *mImage ) ;
                    delete mImage ;
                    mImage = NULL ;
                }
            }
            else
            {
                // It is not possible to have a thumbnail bigger than 64K
                // (0xFFFF) because that is the maximum size of an application
                // segment. File is corrupted.
                mImage = NULL;
                
            }
        }
        else if (( compression( ) == EXIF_COMPRESSION_JPEG ) &&
            ( mCache->mJpegInterchange != NULL ) &&
            ( mCache->mJpegInterchangeLength != NULL ) )
        {
            // Compressed
            exif_uint32 offset ;
            offset = jpegInterchange() ;
                
            mCompressedThumbLength = jpegInterchangeLength() ;
            if ( mCompressedThumbLength < 0xFFFF && mCompressedThumbLength > 0 )
            {
                mCompressedThumb = new uint8[mCompressedThumbLength] ;
                exifio->seek( offset + mExifOffset, SEEK_SET ) ;
                if( exifio->read( mCompressedThumb, mCompressedThumbLength ) !=
                    (exif_int32) mCompressedThumbLength )
                {
                    // There was problems reading in the image (thumbnail).
                    // We probably have a bad file (invalid).
                    mCompressedThumb = NULL;
                    mCompressedThumbLength = 0;
                }
            }
            else
            {
                // It is not possible to have a thumbnail bigger than 64K
                // (0xFFFF) because that is the maximum size of an application
                // segment. It also cannot be 0 or less. File is corrupted.
                mCompressedThumb = NULL;
                mCompressedThumbLength = 0;
            }
    
        }
    }
    
    delete [] dirEntries;
  
    SubIFDIter crntSubIFD, endSubIFD;
    crntSubIFD = mSubDirs.begin();
    endSubIFD = mSubDirs.end();
    while( crntSubIFD != endSubIFD )
    {
        ExifIFDVec dirVec = (*crntSubIFD).second;
        for (unsigned int i=0; i<dirVec.size(); i++)
        {
			dirVec[i]->setLoadImage(bLoadImage); // pog
            if( (errRtn = dirVec[i]->readDirTree( exifio )) != EXIF_OK )
            {
                // HERE HERE - call deleteItems()???
                //return errRtn ;
				continue; // pog
            }
        }

        crntSubIFD++;
    }
  
    return EXIF_OK;
}

void ExifIFD::deleteItems( void )
{
    if ( mCache )
    {
        delete mCache ;
        mCache = NULL ;
    }
    
    EntryIter crntEntry, endEntry;
    crntEntry = mTagEntries.begin();
    endEntry = mTagEntries.end();

    while( crntEntry != endEntry )
    {
        delete (*crntEntry).second;
        crntEntry++ ;
    }
    mTagEntries.clear();
  
    SubIFDIter crntSubIFD, endSubIFD;
    crntSubIFD = mSubDirs.begin();
    endSubIFD = mSubDirs.end();
    while( crntSubIFD != endSubIFD )
    {
        ExifIFDVec dirVec = (*crntSubIFD).second ;
        for (unsigned int i=0; i<dirVec.size(); i++)
        {
            delete dirVec[i];
        }
        dirVec.clear();
        crntSubIFD++;
    }
    mSubDirs.clear();
  
    if (mImage)
    {
        exifDeleteImageDesc( *mImage ) ;
        delete mImage ;
        mImage = NULL ;
    }

    if (mCompressedThumb)
    {
        delete [] mCompressedThumb;
        mCompressedThumb = NULL;
    }
    
    return;
}

void ExifIFD::addSubIFD( ExifTagEntry* tagEntry )
{
    // set subIFDs
    if ( tagEntry->getCount() > 1 )
    {

        ExifTagEntryT<std::vector<exif_uint32> >* offsetVec = 
            dynamic_cast<ExifTagEntryT<std::vector<exif_uint32> >*>(tagEntry);
      
        std::vector<exif_uint32>::iterator crnt, end;
        crnt = offsetVec->getValue().begin();
        end = offsetVec->getValue().end();
        int16 idx = 0;
        while( crnt != end )
        {
            ExifIFD* subDir = new ExifIFD( tagEntry->getTagNum(), 
                                           (*crnt)+mExifOffset, mExifOffset,
                                           idx , mIFDmap, this);
            addSubIFD( subDir );
            crnt++;
            idx++;
        }
    }
    else
    {
        ExifTagEntryT<exif_uint32>* offset = dynamic_cast<ExifTagEntryT<exif_uint32>*>
            (tagEntry);
        ExifIFD* subDir = new ExifIFD( tagEntry->getTagNum(),  
                                       offset->getValue()+mExifOffset,
                                       mExifOffset, 0 , mIFDmap, this);
        addSubIFD( subDir );
    }

    return;
}

/*
 * Fetch a tag that is not handled by special case code.
 */
ExifTagEntry* ExifIFD::fetchNormalTag( ExifIO* exifio, ExifDirEntry* dp )
{

    ExifTagEntry* tagEntry = NULL;
    
    // This is the size of each tag type in bytes (in order of the ExifDataType)
    static const int TAGSize[] = { 0, 1, 1, 2, 4, 8, 2, 1, 2, 4, 8, 4, 8 };
    
    // Now, we need to do some reality checks....
    if ((dp->count > 0) && 
        ( dp->type < EXIF_MAXTYPE ) ) 
    {
        // the total data size used in bytes
        unsigned int dataSize = dp->count * TAGSize[dp->type];
    
        if (( dataSize <= 4 ) || 
            (( dataSize + dp->offset ) < (unsigned int)exifio->size() ) )
        {
        // switch on the data type and read the data
        switch( dp->type ) 
        {
        case EXIF_BYTE:
        {
            uint8* uint8Vec = reinterpret_cast<uint8*>( fetchByteArray( exifio,
                dp ) );
            if ( dp->count == 1 )
                tagEntry = new ExifTagEntryT<uint8>(dp->tag, dp->type,
                dp->count, *uint8Vec);
            else
                tagEntry = new ExifTagEntryT<std::vector<uint8> >(dp->tag, dp->type,
                dp->count, std::vector<uint8>(uint8Vec, (uint8Vec+dp->count)));
            delete [] uint8Vec;
            break;
        }
        

        case EXIF_SBYTE:
        {
            int8* int8Vec = fetchByteArray( exifio, dp );
            if ( dp->count == 1 )
                tagEntry = new ExifTagEntryT<int8>(dp->tag, dp->type,
                dp->count, *int8Vec);
            else
                tagEntry = new ExifTagEntryT<std::vector<int8> >(dp->tag, dp->type,
                dp->count, std::vector<int8>(int8Vec, (int8Vec+dp->count)));
            delete [] int8Vec;
            break;
        }
        
        case EXIF_SHORT:
        {
            uint16* uint16Vec = reinterpret_cast<uint16*>( 
                fetchShortArray( exifio, dp ) );
            // Some tag can be SHORT or LONG. For consistence reason,
            // we represent them as LONG in ExifTagEntry.
            if (canBeSHORTorLONG(dp->tag))
            {
                exif_uint32* exif_uint32Vec = new exif_uint32[dp->count];
                for (unsigned int i=0; i<dp->count; i++)
                    exif_uint32Vec[i] = (exif_uint32)(uint16Vec[i]);
                if ( dp->count == 1)
                    tagEntry = new ExifTagEntryT<exif_uint32>(dp->tag, EXIF_LONG,
                    dp->count, *exif_uint32Vec);
                else
                    tagEntry = new ExifTagEntryT<std::vector<exif_uint32> >(dp->tag,
                    EXIF_LONG, dp->count, std::vector<exif_uint32>(exif_uint32Vec, (exif_uint32Vec+dp->count)));
                delete[] exif_uint32Vec;
            }
            else
            {
                if ( dp->count == 1)
                    tagEntry = new ExifTagEntryT<uint16>(dp->tag, dp->type,
                    dp->count, *uint16Vec);
                else
                    tagEntry = new ExifTagEntryT<std::vector<uint16> >(dp->tag,
                    dp->type, dp->count, std::vector<uint16>(uint16Vec,
                    (uint16Vec+dp->count)));
            }
            delete [] uint16Vec;
            break;
        }
        
        case EXIF_SSHORT:
        {
            int16* int16Vec = fetchShortArray( exifio, dp );
            if ( dp->count == 1 )
                tagEntry = new ExifTagEntryT<int16>(dp->tag, dp->type,
                dp->count, *int16Vec);
            else
                tagEntry = new ExifTagEntryT<std::vector<int16> >(dp->tag,
                dp->type, dp->count, std::vector<int16>(int16Vec,
                (int16Vec+dp->count)));
            delete [] int16Vec;
            break;
        }
        
        case EXIF_LONG:
        {
            exif_uint32* exif_uint32Vec = reinterpret_cast<exif_uint32*>( 
                fetchLongArray( exifio, dp ) );
            if ( dp->count == 1)
                tagEntry = new ExifTagEntryT<exif_uint32>(dp->tag, dp->type,
                dp->count, *exif_uint32Vec);
            else
                tagEntry = new ExifTagEntryT<std::vector<exif_uint32> >(dp->tag,
                dp->type, dp->count, std::vector<exif_uint32>(exif_uint32Vec,
                (exif_uint32Vec+dp->count)));
            delete [] exif_uint32Vec;
            break;
        }
        
        case EXIF_SLONG:
        {
            exif_int32* exif_int32Vec = fetchLongArray( exifio, dp );
            if ( dp->count == 1 )
                tagEntry = new ExifTagEntryT<exif_int32>(dp->tag, dp->type,
                dp->count, *exif_int32Vec);
            else
                tagEntry = new ExifTagEntryT<std::vector<exif_int32> >(dp->tag,
                dp->type, dp->count, std::vector<exif_int32>(exif_int32Vec,
                (exif_int32Vec+dp->count)));
            delete [] exif_int32Vec;
            break;
        }
        
        case EXIF_RATIONAL:
        {
            ExifStatus err ;
            float* floatVec = fetchRationalArray( exifio, dp, err );
            if (err == EXIF_OK)
            {
                if ( dp->count == 1 )
                    tagEntry = new ExifTagEntryT<float>(dp->tag, dp->type,
                    dp->count, *floatVec);
                else
                    tagEntry = new ExifTagEntryT<std::vector<float> >(dp->tag,
                    dp->type, dp->count, std::vector<float>(floatVec,
                    (floatVec+dp->count)));
                delete [] floatVec;
            }
            break;
        }
        case EXIF_SRATIONAL:
        {
            ExifStatus err ;
            float* floatVec = fetchSRationalArray( exifio, dp, err );
            if (err == EXIF_OK)
            {
                if ( dp->count == 1 )
                    tagEntry = new ExifTagEntryT<float>(dp->tag, dp->type,
                    dp->count, *floatVec);
                else
                    tagEntry = new ExifTagEntryT<std::vector<float> >(dp->tag,
                    dp->type, dp->count, std::vector<float>(floatVec,
                    (floatVec+dp->count)));
                delete [] floatVec;
            }
            break;
        }
        
        case EXIF_FLOAT:
        {
            float* floatVec = fetchFloatArray( exifio, dp );
            if ( dp->count == 1 )
                tagEntry = new ExifTagEntryT<float>(dp->tag, dp->type,
                dp->count, *floatVec);
            else
                tagEntry = new ExifTagEntryT<std::vector<float> >(dp->tag,
                dp->type, dp->count, std::vector<float>(floatVec, (floatVec+dp->count)));
            delete [] floatVec;
            break;
        }
        
        case EXIF_DOUBLE:
        {
            double* doubleVec = fetchDoubleArray( exifio, dp );
            if ( dp->count == 1 )
                tagEntry = new ExifTagEntryT<double>(dp->tag, dp->type,
                dp->count, *doubleVec);
            else
                tagEntry = new ExifTagEntryT<std::vector<double> >(dp->tag,
                dp->type, dp->count, std::vector<double>(doubleVec,
                (doubleVec+dp->count)));
            delete [] doubleVec;
            break;
        }
        
        case EXIF_ASCII:
        {
            int8* int8Vec = fetchByteArray( exifio, dp );
            
            // if the string isn't NULL terminated, the last character is probably
            // a valid character, so we don't want to wipe it out.
            if (int8Vec[dp->count-1] != 0) dp->count++;
            tagEntry = new ExifTagEntryT<std::string>(dp->tag, dp->type, dp->count,
                std::string(int8Vec, dp->count-1));
            delete [] int8Vec;
            break;
        }
        
        case EXIF_UNDEFINED:
        {
            int8* int8Vec = fetchByteArray( exifio, dp ) ;
        if (dp->count == 1)
                tagEntry = new ExifTagEntryT<int8>(dp->tag, dp->type,
                dp->count, *int8Vec);
        else
            tagEntry = new ExifTagEntryT<std::vector<int8> >(dp->tag, dp->type,
            dp->count, std::vector<int8>(int8Vec, (int8Vec+dp->count)));
            delete [] int8Vec;
            break;
        }
        }
        }
    }

    return( tagEntry );
}

/*
 * Fetch a contiguous directory item.
 */
void ExifIFD::fetchData( ExifIO* exifio, exifoff_t offset, int8* dataPtr,
                        exif_uint32 byteSize )
{

    exifio->seek( offset + mExifOffset, SEEK_SET );
    exifio->read( dataPtr, byteSize );
  
    return;
}

/*
 * Fetch an array of BYTE or SBYTE values.
 */
int8* ExifIFD::fetchByteArray( ExifIO* exifio, ExifDirEntry* dir )
{
    int8* vec = new int8[ dir->count ];
  
    if ( dir->count < 5 ) 
    {
        // Extract data from offset field.
        if ( exifio->bigEndian() ) 
        {
            switch( dir->count ) 
            {
                case 4: vec[3] = (int8) (dir->offset & 0xff);
                case 3: vec[2] = (int8) ((dir->offset >> 8) & 0xff);
                case 2: vec[1] = (int8) ((dir->offset >> 16) & 0xff);
                case 1: vec[0] = (int8) (dir->offset >> 24);
            }
        } 
        else 
        {
            switch( dir->count ) 
            {
                case 4: vec[3] = (int8) (dir->offset >> 24);
                case 3: vec[2] = (int8) ((dir->offset >> 16) & 0xff);
                case 2: vec[1] = (int8) ((dir->offset >> 8) & 0xff);
                case 1: vec[0] = (int8) (dir->offset & 0xff);
            }
        }
    } 
    else
        fetchData( exifio, dir->offset, vec, dir->count );

    return vec;
}

/*
 * Fetch an array of SHORT or SSHORT values.
 */
int16* ExifIFD::fetchShortArray( ExifIO* exifio, ExifDirEntry* dir )
{
    int16* vec = new int16[dir->count];
  
    if ( dir->count <= 2 ) 
    {
        if ( exifio->bigEndian() ) 
        {
            switch (dir->count) 
            {
                case 2: vec[1] = (int16) (dir->offset & 0xffff);
                case 1: vec[0] = (int16) (dir->offset >> 16);
            }
        } 
        else 
        {
            switch (dir->count) 
            {
                case 2: vec[1] = (int16) (dir->offset >> 16);
                case 1: vec[0] = (int16) (dir->offset & 0xffff);
            }
        }
    } 
    else
    {
        fetchData( exifio, dir->offset, reinterpret_cast<int8*>(vec),
            dir->count*sizeof(int16) );
        exifio->swabArrayOfShort(  reinterpret_cast<uint16*>(vec),
            dir->count );
    }
  
    return vec;
}

/*
 * Fetch an array of LONG or SLONG values.
 */
exif_int32* ExifIFD::fetchLongArray( ExifIO* exifio, ExifDirEntry* dir )
{
    exif_int32* vec = new exif_int32[dir->count];
  
    if ( dir->count == 1 ) 
        vec[0] = dir->offset;
    else
    {
        fetchData( exifio, dir->offset, reinterpret_cast<int8*>(vec),
            dir->count*sizeof(exif_int32) );
        exifio->swabArrayOfLong( reinterpret_cast<exif_uint32*>(vec), dir->count);
    }

    return vec;
}

/*
 * Convert numerator+denominator to float.
 */
ExifStatus ExifIFD::convertRational( ExifDirEntry* dir, exif_uint32 num,
                                    exif_uint32 denom, float* rv)
{
	if (num == 0) {
		*rv = (float)0.0;
		return EXIF_OK;
	}

    if (denom == 0) 
        return EXIF_RATIONAL_HAS_ZERO_DENOMINATOR;
    else 
    {
        if ( dir->type == EXIF_RATIONAL )
            *rv = static_cast<float>(num) / static_cast<float>(denom);
        else
            *rv = static_cast<float>(static_cast<exif_int32>(num)) /
                static_cast<float>(static_cast<exif_int32>(denom));
    }

    return EXIF_OK ;
}

/*
 * Convert numerator+denominator to float.
 */
ExifStatus ExifIFD::convertSRational( ExifDirEntry* dir, exif_int32 num, 
                                     exif_int32 denom, float* rv)
{
    if (denom == 0) 
        return EXIF_RATIONAL_HAS_ZERO_DENOMINATOR;
    else 
    {
        if ( dir->type == EXIF_SRATIONAL )
            *rv = static_cast<float>(num) / static_cast<float>(denom);
        else
            *rv = static_cast<float>(static_cast<exif_int32>(num)) /
                static_cast<float>(static_cast<exif_int32>(denom));
    }

    return EXIF_OK ;
}

/*
 * Fetch an array of RATIONAL
 */
float* ExifIFD::fetchRationalArray( ExifIO* exifio, ExifDirEntry* dir,
                                   ExifStatus& errRtn )
{
    errRtn = EXIF_OK ;
    float* vec = new float[dir->count];
    exif_uint32* l = new exif_uint32[2*dir->count];
    
    fetchData( exifio, dir->offset, reinterpret_cast<int8*>(l),
        2*dir->count*sizeof(exif_uint32) );

    exifio->swabArrayOfLong( l, 2*dir->count);

    exif_uint32 i;
    for (i = 0; i < dir->count; i++)
        if ( (errRtn = convertRational( dir, l[2*i+0], l[2*i+1],
            (vec+i))) != EXIF_OK )
        {
            delete [] vec;
            delete [] l;
            return NULL ;
        }
        
    delete [] l;
    return vec;
}

/*
 * Fetch an array of SRATIONAL
 */
float* ExifIFD::fetchSRationalArray( ExifIO* exifio, ExifDirEntry* dir,
                                    ExifStatus& errRtn )
{
    errRtn = EXIF_OK ;
    float* vec = new float[dir->count];
    exif_int32* l = new exif_int32[2*dir->count];
    
    fetchData( exifio, dir->offset, reinterpret_cast<int8*>(l),
        2*dir->count*sizeof(exif_uint32) );

    exifio->swabArrayOfLong( (exif_uint32 *)l, 2*dir->count);

    exif_uint32 i;
    for (i = 0; i < dir->count; i++)
        if ( (errRtn = convertSRational( dir, l[2*i+0], l[2*i+1],
            (vec+i))) != EXIF_OK )
        {
            delete [] vec;
            delete [] l;
            return NULL ;
        }
        
    delete [] l;
    return vec;
}

/*
 * Fetch an array of FLOAT values.
 */
float* ExifIFD::fetchFloatArray( ExifIO* exifio, ExifDirEntry* dir )
{
    float* vec = new float[dir->count];

    if (dir->count == 1) 
        vec[0] = *(float*) &dir->offset;
    else
        fetchData( exifio, dir->offset, reinterpret_cast<int8 *>(vec),
        dir->count*sizeof(float) );

    exifio->swabArrayOfLong( reinterpret_cast<exif_uint32*>(vec), dir->count);
  
    return vec;
}

/*
 * Fetch an array of DOUBLE values.
 */
double* ExifIFD::fetchDoubleArray( ExifIO* exifio, ExifDirEntry* dir )
{
    double* vec = new double[dir->count];

    fetchData( exifio, dir->offset, reinterpret_cast<int8*>(vec),
        dir->count*sizeof(double) );

    exifio->swabArrayOfDouble( vec, dir->count );
  
    return vec;
}

ExifImageDesc* ExifIFD::getImage()
{
    if( mImage == NULL && mCompressedThumb != NULL )
    {
        ExifJpegImage compImg ;

        compImg.open( mCompressedThumb, mCompressedThumbLength, "r") ;
                
        ExifImageInfo imgInfo ;
        exif_uint32 nextLine ;
        exif_uint32 linesDone ;
        mImage = new ExifImageDesc ;

        compImg.getImageInfo( imgInfo ) ;
        exifAllocImageDesc( imgInfo, *mImage ) ;

        compImg.readScanLines( *mImage, mImage->numberOfRows, nextLine,
            linesDone ) ;
        compImg.close() ;
    }
    return mImage;
}

void ExifIFD::setImage( ExifImageDesc* img, bool compressIt )
{
    if ( mCompressedThumb != NULL )
    {
        delete [] mCompressedThumb ;
        mCompressedThumb = NULL ;
    }

    if ( mImage )
        delete mImage ;

    mImage = img ;

    ExifTagEntryT<exif_uint32>* tagUexif_int32 = NULL ;
    
    ExifTagEntry* tag = getGenericTag( EXIFTAG_COMPRESSION ) ;
    if ( tag == NULL )
    {
        tag = new ExifTagEntryT<uint16>( EXIFTAG_COMPRESSION, EXIF_SHORT, 1,
            compressIt?EXIF_COMPRESSION_JPEG:EXIF_COMPRESSION_NONE );
        setGenericTag( tag ) ;
    }
    else
    {
        ExifTagEntryT<uint16>* tagUint16 = 
            dynamic_cast<ExifTagEntryT<uint16>*>(tag) ;
        tagUint16->setValue( 
            compressIt?EXIF_COMPRESSION_JPEG:EXIF_COMPRESSION_NONE ) ;
    }
    
    tag = getGenericTag( EXIFTAG_XRESOLUTION ) ;
    if ( tag == NULL )
    {
        tag = new ExifTagEntryT<float>
            ( EXIFTAG_XRESOLUTION, EXIF_RATIONAL, 1, 72.0 );
        setGenericTag( tag ) ;
    }

    tag = getGenericTag( EXIFTAG_YRESOLUTION ) ;
    if ( tag == NULL )
    {
        tag = new ExifTagEntryT<float>
            ( EXIFTAG_YRESOLUTION, EXIF_RATIONAL, 1, 72.0 );
        setGenericTag( tag ) ;
    }

    tag = getGenericTag( EXIFTAG_RESOLUTIONUNIT ) ;
    if ( tag == NULL )
    {
        tag = new ExifTagEntryT<uint16>
            ( EXIFTAG_RESOLUTIONUNIT, EXIF_SHORT, 1, RESUNIT_INCH );
        setGenericTag( tag ) ;
    }


    if( !compressIt )
    {
        tag = getGenericTag( EXIFTAG_IMAGEWIDTH ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<exif_uint32>
                ( EXIFTAG_IMAGEWIDTH, EXIF_LONG, 1, mImage->numberOfColumns );
            setGenericTag( tag ) ;
        }
        else
        {
            ExifTagEntryT<exif_uint32>* tagUexif_int32 = 
                dynamic_cast<ExifTagEntryT<exif_uint32>*>(tag) ;
            tagUexif_int32->setValue( mImage->numberOfColumns ) ;
        }
        
        
        tag = getGenericTag( EXIFTAG_IMAGELENGTH ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<exif_uint32>
                ( EXIFTAG_IMAGELENGTH, EXIF_LONG, 1, mImage->numberOfRows );
            setGenericTag( tag ) ;
        }
        else
        {
            tagUexif_int32 = dynamic_cast<ExifTagEntryT<exif_uint32>*>(tag) ;
            tagUexif_int32->setValue( mImage->numberOfRows ) ;
        }
        

        // Default is to store the entire image in one strip
        tag = getGenericTag( EXIFTAG_ROWSPERSTRIP ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<exif_uint32>
                ( EXIFTAG_ROWSPERSTRIP, EXIF_LONG, 1, mImage->numberOfRows );
            setGenericTag( tag ) ;
        }


        tag = getGenericTag( EXIFTAG_BITSPERSAMPLE ) ;
        uint16 bitsPerSample = 8 ;
        switch( mImage->components[0].myColorType.myDataType )
        {
            case UNSIGNED_BYTE:
            case SIGNED_BYTE:
                bitsPerSample = 8 ;
                break ;
            case UNSIGNED_SHORT:
            case SIGNED_SHORT:
                bitsPerSample = 16 ;
                break ;
            case FLOAT_TYPE:
            case DOUBLE_TYPE:
            default:
                bitsPerSample = 16 ;
                break ;
        }
        std::vector<uint16> bpsVec(mImage->numberOfComponents, bitsPerSample) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<std::vector<uint16> >
                ( EXIFTAG_BITSPERSAMPLE, EXIF_SHORT, bpsVec.size(), bpsVec );
            setGenericTag( tag ) ;
        }
        else
        {
            ExifTagEntryT<std::vector<uint16> >* tagVuint16 = 
                dynamic_cast<ExifTagEntryT<std::vector<uint16> >*>(tag) ;
            tagVuint16->setValue( bpsVec ) ;
        }

        uint16 photoInterp = PHOTOMETRIC_RGB ;
        switch( mImage->components[0].myColorType.myColor )
        {
            case YCRCB_Y:
            case YCRCB_CR:
            case YCRCB_CB:
                photoInterp = PHOTOMETRIC_YCBCR ;
                break ;
            case RGB_R:
            case RGB_G:
            case RGB_B:
                photoInterp = PHOTOMETRIC_RGB ;
                break ;
            case UNSPECIFIED_COLOR:
            default:
                photoInterp = PHOTOMETRIC_RGB ;
                break ;
        }
        tag = getGenericTag( EXIFTAG_PHOTOMETRIC ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<uint16>
                ( EXIFTAG_PHOTOMETRIC, EXIF_SHORT, 1, photoInterp );
            setGenericTag( tag ) ;
        }
        else
        {
            ExifTagEntryT<uint16>* tagUint16 = 
                dynamic_cast<ExifTagEntryT<uint16>*>(tag) ;
            tagUint16->setValue( photoInterp ) ;
        }


        tag = getGenericTag( EXIFTAG_SAMPLESPERPIXEL ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<uint16>
                ( EXIFTAG_SAMPLESPERPIXEL, EXIF_SHORT, 1,
                mImage->numberOfComponents );
            setGenericTag( tag ) ;
        }
        else
        {
            ExifTagEntryT<uint16>* tagUint16 =
                dynamic_cast<ExifTagEntryT<uint16>*>(tag) ;
            tagUint16->setValue( mImage->numberOfComponents ) ;
        }


        tag = getGenericTag( EXIFTAG_PLANARCONFIG ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<uint16>
                ( EXIFTAG_PLANARCONFIG, EXIF_SHORT, 1, PLANARCONFIG_CONTIG );
            setGenericTag( tag ) ;
        }

    }
    else
    {
        // The image is to be compressed
        // Just set these up, the real values will be supplied when the
        // image is written
        tag = getGenericTag( EXIFTAG_JPEGIFOFFSET ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<exif_uint32>
                ( EXIFTAG_JPEGIFOFFSET, EXIF_LONG, 1, 0 );
            setGenericTag( tag ) ;
        }

        tag = getGenericTag( EXIFTAG_JPEGIFBYTECOUNT ) ;
        if ( tag == NULL )
        {
            tag = new ExifTagEntryT<exif_uint32>
                ( EXIFTAG_JPEGIFBYTECOUNT, EXIF_LONG, 1, 0 );
            setGenericTag( tag ) ;
        }
    }
    
        
    return ;
}

// This function returns true for tags that can contain SHORT or LONG data
bool ExifIFD::canBeSHORTorLONG(uint16 tagNum) const
{
    if (tagNum == EXIFTAG_IMAGEWIDTH || tagNum == EXIFTAG_IMAGELENGTH ||
        tagNum == EXIFTAG_ROWSPERSTRIP || tagNum == EXIFTAG_STRIPOFFSETS ||
        tagNum == EXIFTAG_STRIPBYTECOUNTS )
        return true;
    else
        return false;
}

std::ostream& ExifIFD::print(std::ostream& c, bool bPrintSubIFDs) const
{
    c << "Dir Index:" << mIndex << " Tag Num:" << mTag;
    c << " Offset:" << mOffset << " Next:" << mNextDirOffset << std::endl;
    c << "Num of Dir Entries:" << mTagEntries.size() << std::endl;

    ConstEntryIter crntEntry, endEntry;
    crntEntry = mTagEntries.begin();
    endEntry = mTagEntries.end();
    while (crntEntry != endEntry)
    {
        ((*crntEntry).second)->print(c);
        c << std::endl;
        crntEntry++;
    }

    printf("\n");

    if (bPrintSubIFDs)
    {
        const SubIFDMap mSubDirs = getSubIFDs();
        ConstSubIFDIter s = mSubDirs.begin();
        while (s != mSubDirs.end())
        {
            ExifIFDVec dirVec = (*s).second;
            for (unsigned int i=0; i<dirVec.size(); i++)
            {
                dirVec[i]->print(c);
            }
            s ++;
        }
    }

    return c;
}

void ExifIFD::setGenericTag( const ExifTagEntry& tagEntry )
{ 
    ExifTagEntry* entry = tagEntry.clone();

    setGenericTag( entry ) ;
    
    return; 
}

void ExifIFD::setGenericTag( ExifTagEntry* entry )
{ 
    // remove the pre-set tagEntry if any
    removeGenericTag(entry->getTagNum());

	//     pair<EntryIter, bool> rtnVal =
	mTagEntries.insert(EntryMap::value_type(entry->getTagNum(), entry));

    // if it is a subIFD tag
    SubIFDVecIter subDirTag = find( msSubDirTags.begin(), msSubDirTags.end(),
        entry->getTagNum() );
    if ( subDirTag != msSubDirTags.end() )
        addSubIFD( entry );
    
    cacheImportantValues( entry ) ;
    
    return; 
}
  
void ExifIFD::getAllTags( ExifPathsTags &pathsTags, exiftag_t parentTag)
{

    // First get the last path tags pair
    ExifIFDPath path ;
    if( getTagNum() != 0 )
    {
        size_t i = pathsTags.size() - 1;
        ExifPathTags * pathTags = NULL;
        for (; (i>=0) && (pathTags == NULL); i--)
            if (pathsTags[i].first.back().first == parentTag)
                pathTags = &(pathsTags[i]);
        if (pathTags == NULL)
            pathTags = &(pathsTags.back()) ;
        if (pathTags != NULL) //paranoia
            path = pathTags->first ;
    }
    path.push_back( ExifIFDPath::value_type(getTagNum(), getIndex()) ) ;
    
    ExifTags tmptags ;
    pathsTags.push_back( ExifPathsTags::value_type( path, tmptags ) ) ;
    ExifTags& tags = pathsTags.back().second ;
    
    ExifIFD::EntryIter crntTag = mTagEntries.begin() ;
    ExifIFD::EntryIter endTag = mTagEntries.end() ;
    while( crntTag != endTag )
    {
        tags.push_back( (*crntTag).second ) ;
        crntTag++ ;
    }
    
    // ALSO, go get the Sub-IFDs and their entries
    SubIFDIter crnt = mSubDirs.begin() ;
    SubIFDIter end = mSubDirs.end() ;
    while( crnt != end )
    {
        ExifIFDVec dirVec1 = (*crnt).second;
        for (unsigned int i=0; i<dirVec1.size(); i++)
        {
            ExifIFD* dir1 = dirVec1[i];
            dir1->getAllTags( pathsTags, this->getTagNum() ) ;
        }
        crnt++ ;
    }

    return ;
}


ExifTagEntry* ExifIFD::getGenericTag(exiftag_t tagNum ) const 
{
    ConstEntryIter entryIter;
    if ( (entryIter = mTagEntries.find( tagNum )) == mTagEntries.end() )
    {
        return NULL;
    }
    return (*entryIter).second;
}

bool ExifIFD::removeGenericTag(exiftag_t tag)
{
    bool returnValue = false;
    EntryIter it = mTagEntries.find(tag);
    if (it != mTagEntries.end())
    {
        delete (*it).second;
        mTagEntries.erase(it);

        ExifIFDVec dirVec;
        // if a subIFD tag
        if (getSubIFDVec(tag, dirVec))
        {
            for (unsigned int i=0; i<dirVec.size(); i++)
            {
                delete dirVec[i];
            }
            mSubDirs.erase(tag);
        }
    
        returnValue = true;
    }

    return returnValue;
}

bool ExifIFD::getSubIFDVec(exiftag_t tag, ExifIFDVec& dirVec)
{
    dirVec.clear();
    SubIFDIter it = mSubDirs.find(tag);
    if (it != mSubDirs.end())
    {
        dirVec = (*it).second;
        return true;
    }
    else
    {
        return false;
    }
}

ExifStatus ExifIFD::addSubIFD( ExifIFD* subDir )
{ 
    ExifStatus retval = EXIF_OK;
    
    if (mIFDmap != NULL)
    {
        ExifIFDMapIter i = mIFDmap->find(subDir->getOffset());
        if (i == mIFDmap->end())
        {
            mIFDmap->insert(ExifIFDMap::value_type(subDir->getOffset(),subDir));
            SubIFDIter s = mSubDirs.find(subDir->getTagNum());
            if (s != mSubDirs.end())
            {
                (*s).second.push_back(subDir);
            }
            else
            {
                ExifIFDVec val;
                val.push_back(subDir);
                mSubDirs.insert(SubIFDMap::value_type(subDir->getTagNum(), val));
            }
        }
        else
            retval = EXIF_ERROR;
    }
    else
    {
        SubIFDIter s = mSubDirs.find(subDir->getTagNum());
        if (s != mSubDirs.end())
        {
            (*s).second.push_back(subDir);
        }
        else
        {
            ExifIFDVec val;
            val.push_back(subDir);
            mSubDirs.insert(SubIFDMap::value_type(subDir->getTagNum(), val));
        }
    }
    return retval ;
}

ExifStatus ExifIFD::addSubIFD(exiftag_t tag)
{
    SubIFDVecIter subDirTag = 
        find(msSubDirTags.begin(), msSubDirTags.end(), tag);
    if ( subDirTag == msSubDirTags.end())
        return EXIF_TAG_IS_NOT_A_SUBIFD ;

    // adding tag
    ExifTagEntry* tte = getGenericTag(tag);
    if (tte)
    {
        std::vector<exif_uint32> offs;
        if (tte->getCount() == 1)
        {
            offs.push_back((dynamic_cast<ExifTagEntryT<exif_uint32>*>
                (tte))->getValue());
        }
        else 
        {
            offs = (dynamic_cast<ExifTagEntryT<std::vector<exif_uint32> >*>
                (tte))->getValue();
        }
        offs.push_back(0);
        
        mTagEntries.erase(tag);
        delete tte;
        tte = new ExifTagEntryT<std::vector<exif_uint32> >
            (tag, EXIF_LONG, offs.size(), offs);
        mTagEntries.insert(EntryMap::value_type(tag, tte));
    }
    else
    {
        tte = new ExifTagEntryT<exif_uint32>(tag, EXIF_LONG, 1, 0);
        mTagEntries.insert(EntryMap::value_type(tag, tte));
    }

    // adding sub dir
    SubIFDIter s = mSubDirs.find(tag);
    if (s != mSubDirs.end())
    {
        ExifIFD* subDir = new ExifIFD( tag, (*s).second.size(), mIFDmap,this);
        (*s).second.push_back(subDir);
    }
    else
    {
        ExifIFD* subDir = new ExifIFD( tag, 0, mIFDmap,this);
        ExifIFDVec val;
        val.push_back(subDir);
        mSubDirs.insert(SubIFDMap::value_type(subDir->getTagNum(), val));
    }
    
    return EXIF_OK ;
}

ExifIFD* ExifIFD::getSubIFD(exiftag_t tag, exif_uint32 idx)
{
    ExifIFDVec dirVec;
    if (getSubIFDVec(tag, dirVec))
    {
        if (idx < dirVec.size())
            return (dirVec[idx]);
        else 
            return 0;
    }
    else
        return 0;
}

tsize_t ExifIFD::scanlineSize()
{
    tsize_t scanline;
    
    scanline = bitsPerSample()[0] * imageWidth();
    if (planarConfig() == PLANARCONFIG_CONTIG)
        scanline *= samplesPerPixel();
    return ((tsize_t) EXIFhowmany(scanline, 8));
}

tsize_t ExifIFD::vStripSize( ExifIO* exifio, exif_uint32 nrows)
{

    if (nrows == (exif_uint32) -1)
        nrows = imageLength();
#ifdef YCBCR_SUPPORT
    if (planarConfig() == PLANARCONFIG_CONTIG &&
        photoMetric() == PHOTOMETRIC_YCBCR &&
        !isUpSampled( exifio )) 
	{
        /*
         * Packed YCbCr data contain one Cb+Cr for every
         * HorizontalSampling*VerticalSampling Y values.
         * Must also roundup width and height when calculating
         * since images that are not a multiple of the
         * horizontal/vertical subsampling area include
         * YCbCr data for the extended image.
         */
        std::vector<uint16> ycbcrsubsampling = yccSubSampling();

        tsize_t w =
            EXIFroundup(imageWidth(), ycbcrsubsampling[0]);
        tsize_t scanline = EXIFhowmany(w*(bitsPerSample())[0], 8);
        tsize_t samplingarea =
            ycbcrsubsampling[0]*ycbcrsubsampling[1];
        nrows = EXIFroundup(nrows, ycbcrsubsampling[1]);
        /* NB: don't need EXIFhowmany here 'cuz everything is rounded */
        return ((tsize_t)
                (nrows*scanline + 2*(nrows*scanline / samplingarea)));
    } 
	else
#endif
        return ((tsize_t)(nrows * scanlineSize()));
}

exif_uint32 ExifIFD::numOfStrips()
{
    exif_uint32 num;

    exif_uint32 rps = rowsPerStrip() ;
    num = (rps == (exif_uint32) -1 ?
           (imageLength() != 0 ? 1 : 0) :
           EXIFhowmany(imageLength(), rps));
    if (planarConfig() == PLANARCONFIG_SEPARATE)
        num *= samplesPerPixel();
    return (num);
}

	void ExifIFD::checkBitsPerSample(ExifTagEntry* entry)
	{
		if (EXIFTAG_BITSPERSAMPLE == entry->getTagNum())
		{
			ExifTagEntryT<std::vector<uint16> >* tempEntry = dynamic_cast<ExifTagEntryT<std::vector<uint16> >*>(entry);
			if (tempEntry)
			{
				mCache->mBitsPerSample = tempEntry;
			}
			else
			{
				ExifTagEntryT<unsigned short>* tempEntryTryUshort = dynamic_cast<ExifTagEntryT<unsigned short> *>(entry);
				if (tempEntryTryUshort)
				{
					unsigned int numberOfCompotents = 3;
					if (mImage)
					{
						numberOfCompotents = mImage->numberOfComponents;
					}
					std::vector<uint16> bpsVec(numberOfCompotents, tempEntryTryUshort->getValue()) ;

					ExifTagEntryT<std::vector<uint16> >* newTag = new ExifTagEntryT<std::vector<uint16> >
						( EXIFTAG_BITSPERSAMPLE, EXIF_SHORT, bpsVec.size(), bpsVec );
					setGenericTag( newTag ) ;
				}
			}
		}		
	}



//////////////////////////////////////////////////////////////
// --------- helper functions -----------

void ExifIFD::cacheImportantValues( ExifTagEntry* entry )
{
    switch( entry->getTagNum() )
    {
        case EXIFTAG_IMAGEWIDTH:
            mCache->mImageWidth = dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            break;
        case EXIFTAG_IMAGELENGTH:
            mCache->mImageLength = dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            break ;
        case EXIFTAG_PHOTOMETRIC:
            mCache->mPhotoMetric = dynamic_cast<ExifTagEntryT<uint16>*>(entry);
            break ;
        case EXIFTAG_PLANARCONFIG:
            mCache->mPlanarConfig = 
                dynamic_cast<ExifTagEntryT<uint16>*>(entry);
            break ;
        case EXIFTAG_SAMPLESPERPIXEL:
            mCache->mSamplesPerPixel = 
                dynamic_cast<ExifTagEntryT<uint16>*>(entry);
            break ;
        case EXIFTAG_ROWSPERSTRIP:
            mCache->mRowsPerStrip = 
                dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            break ;
        case EXIFTAG_COMPRESSION:
            mCache->mCompression = dynamic_cast<ExifTagEntryT<uint16>*>(entry);
            break ;
        case EXIFTAG_JPEGIFOFFSET:
            mCache->mJpegInterchange =
                dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            break ;
        case EXIFTAG_JPEGIFBYTECOUNT:
            mCache->mJpegInterchangeLength =
                dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            break ;
        case EXIFTAG_YCBCRSUBSAMPLING:
            mCache->mYccSubSampling =
                dynamic_cast<ExifTagEntryT<std::vector<uint16> >*>(entry);
            break ;
        case EXIFTAG_BITSPERSAMPLE:
            /*mCache->mBitsPerSample =
                dynamic_cast<ExifTagEntryT<std::vector<uint16> >*>(entry);*/
			checkBitsPerSample(entry);
            break ;
        case EXIFTAG_STRIPOFFSETS:
            if ( entry->getCount() > 1 )
            {
                mCache->mStripOffsets = 
                    dynamic_cast<ExifTagEntryT<std::vector<exif_uint32> >*>(entry);
            }
            else
            {
                mCache->mStripOffsets = 
                    dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            }
            break ;
        case EXIFTAG_STRIPBYTECOUNTS:
            if ( entry->getCount() > 1 )
            {
                mCache->mStripByteCounts = 
                    dynamic_cast<ExifTagEntryT<std::vector<exif_uint32> >*>(entry);
            }
            else
            {
                mCache->mStripByteCounts = 
                    dynamic_cast<ExifTagEntryT<exif_uint32>*>(entry);
            }
            break ;
        default:
            break ;
    }
    
    return ;
}

            

// We really need error checking here!

exif_uint32& ExifIFD::imageWidth( void )
{
    return mCache->mImageWidth->getValue() ;
}

exif_uint32& ExifIFD::imageLength( void )
{
    return mCache->mImageLength->getValue();
}

uint16& ExifIFD::photoMetric( void )
{
    return mCache->mPhotoMetric->getValue();
}

uint16 ExifIFD::planarConfig( void )
{
    uint16 returnValue = 0;
    if ( mCache->mPlanarConfig != NULL )
        returnValue = mCache->mPlanarConfig->getValue();
    return returnValue;
}

uint16& ExifIFD::samplesPerPixel( void )
{
    return mCache->mSamplesPerPixel->getValue();
}

exif_uint32& ExifIFD::rowsPerStrip( void )
{
    return mCache->mRowsPerStrip->getValue();
}

uint16& ExifIFD::compression( void )
{
    return mCache->mCompression->getValue() ;
}


exif_uint32& ExifIFD::jpegInterchange( void )
{
    return mCache->mJpegInterchange->getValue() ;
}


exif_uint32& ExifIFD::jpegInterchangeLength( void )
{
    return mCache->mJpegInterchangeLength->getValue()  ;
}

std::vector<uint16>& ExifIFD::yccSubSampling()
{
    return mCache->mYccSubSampling->getValue();
}

std::vector<uint16>& ExifIFD::bitsPerSample()
{
    return mCache->mBitsPerSample->getValue();
}

std::vector<exif_uint32> ExifIFD::stripOffsets()
{
    std::vector<exif_uint32> stripoffsets ;
    ExifTagEntry* off = mCache->mStripOffsets;
    if( off->getCount() > 1 )
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp =
            (ExifTagEntryT< std::vector<exif_uint32> >*)off;
        stripoffsets = temp->getValue();
    }
    else
    {
        ExifTagEntryT<exif_uint32>* temp = (ExifTagEntryT<exif_uint32>*)off;
        stripoffsets.push_back( temp->getValue() );
    }
    return stripoffsets;
}

std::vector<exif_uint32> ExifIFD::stripByteCounts()
{
    std::vector<exif_uint32> stripbytecounts ;
    ExifTagEntry* cnt = mCache->mStripByteCounts;
    if( cnt->getCount() > 1 )
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp =
            (ExifTagEntryT< std::vector<exif_uint32> >*)cnt;
        stripbytecounts = temp->getValue();
    }
    else
    {
        ExifTagEntryT<exif_uint32>* temp = (ExifTagEntryT<exif_uint32>*)cnt;
        stripbytecounts.push_back( temp->getValue() );
    }
    return stripbytecounts;
}

void ExifIFD::registerDirTag( exiftag_t dirTag )
{ 
    SubIFDVecIter subDirTag = find( msSubDirTags.begin(), msSubDirTags.end(),
        dirTag );
    if ( subDirTag == msSubDirTags.end() )
        msSubDirTags.push_back( dirTag ) ;
}

bool ExifIFD::selfGenerated(exiftag_t tag) const
{
    if (tag == EXIFTAG_STRIPOFFSETS || tag == EXIFTAG_STRIPBYTECOUNTS)
        return true;
    else 
        return false;
}
}