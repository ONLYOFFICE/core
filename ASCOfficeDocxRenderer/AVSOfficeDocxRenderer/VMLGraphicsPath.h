#pragma once

#include "Docx.h"

#include "TransformationMatrix.h"
#include "Utils.h"

namespace DocxRenderer
{
  class VMLGraphicsPath
  {
	class VMLPoint: public IClone
	{
	  protected:
	    double x;
		double y;

	  public:
	    VMLPoint( double _x = 0, double _y = 0 ):
		x(_x), y(_y)
		{
		}

		virtual ~VMLPoint()
		{
		}

		virtual IClone* New() const
		{
		  return new VMLPoint();
		}

		virtual IClone* Clone() const
		{
		  return new VMLPoint( *this );
		}

		virtual std::string ToString() const
		{
		  return ( "," + ::ToString( ceil( this->x ) ) + "," + ::ToString( ceil( this->y ) ) );
		}

		virtual double GetX() const
		{
		  return this->x;
		}

		virtual double GetY() const
		{
		  return this->y;
		}

		void SetPoint( double _x, double _y )
		{
		  this->x = _x;
		  this->y = _y;
		}
	};

	class VMLMove: public VMLPoint
	{
	  public:
	   	VMLMove( double _x = 0, double _y = 0 ):
		VMLPoint( _x, _y )
		{
		}

		virtual ~VMLMove()
		{
		}

		virtual IClone* New() const
		{
		  return new VMLMove();
		}

		virtual IClone* Clone() const
		{
		  return new VMLMove( *this );
		}

		virtual std::string ToString() const
		{
          return ( "m" + ::ToString( ceil( this->x ) ) + "," + ::ToString( ceil( this->y ) ) );
		} 
	};

    class VMLLine: public VMLPoint
	{
	  public:
	   	VMLLine( double _x = 0, double _y = 0 ):
		VMLPoint( _x, _y )
		{
		}

		virtual ~VMLLine()
		{
		}

		virtual IClone* New() const
		{
		  return new VMLLine();
		}

		virtual IClone* Clone() const
		{
		  return new VMLLine( *this );
		}

		virtual std::string ToString() const
		{
		  return ( "l" + ::ToString( ceil( this->x ) ) + "," + ::ToString( ceil( this->y ) ) );
		} 
	};

	class VMLCurve: public VMLPoint
	{
	  public:
	   	VMLCurve( double _x = 0, double _y = 0 ):
		VMLPoint( _x, _y )
		{
		}

		virtual ~VMLCurve()
		{
		}

		virtual IClone* New() const
		{
		  return new VMLCurve();
		}

		virtual IClone* Clone() const
		{
		  return new VMLCurve( *this );
		}

		virtual std::string ToString() const
		{
		  return ( "c" + ::ToString( ceil( this->x ) ) + "," + ::ToString( ceil( this->y ) ) );
		} 
	};

	class VMLArc: public VMLPoint
	{
      public:
	   	VMLArc( double _x = 0, double _y = 0 ):
		VMLPoint( _x, _y )
		{
		}

		virtual ~VMLArc()
		{
		}

		virtual IClone* New() const
		{
		  return new VMLArc();
		}

		virtual IClone* Clone() const
		{
		  return new VMLArc( *this );
		}

		virtual std::string ToString() const
		{
		  return ( "al" + ::ToString( ceil( this->x ) ) + "," + ::ToString( ceil( this->y ) ) );
		}   
	};

	class VMLAngle: public VMLPoint
	{
      public:
	   	VMLAngle( double _x = 0, double _y = 0 ):
		VMLPoint( _x, _y )
		{
		}

		virtual ~VMLAngle()
		{
		}

		virtual IClone* New() const
		{
		  return new VMLAngle();
		}

		virtual IClone* Clone() const
		{
		  return new VMLAngle( *this );
		}

		virtual std::string ToString() const
		{
		  return ( "," + ::ToString( ceil( this->x ) ) + "," + ::ToString( ceil( this->y ) ) );
		}   
	};

    private:
	  std::vector<boost::shared_ptr<VMLPoint> > points;

	  std::string id;
	  std::string penSize;
	  std::string penColor;
	  std::string brushColor;

	  double width;
	  double height;

	  bool closed;

	  TransformationMatrix matrix;

	  //!!!TODO: There is not clearly, how VML save angle values. This is 360 degree approximate value for VML.!!!
	  static const unsigned long DEGREE_360 = 23592000; 

    public:
	  VMLGraphicsPath();	  
	  VMLGraphicsPath( const std::string& _id, const std::string& _penSize, const std::string& _penColor );
	  std::string GetID() const;
	  void SetID( const std::string& _id );
	  std::string GetPenSize() const;
	  void SetPenSize( const std::string& _penSize );
	  std::string GetPenColor() const;
	  void SetPenColor( const std::string& _penColor );
	  void SetBrushColor( const std::string& _brushColor );
	  void SetWidth( double _width );
	  void SetHeight( double _height );
	  void SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF );
	  void ResetTransform();
	  void Close();
	  void MoveTo( double _x, double _y );
      void LineTo( double _x, double _y );
	  void LinesTo( const std::vector<double>& _points );
	  void CurveTo( double _x1, double _y1, double _x2, double _y2, double _x3, double _y3 );
	  void CurvesTo( const std::vector<double>& _points );
	  void ArcTo( double _x, double _y, double _width, double _height, double _startAngle, double _sweepAngle );
	  std::vector<std::pair<double, double> > GetPathPoints() const;
	  OOX::Logic::Shape GetShape( bool _stroked = true, bool _filled = true, unsigned int _resize = 1000000 ) const;
	  void Reset();
  };
}