#ifndef I_BASE_MATRIX_UPDATER
#define I_BASE_MATRIX_UPDATER

namespace NSHtmlRenderer
{
	class IBaseMatrixUpdater
	{
	public: 
		virtual void OnBaseMatrixUpdate(const double& dWidth, const double& dHeight) = 0;
	};
}

#endif