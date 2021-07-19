﻿/*
 * File:	ximagif.cpp
 * Purpose:	Platform Independent GIF Image Class Loader and Writer
 * 07/Aug/2001 Davide Pizzolato - www.xdp.it
 * CxImage version 7.0.2 07/Feb/2011
 */

#include "ximagif.h"

#if CXIMAGE_SUPPORT_GIF

#include "ximaiter.h"

#if defined (_WIN32_WCE)
	#define assert(s)
#else
	#include <assert.h>
#endif

#define USE_RGB_PALETTE
#ifdef USE_RGB_PALETTE

#include <vector>
namespace NSGeneratePalette
{
    class CImage32bit_BGRA
    {
    public:
        unsigned char* m_pPixels;
        int m_lWidth;
        int m_lHeight;
    };

    class CImage8bit
    {
    public:
        unsigned char* m_pPixels;
        int m_lWidth;
        int m_lHeight;

    public:

        CImage8bit()
        {
            m_pPixels = NULL;
            m_lWidth = 0;
            m_lHeight = 0;
        }

        ~CImage8bit()
        {
            Destroy(TRUE);
        }

        void Destroy(BOOL bAttack = TRUE)
        {
            if (bAttack)
            {
                if (NULL != m_pPixels)
                    delete[] m_pPixels;
            }
            m_pPixels = NULL;
        }

        bool Create( int lWidth, int lHeight )
        {
            unsigned char* pPixels = new unsigned char[lWidth * lHeight];
            if( !pPixels )
                return false;

            m_pPixels = pPixels;
            m_lWidth  = lWidth;
            m_lHeight = lHeight;

            return true;
        }

        static bool ImageCut ( CImage8bit* pSource, int X, int Y, int Width, int Height, CImage8bit** pImage )
        {
            if ( NULL != pSource )
            {
                int SrcWidth		=	pSource->m_lWidth;
                int SrcHeigth		=	pSource->m_lHeight;

                unsigned char* pSrc	=	pSource->m_pPixels;

                if ( ( X + Width ) > SrcWidth || ( Y + Height ) > SrcHeigth || NULL == pSrc )
                {
                    return false;
                }

                (*pImage)	=	new CImage8bit ();
                if ( NULL != pImage )
                {
                    (*pImage)->Create ( Width, Height );

                    unsigned char* pDst	=	(*pImage)->m_pPixels;

                    pSrc			+=	X + Y * SrcWidth;

                    for ( int i = 0; i < Height; ++i )
                    {
                        ::memcpy ( pDst, pSrc, Width );
                        pDst		+=	Width;
                        pSrc		+=	SrcWidth;
                    }

                    return true;
                }
            }

            return false;
        }
    };

    struct SColor
    {
        union
        {
            unsigned int Pixel;

            unsigned char Color[4];

            struct
            {
                unsigned char B;
                unsigned char G;
                unsigned char R;
                unsigned char A;
            };
        };

        unsigned char* RefByDstPixel;

        SColor()
        {
            Pixel = 0;
            RefByDstPixel = 0;
        }

        SColor( const SColor& other )
        {
            Pixel = other.Pixel;
            RefByDstPixel = other.RefByDstPixel;
        }

        void SetBgraColor( unsigned char* pBgraColor, unsigned char* pDstPixel )
        {
            Pixel = *((unsigned int*)pBgraColor);
            RefByDstPixel = pDstPixel;

            A = (A & ~15) + 15; // квантуем альфу
        }
    };


    struct SRect3D
    {
        unsigned char MaxWidth; // максимальная ширина
        unsigned char MaxColor; // цвет максимальной ширины: 0 - blue, 1 - green, 2 - red, 3 - alpha

        SRect3D()
        {
            Clear();
        }

        void Clear()
        {
            MaxWidth = 0;
            MaxColor = 0;
        }
    };

    class CPaletteCreator
    {
    public:
        class CBounder
        {
        public:
            SColor*         m_arrPoints;
            unsigned int    m_nCountPoints;

            SRect3D m_oRect;

        public:
            CBounder()
            {
                m_arrPoints = NULL;
                m_nCountPoints = 0;
            }

            ~CBounder()
            {
                // удалять ничего не нужно, так как нет динамических данных
            }


            void Init( SColor* points, unsigned int count )
            {
                m_arrPoints = points;
                m_nCountPoints = count;

                Normalize();
            }

            void Clear()
            {
                m_arrPoints = 0;
                m_nCountPoints = 0;

                m_oRect.Clear();
            }

            void Normalize()
            {
                if( 0 == m_nCountPoints )
                {
                    m_oRect.Clear();
                    return;
                }

                if( 1 == m_nCountPoints )
                {
                    m_oRect.MaxWidth = 0;
                    m_oRect.MaxColor = 0;
                    return;
                }

                // нормализуем по точкам, убивая лишние грани...
                int nMinB, nMaxB;
                int nMinG, nMaxG;
                int nMinR, nMaxR;
                int nMinA, nMaxA;

                SColor* pPoint = m_arrPoints;

                nMinB = nMaxB = pPoint->B;
                nMinG = nMaxG = pPoint->G;
                nMinR = nMaxR = pPoint->R;
                nMinA = nMaxA = pPoint->A;

                for( unsigned int i = 1; i < m_nCountPoints; ++i, ++pPoint )
                {
                    int B = pPoint->B;
                    int G = pPoint->G;
                    int R = pPoint->R;
                    int A = pPoint->A;

                    if( B < nMinB ) nMinB = B;
                    if( B > nMaxB ) nMaxB = B;
                    if( G < nMinG ) nMinG = G;
                    if( G > nMaxG ) nMaxG = G;
                    if( R < nMinR ) nMinR = R;
                    if( R > nMaxR ) nMaxR = R;
                    if( A < nMinA ) nMinA = A;
                    if( A > nMaxA ) nMaxA = A;
                }

                nMaxB -= nMinB;
                nMaxG -= nMinG;
                nMaxR -= nMinR;
                nMaxA -= nMinA;

                int nMaxWidth = nMaxB;
                int nMaxColor = 0;

                if( nMaxWidth < nMaxG )
                {
                    nMaxWidth = nMaxG;
                    nMaxColor = 1;
                }

                if( nMaxWidth < nMaxR )
                {
                    nMaxWidth = nMaxR;
                    nMaxColor = 2;
                }

                if( nMaxWidth < nMaxA )
                {
                    nMaxWidth = nMaxA;
                    nMaxColor = 3;
                }

                m_oRect.MaxWidth = nMaxWidth;
                m_oRect.MaxColor = nMaxColor;
            }

            void GetHistogramm( unsigned int Table[256], int nColorType ) const
            {
                memset( Table, 0, 256 * sizeof(unsigned int) );

                if( !m_nCountPoints )
                    return;

                SColor* pPoint = m_arrPoints;
                for( unsigned int i = 0; i < m_nCountPoints; ++i, ++pPoint )
                {
                    Table[pPoint->Color[nColorType]] += 1;
                }
            }


            bool CreateNew( CBounder& bound1, CBounder& bound2 ) const
            {
                // перераспределяем точки в массиве на две части.
                if( 0 == m_oRect.MaxWidth )
                {
                    return FALSE;
                }

                // сначала определяем самую длинную сторону...
                int nColorType = m_oRect.MaxColor;

                unsigned int hist[256];
                GetHistogramm( hist, nColorType );

                unsigned int nHalfCountPoints = m_nCountPoints / 2;
                unsigned int nIndexH = 0;
                unsigned int nCurCount = 0;

                do
                {
                    nCurCount += hist[nIndexH++];
                } while( nCurCount < nHalfCountPoints );

                --nIndexH;

                if( nCurCount == m_nCountPoints )
                    nCurCount -= hist[nIndexH--];

                SColor* pPoint1 = m_arrPoints;
                SColor* pPoint2 = m_arrPoints + m_nCountPoints;

                while( pPoint1 != pPoint2 )
                {
                    if( pPoint1->Color[nColorType] <= nIndexH )
                    {
                        ++pPoint1;
                    }
                    else
                    {
                        --pPoint2;

                        SColor tmp( *pPoint1 );
                        *pPoint1 = *pPoint2;
                        *pPoint2 = tmp;
                    }
                }

                bound1.m_arrPoints = m_arrPoints;
                bound1.m_nCountPoints = (unsigned int)(((size_t)pPoint2 - (size_t)m_arrPoints) / sizeof(SColor));

                bound2.m_arrPoints = m_arrPoints + bound1.m_nCountPoints;
                bound2.m_nCountPoints = m_nCountPoints - bound1.m_nCountPoints;

                bound1.Normalize();
                bound2.Normalize();

                return true;
            }

