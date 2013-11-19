#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
  class PictureFrameType: public ShapeType
  {
    public: 
	  PictureFrameType():
      ShapeType(msosptPictureFrame)
      {
		this->Path = _T( "m@4@5l@4@11@9@11@9@5xe" );

		this->Formulas.push_back( wstring( _T( "if lineDrawn pixelLineWidth 0" ) ) );
        this->Formulas.push_back( wstring( _T( "sum @0 1 0" ) ) );
        this->Formulas.push_back( wstring( _T( "sum 0 0 @1" ) ) );
        this->Formulas.push_back( wstring( _T( "prod @2 1 2" ) ) );
        this->Formulas.push_back( wstring( _T( "prod @3 21600 pixelWidth" ) ) );
        this->Formulas.push_back( wstring( _T( "prod @3 21600 pixelHeight" ) ) );
        this->Formulas.push_back( wstring( _T( "sum @0 0 1" ) ) );
        this->Formulas.push_back( wstring( _T( "prod @6 1 2" ) ) );
        this->Formulas.push_back( wstring( _T( "prod @7 21600 pixelWidth" ) ) );
        this->Formulas.push_back( wstring( _T( "sum @8 21600 0" ) ) );
        this->Formulas.push_back( wstring( _T( "prod @7 21600 pixelHeight" ) ) );
        this->Formulas.push_back( wstring( _T( "sum @10 21600 0" ) ) );

        //pictures are not stroked or fileld by default
        this->Filled = false;
        this->Stroked = false;

        //pictures have a lock on the aspect ratio by default
		this->Lock.fUsefLockAspectRatio = true;
        this->Lock.fLockAspectRatio = true;
      }

	  virtual ~PictureFrameType()
	  {
	  }
  };
}
