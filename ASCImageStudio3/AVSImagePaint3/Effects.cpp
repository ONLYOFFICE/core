#include "stdafx.h"
#include "Effects.h"
#include "ImageTextDynamic3.h"

void RandomMovements( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();

    for (int index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        // initialize point
		point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = 0;
		
        point.Clear();
        point.Translate(rand() % 300, rand() % 300);
        point.Rotate(0, 0, rand() % 720);
        point.Scale(1 + 0.1f*(rand() % 100), 1 + 0.1f*(rand() % 100));
	    letter->Path.Add(point);

        point.Clear();
        point.Alpha = (BYTE)Alpha;
        point.Translate( finish[2*index] , finish[2*index+1] );
        point.Rotate(0, 0, 0);
        point.Scale(1, 1);
        letter->Path.Add(point);	
    }
}

void RandomMovementsBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();

    for (int index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        // initialize point
        point.Name = letter->Name;
        point.Visible = TRUE;

        point.Clear();
        point.Alpha = (BYTE)Alpha;
        point.Translate( finish[2*index] , finish[2*index+1] );
        point.Rotate(0, 0, 0);
        point.Scale(1, 1);
        letter->Path.Add(point);

		point.Clear();
        point.Alpha = 0;
        point.Translate(rand() % 300, rand() % 300);
        point.Rotate(0, 0, rand() % 720);
        point.Scale(1 + 0.1f*(rand() % 100), 1 + 0.1f*(rand() % 100));
	    letter->Path.Add(point);
    }
}

void AppearFade(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
	    point.Name = letter->Name;
	    point.Visible = TRUE; 
	    point.Alpha = (BYTE)0; 

        point.Clear();
        point.Translate( finish[2*index] , finish[2*index+1] );

    	letter->Path.Add(point);

	    for ( i=0; i<length; i++)
	    {
		    if ( index <= i )
			    point.Alpha = (BYTE)Alpha;
		    else		
			    point.Alpha = 0;
		    letter->Path.Add(point);
	    }
    }	
	return;
}

void AppearFadeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
	    point.Name = letter->Name;
	    point.Visible = TRUE; 
	    point.Alpha = (BYTE)Alpha; 

        point.Clear();
        point.Translate( finish[2*index] , finish[2*index+1] );

    	letter->Path.Add(point);

	    for ( i=0; i<length; i++)
	    {
		    if ( index <= i )
			    point.Alpha = 0;
		    else
			    point.Alpha = (BYTE)Alpha;
		    letter->Path.Add(point);
	    }
    }	
	return;
}

void AppearShade(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      index;

    float    LastX = finish[ (length-1)*2 ];
    float    FinishX, FinishY;
    float    StartX, StartY;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = StartY = finish[2*index+1];
        StartX = FinishX - (LastX + 10);

        point.Clear();
        point.Translate(StartX, StartY);
        point.Shear(0, 0);
        letter->Path.Add(point);

        point.Clear();
        point.Translate(FinishX+20 + (LastX-FinishX)/3, FinishY);
        point.Shear(-2, 0);
        letter->Path.Add(point);

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Shear(0, 0);
        letter->Path.Add(point);
    }
    return;
}

void AppearShadeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      index;

    float    LastX = finish[ (length-1)*2 ];
    float    FirstX = finish[0];
	float    FinishX, FinishY;
    float    StartX, StartY;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = StartY = finish[2*index+1];
        StartX = FinishX + (LastX + 10);

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Shear(0, 0);
        letter->Path.Add(point);

        point.Clear();
        point.Translate(FinishX-20 - (FinishX-FirstX)/3, FinishY);
        point.Shear(2, 0);
        letter->Path.Add(point);

        point.Alpha = 0;
		point.Clear();
        point.Translate(StartX, StartY);
        point.Shear(-1, 0);
        letter->Path.Add(point);
	}
    return;
}

void AppearRotate(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      index;

    float    FinishX, FinishY;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, -1000);
        point.Scale(0, 0);
        letter->Path.Add(point);

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, -500);
        point.Scale(2, 2);
        letter->Path.Add(point);

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, 0);
        point.Scale(1, 1);
        letter->Path.Add(point);
    }
    return;
}

void AppearRotateBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      index;

    float    FinishX, FinishY;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, 0);
        point.Scale(1, 1);
        letter->Path.Add(point);

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, -500);
        point.Scale(2, 2);
        letter->Path.Add(point);

		point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, -1000);
        point.Scale(0.0001, 0.0001);
        letter->Path.Add(point);
	}
    return;
}

void AppearRandom(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      index, i;
    int *    Random_Index = new int[length];
    int      step = 2;
    int      N = length + 2*step;
    float    max_scale = 3;
    float    scale = 1;
    float    scale_step = (max_scale - 1)/step;
    float    alpha_step = (float)Alpha/step;
    float    FinishX, FinishY;

    for ( i=0; i<length; i++ )
        Random_Index[i] = i;
    std::random_shuffle(&Random_Index[0], &Random_Index[length]);

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = 0;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
        for (i=0; i<N; i++)
        {
            if ( i<Random_Index[index] )
            {
                point.Alpha = 0;
                scale = 1;
            }
            else if ( i>=Random_Index[index]+2*step )
            {
                point.Alpha = (BYTE)Alpha;
                scale = 1;
            }
            else
            {
				if ( i-Random_Index[index] < step )
				{    
					point.Alpha = (BYTE)(alpha_step * (i-Random_Index[index]+1));
                    scale = scale_step * (i-Random_Index[index]) + 1;
                } else
                {
                    point.Alpha = (BYTE)Alpha;
                    scale = max_scale - scale_step * (i-Random_Index[index]-step);
                }
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
            point.Scale(scale,scale);
            letter->Path.Add(point);
        }
        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }

    delete [] Random_Index;
    return;
}

void AppearRandomBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      index, i;
    int *    Random_Index = new int[length];
    int      step = 2;
    int      N = length + 2*step;
    float    max_scale = 3;
    float    scale = 1;
    float    scale_step = (max_scale - 1)/step;
    float    alpha_step = (float)Alpha/step;
    float    FinishX, FinishY;

    for ( i=0; i<length; i++ )
        Random_Index[i] = i;
    std::random_shuffle(&Random_Index[0], &Random_Index[length]);

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = 0;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
        for (i=0; i<N; i++)
        {
            if ( i<Random_Index[index] )
            {
                point.Alpha = (BYTE)Alpha;
                scale = 1;
            }
            else if ( i>=Random_Index[index]+2*step )
            {
                point.Alpha = 0;
                scale = 1;
            }
            else
            {
				if ( i-Random_Index[index] < step )
				{    
                    point.Alpha = (BYTE)Alpha;
                    scale = max_scale - scale_step * (i-Random_Index[index]-step);
                } else
                {
					point.Alpha = (BYTE)(alpha_step * (i-Random_Index[index]+1));
                    scale = scale_step * (i-Random_Index[index]) + 1;
                }
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
            point.Scale(scale,scale);
            letter->Path.Add(point);
        }
        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }

    delete [] Random_Index;
    return;
}

void Airport( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    WCHAR    Name;
    int      RndNum = 65;
    int      LettersQty = 200;
    int      step = LettersQty/10;
    int      N = step * length + (LettersQty-step);
    int      index_step;
	int		 Half_Rnd = RAND_MAX/2;
    for ( index = 0; index < length; ++index)
    {
        index_step = index * step;
        letter = &(letters[index]);
        point.Name = Name = letter->Name;

        point.Visible = TRUE;
        point.Alpha = 0;

        point.Clear();
        point.Translate(finish[index*2],finish[index*2+1]);

        if ( Name == 32 )
	    {
		    point.Alpha = (BYTE)Alpha;	
		    for (int i=0; i<N; i++)
    		    letter->Path.Add(point);
		    continue;
	    }
        for (i=0; i<N; i++)
        {
            if ( i<index_step )
            {
                point.Alpha = 0;
            }
            else if ( i>=index_step + LettersQty )
            {
                point.Name = Name;
                point.Alpha = (BYTE)Alpha;
            }
            else
            {
                RndNum = rand()%26;
				if ( rand()<Half_Rnd )
					RndNum += 65;
				else
					RndNum += 97;
                point.Name = (WCHAR)RndNum;
                point.Alpha = (BYTE)(Alpha*(double(i-index_step)/LettersQty));
            }
		    letter->Path.Add(point);
        }
        point.Name = Name;
        point.Alpha = (BYTE)Alpha;
        letter->Path.Add(point);
    }
    return;
}

void AirportBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    WCHAR    Name;
    int      RndNum = 65;
    int      LettersQty = 200;
    int      step = LettersQty/10;
    int      N = step * length + (LettersQty-step);
    int      index_step;
	int		 Half_Rnd = RAND_MAX/2;
    for ( index = 0; index < length; ++index)
    {
        index_step = index * step;
        letter = &(letters[index]);
        point.Name = Name = letter->Name;

        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        point.Clear();
        point.Translate(finish[index*2],finish[index*2+1]);

        if ( Name == 32 )
	    {
		    point.Alpha = (BYTE)Alpha;	
		    for (int i=0; i<N; i++)
    		    letter->Path.Add(point);
		    continue;
	    }
        for (i=0; i<N; i++)
        {
            if ( i<index_step )
            {
                point.Name = Name;
                point.Alpha = (BYTE)Alpha;
            }
            else if ( i>=index_step + LettersQty )
            {
                point.Alpha = 0;
            }
            else
            {
                RndNum = rand()%26;
				if ( rand()<Half_Rnd )
					RndNum += 65;
				else
					RndNum += 97;
                point.Name = (WCHAR)RndNum;
                point.Alpha = (BYTE)(Alpha*(1 - double(i-index_step)/LettersQty));
            }
		    letter->Path.Add(point);
        }
        point.Name = Name;
        point.Alpha = 0;
        letter->Path.Add(point);
    }
    return;
}

void AppearShake(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
	float	 FinishX, FinishY, y;
	float	 shift_angle, shift_x, shift_y;
	float	 dist_xy, dist_angle;
	int		 step = 10;
	int		 N = step + length;
	
    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
	    point.Name = letter->Name;
	    point.Visible = TRUE; 
	    point.Alpha = (BYTE)0; 

		FinishX = finish[2*index];
		FinishY = finish[2*index+1];
        dist_xy = 5.f;
		dist_angle = 20.f;
		point.Clear();
		point.Translate( FinishX , - (FinishX - finish[0]) );
		point.Rotate(0., 0., 0.);
		letter->Path.Add(point);
		
		for ( i=0; i<=N; i++)
	    {
		    if ( index <= i )
			{
				if ( index+step < i )
				{
					shift_x = shift_y = shift_angle = 0.f;
				}
				else
				{
					shift_angle = ((float)rand()/RAND_MAX - 0.5f)*dist_angle;
					shift_x = ((float)rand()/RAND_MAX - 0.5f)*dist_xy;
					shift_y = ((float)rand()/RAND_MAX - 0.5f)*dist_xy;
				}
				point.Alpha = (BYTE)Alpha;
				y = FinishY;
			}
		    else
			{
				point.Alpha = (BYTE)Alpha;
				y = FinishY - (FinishX - finish[2*i]) ;
				shift_angle = shift_x = shift_y = 0.f;
			}
		    point.Clear();
			point.Translate( FinishX + shift_x, y + shift_y);
			point.Rotate(0.f, 0.f, shift_angle);
			letter->Path.Add(point);
	    }
    }	
	return;
}

void AppearShakeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
	float	 FinishX, FinishY, y;
	float	 shift_angle, shift_x, shift_y;
	float	 dist_xy, dist_angle;
	int		 step = 10;
	int		 N = step + length;
	
    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
	    point.Name = letter->Name;
	    point.Visible = TRUE; 
		point.Alpha = (BYTE)Alpha;

		FinishX = finish[2*index];
		FinishY = finish[2*index+1];
        dist_xy = 5.f;
		dist_angle = 20.f;
	
		point.Clear();
		point.Translate( FinishX , FinishY);
		point.Rotate(0.f, 0.f, 0.f);
		letter->Path.Add(point);
		for ( i=0; i<=N; i++)
	    {
		    if ( i <= length-1-index )
			{
				y = FinishY;
				shift_x = shift_y = shift_angle = 0.f;
			}
			else if ( i <= length-1-index+step )
			{
				y = FinishY;
				shift_angle = ((float)rand()/RAND_MAX - 0.5f)*dist_angle;
				shift_x = ((float)rand()/RAND_MAX - 0.5f)*dist_xy;
				shift_y = ((float)rand()/RAND_MAX - 0.5f)*dist_xy;
			}
			else
			{
				y = FinishY + (FinishX - finish[2*(length+step-i)]) ;
				shift_angle = shift_x = shift_y = 0.f;
			}
		    point.Clear();
			point.Translate( FinishX + shift_x, y + shift_y);
			point.Rotate(0.f, 0.f, shift_angle);
			letter->Path.Add(point);
	    }
		point.Alpha = (BYTE)(Alpha/2);
		point.Clear();
		point.Translate( FinishX , 2*FinishY + (FinishX - finish[0]) );
		point.Rotate(0., 0., 0.);
		letter->Path.Add(point);
		point.Alpha = 0;
		point.Clear();
		point.Translate( FinishX , 3*FinishY + (FinishX - finish[0]) );
		point.Rotate(0., 0., 0.);
		letter->Path.Add(point);

    }	
	return;
}

void AppearWedge(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
	int		 compare = RAND_MAX/2;
	float	 FinishX, FinishY;
	float	 delta;
	float	 scale;

    for ( index = 0; index < length; ++index)
    {
		FinishX = finish[2*index];
		FinishY = finish[2*index+1];
		letter = &(letters[index]);
	    point.Name = letter->Name;
	    point.Visible = TRUE; 
	    point.Alpha = (BYTE)0; 

		if ( rand() < compare )
		{
			for ( i = 0; i <= length; ++i)
			{
				delta = (float)i/length;
				point.Alpha = (BYTE)(Alpha * min(2*delta,1)); 
				scale = ((float)rand()/RAND_MAX) * 2;
				point.Clear();
				point.Translate( FinishX , -FinishX/4. + (FinishY + FinishX/4.)*delta);
    			point.Scale( scale , scale );
				letter->Path.Add(point);
			}
		} else
		{
			for ( i = 0; i <= length; ++i)
			{
				delta = (float)i/length;
				point.Alpha = (BYTE)(Alpha * min(2*delta,1)); 
				scale = ((float)rand()/RAND_MAX) * 2;
				point.Clear();
				point.Translate( FinishX , FinishY + FinishY+ FinishX/4.f + ( - FinishY - FinishX/4.f)*delta);
				point.Scale( scale , scale );
				letter->Path.Add(point);
			}
		}
		point.Alpha = (BYTE)Alpha; 
        point.Clear();
        point.Translate( FinishX , FinishY );
		point.Scale( 1 , 1 );
		letter->Path.Add(point);

    }	
	return;
}

void AppearWedgeBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
	int		 compare = RAND_MAX/2;
	float	 FinishX, FinishY;
	float	 LastX = finish[2*(length-1)];
	float	 delta;
	float	 scale;

    for ( index = 0; index < length; ++index)
    {
		FinishX = finish[2*index];
		FinishY = finish[2*index+1];
		letter = &(letters[index]);
	    point.Name = letter->Name;
	    point.Visible = TRUE; 
		point.Alpha = (BYTE)Alpha; 

		point.Clear();
        point.Translate( FinishX , FinishY );
		point.Scale( 1 , 1 );
		letter->Path.Add(point);

		if ( rand() < compare )
		{
			for ( i = 0; i <= length; ++i)
			{
				delta = 1 - (float)i/length;
				point.Alpha = (BYTE)(Alpha * min(2*delta,1)); 
				scale = ((float)rand()/RAND_MAX) * 2;
				point.Clear();
				point.Translate( FinishX , (FinishX-LastX)/4. + (FinishY - (FinishX-LastX)/4.)*delta);
    			point.Scale( scale , scale );
				letter->Path.Add(point);
			}
		} else
		{
			for ( i = 0; i <= length; ++i)
			{
				delta = 1 - (float)i/length;
				point.Alpha = (BYTE)(Alpha * min(2*delta,1)); 
				scale = ((float)rand()/RAND_MAX) * 2;
				point.Clear();
				point.Translate( FinishX , FinishY + FinishY - (FinishX-LastX)/4.f + ( - FinishY + (FinishX-LastX)/4.f)*delta);
				point.Scale( scale , scale );
				letter->Path.Add(point);
			}
		}
    }	
	return;
}

