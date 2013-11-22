#pragma once
	
#include <algorithm>
	
class Category
{
public:

	CString Name;
	CSimpleArray<int> Effects;

	Category()
		{ }
	Category(const CString& _name)
		{ Name = _name; }
	Category& operator= (const Category& x)
	{ 
		Name = x.Name; 
		
		Effects.RemoveAll(); 

		for (int index = 0; index < x.Effects.GetSize(); ++index)
			Effects.Add(x.Effects[index]);
		
		return *this;
	}
};

class Effect
{
public:

	int ID;
	CString Name;
	CString Name2;

	Effect()
		{ Name = Name2 = ""; ID = -1; }
	Effect(const CString& _name, int _id)
		{ Name = _name; ID = _id; }
	Effect(const CString& _name, const CString& _name2, int _id)
		{ Name = _name; Name2 = _name2; ID = _id; }
	Effect& operator= (const Effect& x)
		{ Name = x.Name; Name2 = x.Name2; ID = x.ID; return *this; }
};

class Waypoint
{
public:

	WCHAR Name;
	BOOL Visible;
	BYTE Alpha;
	CSimpleArray<float> Actions;

	Waypoint()
	{ 
		Visible = TRUE; 
	}
	Waypoint& operator= (const Waypoint& x)
	{ 
		Actions.RemoveAll(); 

		for (int index = 0; index < x.Actions.GetSize(); ++index)
			Actions.Add(x.Actions[index]);

		Visible = x.Visible;
		Alpha = x.Alpha;
		Name = x.Name;

		return *this;
	}
	void Clear()
	{
		Actions.RemoveAll();
	}
	void Translate(float X, float Y) 
	{
		Actions.Add(1);
		Actions.Add(X);
		Actions.Add(Y);
	}
	void Scale(float X, float Y) 
	{
		Actions.Add(2);
		Actions.Add(X);
		Actions.Add(Y);
	}
	void Rotate(float X, float Y, float Angle) 
	{
		Actions.Add(3);
		Actions.Add(X);
		Actions.Add(Y);
		Actions.Add(Angle);
	}
	void Shear(float X, float Y) 
	{
		Actions.Add(4);
		Actions.Add(X);
		Actions.Add(Y);
	}
	void Render(Graphics* graphics)
	{
		Matrix matrix;

		for (int index = 0; index < Actions.GetSize(); ++index)
		{
			if (Actions[index] <= 1.5)
			{
				matrix.Translate(Actions[index + 1], Actions[index + 2]);

				index += 2;
			}
			else if (Actions[index] <= 2.5)
			{
				matrix.Scale(Actions[index + 1], Actions[index + 2]);

				index += 2;
			}
			else if (Actions[index] <= 3.5)
			{
				PointF center(Actions[index + 1], Actions[index + 2]);

				if (fabs(center.X) < 0.001 || fabs(center.Y) < 0.001)
					matrix.Rotate(Actions[index + 3]);
				else
					matrix.RotateAt(Actions[index + 3], center);

				index += 3;
			}
			else if (Actions[index] <= 4.5)
			{
				matrix.Shear(Actions[index + 1], Actions[index + 2]);

				index += 2;
			}
		}

		graphics->MultiplyTransform(&matrix);
	}
	void Render2(Graphics* graphics, long Angle)
	{
		Matrix matrix;

		for (int index = 0; index < Actions.GetSize(); ++index)
		{
			if (Actions[index] <= 1.5)
			{
				matrix.Translate(Actions[index + 1], Actions[index + 2]);
				
				index += 2;
			}
			else if (Actions[index] <= 2.5)
			{
				matrix.Scale(Actions[index + 1], Actions[index + 2]);

				index += 2;
			}
			else if (Actions[index] <= 3.5)
			{
				PointF center(Actions[index + 1], Actions[index + 2]);

				if (fabs(center.X) < 0.001 || fabs(center.Y) < 0.001)
					matrix.Rotate(Actions[index + 3] + Angle);
				else
					matrix.RotateAt(Actions[index + 3] + Angle, center);

				index += 3;
			}
			else if (Actions[index] <= 4.5)
			{
				matrix.Shear(Actions[index + 1], Actions[index + 2]);

				index += 2;
			}
		}

		graphics->SetTransform(&matrix);
	}
	void Interpolate(Waypoint& p1, Waypoint& p2, double delta)
	{
		Alpha = (BYTE)(p1.Alpha + (p2.Alpha - p1.Alpha)*delta);
		Visible = (delta < 0.5) ? p1.Visible : p2.Visible;
		Name = (delta < 0.5) ? p1.Name : p2.Name;
		Actions.RemoveAll();

		for (int index = 0; index < p1.Actions.GetSize(); ++index)
			Actions.Add((float)(p1.Actions[index] + (p2.Actions[index] - p1.Actions[index])*delta));
	}
};

