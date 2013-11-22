#include "BaseTransform.h"

void CBaseTransform::fastconvert_RGB32BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame)
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width;
	long	h=InpFrame->Height;

	if (h<2)
	{
		if (h==1) memcpy(dst, src, 4*InpFrame->Width);
		return;
	}

	fastconvert_asm_blend_row_clipped((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);
	if (h-=2)
		do {
			dst = ((BYTE*)dst + OutFrame->Stride[0]);

			blend_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

			src = ((BYTE*)src + InpFrame->Stride[0]);
		} while(--h);
	fastconvert_asm_blend_row_clipped(((BYTE*)dst + OutFrame->Stride[0]), (BYTE*)src, w, InpFrame->Stride[0]);
}

void CBaseTransform::fastconvert_RGB24BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame)
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width*3/4;
	long	h=InpFrame->Height;

	if (h<2)
	{
		if (h==1) memcpy(dst, src, 3*InpFrame->Width);
		return;
	}

	fastconvert_asm_blend_row_clipped((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);
	if (h-=2)
		do {
			dst = ((BYTE*)dst + OutFrame->Stride[0]);

			blend_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

			src = ((BYTE*)src + InpFrame->Stride[0]);
		} while(--h);
	fastconvert_asm_blend_row_clipped(((BYTE*)dst + OutFrame->Stride[0]), (BYTE*)src, w, InpFrame->Stride[0]);
}

void CBaseTransform::fastconvert_YV12BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame)
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width/4;
	long	h=InpFrame->Height;

	if (h<2)
	{
		if (h==1) 
			memcpy(dst, src, InpFrame->Width);
		return;
	}

	fastconvert_asm_blend_row_clipped((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);
	if (h-=2)
	{
		do 
		{
			dst = ((BYTE*)dst + OutFrame->Stride[0]);

			blend_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

			src = ((BYTE*)src + InpFrame->Stride[0]);
		} 
		while(--h);
	}
	fastconvert_asm_blend_row_clipped(((BYTE*)dst + OutFrame->Stride[0]), (BYTE*)src, w, InpFrame->Stride[0]);

#if 0  
	//это вариант без деинтерлейсинга U и V плоскостей
	src=(BYTE*)InpFrame->Plane[1];
	dst=(BYTE*)OutFrame->Plane[1];

	for (i=0; i<InpFrame->Height/4; i++)
	{
		memcpy(dst, src, InpFrame->Width/2);
		dst = ((BYTE*)dst + OutFrame->Stride[1]);
		memcpy(dst, src, InpFrame->Width/2);
		dst = ((BYTE*)dst + OutFrame->Stride[1]);
		src = ((BYTE*)src + InpFrame->Stride[1]);
		src = ((BYTE*)src + InpFrame->Stride[1]);
	}

	src=(BYTE*)InpFrame->Plane[2];
	dst=(BYTE*)OutFrame->Plane[2];

	for (i=0; i<InpFrame->Height/4; i++)
	{
		memcpy(dst, src, InpFrame->Width/2);
		dst = ((BYTE*)dst + OutFrame->Stride[2]);
		memcpy(dst, src, InpFrame->Width/2);
		dst = ((BYTE*)dst + OutFrame->Stride[2]);
		src = ((BYTE*)src + InpFrame->Stride[2]);
		src = ((BYTE*)src + InpFrame->Stride[2]);
	}
#else
	src = (BYTE*)InpFrame->Plane[1];
	dst = (BYTE*)OutFrame->Plane[1];

	w = InpFrame->Width/8;
	h = InpFrame->Height/2;

	fastconvert_asm_blend_row_clipped((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[1]);
	if (h-=2)
	{
		do 
		{
			dst = ((BYTE*)dst + OutFrame->Stride[1]);

			blend_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[1]);

			src = ((BYTE*)src + InpFrame->Stride[1]);
		} 
		while(--h);
	}
	fastconvert_asm_blend_row_clipped(((BYTE*)dst + OutFrame->Stride[1]), (BYTE*)src, w, InpFrame->Stride[1]);
///////////////////////////////////////////////
	src = (BYTE*)InpFrame->Plane[2];
	dst = (BYTE*)OutFrame->Plane[2];

	w = InpFrame->Width/8;
	h = InpFrame->Height/2;

	fastconvert_asm_blend_row_clipped((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[2]);
	if (h-=2)
	{
		do 
		{
			dst = ((BYTE*)dst + OutFrame->Stride[2]);

			blend_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[2]);

			src = ((BYTE*)src + InpFrame->Stride[2]);
		} 
		while(--h);
	}
	fastconvert_asm_blend_row_clipped(((BYTE*)dst + OutFrame->Stride[2]), (BYTE*)src, w, InpFrame->Stride[2]);
#endif
}