void AppearStream(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index, N, N_shade;

    float    LastX = finish[ (length-1)*2 ];
    float    FinishX, FinishY;
    float    StartX;
	int		 border = (length-1)/2;
	float	 FirstX = finish[0];
	int		 step = 5;
	int		 effect_length = 10;
	float	 x, y;
	float	 ShadeX;
	float	 b, delta;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = 0;

		FinishX = finish[2*index];
		FinishY = finish[2*index+1];

		if ( index <= border )
		{
			N = step * (border) + effect_length;
			N_shade = N/2;
			StartX = -10 - (FinishX-FirstX);
			ShadeX = finish[2*(border-index)];

			for ( i=0; i<=N_shade; i++ )
			{
				delta = (float)i/N_shade;
				x = StartX + (ShadeX - StartX)*delta;
				point.Alpha = (BYTE)(Alpha*delta);
				point.Clear();
				point.Translate(x, FinishY);
				point.Shear(-2 * delta, 0);
				letter->Path.Add(point);
			}
			point.Alpha = (BYTE)Alpha;
			int tmp = step * index;
			for ( i=0; i<=N; i++ )
			{
				if ( i <= tmp )
				{
					delta = ( tmp > 0 )? ((float)i/tmp) : 1;
					x = ShadeX + (finish[2*border] - ShadeX)*delta;
					point.Clear();
					point.Translate(x, FinishY);
					point.Shear(-(1-delta), 0);
					letter->Path.Add(point);
				} else if ( i <= tmp + effect_length )
				{
					delta = (float)(i - tmp)/effect_length;
					b = 20;
					x = finish[2*border] + (FinishX - finish[2*border])*delta;
					y = FinishY - b/effect_length * sqrt((double)effect_length*effect_length - (i - tmp)*(i - tmp));
					point.Clear();
					point.Translate(x, y);
					point.Shear(-(1-delta), 0);
					letter->Path.Add(point);
				} else
				{
					point.Clear();
					point.Translate(FinishX, FinishY);
					point.Shear(-0, 0);
					letter->Path.Add(point);
				}
			}
		} else
		{
			N = step * (length -1 - border) + effect_length;
			N_shade = N/2;
			StartX = LastX + 10 + FirstX + (LastX - FinishX);
			ShadeX = finish[2*(length - (index-border))];
			
			for ( i=0; i<=N_shade; i++ )
			{
				delta = (float)i/N_shade;
				x = StartX + (ShadeX - StartX)*delta;
				point.Alpha = (BYTE)(Alpha*delta);
				point.Clear();
				point.Translate(x, FinishY);
				point.Shear(2 * delta, 0);
				letter->Path.Add(point);
			}

			point.Alpha = (BYTE)Alpha;
			int tmp = step * (border+1 - (index-border));
			for ( i=0; i<=N; i++ )
			{
				if ( i <= tmp )
				{
					delta = ( tmp > 0 )? ((float)i/tmp) : 1;
					x = ShadeX + (finish[2*(border+1)] - ShadeX)*delta;
					point.Clear();
					point.Translate(x, FinishY);
					point.Shear(1-delta, 0);
					letter->Path.Add(point);
				} else if ( i <= tmp + effect_length )
				{
					delta = (float)(i - tmp)/effect_length;
					b = 20;
					x = finish[2*(border+1)] + (FinishX - finish[2*(border+1)])*delta;
					y = FinishY - b/effect_length * sqrt((double)effect_length*effect_length - (i - tmp)*(i - tmp));
					point.Clear();
					point.Translate(x, y);
					point.Shear(1-delta, 0);
					letter->Path.Add(point);
				} else
				{
					point.Clear();
					point.Translate(FinishX, FinishY);
					point.Shear(-0, 0);
					letter->Path.Add(point);
				}
			}
		}
	}
    return;
}

void AppearStreamBack(CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index, N, N_shade;

    float    LastX = finish[ (length-1)*2 ];
    float    FinishX, FinishY;
    float    StartX;
	int		 border = (length-1)/2;
	float	 FirstX = finish[0];
	int		 step = 5;
	int		 effect_length = 10;
	float	 x, y;
	float	 ShadeX;
	float	 b, delta;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

		FinishX = finish[2*index];
		FinishY = finish[2*index+1];

		point.Clear();
		point.Translate(FinishX, FinishY);
		point.Shear(0, 0);
		letter->Path.Add(point);

		if ( index <= border )
		{
			N = step * (border) + effect_length;
			N_shade = N/2;
			StartX = -10 - (FinishX-FirstX);
			ShadeX = finish[2*(border-index)];

			int tmp = step * index;
			for ( i=N; i>=0; i-- )
			{
				if ( i <= tmp )
				{
					delta = ( tmp > 0 )? ((float)i/tmp) : 1;
					x = ShadeX + (finish[2*border] - ShadeX)*delta;
					point.Clear();
					point.Translate(x, FinishY);
					point.Shear(-(1-delta), 0);
					letter->Path.Add(point);
				} else if ( i <= tmp + effect_length )
				{
					delta = (float)(i - tmp)/effect_length;
					b = 20;
					x = finish[2*border] + (FinishX - finish[2*border])*delta;
					y = FinishY - b/effect_length * sqrt((double)effect_length*effect_length - (i - tmp)*(i - tmp));
					point.Clear();
					point.Translate(x, y);
					point.Shear(-(1-delta), 0);
					letter->Path.Add(point);
				} else
				{
					point.Clear();
					point.Translate(FinishX, FinishY);
					point.Shear(-0, 0);
					letter->Path.Add(point);
				}
			}
			for ( i=N_shade; i>=0; i-- )
			{
				delta = (float)i/N_shade;
				x = StartX + (ShadeX - StartX)*delta;
				point.Alpha = (BYTE)(Alpha*delta);
				point.Clear();
				point.Translate(x, FinishY);
				point.Shear(-2 * delta, 0);
				letter->Path.Add(point);
			}
		} else
		{
			N = step * (length -1 - border) + effect_length;
			N_shade = N/2;
			StartX = LastX + 10 + FirstX + (LastX - FinishX);
			ShadeX = finish[2*(length - (index-border))];
			
			int tmp = step * (border+1 - (index-border));
			for ( i=N; i>=0; i-- )
			{
				if ( i <= tmp )
				{
					delta = ( tmp > 0 )? ((float)i/tmp) : 1;
					x = ShadeX + (finish[2*(border+1)] - ShadeX)*delta;
					point.Clear();
					point.Translate(x, FinishY);
					point.Shear(1-delta, 0);
					letter->Path.Add(point);
				} else if ( i <= tmp + effect_length )
				{
					delta = (float)(i - tmp)/effect_length;
					b = 20;
					x = finish[2*(border+1)] + (FinishX - finish[2*(border+1)])*delta;
					y = FinishY - b/effect_length * sqrt((double)effect_length*effect_length - (i - tmp)*(i - tmp));
					point.Clear();
					point.Translate(x, y);
					point.Shear(1-delta, 0);
					letter->Path.Add(point);
				} else
				{
					point.Clear();
					point.Translate(FinishX, FinishY);
					point.Shear(-0, 0);
					letter->Path.Add(point);
				}
			}
			for ( i=N_shade; i>=0; i-- )
			{
				delta = (float)i/N_shade;
				x = StartX + (ShadeX - StartX)*delta;
				point.Alpha = (BYTE)(Alpha*delta);
				point.Clear();
				point.Translate(x, FinishY);
				point.Shear(2 * delta, 0);
				letter->Path.Add(point);
			}
		}
	}
    return;
}

void WaveSimple( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX + 10;
	float    FinishX, FinishY;
    float    StartX, StartY;
    int      N = max(2*length, 200);
    float    x, y;
	float    fKoeff = 3.142592f / 100; 
	float    fHeigth = 20;
    float    step = wave_length / N;

	int		 fall_length = 8;
	int		 fall_step = fall_length/2;
	int		 N_fall = fall_step * (length-1) + 2*fall_length;
	float	 max_scale = 3;
    float    scale = 1;
    float    delta;
	int		 index_start;

	for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = StartY = finish[2*index+1];
        StartX = FinishX - wave_length;
	        
		point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

	    point.Clear();
        point.Translate(StartX, StartY);
	    point.Scale(1, 1);

	    letter->Path.Add(point);

        x = StartX; 
        for (i=0; i<N; i++)
        {
            y = FinishY - (cos((x+StartX) * fKoeff) + 1) * fHeigth;
            point.Clear();
            point.Translate(x, y);
		    point.Scale(1, 1);
		    letter->Path.Add(point);
            x += step;
        }
	    x = FinishX;
	    y = FinishY - (cos((x+StartX) * fKoeff) + 1) * fHeigth;
        point.Clear();
        point.Translate(FinishX, y);
	    point.Scale(1, 1);
	    letter->Path.Add(point);

		index_start = (length-1-index)*fall_step;
		for (i=0; i<N_fall; i++)
        {
            if ( i < index_start )
			{
				letter->Path.Add(point);
			}else if ( i < index_start + 2*fall_length)
			{
			    delta = (float)(i - index_start)/fall_length;
				point.Clear();
				if ( i - index_start < fall_length )
				{
					scale = 1 + (max_scale-1)*delta;					
					point.Translate(FinishX, y + (FinishY-y)*delta );
				}
				else
				{
					scale = max_scale + (1-max_scale)*(delta - 1);					
					point.Translate(FinishX, FinishY);
				}
				point.Scale(scale, scale);
				letter->Path.Add(point);
			} else
			{
			    point.Clear();
			    point.Translate(FinishX, FinishY);
			    point.Scale(1, 1);
				letter->Path.Add(point);
			}
        }

		point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1, 1);
        letter->Path.Add(point);
    }
    return;
}

void WaveSimpleBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX + 10;
	float    FinishX, FinishY;
    float    StartX;
    int      N = max(2*length, 200);
    float    x, y;
	float    fKoeff = 3.142592f / 100; 
	float    fHeigth = 20;
    float    step = wave_length / N;

	int		 fall_length = 8;
	int		 fall_step = fall_length/2;
	int		 N_fall = fall_step * (length-1) + 2*fall_length;
	float	 max_scale = 3;
    float    scale = 1;
    float    delta;
	int		 index_start;

	for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
        StartX = FinishX - wave_length;
	        
		point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

	    x = FinishX;
	    y = FinishY - (cos((x+StartX) * fKoeff) + 1) * fHeigth;
        point.Clear();
		point.Translate(FinishX, FinishY);	
		point.Scale(1, 1);
	    letter->Path.Add(point);

		index_start = (index)*fall_step;	
		for (i=0; i<N_fall; i++)
        {
            if ( i < index_start )
			{
				letter->Path.Add(point);
			}
			else if ( i < index_start + 2*fall_length)
			{
			    delta = (float)(i - index_start)/fall_length;
				point.Clear();
				if ( i - index_start < fall_length )
				{
					scale = 1 + (max_scale-1)*delta;					
					point.Translate(FinishX, FinishY);					
				}
				else
				{
					scale = max_scale + (1-max_scale)*(delta - 1);					
					point.Translate(FinishX, FinishY + (y-FinishY)*(delta-1));
				}
				point.Scale(scale, scale);
				letter->Path.Add(point);
			} 
			else
			{
				x = FinishX;
				point.Clear();
			    point.Translate(x, FinishY - (cos((x+StartX) * fKoeff) + 1) * fHeigth);
			    point.Scale(1, 1);
				letter->Path.Add(point);
			}
        }
        x = FinishX; 
        for (i=0; i<=N; i++)
        {
            y = FinishY - (cos((x+StartX) * fKoeff) + 1) * fHeigth;
            point.Alpha = (BYTE)(Alpha*(float)(N-i)/N);
			point.Clear();
            point.Translate(x, y);
		    point.Scale(1, 1);
		    letter->Path.Add(point);
            x += step;
        }
    }
    return;
}

void WaveZoom( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    float    FinishX, FinishY;
    int      step = 6;
    int      N = length-1 + 2*step;
    float    max_scale = 3;
    float    scale = 1;
    float    scale_step = (max_scale - 1)/step;
    float    alpha_step = (float)Alpha/step;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = TRUE;
        point.Alpha = 0;

        for (i=0; i<N; i++)
        {
            if ( i<index )
            {
                point.Alpha = 0;
                scale = 1;
            }
            else if ( i>=index+2*step )
            {
                point.Alpha = (BYTE)Alpha;
                scale = 1;
            }	  
            else
            {
				if ( i-index < step )
                {
                    point.Alpha = (BYTE)(alpha_step * (i-index+1));
                    scale = scale_step * (i-index) + 1;
                } else
				{
                    point.Alpha = (BYTE)Alpha;
                    scale = max_scale - scale_step * (i-index-step);
  				}
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(scale,scale);
            letter->Path.Add(point);
        }
        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }
    return;
}

void WaveZoomBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    float    FinishX, FinishY;
    int      step = 6;
    int      N = length-1 + 2*step;
    float    max_scale = 3;
    float    scale = 1;
    float    scale_step = (max_scale - 1)/step;
    float    alpha_step = (float)Alpha/step;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        for (i=0; i<N; i++)
        {
            if ( i<index )
            {
				point.Alpha = (BYTE)Alpha;
                scale = 1;
            }
            else if ( i>=index+2*step )
            {
                point.Alpha = 0;
                scale = 1;
            }
            else
            {
				if ( i-index < step )
                {
                    point.Alpha = (BYTE)Alpha;
					scale = scale_step * (i-index) + 1;
                } else
				{
                    point.Alpha = (BYTE)(alpha_step * (2*step - (i-index)));
                    scale = max_scale - scale_step * (i-index-step);
  				}
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(scale,scale);
            letter->Path.Add(point);
        }
        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }
    return;
}

void WaveZoomVertical( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    float    FinishX, FinishY;
    int      step = 6;
    int      N = length-1 + 2*step;
    float    max_scale = 6;
    float    scale = 1;
    float    scale_step = (max_scale - 1)/step;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = FALSE;
        point.Alpha = (BYTE)Alpha;

        for (i=0; i<N; i++)
        {
            if ( i<index )
            {
		        point.Visible = FALSE;
                scale = 1;
            }
            else if ( i>=index+2*step )
            {
		        point.Visible = TRUE;
                scale = 1;
            }
            else
            {
                point.Visible = TRUE;
				if ( i-index < step )
                    scale = scale_step * (i-index) + 1;
                else
					scale = max_scale - scale_step * (i-index-step);
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(1,scale);
            letter->Path.Add(point);
        }
        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }
    return;
}

void WaveZoomVerticalBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    float    FinishX, FinishY;
    int      step = 6;
    int      N = length-1 + 2*step;
    float    max_scale = 6;
    float    scale = 1;
    float    scale_step = (max_scale - 1)/step;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        for (i=0; i<N; i++)
        {
            if ( i<index )
            {
		        point.Visible = TRUE;
                scale = 1;
            }
            else if ( i>=index+2*step )
            {
		        point.Visible = FALSE;
                scale = 1;
            }
            else
            {
                point.Visible = TRUE;
				if ( i-index < step )
                    scale = scale_step * (i-index) + 1;
                else
					scale = max_scale - scale_step * (i-index-step);
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(1,scale);
            letter->Path.Add(point);
        }
        point.Visible = FALSE;
		point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }
    return;
}

void WaveFall( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX + 10;
    float    FinishX, FinishY;
    float    StartX, StartY;
    int      N = max(2*length, 300);
    float    x, y;
    float    fKoeff = 3.142592f / 100;
    float    fHeigth = 20;
    float    step = wave_length / N;

	int		 fall_length = 12;
	int		 fall_step = 6;
	int		 N_fall = fall_step * (length-1) + fall_length;
	float	 max_angle = 1080;
    float    delta;
	int		 index_start;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = StartY = finish[2*index+1];
        StartX = FinishX - wave_length;

        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        point.Clear();
        point.Translate(StartX, StartY);
        point.Rotate(0, 0, 0);

        letter->Path.Add(point);
        x = StartX; 
        for (i=0; i<N; i++)
        {
            y = FinishY - (-cos((x-LastX) * fKoeff) + 1) * fHeigth;
            point.Clear();
            point.Translate(x, y);
		    point.Rotate(0, 0, 0);
		    letter->Path.Add(point);
            x += step;
        }
	    x = FinishX;
	    y = FinishY - (-cos((x-LastX) * fKoeff ) + 1) * fHeigth;
        point.Clear();
        point.Translate(x, y);
	    point.Rotate(0, 0, 0);
	    letter->Path.Add(point);

		index_start = (length-1-index)*fall_step;
		for (i=0; i<N_fall; i++)
        {
            if ( i < index_start )
			{
				letter->Path.Add(point);
			}else if ( i < index_start + fall_length)
			{
			    delta = (float)(i - index_start)/fall_length;
				point.Clear();
				point.Translate(FinishX, y + (FinishY-y)*delta );
		        point.Rotate(0, 0, max_angle*delta);
				letter->Path.Add(point);
			} else
			{
			    point.Clear();
			    point.Translate(FinishX, FinishY);
		        point.Rotate(0, 0, max_angle);
				letter->Path.Add(point);
			}
        }
		point.Clear();
        point.Translate(FinishX, FinishY);
        point.Rotate(0, 0, max_angle);
        letter->Path.Add(point);
    }
    return;
}

void WaveFallBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX + 10;
    float    FinishX, FinishY;
    int      N = max(2*length, 300);
    float    x, y;
    float    fKoeff = 3.142592f / 100;
    float    fHeigth = 20;
    float    step = wave_length / N;

	int		 fall_length = 12;
	int		 fall_step = 6;
	int		 N_fall = fall_step * (length-1) + fall_length;
	float	 max_angle = 1080;
    float    delta;
	int		 index_start;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

		x = FinishX;
	    y = FinishY - (-cos((x-LastX) * fKoeff ) + 1) * fHeigth;
        
		point.Clear();
        point.Translate(FinishX, FinishY);
	    point.Rotate(0, 0, max_angle);
	    letter->Path.Add(point);

		index_start = index*fall_step;	
		for (i=0; i<N_fall; i++)
        {
            if ( i < index_start )
			{
				letter->Path.Add(point);
			}
			else if ( i < index_start + fall_length)
			{
			    delta = (float)(i - index_start)/fall_length;
				point.Clear();
				point.Translate(FinishX, FinishY + (y-FinishY)*delta );
		        point.Rotate(0, 0, max_angle*(1-delta));
				letter->Path.Add(point);
			} else
			{
			    point.Clear();
			    point.Translate(FinishX, y);
		        point.Rotate(0, 0, 0);
				letter->Path.Add(point);
			}
        }

        x = FinishX; 
        for (i=0; i<=N; i++)
        {
            y = FinishY - (-cos((x-LastX) * fKoeff) + 1) * fHeigth;
            point.Alpha = (BYTE)(Alpha*(float)(N-i)/N);
			point.Clear();
            point.Translate(x, y);
		    point.Rotate(0, 0, 0);
		    letter->Path.Add(point);
            x += step;
        }
    }
    return;
}

void WaveRunShift( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX + 10;
    float    FinishX, FinishY;
    float    StartX, StartY;
    int      N = max(length*2, 300);
    float    x, y, y_prev, shift;
    float    fKoeff = 3.142592f / 100;
    float    fHeigth = 20;
    float    step = wave_length / N;

	int		 fall_length = N/2/length;
    float    delta;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = StartY = finish[2*index+1];
        StartX = FinishX - wave_length;
        
        point.Clear();
        point.Translate(StartX, StartY);

    	letter->Path.Add(point);

        x = StartX; 
        for (i=0; i<N; i++)
        {
            y = FinishY - (-cos((x-LastX) * fKoeff) + 1) * fHeigth;
            point.Clear();
            point.Translate(x, y);
		    letter->Path.Add(point);
            x += step;
        }
	    x = FinishX;
	    y = FinishY - (-cos((x-LastX) * fKoeff ) + 1) * fHeigth;
        point.Clear();
        point.Translate(x, y);
	    letter->Path.Add(point);

		for (i=0; i<length; i++)
        {
            if ( index > length-1-i )
		    {
			    for (int j=0; j<fall_length; j++)
				{
					point.Clear();
					point.Translate(FinishX, FinishY);
				    letter->Path.Add(point);
				}
		    }
			else
		    {
			    shift = finish[(length-i-1)*2];
			    y_prev = y;
				y = FinishY - (-cos((x-shift)*fKoeff ) + 1) * fHeigth;
				for (int j=0; j<fall_length; j++)
				{
				    delta = (float)j/fall_length;
					point.Clear();
					point.Translate( FinishX, y_prev + (y-y_prev)*delta );
				    letter->Path.Add(point);
				}
		    }
        }
    }
    return;   	
}

void WaveRunShiftBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX + 10;
    float    FinishX, FinishY;
    int      N = max(length*2, 300);
    float    x, y, y_prev, shift;
    float    fKoeff = 3.142592f / 100;
    float    fHeigth = 20;
    float    step = wave_length / N;

	int		 fall_length = N/2/length;
    float    delta;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
        
	    x = FinishX;
		y = FinishY;
        point.Clear();
        point.Translate(x, y);
	    letter->Path.Add(point);

		for (i=0; i<length; i++)
        {
			if ( i < length-1-index )
		    {
			    for (int j=0; j<fall_length; j++)
				{
					point.Clear();
					point.Translate(FinishX, FinishY);
				    letter->Path.Add(point);
				}
		    }
			else
		    {
			    shift = finish[(length-i-1)*2];
				y_prev = y;
				y = FinishY - (-cos((x-shift)*fKoeff ) + 1) * fHeigth;
				for (int j=0; j<fall_length; j++)
				{
				    delta = (float)j/fall_length;
					point.Clear();
					point.Translate( FinishX, y_prev + (y-y_prev)*delta );
					letter->Path.Add(point);
				}
		    }
        }
		x = FinishX; 
        for (i=0; i<=N; i++)
        {
			y = FinishY - (-cos((x-shift) * fKoeff) + 1) * fHeigth;         
			point.Alpha = (BYTE)(Alpha*(float)(N-i)/N);
			point.Clear();
            point.Translate(x, y);
		    letter->Path.Add(point);
            x += step;
        }
    }
    return;   	
}

void WaveSwing( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX*1.5f;
    float    FinishX, FinishY;
    float    y;
    float    fKoeff = 3.142592f / wave_length;
    float    fHeight = 80;
	int		 step = 5;
    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
		point.Alpha = 0;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
 
		y = FinishY - sin(fabs(FinishX+10)*fKoeff) * fHeight;
		point.Clear();
		point.Translate(FinishX, y);
		letter->Path.Add(point);

		for (i=0; i<length; i++)
        {
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(1-(float)i/length * 0.2);
			point.Clear();
			
			if ( (i<=index-step) && (index != step))
				point.Alpha = (BYTE)(Alpha/2 * (float)i/(index-step));
			else if ( i<=index )
				point.Alpha = (BYTE)((2-(float)(index-i)/step)*Alpha/2); 
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}	 
		point.Alpha = (BYTE)Alpha;
		for (i=length-1; i>=0; i--)
        {
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.8-(float)(length-i-1)/length * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		for (i=0; i<length; i++)
		{
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.6-(float)i/length * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		for (i=length-1; i>=0; i--)
		{
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.4-(float)(length-i-1)/length * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}

		for (i=0; i<length; i++)        
		{
			if ( i<index )
			{
				y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.2-(float)i/length * 0.2);
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			}
			else
			{
				point.Clear();
				point.Translate(FinishX, FinishY);
				letter->Path.Add(point);
			}
		}	
	}
    return;   	
}

void WaveSwingBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    wave_length = LastX*1.5f;
    float    FinishX, FinishY;
    float    y;
    float    fKoeff = 3.142592f / wave_length;
    float    fHeight = 80;
	int		 step = 5;
    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
 
		point.Alpha = (BYTE)Alpha;
		for (i=length-1; i>=0; i--)        
		{
			if ( i<=index )
			{
				y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.2-(float)i/length * 0.2);
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			}
			else
			{
				point.Clear();
				point.Translate(FinishX, FinishY);
				letter->Path.Add(point);
			}
		}
		for (i=0; i<length; i++)
		{
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.4-(float)(length-i-1)/length * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		for (i=length-1; i>=0; i--)
		{
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.6-(float)i/length * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		for (i=0; i<length; i++)
        {
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(0.8-(float)(length-i-1)/length * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		for (i=length-1; i>=0; i--)
        {
			y = FinishY - sin(fabs(FinishX-finish[i*2])*fKoeff) * fHeight*(1-(float)i/length * 0.2);
			point.Clear();
			
			if ( (i<=index-step) && (index != step))
				point.Alpha = (BYTE)(Alpha/2 * (float)i/(index-step));
			else if ( i<=index )
				point.Alpha = (BYTE)((2-(float)(index-i)/step)*Alpha/2);
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		point.Alpha	= 0;
		y = FinishY - sin(fabs(FinishX+10)*fKoeff) * fHeight;
		point.Clear();
		point.Translate(FinishX, y);
		letter->Path.Add(point);

	}
    return;   	
}

void WaveRun( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    FirstX = finish[0];
	float    wave_length;
    float    FinishX, FinishY;
    float    y;
    float    fKoeff, prev_Koeff;
    float    fHeight = 20;
	float	 step = 10;
	float	 shift;
    int		 N = 50;
	float	 L;
	for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
		point.Alpha = 0;
        
		FinishX = finish[2*index];
        FinishY = finish[2*index+1];
 
		wave_length = 350;
		fKoeff = 2 * 3.142592f / wave_length;	
		shift = FirstX;
		L = ((int)((LastX - FirstX)/wave_length)+2) * wave_length;
		step = L/N;

		for (i=0; i<=N; i++)
        {
			y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*(1-(float)i/N * 0.2);
			point.Clear();
			point.Alpha = (BYTE)((float)i/N*Alpha);		
			point.Translate(FinishX, y);
			letter->Path.Add(point);
			shift += step;		
		}
		point.Alpha = (BYTE)Alpha;
		prev_Koeff = fKoeff;
		wave_length = 200;
		fKoeff = 2 * 3.142592f / wave_length;	
		L = ((int)((LastX - FirstX)/wave_length)+2) * wave_length;
		step = L/N;
		shift = FirstX;
		for (i=0; i<=N; i++)
        {
			if ( shift > finish[2*index] )
				y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*(0.8-(float)i/N * 0.2);
			else
				y = FinishY - (-cos((FinishX-shift)*prev_Koeff ) + 1) * fHeight*(0.8-(float)i/N * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
			shift += step;		
		}
		prev_Koeff = fKoeff;
		wave_length = 100;
		fKoeff = 2 * 3.142592f / wave_length;	
		L = ((int)((LastX - FirstX)/wave_length)+1) * wave_length;
		step = L/N;
		shift = FirstX;
		for (i=0; i<=N; i++)
        {
			if ( shift > finish[2*index] )
				y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*(0.6-(float)i/N * 0.2);
			else
				y = FinishY - (-cos((FinishX-shift)*prev_Koeff ) + 1) * fHeight*(0.6-(float)i/N * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
			shift += step;		
		}

		for (i=0; i<length; i++)
        {
			shift = finish[i*2];		
			if ( i > index )
				y = FinishY;
			else
				y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*0.4;
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
	}
    return;   	
}

void WaveRunBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    LastX = finish[ (length-1)*2 ];
    float    FirstX = finish[0];
	float    wave_length;
    float    FinishX, FinishY;
    float    y;
    float    fKoeff, prev_Koeff;
    float    fHeight = 20;
	float	 step = 10;
	float	 shift;
    int		 N = 50;
	float	 L;
	for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
        
		FinishX = finish[2*index];
        FinishY = finish[2*index+1];

		point.Clear();
		point.Translate(FinishX, FinishY);
		letter->Path.Add(point);

		wave_length = 100;
		fKoeff = 2 * 3.142592f / wave_length;	
		L = ((int)((LastX - FirstX)/wave_length)+1) * wave_length;
		step = L/N;
		shift = FirstX;
		for (i=length-1; i>=0; i--)
        {
			shift = finish[i*2];		
			if ( i > index )
				y = FinishY;
			else
				y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*0.4;
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		}
		
		prev_Koeff = fKoeff;
		wave_length = 200;
		fKoeff = 2 * 3.142592f / wave_length;	
		L = ((int)((LastX - FirstX)/wave_length)+2) * wave_length;
		step = L/N;
		shift = FirstX;
		
		for (i=N; i>=0; i--)
        {
			if ( shift > finish[2*index] )
				y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*(0.6-(float)i/N * 0.2);
			else
				y = FinishY - (-cos((FinishX-shift)*prev_Koeff ) + 1) * fHeight*(0.6-(float)i/N * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
			shift += step;		
		}

		wave_length = 350;
		prev_Koeff = fKoeff;
		fKoeff = 2 * 3.142592f / wave_length;	
		shift = FirstX;
		L = ((int)((LastX - FirstX)/wave_length)+2) * wave_length;
		step = L/N;

		for (i=N; i>=0; i--)
        {
			if ( shift > finish[2*index] )
				y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*(0.8-(float)i/N * 0.2);
			else
				y = FinishY - (-cos((FinishX-shift)*prev_Koeff ) + 1) * fHeight*(0.8-(float)i/N * 0.2);
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
			shift += step;		
		}

		for (i=N; i>=0; i--)
        {
			y = FinishY - (-cos((FinishX-shift)*fKoeff ) + 1) * fHeight*(1-(float)i/N * 0.2);
			point.Clear();
			point.Alpha = (BYTE)((float)i/N*Alpha);		
			point.Translate(FinishX, y);
			letter->Path.Add(point);
			shift += step;		
		}
	}
    return;   	
}

void WaveShift( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    FinishX, FinishY;
	int		 step_letters = 8;
    int      N = length;
    float    y, shift;
    float    fKoeff = 3.142592f / 70;
    float    fHeigth = 20;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];
		
		if ( index <= step_letters )
		{
			point.Visible = TRUE;
			point.Clear();
		    point.Translate(-10, FinishY);
		  	letter->Path.Add(point);
		}
		else
		{
			point.Visible = FALSE;
			point.Clear();
		    point.Translate(FinishX, FinishY);
		  	letter->Path.Add(point);
		}
	
        for (i=0; i<N; i++)
        {
			if ( i>=index )
			{
				point.Visible = TRUE;
				point.Clear();
				point.Translate(FinishX, FinishY);
				letter->Path.Add(point);
			} else if ( i>index - step_letters )
			{
				shift = finish[i*2];
				y = FinishY - (sin((FinishX-shift)*fKoeff )) * fHeigth;
				point.Visible = TRUE;
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			} 
			else if ( i==index - step_letters )
			{
				shift = finish[i*2];
				y = FinishY - (sin((FinishX-shift)*fKoeff )) * fHeigth;
				point.Visible = FALSE;
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			}
			else
			{
				point.Visible = FALSE;
				point.Clear();
				point.Translate(FinishX, FinishY);
				letter->Path.Add(point);
			}
		}
    }
    return;   	
}

void WaveShiftBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;

    float    FinishX, FinishY;
	int		 step_letters = 8;
    int      N = length;
    float    y, shift;
    float    fKoeff = 3.142592f / 70;
    float    fHeigth = 20;

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;
        point.Alpha = (BYTE)Alpha;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

		point.Visible = TRUE;
		point.Clear();
		point.Translate(FinishX, FinishY);
		letter->Path.Add(point);

        for (i=0; i<N; i++)
        {
			if ( i<=index )
			{
				shift = finish[i*2];
				point.Visible = TRUE;
				point.Clear();
				point.Translate(FinishX, FinishY);
				letter->Path.Add(point);
			}
			else if ( i<index+step_letters )
			{
				shift = finish[i*2];
				y = FinishY + (sin((FinishX-shift)*fKoeff )) * fHeigth;
				point.Visible = TRUE;
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			}
			else if ( i==index+step_letters )
			{
				shift = finish[i*2];
				y = FinishY + (sin((FinishX-shift)*fKoeff )) * fHeigth;
				point.Visible = FALSE;
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			}
			else
			{
				point.Visible = FALSE;
				point.Clear();
				point.Translate(FinishX, FinishY);
				letter->Path.Add(point);
			}
		}
		int delta = finish[min(1,length-1)*2]-finish[0];
		
		if ( index+step_letters > length-1 )
		{
			for (i=0; i<index+step_letters-(length-1); i++)
			{
				shift += delta;
				y = FinishY + (sin((FinishX-shift)*fKoeff )) * fHeigth;
				point.Visible = TRUE;
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			
			}
			for (i=0; i<(length-1-index); i++)
			{
				shift += delta;
				y = FinishY + (sin((FinishX-shift)*fKoeff )) * fHeigth;
				point.Visible = FALSE;
				point.Clear();
				point.Translate(FinishX, y);
				letter->Path.Add(point);
			}
			shift += delta;
			y = FinishY + (sin((FinishX-shift)*fKoeff )) * fHeigth;
			point.Visible = FALSE;
			point.Clear();
			point.Translate(FinishX, y);
			letter->Path.Add(point);
		
		} else
		{
			for (i=0; i<step_letters; i++)
			{
				point.Visible = FALSE;
				point.Clear();
				point.Translate(finish[2*(length-1)]+10, FinishY);
				letter->Path.Add(point);		
			}
		}
    }
    return;   	
}

