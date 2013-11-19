#pragma once

#include "..\ShapeType.h"

namespace DocFileFormat
{
  class ParallelogramType: public ShapeType
  {
    public: 
	  ParallelogramType():
      ShapeType(msosptParallelogram)
      {
        this->ShapeConcentricFill = true;

        this->Joins = miter;

        this->Path = _T( "m@0,l,21600@1,21600,21600,xe" );

        this->Formulas.push_back( _T( "val #0" ) );
        this->Formulas.push_back( _T( "sum width 0 #0" ) );
        this->Formulas.push_back( _T( "prod #0 1 2" ) );
        this->Formulas.push_back( _T( "sum width 0 @2" ) );
        this->Formulas.push_back( _T( "mid #0 width" ) );
        this->Formulas.push_back( _T( "mid @1 0" ) );
        this->Formulas.push_back( _T( "prod height width #0" ) );
        this->Formulas.push_back( _T( "prod @6 1 2" ) );
        this->Formulas.push_back( _T( "sum height 0 @7" ) );
        this->Formulas.push_back( _T( "prod width 1 2" ) );
        this->Formulas.push_back( _T( "sum #0 0 @9" ) );
        this->Formulas.push_back( _T( "if @10 @8 0") );
        this->Formulas.push_back( _T( "if @10 @7 height" ) );

        this->AdjustmentValues = _T( "5400" );
            
        this->ConnectorLocations = _T( "@4,0;10800,@11;@3,10800;@5,21600;10800,@12;@2,10800" );

        this->TextBoxRectangle = _T( "1800,1800,19800,19800;8100,8100,13500,13500;10800,10800,10800,10800" );

        Handle HandleOne;
        HandleOne.position = _T( "#0,topLeft" );
        HandleOne.xrange = _T( "0,21600" );
        this->Handles.push_back( HandleOne );
	  }

	  virtual ~ParallelogramType()
	  {
	  }
  };
}