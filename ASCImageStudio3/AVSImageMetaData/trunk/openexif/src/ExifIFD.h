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
 */ 


#ifndef _EXIF_IFD_H_
#define _EXIF_IFD_H_

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

#include "ExifTags.h"
#include "ExifErrorCodes.h"
#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifTagEntry.h"
#include "ExifIO.h"

namespace Exif 
{

typedef std::vector<ExifTagEntry*>  ExifTags ;
typedef ExifTags::iterator    ExifTagsIter ;
typedef ExifTags::const_iterator    ExifTagsConstIter ;

typedef std::pair<ExifIFDPath, ExifTags> ExifPathTags ;
typedef std::vector< ExifPathTags > ExifPathsTags ;
typedef ExifPathsTags::iterator ExifPathsTagsIter ;
typedef ExifPathsTags::const_iterator ExifPathsTagsConstIter ;

struct ExifImportantValuesCache ;

class ExifIFD;
typedef std::map < exifoff_t, ExifIFD * > ExifIFDMap;
typedef ExifIFDMap::iterator ExifIFDMapIter;
typedef ExifIFDMap::const_iterator ExifIFDMapConstIter;
typedef std::pair < bool , ExifIFDMap::value_type > ExifIFDMapInsertResult;
typedef std::vector < ExifIFD * > ExifIFDVec;
typedef ExifIFDVec::iterator ExifIFDVecIter;
typedef ExifIFDVec::const_iterator ExifIFDVecConstIter;
/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Mon Jan 21 06:08:54 2002
  
  \brief  ExifTiffAppSeg support class, provides an in-memory representation
          of a single IFD
  
  The ExifIFD class provides the interface to:
  - read/write tag entries, including defined and private/undefined tag.
  - traverse/create sub IFDs, including defined and private/undefined sub IFDs.
  - read/write thumbnails
  - register private sub IFDs.

  The support of the private/undefined sub IFDs is provided through a
  registry mechanism.  To handle a private/undefined sub IFD, the sub
  IFD tag must be added to the registry.  Some sub IFD tags, including
  EXIFTAG_SUBIFD, EXIFTAG_EXIFIFD, EXIFTAG_GPSIFD, EXIFTAG_INTERIFD,
  are added to the registry automatically by the library.

  Each ExifIFD object is identified by its tag number and index. In
  the case of a main IFD, the tag number will be EXIF_MAINIFD and the
  index will be the main IFD index.

*/
class ExifIFD
{
    public:
        //! \name Container type and iterators for tag entries
        //@{
        typedef std::map<exiftag_t, ExifTagEntry*> EntryMap ;
        typedef EntryMap::iterator EntryIter ;
        typedef EntryMap::const_iterator ConstEntryIter ;
        //@}

        //! \name Container type and iterator for a vector of sub IFDs
        //@{
        typedef std::vector<exiftag_t> SubIFDVec ;
        typedef SubIFDVec::iterator SubIFDVecIter ;
        //@}
        typedef std::vector<ExifIFD*> ExifIFDVec;

        typedef std::map<exiftag_t, ExifIFDVec> SubIFDMap ;
        typedef SubIFDMap::iterator SubIFDIter ;
        typedef SubIFDMap::const_iterator ConstSubIFDIter;
        
        
        /*! Constructor that is used in the library internally. 
        
            \param tagEntry Pointer to the tag entry information of the IFD.
            \param _offset  Offset to the IFD.
            \param idx      Index of the IFD
        */
        ExifIFD( exiftag_t tagNum, exifoff_t _offset, exifoff_t _exifOffset,
            int16 idx, ExifIFDMap * _ifdmap, ExifIFD * parent = NULL ) ;

        /*! Constructor that is used in the library internally. 
        
            \param tagNum  Tag number of the IFD.
            \param idx     Index of the IFD
        */
        ExifIFD( exiftag_t tagNum, int16 idx, 
                 ExifIFDMap * _ifdmap, ExifIFD * parent = NULL  ) ;
    
        /*! Copy Constructor that is used in the library internally.
        
            Performs a deep copy of an ExifIFD. The whole IFD tree will be 
            duplicated
        
          \param ifd  the ExifIFD to copy from  
        */
        ExifIFD( const ExifIFD& ifd ) ;   
    
        //! Destructor
        ~ExifIFD() { deleteItems(); }

