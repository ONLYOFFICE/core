#include "../../../agg-2.4/include/agg_basics.h"
#include "../../../agg-2.4/include/agg_ellipse.h"

#include "../../../agg-2.4/include/agg_color_rgba.h"
#include "../../../agg-2.4/include/agg_pixfmt_rgba.h"
#include "../../../agg-2.4/include/agg_rasterizer_scanline_aa.h"
#include "../../../agg-2.4/include/agg_renderer_scanline.h"
#include "../../../agg-2.4/include/agg_scanline_u.h"

#include "../../../agg-2.4/svg/agg_svg_rasterizer.h"

#include "../../../agg-2.4/include/agg_renderer_base.h"
#include "../../../agg-2.4/include/agg_alpha_mask_u8.h"

#include "../../../agg-2.4/include/agg_scanline_p.h"

#include "../../../agg-2.4/include/agg_pixfmt_gray.h"

#include "../../../raster/BgraFrame.h"

typedef agg::rendering_buffer         rendering_buffer_type;
typedef agg::rasterizer_scanline_aa<> rasterizer_type;
typedef agg::scanline_u8              scanline_type;

typedef unsigned char BYTE;

void testAlphaMask(const std::wstring& wsResultPath, unsigned int unWidth, unsigned int unHeight)
{
	if (wsResultPath.empty() || 0 == unWidth || 0 == unHeight)
		return;

	typedef agg::pixfmt_bgra32                         pixfmt_type;
	typedef agg::alpha_mask_bgra32gray			       alpha_mask_type;
	typedef agg::renderer_base<agg::pixfmt_bgra32>     ren_base;
	typedef agg::renderer_scanline_aa_solid<ren_base>  renderer;

	rasterizer_type oRasterizer;

	// Генерируем альфа-маску

	agg::rendering_buffer   oAlphaMaskRbuf;
	alpha_mask_type         oAlphaMask(oAlphaMaskRbuf);

	agg::int8u* pAlpha_buf = new agg::int8u[unWidth * unHeight * agg::svg::frame_buffer_rgba::pix_size];

	oAlphaMaskRbuf.attach(pAlpha_buf, unWidth,unHeight, -4 * unWidth);

	pixfmt_type oPixfAlpha(oAlphaMaskRbuf);
	ren_base oRenBaseAlpha(oPixfAlpha);

	oRenBaseAlpha.clear(agg::rgba(0, 0, 0));

	renderer oRendereAlpha(oRenBaseAlpha);

	agg::ellipse ell;

	ell.init(unWidth / 10, unWidth / 5, unWidth / 3, unWidth / 3, 100);

	oRasterizer.add_path(ell);
	oRendereAlpha.color(agg::rgba8(255, 0, 0));

	agg::scanline_p8 oScanLineAlpha;

	agg::render_scanlines(oRasterizer, oScanLineAlpha, oRendereAlpha);

	//

	ell.init(unWidth / 2, unHeight / 2, unWidth / 2, unHeight /2, 100);
	oRasterizer.add_path(ell);

	typedef agg::scanline_u8_am<alpha_mask_type> scanline_type;

	scanline_type oScanLine(oAlphaMask);

	agg::int8u* pFrameBuffer = new agg::int8u[unWidth * unHeight * agg::svg::frame_buffer_rgba::pix_size];

	rendering_buffer_type oRBuf(pFrameBuffer, unWidth,unHeight, -4 * unWidth);

	pixfmt_type pixf(oRBuf);
	ren_base oRenBase(pixf);
	renderer oRenderer(oRenBase);

	oRenBase.clear(agg::rgba8(0, 0, 255));
	oRenderer.color(agg::rgba8(0, 255, 0));

	agg::render_scanlines(oRasterizer, oScanLine, oRenderer);

	CBgraFrame oFrame;
	oFrame.put_Data(pFrameBuffer);
	oFrame.put_Width(unWidth);
	oFrame.put_Height(unHeight);

	oFrame.SaveFile(wsResultPath, 4);

	delete [] pAlpha_buf;
}

void testAlphaChennel(const std::wstring& wsResultPath, unsigned int unWidth, unsigned int unHeight)
{
	if (wsResultPath.empty() || 0 == unWidth || 0 == unHeight)
		return;

	typedef agg::pixfmt_gray8                          apixfmt_type;
	typedef agg::alpha_mask_gray8                      alpha_mask_type;
	typedef agg::renderer_base<apixfmt_type>           aren_base;
	typedef agg::renderer_scanline_aa_solid<aren_base> arenderer;

	rasterizer_type oRasterizer;

	// Генерируем альфа-маску
	agg::rendering_buffer   oAlphaMaskRbuf;
	alpha_mask_type         oAlphaMask(oAlphaMaskRbuf);

	agg::int8u* pAlpha_buf = new agg::int8u[unWidth * unHeight];

	oAlphaMaskRbuf.attach(pAlpha_buf, unWidth,unHeight, -unWidth);

	apixfmt_type oPixfAlpha(oAlphaMaskRbuf);
	aren_base oRenBaseAlpha(oPixfAlpha);

	oRenBaseAlpha.clear(agg::gray8(150));

	arenderer oRendereAlpha(oRenBaseAlpha);

	agg::ellipse ell;

	ell.init(unWidth / 10, unWidth / 5, unWidth / 3, unWidth / 3, 100);

	oRasterizer.add_path(ell);
	oRendereAlpha.color(agg::gray8(50));

	agg::scanline_p8 oScanLineAlpha;

	agg::render_scanlines(oRasterizer, oScanLineAlpha, oRendereAlpha);

	//

	typedef agg::pixfmt_bgra32                         pixfmt_type;
	typedef agg::renderer_base<agg::pixfmt_bgra32>     ren_base;
	typedef agg::renderer_scanline_aa_solid<ren_base>  renderer;
	typedef agg::scanline_u8_am<alpha_mask_type>       scanline_type;

	ell.init(unWidth / 2, unHeight / 2, unWidth / 2, unHeight /2, 100);
	oRasterizer.add_path(ell);

	scanline_type oScanLine(oAlphaMask);

	agg::int8u* pFrameBuffer = new agg::int8u[unWidth * unHeight * agg::svg::frame_buffer_rgba::pix_size];

	rendering_buffer_type oRBuf(pFrameBuffer, unWidth,unHeight, -4 * unWidth);

	pixfmt_type pixf(oRBuf);
	ren_base oRenBase(pixf);
	renderer oRenderer(oRenBase);

	oRenBase.clear(agg::rgba8(0, 0, 255));
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
	const std::wstring wsTest1 = L"alphaMask.png";
	testAlphaMask(wsTest1, 1000, 1000);

	const std::wstring wsTest2 = L"alphaChennelMask.png";
	testAlphaChennel(wsTest2, 1000, 1000);

	return 0;
}