void CBaseTransform::fastconvert_RGB32MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame)
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width;
	long	h=InpFrame->Height;
	int		i;

	if (h<2)
	{
		if (h==1)
			memcpy(OutFrame->Plane[0], InpFrame->Plane[0], 4*InpFrame->Width);
		return;
	}

	dst = ((BYTE*)dst + OutFrame->Stride[0]);
	for (i=0; i<(InpFrame->Height/2-1); i++)
	{
		memcpy(dst-OutFrame->Stride[0], src, 4*InpFrame->Width);

		middle_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

		dst = ((BYTE*)dst + 2*OutFrame->Stride[0]);
		src = ((BYTE*)src + 2*InpFrame->Stride[0]);
	}

	memcpy(dst-OutFrame->Stride[0], src, 4*InpFrame->Width);
	memcpy(dst, src+InpFrame->Stride[0], 4*InpFrame->Width);
}

void CBaseTransform::fastconvert_RGB24MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame)
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width*3/4;
	long	h=InpFrame->Height;
	int		i;

	if (h<2)
	{
		if (h==1)
			memcpy(OutFrame->Plane[0], InpFrame->Plane[0], 3*InpFrame->Width);
		return;
	}

	dst = ((BYTE*)dst + OutFrame->Stride[0]);
	for (i=0; i<(InpFrame->Height/2-1); i++)
	{
		memcpy(dst-OutFrame->Stride[0], src, 3*InpFrame->Width);

		middle_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

		dst = ((BYTE*)dst + 2*OutFrame->Stride[0]);
		src = ((BYTE*)src + 2*InpFrame->Stride[0]);
	}

	memcpy(dst-OutFrame->Stride[0], src, 3*InpFrame->Width);
	memcpy(dst, src+InpFrame->Stride[0], 3*InpFrame->Width);
}

void CBaseTransform::fastconvert_YV12MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame)
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	
	long	w=InpFrame->Width/4;
	long	h=InpFrame->Height;
	int i;
	
	if (h<2)
	{
		if (h==1)
		{
			memcpy(OutFrame->Plane[0], InpFrame->Plane[0], InpFrame->Width);
			memcpy(OutFrame->Plane[1], InpFrame->Plane[1], InpFrame->Width/2);
			memcpy(OutFrame->Plane[2], InpFrame->Plane[2], InpFrame->Width/2);
		}
		return;
	}

	dst = ((BYTE*)dst + OutFrame->Stride[0]);
	
	for (i=0; i<(InpFrame->Height/2-1); i++)
	{
		memcpy(dst-OutFrame->Stride[0], src, 3*InpFrame->Width);

		middle_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

		dst = ((BYTE*)dst + 2*OutFrame->Stride[0]);
		src = ((BYTE*)src + 2*InpFrame->Stride[0]);
	}

	long sz = InpFrame->Width;// с какого здесь еще стоял множитель "3"????
	
	memcpy(dst-OutFrame->Stride[0], src, sz);
	memcpy(dst, src+InpFrame->Stride[0], sz);
 
