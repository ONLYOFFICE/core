#ifndef _BUILD_CALPHAMASK_H_
#define _BUILD_CALPHAMASK_H_

#include "aggplustypes.h"

#include "../agg-2.4/include/agg_alpha_mask_u8.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/svg/agg_svg_rasterizer.h"

namespace Aggplus
{
	typedef agg::alpha_mask_bgra32gray			      alpha_mask_type;
	typedef agg::scanline_u8_am<alpha_mask_type>      amask_scanline_type;

	enum StatusAlphaMask
	{
		EmptyAlphaMask,
		GenerationAlphaMask,
		ApplyingAlphaMask
	};

	class CAlphaMask
	{
		StatusAlphaMask                        m_enStatus;

		agg::rendering_buffer                  m_oRenderingBuffer;
		agg::pixfmt_bgra32                     m_oPixfmt;
		agg::renderer_base<agg::pixfmt_bgra32> m_oRendererBase;
		alpha_mask_type                        m_oAlphaMask;

		amask_scanline_type                    m_oScanLine; // Используется для применения альфа маски
	public:
		CAlphaMask();
		~CAlphaMask();

		StatusAlphaMask GetStatus() const;

		void Destroy();
		Status Create(unsigned int unWidth, unsigned int unHeight);

		agg::renderer_base<agg::pixfmt_bgra32>& GetRendereBase();
		agg::rendering_buffer&                  GetRenderingBuffer();
		amask_scanline_type&                    GetScanline();

		void StartApplying();
	};
}

#endif // _BUILD_CALPHAMASK_H_
