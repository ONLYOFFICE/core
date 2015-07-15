#ifndef _XPS_XPSLIB_STATICRESOURCES_H
#define _XPS_XPSLIB_STATICRESOURCES_H

#include "Utils.h"
#include <map>

class XmlUtils::CXmlLiteReader;
class XPS::CWString;
class IRenderer;

namespace XPS
{
	class CBrush;
	class CStaticResource
	{
	public:
		CStaticResource(const wchar_t* wsPath);
		CStaticResource(XmlUtils::CXmlLiteReader& oReader);
		~CStaticResource();
		const wchar_t* GetFigure(CWString& wsKey);
		CBrush*        GetBrush(CWString& wsKey);

	private:

		void Parse(XmlUtils::CXmlLiteReader& oReader);
		void AddFigure(const CWString& wsKey, const CWString& wsValue);
		void AddBrush(const CWString& wsKey, CBrush* pBrush);

	private:

		std::map<CWString, CWString> m_mFigures;
		std::map<CWString, CBrush*>  m_mBrushes;
	};

	class CBrush
	{
	public:
		CBrush(){}
		virtual ~CBrush(){}
		virtual bool SetToRenderer(IRenderer* pRenderer) { return false; }
		virtual bool IsImageBrush() { return false; }	
	};
	class CSolidBrush : public CBrush
	{
	public:
		CSolidBrush(int nBgr, int nAlpha) : m_nBgr(nBgr), m_nAlpha(nAlpha){}
		bool SetToRenderer(IRenderer* pRenderer);

	private:

		int m_nBgr;
		int m_nAlpha;
	};
	class CImageBrush : public CBrush
	{
	public:
		CImageBrush(const wchar_t* wsPath)
		{
			m_wsPath.create(wsPath, true);
		}
		bool SetToRenderer(IRenderer* pRenderer);
		bool IsImageBrush() { return true; }
		void SetPaths(const wchar_t* wsRoot, const wchar_t* wsPage);
	private:

		CWString m_wsPath;
		CWString m_wsRoot;
		CWString m_wsPage;
	};
	class CGradientBrush : public CBrush
	{
	public:
		CGradientBrush()
		{
			m_pColors    = NULL;
			m_pPositions = NULL;
			m_lCount     = 0;
		}
		virtual ~CGradientBrush()
		{
			RELEASEARRAYOBJECTS(m_pColors);
			RELEASEARRAYOBJECTS(m_pPositions);
		}
		void SetGradientStops(LONG* pColors, double* pPositions, LONG lCount)
		{
			m_pColors    = pColors;
			m_pPositions = pPositions;
			m_lCount     = lCount;
		}

	protected:

		LONG*   m_pColors;
		double* m_pPositions;
		LONG    m_lCount;
	};
	class CLinearGradientBrush : public CGradientBrush
	{
	public:
		CLinearGradientBrush(const double& dX0, const double& dY0, const double& dX1, const double& dY1) : m_dX0(dX0), m_dY0(dY0), m_dX1(dX1), m_dY1(dY1)
		{
		}
		bool SetToRenderer(IRenderer* pRenderer);

	private:

		double m_dX0;
		double m_dY0;
		double m_dX1;
		double m_dY1;
	};
	class CRadialGradientBrush : public CGradientBrush
	{
	public:
		CRadialGradientBrush(const double& dXcenter, const double& dYcenter, const double& dXorigin, const double& dYorigin, const double& dRadX, const double& dRadY) :
			m_dXc(dXcenter), m_dYc(dYcenter), m_dXo(dXorigin), m_dYo(dYorigin), m_dRadX(dRadX), m_dRadY(dRadY)
		{

		}
		bool SetToRenderer(IRenderer* pRenderer);
	private:

		double m_dXc;
		double m_dYc;
		double m_dXo;
		double m_dYo;
		double m_dRadX;
		double m_dRadY;
	};
	CBrush* ReadBrushNode(XmlUtils::CXmlLiteReader& oReader, const double& dOpacity = 1.0, CWString* pwsKey = NULL);
	CBrush* ReadBrush(XmlUtils::CXmlLiteReader& oReader, const double& dOpacity = 1.0, CWString* pwsKey = NULL);
	CBrush* ReadBrush(const wchar_t* wsString, const double& dOpacity = 1.0);
}

#endif //_XPS_XPSLIB_STATICRESOURCES_H