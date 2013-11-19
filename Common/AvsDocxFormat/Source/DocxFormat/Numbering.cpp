
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Numbering.h"
#include "Exception/log_runtime_error.h"
#include "FileTypes.h"


namespace OOX
{

	Numbering::Numbering()
	{
	}


	Numbering::Numbering(const boost::filesystem::wpath& filename)
	{
		read(filename);
	}


	Numbering::~Numbering()
	{
	}


	void Numbering::read(const boost::filesystem::wpath& filename)
	{
		const XML::XDocument document(filename);
		XML::Fill(AbstractNums, document.Root, "abstractNum");
		XML::Fill(Nums, document.Root, "num");
	}


	void Numbering::write(const boost::filesystem::wpath& filename, const boost::filesystem::wpath& directory, ContentTypes::File& content) const
	{
		XML::XElement(ns.w + "numbering",
			XML::Write(AbstractNums) +
			XML::Write(Nums)
			).Save(filename);

		content.registration(type().OverrideType(), directory, filename);
	}


	void Numbering::clear()
	{
		AbstractNums->clear();
		Nums->clear();
	}


	const int Numbering::getAbstractNumId(const int numId) const
	{
		BOOST_FOREACH(const Numbering::Num& num, *Nums)
		{
			if (num.NumId == numId)
				return num.AbstractNumId;
		}
		throw log_runtime_error("bad numID");
	}


	const Numbering::AbstractNum Numbering::getAbstractNum(const int abstractNumId) const
	{
		BOOST_FOREACH(const Numbering::AbstractNum& abstractNum, *AbstractNums)
		{
			if (abstractNum.Id == abstractNumId)
				return abstractNum;
		}
		throw log_runtime_error("bad abstractNumId");
	}


	const FileType Numbering::type() const
	{
		return FileTypes::Numbering;
	}


	const boost::filesystem::wpath Numbering::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}


	const boost::filesystem::wpath Numbering::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

} // namespace OOX