#include "stdafx.h"

#include "TransformationMatrix.h"

namespace DocxRenderer
{
  const double DocxRenderer::TransformationMatrix::PI = 3.14159265358979323846;
	
  TransformationMatrix::TransformationMatrix():
  dA(1), dB(0), dC(0), dD(1), dE(0), dF(0)
  {
  }
  
  /*========================================================================================================*/

  TransformationMatrix::TransformationMatrix( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF ):
  dA(_dA), dB(_dB), dC(_dC), dD(_dD), dE(_dE), dF(_dF)
  {
  }

  /*========================================================================================================*/

  void TransformationMatrix::SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF )
  {
    this->dA = _dA;
	this->dB = _dB;
	this->dC = _dC;
	this->dD = _dD;
	this->dE = _dE;
	this->dF = _dF;
  }

  /*========================================================================================================*/

  void TransformationMatrix::ResetTransform()
  {
    this->dA = 1;
	this->dB = 0;
	this->dC = 0;
	this->dD = 1;
	this->dE = 0;
	this->dF = 0;
  }

  /*========================================================================================================*/

  void TransformationMatrix::GetTransform( double* _pdA, double* _pdB, double* _pdC, double* _pdD, double* _pdE, double* _pdF ) const
  {
    if ( _pdA != NULL )
    {
      *_pdA = this->dA;
    }

    if ( _pdB != NULL )
    {
      *_pdB = this->dB;
    }

    if ( _pdC != NULL )
    {
      *_pdC = this->dC;
    }

    if ( _pdD != NULL )
    {
      *_pdD = this->dD;
    }

    if ( _pdE != NULL )
    {
      *_pdE = this->dE;
    }

    if ( _pdF != NULL )
    {
      *_pdF = this->dF;
    }
  }

  /*========================================================================================================*/

  double TransformationMatrix::GetA() const
  {
    return this->dA;
  }

  /*========================================================================================================*/

  double TransformationMatrix::GetB() const
  {
    return this->dB;
  }

  /*========================================================================================================*/

  double TransformationMatrix::GetC() const
  {
    return this->dC;
  }

  /*========================================================================================================*/

  double TransformationMatrix::GetD() const
  {
    return this->dD;
  }

  /*========================================================================================================*/

  double TransformationMatrix::GetE() const
  {
    return this->dE;  
  }

  /*========================================================================================================*/

  double TransformationMatrix::GetF() const
  {
    return this->dF;
  }

  /*========================================================================================================*/

  void TransformationMatrix::Multiply( const TransformationMatrix& _matrix, MatrixOrder matrixOrder )
  {
	double newA = this->dA;
	double newB = this->dB;
	double newC = this->dC;
	double newD = this->dD;
	double newE = this->dE;
	double newF = this->dF;
		  
	if ( matrixOrder == MatrixOrderPrepend )
	{
	  newA = ( ( _matrix.dA * this->dA ) + ( _matrix.dB * this->dC ) );
	  newB = ( ( _matrix.dA * this->dB ) + ( _matrix.dB * this->dD ) );
	  newC = ( ( _matrix.dC * this->dA ) + ( _matrix.dD * this->dC ) );
	  newD = ( ( _matrix.dC * this->dB ) + ( _matrix.dD * this->dD ) );
	  newE = ( ( _matrix.dE * this->dA ) + ( _matrix.dF * this->dC ) + this->dE );
	  newF = ( ( _matrix.dE * this->dB ) + ( _matrix.dF * this->dD ) + this->dF );
	}
	else if ( matrixOrder == MatrixOrderAppend )
	{
	  newA = ( ( this->dA * _matrix.dA ) + ( this->dB * _matrix.dC ) );
	  newB = ( ( this->dA * _matrix.dB ) + ( this->dB * _matrix.dD ) );
	  newC = ( ( this->dC * _matrix.dA ) + ( this->dD * _matrix.dC ) );
	  newD = ( ( this->dC * _matrix.dB ) + ( this->dD * _matrix.dD ) );
	  newE = ( ( this->dE * _matrix.dA ) + ( this->dF * _matrix.dC ) + _matrix.dE );
	  newF = ( ( this->dE * _matrix.dB ) + ( this->dF * _matrix.dD ) + _matrix.dF );
	}

    this->dA = newA;
	this->dB = newB;
	this->dC = newC;
	this->dD = newD;
	this->dE = newE;
	this->dF = newF;
  }

  /*========================================================================================================*/

  void TransformationMatrix::Rotate( double _angle, MatrixOrder matrixOrder )
  {
    TransformationMatrix rotationMatrix( cos( _angle * 180 / PI ), sin( _angle * 180 / PI ), -sin( _angle * 180 / PI ), cos( _angle * 180 / PI ), 0, 0 );

	this->Multiply( rotationMatrix, matrixOrder );
  }

  /*========================================================================================================*/

  void TransformationMatrix::Scale( double _scaleX, double _scaleY, MatrixOrder matrixOrder )
  {
    TransformationMatrix scaleMatrix( _scaleX, 0, 0, _scaleY, 0, 0 );

	this->Multiply( scaleMatrix, matrixOrder );
  }

  /*========================================================================================================*/

  void TransformationMatrix::Translate( double _offsetX, double _offsetY, MatrixOrder matrixOrder )
  {
    TransformationMatrix translateMatrix( 1, 0, 0, 1, _offsetX, _offsetY );

	this->Multiply( translateMatrix, matrixOrder );
  }

  /*========================================================================================================*/

  void TransformationMatrix::LinearTransform( double& _x, double& _y ) const
  {
    double x = _x;
    double y = _y;
		  
    _x = ( ( this->dA * x ) + ( this->dC * y ) );
    _y = ( ( this->dB * x ) + ( this->dD * y ) );  
  }

  /*========================================================================================================*/

  void TransformationMatrix::AffineTransform( double& _x, double& _y ) const
  {
    double x = _x;
    double y = _y;
		  
    _x = ( ( this->dA * x ) + ( this->dC * y ) + this->dE );
    _y = ( ( this->dB * x ) + ( this->dD * y ) + this->dF );
  }
}