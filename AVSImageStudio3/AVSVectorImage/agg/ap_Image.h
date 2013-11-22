// Image.h: interface for the Image class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_IMAGE_H__4DF00F5C_76B7_431D_8849_6334D98F4477__INCLUDED_)
#define Aggplus_IMAGE_H__4DF00F5C_76B7_431D_8849_6334D98F4477__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ap_AggPlusEnums.h"
#include "ap_AggPlusTypes.h"


#include "svg/agg_svg_exception.h"
#include "svg/agg_svg_parser.h"
#include "svg/agg_svg_indexation_interpreter.h"
#include "svg/agg_svg_rendering_interpreter.h"
#include "svg/agg_svg_attributes_map.h"
#include "svg/agg_svg_rasterizer.h"


namespace Aggplus
{
		class Image  
		{
		public:
			friend class Graphics;
			friend class BrushTexture;
		protected:
			Image();
		public:
			Image(const WCHAR *filename, BOOL useEmbeddedColorManagement = false);
			virtual ~Image();
			Image *Clone() const;

			inline DWORD GetWidth() const { return(m_dwWidth); }
			inline DWORD GetHeight() const { return(m_dwHeight); }
			 
			inline Status GetLastStatus() const { return(m_Status); }

		//Image *GetThumbnailImage(UINT thumbWidth, UINT thumbHeight, GetThumbnailImageAbort callback, void* callbackData);
		protected:
			Status m_Status;
			DWORD m_dwWidth, m_dwHeight;
			int m_nStride;

			void *m_pImgData;
			bool m_bExternalBuffer;
			 
			void Create(const WCHAR *filename);
			void CopyFrom(const Image *pSource);
			void Destroy();
		};

		//---------------------------------------------------------------------------
		// Information about image pixel data
		//---------------------------------------------------------------------------

		class BitmapData
		{
		public:
			UINT Width;
			UINT Height;
			INT Stride;
			PixelFormat PixelFormat;
			void* Scan0;
			UINT_PTR Reserved;
		};

		class Bitmap : public Image
		{
		public:
			Bitmap(INT width, INT height, PixelFormat format=PixelFormat32bppARGB);
			Bitmap(INT width, INT height, INT stride, PixelFormat format, BYTE* scan0);
			 

			Bitmap(HBITMAP hbm, HPALETTE hpal);
			Bitmap(HINSTANCE hInstance, const WCHAR* bitmapName);
			Bitmap(const WCHAR *filename, BOOL useIcm = false);
			~Bitmap();

			static inline Bitmap* FromHBITMAP(HBITMAP hbm, HPALETTE hpal) { return new Bitmap(hbm, hpal); }

			inline void *GetBitmapBits() { assert(m_Status==Ok); return(m_pImgData); }
		protected:
			Bitmap() : Image(NULL) { assert(FALSE); }
			void Create(HBITMAP hbm, HPALETTE hpal);


		};

		class SVGImage
		{
			friend class Graphics;

			agg::svg::dom_storage             m_storage;
			agg::svg::dom_storage::map_type   m_id2elem_map;
			agg::svg::global_settings         m_settings;
			agg::svg::attributes              m_attributes;
			agg::svg::attributes_map          m_attr_map;

			agg::svg::pipeline                m_pipeline;
			agg::svg::gradient_lut_cache      m_gradient_lut_cache;

			void indexation();
			void reset();
		public:
			SVGImage();
		   ~SVGImage(); 

			void  Clear();
			bool  Open(const WCHAR *filename);

		};


}
#endif // !defined(Aggplus_IMAGE_H__4DF00F5C_76B7_431D_8849_6334D98F4477__INCLUDED_)
