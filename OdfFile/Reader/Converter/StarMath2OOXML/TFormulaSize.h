#ifndef TFORMULASIZE_H
#define TFORMULASIZE_H

#ifndef STARMATH_USE_DYNAMIC_LIBRARY
#define STARMATH_DECL_EXPORT
#else
#include "../../../../DesktopEditor/common/base_export.h"
#define STARMATH_DECL_EXPORT Q_DECL_EXPORT
#endif
namespace StarMath
{
	struct STARMATH_DECL_EXPORT TFormulaSize
	{
		TFormulaSize():m_iHeight(0),m_iWidth(0) {};
		TFormulaSize(const unsigned int& iHeight,const unsigned int& iwidth):m_iHeight(iHeight),m_iWidth(iwidth) {};
		float m_iHeight;
		float m_iWidth;
		void Zeroing()
		{
			this->m_iHeight = 0;
			this->m_iWidth = 0;
		}
	};
}
#endif // TFORMULASIZE_H
