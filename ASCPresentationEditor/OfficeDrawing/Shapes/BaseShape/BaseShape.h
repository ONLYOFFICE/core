#pragma once
#include "Path.h"

namespace NSPresentationEditor
{
	using namespace NSBaseShape;
	class CHandle_
	{
	public:
		CString position;
		CString xrange;
		CString yrange;
		CString switchHandle;
		CString polar;
		CString radiusrange; 

	public:
		CHandle_()
		{
			position = _T("");
			xrange = _T("");
			yrange = _T("");
			switchHandle = _T("");
			polar = _T("");
			radiusrange = _T(""); 
		}
		CHandle_& operator =(const CHandle_& oSrc)
		{
			position		= oSrc.position;
			xrange			= oSrc.xrange;
			yrange			= oSrc.yrange;
			switchHandle	= oSrc.switchHandle;
			polar			= oSrc.polar;
			radiusrange		= oSrc.radiusrange;
			return (*this);
		}
	};

	class CBaseShape
	{
	public:
		CSimpleArray<long>		m_arAdjustments;
		CSimpleArray<double>	Guides;
	public: 
		LONG					m_eJoin;
		bool					m_bConcentricFill;

		CSimpleArray<CPoint>	m_arConnectors;
		CSimpleArray<LONG>		m_arConnectorAngles;

		CSimpleArray<RECT>		m_arTextRects;
		
		CSimpleArray<CHandle_>	m_arHandles;

		CString					m_strTransformXml;

		CString					m_strPath;
		CString					m_strRect;

		LONG					m_lLimoX;
		LONG					m_lLimoY;
		
		CPath					m_oPath;
	public:
		CBaseShape()
		{
		}

		virtual bool LoadFromXML(const CString& xml)			= 0;
		virtual bool LoadFromXML(XmlUtils::CXmlNode& xmlNode)	= 0;
		virtual bool LoadAdjustValuesList(const CString& xml)	= 0;
		virtual bool LoadGuidesList(const CString& xml)			= 0;
		virtual bool LoadAdjustHandlesList(const CString& xml)	= 0;
		virtual bool LoadConnectorsList(const CString& xml)		= 0;
		virtual bool LoadTextRect(const CString& xml)			= 0;
		virtual bool LoadPathList(const CString& xml)			= 0;
		virtual bool SetAdjustment(long index, long value)		= 0;
		virtual void ReCalculate()								= 0;

		virtual CString ToXML(CGeomShapeInfo& GeomInfo, CMetricInfo& MetricInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen) = 0;
		virtual void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, CMetricInfo& pInfo)
		{
			m_oPath.ToRenderer(pRenderer, pGeomInfo, dStartTime, dEndTime, pPen, pFore, pInfo, GetClassType());
		}

		virtual void SetWidthHeightLogic(const double& dWidth, const double& dHeight)
		{
			// none
		}
		virtual void GetWidthHeightLogic(double& dWidth, double& dHeight)
		{
			// none
		}

		virtual void AddGuide(const CString& strGuide)	{}

		static CBaseShape* CreateByType(ClassType ClassType, int ShapeType);
		
		virtual const ClassType GetClassType()const				= 0;
		bool SetType(ClassType ClassType, int ShapeType);

		virtual bool SetProperties(CBaseShape* Shape)
		{
			if( Shape == NULL)
				return false;

			m_oPath		= Shape->m_oPath;
			m_strPath	= Shape->m_strPath;
			m_strRect	= Shape->m_strRect;
			
			m_arAdjustments.RemoveAll();
			for(int i = 0; i < Shape->m_arAdjustments.GetSize(); i++)
				m_arAdjustments.Add(Shape->m_arAdjustments[i]);

			Guides.RemoveAll();
			for(int i = 0; i < Shape->Guides.GetSize(); i++)
				Guides.Add(Shape->Guides[i]);

			m_eJoin				= Shape->m_eJoin;
			m_bConcentricFill	= Shape->m_bConcentricFill;

			m_arConnectors.RemoveAll();
			for(int i = 0; i < Shape->m_arConnectors.GetSize(); i++)
				m_arConnectors.Add(Shape->m_arConnectors[i]);

			m_arConnectorAngles.RemoveAll();
			for(int i = 0; i < Shape->m_arConnectorAngles.GetSize(); i++)
				m_arConnectorAngles.Add(Shape->m_arConnectorAngles[i]);

			m_arTextRects.RemoveAll();
			for(int i = 0; i < Shape->m_arTextRects.GetSize(); i++)
				m_arTextRects.Add(Shape->m_arTextRects[i]);

			m_arHandles.RemoveAll();
			for(int i = 0; i < Shape->m_arHandles.GetSize(); i++)
				m_arHandles.Add(Shape->m_arHandles[i]);


			m_strRect = Shape->m_strRect;
			m_strTransformXml = Shape->m_strTransformXml;
			return true;
		}

		virtual bool SetToDublicate(CBaseShape* Shape)
		{
			if( Shape == NULL)
				return false;

			Shape->m_oPath		= m_oPath;
			Shape->m_strPath	= m_strPath;
			Shape->m_strRect	= m_strRect;
			
			Shape->m_arAdjustments.RemoveAll();
			for(int i = 0; i < m_arAdjustments.GetSize(); i++)
				Shape->m_arAdjustments.Add(m_arAdjustments[i]);

			Shape->Guides.RemoveAll();
			for(int i = 0; i < Guides.GetSize(); i++)
				Shape->Guides.Add(Guides[i]);

			Shape->m_eJoin				= m_eJoin;
			Shape->m_bConcentricFill	= m_bConcentricFill;

			Shape->m_arConnectors.RemoveAll();
			for(int i = 0; i < m_arConnectors.GetSize(); i++)
				Shape->m_arConnectors.Add(m_arConnectors[i]);

			Shape->m_arConnectorAngles.RemoveAll();
			for(int i = 0; i < m_arConnectorAngles.GetSize(); i++)
				Shape->m_arConnectorAngles.Add(m_arConnectorAngles[i]);

			Shape->m_arTextRects.RemoveAll();
			for(int i = 0; i < m_arTextRects.GetSize(); i++)
				Shape->m_arTextRects.Add(m_arTextRects[i]);
			Shape->m_strRect = m_strRect;

			Shape->m_arHandles.RemoveAll();
			for(int i = 0; i < m_arHandles.GetSize(); i++)
				Shape->m_arHandles.Add(m_arHandles[i]);

			Shape->m_strTransformXml = m_strTransformXml;
			return true;
		}
	};
}