class Letter
{
public:

    WCHAR Name;
    CSimpleArray<Waypoint> Path;
	int Color;
	long fontSize;
	Letter& operator= (const Letter& x)
	{ 
		Path.RemoveAll(); 

		for (int index = 0; index < x.Path.GetSize(); ++index)
			Path.Add(x.Path[index]);

		Color = x.Color;

		return *this;
	}

	BOOL GetWaypoint(double Frame, Waypoint& point)
	{
		if (Frame < 0 || Frame > 1.0 || Path.GetSize() < 2)
			return FALSE;

		// find index of the nearest waypoint
		int waypoint_index = (int)floor((Path.GetSize() - 1)*Frame);

		// check that the waypoint index is correct
		if (waypoint_index < 0 || waypoint_index >= Path.GetSize())
			return FALSE;
		if (waypoint_index == Path.GetSize() - 1)
		{
			point = Path[waypoint_index];
			return TRUE;
		}

		// compute delta
		double waypoint_step = 1.0/(Path.GetSize() - 1);
		double waypoint_delta = Frame - waypoint_index*waypoint_step;

		// interpolate waypoint
		point.Interpolate(Path[waypoint_index], Path[waypoint_index + 1], waypoint_delta/waypoint_step);

		return TRUE;
	}
    void Initialize(int color, long font_size)
	{
		Color = color;
		fontSize = font_size;
	}
};
	
void RandomMovements( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void RandomMovementsBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearFade( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearFadeBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearShade(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearShadeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearRotate(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearRotateBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearRandom(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearRandomBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Airport( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AirportBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearShake(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearShakeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearWedge(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearWedgeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearStream(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void AppearStreamBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveSimple( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveSimpleBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveZoom( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveZoomBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveFall( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveFallBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveRunShift( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveRunShiftBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveSwing( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveSwingBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveRun( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveRunBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveZoomVertical( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveZoomVerticalBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveShift( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveShiftBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WavePulse( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WavePulseBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );

 //new
void Ejection( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Ellipse( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Wheel( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Miscarry( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Rotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Increase( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void TwoString( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Tumbling( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Leave( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Unfold( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Centrifuge( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HorizontalRotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HorizontalRotation3D( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void VerticalRotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void CheChe( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );

// Erase new	
void EjectionBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void EllipseBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WheelBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void MiscarryBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void RotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void IncreaseBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void TwoStringBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void TumblingBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void LeaveBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void UnfoldBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void CentrifugeBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HorizontalRotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HorizontalRotation3DBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void VerticalRotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void CheCheBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );

 // new2
void Boomerang( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Flash( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Flight( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Jump( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Compress( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Motion( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Manifestation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Shot( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Reduction( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void VHRotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Rotate3D( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Spiral( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Flag( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Perpendicular( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Clock( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );

//Erase new2
void BoomerangBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void FlashBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void FlightBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void JumpBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void CompressBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void MotionBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void ManifestationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void ShotBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void ReductionBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void VHRotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Rotate3DBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void SpiralBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void FlagBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void PerpendicularBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void ClockBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );

 //new3
void Galaxy( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveRotate3D( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void RandomJump( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Galaxy2( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Roll( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Conga( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void _Matrix( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HotPinkSpin( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HopInJumpOut( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void ConJoin( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HopsCotch( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Twist( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void StarWars( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Squeeze( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Terminator( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );

//Erase new3
void GalaxyBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void WaveRotate3DBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void RandomJumpBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void Galaxy2Back( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void RollBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void CongaBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void _MatrixBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HotPinkSpinBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HopInJumpOutBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void ConJoinBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void HopsCotchBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void TwistBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void StarWarsBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void SqueezeBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );
void TerminatorBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha );