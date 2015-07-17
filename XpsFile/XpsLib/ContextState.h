#ifndef _XPS_XPSLIB_CONTEXTSTATE_H
#define _XPS_XPSLIB_CONTEXTSTATE_H

#include "Utils.h"

#include "../../DesktopEditor/graphics/Matrix.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

#include <list>
#include <map>

namespace XPS
{
	class CBrush;
	class CStaticResource;

	class CContextState
	{
	private:

		struct TStaticRecource
		{
			TStaticRecource(CStaticResource* resource, bool own)
			{
				pResource = resource;
				bOwn      = own;
			}

			CStaticResource* pResource;
			bool             bOwn;
		};

	public:

		CContextState(IRenderer* pRenderer);
		~CContextState();

		void    PushOpacity(const double& dOpacity);
		void    PopOpacity();
		double  GetCurrentOpacity();
		void    PushClip(const CWString& wsClip);
		void    PopClip();
		void    PushTransform(const double arrTransform[6]);
		void    PopTransform();
		double  NormalizeTransform();
		void    PushResource(CStaticResource* pResource, bool bOwn);
		void    PopResource();
			    
		void    GetPathGeometry(const CWString& wsKey, CWString& wsPathData, CWString& wsPathTransform);
		CBrush* GetBrush(const CWString& wsKey);
		void    GetTransform(const CWString& wsKey, CWString& wsTransform);
		Aggplus::CMatrix GetCurrentTransform()
		{
			return m_oCurrentTransform;
		}

	private:

		void SetClipToRenderer(const CWString& wsClip);
		void SetTransformToRenderer();

	private:

		Aggplus::CMatrix            m_oCurrentTransform;
		std::list<Aggplus::CMatrix> m_lTransformStack;
		std::vector<CWString>       m_vClipStack;
		IRenderer*                  m_pRenderer;
		std::vector<double>         m_vOpacity;
		double                      m_dCurOpacity;
		std::vector<TStaticRecource>m_vResourcesStack;
	};
}

#endif //_XPS_XPSLIB_CONTEXTSTATE_H