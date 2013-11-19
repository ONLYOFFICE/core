#pragma once
#include <string>
#include <map>
#include <vector>


class MhtFile
{
public:

	MhtFile();
    ~MhtFile ();

public:
	void ReadFile(const std::wstring& fileName);

public:
	bool savePicture(const std::wstring& name, const std::wstring& path) const;

	const std::wstring getStyle(const std::wstring& name) const;
	const std::wstring getFrame(const std::wstring& name) const;

	const std::wstring getHtmlFile() const;
	const std::wstring getHtmlFileName() const;
	
	const std::wstring base64To8bit(const std::wstring& fileBase64) const;

public:
	void setHtmlFile (std::wstring name, std::wstring content);
	void addFrameFile (std::wstring name, std::wstring frame_name);
	void removeFrameFile (std::wstring frame_name);

private:	
	const std::wstring getPicture(const std::wstring& name) const;

private:
	const std::wstring getFileProperty(const std::wstring& line, const std::wstring& propertyName) const;
	const std::string encodingQuotedPrintable(const std::string& line) const;
	const std::wstring file2ansi(const std::vector<std::string>& file) const;

private:
	std::wstring						 m_content;
	std::wstring						 m_content_file_name;
	std::map<std::wstring, std::wstring> m_pictures;
	std::map<std::wstring, std::wstring> m_styles;
	std::map<std::wstring, std::wstring> m_frames;
};