            unsigned int GetColor() const
            {
                union
                {
                    unsigned int value;

                    struct
                    {
                        unsigned char B;
                        unsigned char G;
                        unsigned char R;
                        unsigned char A;
                    };
                } color;

                SColor* pPoint = m_arrPoints;
                if( !pPoint )
                    return 0;

                color.value = pPoint->Pixel;

                if( 0 == m_oRect.MaxWidth )
                {
                    return color.value;
                }

                double dB = pPoint->B;
                double dG = pPoint->G;
                double dR = pPoint->R;
                double dA = pPoint->A;

                for( unsigned int i = 1; i < m_nCountPoints; ++i, ++pPoint )
                {
                    dB += pPoint->B;
                    dG += pPoint->G;
                    dR += pPoint->R;
                    dA += pPoint->A;
                }

                color.B = (unsigned char)(dB / m_nCountPoints + 0.5);
                color.G = (unsigned char)(dG / m_nCountPoints + 0.5);
                color.R = (unsigned char)(dR / m_nCountPoints + 0.5);
                color.A = (unsigned char)(dA / m_nCountPoints + 0.5);

                return color.value;
            }

            void ApplyPaletteColor( unsigned char color ) const
            {
                SColor* pPoint = m_arrPoints;
                for( unsigned int i = 0; i < m_nCountPoints; ++i, ++pPoint )
                {
                    pPoint->RefByDstPixel[0] = color;
                }
            }
        };

    public:

        CPaletteCreator()
        {
        }

        ~CPaletteCreator()
        {
        }

        void Destroy()
        {
        }

        std::vector<CImage8bit*> Convert(std::vector<CImage32bit_BGRA>& arImages, unsigned int** pDstPalette)
        {
            std::vector<CImage8bit*> arDst;
            int nCountImages = (int)arImages.size();

            if( !nCountImages || pDstPalette == NULL )
                return arDst;

            // создаём палитру
            unsigned int* pPalette = new unsigned int[256];

            *pDstPalette = pPalette;

            if( !pPalette )
                return arDst;

            memset( pPalette, 0, 256 * sizeof(unsigned int) );

            unsigned int lCountPoints = 0;

            for( int nImage = 0; nImage < nCountImages; ++nImage )
            {
                int lWidth  = arImages[nImage].m_lWidth;
                int lHeight = arImages[nImage].m_lHeight;

                CImage8bit* pImage8 = new CImage8bit();
                if( pImage8 )
                {
                    if( pImage8->Create( lWidth, lHeight ) )
                    {
                        lCountPoints += lWidth * lHeight;

                        ::memset( pImage8->m_pPixels, 0, lWidth * lHeight );
                    }
                    else
                    {
                        delete pImage8;
                        pImage8 = NULL;
                    }
                }

                arDst.push_back( pImage8 );
            }

            SColor* arrPoints = (SColor*)(new unsigned char[lCountPoints * sizeof(SColor)]);
            if( !arrPoints )
                return arDst;

            SColor* pPoint = arrPoints;

            for( int nImage = 0; nImage < nCountImages; ++nImage )
            {
                CImage8bit* pImage8 = arDst[nImage];
                if( !pImage8 )
                    continue;

                unsigned int lCountPixels = pImage8->m_lWidth * pImage8->m_lHeight;
                unsigned char* pBgraPixel = arImages[nImage].m_pPixels;

                for( unsigned int i = 0; i < lCountPixels; ++i, pBgraPixel += 4 )
                {
                    if( pBgraPixel[3] > 4 )
                    {
                        pPoint->SetBgraColor( pBgraPixel, pImage8->m_pPixels + i );

                        ++pPoint;
                    }
                }
            }

            if( pPoint == arrPoints )
            {
                delete [] (unsigned char*)arrPoints;
                return arDst;
            }

            size_t nActivePoints = (size_t(pPoint) - size_t(arrPoints)) / sizeof(SColor);

            CBounder bounds[255];

            CBounder oNew1;
            CBounder oNew2;

            bounds[0].Init( arrPoints, nActivePoints );
            int nCountBounds = 1;

            while( nCountBounds < 255 )
            {
                int nBoundIndex = FindBoundIndexWithMaxWidth( bounds, nCountBounds );
                if( nBoundIndex < 0 )
                    break;

                bounds[nBoundIndex].CreateNew( oNew1, oNew2 );

                bounds[nBoundIndex]  = oNew1;
                bounds[nCountBounds] = oNew2;

                ++nCountBounds;
            }

            for( int i = 0; i < nCountBounds; i++ )
            {
                pPalette[i + 1] = bounds[i].GetColor();
                bounds[i].ApplyPaletteColor( i + 1 );
            }

            delete [] (unsigned char*)arrPoints;

            //for( int nImage = 0; nImage < nCountImages; ++nImage )
            //{
            //	CImage8bit* pImage8 = arDst[nImage];
            //	if( !pImage8 )
            //		continue;

            //	if( !pImage8->m_pPixels )
            //		continue;

            //	unsigned int nSize = pImage8->m_lWidth * pImage8->m_lHeight;
            //	unsigned int* buffer = new unsigned int[nSize];
            //	if( !buffer )
            //		continue;

            //	for( unsigned int i = 0; i < nSize; i++ )
            //	{
            //		buffer[i] = pPalette[pImage8->m_pPixels[i]];
            //	}

            //	delete [] buffer;
            //}

            return arDst;
        }

        static int FindBoundIndexWithMaxWidth( const CBounder* pBounds, int nCountBounds )
        {
            int nMaxWidth = 0;
            int nMaxIndex = -1;

            for( int i = 0; i < nCountBounds; ++i, ++pBounds )
            {
                int width = pBounds->m_oRect.MaxWidth;
                if( width > nMaxWidth )
                {
                    nMaxWidth = width;
                    nMaxIndex = i;
                }
            }

            return nMaxIndex;
        }

        unsigned char* CreateFrom8bit(CImage8bit* pImage, unsigned int* pPalette)
        {
            unsigned char* pData = new unsigned char[4 * pImage->m_lWidth * pImage->m_lHeight];
            unsigned char* pDataMem = pData;

            long lCount = pImage->m_lWidth * pImage->m_lHeight;

            for (long nIndex = 0; nIndex < lCount; ++nIndex)
            {
                unsigned int color = pPalette[pImage->m_pPixels[nIndex]]; // ARGB

                pDataMem[2] = (unsigned char)((color >> 16) & 0xFF);
                pDataMem[1] = (unsigned char)((color >> 8) & 0xFF);
                pDataMem[0] = (unsigned char)(color & 0xFF);
                pDataMem[3] = (unsigned char)(color >> 24);
                pDataMem += 4;
            }

            return pData;
        }
    };
}

#endif

