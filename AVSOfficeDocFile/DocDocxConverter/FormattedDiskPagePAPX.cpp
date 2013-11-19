#include "stdafx.h"
#include "FormattedDiskPagePAPX.h"

namespace DocFileFormat
{
	FormattedDiskPagePAPX::~FormattedDiskPagePAPX()
	{
	  RELEASEARRAYOBJECTS( this->rgfc );
	  RELEASEARRAYOBJECTS( this->rgbx );

	  if ( this->grppapx != NULL )
	  {
	    for ( unsigned int i = 0; i < this->grppapxSize; i++ )
		{
		  RELEASEOBJECT( this->grppapx[i] );
		}

		RELEASEARRAYOBJECTS( this->grppapx );
	  }
	}

	/*========================================================================================================*/
	
	FormattedDiskPagePAPX::FormattedDiskPagePAPX( IStream* wordStream, int offset, IStream* dataStream ): 
	FormattedDiskPage(), rgbx(NULL), grppapxSize(0), grppapx(NULL)
    {
	  this->Type = Paragraph;
      this->WordStream = wordStream;

      //read the 512 bytes (FKP)
      byte* bytes = NULL;
	  bytes = new byte[512];
	  
	  LARGE_INTEGER nSeekPos = { offset, 0 };
	  this->WordStream->Seek( nSeekPos, STREAM_SEEK_SET, NULL );
      this->WordStream->Read( bytes, 512, NULL );

      //get the count
      this->crun = bytes[511];

      //create and fill the array with the adresses
	  this->rgfcSize = this->crun + 1;
	  this->rgfc = new int[this->rgfcSize];

	  int j = 0;
      
	  for ( unsigned int i = 0; i < this->rgfcSize; i++ )
      {
	    rgfc[i] = FormatUtils::BytesToInt32( bytes, j, 512 );
        j += 4;
	  }

      //create arrays
      this->rgbx = new BX[this->crun];
	  this->grppapxSize = this->crun;
      this->grppapx = new ParagraphPropertyExceptions*[this->grppapxSize];

	  for ( unsigned int i = 0; i < this->grppapxSize; i++ )
	  {
	    this->grppapx[i] = NULL;
	  }

      j = 4 * ( this->crun + 1 );

	  //read the 12 for PHE
      byte* phe = NULL;
	  phe = new byte[12];

      byte* papx = NULL;
      
	  for ( int i = 0; i < this->crun; i++ )
      {
		memcpy( phe, ( bytes + j + 1 ), 12 );

        //fill the rgbx array
        BX bx;
		bx.wordOffset = bytes[j];
		bx.phe = ParagraphHeight( phe, 12, false );

        rgbx[i] = bx;
        j += 13;

        if ( bx.wordOffset != 0 )
        {
          //read first byte of PAPX
          //PAPX is stored in a FKP; so the first byte is a count of words
          byte padbyte = 0;
          byte cw = bytes[bx.wordOffset * 2];

          //if that byte is zero, it's a pad byte, and the word count is the following byte
          if ( cw == 0 )
          {
            padbyte = 1;
            cw = bytes[bx.wordOffset * 2 + 1];
          }

          if ( cw != 0 )
          {
            //read the bytes for papx
            papx = new byte[cw * 2];
			memcpy( papx, ( bytes + (bx.wordOffset * 2) + padbyte + 1 ), ( cw * 2 ) );

            //parse PAPX and fill grppapx
            this->grppapx[i] = new ParagraphPropertyExceptions( papx, ( cw * 2 ), dataStream );

			RELEASEARRAYOBJECTS( papx );
          }
        }
        else
        {
          //create a PAPX which doesn't modify anything
          this->grppapx[i] = new ParagraphPropertyExceptions();
        }
      }

	  RELEASEARRAYOBJECTS( phe );
	  RELEASEARRAYOBJECTS( bytes );
    }

	/*========================================================================================================*/

