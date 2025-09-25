#pragma once
#include "../../../../DesktopEditor/graphics/Image.h"
#include "../../resources/ImageInfo.h"
#include <memory>

#ifndef DOCXRENDERER_USE_DYNAMIC_LIBRARY
#define DOCXRENDERER_DECL_EXPORT
#else
#include "../../../../DesktopEditor/common/base_export.h"
#define DOCXRENDERER_DECL_EXPORT Q_DECL_EXPORT
#endif

namespace NSDocxRenderer
{
	class DOCXRENDERER_DECL_EXPORT IImageStorage
	{
	public:
		IImageStorage();
		virtual ~IImageStorage();

	public:
		virtual std::shared_ptr<CImageInfo> GenerateImageID(Aggplus::CImage* pImage) = 0;
		virtual std::string* GetBase64(const int& nRId) = 0;
	};

	DOCXRENDERER_DECL_EXPORT IImageStorage* CreateWasmImageStorage();
}