////////////////////////////////////////////////////////////////////////////////
CxImageGIF::CxImageGIF(): CxImage(CXIMAGE_FORMAT_GIF)
{
	buf = new uint8_t [GIFBUFTAM + 1];

	stack = new uint8_t [MAX_CODES + 1];
	suffix = new uint8_t [MAX_CODES + 1];
	prefix = new uint16_t [MAX_CODES + 1];

	htab = new int32_t [HSIZE];
	codetab = new uint16_t [HSIZE];

	byte_buff = new uint8_t [257];
	accum = new char [256];
	m_comment = new char [256];

	m_loops=0;
	info.dispmeth=0;
	m_comment[0]='\0';

}
////////////////////////////////////////////////////////////////////////////////
CxImageGIF::~CxImageGIF()
{
	delete [] buf;

	delete [] stack;
	delete [] suffix;
	delete [] prefix;

	delete [] htab;
	delete [] codetab;

	delete [] byte_buff;
	delete [] accum;
	delete [] m_comment;
}
////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
bool CxImageGIF::Decode(CxFile *fp)
{
	/* AD - for transparency */
	struct_dscgif dscgif;
	struct_image image;
	struct_TabCol TabCol;

	if (fp == NULL) return false;

	fp->Read(&dscgif,/*sizeof(dscgif)*/13,1);
	//if (strncmp(dscgif.header,"GIF8",3)!=0) {
	if (strncmp(dscgif.header,"GIF8",4)!=0) return FALSE;

	// Avoid Byte order problem with Mac <AMSN>
	dscgif.scrheight = m_ntohs(dscgif.scrheight);
	dscgif.scrwidth = m_ntohs(dscgif.scrwidth);

	if (info.nEscape == -1) {
		// Return output dimensions only
		head.biWidth = dscgif.scrwidth;
		head.biHeight = dscgif.scrheight;
		info.dwType = CXIMAGE_FORMAT_GIF;
		return true;
	}

	/* AD - for interlace */
	TabCol.sogct = (int16_t)(1 << ((dscgif.pflds & 0x07)+1));
	TabCol.colres = (int16_t)(((dscgif.pflds & 0x70) >> 4) + 1);

	// assume that the image is a truecolor-gif if
	// 1) no global color map found
	// 2) (image.w, image.h) of the 1st image != (dscgif.scrwidth, dscgif.scrheight)
	int32_t bTrueColor=0;
	CxImage* imaRGB=NULL;

	// Global colour map?
	if (dscgif.pflds & 0x80)
		fp->Read(TabCol.paleta,sizeof(struct rgb_color)*TabCol.sogct,1);
	else 
		bTrueColor++;	//first chance for a truecolor gif

	int32_t first_transparent_index = 0;

	int32_t iImage = 0;
	info.nNumFrames=get_num_frames(fp,&TabCol,&dscgif);

	if ((info.nFrame<0)||(info.nFrame>=info.nNumFrames)) return false;

	//it cannot be a true color GIF with only one frame
	if (info.nNumFrames == 1)
		bTrueColor=0;

	char ch;
	bool bPreviousWasNull = true;
	int32_t  prevdispmeth = 0;
	CxImage *previousFrame = NULL;

	for (BOOL bContinue = TRUE; bContinue; )
	{
		if (fp->Read(&ch, sizeof(ch), 1) != 1) {break;}

		if (info.nEscape > 0) return false; // <vho> - cancel decoding
		if (bPreviousWasNull || ch==0)
		{
			switch (ch)
			{
			case '!': // extension
				{
				bContinue = DecodeExtension(fp);
				break;
				}
			case ',': // image
				{
				assert(sizeof(image) == 9);
				fp->Read(&image,sizeof(image),1);
				//avoid byte order problems with Solaris <candan> <AMSN>
				image.l = m_ntohs(image.l);
				image.t = m_ntohs(image.t);
				image.w = m_ntohs(image.w);
				image.h = m_ntohs(image.h);

				if (((image.l + image.w) > dscgif.scrwidth)||((image.t + image.h) > dscgif.scrheight))
					break;

				// check if it could be a truecolor gif
				if ((iImage==0) && (image.w != dscgif.scrwidth) && (image.h != dscgif.scrheight))
					bTrueColor++;

				rgb_color  locpal[256];				//Local Palette 
				rgb_color* pcurpal = TabCol.paleta;	//Current Palette 
				int16_t palcount = TabCol.sogct;		//Current Palette color count  

				// Local colour map?
				if (image.pf & 0x80) {
					palcount = (int16_t)(1 << ((image.pf & 0x07) +1));
					assert(3 == sizeof(struct rgb_color));
					fp->Read(locpal,sizeof(struct rgb_color)*palcount,1);
					pcurpal = locpal;
				}

				int32_t bpp; //<DP> select the correct bit per pixel value
				if		(palcount <= 2)  bpp = 1;
				else if (palcount <= 16) bpp = 4;
				else					 bpp = 8;

				CxImageGIF backimage;
				backimage.CopyInfo(*this);
				if (iImage==0){
					//first frame: build image background
					backimage.Create(dscgif.scrwidth, dscgif.scrheight, bpp, CXIMAGE_FORMAT_GIF);
					first_transparent_index = info.nBkgndIndex;
					backimage.Clear((uint8_t)gifgce.transpcolindex);
					previousFrame = new CxImage(backimage);
					previousFrame->SetRetreiveAllFrames(false);
				} else {
				//generic frame: handle disposal method from previous one
				/*Values :  0 -   No disposal specified. The decoder is
								  not required to take any action.
							1 -   Do not dispose. The graphic is to be left
								  in place.
							2 -   Restore to background color. The area used by the
								  graphic must be restored to the background color.
							3 -   Restore to previous. The decoder is required to
								  restore the area overwritten by the graphic with
								  what was there prior to rendering the graphic.
				*/
				/*	backimage.Copy(*this);
					if (prevdispmeth==2){
						backimage.Clear((uint8_t)first_transparent_index);
					}*/
					if (prevdispmeth==2){
						backimage.Copy(*this,false,false,false);
						backimage.Clear((uint8_t)first_transparent_index);
					} else if (prevdispmeth==3) {
						backimage.Copy(*this,false,false,false);
						backimage.Create(previousFrame->GetWidth(),
							previousFrame->GetHeight(),
							previousFrame->GetBpp(),CXIMAGE_FORMAT_GIF);
						memcpy(backimage.GetDIB(),previousFrame->GetDIB(),
							backimage.GetSize());
						//backimage.AlphaSet(*previousFrame);
					} else {
						backimage.Copy(*this);
					}
				}

				//active frame
				Create(image.w, image.h, bpp, CXIMAGE_FORMAT_GIF);

				if ((image.pf & 0x80) || (dscgif.pflds & 0x80)) {
					uint8_t r[256], g[256], b[256];
					int32_t i, has_white = 0;

					for (i=0; i < palcount; i++) {
						r[i] = pcurpal[i].r;
						g[i] = pcurpal[i].g;
						b[i] = pcurpal[i].b;
						if (RGB(r[i],g[i],b[i]) == 0xFFFFFF) has_white = 1;
					}

					// Force transparency colour white...
					//if (0) if (info.nBkgndIndex >= 0)
					//	r[info.nBkgndIndex] = g[info.nBkgndIndex] = b[info.nBkgndIndex] = 255;
					// Fill in with white // AD
					if (info.nBkgndIndex >= 0) {
						while (i < 256)	{
							has_white = 1;
							r[i] = g[i] = b[i] = 255;
							i++;
						}
					}

					// Force last colour to white...   // AD
					//if ((info.nBkgndIndex >= 0) && !has_white) {
					//	r[255] = g[255] = b[255] = 255;
					//}

					SetPalette((info.nBkgndIndex >= 0 ? 256 : palcount), r, g, b);
				}

				CImageIterator* iter = new CImageIterator(this);
				iter->Upset();
				int32_t badcode=0;
				ibf = GIFBUFTAM+1;

				interlaced = image.pf & 0x40;
				iheight = image.h;
				istep = 8;
				iypos = 0;
				ipass = 0;

				int32_t pos_start = fp->Tell();
				//if (interlaced) log << "Interlaced" << endl;
				decoder(fp, iter, image.w, badcode);
				delete iter;

				if (info.nEscape) return false; // <vho> - cancel decoding

				if (bTrueColor<2 ){ //standard GIF: mix frame with background
					backimage.IncreaseBpp(bpp);
					backimage.GifMix(*this,image);
					backimage.SetTransIndex(first_transparent_index);
					backimage.SetPalette(GetPalette());
					Transfer(backimage,false);
				} else { //it's a truecolor gif!
					//force full image decoding
					info.nFrame=info.nNumFrames-1;
					//build the RGB image
					if (imaRGB==NULL) imaRGB = new CxImage(dscgif.scrwidth,dscgif.scrheight,24,CXIMAGE_FORMAT_GIF);
					//copy the partial image into the full RGB image
					for(int32_t y=0;y<image.h;y++){
						for (int32_t x=0;x<image.w;x++){
							imaRGB->SetPixelColor(x+image.l,dscgif.scrheight-1-image.t-y,GetPixelColor(x,image.h-y-1));
						}
					}
				}

				prevdispmeth = (gifgce.flags >> 2) & 0x7;

				//restore the correct position in the file for the next image
				if (badcode){
					seek_next_image(fp,pos_start);
				} else {
					fp->Seek(-(ibfmax - ibf - 1), SEEK_CUR);
				}

				if (info.bGetAllFrames && imaRGB == NULL) {
					if (iImage == 0) {
						DestroyFrames();
						ppFrames = new CxImage*[info.nNumFrames];
						for(int32_t frameIdx = 0; frameIdx < info.nNumFrames; frameIdx++){
							ppFrames[frameIdx] = NULL;
						}
					}
					ppFrames[iImage] = new CxImage(*this);
					ppFrames[iImage]->SetRetreiveAllFrames(false);
				}
				if (prevdispmeth <= 1) {
					delete previousFrame;
					previousFrame = new CxImage(*this);
					previousFrame->SetRetreiveAllFrames(false);
				}

				if ((info.nFrame==iImage) && (info.bGetAllFrames==false)) bContinue=false; else iImage++;

				break;
				}
			case ';': //terminator
				bContinue=false;
				break;
			default:
				bPreviousWasNull = (ch==0);
				break;
			}
		}
	}

	if (bTrueColor>=2 && imaRGB){
		if (gifgce.flags & 0x1){
			imaRGB->SetTransColor(GetPaletteColor((uint8_t)info.nBkgndIndex));
			imaRGB->SetTransIndex(0);
		}
		Transfer(*imaRGB);
	}
	delete imaRGB;

	delete previousFrame;

	return true;

}
////////////////////////////////////////////////////////////////////////////////
bool CxImageGIF::DecodeExtension(CxFile *fp)
{
	bool bContinue;
	uint8_t count;
	uint8_t fc;

	bContinue = (1 == fp->Read(&fc, sizeof(fc), 1));
	if (bContinue) {
		/* AD - for transparency */
		if (fc == 0xF9)	{
			bContinue = (1 == fp->Read(&count, sizeof(count), 1));
			if (bContinue) {
				assert(sizeof(gifgce) == 4);
				bContinue = (count == fp->Read(&gifgce, 1, sizeof(gifgce)));
				gifgce.delaytime = m_ntohs(gifgce.delaytime); // Avoid Byte order problem with Mac <AMSN>
				if (bContinue) {
					info.nBkgndIndex  = (gifgce.flags & 0x1) ? gifgce.transpcolindex : -1;
					info.dwFrameDelay = gifgce.delaytime;
					SetDisposalMethod((gifgce.flags >> 2) & 0x7);
		}	}	}

		if (fc == 0xFE) { //<DP> Comment block
			bContinue = (1 == fp->Read(&count, sizeof(count), 1));
			if (bContinue) {
				bContinue = (1 == fp->Read(m_comment, count, 1));
				m_comment[count]='\0';
		}	}

		if (fc == 0xFF) { //<DP> Application Extension block
			bContinue = (1 == fp->Read(&count, sizeof(count), 1));
			if (bContinue) {
				bContinue = (count==11);
				if (bContinue){
					char AppID[11];
					bContinue = (1 == fp->Read(AppID, count, 1));
					if (bContinue) {
						bContinue = (1 == fp->Read(&count, sizeof(count), 1));
						if (bContinue) {
							uint8_t* dati = (uint8_t*)malloc(count);
							bContinue = (dati!=NULL);
							if (bContinue){
								bContinue = (1 == fp->Read(dati, count, 1));
								if (count>2){
									m_loops = dati[1]+256*dati[2];
								}
							}
							free(dati);
		}	}	}	}	}

		while (bContinue && fp->Read(&count, sizeof(count), 1) && count) {
			//log << "Skipping " << count << " bytes" << endl;
			fp->Seek(count, SEEK_CUR);
		}
	}
	return bContinue;

}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////

