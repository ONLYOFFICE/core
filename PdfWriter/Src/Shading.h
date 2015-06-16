#ifndef _PDF_WRITER_SRC_SHADING_H
#define _PDF_WRITER_SRC_STREAMS_H

#include "Objects.h"

namespace PdfWriter
{
	enum EShadingType
	{
		shadingtype_Unknown = 0,
		shadingtype_Axial   = 2,
		shadingtype_Radial  = 3
	};
	//----------------------------------------------------------------------------------------
	// CShading
	//----------------------------------------------------------------------------------------
	class CShading : public CDictObject
	{
	public:
		CShading(CXref* pXref);
		virtual ~CShading();

		void SetColors(unsigned char* pColors, double* dPoints, int nCount);
		void SetExtend(bool bBeing, bool bEnd);
		bool CompareColors(unsigned char* pColors, double* pPoints, int nCount);
		bool CompareExtend(bool bBeing, bool bEnd);

		virtual EShadingType GetShadingType()
		{
			return shadingtype_Unknown;
		}

	protected:

		CXref*       m_pXref;
		
	private:

		unsigned char* m_pColors;
		double*        m_pColorsPoints;
		int            m_nColorsCount;

		bool           m_bBeginExtend;
		bool           m_bEndExtend;
	};
	//----------------------------------------------------------------------------------------
	// CAxialShading
	//----------------------------------------------------------------------------------------
	class CAxialShading : public CShading
	{
	public:
		CAxialShading(CXref* pXref, double dX0, double dY0, double dX1, double dY1);
		bool Compare(double dX0, double dY0, double dX1, double dY1);
		EShadingType GetShadingType()
		{
			return shadingtype_Axial;
		}
	private:
		double m_dX0;
		double m_dY0;
		double m_dX1;
		double m_dY1;
	};
	//----------------------------------------------------------------------------------------
	// CRadialShading
	//----------------------------------------------------------------------------------------
	class CRadialShading : public CShading
	{
	public:
		CRadialShading(CXref* pXref, double dX0, double dY0, double dR0, double dX1, double dY1, double dR1);
		bool Compare(double dX0, double dY0, double dR0, double dX1, double dY1, double dR1);
		EShadingType GetShadingType()
		{
			return shadingtype_Radial;
		}
	private:
		double m_dX0;
		double m_dY0;
		double m_dR0;
		double m_dX1;
		double m_dY1;
		double m_dR1;
	};
}

#endif // _PDF_WRITER_SRC_STREAMS_H