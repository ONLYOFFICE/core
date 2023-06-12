#include "CAlphaMask.h"

#include "../raster/BgraFrame.h"

namespace Aggplus
{
	CAlphaMask::CAlphaMask()
		: m_enStatus(EmptyAlphaMask), m_oScanLine(m_oAlphaMask)
	{}

	CAlphaMask::~CAlphaMask()
	{
		Destroy();
	}

	StatusAlphaMask CAlphaMask::GetStatus() const
	{
		return m_enStatus;
	}

	void CAlphaMask::Destroy()
	{
		if (NULL != m_oRenderingBuffer.buf())
			delete m_oRenderingBuffer.buf();

		m_oRenderingBuffer.attach(NULL, 0, 0, 0);

		m_enStatus = EmptyAlphaMask;
	}

	Status CAlphaMask::Create(unsigned int unWidth, unsigned int unHeight)
	{
		if (0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		Destroy();

		agg::int8u* pAlpha_buf = new agg::int8u[unWidth * unHeight * agg::svg::frame_buffer_rgba::pix_size];

		if (NULL == pAlpha_buf)
			return OutOfMemory;

		m_oRenderingBuffer.attach(pAlpha_buf, unWidth, unHeight, -4 * unWidth);
		m_oPixfmt.attach(m_oRenderingBuffer);
		m_oRendererBase.attach(m_oPixfmt);

		m_oAlphaMask.attach(m_oRenderingBuffer);

		m_oRendererBase.clear(agg::rgba8(0, 0, 0));

		m_enStatus = GenerationAlphaMask;

		return Ok;
	}

	agg::renderer_base<agg::pixfmt_bgra32> &CAlphaMask::GetRendereBase()
	{
		return m_oRendererBase;
	}

	agg::rendering_buffer &CAlphaMask::GetRenderingBuffer()
	{
		return m_oRenderingBuffer;
	}

	amask_scanline_type& CAlphaMask::GetScanline()
	{
		return m_oScanLine;
	}

	void CAlphaMask::StartApplying()
	{
		m_enStatus = ApplyingAlphaMask;
	}
}