void WavePulse( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    float    FinishX, FinishY;
    int      step = 6;
    int      N = (length/(2*step) + 3)*2*step;
    float    max_scale = 4.f;
	float    min_scale = 0.2f;
    float    scale = 1;
    float    scale_step = (max_scale - min_scale)/step;
    int	     rest;	

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = FALSE;
        point.Alpha = (BYTE)Alpha;

        for (i=0; i<N; i++)
        {
            if ( i<index )
				scale = min_scale;
            else
            {
				point.Visible = TRUE;  
				rest = (i-index)%(2*step);
				if ( rest < step )
                    scale = scale_step * rest + min_scale;
                else
                    scale = max_scale - scale_step * (rest-step);
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(1,scale);
            letter->Path.Add(point);
        }
		for (i=0; i<length; i++)
        {
            if ( i>=index )
				scale = 1;
            else
            {
				point.Visible = TRUE;  
				rest = (index-i)%(2*step);
				if ( rest < step )
                    scale = scale_step * rest + min_scale;
                else
                    scale = max_scale - scale_step * (rest-step);
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(1,scale);
            letter->Path.Add(point);
        }
        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);
    }
    return;
}

void WavePulseBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
    Letter * letter;
    Waypoint point; 
    int      length = letters.GetSize();
    int      i, index;
    float    FinishX, FinishY;
    int      step = 6;
    int      N = (length/(2*step) + 3)*2*step;
    float    max_scale = 4.f;
	float    min_scale = 0.2f;
    float    scale = 1;
    float    scale_step = (max_scale - min_scale)/step;
    int	     rest;	

    for ( index = 0; index < length; ++index)
    {
        letter = &(letters[index]);
        point.Name = letter->Name;

        FinishX = finish[2*index];
        FinishY = finish[2*index+1];

        point.Visible = TRUE;
        point.Alpha = (BYTE)Alpha;

        point.Clear();
        point.Translate(FinishX, FinishY);
        point.Scale(1,1);
        letter->Path.Add(point);

		for (i=0; i<length; i++)
        {
            if ( i>=index )
			{
				point.Visible = TRUE;  
				rest = (i-index)%(2*step);
				if ( rest < step )
                    scale = scale_step * rest + min_scale;
                else
                    scale = max_scale - scale_step * (rest-step);
			}
            else
            {
				scale = 1;
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(1,scale);
            letter->Path.Add(point);
		}
        for (i=0; i<=N; i++)
        {
            if ( i<N-(length-1-index) )
			{
				point.Visible = TRUE;  
				rest = (N-(length-1-index)-i)%(2*step);
				if ( rest < step )
                    scale = scale_step * rest + min_scale;
                else
                    scale = max_scale - scale_step * (rest-step);
			}
            else
            {
				point.Visible = FALSE;  
				scale = min_scale;
            }
            point.Clear();
            point.Translate(FinishX, FinishY);
		    point.Scale(1,scale);
            letter->Path.Add(point);
        }
		point.Visible = FALSE;
		point.Clear();
        point.Translate(FinishX, FinishY);
		point.Scale(1,scale);
        letter->Path.Add(point);
    }
    return;
}


//new
void Ejection( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha)
{   Letter* letter;
    Waypoint point; 
    float x, y, start;
	int length;
	length = letters.GetSize();
	if(length > 1)
		start = (finish[2] - finish[0])*10;
	else
		start = 100;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = 0;

		x = finish[2*i];			   
		y = finish[2*i+1];

		for(int j = start; j >= 0; j -= start/5)
		{	
			point.Clear();
			if(j != start)
				point.Alpha += Alpha/10;
			
			point.Translate(x, y + j);
			letter->Path.Add(point);

			if (start/5 < 1)
				break;
		}
			
		point.Clear();
		point.Alpha += Alpha/10;
		point.Translate(x, y - start/5);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		letter->Path.Add(point);
	}
}				
void Ellipse( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, R, centerX, centerY;
	int sch = 0;
	if(length > 1)
		R = (finish[2] - finish[0])*3;
	else
		R = 30;
	for (int i = 0; i < length; i++)
	{
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;
		x = finish[2*i];
		y =	finish[2*i+1];
		centerX = x;
		centerY = y + R;

		for(int temp = 0; temp < length*10; temp++)
		{  
			if((i*10) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				letter->Path.Add(point);
			}
			else
			{
				for(int j = 0; j < R; j++)
				{	
					y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
					y -= finish[2*i+1];
					y /= 2;
					y += finish[2*i+1];			
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(x, y);
					letter->Path.Add(point);
					x++;
				}

				for(int j = 0; j < R*2; j++)
				{	
					y = centerY + sqrt(R*R - (x - centerX)*(x - centerX));
					y -= finish[2*i+1];
					y /= 2;
					y += finish[2*i+1];		
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x--;
				}

				for(int j = 0; j < R + 1; j++)
				{	
					y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
					y -= finish[2*i+1];
					y /= 2;
					y += finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x++;
				}
				for(int j = 0;j < length*10 - sch;j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
				}

			break;
			}
		}
	}
}	

void Wheel( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;
	for (int i = 0; i < length; i++)
	{
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = 0;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*10; temp++)
		{  
			if((i*10) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Rotate(0, 0, -540);
				point.Scale(0.4, 0.4);
				letter->Path.Add(point);
			}
			else
			{
				for(float j = 0.; j < 50.; j += 1.)
				{
					float t = 54 * j/5;
					float s = 0.06 * j/5;
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, -540 + t);
					point.Scale(0.4 + s, 0.4 + s);
					point.Alpha += Alpha/50 ;
					letter->Path.Add(point);
				}

				for(int j = 0; j < length*10 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, 0);
					point.Scale(1, 1);
					point.Alpha = (BYTE)Alpha;
					letter->Path.Add(point);
				}
			break;
			}
		}
	}
}		

void Miscarry( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{   
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	float end = finish[2*(length - 1)];

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = 0;
		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();
		point.Translate(x, y);
		point.Rotate(0, 0, 90);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = 0;
		if(i == 1)
			point.Alpha = (BYTE)Alpha/10;
		if(i==0)
			point.Alpha = (BYTE)Alpha;

		point.Translate(end, y);
		point.Rotate(0, 0, 90);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		point.Rotate(0, 0, 0);
		letter->Path.Add(point);
	}
}				

void Rotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, s;	  //s - koef. scale
	int sch = 0;
	for (int i = 0; i < length; i++)
	{  
		sch=0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = 0;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*10; temp++)
		{  
			if((i*10) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				for(float j = 48.; j >= 0.; j -= 1.)
				{
					if(j >= 36)
						s = 0.0833*(j - 36);
					if((j < 36) && (j >= 24))
						s = -0.0833*(36 - j);
					if((j < 24) && (j >= 12))
						s = -0.0833*(j - 12);
					if(j < 12)
						s = 0.0833*(12 - j);
					
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(s, 1);
					point.Alpha += Alpha/48 ;
					letter->Path.Add(point);
				}

				for(int j = 0; j < length*10 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Alpha = (BYTE)Alpha;
					letter->Path.Add(point);
				}
			break;
			}
		}
	}
}		


void Increase( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{   
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, start;
	if(length > 1)
		start = (finish[2] - finish[0])*10;
	else
		start = 100;

    for (int i = 0; i < length; i++)
    {
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = 0;

		x = finish[2*i];
		y = finish[2*i+1];

		x = (finish[2*length - 2] - finish[0])/2;

		point.Clear();
		point.Translate(x, y + start);
		point.Scale(0.5, 0.5);
		letter->Path.Add(point);

		point.Clear();
		if(i < 3)		
			point.Alpha = Alpha/1.5;
		point.Translate(x, y + start);
		point.Scale(1.5, 1.5);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = Alpha/3;
		if(i > 2)
			point.Alpha = 0;
		point.Translate(x, y + start);
		point.Scale(3, 7);
		letter->Path.Add(point);

		x = finish[2*i];
		y = finish[2*i+1];
		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		point.Scale(1, 1);
		letter->Path.Add(point);
	}
}		


void TwoString( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, scl, str1, str2, step;
	int sch = 0;
	if(length > 1)
	{
		str1 = (finish[2]-finish[0])*5;
		str2 = (finish[2]-finish[0])*10;
		step = (finish[2]-finish[0])*2;
	}
	else
	{
		str1 = 50;
		str2 = 100;
		step = 20;
	}
	for (int i = 0; i < length; i++)
	{
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;							
		point.Alpha =(BYTE)0;

		for(int temp = -1; temp < length*3; temp++)
		{  
			if((i*3) > temp)
			{
				sch++;
				if(i < length/2)
				{
					y = finish[2*i+1] + str1;
					x = finish[0] + step*i;
				}
			else
			{
				y = finish[2*i+1] + str2;
				x = finish[0] + step*(-length/2 + i);
			}
				point.Clear();
				point.Translate(x, y);
				point.Scale(2, 2);
				letter->Path.Add(point); 
			}   

			else
			{
				scl = 2.;	 
				for(int j = 0; j < 50; j++)
				{
					point.Clear();
					point.Translate(x += step/50, y += step/100 + i/50);
					point.Scale(scl, scl);
					point.Alpha += Alpha/50;
					letter->Path.Add(point);
					scl -= .02;
				}	
				x = finish[2*i] - 10;
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Alpha = (BYTE)Alpha;
				letter->Path.Add(point);


				for(int j = 0; j < 10; j++)
				{
					x++;		
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Alpha = (BYTE)Alpha;
					letter->Path.Add(point);
				}

				for(int j = 0; j < length*3 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Alpha = (BYTE)Alpha;
					letter->Path.Add(point);
				}
				break;
			}

		}
	}
}		

void Tumbling( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int      length = letters.GetSize();
	float x, y, start, stepX, stepY, scl;
	int sch = 0;
	start = 0;
	if(length > 1)
	{
		stepX = (finish[2] - finish[0]) * 1.7/10;
		stepY = (finish[1] - start)/10;
	}
	else
	{
		stepX = 1.7;
		stepY = (finish[1] - start)/10;
	}


	for (int i = 0; i < length; i++)
	{
		sch = 0;
		scl = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;
		x = finish[2*i] + stepX*10;
		y = start;

		for(int temp = 0; temp < length*1.5; temp++)
		{  
			if((i*1.5) >= temp)
			{
				sch++;
				x = finish[2*i] + stepX*10;
				y = start;
				point.Clear();
				point.Translate(x, y);

				point.Rotate(0, 0, -270);
				point.Scale(0, 0);
				letter->Path.Add(point);
			}
			else
			{
				point.Alpha = (BYTE)Alpha;
				for(int j = 10; j >= 0; j--)
				{ 
					if(j > 7) 
						scl -= 0.333;
					else 
						scl += 0.25;

					point.Clear();
					point.Translate(x, y);

					point.Rotate(0, 0, -27*j);
					point.Scale(scl, 1);
					letter->Path.Add(point);
					if(j <= 5)
						x -= stepX*2;
					
					y += stepY;
				}

				for(int j = 0; j < length*1.5 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);

					point.Rotate(0, 0, 0);
					point.Scale(1, 1);
					letter->Path.Add(point);
				}
			break;
			}

		}   
	}
}		

void Leave( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	int i, j;
	float x, y, start, step;
	if(length > 1)
		start = (finish[2] - finish[0])*6;
	else
		start = 60;
	step = start / length;
	for ( i = length - 1; i >= 0; i--)
	{  
		x = finish[2*i];
		y = finish[2*i+1];

		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE; 
		point.Alpha = (BYTE)0; 

		point.Clear();
		point.Translate(x - start, y);
		letter->Path.Add(point);

		for ( j = length - 1; j >= 0; j--)
		{	
			point.Clear();
			if (i >= j)
			{ 
				point.Alpha = (BYTE)Alpha;
				point.Translate(x - start, y);
			}
			else		
			{   
				point.Alpha = 0;
				point.Translate(x - start, y);
			}
			x += step;
			letter->Path.Add(point);

		}
		x = finish[2*i];
		point.Clear();
		point.Translate(x, y);
		letter->Path.Add(point);
	}	
}		
void Unfold( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, start, stepX, stepY;
	start = -30;
	stepY = (finish[1] - start)/50;
	int sch = 0;
	for (int i = 0; i < length; i++)
	{  
		sch = 0;
		stepX = (finish[2*i] - start)/50;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*4; temp++)
		{  
			if((i*4) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(start, start);
				letter->Path.Add(point);
			}
			else
			{
				x = y = start;
				for(float j = 0.; j < 50.; j += 1.)
				{
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x += stepX;
					y += stepY;
				}

				for(int j = 0; j < length*4 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
				}
				break;
			}

		}
	}
}		
void Centrifuge( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;

	center = (finish[2*(length - 1)] + finish[0])/2;
	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(center, y);	
		point.Scale(0, 0);
		point.Rotate(center, 0, -720);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Rotate(center, 0, 0);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

	}
}		
void HorizontalRotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length - 1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha/10;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 4; j++)
		{
			point.Clear();	
			point.Translate(center, finish[2*i+1]/2);

			if(j == 0)
				point.Scale(0, 1);
			else
			{
				if(j%2 == 0)
					point.Scale(-1, 1);
				else
					point.Scale(1, 1);
			}
			point.Translate(-center + x, finish[2*i+1]/2); 
			letter->Path.Add(point);
			point.Alpha =(BYTE)Alpha;
		}

	}
}

void HorizontalRotation3D( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	float length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (float i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha/10;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 6; j++)
		{
			point.Clear();	
			point.Translate(center, finish[2*i+1]/2);
			if(j == 0)
				point.Scale(0, 1);
			if(j == 1)
				point.Scale(1, 1);
			if(j == 2)
				point.Scale(0, 1);
			if(j == 3)
				point.Scale(-1, 1);
			if(j == 4)
				point.Scale(0, 1);
			if(j == 5)		   
				point.Scale(1, 1);

			point.Translate(-center + x, finish[2*i+1]/2); 

			if((j == 0) || (j == 4))
				point.Scale(4.1-i*4/length,4.1-i*4/length);
			if((j == 1) || (j == 3) || (j == 5))
				point.Scale(1, 1);
			if(j == 2)
				point.Scale(4.1 - (4 - i*4/length), 4.1 - (4 - i*4/length));
			letter->Path.Add(point);
			point.Alpha = (BYTE)Alpha;
		}

	}
}		


void VerticalRotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;
	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha/10;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 4; j++)
		{
			point.Clear();	
			point.Translate(center,finish[2*i+1]);
			if(j == 0)
				point.Scale(1, 0);
			else
			{
				if(j%2 == 0)
					point.Scale(1, -1);
				else
					point.Scale(1, 1);
			}
			point.Translate(-center + x, 0); 
			letter->Path.Add(point);
			point.Alpha = (BYTE)Alpha;
		}

	}
}		



void CheChe( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;

	for(int i = 0; i < length; i++)
	{ 
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length+1; temp++)
		{  
			if((i) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);
			}
			else
			{
				x = finish[2*i];
				y = finish[2*i+1];
				float temp = 1.;
				float temp2 = 1.;
				int koefAlpha = 0; 
				for(int z = 0; z < 5; z++ )
				{

					point.Alpha += Alpha/5;
					if(point.Alpha > 255)
						point.Alpha = 255;
					for(float j = 1.; j >= 0.5; j -= 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);
						if(point.Alpha != 0)
							koefAlpha = point.Alpha;
						if((temp > 37))
						{		   
							point.Alpha = 0;
						}
						else 
						{
							point.Alpha = koefAlpha;
						}

						if(j > 0.75)
						{
							point.Scale(temp2, temp2);
							point.Rotate(0, 0, temp);
							temp2 -= 0.15;
							temp +=	8;
						}
						else 
						{
							point.Scale(temp2, temp2);
							point.Rotate(0, 0, temp);
							temp2 += 0.16;
							temp -=	8;
						}
						letter->Path.Add(point);
					}
					
					

				}					
				for(int j = 0; j < length - sch+1; j++)
				{   point.Alpha = Alpha;
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Rotate(0, 0, 0);
					letter->Path.Add(point);
				}	
				break;
			}
		}
	}
}	

