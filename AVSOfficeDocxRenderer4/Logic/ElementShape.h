#pragma once
#include "Common.h"

namespace NSDocxRenderer
{
	static _bstr_t g_vml_bstr_ClosePath			= L"x";
	static _bstr_t g_vml_bstr_EndPath			= L"e";
	static _bstr_t g_vml_bstr_NoFill			= L"nf";
	static _bstr_t g_vml_bstr_NoStroke			= L"ns";
	static CString g_vml_string_MoveTo			= _T("m%d,%d");
	static CString g_vml_string_LineTo			= _T("l%d,%d");
	static CString g_vml_string_CurveTo			= _T("c%d,%d %d,%d %d,%d");

	static _bstr_t g_bstr_shape1				= L"<w:r><w:pict><v:shape id=\"\" o:spid=\"\" style=\"position:absolute;";
	static _bstr_t g_bstr_shape2				= L"z-index:-1;mso-position-horizontal-relative:page;mso-position-vertical-relative:page;\"";
	static CString g_string_shape_position		= _T("margin-left:%.2lfmm;margin-top:%.2lfmm;width:%.2lfmm;height:%.2lfmm;");

	static CString g_string_shape_geometry		= _T(" coordsize=\"%d,%d\" path=\"%s\" fillcolor=\"#%06x\" strokecolor=\"#%06x\" strokeweight=\"%.2lfmm\">");

	static CString g_string_tx_rid				= _T("<v:imagedata r:id=\"rId%d\" o:title=\"\"/>");
	static CString g_string_fill_opacity		= _T("<v:fill opacity=\"%.2lf\"/>");
	static CString g_string_stroke_opacity		= _T("<v:stroke opacity=\"%.2lf\"/>");

	static _bstr_t g_bstr_shape3				= L"<w10:wrap anchorx=\"page\" anchory=\"page\"/></v:shape></w:pict></w:r>";

	class CVectorGraphics
	{
	public:
		double*	m_pData;
		size_t	m_lSize;

		double*	m_pDataCur;
		size_t	m_lSizeCur;

	public:
		double m_dLeft;
		double m_dTop;
		double m_dRight;
		double m_dBottom;

	public:
		CVectorGraphics()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;

			End();
		}
		~CVectorGraphics()
		{
			RELEASEMEM(m_pData);
		}

