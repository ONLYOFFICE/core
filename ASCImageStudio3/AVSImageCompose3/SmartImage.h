#pragma once

#include <ImageColors.h>
#include <ImageCore.h>

class SmartImage
{
	ImageStudio::Core::MediaData m_oMediaData;
	
public:
	
	int Width;
	int Height;
	BYTE* Pixels;
	BYTE* Backup;
	
public:
	
	SmartImage()
	{
		Width = 0;
		Height = 0;
		Pixels = 0;
		Backup = 0;
	}
	~SmartImage()
	{
		Destroy();
	}
	
	void Destroy()
	{
		m_oMediaData.Destroy();

		if (Backup)
		{
			delete[] Backup;
			Backup = 0;
		}

		Width = 0;
		Height = 0;
		Pixels = 0;
	}
	BOOL IsValid()
	{
		if (Width < 1 || Height < 1 || !Pixels)
			return FALSE;
		return TRUE;
	}
		
	BOOL BackupImage()
	{
		// check for valid image
		if (!IsValid())
			return FALSE;

		// delete existing backup (if any)
		if (Backup)
		{
			delete[] Backup;
			Backup = 0;
		}

		// allocate memory for backup
		Backup = new BYTE[4*Width*Height];

		// check for valid image
		if (!Backup)
			return FALSE;

		// save memory
		memcpy(Backup, Pixels, 4*Width*Height);

		// all ok
		return TRUE;
	}
	BOOL CopyFrom(SmartImage& image)
	{
		// check for valid image
		if (!image.IsValid())
			return FALSE;

		// check for the same sizes
		if (Width != image.Width || Height != image.Height)
			return FALSE;

		// copy memory context
		memcpy(Pixels, image.Pixels, 4*Width*Height);

		// backup image (if necessary)
		if (image.Backup)
		{
			return BackupImage();
		}

		// all ok
		return TRUE;
	}
	ImageStudio::Core::MediaData& GetMediaData()
	{
		return m_oMediaData;
	}
	
	BOOL Extract(SAFEARRAY** Array, BOOL BackupIt = FALSE)
	{
		// destroy existing image
		Destroy();

		int Channels = 0;
		Pixels = ImageStudioUtils::ExtractImage(Array, Width, Height, Channels);

		if (!Pixels)
			return FALSE;

		// backup image (if necessary)
		if (BackupIt)
			return BackupImage();
			
		// all ok
		return IsValid();
	}
	BOOL Extract(IUnknown** pMediaData, BOOL BackupIt = FALSE)
	{
		if (NULL == pMediaData)
			return FALSE;

		Destroy();

		if (!m_oMediaData.Create(*pMediaData, FALSE) || !m_oMediaData.IsValidBGRA())
			return FALSE;

		Pixels = m_oMediaData.GetBuffer();
		Width = m_oMediaData.GetWidth();
		Height = m_oMediaData.GetHeight();

		// backup image (if necessary)
		if (BackupIt)
			return BackupImage();

		return IsValid();
	}
		
	BYTE GetBilinear(double x, double y, int c, BYTE def)
	{
		double e = 1; // 0.0001
		if (x < -e || y < -e || x > Width - 1 + e || y > Height - 1 + e)
			return def;

		if (x < 0)			x = 0;
		if (y < 0)			y = 0;
		if (x > Width - 1)	x = Width - 1;
		if (y > Height - 1)	y = Height - 1;

		int nX = (int)floor(x);
		int nY = (int)floor(y);

		// no interpolation: return Pixels[c + 4*(nX + nY*Width)];

		if (nX == Width - 1)	
			nX = Width - 2; 
		if (nY == Height - 1)
			nY = Height - 2;

		double dX = x - nX;
		double dY = y - nY;

		int nIndex = c + 4*(nX + nY*Width);

		BYTE byLT = Pixels[nIndex];
		BYTE byRT = Pixels[nIndex + 4];
		BYTE byRB = Pixels[nIndex + 4 + 4*Width];
		BYTE byLB = Pixels[nIndex + 4*Width];

		return (BYTE)
		(
			(1 - dX)*(1 - dY)*byLT + // Left - Top corner
			(    dX)*(1 - dY)*byRT + // Right - Top corner
			(    dX)*(    dY)*byRB + // Right - Bottom corner
			(1 - dX)*(    dY)*byLB   // Left - Bottom corner
		);
	}
};