//Erase new
void EjectionBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{  
	Letter * letter;
	Waypoint point; 
	int      length = letters.GetSize();
	int      i;
	float    x, y, start;
	if(length > 1)
		start = (finish[2] - finish[0])*10;
	else
		start = 100;

	for(i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha -= Alpha/10;
		point.Translate(x, y - start/5);
		letter->Path.Add(point);

		for(int j = 0; j < start; j += start/5)
		{
			point.Clear();
			if(j != start)
				point.Alpha -= Alpha/10;
			point.Translate(x, y + j);
			letter->Path.Add(point);
		}
		point.Clear();
		point.Alpha = 0;
		point.Translate(x, y + start);
		letter->Path.Add(point);


	}

}				
void EllipseBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, R, centerX, centerY;
	int sch = 0;
	if(length > 1)
		R = (finish[2] - finish[0])*3;
	else
		R = 30;
	for (int i = 0; i < length; i++)
	{
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
		x = finish[2*i];
		y =	finish[2*i+1];
		centerX = x;
		centerY = y + R;

		for(int temp = 0; temp < length*10; temp++)
		{  
			if((i*10) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				letter->Path.Add(point);
			}
			else
			{
				for(int j = 0; j < R; j++)
				{	
					y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
					y -= finish[2*i+1];
					y /= 2;
					y += finish[2*i+1];			
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x++;
				}

				for(int j = 0; j < R*2; j++)
				{	
					y = centerY + sqrt(R*R - (x - centerX)*(x - centerX));
					y -= finish[2*i+1];
					y /= 2;
					y += finish[2*i+1];		
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x--;
				}

				for(int j = 0; j < R + 1; j++)
				{	
					y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
					y -= finish[2*i+1];
					y /= 2;
					y += finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x++;
				}
				for(int j = 0;j < length*10 - sch;j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Alpha = (BYTE)0;
					point.Translate(x, y);
					letter->Path.Add(point);
				}

			break;
			}
		}
	}
}	
void WheelBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, R = 30;
	int sch = 0;
	for (int i = 0; i < length; i++)
	{  
		sch=0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*10; temp++)
		{  
			if((i*10) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Rotate(0, 0, 1);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				for(float j = 0.; j < 50.; j += 1.)
				{
					float t = -54 * j/5;
					float s = 1 - 0.06 * j/5;
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, t);
					point.Scale(s, s);
					point.Alpha -= Alpha/50 ;
					letter->Path.Add(point);

				}

				for(int j = 0; j < length*10 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, -540);
					point.Scale(0.4, 0.4);
					point.Alpha = (BYTE)0 ;
					letter->Path.Add(point);
				}
				break;
			}

		}
	}
}		
void MiscarryBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{  
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int i;
	float x, y;
	float end = finish[2*(length-1)];

	for(i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;


		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		point.Rotate(0, 0, 0);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = 0;
		if(i == 1)
			point.Alpha = (BYTE)Alpha/10;
		if(i == 0)
			point.Alpha = (BYTE)Alpha;
		point.Translate(end, y);
		point.Rotate(0, 0, 90);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = 0;
		point.Translate(x, y);
		point.Rotate(0, 0, 90);
		letter->Path.Add(point);
	}
}				
void RotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, s;
	int sch = 0;
	for (int i = 0; i < length; i++)
	{  
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*10; temp++)
		{  
			if((i*10) >= temp)
			{
				sch++;
				x=finish[2*i];
				y=finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				for(float j = 48.; j >= 0.; j -= 1.)
				{
					if (j >= 36)
						s = 0.0833*(j - 36);
					if ((j < 36) && (j >= 24))
						s = -0.0833*(36 - j);
					if ((j < 24) && (j >= 12))
						s = -0.0833*(j - 12);
					if (j < 12)
						s = 0.0833*(12 - j);
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(s, 1);
					point.Alpha -= Alpha/48 ;
					letter->Path.Add(point);
				}

			
				for(int j = 0; j < length*10 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Alpha = (BYTE)0 ;
					letter->Path.Add(point);
				}
				break;
			}

		}
	}
}		

void IncreaseBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{   
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int i;
	float x, y, start;
	if(length > 1)
		start = (finish[2] - finish[0])*10;
	else
		start = 100;

	for (i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		point.Scale(1, 1);
		letter->Path.Add(point);

		x = (finish[2*length-2] - finish[0])/2;

		point.Clear();
		point.Alpha = Alpha/3;
		if(i > 2)
			point.Alpha = 0;
		point.Translate(x, y + start);
		point.Scale(3, 7);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = 0;
		if(i < 3)		
			point.Alpha = Alpha/1.5;
		point.Translate(x, y + start);
		point.Scale(1.5, 1.5);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = 0;
		point.Translate(x, y + start);
		point.Scale(0.5, 0.5);
		letter->Path.Add(point);
	}
}		

void TwoStringBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, scl, str1, str2, step;
	int sch = 0;
	if(length > 1)
	{
		str1 = (finish[2]-finish[0])*5;
		str2 = (finish[2]-finish[0])*10;
		step = (finish[2]-finish[0])*2;
	}
	else
	{
		str1 = 50;
		str2 = 100;
		step = 20;
	}

	for (int i = 0; i < length; i++)
	{
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;							
		point.Alpha = (BYTE)0;

		for(int temp = -1; temp < length*3; temp++)
		{  
			if((i*3) > temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Alpha =(BYTE)Alpha;
				letter->Path.Add(point);


			}

			else
			{	
				for(int j = 0; j < 10; j++)
				{
					x--;		
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Alpha = (BYTE)Alpha;
					letter->Path.Add(point);
				}

				scl = 2.;	 
				if(i < length/2)
				{
					y = finish[2*i+1] + str1;
					x = finish[0] + step*i;
				}
				else
				{
					y = finish[2*i+1] + str2;
					x = finish[0] + step*(-length/2 + i);
				}
				
				for(int j = 0; j < 50; j++)
				{
					point.Clear();
					point.Translate(x += step/50, y += step/100 + i/50);
					point.Scale(scl, scl);
					point.Alpha -= Alpha/50;
					letter->Path.Add(point);
					scl -= .02;
				}	

				for(int j = 0; j < length*3 - sch; j++)
				{
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Alpha = (BYTE)0;
					letter->Path.Add(point);
				}	 
				break;
			}

		}
	}
}		
void TumblingBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, start, stepX, stepY, scl;
	int sch = 0;
	start = 0;
	if(length > 1)
	{
		stepX = (finish[2] - finish[0]) * 1.7/10;
		stepY = (finish[1] - start)/10;
	}
	else
	{
		stepX = 1.7;
		stepY = (finish[1] - start)/10;
	}

	for (int i = 0; i < length; i++)
	{
		sch = 0;
		scl = 1;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
		x = finish[2*i] + stepX*10;
		y = start;

		for(int temp = 0; temp < length*1.5; temp++)
		{  
			if((i*1.5) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);		
				point.Rotate(0, 0, 0);
				point.Scale(1, 1);
				letter->Path.Add(point);

			}
			else
			{
				for(int j = 0; j <= 10; j++)
				{ 
					if(j > 7)
						scl += 0.333;
					else 
						scl -= 0.25;

					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, -27*j);
					point.Scale(scl, 1);
					letter->Path.Add(point);
					if(j <= 5)
						x -= stepX*2;
					y += stepY;
				}	

				for(int j = 0; j < length*1.5 - sch; j++)
				{
					point.Clear();
					point.Alpha = (BYTE)0;
					point.Translate(x, y);
					point.Rotate(0, 0, -270);
					point.Scale(0, 0);
					letter->Path.Add(point);
				}
				break;
			}

		}   
	}
}		
void LeaveBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	int i, j;
	float x, y, start, step;
	if(length > 1)
		start = (finish[2] - finish[0])*6;
	else
		start = 60;
	step = start/length;
	for(i = (length - 1); i >= 0; i--)
	{ 
		x = finish[2*i];
		y = finish[2*i+1];

		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE; 
		point.Alpha = (BYTE)Alpha; 

		point.Clear();
		point.Translate(x, y);
		letter->Path.Add(point);

		for(j = (length - 1); j >= 0; j--)
		{	
			point.Clear();
			if (i >= j)
			{ 
				point.Alpha = (BYTE)0;
				point.Translate(x, y);
			}
			else		
			{
				point.Alpha = Alpha;
				point.Translate(x, y);
			}
			x += step;
			letter->Path.Add(point);
		}
	}	
}		
void UnfoldBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, start, stepX, stepY, step;
	if(length > 1)	
		step = (finish[2] - finish[0]);
	else
	    step = 10;
	start = finish[2*(length-1)] + step*4;
	stepY = (finish[1] + step*3)/50;
	int sch = 0;

	for(int i = 0; i < length; i++)
	{ 
		sch = 0;
		stepX = (finish[2*i] - start)/50;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*4; temp++)
		{  
			if((i*4) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				letter->Path.Add(point);
			}
			else
			{
				x = finish[2*i];
				y = finish[2*i+1];

				for(float j = 0.; j < 50.; j += 1.)
				{
					point.Clear();
					point.Translate(x, y);
					letter->Path.Add(point);
					x -= stepX;
					y -= stepY;
				}

				for(int j = 0; j < length*4 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(start, -step*3);
					letter->Path.Add(point);
				}
				break;
			}
		}
	}
}		
void CentrifugeBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Rotate(center, 0, 0);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)0;
		point.Translate(center, y);	
		point.Scale(0, 0);
		point.Rotate(center, 0, 720);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}		
void HorizontalRotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 6; j++)
		{
			point.Clear();	
			point.Translate(center, finish[2*i+1]/2);
			if((j == 0) || ( j == 4))
				point.Scale(1, 1);
			else
			{
				if(j%2 == 0)
					point.Scale(-1,1);
				else
					point.Scale(0.1,1);
			}
			point.Translate(-center+x,finish[2*i+1]/2); 
			if(j == 5)
				point.Alpha = (BYTE)0;
			letter->Path.Add(point);
		}
	}
}		
void HorizontalRotation3DBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	float length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (float i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 6; j++)
		{
			point.Clear();	
			point.Translate(center, finish[2*i+1]/2);
			if(j == 0)
				point.Scale(1, 1);
			if(j == 1)
				point.Scale(0.1, 1);
			if(j == 2)
				point.Scale(-1, 1);
			if(j == 3)
				point.Scale(0.1, 1);
			if(j == 4)
				point.Scale(1, 1);
			if(j == 5)
				point.Scale(0.1, 1);

			point.Translate(-center + x, finish[2*i+1]/2); 
			if((j == 1) || (j == 5))
				point.Scale(4.1 - i*4/length,4.1 - i*4/length);
			if((j == 0) || (j == 2) || (j == 4))
				point.Scale(1, 1);
			if(j == 3)
				point.Scale(4.1 - (4 - i*4/length),4.1 - (4 - i*4/length));
			if(j == 5)
				point.Alpha = 0;
			letter->Path.Add(point);
		}
	}
}		



void VerticalRotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 6; j++)
		{
			point.Clear();	
			point.Translate(center, finish[2*i+1]);
			if((j == 0) || (j == 4))
				point.Scale(1, 1);
			else
			{
				if(j%2 == 0)
					point.Scale(1, -1);
				else
					point.Scale(1, 0.1);
			}
			point.Translate(-center + x, 0); 
			if(j == 5)
				point.Alpha = (BYTE)0;
			letter->Path.Add(point);
		}
	}
}		



void CheCheBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;

	for(int i = 0; i < length; i++)
	{ 
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);
			}
			else
			{
				x = finish[2*i];
				y = finish[2*i+1];
				float temp = 1.;
				float temp2 = 1.;
				int koefAlpha = 0; 
				for(int z = 0; z < 5; z++ )
				{

					point.Alpha -= Alpha/5;
					if(point.Alpha < 0)
						point.Alpha = 0;
					for(float j = 1.; j >= 0.5; j -= 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);
						if(point.Alpha != 0)
							koefAlpha = point.Alpha;
						if((temp > 37))
						{		   
							point.Alpha = 0;
						}
						else 
						{
							point.Alpha = koefAlpha;
						}

						if(j > 0.75)
						{
							point.Scale(temp2, temp2);
							point.Rotate(0, 0, temp);
							temp2 -= 0.15;
							temp +=	8;
						}
						else 
						{
							point.Scale(temp2, temp2);
							point.Rotate(0, 0, temp);
							temp2 += 0.16;
							temp -=	8;
						}
						letter->Path.Add(point);
					}
					
					

				}
					   
				
					
				for(int j = 0; j <= length - sch+1; j++)
				{   point.Alpha = 0;
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Rotate(0, 0, 0);
					letter->Path.Add(point);
				}	
				break;
			}
		}
	}
}	
//new2
void Boomerang( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	center = (finish[2*(length-1)] + finish[0])/2;
	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(finish[2*(length-1)], 0);
		point.Rotate(center, 0, -90);
		point.Scale(0.4, 1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate((center + finish[2*(length-1)])/2,y + step*6);	
		point.Rotate(center, 0, -45);
		point.Scale(0.5, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Rotate(center, 0, 0);
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}
void Flash( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(-30, y);
		point.Scale(1.2, 0.1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);
		point.Scale(1.2, 0.1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(center, y);
		point.Scale(1, 1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(center, y);
		point.Scale(1.3, 1.3);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1.15, 1.15);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point); 

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);  
	}
}
void Flight( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(finish[2*(length-1)], 0);
		point.Rotate(0, 0, -90);		
		point.Translate(x, step*2); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/2;
		point.Translate((finish[2*(length-1)] + (center - step))/2, (y + step)/2);
		point.Rotate(0, 0, -45);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha =(BYTE)Alpha;
		point.Translate(center - step*4,y + step*4);	
		point.Rotate(0, 0, 0);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Rotate(0, 0, 0);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

	}
}

void Jump( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step, step2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] - finish[0])/2;
	step2 = (center - finish[0])/7;
	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();			
		point.Translate(-center + x, -step*2); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + step2 + x, y/1.5); 
		point.Scale(1, 0.5);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 2*step2 + x, y/7); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 3*step2 + x, y/1.5); 
		point.Scale(1, 0.5);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 4*step2 + x, y/4); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 5*step2 + x, y/1.5); 
		point.Scale(1, 0.5);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 6*step2 + x, y/2); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(x, y); 
		point.Scale(1, 1);
		letter->Path.Add(point);

	}
}		
void Compress( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center=(finish[2*(length-1)]+finish[0])/2;
	
	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha =(BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();			
		point.Translate(0, y); 
		point.Scale(2, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha =(BYTE)Alpha;
		point.Translate(0, y); 
		point.Scale(1, 1);
		point.Translate(x, 0);
		letter->Path.Add(point);

	}
}		
void Motion( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();
		point.Translate(x/2 - step, y);
		point.Alpha = (BYTE)0 ;
		letter->Path.Add(point);

		for(int j = 0; j < length; j++)
		{ 
			if(j == i)
			{
				point.Clear();
				point.Translate(x/2 + step*j/2, y);		
				point.Alpha = (BYTE)Alpha;
				letter->Path.Add(point);
			} 
			if(j < i)
			{
				point.Clear();
				point.Translate(x/2 + step*j/2, y);
				if((i - j) <= 5)
					point.Alpha = (BYTE)Alpha/2;
				letter->Path.Add(point);
			}
			else
			{
				point.Clear();
				point.Translate(x, y);		
				point.Alpha = (BYTE)Alpha;
				letter->Path.Add(point);
			}
		}
	}
}		

void Manifestation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] + finish[0])/2;
	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();			
		point.Translate(center,y - step*3); 
		point.Scale(0.1, 0.1); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y); 
		point.Scale(1, 1);
		letter->Path.Add(point);
	}
}		


void Shot( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();			
		point.Translate(x + step*6,y - step*3); 
		point.Rotate(0, 0, -90);
		point.Scale(1, 1); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y + step*2); 
		point.Rotate(0, 0, 0);
		point.Scale(0.1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(x, y); 
		point.Rotate(0, 0, 0);
		point.Scale(1, 1);
		letter->Path.Add(point);
	}
}		



void Reduction( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;
	for (int i = 0; i < length; i++)
	{ 
		sch = 0;	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*4; temp++)
		{  
			if((i*4) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 30);
				letter->Path.Add(point);
			}
			else
			{
				for(float j = 0.; j < 30.; j += 1.)
				{
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 30 - j);
					point.Alpha = point.Alpha + Alpha/30;
					letter->Path.Add(point);
				}

				for(int j = 0; j < length*4 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					letter->Path.Add(point);
				}
				break;
			}
		}
	}
}	