		AVSINLINE void AddSize(size_t nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 500);				
				m_pData = (double*)malloc(m_lSize * sizeof(double));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				double* pRealloc = (double*)realloc(m_pData, m_lSize * sizeof(double));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					double* pMalloc = (double*)malloc(m_lSize * sizeof(double));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(double));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		AVSINLINE void MoveTo(const double& x1, const double& y1)
		{
			AddSize(3);
			*m_pDataCur = 0;	++m_pDataCur;

			*m_pDataCur = x1;	++m_pDataCur;
			*m_pDataCur = y1;	++m_pDataCur;

			m_lSizeCur += 3;

			CheckPoint(x1, y1);			
		}
		AVSINLINE void LineTo(const double& x1, const double& y1)
		{
			AddSize(3);
			*m_pDataCur = 1;	++m_pDataCur;

			*m_pDataCur = x1;	++m_pDataCur;
			*m_pDataCur = y1;	++m_pDataCur;

			m_lSizeCur += 3;

			CheckPoint(x1, y1);
		}
		AVSINLINE void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
		{
			AddSize(7);
			*m_pDataCur = 2;	++m_pDataCur;

			*m_pDataCur = x1;	++m_pDataCur;
			*m_pDataCur = y1;	++m_pDataCur;
			*m_pDataCur = x2;	++m_pDataCur;
			*m_pDataCur = y2;	++m_pDataCur;
			*m_pDataCur = x3;	++m_pDataCur;
			*m_pDataCur = y3;	++m_pDataCur;

			m_lSizeCur += 7;

			CheckPoint(x1, y1);
			CheckPoint(x2, y2);
			CheckPoint(x3, y3);
		}
		AVSINLINE void Close()
		{
			AddSize(1);
			*m_pDataCur = 3;	++m_pDataCur;

			m_lSizeCur += 1;
		}

		AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}

		AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		AVSINLINE void End()
		{
			ClearNoAttack();

			m_dLeft		= 0xFFFFFF;
			m_dTop		= 0xFFFFFF;
			m_dRight	= -0xFFFFFF;
			m_dBottom	= -0xFFFFFF;
		}

		AVSINLINE void CheckPoint(const double& x, const double& y)
		{
			if (m_dLeft > x)
				m_dLeft = x;
			if (m_dRight < x)
				m_dRight = x;
			if (m_dTop > y)
				m_dTop = y;
			if (m_dBottom < y)
				m_dBottom = y;
		}
	};

	class CShape : public CBaseItem
	{
	public:
		CString m_strPath;
		NSStructures::CBrush	m_oBrush;
		NSStructures::CPen		m_oPen;

		double m_dLeft;
		double m_dTop;
		double m_dWidth;
		double m_dHeight;
		
		bool m_bIsFill;
		bool m_bIsStroke;

		LONG m_lCoordSizeX;
		LONG m_lCoordSizeY;

		LONG m_lTxId;

	public:
		CShape()
		{
			m_dLeft		= 0;
			m_dTop		= 0;
			m_dWidth	= 0;
			m_dHeight	= 0;
			
			m_bIsFill	= false;
			m_bIsStroke	= false;

			m_lCoordSizeX	= 100000;
			m_lCoordSizeY	= 100000;

			m_lTxId			= -1;
		}

		CShape(const CShape& oSrc)
		{
			*this = oSrc;
		}

		CShape& operator=(const CShape& oSrc)
		{
			m_eType		= etShape;
			m_strPath	= oSrc.m_strPath;

			m_oBrush	= oSrc.m_oBrush;
			m_oPen		= oSrc.m_oPen;

			m_dLeft		= oSrc.m_dLeft;
			m_dTop		= oSrc.m_dTop;
			m_dWidth	= oSrc.m_dWidth;
			m_dHeight	= oSrc.m_dHeight;

			m_bIsFill	= oSrc.m_bIsFill;
			m_bIsStroke	= oSrc.m_bIsStroke;

			m_lTxId		= oSrc.m_lTxId;

			return *this;
		}

		void CreateFromVectorData(CVectorGraphics* pVector, NSDocxRenderer::CStringWriter& oWriter, const LONG& lCoordSize, LONG lType)
		{
			m_dLeft		= pVector->m_dLeft;
			m_dTop		= pVector->m_dTop;
			m_dWidth	= pVector->m_dRight - m_dLeft;
			m_dHeight	= pVector->m_dBottom - m_dTop;

			m_lCoordSizeX = lCoordSize;
			m_lCoordSizeY = lCoordSize;
			
			size_t nCount = pVector->GetCurSize();
			double* pData = pVector->m_pData;
			while (nCount > 0)
			{
				double dType = *pData++;
				if (0 == dType)
				{
					LONG lX = (LONG)((*pData - m_dLeft) * lCoordSize / m_dWidth);
					++pData;
					LONG lY = (LONG)((*pData - m_dTop) * lCoordSize / m_dHeight);
					++pData;

					CString strPath = _T("");
					strPath.Format(g_vml_string_MoveTo, lX, lY);

					oWriter.WriteString(strPath);

					nCount -= 3;
				}
				else if (1 == dType)
				{
					LONG lX = (LONG)((*pData - m_dLeft) * lCoordSize / m_dWidth);
					++pData;
					LONG lY = (LONG)((*pData - m_dTop) * lCoordSize / m_dHeight);
					++pData;

					CString strPath = _T("");
					strPath.Format(g_vml_string_LineTo, lX, lY);

					oWriter.WriteString(strPath);

					nCount -= 3;
				}
				else if (2 == dType)
				{
					LONG lX1 = (LONG)((*pData - m_dLeft) * lCoordSize / m_dWidth);
					++pData;
					LONG lY1 = (LONG)((*pData - m_dTop) * lCoordSize / m_dHeight);
					++pData;

					LONG lX2 = (LONG)((*pData - m_dLeft) * lCoordSize / m_dWidth);
					++pData;
					LONG lY2 = (LONG)((*pData - m_dTop) * lCoordSize / m_dHeight);
					++pData;

					LONG lX3 = (LONG)((*pData - m_dLeft) * lCoordSize / m_dWidth);
					++pData;
					LONG lY3 = (LONG)((*pData - m_dTop) * lCoordSize / m_dHeight);
					++pData;

					CString strPath = _T("");
					strPath.Format(g_vml_string_CurveTo, lX1, lY1, lX2, lY2, lX3, lY3);

					oWriter.WriteString(strPath);

					nCount -= 7;
				}
				else
				{
					oWriter.WriteString(g_vml_bstr_ClosePath);
					--nCount;
				}
			}

			if (0x00 == (lType & 0x01))
				oWriter.WriteString(g_vml_bstr_NoStroke);
			if (0x00 == (lType >> 8))
				oWriter.WriteString(g_vml_bstr_NoFill);

			oWriter.WriteString(g_vml_bstr_EndPath);

			m_strPath = oWriter.GetData();
			oWriter.ClearNoAttack();
		}

		virtual void ToXml(NSDocxRenderer::CStringWriter& oWriter)
		{
			oWriter.WriteString(g_bstr_shape1);

			CString strPosition = _T("");
			strPosition.Format(g_string_shape_position, m_dLeft, m_dTop, m_dWidth, m_dHeight);
			oWriter.WriteString(strPosition);

			oWriter.WriteString(strPosition);
			oWriter.WriteString(g_bstr_shape2);

			CString strStyle = _T("");
			strStyle.Format(g_string_shape_geometry, m_lCoordSizeX, m_lCoordSizeY, m_strPath, ConvertColor(m_oBrush.Color1), ConvertColor(m_oPen.Color), m_oPen.Size);
			oWriter.WriteString(strStyle);

			if (c_BrushTypeTexture == m_oBrush.Type)
			{
				CString strImage = _T("");
				strImage.Format(g_string_tx_rid, 10 + m_lTxId);
				oWriter.WriteString(strImage);

				if (0xFF != m_oBrush.TextureAlpha)
				{
					CString strFillOpacity = _T("");
					strFillOpacity.Format(g_string_fill_opacity, (double)m_oBrush.TextureAlpha / 255.0);
					oWriter.WriteString(strFillOpacity);
				}
			}
			else
			{
				if (0xFF != m_oBrush.Alpha1)
				{
					CString strFillOpacity = _T("");
					strFillOpacity.Format(g_string_fill_opacity, (double)m_oBrush.Alpha1 / 255.0);
					oWriter.WriteString(strFillOpacity);
				}
				if (0xFF != m_oPen.Alpha)
				{
					CString strPenOpacity = _T("");
					strPenOpacity.Format(g_string_stroke_opacity, (double)m_oPen.Alpha / 255.0);
					oWriter.WriteString(strPenOpacity);
				}
			}

			oWriter.WriteString(g_bstr_shape3);
		}
	};
}