        /*! Get the ExifImage object of this IFD
        
            \return Pointer to the image object if it exists or can be created;
                  otherwise NULL.

          In Exif, this method is used to retrive the thumbnail.
        */
        ExifImageDesc* getImage();

        /*! Set the ExifImage object of this IFD.
            
            In Exif, this method is used to set the thumbnail.
        */
        void setImage( ExifImageDesc* img, bool compressIt=true );

        /*! Get the tag number of this IFD.
        
            \return Tag number of the IFD.
        */
        exiftag_t getTagNum( void ) const
        { return mTag ; }

        /*! Get the index of this IFD.
        
            \return Index of the IFD.
        */
        int16  getIndex( void ) const
        { return mIndex ; }

        /*! Return the number of metadata (tag) entries in this IFD.
        
            \return: Number of metadata entries.
        */
        EntryMap::size_type numEntries( void ) const 
        { return mTagEntries.size(); }

        /*! 
        
            \name Set the tag entry to this IFD. 
            If the tag entry is a sub IFD tag, a ExifIFD object for
            the sub IFD will be created internally and can be retrieved
            with the getSubIFD(...) method. These method will overwrite any
            existing tag that matches the tag entry to set.
        */
        //@{
        //! \param tagEntry - reference to metadata (tag) entry to set.
        void setGenericTag( const ExifTagEntry& tagEntry );
        //! \param entry - pointer to metadata (tag) entry to set.
        void setGenericTag( ExifTagEntry* entry ) ;
        //@}

        //! Get the tag entry with the given tag number. 
        /*!
          \param tagNum - Tag number of metadata (tag) to retrieve
          \return Pointer to the tag entry if the tag exists; otherwise NULL.

          If the given tag number is a sub IFD tag, the returned tag
          entry contains the offset information of the sub IFD.
        */
        ExifTagEntry* getGenericTag(exiftag_t tagNum ) const;

        /*! Get all the metadata tags in this IFD
            \param pathsTags the metadata retrieved from this IFD
            \sa ExifImageFile::getAllTags()
            \sa ExifTiffAppSeg::getAllTags()

            This is here to support ExifImageFile::getAllTags().
        */
        void getAllTags( ExifPathsTags &pathsTags, exiftag_t parentTag = 0 ) ;
        
        /*! Remove the given tag from the IFD. 
        
            \param tag Tag number of the metadata (tag) to be deleted
            \return true if the tag exists and is removed successfully;
                 otherwise false.

          If the tag is a sub IFD tag, all the content of the sub IFD will be
          removed as well.
        */
        bool removeGenericTag(exiftag_t tag);

        /*! Add a sub IFD. 
            \param tag  Sub IFD tag number
          
            The tag number must have been previously registered as a sub IFD
            tag via the registerDirTag() method. All Exif defined sub IFDs are
            pre-registered. The index of the sub IFD is the next available
            index.
        */
        ExifStatus addSubIFD(exiftag_t tag);
        
        /*! Get the sub IFD with the tag and index.
        
            \param tag tag number of the sub IFD
            \param idx the index of the sub IFD 
            \return pointer to the sub IFD object if the sub IFD exists;
                  otherwise NULL
        */
        ExifIFD* getSubIFD(exiftag_t tag, exif_uint32 idx = 0);

        /*! Get a list of sub IFDs with the same tag number.
        
            \param tag tag number of the sub IFD
            \param dirVec reference to the vector of the sub IFDs returned
            \return true if the sub IFDs exist; otherwise false.

            The sub IFDs are placed in the vector in index order.
        */
        bool getSubIFDVec(exiftag_t tag, ExifIFDVec& dirVec);

        /*! Register a sub IFD tag. 
          
            \param dirTag  sub IFD tag number.

            The sub IFD are handled with a registry mechanism.  In order
            to support a sub IFD, its tag number needs to be added to
            the registry.  Some sub IFD tags, including EXIFTAG_SUBIFD,
            EXIFTAG_EXIFIFD, EXIFTAG_EXIFIFD, EXIFTAG_INTERIFD, 
            are added to the registry automatically by the library.
        */
        static void registerDirTag( exiftag_t dirTag ) ;

        //! Get the offset value of this IFD object.
        exifoff_t getOffset() const { return mOffset; }
    