void VHRotation( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;
		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 2; j++)
		{

			if(i%2 == 0)
			{
				if(j == 0)
					point.Alpha = (BYTE)0;
				else
					point.Alpha = (BYTE)Alpha/2;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(1, -1); 
				letter->Path.Add(point);

				if(j == 0)
					point.Alpha = (BYTE)Alpha/4;
				else
					point.Alpha = (BYTE)Alpha;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(1, 1); 
				letter->Path.Add(point);
			}
			if(i%2 == 1)
			{
				if(j == 0)
					point.Alpha = (BYTE)0;
				else
					point.Alpha = (BYTE)Alpha/2;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(-1, 1); 
				letter->Path.Add(point);

				if(j == 0)
					point.Alpha =(BYTE)Alpha/4;
				else
					point.Alpha = (BYTE)Alpha;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(1, 1); 
				letter->Path.Add(point);
			}
		}
	}
}		

void Rotate3D( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, center;
	center = (finish[2*(length-1)] + finish[0])/2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y);	
		point.Scale(0.6, -0.6);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)Alpha/5;
		point.Translate(center, y + step*4);	
		point.Scale(0.8, 0.01);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)Alpha/4;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/3;
		point.Translate(center, y - step*4);	
		point.Scale(0.8, 0.01);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/2;
		point.Translate(center, y);	
		point.Scale(0.6, -0.6);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y + step*4);	
		point.Scale(0.8, 0.01);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}		

void Spiral( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, R = 30, centerX, centerY, center, scl = 0.5, stepScl;
	int sch = 0;
	center = (finish[2*(length-1)] + finish[0])/2;
	if(length > 1)	
		R = (finish[2] - finish[0])*3;
	else
		R = 30;
	stepScl = (1 - scl)/max(0.01, R*4);
	for (int i = 0; i < length; i++)
	{
		sch = 0;	
		scl = 0.5;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
		x = finish[2*i];
		y = finish[2*i+1];
		centerX = x;
		centerY = y;

		for(int j = 0; j < R; j++)
		{	
			y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
			y -= finish[2*i+1];
			y *= 2;
			y += finish[2*i+1];			
			point.Clear();
			point.Translate(center, y);	
			point.Scale(scl, scl);
			point.Translate(-center + x, 0);
			letter->Path.Add(point);
			scl += stepScl;
			x++;
		}  
		
		centerY = finish[2*i+1] + R;
		for(int j = 0; j < R*2; j++)
		{	
			y = centerY + sqrt(R*R - (x - centerX)*(x - centerX));
			y -= finish[2*i+1];
			y /= 1;
			y += finish[2*i+1];		
			point.Clear();
			point.Translate(center, y);	
			point.Scale(scl, scl);
			point.Translate(-center + x, 0);
			letter->Path.Add(point);
			scl += stepScl;
			x--;
		}

		for(int j = 0; j < R + 1; j++)
		{	
			y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
			y -= finish[2*i+1];
			y /= 2;
			y += finish[2*i+1];
			point.Clear();
			point.Translate(center, y);	
			point.Scale(scl, scl);
			point.Translate(-center + x, 0);
			letter->Path.Add(point);
			scl += stepScl;
			x++;
		}
	}
}	



void Flag( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;

	for(int i = 0; i < length; i++)
	{ 
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*2; temp++)
		{  
			if((i*2) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				x = finish[2*i];
				y = finish[2*i+1];
				float temp = 1.;
				float temp2 = 3.;
				for(int z = 0; z < 8; z++ )
				{	
					point.Alpha += Alpha/8;
					if(point.Alpha > 255)
						point.Alpha = 255;
					for(float j = 1.; j >= 0.5; j -= 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);
						if(j > 0.75)
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}
					
					for(float j = 0.5; j <= 1.; j += 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);			
						if(j < 0.75)
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}
					temp2 -= 0.3;
					
				}

				for(int z = 0; z < 5; z++ )
				{
					for(float j = 1.; j >= 0.5; j -= 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);
						if(j > 0.75)
						{
							point.Scale(j/temp, j);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp, j);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}
					
					for(float j = 0.5; j <= 1.; j += 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);			
						if(j < 0.75)
						{
							point.Scale(j/temp, j);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp, j);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}

				}
					   
				
					
				for(int j = 0; j < length*2 - sch; j++)
				{   point.Alpha = Alpha;
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					letter->Path.Add(point);
				}	
				break;
			}
		}
	}
}	
void Perpendicular( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	int flag = 0;
	int stop = 0;
	for (int i = 0; i < length; i++)
	{  
		flag = 0;
		stop = 0;						   
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;
			   
		x = finish[0];
		y = finish[2*i+1] + (length - i)*step;
		point.Clear();
		point.Alpha = (BYTE)0;
		point.Translate(x, y);
		point.Rotate(0, 0, -90);
		letter->Path.Add(point);

		
		for (int j = 0; j < length; j++)
		{	
			

			if(flag == 0)	 
			{
				x = finish[0];
				y -= step;
				point.Clear();
				point.Translate(x, y);
				point.Rotate(0, 0, -90);
				if((point.Alpha > Alpha) || ((point.Alpha + Alpha/length) > Alpha))
					point.Alpha = Alpha;
				else
					point.Alpha += Alpha/length;
				letter->Path.Add(point);
			}
			if ((y == finish[2*i+1]) && (flag == 0))
			{
				x = finish[0];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Rotate(0, 0, 0);	
				if((point.Alpha > Alpha) || ((point.Alpha + Alpha/length) > Alpha))
					point.Alpha = Alpha;
				else
					point.Alpha += Alpha/length;
				letter->Path.Add(point);
				flag = 1;
				if(i == 0)
				{
					x = finish[0];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, 0);		
					if((point.Alpha > Alpha) || ((point.Alpha + Alpha/length) > Alpha))
						point.Alpha = Alpha;
					else
						point.Alpha += Alpha/length;
					letter->Path.Add(point);
					break;
				}
			}
			if (flag == 1)
			{	
				for (int j = 0; j < length; j++)
				{
					if(stop == 0)
					{
						x += step;
						y = finish[2*i+1];
						point.Clear();
						point.Translate(x, y);
						point.Rotate(0, 0, 0);					
						if((point.Alpha > Alpha) || ((point.Alpha + Alpha/length) > Alpha))
							point.Alpha = Alpha;
						else
							point.Alpha += Alpha/length;
						letter->Path.Add(point);
					}
					if((x == finish[2*i]) && (stop == 0))
					{
						x = finish[2*i];
						y = finish[2*i+1];
						point.Clear();
						point.Translate(x, y);
						point.Rotate(0, 0, 0);					
						if((point.Alpha > Alpha) || ((point.Alpha + Alpha/length) > Alpha))
							point.Alpha = Alpha;
						else
							point.Alpha += Alpha/length;	
						letter->Path.Add(point);
						stop = 1;
					}
				}
			}	
		}		
	}
}		
void Clock( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;

	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)
	center =(int)( 0.6*(float)length);

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];
		if(i < center)
		{
			point.Clear();	
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, -900);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = (BYTE)Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
		}
		if(i > center)
		{
			point.Clear();	
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, -225);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = (BYTE)Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
		}

		if(i == center)
		{

			for(int j = 0; j<5; j++)
			{					
				point.Clear();	 
				point.Translate(x, y);	
				point.Scale(1.5, 1.5);
				point.Rotate(0, 0, 0);
				point.Translate(0, 0); 
				letter->Path.Add(point);

				point.Alpha += Alpha/5;
			}

			point.Alpha = Alpha;
			point.Clear();	
			point.Translate(x, y);	
			point.Scale(1, 1);
			point.Rotate(0, 0, 0);
			point.Translate(0, 0); 
			letter->Path.Add(point);
		}

	}
}		

//Erase new2
void BoomerangBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Rotate(center, 0, 0);
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate((center + finish[2*(length-1)])/2,y + step*7);	
		point.Rotate(center, 0, -45);
		point.Scale(0.5, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)0;
		point.Translate(finish[2*(length-1)], 0);
		point.Rotate(center, 0, -90);
		point.Scale(0.4, 1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}
void FlashBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point); 

		point.Clear();	
		point.Translate(center, y);	
		point.Scale(1.15, 1.15);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point); 

		point.Clear();	
		point.Translate(center, y);
		point.Scale(1.3, 1.3);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(center, y);
		point.Scale(1, 1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(center, y);
		point.Scale(1.2, 0.1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(-30, y);
		point.Scale(1.2, 0.1);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}

void FlightBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] + finish[0])/2;
	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(center, y);	
		point.Rotate(center, 0, 0);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(center - step*4, y + step*4);	
		point.Rotate(center, 0, 0);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/2;
		point.Translate((finish[2*(length-1)] + (center - step))/2, (y + step)/2);
		point.Rotate(center, 0, -45);		
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(finish[2*(length-1)], 0);
		point.Rotate(center, 0, -90);		
		point.Translate(x, 0); 
		letter->Path.Add(point);
	}
}

void JumpBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step, step2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] - finish[0])/2;
	step2 = (center - finish[0])/7;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(x, y); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 6*step2 + x, y/2); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 5*step2 + x, y/1.5); 
		point.Scale(1, 0.5);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 4*step2 + x, y/4); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + 3*step2 + x, y/1.5); 
		point.Scale(1, 0.5);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center+ 2*step2 + x, y/7); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(-center + step2 + x, y/1.5); 
		point.Scale(1, 0.5);
		letter->Path.Add(point);

		point.Clear();			
		point.Translate(-center + x, -step*2); 
		point.Scale(1, 1);
		letter->Path.Add(point);
	}
}		
void CompressBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length - 1)] + finish[0])/2;
	
	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Alpha =(BYTE)Alpha;
		point.Translate(0, y); 
		point.Scale(1, 1);
		point.Translate(x, 0);
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)0;
		point.Translate(0, y); 
		point.Scale(2, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}		
void MotionBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();
		point.Translate(x, y);
		point.Alpha = (BYTE)Alpha ;
		letter->Path.Add(point);

		for(int j = 0; j < (length + 10); j++)
		{					
			if(j > i)
			{
				point.Clear();
				point.Translate(x/2 + step*j/2, y);
				if((j-i) > 5)
					if(point.Alpha != 0)
						point.Alpha -= Alpha/3;
				letter->Path.Add(point);
			}
			else
			{
				point.Clear();
				point.Translate(x, y);		
				point.Alpha = (BYTE)Alpha;
				letter->Path.Add(point);
			}
		}
	}
}		
void ManifestationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	center = (finish[2*(length-1)] + finish[0])/2;
	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y); 
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y - step*3); 
		point.Scale(0.1, 0.1); 
		letter->Path.Add(point);
	}
}		
void ShotBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(x, y); 
		point.Rotate(0, 0, 0);
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(x, y + step*2); 
		point.Rotate(0, 0, 0);
		point.Scale(0.1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(x + step*6, y - step*3); 
		point.Rotate(0, 0, -90);
		point.Scale(1, 1); 
		letter->Path.Add(point);
	}
}		

void ReductionBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;
	for(int i = 0; i < length; i++)
	{ 
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*4; temp++)
		{  
			if((i*4) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				for(float j = 0.; j < 30.; j += 1.)
				{
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, j + 1);
					point.Alpha = point.Alpha - Alpha/30;
					letter->Path.Add(point);
				}

				for(int j = 0; j < length*4 - sch; j++)
				{
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 30);
					point.Alpha = (BYTE)0;
					letter->Path.Add(point);
				}
				break;
			}
		}
	}
}	

void VHRotationBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;
		x = finish[2*i];
		y = finish[2*i+1];

		for(int j = 0; j < 2; j++)
		{

			if(i%2 == 0)
			{
				if(j == 0)
					point.Alpha = (BYTE)Alpha;
				else
					point.Alpha = (BYTE)Alpha/4;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(1, 1); 
				letter->Path.Add(point);

				if(j == 0)
					point.Alpha = (BYTE)Alpha/2;
				else
					point.Alpha = (BYTE)0;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(1, -1); 
				letter->Path.Add(point);

			}
			if(i%2 == 1)
			{
				if(j == 0)
					point.Alpha = (BYTE)Alpha;
				else
					point.Alpha = (BYTE)Alpha/4;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(1, 1); 
				letter->Path.Add(point);

				if(j == 0)
					point.Alpha = (BYTE)Alpha/2;
				else
					point.Alpha = (BYTE)0;
				point.Clear();			
				point.Translate(x, y); 
				point.Scale(-1, 1); 
				letter->Path.Add(point);
			}
		}
	}
}		
void Rotate3DBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, center;
	center = (finish[2*(length - 1)] + finish[0])/2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;
	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)Alpha/2;
		point.Translate(center, y - step*4);	
		point.Scale(0.8, 0.01);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();
		point.Alpha = (BYTE)Alpha/3;
		point.Translate(center, y);	
		point.Scale(0.6, -0.6);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/4;
		point.Translate(center, y + step*4);	
		point.Scale(0.8, 0.01);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/5;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha/6;
		point.Translate(center, y - step*4);	
		point.Scale(0.8, 0.01);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y);	
		point.Scale(0.6, -0.6);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

	}
}		
void SpiralBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, R = 30, centerX, centerY, center, scl = 1, stepScl;
	int sch = 0;
	center = (finish[2*(length-1)] + finish[0])/2;
	if(length > 1)	
		R = (finish[2] - finish[0])*3;
	else
		R = 30;

	if (R < 2)
		R = 2;

	stepScl = 0.5 / (R*4);
	for (int i = 0; i < length; i++)
	{
		sch = 0;	
		scl = 1;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
		x = finish[2*i];
		y = finish[2*i+1];
		centerX = x;
		centerY = y;
		x++;
		centerY = finish[2*i+1] + R;
		for(int j = R; j >= 0; j--)
		{	
			y = centerY - sqrt(max(0, R*R - (x - centerX)*(x - centerX)));
			y -= finish[2*i+1];
			y /= 2;
			y += finish[2*i+1];
			point.Clear();
			point.Translate(center, y);	
			point.Scale(scl, scl);
			point.Translate(-center + x, 0);
			letter->Path.Add(point);
			scl -= stepScl;
			x--;
		}

		for(int j=R*2-1;j>=0;j--)
		{	
			y = centerY + sqrt(R*R - (x - centerX)*(x - centerX));
			y -= finish[2*i+1];
			y /= 1;
			y += finish[2*i+1];		
			point.Clear();
			point.Translate(center, y);	
			point.Scale(scl, scl);
			point.Translate(-center + x, 0);
			letter->Path.Add(point);
			scl -= stepScl;
			x++;
		}		 
		centerX = finish[2*i];
		centerY = finish[2*i+1];
		for(int j = R - 1; j >= 0; j--)
		{	
			y = centerY - sqrt(R*R - (x - centerX)*(x - centerX));
			y -= finish[2*i+1];
			y *= 2;
			y += finish[2*i+1];			
			point.Clear();
			point.Translate(center, y);	
			point.Scale(scl, scl);
			point.Translate(-center + x, 0);
			letter->Path.Add(point);
			scl -= stepScl;
			x--;
		}  
	}
}	
void FlagBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y;
	int sch = 0;

	for(int i = 0; i < length; i++)
	{ 
		sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		for(int temp = 0; temp < length*2; temp++)
		{  
			if((i*2) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				x = finish[2*i];
				y = finish[2*i+1];
				float temp = 1.;
				float temp2 = 1.; 
				for(int z = 0; z < 5; z++ )
				{
					for(float j = 1.; j >= 0.5; j -= 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);
						if(j > 0.75)
						{
							point.Scale(j/temp, j);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp, j);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}
					
					for(float j = 0.5; j <= 1.; j += 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);			
						if(j < 0.75)
						{
							point.Scale(j/temp, j);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp, j);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}

				}
					   
				for(int z = 0; z < 8; z++ )
				{	
					point.Alpha -= Alpha/8;
					if(point.Alpha < 0)
						point.Alpha = 0;
					for(float j = 1.; j >= 0.5; j -= 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);
						if(j > 0.75)
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}
					
					for(float j = 0.5; j <= 1.; j += 0.05)
					{
						point.Clear(); 
						point.Translate(x, y);			
						if(j < 0.75)
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp +=	0.2;
						}
						else 
						{
							point.Scale(j/temp*temp2, j*temp2);
							temp -=	0.2;
						}
						letter->Path.Add(point);
					}
					temp2 += 0.3;
					
				}
					
				for(int j = 0; j < length*2 - sch; j++)
				{   point.Alpha = 0;
					x = finish[2*i];
					y = finish[2*i+1];
					point.Clear();
					point.Translate(x, y);
					point.Scale(1, 1);
					letter->Path.Add(point);
				}	
				break;
			}
		}
	}
}	
void PerpendicularBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step;
	int flag = 0;
	int stop = 0;

	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{  
		flag = 0;
		stop = 0;						   
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;
			   
		x = finish[2*i];
		y = finish[2*i+1];
		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);
		point.Rotate(0, 0, 0);
		letter->Path.Add(point);

		
		for (int j = 0; j < length; j++)
		{	
			
			if (flag == 0)	 
			{
				x += step;
				y = finish[2*i+1];
				point.Clear();
				point.Translate(x, y);
				point.Rotate(0, 0, 0);
				if((point.Alpha < 0) || (point.Alpha < Alpha/length))
					point.Alpha = 0;
				else
					point.Alpha -= Alpha/length;
				letter->Path.Add(point);
			}
			if ((x == finish[2*(length - 1)] + step) && (flag == 0))
			{
				point.Clear();
				point.Translate(x, y);
				point.Rotate(0, 0, 90);	
				if((point.Alpha < 0) || (point.Alpha < Alpha/length))
					point.Alpha = 0;
				else
					point.Alpha -= Alpha/length;
				letter->Path.Add(point);
				flag = 1;
				if(i == 0)
				{	
					y+=step;
					point.Clear();
					point.Translate(x, y);
					point.Rotate(0, 0, 90);		
					if((point.Alpha < 0) || (point.Alpha < Alpha/length))
						point.Alpha = 0;
					else
						point.Alpha -= Alpha/length;
					letter->Path.Add(point);  
					break;
				}		  
			}
			if (flag == 1)
			{	
				for (int j = 0; j < length; j++)
				{
					if(stop == 0)
					{
						y += step;
						point.Clear();
						point.Translate(x, y);
						point.Rotate(0, 0, 90);					
						if((point.Alpha < 0) || (point.Alpha < Alpha/length))
							point.Alpha = 0;
						else
							point.Alpha -= Alpha/length;
						letter->Path.Add(point);
					}
					if((y == (i*step + finish[2*i+1])) && (stop == 0))
					{
						y += step;
						point.Clear();
						point.Translate(x, y);
						point.Rotate(0, 0, 90);					
						if((point.Alpha < 0) || (point.Alpha < Alpha/length))
							point.Alpha = 0;
						else
							point.Alpha -= Alpha/length;	
						letter->Path.Add(point);
						stop = 1;
					}	 
				}
			}	
		}
		y += step;
		point.Clear();
		point.Alpha = 0;
		point.Translate(x, y);
		point.Rotate(0, 0, 90);
		letter->Path.Add(point); 
	}
}		
void ClockBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;

	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)
	center =(int)( 0.6*(float)length);

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		if(i < center)
		{
			point.Clear();				
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
			
			point.Clear();
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 900);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
		}
		if(i > center)
		{
			point.Clear();				
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 225);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
		}

		if(i == center)
		{

			point.Alpha = Alpha;
			point.Clear();	
			point.Translate(x, y);	
			point.Scale(1, 1);
			point.Rotate(0, 0, 0);
			point.Translate(0, 0); 
			letter->Path.Add(point);

			for(int j = 0; j<5; j++)
			{	
				if(j == 4)
					point.Alpha = 0;

				point.Clear();	 
				point.Translate(x, y);	
				point.Scale(1.5, 1.5);
				point.Rotate(0, 0, 0);
				point.Translate(0, 0); 
				letter->Path.Add(point);

				point.Alpha -= Alpha/5;
				
			}
		}

	}
}		




