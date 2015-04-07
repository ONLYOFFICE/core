
#include "FormattedDiskPagePAPX.h"

namespace DocFileFormat
{
	FormattedDiskPagePAPX::~FormattedDiskPagePAPX()
	{
	  RELEASEARRAYOBJECTS( rgfc );
	  RELEASEARRAYOBJECTS( rgbx );

	  if ( grppapx != NULL )
	  {
	    for ( unsigned int i = 0; i < grppapxSize; i++ )
		{
		  RELEASEOBJECT( grppapx[i] );
		}

		RELEASEARRAYOBJECTS( grppapx );
	  }
	}

	/*========================================================================================================*/
	
	FormattedDiskPagePAPX::FormattedDiskPagePAPX( POLE::Stream* wordStream, int offset, POLE::Stream* dataStream ): 
	FormattedDiskPage(), rgbx(NULL), grppapxSize(0), grppapx(NULL)
    {
	  Type = Paragraph;
      WordStream = wordStream;

      //read the 512 bytes (FKP)
      unsigned char* bytes = NULL;
	  bytes = new unsigned char[512];
	  
	  WordStream->seek( offset);
      WordStream->read( bytes, 512);

      //get the count
      crun = bytes[511];

      //create and fill the array with the adresses
	  rgfcSize = crun + 1;
	  rgfc = new int[rgfcSize];

	  int j = 0;
      
	  for ( unsigned int i = 0; i < rgfcSize; i++ )
      {
	    rgfc[i] = FormatUtils::BytesToInt32( bytes, j, 512 );
        j += 4;
	  }

      //create arrays
      rgbx = new BX[crun];
	  grppapxSize = crun;
      grppapx = new ParagraphPropertyExceptions*[grppapxSize];

	  for ( unsigned int i = 0; i < grppapxSize; i++ )
	  {
	    grppapx[i] = NULL;
	  }

      j = 4 * ( crun + 1 );

	  //read the 12 for PHE
      unsigned char* phe = NULL;
	  phe = new unsigned char[12];

      unsigned char* papx = NULL;
      
	  for ( int i = 0; i < crun; i++ )
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
          //read first unsigned char of PAPX
          //PAPX is stored in a FKP; so the first unsigned char is a count of words
          unsigned char padbyte = 0;
          unsigned char cw = bytes[bx.wordOffset * 2];

          //if that unsigned char is zero, it's a pad unsigned char, and the word count is the following unsigned char
          if ( cw == 0 )
          {
            padbyte = 1;
            cw = bytes[bx.wordOffset * 2 + 1];
          }

          if ( cw != 0 )
          {
            //read the bytes for papx
            papx = new unsigned char[cw * 2];
			memcpy( papx, ( bytes + (bx.wordOffset * 2) + padbyte + 1 ), ( cw * 2 ) );

            //parse PAPX and fill grppapx
            grppapx[i] = new ParagraphPropertyExceptions( papx, ( cw * 2 ), dataStream );

			RELEASEARRAYOBJECTS( papx );
          }
        }
        else
        {
          //create a PAPX which doesn't modify anything
          grppapx[i] = new ParagraphPropertyExceptions();
        }
      }

	  RELEASEARRAYOBJECTS( phe );
	  RELEASEARRAYOBJECTS( bytes );
    }

	/*========================================================================================================*/

	/// Parses the 0Table (or 1Table) for FKP _entries containing PAPX
	list<FormattedDiskPagePAPX*>* FormattedDiskPagePAPX::GetAllPAPXFKPs( FileInformationBlock* fib, POLE::Stream* wordStream, POLE::Stream* tableStream, POLE::Stream* dataStream)
    {
      list<FormattedDiskPagePAPX*>* PAPXlist = new list<FormattedDiskPagePAPX*>();

      //get bintable for PAPX
	  unsigned char* binTablePapx = new unsigned char[fib->m_FibWord97.lcbPlcfBtePapx];

	  tableStream->seek( fib->m_FibWord97.fcPlcfBtePapx);
	  tableStream->read( binTablePapx, fib->m_FibWord97.lcbPlcfBtePapx);

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
      POLE::Stream* wordStream, 
      POLE::Stream* tableStream,
      POLE::Stream* dataStream
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
      POLE::Stream* wordStream, 
      POLE::Stream* tableStream,
      POLE::Stream* dataStream
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