//   - This external (machine specific) function is expected to return
// either the next uint8_t from the GIF file, or a negative error number.
int32_t CxImageGIF::get_byte(CxFile* file)
{
	if (ibf>=GIFBUFTAM){
		// FW 06/02/98 >>>
		ibfmax = (int32_t)file->Read( buf , 1 , GIFBUFTAM) ;
		if( ibfmax < GIFBUFTAM ) buf[ ibfmax ] = 255 ;
		// FW 06/02/98 <<<
		ibf = 0;
	}
	if (ibf>=ibfmax) return -1; //<DP> avoid overflows
	return buf[ibf++];
}
////////////////////////////////////////////////////////////////////////////////
/*   - This function takes a full line of pixels (one uint8_t per pixel) and
 * displays them (or does whatever your program wants with them...).  It
 * should return zero, or negative if an error or some other event occurs
 * which would require aborting the decode process...  Note that the length
 * passed will almost always be equal to the line length passed to the
 * decoder function, with the sole exception occurring when an ending code
 * occurs in an odd place in the GIF file...  In any case, linelen will be
 * equal to the number of pixels passed...
*/
int32_t CxImageGIF::out_line(CImageIterator* iter, uint8_t *pixels, int32_t linelen)
{
	if (iter == NULL || pixels == NULL)
		return -1;

	//<DP> for 1 & 4 bpp images, the pixels are compressed
	if (head.biBitCount < 8){
		for(int32_t x=0;x<head.biWidth;x++){
			uint8_t pos;
			uint8_t* iDst= pixels + (x*head.biBitCount >> 3);
			if (head.biBitCount==4){
				pos = (uint8_t)(4*(1-x%2));
				*iDst &= ~(0x0F<<pos);
				*iDst |= ((pixels[x] & 0x0F)<<pos);
			} else if (head.biBitCount==1){
				pos = (uint8_t)(7-x%8);
				*iDst &= ~(0x01<<pos);
				*iDst |= ((pixels[x] & 0x01)<<pos);
			}
		}
	}

	/* AD - for interlace */
	if (interlaced) {
		iter->SetY(iheight-iypos-1);
		iter->SetRow(pixels, linelen);

		if ((iypos += istep) >= iheight) {
			do {
				if (ipass++ > 0) istep /= 2;
				iypos = istep / 2;
			}
			while (iypos > iheight);
		}
		return 0;
	} else {
		if (iter->ItOK()) {
			iter->SetRow(pixels, linelen);
			(void)iter->PrevRow();
			return 0;
		} else {
			//	 puts("chafeo");
			return -1;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////
// SaveFile - writes GIF87a gif file
// Randy Spann 6/15/97
// R.Spann@ConnRiver.net
bool CxImageGIF::Encode(CxFile * fp)
{
	if (EncodeSafeCheck(fp)) return false;

	if(head.biBitCount > 8)	{
		//strcpy(info.szLastError,"GIF Images must be 8 bit or less");
		//return FALSE;
		return EncodeRGB(fp);
	}

	if ( GetNumFrames()>1 && ppFrames ) {
		return Encode(fp, ppFrames, GetNumFrames() );
	}

	EncodeHeader(fp);

	EncodeExtension(fp);

	EncodeComment(fp);

	EncodeBody(fp);

	fp->PutC(';'); // Write the GIF file terminator

	return true; // done!
}
////////////////////////////////////////////////////////////////////////////////
bool CxImageGIF::Encode(CxFile * fp, CxImage ** pImages, int32_t pagecount, bool bLocalColorMap, bool bLocalDispMeth)
{
  cx_try {
	if (fp==NULL) cx_throw("invalid file pointer");
	if (pImages==NULL || pagecount<=0 || pImages[0]==NULL) cx_throw("multipage GIF, no images!");

	int32_t i;
	for (i=0; i<pagecount; i++){
		if (pImages[i]==NULL)
			cx_throw("Bad image pointer");
		if (!(pImages[i]->IsValid()))
			cx_throw("Empty image");
		if (pImages[i]->GetNumColors()==0)
			cx_throw("CxImageGIF::Encode cannot create animated GIFs with a true color frame. Use DecreaseBpp before");
	}

	CxImageGIF ghost;

	//write the first image
	ghost.Ghost(pImages[0]);
	ghost.EncodeHeader(fp);

	if (m_loops!=1){
        ghost.SetLoops((std::max)(0,m_loops-1));
		ghost.EncodeLoopExtension(fp);
	}

	if (bLocalDispMeth) {
		ghost.EncodeExtension(fp);
	} else {
		uint8_t dm = ghost.GetDisposalMethod();
		ghost.SetDisposalMethod(GetDisposalMethod());
		ghost.EncodeExtension(fp);
		ghost.SetDisposalMethod(dm);
	}

	EncodeComment(fp);

	ghost.EncodeBody(fp);

	for (i=1; i<pagecount; i++){
		ghost.Ghost(pImages[i]);

		if (bLocalDispMeth) {
			ghost.EncodeExtension(fp);
		} else {
			uint8_t dm = ghost.GetDisposalMethod();
			ghost.SetDisposalMethod(GetDisposalMethod());
			ghost.EncodeExtension(fp);
			ghost.SetDisposalMethod(dm);
		}

		ghost.EncodeBody(fp,bLocalColorMap);
	}

	fp->PutC(';'); // Write the GIF file terminator

  } cx_catch {
	  if (strcmp(message,"")) strncpy(info.szLastError,message,255);
	  return false;
  }
	return true;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::EncodeHeader(CxFile *fp)
{
	fp->Write("GIF89a",1,6);	   //GIF Header

	Putword(head.biWidth,fp);			   //Logical screen descriptor
	Putword(head.biHeight,fp);

	uint8_t Flags;
	if (head.biClrUsed==0){
		Flags=0x11;
	} else {
		Flags = 0x80;
		Flags |=(head.biBitCount - 1) << 5;
		Flags |=(head.biBitCount - 1);
	}

	fp->PutC(Flags); //GIF "packed fields"
	fp->PutC(0);	 //GIF "BackGround"
	fp->PutC(0);	 //GIF "pixel aspect ratio"

	if (head.biClrUsed!=0){
		RGBQUAD* pPal = GetPalette();
		for(uint32_t i=0; i<head.biClrUsed; ++i) 
		{
			fp->PutC(pPal[i].rgbRed);
			fp->PutC(pPal[i].rgbGreen);
			fp->PutC(pPal[i].rgbBlue);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::EncodeExtension(CxFile *fp)
{
	// TRK BEGIN : transparency
	fp->PutC('!');
	fp->PutC(TRANSPARENCY_CODE);

	gifgce.flags = 0;
	gifgce.flags |= ((info.nBkgndIndex != -1) ? 1 : 0);
	gifgce.flags |= ((GetDisposalMethod() & 0x7) << 2);
	gifgce.delaytime = (uint16_t)info.dwFrameDelay;
	gifgce.transpcolindex = (uint8_t)info.nBkgndIndex;	   

	//Invert byte order in case we use a byte order arch, then set it back <AMSN>
	gifgce.delaytime = m_ntohs(gifgce.delaytime);
	fp->PutC(sizeof(gifgce));
	fp->Write(&gifgce, sizeof(gifgce), 1);
	gifgce.delaytime = m_ntohs(gifgce.delaytime);

	fp->PutC(0);
	// TRK END
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::EncodeLoopExtension(CxFile *fp)
{
	fp->PutC('!');		//byte  1  : 33 (hex 0x21) GIF Extension code
	fp->PutC(255);		//byte  2  : 255 (hex 0xFF) Application Extension Label
	fp->PutC(11);		//byte  3  : 11 (hex (0x0B) Length of Application Block (eleven bytes of data to follow)
	fp->Write("NETSCAPE2.0",11,1);
	fp->PutC(3);			//byte 15  : 3 (hex 0x03) Length of Data Sub-Block (three bytes of data to follow)
	fp->PutC(1);			//byte 16  : 1 (hex 0x01)
	Putword(m_loops,fp); //bytes 17 to 18 : 0 to 65535, an unsigned integer in lo-hi byte format. 
						//This indicate the number of iterations the loop should be executed.
	fp->PutC(0);			//bytes 19       : 0 (hex 0x00) a Data Sub-block Terminator. 
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::EncodeBody(CxFile *fp, bool bLocalColorMap)
{
	curx = 0;
	cury = head.biHeight - 1;	//because we read the image bottom to top
	CountDown = (int32_t)head.biWidth * (int32_t)head.biHeight;

	fp->PutC(',');

	Putword(info.xOffset,fp);
	Putword(info.yOffset,fp);
	Putword(head.biWidth,fp);
	Putword(head.biHeight,fp);

	uint8_t Flags=0x00; //non-interlaced (0x40 = interlaced) (0x80 = LocalColorMap)
	if (bLocalColorMap)	{ Flags|=0x80; Flags|=head.biBitCount-1; }
	fp->PutC(Flags);

	if (bLocalColorMap){
		Flags|=0x87;
		RGBQUAD* pPal = GetPalette();
		for(uint32_t i=0; i<head.biClrUsed; ++i) 
		{
			fp->PutC(pPal[i].rgbRed);
			fp->PutC(pPal[i].rgbGreen);
			fp->PutC(pPal[i].rgbBlue);
		}
	}

	int32_t InitCodeSize = head.biBitCount <=1 ? 2 : head.biBitCount;
	 // Write out the initial code size
	fp->PutC((uint8_t)InitCodeSize);

	 // Go and actually compress the data
	switch (GetCodecOption(CXIMAGE_FORMAT_GIF))
	{
	case 1:	//uncompressed
		compressNONE(InitCodeSize+1, fp);
		break;
	case 2: //RLE
		compressRLE(InitCodeSize+1, fp);
		break;
	default: //LZW
		compressLZW(InitCodeSize+1, fp);
	}

	 // Write out a Zero-length packet (to end the series)
	fp->PutC(0);
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::EncodeComment(CxFile *fp)
{
	uint32_t n = (uint32_t) strlen(m_comment);
	if (n>255) n=255;
	if (n) {
		fp->PutC('!');	//extension code:
		fp->PutC(254);	//comment extension
		fp->PutC((uint8_t)n);	//size of comment
		fp->Write(m_comment,n,1);
		fp->PutC(0);	//block terminator
	}
}
////////////////////////////////////////////////////////////////////////////////
bool CxImageGIF::EncodeRGB(CxFile *fp)
{
	EncodeHeader(fp);

//	EncodeLoopExtension(fp);

	EncodeComment(fp);

#ifndef USE_RGB_PALETTE
	uint32_t w,h;
	w=h=0;
    int32_t cellw = 17;
    int32_t cellh = 15;
	CxImageGIF tmp;
	for (int32_t y=0;y<head.biHeight;y+=cellh){
		for (int32_t x=0;x<head.biWidth;x+=cellw){
			if ((head.biWidth -x)<cellw) w=head.biWidth -x; else w=cellw;
			if ((head.biHeight-y)<cellh) h=head.biHeight-y; else h=cellh;

			if (w!=tmp.GetWidth() || h!=tmp.GetHeight()) tmp.Create(w,h,8);

			if (IsTransparent()){
				tmp.SetTransIndex(0);
				tmp.SetPaletteColor(0,GetTransColor());
			}

			uint8_t i;
			for (uint32_t j=0;j<h;j++){
				for (uint32_t k=0;k<w;k++){
					i=(uint8_t)(1+k+cellw*j);
					tmp.SetPaletteColor(i,GetPixelColor(x+k,head.biHeight-y-h+j));
					tmp.SetPixelIndex(k,j,tmp.GetNearestIndex(tmp.GetPaletteColor(i)));
				}
			}

			tmp.SetOffset(x,y);
			tmp.EncodeExtension(fp);
			tmp.EncodeBody(fp,true);
		}
	}
#else
    CxImageGIF tmp;
    tmp.Create(head.biWidth, head.biHeight, 8);
    tmp.SetTransIndex(0);

    NSGeneratePalette::CImage32bit_BGRA srcBgra;
    srcBgra.m_lWidth = head.biWidth;
    srcBgra.m_lHeight = head.biHeight;
    srcBgra.m_pPixels = new unsigned char[4 * srcBgra.m_lWidth * srcBgra.m_lHeight];
    unsigned char* pixelsTmp = srcBgra.m_pPixels;

    for (uint32_t j=0;j<head.biHeight;j++)
    {
        for (uint32_t k=0;k<head.biWidth;k++)
        {
            RGBQUAD c = GetPixelColor(k,j);
            *pixelsTmp++ = c.rgbBlue;
            *pixelsTmp++ = c.rgbGreen;
            *pixelsTmp++ = c.rgbRed;
            *pixelsTmp++ = c.rgbReserved;
        }
    }

    NSGeneratePalette::CPaletteCreator oCreator;
    std::vector<NSGeneratePalette::CImage32bit_BGRA> imagesSrc;
    imagesSrc.push_back(srcBgra);

    unsigned int* pPalette = NULL;
    std::vector<NSGeneratePalette::CImage8bit*> imagesDst = oCreator.Convert(imagesSrc, &pPalette);

    NSGeneratePalette::CImage8bit* pImage8 = imagesDst[0];

    for (int i = 0; i < 256; i++)
    {
        unsigned int c = pPalette[i];
        RGBQUAD cRGB;
        //cRGB.rgbRed = (c >> 8) & 0xFF;
        //cRGB.rgbGreen = (c >> 16) & 0xFF;
        //cRGB.rgbBlue = (c >> 24) & 0xFF;
        //cRGB.rgbReserved = c & 0xFF;
        cRGB.rgbRed = (c >> 16) & 0xFF;
        cRGB.rgbGreen = (c >> 8) & 0xFF;
        cRGB.rgbReserved = (c >> 24) & 0xFF;
        cRGB.rgbBlue = c & 0xFF;
        tmp.SetPaletteColor(i, cRGB);
    }

    unsigned char* pImage8Tmp = pImage8->m_pPixels;
    for (uint32_t j=0;j<head.biHeight;j++)
    {
        for (uint32_t k=0;k<head.biWidth;k++)
        {
            tmp.SetPixelIndex(k, j, *pImage8Tmp++);
        }
    }

    delete[] srcBgra.m_pPixels;
    delete[] pPalette;

    delete pImage8;

    tmp.SetOffset(0,0);
    tmp.EncodeExtension(fp);
    tmp.EncodeBody(fp,true);
#endif

	fp->PutC(';'); // Write the GIF file terminator

	return true; // done!
}
////////////////////////////////////////////////////////////////////////////////
#endif // CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////
// Return the next pixel from the image
// <DP> fix for 1 & 4 bpp images
int32_t CxImageGIF::GifNextPixel( )
{
	if( CountDown == 0 ) return EOF;
	--CountDown;
	int32_t r = GetPixelIndex(curx,cury);
	// Bump the current X position
	++curx;
	if( curx == head.biWidth ){
		curx = 0;
		cury--;	             //bottom to top
	}
	return r;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::Putword(int32_t w, CxFile *fp )
{
	fp->PutC((uint8_t)(w & 0xff));
	fp->PutC((uint8_t)((w >> 8) & 0xff));
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::compressNONE( int32_t init_bits, CxFile* outfile)
{
	register int32_t c;
	register int32_t ent;

	// g_init_bits - initial number of bits
	// g_outfile   - pointer to output file
	g_init_bits = init_bits;
	g_outfile = outfile;

	 // Set up the necessary values
	cur_accum = cur_bits = clear_flg = 0;
	maxcode = (int16_t)MAXCODE(n_bits = g_init_bits);
	code_int maxmaxcode = (code_int)1 << MAXBITSCODES;

	ClearCode = (1 << (init_bits - 1));
	EOFCode = ClearCode + 1;
	free_ent = (int16_t)(ClearCode + 2);

	a_count=0;
	ent = GifNextPixel( );

	output( (code_int)ClearCode );

	while ( ent != EOF ) {    
		c = GifNextPixel();

		output ( (code_int) ent );
		ent = c;
		if ( free_ent < maxmaxcode ) {  
			free_ent++;
		} else {
			free_ent=(int16_t)(ClearCode+2);
			clear_flg=1;
			output((code_int)ClearCode);
		}
	}
	 // Put out the final code.
	output( (code_int) EOFCode );
}
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************
 *
 *  GIFCOMPR.C       -     LZW GIF Image compression routines
 *
 ***************************************************************************/

void CxImageGIF::compressLZW( int32_t init_bits, CxFile* outfile)
{
	register int32_t fcode;
	register int32_t c;
	register int32_t ent;
	register int32_t hshift;
	register int32_t disp;
	register int32_t i;

	// g_init_bits - initial number of bits
	// g_outfile   - pointer to output file
	g_init_bits = init_bits;
	g_outfile = outfile;

	 // Set up the necessary values
	cur_accum = cur_bits = clear_flg = 0;
	maxcode = (int16_t)MAXCODE(n_bits = g_init_bits);
	code_int maxmaxcode = (code_int)1 << MAXBITSCODES;

	ClearCode = (1 << (init_bits - 1));
	EOFCode = ClearCode + 1;
	free_ent = (int16_t)(ClearCode + 2);

	a_count=0;
	ent = GifNextPixel( );

	hshift = 0;
	for ( fcode = (int32_t) HSIZE;  fcode < 65536L; fcode *= 2L )	++hshift;
	hshift = 8 - hshift;                /* set hash code range bound */
	cl_hash((int32_t)HSIZE);        /* clear hash table */
	output( (code_int)ClearCode );

	while ( (c = GifNextPixel( )) != EOF ) {    

		fcode = (int32_t) (((int32_t) c << MAXBITSCODES) + ent);
		i = (((code_int)c << hshift) ^ ent);    /* xor hashing */

		if ( HashTabOf (i) == fcode ) {
			ent = CodeTabOf (i);
			continue;
		} else if ( (int32_t)HashTabOf (i) < 0 )      /* empty slot */
			goto nomatch;
		disp = HSIZE - i;           /* secondary hash (after G. Knott) */
		if ( i == 0 )	disp = 1;
probe:
		if ( (i -= disp) < 0 )	i += HSIZE;
		if ( HashTabOf (i) == fcode ) {	ent = CodeTabOf (i); continue; }
		if ( (int32_t)HashTabOf (i) > 0 )	goto probe;
nomatch:
		output ( (code_int) ent );
		ent = c;
		if ( free_ent < maxmaxcode ) {  
			CodeTabOf (i) = free_ent++; /* code -> hashtable */
			HashTabOf (i) = fcode;
		} else {
			cl_hash((int32_t)HSIZE);
			free_ent=(int16_t)(ClearCode+2);
			clear_flg=1;
			output((code_int)ClearCode);
		}
	}
	 // Put out the final code.
	output( (code_int)ent );
	output( (code_int) EOFCode );
}
////////////////////////////////////////////////////////////////////////////////

static const uint32_t code_mask[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
								  0x001F, 0x003F, 0x007F, 0x00FF,
								  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
								  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };

////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::output( code_int  code)
{
	cur_accum &= code_mask[ cur_bits ];

	if( cur_bits > 0 )
		cur_accum |= ((int32_t)code << cur_bits);
	else
		cur_accum = code;

	cur_bits += n_bits;

	while( cur_bits >= 8 ) {
		char_out( (uint32_t)(cur_accum & 0xff) );
		cur_accum >>= 8;
		cur_bits -= 8;
	}

	/*
	 * If the next entry is going to be too big for the code size,
	 * then increase it, if possible.
	 */

	if ( free_ent > maxcode || clear_flg ) {
		if( clear_flg ) {
			maxcode = (int16_t)MAXCODE(n_bits = g_init_bits);
			clear_flg = 0;
		} else {
			++n_bits;
			if ( n_bits == MAXBITSCODES )
				maxcode = (code_int)1 << MAXBITSCODES; /* should NEVER generate this code */
			else
				maxcode = (int16_t)MAXCODE(n_bits);
		}
	}
	
	if( code == EOFCode ) {
		 // At EOF, write the rest of the buffer.
		while( cur_bits > 0 ) {
			char_out( (uint32_t)(cur_accum & 0xff) );
			cur_accum >>= 8;
			cur_bits -= 8;
		}
	
		flush_char();
		g_outfile->Flush();

		if(g_outfile->Error()) strcpy(info.szLastError,"Write Error in GIF file");
	}
}
////////////////////////////////////////////////////////////////////////////////

void CxImageGIF::cl_hash(int32_t hsize)

{
	register int32_t *htab_p = htab+hsize;

	register int32_t i;
	register int32_t m1 = -1L;

	i = hsize - 16;

	do {
		*(htab_p-16)=m1;
		*(htab_p-15)=m1;
		*(htab_p-14)=m1;
		*(htab_p-13)=m1;
		*(htab_p-12)=m1;
		*(htab_p-11)=m1;
		*(htab_p-10)=m1;
		*(htab_p-9)=m1;
		*(htab_p-8)=m1;
		*(htab_p-7)=m1;
		*(htab_p-6)=m1;
		*(htab_p-5)=m1;
		*(htab_p-4)=m1;
		*(htab_p-3)=m1;
		*(htab_p-2)=m1;
		*(htab_p-1)=m1;
		
		htab_p-=16;
	} while ((i-=16) >=0);

	for (i+=16;i>0;--i)
		*--htab_p=m1;
}

/*******************************************************************************
*   GIF specific
*******************************************************************************/

void CxImageGIF::char_out(int32_t c)
{
	accum[a_count++]=(char)c;
	if (a_count >=254)
		flush_char();
}

void CxImageGIF::flush_char()
{
	if (a_count > 0) {
		g_outfile->PutC((uint8_t)a_count);
		g_outfile->Write(accum,1,a_count);
		a_count=0;
	}
}

/*******************************************************************************
*   GIF decoder
*******************************************************************************/
/* DECODE.C - An LZW decoder for GIF
 * Copyright (C) 1987, by Steven A. Bennett
 * Copyright (C) 1994, C++ version by Alejandro Aguilar Sierra
*
 * Permission is given by the author to freely redistribute and include
 * this code in any program as int32_t as this credit is given where due.
 *
 * In accordance with the above, I want to credit Steve Wilhite who wrote
 * the code which this is heavily inspired by...
 *
 * GIF and 'Graphics Interchange Format' are trademarks (tm) of
 * Compuserve, Incorporated, an H&R Block Company.
 *
 * Release Notes: This file contains a decoder routine for GIF images
 * which is similar, structurally, to the original routine by Steve Wilhite.
 * It is, however, somewhat noticably faster in most cases.
 *
 */

#if CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////

int16_t CxImageGIF::init_exp(int16_t size)
{
	curr_size = (int16_t)(size + 1);
	top_slot = (int16_t)(1 << curr_size);
	clear = (int16_t)(1 << size);
	ending = (int16_t)(clear + 1);
	slot = newcodes = (int16_t)(ending + 1);
	navail_bytes = nbits_left = 0;

	memset(stack,0,MAX_CODES + 1);
	memset(prefix,0,MAX_CODES + 1);
	memset(suffix,0,MAX_CODES + 1);
	return(0);
}
////////////////////////////////////////////////////////////////////////////////

/* get_next_code()
 * - gets the next code from the GIF file.  Returns the code, or else
 * a negative number in case of file errors...
 */
int16_t CxImageGIF::get_next_code(CxFile* file)
{
	int16_t i, x;
	uint32_t ret;

	if (nbits_left == 0) {
		if (navail_bytes <= 0) {
			/* Out of bytes in current block, so read next block */
			pbytes = byte_buff;
			if ((navail_bytes = (int16_t)get_byte(file)) < 0)
				return(navail_bytes);
			else if (navail_bytes) {
				for (i = 0; i < navail_bytes; ++i) {
					if ((x = (int16_t)get_byte(file)) < 0) return(x);
					byte_buff[i] = (uint8_t)x;
				}
			}
		}
		b1 = *pbytes++;
		nbits_left = 8;
		--navail_bytes;
	}

	if (navail_bytes<0) return ending; // prevent deadlocks (thanks to Mike Melnikov)

	ret = b1 >> (8 - nbits_left);
	while (curr_size > nbits_left){
		if (navail_bytes <= 0){
			/* Out of bytes in current block, so read next block*/
			pbytes = byte_buff;
			if ((navail_bytes = (int16_t)get_byte(file)) < 0)
				return(navail_bytes);
			else if (navail_bytes){
				for (i = 0; i < navail_bytes; ++i){
					if ((x = (int16_t)get_byte(file)) < 0) return(x);
					byte_buff[i] = (uint8_t)x;
				}
			}
		}
		b1 = *pbytes++;
		ret |= b1 << nbits_left;
		nbits_left += 8;
		--navail_bytes;
	}
	nbits_left = (int16_t)(nbits_left-curr_size);
	ret &= code_mask[curr_size];
	return((int16_t)(ret));
}
////////////////////////////////////////////////////////////////////////////////

/* int16_t decoder(linewidth)
 *    int16_t linewidth;               * Pixels per line of image *
 *
 * - This function decodes an LZW image, according to the method used
 * in the GIF spec.  Every *linewidth* "characters" (ie. pixels) decoded
 * will generate a call to out_line(), which is a user specific function
 * to display a line of pixels.  The function gets it's codes from
 * get_next_code() which is responsible for reading blocks of data and
 * seperating them into the proper size codes.  Finally, get_byte() is
 * the global routine to read the next uint8_t from the GIF file.
 *
 * It is generally a good idea to have linewidth correspond to the actual
 * width of a line (as specified in the Image header) to make your own
 * code a bit simpler, but it isn't absolutely necessary.
 *
 * Returns: 0 if successful, else negative.  (See ERRS.H)
 *
 */
/* bad_code_count is incremented each time an out of range code is read.
 * When this value is non-zero after a decode, your GIF file is probably
 * corrupt in some way...
 */
int16_t CxImageGIF::decoder(CxFile* file, CImageIterator* iter, int16_t linewidth, int32_t &bad_code_count)
{
	register uint8_t *sp, *bufptr;
	uint8_t *buf;
	register int16_t code, fc, oc, bufcnt;
	int16_t c, size, ret;

	if (linewidth<=0)
		return BAD_LINE_WIDTH;

	/* Initialize for decoding a new image... */
	bad_code_count = 0;
	if ((size = (int16_t)get_byte(file)) < 0)	return(size);
	if (size < 2 || 9 < size)				return(BAD_CODE_SIZE);
	// out_line = outline;
	init_exp(size);
	//printf("L %d %x\n",linewidth,size);

	/* Initialize in case they forgot to put in a clear code.
	 * (This shouldn't happen, but we'll try and decode it anyway...)
	 */
	oc = fc = 0;

   /* Allocate space for the decode buffer */
	if ((buf = new uint8_t[linewidth + 1]) == NULL) return(OUT_OF_MEMORY);

   /* Set up the stack pointer and decode buffer pointer */
	sp = stack;
	bufptr = buf;
	bufcnt = linewidth;

   /* This is the main loop.  For each code we get we pass through the
	* linked list of prefix codes, pushing the corresponding "character" for
	* each code onto the stack.  When the list reaches a single "character"
	* we push that on the stack too, and then start unstacking each
    * character for output in the correct order.  Special handling is
	* included for the clear code, and the whole thing ends when we get
    * an ending code.
    */
	while ((c = get_next_code(file)) != ending) {
		/* If we had a file error, return without completing the decode*/
		if (c < 0){
			delete [] buf;
			return(0);
		}
		/* If the code is a clear code, reinitialize all necessary items.*/
		if (c == clear){
			curr_size = (int16_t)(size + 1);
			slot = newcodes;
			top_slot = (int16_t)(1 << curr_size);

			/* Continue reading codes until we get a non-clear code
			* (Another unlikely, but possible case...)
			*/
			while ((c = get_next_code(file)) == clear);

			/* If we get an ending code immediately after a clear code
			* (Yet another unlikely case), then break out of the loop.
			*/
			if (c == ending) break;

			/* Finally, if the code is beyond the range of already set codes,
			* (This one had better NOT happen...  I have no idea what will
			* result from this, but I doubt it will look good...) then set it
			* to color zero.
			*/
			if (c >= slot) c = 0;
			oc = fc = c;

			/* And let us not forget to put the char into the buffer... And
			* if, on the off chance, we were exactly one pixel from the end
			* of the line, we have to send the buffer to the out_line()
			* routine...
			*/
			*bufptr++ = (uint8_t)c;
			if (--bufcnt == 0) {
				if (iter) {
					if ((ret = (int16_t)out_line(iter, buf, linewidth)) < 0) {
						delete [] buf;
						return(ret);
					}
				}
				bufptr = buf;
				bufcnt = linewidth;
            }
		} else {
			/* In this case, it's not a clear code or an ending code, so
			* it must be a code code...  So we can now decode the code into
			* a stack of character codes. (Clear as mud, right?)
			*/
			code = c;

			/* Here we go again with one of those off chances...  If, on the
			* off chance, the code we got is beyond the range of those already
			* set up (Another thing which had better NOT happen...) we trick
			* the decoder into thinking it actually got the last code read.
			* (Hmmn... I'm not sure why this works...  But it does...)
			*/
			if (code >= slot && sp<(stack+MAX_CODES-1)) {
				if (code > slot)
					++bad_code_count;
				code = oc;
				*sp++ = (uint8_t)fc;
            }

			/* Here we scan back along the linked list of prefixes, pushing
			* helpless characters (ie. suffixes) onto the stack as we do so.
			*/
			while (code >= newcodes && sp<(stack+MAX_CODES-1)) {
				*sp++ = suffix[code];
				code = prefix[code];
            }

			/* Push the last character on the stack, and set up the new
			* prefix and suffix, and if the required slot number is greater
			* than that allowed by the current bit size, increase the bit
			* size.  (NOTE - If we are all full, we *don't* save the new
			* suffix and prefix...  I'm not certain if this is correct...
			* it might be more proper to overwrite the last code...
			*/
			*sp++ = (uint8_t)code;
			if (slot < top_slot){
				suffix[slot] = (uint8_t)(fc = (uint8_t)code);
				prefix[slot++] = oc;
				oc = c;
            }
			if (slot >= top_slot){
				if (curr_size < 12) {
					top_slot <<= 1;
					++curr_size;
				}
			}

			/* Now that we've pushed the decoded string (in reverse order)
			* onto the stack, lets pop it off and put it into our decode
			* buffer...  And when the decode buffer is full, write another
			* line...
			*/
			while (sp > stack) {
				*bufptr++ = *(--sp);
				if (--bufcnt == 0) {
					if (iter) {
						if ((ret = (int16_t)out_line(iter, buf, linewidth)) < 0) {
							delete [] buf;
							return(ret);
						}
					}
					bufptr = buf;
					bufcnt = linewidth;
				}
			}
		}
	}
	ret = 0;
	if (bufcnt != linewidth && iter)
		ret = (int16_t)out_line(iter, buf, (linewidth - bufcnt));
	delete [] buf;
	return(ret);
}
////////////////////////////////////////////////////////////////////////////////
int32_t CxImageGIF::get_num_frames(CxFile *fp,struct_TabCol* TabColSrc,struct_dscgif* dscgif)
{
	struct_image image;

	int32_t pos=fp->Tell();
	int32_t nframes=0;

	struct_TabCol TempTabCol;
	memcpy(&TempTabCol,TabColSrc,sizeof(struct_TabCol));

	char ch;
	bool bPreviousWasNull = true;

	for (BOOL bContinue = TRUE; bContinue; )
	{
		if (fp->Read(&ch, sizeof(ch), 1) != 1) {break;}

		if (bPreviousWasNull || ch==0)
		{
			switch (ch)
			{
			case '!': // extension
				{
				DecodeExtension(fp);
				break;
				}
			case ',': // image
				{

				assert(sizeof(image) == 9);
				//log << "Image header" << endl;
				fp->Read(&image,sizeof(image),1);

				//avoid byte order problems with Solaris <candan> <AMSN>
				image.l = m_ntohs(image.l);
				image.t = m_ntohs(image.t);
				image.w = m_ntohs(image.w);
				image.h = m_ntohs(image.h);

				// in case of images with empty screen descriptor, give a last chance
				if (dscgif->scrwidth==0 && dscgif->scrheight==0){
					dscgif->scrwidth = image.w;
					dscgif->scrheight = image.h;
				}

				if (((image.l + image.w) > dscgif->scrwidth)||((image.t + image.h) > dscgif->scrheight))
					break;

				nframes++;

				// Local colour map?
				if (image.pf & 0x80) {
					TempTabCol.sogct = (int16_t)(1 << ((image.pf & 0x07) +1));
					assert(3 == sizeof(struct rgb_color));
					fp->Read(TempTabCol.paleta,sizeof(struct rgb_color)*TempTabCol.sogct,1);
					//log << "Local colour map" << endl;
				}

				int32_t badcode=0;
				ibf = GIFBUFTAM+1;

				interlaced = image.pf & 0x40;
				iheight = image.h;
				istep = 8;
				iypos = 0;
				ipass = 0;

				int32_t pos_start = fp->Tell();

				//if (interlaced) log << "Interlaced" << endl;
				decoder(fp, 0, image.w, badcode);

				if (badcode){
					seek_next_image(fp,pos_start);
				} else {
					fp->Seek(-(ibfmax - ibf - 1), SEEK_CUR);
				}
		
				break;
				}
			case ';': //terminator
				bContinue=false;
				break;
			default:
				bPreviousWasNull = (ch==0);
				break;
			}
		}
	}

	fp->Seek(pos,SEEK_SET);
	return nframes;
}
////////////////////////////////////////////////////////////////////////////////
int32_t CxImageGIF::seek_next_image(CxFile* fp, int32_t position)
{
	fp->Seek(position, SEEK_SET);
	char ch1,ch2;
	ch1=ch2=0;
	while(fp->Read(&ch2,sizeof(char),1)>0){
		if (ch1 == 0 && ch2 == ','){
			fp->Seek(-1,SEEK_CUR);
			return fp->Tell();
		} else {
			ch1 = ch2;
		}
	}
	return -1;
}
#endif //CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::SetLoops(int32_t loops)
{	m_loops=loops; }
////////////////////////////////////////////////////////////////////////////////
int32_t CxImageGIF::GetLoops()
{	return m_loops; }
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::SetComment(const char* sz_comment_in)
{	if (sz_comment_in) strncpy(m_comment,sz_comment_in,255); }
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::GetComment(char* sz_comment_out)
{	if (sz_comment_out) strncpy(sz_comment_out,m_comment,255); }
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::GifMix(CxImage & imgsrc2, struct_image & imgdesc)
{
    int32_t ymin = (std::max)(0,(int32_t)(GetHeight()-imgdesc.t - imgdesc.h));
	int32_t ymax = GetHeight()-imgdesc.t;
	int32_t xmin = imgdesc.l;
    int32_t xmax = (std::min)(GetWidth(), (uint32_t)(imgdesc.l + imgdesc.w));

	int32_t ibg2= imgsrc2.GetTransIndex();
    uint8_t i2;

	for(int32_t y = ymin; y < ymax; y++){
		for(int32_t x = xmin; x < xmax; x++){
			i2 = imgsrc2.GetPixelIndex(x-xmin,y-ymin);
			if(i2!=ibg2) SetPixelIndex(x,y,i2);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------
 *
 * miGIF Compression - mouse and ivo's GIF-compatible compression
 *
 *          -run length encoding compression routines-
 *
 * Copyright (C) 1998 Hutchison Avenue Software Corporation
 *               http://www.hasc.com
 *               info@hasc.com
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  This software is provided "AS IS." The Hutchison Avenue 
 * Software Corporation disclaims all warranties, either express or implied, 
 * including but not limited to implied warranties of merchantability and 
 * fitness for a particular purpose, with respect to this code and accompanying
 * documentation. 
 * 
 * The miGIF compression routines do not, strictly speaking, generate files 
 * conforming to the GIF spec, since the image data is not LZW-compressed 
 * (this is the point: in order to avoid transgression of the Unisys patent 
 * on the LZW algorithm.)  However, miGIF generates data streams that any 
 * reasonably sane LZW decompresser will decompress to what we want.
 *
 * miGIF compression uses run length encoding. It compresses horizontal runs 
 * of pixels of the same color. This type of compression gives good results
 * on images with many runs, for example images with lines, text and solid 
 * shapes on a solid-colored background. It gives little or no compression 
 * on images with few runs, for example digital or scanned photos.
 *
 *                               der Mouse
 *                      mouse@rodents.montreal.qc.ca
 *            7D C8 61 52 5D E7 2D 39  4E F1 31 3E E8 B3 27 4B
 *
 *                             ivo@hasc.com
 *
 * The Graphics Interchange Format(c) is the Copyright property of
 * CompuServe Incorporated.  GIF(sm) is a Service Mark property of
 * CompuServe Incorporated.
 *
 */
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_clear(struct_RLE* rle)
{
	rle->out_bits = rle->out_bits_init;
	rle->out_bump = rle->out_bump_init;
	rle->out_clear = rle->out_clear_init;
	rle->out_count = 0;
	rle->rl_table_max = 0;
	rle->just_cleared = 1;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_flush(struct_RLE* rle)
{
	if (rle->rl_count == 1){
		rle_output_plain(rle->rl_pixel,rle);
		rle->rl_count = 0;
		return;
	}
	if (rle->just_cleared){
		rle_flush_fromclear(rle->rl_count,rle);
	} else if ((rle->rl_table_max < 2) || (rle->rl_table_pixel != rle->rl_pixel)) {
		rle_flush_clearorrep(rle->rl_count,rle);
	} else {
		rle_flush_withtable(rle->rl_count,rle);
	}
	rle->rl_count = 0;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_output_plain(int32_t c,struct_RLE* rle)
{
	rle->just_cleared = 0;
	rle_output(c,rle);
	rle->out_count++;
	if (rle->out_count >= rle->out_bump){
		rle->out_bits ++;
		rle->out_bump += 1 << (rle->out_bits - 1);
	}
	if (rle->out_count >= rle->out_clear){
		rle_output(rle->code_clear,rle);
		rle_clear(rle);
	}
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_flush_fromclear(int32_t count,struct_RLE* rle)
{
	int32_t n;

	rle->out_clear = rle->max_ocodes;
	rle->rl_table_pixel = rle->rl_pixel;
	n = 1;
	while (count > 0){
		if (n == 1){
			rle->rl_table_max = 1;
			rle_output_plain(rle->rl_pixel,rle);
			count --;
		} else if (count >= n){
			rle->rl_table_max = n;
			rle_output_plain(rle->rl_basecode+n-2,rle);
			count -= n;
		} else if (count == 1){
			rle->rl_table_max ++;
			rle_output_plain(rle->rl_pixel,rle);
			count = 0;
		} else {
			rle->rl_table_max ++;
			rle_output_plain(rle->rl_basecode+count-2,rle);
			count = 0;
		}
		if (rle->out_count == 0) n = 1; else n ++;
	}
	rle_reset_out_clear(rle);
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_reset_out_clear(struct_RLE* rle)
{
	rle->out_clear = rle->out_clear_init;
	if (rle->out_count >= rle->out_clear){
		rle_output(rle->code_clear,rle);
		rle_clear(rle);
	}
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_flush_withtable(int32_t count, struct_RLE* rle)
{
	int32_t repmax;
	int32_t repleft;
	int32_t leftover;

	repmax = count / rle->rl_table_max;
	leftover = count % rle->rl_table_max;
	repleft = (leftover ? 1 : 0);
	if (rle->out_count+repmax+repleft > rle->max_ocodes){
		repmax = rle->max_ocodes - rle->out_count;
		leftover = count - (repmax * rle->rl_table_max);
		repleft = 1 + rle_compute_triangle_count(leftover,rle->max_ocodes);
	}
	if (1+rle_compute_triangle_count(count,rle->max_ocodes) < (uint32_t)(repmax+repleft)){
		rle_output(rle->code_clear,rle);
		rle_clear(rle);
		rle_flush_fromclear(count,rle);
		return;
	}
	rle->out_clear = rle->max_ocodes;
	for (;repmax>0;repmax--) rle_output_plain(rle->rl_basecode+rle->rl_table_max-2,rle);
	if (leftover){
		if (rle->just_cleared){
			rle_flush_fromclear(leftover,rle);
		} else if (leftover == 1){
			rle_output_plain(rle->rl_pixel,rle);
		} else {
			rle_output_plain(rle->rl_basecode+leftover-2,rle);
		}
	}
	rle_reset_out_clear(rle);
}
////////////////////////////////////////////////////////////////////////////////
uint32_t CxImageGIF::rle_compute_triangle_count(uint32_t count, uint32_t nrepcodes)
{
	uint32_t perrep;
	uint32_t cost;

	cost = 0;
	perrep = (nrepcodes * (nrepcodes+1)) / 2;
	while (count >= perrep){
		cost += nrepcodes;
		count -= perrep;
	}
	if (count > 0){
		uint32_t n;
		n = rle_isqrt(count);
		while ((n*(n+1)) >= 2*count) n --;
		while ((n*(n+1)) < 2*count) n ++;
		cost += n;
	}
	return(cost);
}
////////////////////////////////////////////////////////////////////////////////
uint32_t CxImageGIF::rle_isqrt(uint32_t x)
{
	uint32_t r;
	uint32_t v;

	if (x < 2) return(x);
	for (v=x,r=1;v;v>>=2,r<<=1) ;
	for( ;; )
	{
		v = ((x / r) + r) / 2;
		if ((v == r) || (v == r+1)) return(r);
		r = v;
	}
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_flush_clearorrep(int32_t count, struct_RLE* rle)
{
	int32_t withclr;
	withclr = 1 + rle_compute_triangle_count(count,rle->max_ocodes);
	if (withclr < count) {
		rle_output(rle->code_clear,rle);
		rle_clear(rle);
		rle_flush_fromclear(count,rle);
	} else {
		for (;count>0;count--) rle_output_plain(rle->rl_pixel,rle);
	}
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_write_block(struct_RLE* rle)
{
	g_outfile->PutC((uint8_t)rle->oblen);
	g_outfile->Write(rle->oblock,1,rle->oblen);
	rle->oblen = 0;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_block_out(uint8_t c, struct_RLE* rle)
{
	rle->oblock[rle->oblen++] = c;
	if (rle->oblen >= 255) rle_write_block(rle);
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_block_flush(struct_RLE* rle)
{
	if (rle->oblen > 0) rle_write_block(rle);
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_output(int32_t val, struct_RLE* rle)
{
	rle->obuf |= val << rle->obits;
	rle->obits += rle->out_bits;
	while (rle->obits >= 8){
		rle_block_out((uint8_t)(rle->obuf&0xff),rle);
		rle->obuf >>= 8;
		rle->obits -= 8;
	}
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::rle_output_flush(struct_RLE* rle)
{
	 if (rle->obits > 0) rle_block_out((uint8_t)(rle->obuf),rle);
	 rle_block_flush(rle);
}
////////////////////////////////////////////////////////////////////////////////
void CxImageGIF::compressRLE( int32_t init_bits, CxFile* outfile)
{
	g_init_bits = init_bits;
	g_outfile = outfile;

	struct_RLE rle;
	rle.code_clear = 1 << (init_bits - 1);
	rle.code_eof = rle.code_clear + 1;
	rle.rl_basecode = rle.code_eof + 1;
	rle.out_bump_init = (1 << (init_bits - 1)) - 1;
	rle.out_clear_init = (init_bits <= 3) ? 9 : (rle.out_bump_init-1);
	rle.out_bits_init = init_bits;
	rle.max_ocodes = (1 << MAXBITSCODES) - ((1 << (rle.out_bits_init - 1)) + 3);
	rle.rl_count = 0;
	rle_clear(&rle);
	rle.obuf = 0;
	rle.obits = 0;
	rle.oblen = 0;

	rle_output(rle.code_clear,&rle);

	int32_t c;
	for( ;; )
	{
		c = GifNextPixel();
		if ((rle.rl_count > 0) && (c != rle.rl_pixel)) rle_flush(&rle);
		if (c == EOF) break;
		if (rle.rl_pixel == c){
			rle.rl_count++;
		} else {
			rle.rl_pixel = c;
			rle.rl_count = 1;
		}
	}
	rle_output(rle.code_eof,&rle);
	rle_output_flush(&rle);
}
////////////////////////////////////////////////////////////////////////////////
#endif // CXIMAGE_SUPPORT_GIF
