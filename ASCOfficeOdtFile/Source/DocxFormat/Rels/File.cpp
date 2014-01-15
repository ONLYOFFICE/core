
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "File.h"
#include "./../FileTypes.h"
#include "FileNameUtility.h"


namespace OOX
{
	namespace Rels
	{

		File::File()
		{
		}


		File::File(const boost::filesystem::wpath& filename)
		{
			read(filename);
		}


		File::~File()
		{
		}


		void File::read(const boost::filesystem::wpath& filename)
		{
			const boost::filesystem::wpath file = createFileName(filename);

			if (boost::filesystem::exists(file))
			{
				const XML::XDocument document(file);
				Relations = document.Root;
			}
		}


		void File::write(const boost::filesystem::wpath& filename) const
		{
			if (!Relations->empty())
			{
				const boost::filesystem::wpath file = createFileName(filename);
				boost::filesystem::create_directories(file.parent_path());

				XML::XElement("Relationships",
					XML::XNamespace("xmlns", "http://schemas.openxmlformats.org/package/2006/relationships") +
					XML::Write(Relations)
				).Save(file);
			}
		}


		const bool File::isValid() const
		{
			return true;
		}


		void File::registration(const RId& rId, const FileType& type, const boost::filesystem::wpath& filename)
		{
			if(!(type == FileTypes::Unknow))
			{
				std::wstring newFilename = filename.filename();
				boost::filesystem::wpath newFilePath = filename.parent_path();
				std::wstring ext = filename.extension();
				if (filename.extension() == L"")
				{
					if (type.RelationType() =="http://schemas.openxmlformats.org/officeDocument/2006/relationships/oleObject")
					{
						newFilename += L".bin" ;
						Relations->registration(rId, type.RelationType(), newFilePath/newFilename);
					}
					else if (type.RelationType() =="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image")
					{
						newFilename += L".wmf" ;
						Relations->registration(rId, type.RelationType(), newFilePath/newFilename);
					}
				}
				else
				{
					Relations->registration(rId, type.RelationType(), replace_extension(filename, L"svm", L"png"));
				}
			}
		}


		void File::registration(const RId& rId, const boost::shared_ptr<External> external)
		{
			Relations->registration(rId, external);
		}


		const boost::filesystem::wpath File::createFileName(const boost::filesystem::wpath& filename) const
		{
			boost::filesystem::wpath name = filename.parent_path() / L"_rels";
			if (filename.filename() == L".")
				name /= L".rels";
			else
				name /= filename.filename() + L".rels";
			return name;
		}

	} // namespace Rels
} // namespace OOX