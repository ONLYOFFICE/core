// AVSMatrix.h : Declaration of the CAVSMatrix

#pragma once
#include "..\stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "Structures.h"
#include "..\Interfaces\IAVSMatrix.h"

// CAVSMatrix
[coclass, uuid("E7D46717-49CE-4733-A917-AEF1A1E22738"), threading(apartment), vi_progid("AVSGraphics.Matrix"), progid("AVSGraphics.Matrix"), version(1.0), support_error_info(IAVSMatrix), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSMatrix
	:	public IAVSMatrix
{
protected:
	Gdiplus::Matrix m_oMatrix;

	friend class CAVSGraphics;

public:

	CAVSMatrix() : m_oMatrix()
	{
	}

	~CAVSMatrix()
	{
	}

public:
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(CreateDublicate)(IAVSGraphicsBase** ppGraphicsBase)
	{
		if (NULL == ppGraphicsBase)
			return S_FALSE;

		RELEASEINTERFACE((*ppGraphicsBase));
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSMatrix), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			NSStructures::SetMatrix(((CAVSMatrix*)(*ppGraphicsBase))->m_oMatrix, m_oMatrix);
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(LoadFromXml)(BSTR bstrXML)
	{
		return S_OK;
	}

	STDMETHOD(SetElements)(double m11, double m12, double m21, double m22, double dx, double dy)
	{
		m_oMatrix.SetElements((float)m11, (float)m12, (float)m21, (float)m22, (float)dx, (float)dy);
		return S_OK;
	}
	STDMETHOD(GetElements)(double* m11, double* m12, double* m21, double* m22, double* dx, double* dy)
	{
		if (NULL == m11 || NULL == m12 || NULL == m21 || NULL == m22 || NULL == dx || NULL == dy)
			return S_FALSE;

		float mass[6];
		m_oMatrix.GetElements(mass);

		*m11	= mass[0];
		*m12	= mass[1];
		*m21	= mass[2];
		*m22	= mass[3];
		*dx		= mass[4];
		*dy		= mass[5];

		return S_OK;
	}

	STDMETHOD(Multiply)(IAVSMatrix* pMatrix, LONG lMatrixOrder)
	{
		if (NULL == pMatrix)
			return S_FALSE;

		m_oMatrix.Multiply(&((CAVSMatrix*)pMatrix)->m_oMatrix, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}
	
	STDMETHOD(Rotate)(double dAngle, LONG lMatrixOrder)
	{
		m_oMatrix.Rotate((float)dAngle, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}

	STDMETHOD(RotateAt)(double dAngle, double dPointX, double dPointY, LONG lMatrixOrder)
	{
		Gdiplus::PointF oCentre((float)dPointX, (float)dPointY);
		m_oMatrix.RotateAt((float)dAngle, oCentre, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}

	STDMETHOD(Scale)(double dScaleX, double dScaleY, LONG lMatrixOrder)
	{
		m_oMatrix.Scale((float)dScaleX, (float)dScaleY, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}

	STDMETHOD(Translate)(double dOffsetX, double dOffsetY, LONG lMatrixOrder)
	{
		m_oMatrix.Translate((float)dOffsetX, (float)dOffsetY, (Gdiplus::MatrixOrder)lMatrixOrder);
		return S_OK;
	}

	STDMETHOD(Transpose)()
	{
		return S_OK;
	}
	STDMETHOD(Invert)()
	{
		if (m_oMatrix.IsInvertible())
		{
			m_oMatrix.Invert();
			return S_OK;
		}
		return S_FALSE;
	}
	
	STDMETHOD(Reset)()
	{
		m_oMatrix.Reset();
		return S_OK;
	}	
};