#pragma once

#include "FileInformationBlock.h"
#include "VirtualStreamReader.h"

namespace DocFileFormat
{
  class AnnotationOwnerList: public vector<wstring>
  {
    public: 
	  AnnotationOwnerList( FileInformationBlock* fib, IStream* tableStream): 
	  vector<wstring>()
      {
	    VirtualStreamReader tStream( tableStream, fib->m_FibWord97.fcGrpXstAtnOwners );
		  
		while ( tStream.GetPosition() < ( fib->m_FibWord97.fcGrpXstAtnOwners + fib->m_FibWord97.lcbGrpXstAtnOwners ) )
        {
	      this->push_back( tStream.ReadXstz() );
        }
      }

	  virtual ~AnnotationOwnerList()
	  {
	  }
  };
}