        //! Set the offset value of this IFD object.
        void inline setDataOffset( exifoff_t dOffset)
            { mParentDataOffset = dOffset ; }

        /*! Check if this IFD object is a main IFD
          
            \return true if this is a main IFD; otherwise false.
        */
        bool inline isMainIFD() const 
        { return (mTag == EXIF_MAINIFD); }

        /*! Check if the library is doing data up-sampling.
        
            \return: True if doing up-sampling; otherwise false
        */
        bool inline isUpSampled( ExifIO* exifio ) const 
        { return ((exifio->flags() & EXIF_UPSAMPLED) != 0); }

        /*! Check if the fill order flag is set in the given parameter.
        
            \param: fillOrder - Value to check.
            \return: True if the fill order flag is set; otherwise false.
         */
        bool inline isFillOrder( ExifIO* exifio, uint16 fillOrder ) const 
        { return ((exifio->flags() & (fillOrder)) != 0); }

        /*! Check if containing the tag entry.
            
            \param tagNum  tag number.
            \return true if the tag exists; otherwise false.
        */
        bool inline hasTag(exiftag_t tagNum) const
        {
            ExifTagEntry* p = getGenericTag(tagNum);
            return (p ? true : false);
        }

        /*! 
        
            \name Helper functions for ExifStripImage
            ExifStripImage is used to write uncompressed thumbnails. 
        */
        //@{
        //! Compute the size of scanline in byte.
        tsize_t scanlineSize();

        //! Compute the number of bytes in a variable height, row-aligned strip
        tsize_t vStripSize( ExifIO* exifio, exif_uint32 nrows);

        //! Compute how many strips in an image.
        exif_uint32 numOfStrips();
        //@}

        /*! Print the IFD information. This method is for debugging purpose.
        
            \param c output stream for printing.
            \param bPrintSubIFDs whether to print sub IFD inforamtion recursively
            \return output stream for printing.
        */
        std::ostream& print(std::ostream& c = std::cout, bool bPrintSubIFDs = true) const;
    
        //! \name Misc Helper Functions
        //@{
        inline exifoff_t  getExifOffset( void ) const { return mExifOffset ;}
        exif_uint32& imageWidth( void );
        exif_uint32& imageLength( void );
        uint16& photoMetric( void );
        uint16 planarConfig( void );
        uint16& samplesPerPixel( void );
        exif_uint32& rowsPerStrip( void );
        exif_uint32& jpegInterchange( void ) ;
        exif_uint32& jpegInterchangeLength( void ) ;
        uint16& compression( void );
        std::vector<uint16>& yccSubSampling( void );
        std::vector<uint16>& bitsPerSample( void );
        std::vector<exif_uint32> stripOffsets( void );
        std::vector<exif_uint32> stripByteCounts( void );
        //@}

        int readDir( ExifIO* exifio, exifoff_t offset, ExifDirEntry*& dirEntries,
            ExifStatus& errRtn );
        ExifStatus readDirTree( ExifIO* exifio );
        exifoff_t writeDir( ExifIO* exifio );
        exifoff_t writeDirTree( ExifIO* exifio, exifoff_t& tiffHeaderOffset );
        inline exifoff_t getEndOfData( void ) const { return mEndOfData ; }

		bool getLoadImage(){return bLoadImage;}
		void setLoadImage(bool _bLoadImage){bLoadImage = _bLoadImage;}
    
    private:

		bool bLoadImage;

        ExifImageDesc*  mImage;
        uint8* mCompressedThumb;
        exif_uint32 mCompressedThumbLength;

        exifoff_t mEntryOffset;
        exifoff_t mDataOffset;
        exifoff_t mEndOfData;

        exifoff_t mOffset;         // for writing - already includes the addition
                                // of the exifOffset
        exifoff_t mExifOffset ;    // holds the offset due to exif to pass on to
                                // sub-ifd's
        exifoff_t mNextDirOffset;
        exifoff_t mWriteOffset ;
        exifoff_t mParentDataOffset ;
        exifoff_t mOffsetToStripOffsets ;
        exifoff_t mOffsetToStripByteCounts ;
        exifoff_t mOffsetToJpegInterop ;
        exifoff_t mOffsetToJpegByteCount ;

        exiftag_t mTag;
        int16  mIndex;
        EntryMap mTagEntries;
        SubIFDMap mSubDirs;
        ExifIFDMap * mIFDmap;
        ExifIFD * mParent;

        static SubIFDVec msSubDirTags;   // known sub-directory tags

        ExifImportantValuesCache* mCache ;

        // clean up
        void deleteItems( void );
        
        // can the specified tag be of type SHORT or LONG ???
        bool canBeSHORTorLONG(uint16 tagNum) const;
        
        void checkBitsPerSample(ExifTagEntry* entry);
		void cacheImportantValues( ExifTagEntry* entry ) ;

        // Internally the sub IFDs are stored in a STL map. This method returns
        // the reference to the map.
        //!return: Reference to the sub IFD map.
        const SubIFDMap& getSubIFDs() const { return mSubDirs; }

        // Internally the tag entries are stored in a STL map. This method
        // returns the reference to the map.
        //!return: Reference to the tag entry map.
        const EntryMap& getTagEntries() const { return mTagEntries; }

        // convert numerator+denominator to float value
        ExifStatus convertSRational( ExifDirEntry* dir, exif_int32 num, exif_int32 denom,
            float* rv);
        ExifStatus convertRational( ExifDirEntry* dir, exif_uint32 num,
            exif_uint32 denom, float* rv);
            
        // convert a float value to a numerator+denominator (rational)
        ExifStatus rationalize( const double& fpNum, exif_uint32& numer,
            exif_uint32& denom, const double error=1.0e-10 );

        // Fetch a tag that is not handled by special case cose
        ExifTagEntry* fetchNormalTag( ExifIO* exifio, ExifDirEntry* dp );
        
        // Fetch a contiguous directory item
        void fetchData( ExifIO* exifio, exifoff_t offset, int8* dataPtr,
            exif_uint32 byteSize );
            
        // Fetch an array of BYTE or SBYTE values
        int8* fetchByteArray( ExifIO* exifio, ExifDirEntry* dir );
        
        // Fetch an array of SHORT or SSHORT values
        int16* fetchShortArray( ExifIO* exifio, ExifDirEntry* dir );
        
        // Fetch an array of LONG or SLONG values
        exif_int32* fetchLongArray( ExifIO* exifio, ExifDirEntry* dir );
        
        // Fetch an array of SRATIONAL value
        float* fetchSRationalArray( ExifIO* exifio, ExifDirEntry* dir,
            ExifStatus& errRtn );
            
        // Fetch an array of RATIONAL values
        float* fetchRationalArray( ExifIO* exifio, ExifDirEntry* dir,
            ExifStatus& errRtn );
            
        // Fetch an array of FLOAT values
        float* fetchFloatArray( ExifIO* exifio, ExifDirEntry* dir );
        
        // Fetch an array of DOUBLE values
        double* fetchDoubleArray( ExifIO* exifio, ExifDirEntry* dir );
    
        void writeSubIFDArray( ExifIO* exifio,ExifTagEntry *tagEntry,
            ExifIFDVec &dirVec ) ;
        void writeStripOffsetByteCount( ExifIO* exifio ) ;
        void writeJPEGOffsetByteCount( ExifIO* exifio ) ;
        
        void writeNormalTag(  ExifIO* exifio,ExifTagEntry* tagEntry );
        void writeByteArray( ExifIO* exifio, ExifDirEntry& dirEntry,
            int8* dp );
        void writeShortArray( ExifIO* exifio, ExifDirEntry& dirEntry,
            int16* dp );
        void writeLongArray( ExifIO* exifio, ExifDirEntry& dirEntry,
            exif_int32* dp );
        void writeRationalArray( ExifIO* exifio, ExifDirEntry& dirEntry,
            exif_int32* dp );
        void writeFloatArray( ExifIO* exifio, ExifDirEntry& dirEntry,
            float* dp );
        void writeDoubleArray( ExifIO* exifio, ExifDirEntry& dirEntry,
            double* dp );
            
        ExifStatus addSubIFD( ExifIFD* subDir );
        
        // This function will replace the existing sub IFDs
        void addSubIFD( ExifTagEntry* tagEntry );
        
        // this method does a deep copy of the object
        void copy( const ExifIFD& ifd ) ;
        
        // is this tag self generated within the class?
        bool selfGenerated( exiftag_t tag ) const ;
} ;
}
#endif //_EXIF_IFD_H_
