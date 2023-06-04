#include "../../../agg-2.4/include/agg_basics.h"
#include "../../../agg-2.4/include/agg_ellipse.h"

#include "../../../agg-2.4/include/agg_color_rgba.h"
#include "../../../agg-2.4/include/agg_pixfmt_rgba.h"
#include "../../../agg-2.4/include/agg_rasterizer_scanline_aa.h"
#include "../../../agg-2.4/include/agg_renderer_scanline.h"
#include "../../../agg-2.4/include/agg_scanline_u.h"

#include "../../../agg-2.4/include/agg_pixfmt_amask_adaptor.h"

#include "../../../agg-2.4/svg/agg_svg_rasterizer.h"

#include "../../../agg-2.4/include/agg_renderer_base.h"
#include "../../../agg-2.4/include/agg_alpha_mask_u8.h"

#include "../../../agg-2.4/include/agg_scanline_p.h"

#include "../../../raster/BgraFrame.h"

typedef agg::rendering_buffer rendering_buffer_type;
typedef agg::pixfmt_bgra32 pixformat_type;

typedef agg::blender_rgba< agg::svg::color_type, agg::svg::component_order >						blender_type;
typedef agg::comp_op_adaptor_rgba< agg::svg::color_type, agg::svg::component_order >				blender_type_comp;
typedef agg::pixfmt_alpha_blend_rgba< blender_type, agg::rendering_buffer, agg::svg::pixel_type >	pixfmt_type;
typedef agg::pixfmt_custom_blend_rgba< blender_type_comp, agg::rendering_buffer>					pixfmt_type_comp;

typedef agg::rasterizer_scanline_aa<> rasterizer_type;

typedef agg::renderer_base<pixfmt_type> base_renderer_type;
typedef agg::renderer_base<pixfmt_type_comp> comp_renderer_type;
typedef agg::scanline_u8 scanline_type;

typedef agg::amask_no_clip_bgra32gray			  alpha_mask_type;
typedef agg::renderer_base<pixformat_type>        ren_base;
typedef agg::renderer_scanline_aa_solid<ren_base> renderer;

typedef unsigned char BYTE;

void testAlphaMask(const std::wstring& wsResultPath, unsigned int unWidth, unsigned int unHeight)
{
	if (0 == unWidth || 0 == unHeight)
		return;

	rasterizer_type oRasterizer;

	// Генерируем альфа-маску

	agg::rendering_buffer   oAlphaMaskRbuf;
	alpha_mask_type         oAlphaMask(oAlphaMaskRbuf);

	agg::int8u* pAlpha_buf = new agg::int8u[unWidth * unHeight * agg::svg::frame_buffer_rgba::pix_size];

	oAlphaMaskRbuf.attach(pAlpha_buf, unWidth,unHeight, -4 * unWidth);

	pixformat_type oPixfAlpha(oAlphaMaskRbuf);
	ren_base oRenBaseAlpha(oPixfAlpha);
	renderer oRendereAlpha(oRenBaseAlpha);

	agg::ellipse ell;

	ell.init(100, 400, 300, 300, 100);

	oRasterizer.add_path(ell);
	oRendereAlpha.color(agg::rgba8(255, 0, 0));

	agg::scanline_p8 oScanLineAlpha;

	agg::render_scanlines(oRasterizer, oScanLineAlpha, oRendereAlpha);

	ell.init(500, 500, 500, 500, 100);
	oRasterizer.add_path(ell);

	typedef agg::scanline_u8_am<alpha_mask_type> scanline_type;

	scanline_type oScanLine(oAlphaMask);

	agg::int8u* pFrameBuffer = new agg::int8u[unWidth * unHeight * agg::svg::frame_buffer_rgba::pix_size];

	rendering_buffer_type oRBuf(pFrameBuffer, unWidth,unHeight, -4 * unWidth);

	pixformat_type pixf(oRBuf);
	ren_base oRenBase(pixf);
	renderer oRenderer(oRenBase);
	oRenderer.color(agg::rgba8(0, 255, 0));

	agg::render_scanlines(oRasterizer, oScanLine, oRenderer);

	CBgraFrame oFrame;
	oFrame.put_Data(pFrameBuffer);
	oFrame.put_Width(unWidth);
	oFrame.put_Height(unHeight);

	oFrame.SaveFile(wsResultPath, 4);

	delete [] pAlpha_buf;
}

int main(int argc, char *argv[])
{
	const std::wstring wsPath = L"alphaMask.png";
	testAlphaMask(wsPath, 1000, 1000);

	return 0;
}
