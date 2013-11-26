#pragma once

#include <math.h>

namespace DocxRenderer
{
  /*
  Matrix is a affine transform in a 2D-coordinate system:

  | dA dB 0 |
  | dC dD 0 |
  | dE dF 1 |

  Linear transform:
  | dA dB |
  | dC dD |
  */
	
  class TransformationMatrix
  {
    public:
	  enum MatrixOrder
	  {
        MatrixOrderPrepend = 0,
        MatrixOrderAppend
      };

	  static const double PI; //3.14159265358979323846;

    private:
	  double dA;
	  double dB;
	  double dC;
	  double dD;
	  double dE;
	  double dF;

    public:
	  TransformationMatrix();
	  TransformationMatrix( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF );
	  void SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF );
	  void GetTransform( double* _pdA, double* _pdB, double* _pdC, double* _pdD, double* _pdE, double* _pdF ) const;
	  double GetA() const;
	  double GetB() const;
	  double GetC() const;
	  double GetD() const;
	  double GetE() const;
	  double GetF() const;
	  void ResetTransform();
	  void Multiply( const TransformationMatrix& _matrix, MatrixOrder matrixOrder = MatrixOrderPrepend );
	  void Rotate( double _angle, MatrixOrder matrixOrder = MatrixOrderPrepend );
	  void Scale( double _scaleX, double _scaleY, MatrixOrder matrixOrder = MatrixOrderPrepend );
	  void Translate( double _offsetX, double _offsetY, MatrixOrder matrixOrder = MatrixOrderPrepend );
	  void LinearTransform( double& _x, double& _y ) const;
	  void AffineTransform( double& _x, double& _y ) const;
  };
}