//new3
void Galaxy( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )					 
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, multip = 0.1;
	int koef;
	
	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)
	center =(int)( 0.5*(float)length);

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		
		if(i <= center)
		{	koef = (int)(0.7 * center);
			if(i < koef)
				multip += ((float)length)/1000.;
			else
				multip -= ((float)length)/1000.;
			point.Clear();
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.1, 0.01);
			point.Rotate(finish[center*2], 0, multip * 1000.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
			

		}
		if(i > center)
		{	
			koef = (int)(1.4*center);
			if(i < koef)
				multip += ((float)length)/1000.;
			else
				multip -= ((float)length)/1000.;

			point.Clear();	
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.1, 0.1);
			point.Rotate(finish[center*2], 0, multip * 1000.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
		  	point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);


		}

	}
}		
void WaveRotate3D( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step, center;
	center = (finish[2*(length-1)] - finish[0])/2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(center, y + step);	
				point.Scale(0.6, -0.6);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();	
				point.Alpha = (BYTE)Alpha/4;
				point.Translate(center, y);	
				point.Scale(0.6, -0.6);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(center, y - step*2);	
				point.Scale(0.65, -0.3);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/2;
				point.Translate(center, y - step*4);	
				point.Scale(0.8, 0.01);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/1.5;
				point.Translate(center, y - step*2);	
				point.Scale(0.95, 0.6);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(center, y);	
				point.Scale(1, 1);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				for(int j = 0; j < length - sch; j++)
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(1, 1);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
				break;
			}

		}



	}
}		
void RandomJump( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, jumpY, sclX, sclY;
	int flag = 1;
	int sch = 0;
	int stop = 1;
	int step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	if (step < 1)
		step = 1;

	for (int i = 0; i < length; i++)
	{ 
		sch = 0;
		stop = 1;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];
	   		
		for(int j = 0; j < 7; j++)
		{
			flag = rand() % 2;
			if(flag == 0)
				flag = -1;
			else
				flag = 1;

			
			if(stop)
			{
				stop = rand() % 3;			
				jumpY = rand() % (step * 4);
				sclX = rand() % 3;
				if(sclX == 0)
					sclX = 0.1;
				sclY = rand() % 3;
				if(sclY == 0)
					sclY = 0.1;
				point.Clear();
				point.Translate(x, jumpY);
				point.Scale(sclX, sclY);
				point.Rotate(0, 0, flag * (rand() % 30));
				letter->Path.Add(point);
				
				point.Clear();	
				point.Alpha = Alpha;
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);	

			}
				
		}

	}
}


void Galaxy2( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )					 
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, multip = 0.1;
	int koef;
	
	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)
	center =(int)( 0.5*(float)length);

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		
		if(i <= center)
		{	koef = (int)(0.7 * center);
			if(i < koef)
				multip += ((float)length)/100.;
			else
				multip -= ((float)length)/100.;
			point.Clear();
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.01, 0.01);
			point.Rotate(finish[center*2], 0, multip * 500.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);
			

		}
		if(i > center)
		{	
			if(length < 18)
				koef = (int)(1.6*center);
			else
				koef = (int)(1.4*center);
			if(i < koef)
				multip += ((float)length)/100.;
			else
				multip -= ((float)length)/100.;

			point.Clear();	
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.01, 0.01);
			point.Rotate(finish[center*2], 0, multip * 500.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
		  	point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);


		}

	}
}		




void Roll( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, start;	
	start = finish[2*(length - 1)];

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(x - start - 20, y);
		point.Rotate(0, 0, -50 * length); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x, y);	
		point.Rotate(0, 0, 0); 
		letter->Path.Add(point);

	}
}
void Conga( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step, center;
	center = (finish[2*(length-1)] - finish[0])/2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x, y - step*6);	
				point.Scale(1.2, 1.2);				
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);	
				point.Scale(1.2, 1.2);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x + step, y + step);	
				point.Scale(0.8, 0.8);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x + step*1.5, y + step/2);	
				point.Scale(0.6, 0.6);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x + step*1.5, y - step/2);	
				point.Scale(0.6, 0.6);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x - step, y);	
				point.Scale(0.4, 0.4);				
				letter->Path.Add(point);

				for(int j = 0; j <= (length - sch); j++)
				{
				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x, y);	
				point.Scale(1, 1);				
				letter->Path.Add(point);
				}
				break;
			}

		}

	}
}		
void _Matrix( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, sclX, sclY;
	int flag = 1;
	int sch = 0;
	int stop = 1;
	int step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		sch = 0;
		stop = 1;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];
	   		
		for(int j = 0; j < 7; j++)
		{
			flag = rand() % 2;
			if(flag == 0)
				flag = -1;
			else
				flag = 1;

			
			if(stop)
			{
				stop = rand() % 3;							
				sclX = rand() % 3;
				if(sclX == 0)
					sclX = 0.1;
				sclY = rand() % 3;
				if(sclY == 0)
					sclY = 0.1;
			
				point.Clear();
				point.Alpha = 0;
				point.Translate(x, y - step*12);
				point.Scale(sclX, sclY);
				letter->Path.Add(point);

				point.Clear();
				point.Alpha = Alpha;
				point.Translate(x, y);
				point.Scale(sclX, sclY);
				letter->Path.Add(point);
				
				point.Clear();	
				point.Alpha = 0;
				point.Translate(x, y + step*12);
				point.Scale(sclX, sclY);
				letter->Path.Add(point);	

			}
				
		}
		point.Clear();
		point.Alpha = 0;
		point.Translate(x, y - step*12);
		point.Scale(sclX, sclY);
		letter->Path.Add(point); 
	  	point.Clear();	
		point.Alpha = Alpha;
		point.Translate(x, y);
		point.Scale(1, 1);
		letter->Path.Add(point);	

	}
}

void HotPinkSpin( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];				
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x, y);
				point.Rotate(0, 0, 90);
				point.Shear(-2, 0);
				point.Scale(-3, 5);								
				letter->Path.Add(point); 
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];
				
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x, y);
				point.Rotate(0, 0, 90);
				point.Shear(-2, 0);
				point.Scale(-3, 5);								
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);
				point.Rotate(0, 0, 45);
				point.Shear(-3, 0);
				point.Scale(-1, 5);								
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Shear(-4, 0);
				point.Scale(1, 5);								
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Shear(-2.25, 0);
				point.Scale(1, 4);								
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);			   
				point.Shear(-0.5, 0);
				point.Scale(1, 3);								
				letter->Path.Add(point);
								 
				for(int j = 0; j <= length - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(x, y);					
					point.Rotate(0, 0, 0);
					point.Shear(0, 0);
					point.Scale(1, 1);										
					letter->Path.Add(point);				
				}
				break;
			}

		}

	}
}		

void HopInJumpOut( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step, scl = 3., transX, transY, rot;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	if (step < 0.01)
		step = 0.01;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		scl = 3.;
		rot = 0;
		transY  = 12.;
		transX  = 3.;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length*4; temp++)
		{  
			if((i*4) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];				
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x - step*transX, y - step*transY);
				point.Rotate(0, 0, 0);			
				point.Scale(scl +1, scl);								
				letter->Path.Add(point); 
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];
				for(int z = 0; z < 100; z++)
				{
					point.Clear();	
					if(point.Alpha < Alpha/2)
						point.Alpha += Alpha/200;

					point.Translate(x - step*transX, y - step*transY);
					point.Rotate(0, 0, 0);				
					point.Scale(scl + 1, scl);								
					letter->Path.Add(point);

					scl += 0.01;
					transX += 0.04;
					transY -= 0.09;				  
				}
								 
				for(int z = 0; z < 50; z++)
				{
					point.Clear();	
					if(point.Alpha < (point.Alpha + Alpha/100))
						point.Alpha += Alpha/100;
					else
						point.Alpha = Alpha;
					point.Translate(x - step*transX, y - step*transY);
					point.Rotate(0, 0, rot);				
					point.Scale(scl + 1, scl);								
					letter->Path.Add(point);
					scl -= 0.04;
					rot += 2;
					transX -= 0.1;
					transY -= 0.6/step;				  
				}
				 
				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x - step*2, y);
				point.Rotate(0, 0, 0);				
				point.Scale(1, 1);								
				letter->Path.Add(point);
				
				transX = 2;   
				for(int z = 0; z < 50; z++)
				{
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(x - step*transX, y - step * z * 0.02);
					if(z < 25)
						point.Rotate(0, 0, -z*0.25);				
					else
						point.Rotate(0, 0, z*0.25);				
					point.Scale(1, 1);								
					letter->Path.Add(point);

					scl += 0.02;
					
					transX -= 0.02;
					
				}			  
				
				for(int z = 0; z < 50; z++)
				{
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(x - step*transX, y - step * (50-z) * 0.02);
					if(z < 25)
						point.Rotate(0, 0, z*0.25);				
					else
						point.Rotate(0, 0, -z*0.25);						
					point.Scale(1, 1);								
					letter->Path.Add(point);

					scl += 0.02;
					
					transX -= 0.02;
					
				}	

				for(int j = 0; j <= length*4 - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(x, y);					
					point.Rotate(0, 0, 0);
					point.Scale(1, 1);										
					letter->Path.Add(point);				
				}
				break;
			}

		}

	}
}		
void ConJoin( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x, y);	
				point.Scale(3, 3);
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();	
				point.Alpha = (BYTE)Alpha/4;
				point.Translate(x, y + step*4);
				point.Scale(3, 3); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(x, y);	
				point.Scale(0.65, -0.3);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/2;
				point.Translate(x, y);	
				point.Scale(0.8, 0.01);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/1.5;
				point.Translate(x, y);	
				point.Scale(0.95, 0.6);
				letter->Path.Add(point);

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);

				for(int j = 0; j < length - sch; j++)
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(x, y);
					point.Scale(1, 1);
					letter->Path.Add(point);
				}
				break;
			}

		}



	}
}		
void HopsCotch( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x - step*3, y);	
				point.Scale(3, 3);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();	
				point.Alpha = (BYTE)Alpha/4;
				point.Translate(x + step*10, y);
				point.Scale(3, 3); 
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(x, y);	
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(x - step, y);	
				point.Scale(1, 1);
				point.Rotate(0, 0, -50);
				letter->Path.Add(point);
		
				for(int j = 0; j <= length - sch; j++)
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(x, y);
					point.Scale(1, 1);
					point.Rotate(0, 0, 0);
					letter->Path.Add(point);
				}
				break;
			}

		}

	}
}		
void Twist( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )					 
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, center, multip = 0;
	int koef, centr_ind;
	
	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)

	center = (finish[2*(length - 1)] + finish[0])/2;
	centr_ind =	(int)( 0.7*(float)length);
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];
		
		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y);	
		point.Scale(0.6, -0.6);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		if(i <= centr_ind)
		{
			multip++;
	
			x = finish[2*i];
			y = finish[2*i+1];
			for(int j = 0; j < multip; j++)
			{	
				if(j%2 == 0)
				{  
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(0.6, -0.6);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y + step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
				else
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(1, 1);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y - step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
			}

		}
		if(i > centr_ind)
		{	
				
			x = finish[2*i];
			y = finish[2*i+1];
			for(int j = 0; j < multip; j++)
			{
				if(j%2 == 0)
			
				{  
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(0.6, -0.6);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y + step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
				else
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(1, 1);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y - step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}

			}

		}	  
	  	point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}		


void StarWars( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )					 
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, center, multip = 0, shr = -1;
	int koef, centr_ind;
	int trans = 12;
	float scl = 3;

	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)

	center = (finish[2*(length - 1)] + finish[0])/2;
	centr_ind =	(int)( 0.5*(float)length);
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	
		scl	= 3;
		trans = 12;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];		
		for(int j = 0; j < 18; j++)
		{
			point.Clear();	
			point.Translate(center, y + step*trans);	
			point.Shear(shr, 0);
			point.Scale(scl, scl/1.5);
			point.Translate(-center + x, 0); 
			letter->Path.Add(point);

			point.Alpha = (BYTE)Alpha;
			scl -= 0.15;
			trans--;
		}

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y + step*trans);	
		point.Shear(shr, 0);
		point.Scale(scl, scl/1.5);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		shr += (float)(2./length); 
	}
}		



void Squeeze( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, koef;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		koef = 8;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];
		
		point.Clear();	
		point.Translate(x - step*koef, y);	
		point.Scale(1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);
		point.Scale(0.75, 1); 
		point.Shear(0, -0.25);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);
		point.Scale(0.5, 1); 
		point.Shear(0, -0.5);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(0.75, 1); 
		point.Shear(0, -0.25);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;

		
		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);
		point.Scale(0.01, 1); 
		point.Shear(0, 1);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(-1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	 
		point.Scale(0.01, 1); 
		point.Shear(0, -1);
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;
		

	}
}

void Terminator( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(0.1, 0.1); 
		point.Translate(0, 0);
		letter->Path.Add(point);
		
		point.Alpha = (BYTE)Alpha;

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(0.1, 0.1); 
		point.Translate(0, 0);
		letter->Path.Add(point);

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(1, 0.1); 
		point.Translate(-center + x, 0);
		letter->Path.Add(point);
		
		point.Clear();			
		point.Translate(center, y); 
		point.Scale(1, 0.1); 
		point.Translate(-center + x, 0);
		letter->Path.Add(point);

		point.Clear();	
		point.Translate(x, y); 
		point.Scale(1, 1);
		point.Translate(0, 0);
		letter->Path.Add(point);
	}
}		
	  