	/// Parses the 0Table (or 1Table) for FKP _entries containing PAPX
	list<FormattedDiskPagePAPX*>* FormattedDiskPagePAPX::GetAllPAPXFKPs( FileInformationBlock* fib, IStream* wordStream, IStream* tableStream, IStream* dataStream)
    {
      list<FormattedDiskPagePAPX*>* PAPXlist = new list<FormattedDiskPagePAPX*>();

      //get bintable for PAPX
	  byte* binTablePapx = new byte[fib->m_FibWord97.lcbPlcfBtePapx];

	  LARGE_INTEGER nSeekPos = { (int)fib->m_FibWord97.fcPlcfBtePapx, 0 };
	  tableStream->Seek( nSeekPos, STREAM_SEEK_SET, NULL );
	  tableStream->Read( binTablePapx, fib->m_FibWord97.lcbPlcfBtePapx, NULL );

      //there are n offsets and n-1 fkp's in the bin table
      int n = ( ( (int)fib->m_FibWord97.lcbPlcfBtePapx - 4 ) / 8 ) + 1;

      //Get the indexed PAPX FKPs
      for ( unsigned int i = ( n * 4 ); i < fib->m_FibWord97.lcbPlcfBtePapx; i += 4 )
      {
        //indexed FKP is the xth 512byte page
	    int fkpnr = FormatUtils::BytesToInt32( binTablePapx, i, fib->m_FibWord97.lcbPlcfBtePapx );

        //so starts at:
        int offset = fkpnr * 512;

        //parse the FKP and add it to the list
		PAPXlist->push_back( new FormattedDiskPagePAPX( wordStream, offset, dataStream ) );
      }

	  RELEASEARRAYOBJECTS( binTablePapx );

      return PAPXlist;
    }

	/*========================================================================================================*/

	/// Returns a list of all PAPX FCs between they given boundaries.
	list<int>* FormattedDiskPagePAPX::GetFileCharacterPositions
	(
      int fcMin,
      int fcMax,
      FileInformationBlock* fib, 
      IStream* wordStream, 
      IStream* tableStream,
      IStream* dataStream
	)
    {
      list<int>* cpList = new list<int>();
	  list<FormattedDiskPagePAPX*> *fkps = FormattedDiskPagePAPX::GetAllPAPXFKPs( fib, wordStream, tableStream, dataStream );
      unsigned int i = 0;
	  FormattedDiskPagePAPX* fkp = NULL;

	  for ( list<FormattedDiskPagePAPX*>::iterator iter = fkps->begin(); iter != fkps->end(); iter++ )
      {
		fkp = (*iter);

        //the last entry of each is always the same as the first entry of the next FKP
        //so, ignore all last _entries except for the last FKP.
		int max = fkp->rgfcSize;
        
		if ( i++ < fkps->size() - 1 )
		{
		  max--;
		}
         
		for ( int j = 0; j < max; j++ )
        {
          if( ( fkp->rgfc[j] >= fcMin ) && ( fkp->rgfc[j] < fcMax ) )
		  {
			  cpList->push_back( fkp->rgfc[j] );
          }
        }

		RELEASEOBJECT( fkp );
	  }

	  fkps->clear();
	  RELEASEOBJECT( fkps );

      return cpList;
    }

	/*========================================================================================================*/

    /// Returnes a list of all ParagraphPropertyExceptions which correspond to text 
    /// between the given offsets.
	list<ParagraphPropertyExceptions*>* FormattedDiskPagePAPX::GetParagraphPropertyExceptions
	(
      int fcMin,
      int fcMax,
      FileInformationBlock* fib, 
      IStream* wordStream, 
      IStream* tableStream,
      IStream* dataStream
	)
    {
      list<ParagraphPropertyExceptions*>* ppxList = new list<ParagraphPropertyExceptions*>();
	  list<FormattedDiskPagePAPX*>* fkps = FormattedDiskPagePAPX::GetAllPAPXFKPs( fib, wordStream, tableStream, dataStream );
	  FormattedDiskPagePAPX *fkp = NULL;
      
	  for ( list<FormattedDiskPagePAPX*>::iterator iter = fkps->begin(); iter != fkps->end(); iter++ )
      {
        fkp = (*iter);

		for ( unsigned int j = 0; j < fkp->grppapxSize; j++ )
        {
          if ( ( fkp->rgfc[j] >= fcMin ) && ( fkp->rgfc[j] < fcMax ) )
		  {
			  ppxList->push_back( fkp->grppapx[j] );
          }
        }

		RELEASEOBJECT( fkp );

	    fkps->clear();
		RELEASEOBJECT( fkps );
      }

      return ppxList;
	}
}