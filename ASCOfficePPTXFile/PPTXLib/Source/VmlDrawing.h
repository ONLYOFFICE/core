#pragma once
#ifndef PPTX_VMLDRAWING_INCLUDE_H_
#define PPTX_VMLDRAWING_INCLUDE_H_

#include <boost/filesystem.hpp>
#include "WrapperFile.h"
#include "FileContainer.h"
#include "XML.h"
#include <map>

namespace PPTX
{
	class VmlDrawing : public WrapperFile, public PPTX::FileContainer
	{
	public:
		VmlDrawing();
		VmlDrawing(const boost::filesystem::wpath& filename, FileMap& map);
		virtual ~VmlDrawing();

	public:
		virtual void read(const boost::filesystem::wpath& filename, FileMap& map);
		virtual void write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, OOX::ContentTypes::File& content)const;

	public:
		virtual const OOX::FileType type() const;
		virtual const boost::filesystem::wpath DefaultDirectory() const;
		virtual const boost::filesystem::wpath DefaultFileName() const;

		std::map<std::string, boost::filesystem::wpath> SpIds;
		void FillRIds();
	private:
		std::string m_strDocument;
		void GetRIds(const XML::XElement& element);
	};
} // namespace PPTX

#endif // PPTX_VmlDrawingS_VmlDrawing_INCLUDE_H_