//Erase new3
void GalaxyBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, multip = 0.1;
	int koef;
	
	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)
	center =(int)( 0.5*(float)length);

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		
		if(i <= center)
		{	koef = (int)(0.7 * center);
			if(i < koef)
				multip += ((float)length)/1000.;
			else
				multip -= ((float)length)/1000.;

			point.Clear();	
			point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.1, 0.01);
			point.Rotate(finish[center*2], 0, multip * 1000.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);			

		}
		if(i > center)
		{	
			koef = (int)(1.4*center);
			if(i < koef)
				multip += ((float)length)/1000.;
			else
				multip -= ((float)length)/1000.;

			point.Clear();	
		  	point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.1, 0.1);
			point.Rotate(finish[center*2], 0, multip * 1000.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

		}

	}
}		
void WaveRotate3DBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step, center;
	center = (finish[2*(length-1)] - finish[0])/2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(center, y);	
				point.Scale(1, 1);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(center, y);	
				point.Scale(1, 1);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/1.5;
				point.Translate(center, y - step*2);	
				point.Scale(0.95, 0.6);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/2;
				point.Translate(center, y - step*4);	
				point.Scale(0.8, 0.01);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(center, y - step*2);	
				point.Scale(0.65, -0.3);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/4;
				point.Translate(center, y);	
				point.Scale(0.6, -0.6);
				point.Translate(-center + x, 0); 
				letter->Path.Add(point);	

				for(int j = 0; j <= length - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)0;
					point.Translate(center, y + step);	
					point.Scale(0.6, -0.6);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

				}
				break;
			}

		}



	}
}		
void RandomJumpBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, jumpY, sclX, sclY;
	int flag = 1;
	int sch = 0;
	int stop = 1;
	int step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	if (step < 1)
		step = 1;

	for (int i = 0; i < length; i++)
	{ 
		sch = 0;
		stop = 1;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
	   		
		for(int j = 0; j < 7; j++)
		{
			flag = rand() % 2;
			if(flag == 0)
				flag = -1;
			else
				flag = 1;

			
			if(stop)
			{
				stop = rand() % 3;
				point.Clear();				
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);				
				
				jumpY = rand() % (step * 4);
				sclX = rand() % 3;
				if(sclX == 0)
					sclX = 0.1;
				sclY = rand() % 3;
				if(sclY == 0)
					sclY = 0.1;
				point.Clear();
				if((stop == 0) || (j == 6))
					point.Alpha = 0;
				point.Translate(x, jumpY);
				point.Scale(sclX, sclY);
				point.Rotate(0, 0, flag * (rand() % 30));
				letter->Path.Add(point);
				
			}
				
		}

	}
}
void Galaxy2Back( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center, multip = 0.1;
	int koef;
	
	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)
	center =(int)( 0.5*(float)length);

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		
		if(i <= center)
		{	koef = (int)(0.7 * center);
			if(i < koef)
				multip += ((float)length)/100.;
			else
				multip -= ((float)length)/100.;

			point.Clear();	
			point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.01, 0.01);
			point.Rotate(finish[center*2], 0, multip * 500.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);			

		}
		if(i > center)
		{	
			if(length < 18)
				koef = (int)(1.6*center);
			else
				koef = (int)(1.4*center);

			if(i < koef)
				multip += ((float)length)/100.;
			else
				multip -= ((float)length)/100.;

			point.Clear();	
		  	point.Alpha = Alpha;
			point.Translate(finish[center*2], y);	
			point.Scale(1, 1);
			point.Rotate(finish[center*2], 0, 0);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

			point.Clear();	
			point.Alpha = 0;
			point.Translate(finish[center*2], y);	
			point.Scale(0.01, 0.01);
			point.Rotate(finish[center*2], 0, multip * 500.);
			point.Translate(-finish[center*2] + x, 0); 
			letter->Path.Add(point);

		}

	}
}		


void RollBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, start;	
	start = finish[2*(length - 1)];

	for (int i = 0; i < length; i++)
	{ 
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;	

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(x, y);	
		point.Rotate(0, 0, 0); 
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(x + start + 20, y);
		point.Rotate(0, 0, 50 * length); 
		letter->Path.Add(point);


	}
}
void CongaBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step, center;
	center = (finish[2*(length-1)] - finish[0])/2;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;
				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x, y);	
				point.Scale(1, 1);				
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x - step, y);	
				point.Scale(0.4, 0.4);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x + step*1.5, y - step/2);	
				point.Scale(0.6, 0.6);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;				
				point.Translate(x + step*1.5, y + step/2);	
				point.Scale(0.6, 0.6);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x + step, y + step);	
				point.Scale(0.8, 0.8);				
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);	
				point.Scale(1.2, 1.2);				
				letter->Path.Add(point);

				for(int j = 0; j <= (length - sch); j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)0;
					point.Translate(x, y - step*6);	
					point.Scale(1.2, 1.2);				
					letter->Path.Add(point);
				}
				break;
			}

		}

	}
}		
void _MatrixBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, sclX, sclY;
	int flag = 1;
	int sch = 0;
	int stop = 1;
	int step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		sch = 0;
		stop = 1;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
	    
	  	point.Clear();			
		point.Translate(x, y);
		point.Scale(1, 1);
		letter->Path.Add(point);

		point.Clear();	
		point.Alpha = 0;
		point.Translate(x, y + step*12);
		point.Scale(1, 1);
		letter->Path.Add(point);

		for(int j = 0; j < 7; j++)
		{
			flag = rand() % 2;
			if(flag == 0)
				flag = -1;
			else
				flag = 1;
		
			if(stop)
			{
				stop = rand() % 3;						
				sclX = rand() % 3;
				if(sclX == 0)
					sclX = 0.1;
				sclY = rand() % 3;
				if(sclY == 0)
					sclY = 0.1;
			
				point.Clear();
				point.Alpha = 0;
				point.Translate(x, y - step*12);
				point.Scale(sclX, sclY);
				letter->Path.Add(point);

				point.Clear();
				point.Alpha = Alpha;
				point.Translate(x, y);
				point.Scale(sclX, sclY);
				letter->Path.Add(point);
				
				point.Clear();	
				point.Alpha = 0;
				point.Translate(x, y + step*12);
				point.Scale(sclX, sclY);
				letter->Path.Add(point);	

			}
				
		}
	
	}
}



void HotPinkSpinBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];
				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Shear(0, 0);
				point.Scale(1, 1);						
				
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];
				
				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Shear(-0.5, 0);
				point.Scale(1, 3);								
				letter->Path.Add(point);
				
				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Shear(-2.25, 0);
				point.Scale(1, 4);								
				letter->Path.Add(point);  

			 	point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Shear(-4, 0);
				point.Scale(1, 5);								
				letter->Path.Add(point);  							   

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);
				point.Rotate(0, 0, 45);
				point.Shear(-3, 0);
				point.Scale(-1, 5);								
				letter->Path.Add(point);

			 	point.Clear();	
				point.Alpha = (BYTE)0;
				point.Translate(x, y);
				point.Rotate(0, 0, 90);
				point.Shear(-2, 0);
				point.Scale(-3, 5);								
				letter->Path.Add(point); 

				for(int j = 0; j <= length - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)0;
					point.Translate(x, y);
					point.Rotate(0, 0, 90);
					point.Shear(-2, 0);
					point.Scale(-3, 5);								
					letter->Path.Add(point); 
				}
				break;
			}

		}



	}
}		
void HopInJumpOutBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step, scl = 3., transX, transY, rot;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	if (step < 0.1)
		step = 0.1;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		scl = 3.;
		rot = 0;
		transY  = 12.;
		transX  = 3.;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length*4; temp++)
		{  
			if((i*4) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];				
				
				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);					
				point.Rotate(0, 0, 0);
				point.Scale(1, 1);										
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];
				transX = 0;
				for(int z = 0; z < 50; z++)
				{
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(x - step*transX, y - step * z * 0.02);
					if(z < 25)
						point.Rotate(0, 0, -z*0.25);				
					else
						point.Rotate(0, 0, z*0.25);						
					point.Scale(1, 1);								
					letter->Path.Add(point);

					scl += 0.02;
					
					transX += 0.02;
					
				}	
				

				//transX = 2;   
				for(int z = 0; z < 50; z++)
				{
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(x - step*transX, y - step * (50-z) * 0.02);
					if(z < 25)
						point.Rotate(0, 0, z*0.25);				
					else
						point.Rotate(0, 0, -z*0.25);				
					point.Scale(1, 1);								
					letter->Path.Add(point);

					scl += 0.02;
					
					transX += 0.02;
					
				}	

				point.Clear();	
				point.Alpha = (BYTE)Alpha;
				point.Translate(x - step*2, y);
				point.Rotate(0, 0, 0);				
				point.Scale(1, 1);								
				letter->Path.Add(point);

				transX = 2;
				transY = 0;
				rot = 90;
				for(int z = 0; z < 50; z++)
				{
					point.Clear();	
					if(point.Alpha > Alpha/100)
						point.Alpha -= Alpha/100;
					else
						point.Alpha = 0;
					point.Translate(x - step*transX, y - step*transY);
					point.Rotate(0, 0, rot);				
					point.Scale(scl + 1, scl);								
					letter->Path.Add(point);
					scl -= 0.04;
					rot -= 2;
					transX += 0.1;
					transY += 0.6/step;				  
				}

				for(int z = 0; z < 100; z++)
				{
					point.Clear();	
					if(point.Alpha > Alpha/200)
						point.Alpha -= Alpha/200;
					else
						point.Alpha = 0;

					point.Translate(x - step*transX, y - step*transY);
					point.Rotate(0, 0, 0);				
					point.Scale(scl + 1, scl);								
					letter->Path.Add(point);

					scl -= 0.01;
					transX -= 0.04;
					transY += 0.09;				  
				}	

				for(int j = 0; j <= length*4 - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)0;
					point.Translate(x - step*transX, y - step*transY);
					point.Rotate(0, 0, 0);			
					point.Scale(scl +1, scl);								
					letter->Path.Add(point); 									
				}
				break;
			}

		}

	}
}		



void ConJoinBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);
				point.Scale(1, 1);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/1.5;
				point.Translate(x, y);	
				point.Scale(0.95, 0.6);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/2;
				point.Translate(x, y);	
				point.Scale(0.8, 0.01);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(x, y);	
				point.Scale(0.65, -0.3);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/4;
				point.Translate(x, y + step*4);
				point.Scale(3, 3); 
				letter->Path.Add(point);

				for(int j = 0; j <= length - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)0;
					point.Translate(x, y);	
					point.Scale(3, 3);
					letter->Path.Add(point);					
				}
				break;
			}
		}

	}
}		
void HopsCotchBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	int sch;
	float x, y, step;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	sch = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		for(int temp = 0; temp <= length; temp++)
		{  
			if((i) >= temp)
			{
				sch++;

				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();
				point.Alpha = (BYTE)Alpha;
				point.Translate(x, y);
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);
			}
			else
			{
				
				x = finish[2*i];
				y = finish[2*i+1];

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(x - step, y);	
				point.Scale(1, 1);
				point.Rotate(0, 0, -50);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/3;
				point.Translate(x, y);	
				point.Scale(1, 1);
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);

				point.Clear();	
				point.Alpha = (BYTE)Alpha/4;
				point.Translate(x + step*10, y);
				point.Scale(3, 3); 
				point.Rotate(0, 0, 0);
				letter->Path.Add(point);								
		
				for(int j = 0; j <= length - sch; j++)
				{
					point.Clear();	
					point.Alpha = (BYTE)0;
					point.Translate(x - step*3, y);	
					point.Scale(3, 3);
					point.Rotate(0, 0, 0);
					letter->Path.Add(point);					
				}
				break;
			}

		}

	}
}		
void TwistBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )					 
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, center, multip = 0;
	int koef, centr_ind;
	
	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)

	center = (finish[2*(length - 1)] + finish[0])/2;
	centr_ind =	(int)( 0.7*(float)length);
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];
		
		point.Clear();
		point.Alpha = (BYTE)Alpha;
		point.Translate(center, y);	
		point.Scale(1, 1);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);

		if(i <= centr_ind)
		{
			multip++;
	
			x = finish[2*i];
			y = finish[2*i+1];
			for(int j = 0; j < multip; j++)
			{	
				if(j%2 == 1)
				{  
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(0.6, -0.6);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y + step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
				else
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(1, 1);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y - step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
			}

		}
		if(i > centr_ind)
		{					
			x = finish[2*i];
			y = finish[2*i+1];
			for(int j = 0; j < multip; j++)
			{
				if(j%2 == 1)
			
				{  
					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(0.6, -0.6);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y + step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}
				else
				{
					point.Clear();
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y);	
					point.Scale(1, 1);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);

					point.Clear();	
					point.Alpha = (BYTE)Alpha;
					point.Translate(center, y - step*4);	
					point.Scale(0.8, 0.01);
					point.Translate(-center + x, 0); 
					letter->Path.Add(point);
				}

			}

		}	  
	  	point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y);	
		point.Scale(0.6, -0.6);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
	}
}		 
void StarWarsBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )					 
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, center, multip = 0, shr = -1;
	int koef, centr_ind;
	int trans = -6;
	float scl = 0.3;

	int sch = 0;
	if(length == 1)
		center = 0;
	if((length == 2) || (length == 3))
		center = 1;
	if(length == 4)
		center = 2;
	if(length > 4)

	center = (finish[2*(length - 1)] + finish[0])/2;
	centr_ind =	(int)( 0.5*(float)length);
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{	
		scl	= 0.3;
		trans = -6;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)0;

		x = finish[2*i];
		y = finish[2*i+1];		
		for(int j = 0; j < 18; j++)
		{
			point.Clear();	
			point.Translate(center, y + step*trans);	
			point.Shear(shr, 0);
			point.Scale(scl, scl/1.5);
			point.Translate(-center + x, 0); 
			letter->Path.Add(point);

			point.Alpha = (BYTE)Alpha;
			scl += 0.15;
			trans++;
		}

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(center, y + step*trans);	
		point.Shear(shr, 0);
		point.Scale(scl, scl/1.5);
		point.Translate(-center + x, 0); 
		letter->Path.Add(point);
						 
		shr += (float)(2./length); 
	}
}		

void SqueezeBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter* letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, step, koef;
	if(length > 1)
		step = finish[2] - finish[0];
	else
		step = 10;

	for (int i = 0; i < length; i++)
	{ 
		koef = 0;
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];
		
		point.Clear();	
		point.Translate(x - step*koef, y);	
		point.Scale(1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);
		point.Scale(0.75, 1); 
		point.Shear(0, -0.25);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);
		point.Scale(0.5, 1); 
		point.Shear(0, -0.5);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(0.75, 1); 
		point.Shear(0, -0.25);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;

		
		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);
		point.Scale(0.01, 1); 
		point.Shear(0, 1);		
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)Alpha;
		point.Translate(x - step*koef, y);	
		point.Scale(-1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;
		
		point.Clear();	
		point.Alpha = (BYTE)Alpha/2;
		point.Translate(x - step*koef, y);	 
		point.Scale(0.01, 1); 
		point.Shear(0, -1);
		letter->Path.Add(point);
		koef--;

		point.Clear();	
		point.Alpha = (BYTE)0;
		point.Translate(x - step*koef, y);	
		point.Scale(1, 1); 
		point.Shear(0, 0);		
		letter->Path.Add(point);
		koef--;		

	}
}
void TerminatorBack( CSimpleArray<Letter> &letters, CSimpleArray<float> &finish, long Alpha )
{	
	Letter * letter;
	Waypoint point; 
	int length = letters.GetSize();
	float x, y, center;
	center = (finish[2*(length-1)] + finish[0])/2;

	for (int i = 0; i < length; i++)
	{
		letter = &(letters[i]);
		point.Name = letter->Name;
		point.Visible = TRUE;
		point.Alpha = (BYTE)Alpha;

		x = finish[2*i];
		y = finish[2*i+1];

		point.Clear();	
		point.Translate(x, y); 
		point.Scale(1, 1);
		point.Translate(0, 0);
		letter->Path.Add(point);

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(1, 0.1); 
		point.Translate(-center + x, 0);
		letter->Path.Add(point);

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(1, 0.1); 
		point.Translate(-center + x, 0);
		letter->Path.Add(point);

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(0.1, 0.1); 
		point.Translate(0, 0);
		letter->Path.Add(point);

		point.Alpha = (BYTE)0;

		point.Clear();			
		point.Translate(center, y); 
		point.Scale(0.1, 0.1); 
		point.Translate(0, 0);
		letter->Path.Add(point);
	}
}		