#if 0  
	//это вариант без деинтерлейсинга U и V плоскостей
	src=(BYTE*)InpFrame->Plane[1];
	dst=(BYTE*)OutFrame->Plane[1];

	for (i=0; i<InpFrame->Height/2; i++)
	{
		memcpy(dst, src, InpFrame->Width/2);
		dst = ((BYTE*)dst + OutFrame->Stride[1]);
		src = ((BYTE*)src + InpFrame->Stride[1]);
	}

	src=(BYTE*)InpFrame->Plane[2];
	dst=(BYTE*)OutFrame->Plane[2];

	for (i=0; i<InpFrame->Height/2; i++)
	{
		memcpy(dst, src, InpFrame->Width/2);
		dst = ((BYTE*)dst + OutFrame->Stride[2]);
		src = ((BYTE*)src + InpFrame->Stride[2]);
	}
#else
	src = (BYTE*)InpFrame->Plane[1];
	dst = (BYTE*)OutFrame->Plane[1];
	w = InpFrame->Width/8;
	h = InpFrame->Height/2;

	dst = ((BYTE*)dst + OutFrame->Stride[1]);
	for (i=0; i<(InpFrame->Height/4 - 1); i++)
	{
		memcpy(dst-OutFrame->Stride[1], src, 3*(InpFrame->Width/2));

		middle_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[1]);

		dst = ((BYTE*)dst + 2*OutFrame->Stride[1]);
		src = ((BYTE*)src + 2*InpFrame->Stride[1]);
	}

	memcpy(dst - OutFrame->Stride[1], src,sz/2);
	memcpy(dst, src + InpFrame->Stride[1], sz/2);

//////////////////////////
	src = (BYTE*)InpFrame->Plane[2];
	dst = (BYTE*)OutFrame->Plane[2];

	dst = ((BYTE*)dst + OutFrame->Stride[2]);
	for (i=0; i<(InpFrame->Height/4 - 1); i++)
	{
		memcpy(dst-OutFrame->Stride[2], src, 3*(InpFrame->Width/2));

		middle_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[2]);

		dst = ((BYTE*)dst + 2*OutFrame->Stride[2]);
		src = ((BYTE*)src + 2*InpFrame->Stride[2]);
	}

	memcpy(dst - OutFrame->Stride[2], src, sz/2);
	memcpy(dst, src + InpFrame->Stride[2], sz/2);

#endif
}
void CBaseTransform::fastconvert_YUY2BlendDeinterlace(SUncompressedVideoFrame* InpFrame,
													  SUncompressedVideoFrame* OutFrame )
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width/2;
	long	h=InpFrame->Height;

	if (h<2)
	{
		if (h==1) memcpy(dst, src, InpFrame->Width);
		return;
	}

	fastconvert_asm_blend_row_clipped((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);
	if (h-=2)
		do {
			dst = ((BYTE*)dst + OutFrame->Stride[0]);

			blend_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

			src = ((BYTE*)src + InpFrame->Stride[0]);
		} while(--h);
	fastconvert_asm_blend_row_clipped(((BYTE*)dst + OutFrame->Stride[0]), (BYTE*)src, w, InpFrame->Stride[0]);
}
void CBaseTransform::fastconvert_YUY2MiddleDeinterlace(SUncompressedVideoFrame* InpFrame,
													   SUncompressedVideoFrame* OutFrame )
{
	BYTE*	src=(BYTE*)InpFrame->Plane[0];
	BYTE*	dst=(BYTE*)OutFrame->Plane[0];
	long	w=InpFrame->Width/2;
	long	h=InpFrame->Height;
	int		i;

	if (h<2)
	{
		if (h==1)
			memcpy(OutFrame->Plane[0], InpFrame->Plane[0], 2*InpFrame->Width);
		return;
	}

	dst = ((BYTE*)dst + OutFrame->Stride[0]);
	for (i=0; i<(InpFrame->Height/2-1); i++)
	{
		memcpy(dst-OutFrame->Stride[0], src, 2*InpFrame->Width);

		middle_row((BYTE*)dst, (BYTE*)src, w, InpFrame->Stride[0]);

		dst = ((BYTE*)dst + 2*OutFrame->Stride[0]);
		src = ((BYTE*)src + 2*InpFrame->Stride[0]);
	}

	memcpy(dst-OutFrame->Stride[0], src, 2*InpFrame->Width);
	memcpy(dst, src+InpFrame->Stride[0], 2*InpFrame->Width);
}