#include "stdafx.h"

#include "VMLGraphicsPath.h"

namespace DocxRenderer
{
  VMLGraphicsPath::VMLGraphicsPath():
  width(0), height(0), closed(false)
  {
  }

  /*========================================================================================================*/
	  
  VMLGraphicsPath::VMLGraphicsPath( const std::string& _id, const std::string& _penSize, const std::string& _penColor ):
  width(0), height(0), closed(false), id(_id), penSize(_penSize), penColor(_penColor)
  {
  }

  /*========================================================================================================*/

  std::string VMLGraphicsPath::GetID() const
  {
    return this->id;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetID( const std::string& _id )
  {
    this->id = _id;
  }

  /*========================================================================================================*/

  std::string VMLGraphicsPath::GetPenSize() const
  {
    return this->penSize;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetPenSize( const std::string& _penSize )
  {
    this->penSize = _penSize;
  }

  /*========================================================================================================*/

  std::string VMLGraphicsPath::GetPenColor() const
  {
    return this->penColor;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetPenColor( const std::string& _penColor )
  {
    if ( _penColor.size() == 8 )
	{
	  this->penColor = _penColor;
	  //!!!TODO!!!
	  this->penColor.erase( 6, 2 );
	}
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetBrushColor( const std::string& _brushColor )
  {
    if ( _brushColor.size() == 8 )
	{
	  this->brushColor = _brushColor;
	  //!!!TODO!!!
	  this->brushColor.erase( 6, 2 );
	}
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetWidth( double _width )
  {
    this->width = _width;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetHeight( double _height )
  {
    this->height = _height;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::SetTransform( double _dA, double _dB, double _dC, double _dD, double _dE, double _dF )
  {
	this->matrix.SetTransform( _dA, _dB, _dC, _dD, _dE, _dF );
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::ResetTransform()
  {
	this->matrix.ResetTransform();
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::Close()
  {
	this->closed = true;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::MoveTo( double _x, double _y )
  {
	this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLMove( _x, _y ) ) );
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::LineTo( double _x, double _y )
  {
    this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLLine( _x, _y ) ) );
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::LinesTo( const std::vector<double>& _points )
  {
    size_t pointsSize = _points.size();

	if ( ( pointsSize >= 4 ) && ( ( pointsSize % 2 ) == 0 ) )
	{
	  this->MoveTo( _points[0], _points[1] );

	  for ( unsigned int i = 2; i < ( pointsSize - 1 ); i++ )
	  {
	    this->LineTo( _points[i], _points[i + 1] );
	  }
	}
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::CurveTo( double _x1, double _y1, double _x2, double _y2, double _x3, double _y3 )
  {
    this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLCurve( _x1, _y1 ) ) );
    this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLPoint( _x2, _y2 ) ) );
    this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLPoint( _x3, _y3 ) ) );
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::CurvesTo( const std::vector<double>& _points )
  {
    size_t pointsSize = _points.size();

	if ( pointsSize >= 8 )
	{
	  this->MoveTo( _points[0], _points[1] );

	  for ( unsigned int i = 2; i < ( pointsSize - 5 ); ( i += 6 ) )
	  {
	    this->CurveTo( _points[i], _points[i + 1], _points[i + 2], _points[i + 3], _points[i + 4], _points[i + 5] );
	  }
	}
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::ArcTo( double _x, double _y, double _width, double _height, double _startAngle, double _sweepAngle )
  {
    this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLArc( ( _x + ( _width / 2 ) ), ( _y + ( _height / 2 ) ) ) ) );
    this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLPoint( ( _width / 2 ), ( _height / 2 ) ) ) );
	this->points.push_back( boost::shared_ptr<VMLPoint>( new VMLAngle( ( ( 0 - _startAngle ) * VMLGraphicsPath::DEGREE_360 / 360 ), ( ( ( 0 - _sweepAngle ) * VMLGraphicsPath::DEGREE_360 / 360 ) ) ) ) );
  }

  /*========================================================================================================*/

  std::vector<std::pair<double, double> > VMLGraphicsPath::GetPathPoints() const
  {
    std::vector<std::pair<double, double> > pathPoints;
		  
	for ( unsigned int i = 0; i < this->points.size(); i++ )
	{
	  pathPoints.push_back( std::make_pair( this->points[i]->GetX(), this->points[i]->GetY() ) );
	}

	return pathPoints;
  }

  /*========================================================================================================*/

  OOX::Logic::Shape VMLGraphicsPath::GetShape( bool _stroked, bool _filled, unsigned int _resize ) const
  {
	OOX::Logic::Shape shape;

	OOX::Logic::ShapeStyle shapeStyle;
    shapeStyle.Position = "absolute";
    shapeStyle.PositionHorizontalRelative = "page";
    shapeStyle.PositionVerticalRelative = "page";

    OOX::Logic::Wrap wrap;
    wrap.AnchorX = "page";
    wrap.AnchorY = "page";
  
    shape.Style = shapeStyle;
    shape.Wrap = wrap;

	std::string path;
			
    shape.CoordSize = ( ToString( ceil( this->width ) * _resize ) + "," + ToString( ceil( this->height ) * _resize ) );
	  
	UniversalUnit mmX( UniversalUnit::Mm );
    UniversalUnit mmY( UniversalUnit::Mm );

    mmX = 0;
    mmY = 0;
		  
	Common::Point<Unit<double, Pt, 0> > point;
	point.X = mmX.value( UniversalUnit::Pt );
	point.Y = mmY.value( UniversalUnit::Pt );

    shape.Style->Point = point;
		  
	mmX = this->width;
    mmY = this->height;
	    
  	Common::Size<Unit<double, Pt, 3> > size;
	size.Width = mmX.value( UniversalUnit::Pt );
	size.Height = mmY.value( UniversalUnit::Pt );

	shape.Style->Size = size;
	
	double x = 0;
	double y = 0;

	for ( unsigned int i = 0; i < this->points.size(); i++ )
    {
	  x = this->points[i]->GetX();
	  y = this->points[i]->GetY();

	  this->matrix.AffineTransform( x, y );
	  
	  if ( typeid(*this->points[i].get()) != typeid(VMLAngle) )
	  {
	    x *= _resize;
	    y *= _resize;
	  }

	  boost::scoped_ptr<VMLPoint> newVMLPoint(static_cast<VMLPoint*>(this->points[i]->Clone()));

      newVMLPoint->SetPoint( x, y );

	  path += newVMLPoint->ToString();
	}

	if ( this->closed )
	{
	  path += "x";
	}

	if ( _filled )
	{
	  path += "e";

  	  shape.Style->ZIndex = "-999999999";
	  
	  if ( _stroked )
	  {
	    shape.StrokeColor = ( "#" + this->penColor );

		shape.StrokeWeight = this->penSize;
	  }
	  else
	  {
	    shape.StrokeColor = ( "#" + this->brushColor );

		UniversalUnit zeroStrokeWeight( UniversalUnit::Pt );
        
		zeroStrokeWeight = 0;
		
		shape.StrokeWeight = zeroStrokeWeight.ToString();
	  }

	  shape.FillColor = ( "#" + this->brushColor );
	}
	else
	{
	  path += "nfe";

	  shape.Style->ZIndex = "-1";

	  if ( _stroked )
	  {
	    shape.StrokeColor = ( "#" + this->penColor );

		shape.StrokeWeight = this->penSize;
	  }
	}

	shape.Id = this->id;
	shape.Path = path;
	    
	return shape;
  }

  /*========================================================================================================*/

  void VMLGraphicsPath::Reset()
  {
    this->points.clear();
	this->closed = false;
  }
}