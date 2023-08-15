#pragma comment(lib, "../../../../../build/lib/win_64/debug/kernel.lib")
#pragma comment(lib, "../../../../../build/lib/win_64/debug/UnicodeConverter.lib")
#include <iostream>
#include <random>
#include <sstream>
#include <chrono>
#include <codecvt>
#include <fstream>
#include "../../../utf8cpp/utf8.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../simple_xml_writer.h"
#include "../../simple_xml_writer4.h"

using namespace cpdoccore;

class BufferTest : public testing::Test
{
public:
	std::wstring data;
	std::wstring data1;
	std::wstring data2;
public:
	virtual void SetUp() override
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0x0020, 0x007E);

		data.reserve(250);
		data1.reserve(100);
		data2.reserve(50);

		for (size_t i = 0; i < 250; ++i) data.push_back(static_cast<wchar_t>(dis(gen)));
		for (size_t i = 0; i < 100; ++i) data1.push_back(static_cast<wchar_t>(dis(gen)));
		for (size_t i = 0; i < 50; ++i) data2.push_back(static_cast<wchar_t>(dis(gen)));
	}
	virtual void TearDown() override
	{
		data.clear();
		data1.clear();
		data2.clear();
	}
};

class ReadAndWrite
{
public:
	ReadAndWrite(xml_writer& _xml_wr_, XmlUtils::CXmlLiteReader& oReader) : _xml_wr_(_xml_wr_), oReader(oReader) {};
	~ReadAndWrite() {};
	void AddNode(int depth)
	{
		while (oReader.ReadNextSiblingNode(depth))
		{
			int tmp = oReader.GetDepth();
			std::wstring sName = oReader.GetName();
			CP_XML_NODE(sName)
			{
				int na = oReader.GetAttributesCount();
				if (na > 0)
				{
					if (oReader.MoveToFirstAttribute())
					{
						std::wstring sNameAttr = oReader.GetName();
						std::wstring sAttr = oReader.GetText();
						CP_XML_ATTR2(sNameAttr, sAttr);
						for (int i = 1; i < na; i++)
						{
							if (oReader.MoveToNextAttribute())
							{
								sNameAttr = oReader.GetName();
								sAttr = oReader.GetText();
								CP_XML_ATTR2(sNameAttr, sAttr);
							}
						}		
						oReader.MoveToElement();
					}
				}
				if ((sName == L"w:t" || sName == L"w:instrText" || sName == L"m:t" || sName == L"wp:posOffset" || sName == L"w14:pctHeight" || sName == L"w14:pctWidth" || sName == L"wp:align") && !oReader.IsEmptyNode())
				{
					std::wstring sVal = oReader.GetText2();
					CP_XML_CONTENT(sVal);
				}
				else if (!oReader.IsEmptyNode()) this->AddNode(tmp);
			}
		}
	}
	
public:
	xml_writer& _xml_wr_;
	XmlUtils::CXmlLiteReader& oReader;
};

class ReadAndWrite4
{
public:
	ReadAndWrite4(xml_writer4& _xml_wr_4, XmlUtils::CXmlLiteReader& oReader) : _xml_wr_4(_xml_wr_4), oReader(oReader) {};
	~ReadAndWrite4() {};
	void AddNode(int depth)
	{
		while (oReader.ReadNextSiblingNode(depth))
		{
			int tmp = oReader.GetDepth();
			std::wstring sName = oReader.GetName();
			CP_XML_NODE_4(sName)
			{
				int na = oReader.GetAttributesCount();
				if (na > 0)
				{
					if (oReader.MoveToFirstAttribute())
					{
						std::wstring sNameAttr = oReader.GetName();
						std::wstring sAttr = oReader.GetText();
						CP_XML_ATTR2_4(sNameAttr, sAttr);
						for (int i = 1; i < na; i++)
						{
							if (oReader.MoveToNextAttribute())
							{
								sNameAttr = oReader.GetName();
								sAttr = oReader.GetText();
								CP_XML_ATTR2_4(sNameAttr, sAttr);
							}
						}
						oReader.MoveToElement();
					}
				}
				if ((sName == L"w:t" || sName == L"w:instrText" || sName == L"m:t" || sName == L"wp:posOffset" || sName == L"w14:pctHeight" || sName == L"w14:pctWidth" || sName == L"wp:align") && !oReader.IsEmptyNode())
				{
					std::wstring sVal = oReader.GetText2();
					CP_XML_CONTENT_4(sVal);
				}
				else if (!oReader.IsEmptyNode()) this->AddNode(tmp);
			}
		}
	}
public:
	xml_writer4& _xml_wr_4;
	XmlUtils::CXmlLiteReader& oReader;
};

class ReadAndStringBuild
{
public:
	ReadAndStringBuild(XmlUtils::CXmlLiteReader& R, NSStringUtils::CStringBuilder& B) : oReader(R), oBuilder(B) {};
	~ReadAndStringBuild() {};
	void AddNode(int depth)
	{
		while (oReader.ReadNextSiblingNode(depth))
		{
			int tmp = oReader.GetDepth();
			std::wstring sName = oReader.GetName();
			oBuilder.WriteString(L"<");
			oBuilder.WriteEncodeXmlString(sName);
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					std::wstring sNameAttr = oReader.GetName();
					std::wstring sAttr = oReader.GetText();
					oBuilder.WriteString(L" ");
					oBuilder.WriteEncodeXmlString(sNameAttr);
					oBuilder.WriteString(L"=\"");
					oBuilder.WriteEncodeXmlString(sAttr);
					oBuilder.WriteString(L"\"");
					for (int i = 1; i < na; i++)
					{
						if (oReader.MoveToNextAttribute())
						{
							sNameAttr = oReader.GetName();
							sAttr = oReader.GetText();
							oBuilder.WriteString(L" ");
							oBuilder.WriteEncodeXmlString(sNameAttr);
							oBuilder.WriteString(L"=\"");
							oBuilder.WriteEncodeXmlString(sAttr);
							oBuilder.WriteString(L"\"");
						}
					}
					oReader.MoveToElement();
				}
			}
			if (oReader.IsEmptyNode()) oBuilder.WriteString(L"/>");
			else oBuilder.WriteString(L">");
			if ((sName == L"w:t" || sName == L"w:instrText" || sName == L"m:t" || sName == L"wp:posOffset" || sName == L"w14:pctHeight" || sName == L"w14:pctWidth" || sName == L"wp:align") && !oReader.IsEmptyNode())
			{
				std::wstring sVal = oReader.GetText2();
				oBuilder.WriteEncodeXmlString(sVal);
				oBuilder.WriteString(L"</");
				oBuilder.WriteEncodeXmlString(sName);
				oBuilder.WriteString(L">");
			}
			else if (!oReader.IsEmptyNode())
			{
				this->AddNode(tmp);
				oBuilder.WriteString(L"</");
				oBuilder.WriteEncodeXmlString(sName);
				oBuilder.WriteString(L">");
			}
		}
	}
public:
	XmlUtils::CXmlLiteReader& oReader;
	NSStringUtils::CStringBuilder& oBuilder;
};

TEST_F(BufferTest, main_test_read_and_write_xml_with_buffer_and_sstream)
{
	std::wstring filename(__argv[1], __argv[1] + strlen(__argv[1]));
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(filename))
		return;
	if (!oReader.ReadNextNode())
		return;
	
	int n = oReader.GetDepth();
	std::wstring sName = oReader.GetName();
	
	//Reading and writing an XML doc via buffer
	auto begin1 = std::chrono::steady_clock::now();
	xml::CBufferXml3 outputbuffer;
	CP_XML_WRITER_4(outputbuffer)
	{
		CP_XML_NODE_4(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite4 rnw(_xml_wr_4, oReader);
			rnw.AddNode(n);
		}
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);

	if (!oReader.MoveToStart())
		return;
	if (!oReader.ReadNextNode())
		return;
	
	sName = oReader.GetName();
	n = oReader.GetDepth();
	
	//Reading and writing an XML doc via sstream
	auto begin2 = std::chrono::steady_clock::now();
	std::wstringstream outputstream;
	CP_XML_WRITER(outputstream)
	{
		CP_XML_NODE(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite rnw(_xml_wr_, oReader);
			rnw.AddNode(n);
		}
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2);
	EXPECT_EQ(outputstream.str(), outputbuffer.str());
	std::string res;
	std::wstring out = outputstream.str();
	utf8::utf16to8(out.begin(), out.end(), std::back_inserter(res));
	NSFile::CFileBinary file;
	if (file.CreateFileW(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(__argv[3])) == true)
	{
		std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
		file.WriteFile((BYTE*)root.c_str(), root.length());
		file.WriteFile((BYTE*)res.c_str(), res.length());
		file.CloseFile();
	}
	
	std::cout << '\n' << "Reading and writing an XML doc via buffer time - "  << elapsed_ms1.count() / 1000 / 60 << "(min)" << elapsed_ms1.count() / 1000 % 60 << "(s)" << elapsed_ms1.count() % 1000 << "(ms)" << '\n' << "Reading and writing an XML doc via sstream time - " << elapsed_ms2.count() / 1000 / 60 << "(min)" << elapsed_ms2.count() / 1000 % 60 << "(s)" << elapsed_ms2.count() % 1000 << "(ms)" << '\n';
}

TEST_F(BufferTest, test_stringstream_operator)
{
	std::wstring filename(__argv[1], __argv[1] + strlen(__argv[1]));
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(filename))
		return;
	if (!oReader.ReadNextNode())
		return;

	int n = oReader.GetDepth();
	std::wstring sName = oReader.GetName();

	//Reading and writing an XML doc via sstream
	std::wstringstream outputstream;
	CP_XML_WRITER(outputstream)
	{
		CP_XML_NODE(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite rnw(_xml_wr_, oReader);
			rnw.AddNode(n);
		}
	}

	auto begin1 = std::chrono::steady_clock::now();
	std::wstringstream outputstream1;
	for (size_t i = 0; i < 10; i++)
	{
		outputstream1 << outputstream.str();
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_mcs1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	std::cout << "Time of adding two streams into one " << elapsed_mcs1.count() << "(mcs)" << '\n';
}

TEST_F(BufferTest, test_buffer_operators_sum_and_eq)
{
	std::wstring filename(__argv[2], __argv[2] + strlen(__argv[2]));
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(filename))
		return;
	if (!oReader.ReadNextNode())
		return;

	int n = oReader.GetDepth();
	std::wstring sName = oReader.GetName();

	//Reading and writing an XML doc via buffer
	xml::CBufferXml3 outputbuffer1;
	CP_XML_WRITER_4(outputbuffer1)
	{
		CP_XML_NODE_4(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite4 rnw(_xml_wr_4, oReader);
			rnw.AddNode(n);
		}
	}

	std::wstring filename1(__argv[1], __argv[1] + strlen(__argv[1]));
	if (!oReader.FromFile(filename1))
		return;
	if (!oReader.ReadNextNode())
		return;

	n = oReader.GetDepth();
	sName = oReader.GetName();

	//Reading and writing an XML doc via buffer
	xml::CBufferXml3 outputbuffer2;
	CP_XML_WRITER_4(outputbuffer2)
	{
		CP_XML_NODE_4(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite4 rnw(_xml_wr_4, oReader);
			rnw.AddNode(n);
		}
	}

	auto begin1 = std::chrono::steady_clock::now();
	for (size_t i = 0; i < 10; i++)
	{
		outputbuffer1 = outputbuffer1 + outputbuffer2;
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_mcs1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);
	
	std::string res = outputbuffer1.utf8();
	NSFile::CFileBinary file;
	if (file.CreateFileW(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(__argv[4])) == true)
	{
		std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
		file.WriteFile((BYTE*)root.c_str(), root.length());
		file.WriteFile((BYTE*)res.c_str(), res.length());
		file.CloseFile();
	}
	
	std::cout << "Time of adding two buffers into one " << elapsed_mcs1.count() << "(mcs)" << '\n';
}

TEST_F(BufferTest, test_buffer_operator_add)
{
	std::wstring filename(__argv[2], __argv[2] + strlen(__argv[2]));
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(filename))
		return;
	if (!oReader.ReadNextNode())
		return;

	int n = oReader.GetDepth();
	std::wstring sName = oReader.GetName();

	//Reading and writing an XML doc via buffer
	xml::CBufferXml3 outputbuffer1;
	CP_XML_WRITER_4(outputbuffer1)
	{
		CP_XML_NODE_4(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite4 rnw(_xml_wr_4, oReader);
			rnw.AddNode(n);
		}
	}

	std::wstring filename1(__argv[1], __argv[1] + strlen(__argv[1]));
	if (!oReader.FromFile(filename1))
		return;
	if (!oReader.ReadNextNode())
		return;

	n = oReader.GetDepth();
	sName = oReader.GetName();

	//Reading and writing an XML doc via buffer
	xml::CBufferXml3 outputbuffer2;
	CP_XML_WRITER_4(outputbuffer2)
	{
		CP_XML_NODE_4(sName)
		{
			int na = oReader.GetAttributesCount();
			if (na > 0)
			{
				if (oReader.MoveToFirstAttribute())
				{
					CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					for (int i = 1; i < na; i++)
						if (oReader.MoveToNextAttribute())
							CP_XML_ATTR2_4(oReader.GetName(), oReader.GetText());
					oReader.MoveToElement();
				}
			}
			ReadAndWrite4 rnw(_xml_wr_4, oReader);
			rnw.AddNode(n);
		}
	}

	auto begin2 = std::chrono::steady_clock::now();
	for (size_t i = 0; i < 10; i++)
	{
		outputbuffer1 += outputbuffer2;
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_mcs2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);
	
	NSFile::CFileBinary file;
	std::string res = outputbuffer1.utf8();
	if (file.CreateFileW(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(__argv[5])) == true)
	{
		std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
		file.WriteFile((BYTE*)root.c_str(), root.length());
		file.WriteFile((BYTE*)res.c_str(), res.length());
		file.CloseFile();
	}
	
	std::cout << "Time of adding the second buffer to the first one " << elapsed_mcs2.count() << "(mcs)" << '\n';

}

TEST_F(BufferTest, test_StringBuild_time)
{
	//Reading and writing an XML doc via StringBuild
	std::wstring filename(__argv[1], __argv[1] + strlen(__argv[1]));
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(filename))
		return;
	if (!oReader.ReadNextNode())
		return;

	int n = oReader.GetDepth();
	std::wstring sName = oReader.GetName();
	auto begin1 = std::chrono::steady_clock::now();
	NSStringUtils::CStringBuilder oBuilder;
	oBuilder.WriteString(L"<");
	oBuilder.WriteEncodeXmlString(sName);
	int na = oReader.GetAttributesCount();
	if (na > 0)
	{
		if (oReader.MoveToFirstAttribute())
		{
			std::wstring sNameAttr = oReader.GetName();
			std::wstring sAttr = oReader.GetText();
			oBuilder.WriteString(L" ");
			oBuilder.WriteEncodeXmlString(sNameAttr);
			oBuilder.WriteString(L"=\"");
			oBuilder.WriteEncodeXmlString(sAttr);
			oBuilder.WriteString(L"\"");
			for (int i = 1; i < na; i++)
			{
				if (oReader.MoveToNextAttribute())
				{
					sNameAttr = oReader.GetName();
					sAttr = oReader.GetText();
					oBuilder.WriteString(L" ");
					oBuilder.WriteEncodeXmlString(sNameAttr);
					oBuilder.WriteString(L"=\"");
					oBuilder.WriteEncodeXmlString(sAttr);
					oBuilder.WriteString(L"\"");
				}
			}
			oReader.MoveToElement();
		}
	}
	if (oReader.IsEmptyNode()) oBuilder.WriteString(L"/>");
	else oBuilder.WriteString(L">");
	ReadAndStringBuild abc(oReader, oBuilder);
	abc.AddNode(n);
	oBuilder.WriteString(L"</");
	oBuilder.WriteEncodeXmlString(sName);
	oBuilder.WriteString(L">");
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);
	
	std::string res;
	std::wstring out = oBuilder.GetData();
	utf8::utf16to8(out.begin(), out.end(), std::back_inserter(res));
	NSFile::CFileBinary file;
	if (file.CreateFileW(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(__argv[6])) == true)
	{
		std::string root = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
		file.WriteFile((BYTE*)root.c_str(), root.length());
		file.WriteFile((BYTE*)res.c_str(), res.length());
		file.CloseFile();
	}
	
	std::cout << '\n' << "Reading and writing an XML doc via StringBuild - " << elapsed_ms1.count() / 1000 / 60 << "(min)" << elapsed_ms1.count() / 1000 % 60 << "(s)" << elapsed_ms1.count() % 1000 << "(ms)" << '\n';
}

TEST_F(BufferTest, test_stringbuild_operator)
{
	//Reading and writing an XML doc via StringBuild
	std::wstring filename(__argv[1], __argv[1] + strlen(__argv[1]));
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(filename))
		return;
	if (!oReader.ReadNextNode())
		return;

	int n = oReader.GetDepth();
	std::wstring sName = oReader.GetName();
	NSStringUtils::CStringBuilder oBuilder;
	oBuilder.WriteString(L"<");
	oBuilder.WriteEncodeXmlString(sName);
	int na = oReader.GetAttributesCount();
	if (na > 0)
	{
		if (oReader.MoveToFirstAttribute())
		{
			std::wstring sNameAttr = oReader.GetName();
			std::wstring sAttr = oReader.GetText();
			oBuilder.WriteString(L" ");
			oBuilder.WriteEncodeXmlString(sNameAttr);
			oBuilder.WriteString(L"=\"");
			oBuilder.WriteEncodeXmlString(sAttr);
			oBuilder.WriteString(L"\"");
			for (int i = 1; i < na; i++)
			{
				if (oReader.MoveToNextAttribute())
				{
					sNameAttr = oReader.GetName();
					sAttr = oReader.GetText();
					oBuilder.WriteString(L" ");
					oBuilder.WriteEncodeXmlString(sNameAttr);
					oBuilder.WriteString(L"=\"");
					oBuilder.WriteEncodeXmlString(sAttr);
					oBuilder.WriteString(L"\"");
				}
			}
			oReader.MoveToElement();
		}
	}
	if (oReader.IsEmptyNode()) oBuilder.WriteString(L"/>");
	else oBuilder.WriteString(L">");
	ReadAndStringBuild abc(oReader, oBuilder);
	abc.AddNode(n);
	oBuilder.WriteString(L"</");
	oBuilder.WriteEncodeXmlString(sName);
	oBuilder.WriteString(L">");

	NSStringUtils::CStringBuilder oBuilder1;
	auto begin1 = std::chrono::steady_clock::now();
	for (size_t i = 0; i < 10; i++)
	{
		oBuilder1.Write(oBuilder);
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_mcs1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	std::cout << "Time of adding two StringBuild into one " << elapsed_mcs1.count() << "(mcs)" << '\n';
}

TEST_F(BufferTest, test_buffer_and_sstream_write_node)
{
	//Writing an XML node via buffer
	auto begin1 = std::chrono::steady_clock::now();
	xml::CBufferXml3 outputbuffer;
	CP_XML_WRITER_4(outputbuffer)
	{
		CP_XML_NODE_4(L"NameNode")
		{
			CP_XML_ATTR_4(L"NameString", data);
		}
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	//Writing an XML node via sstream
	auto begin2 = std::chrono::steady_clock::now();
	std::wstringstream outputstream;
	CP_XML_WRITER(outputstream)
	{
		CP_XML_NODE(L"NameNode")
		{
			CP_XML_ATTR(L"NameString", data);
		}
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);

	EXPECT_EQ(outputstream.str(), outputbuffer.str());
	
	std::cout << '\n' << "Writing an XML node via buffer time - " << elapsed_ms1.count() << "(ms)" << '\n' << "Writing an XML node via sstream time - " << elapsed_ms2.count() << "(ms)" << '\n';
}

TEST_F(BufferTest, test_buffer_and_sstream_write_two_nodes_whith_content)
{
	//Writing an two XML node whith content via buffer
	auto begin1 = std::chrono::steady_clock::now();
	xml::CBufferXml3 outputbuffer;
	CP_XML_WRITER_4(outputbuffer)
	{
		CP_XML_NODE_4(L"NameNode")
		{
			CP_XML_ATTR_4(L"NameString", L"data");

			CP_XML_NODE_4(L"NameNodeTwo")
			{
				for (size_t i = 0; i < 100; i++)
				{
					CP_XML_CONTENT_4(static_cast<wchar_t>(i));
				}
			}
		}
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	//Writing an two XML node whith content via sstream
	auto begin2 = std::chrono::steady_clock::now();
	std::wstringstream outputstream;
	CP_XML_WRITER(outputstream)
	{
		CP_XML_NODE(L"NameNode")
		{
			CP_XML_ATTR(L"NameString", L"data");

			CP_XML_NODE(L"NameNodeTwo")
			{
				for (size_t i = 0; i < 100; i++)
				{
					CP_XML_CONTENT(static_cast<wchar_t>(i));
				}
			}
		}
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);

	EXPECT_EQ(outputstream.str(), outputbuffer.str());
	std::cout << '\n' << "Writing an XML node via buffer time - " << elapsed_ms1.count() << "(ms)" << '\n' << "Writing an XML node via sstream time - " << elapsed_ms2.count() << "(ms)" << '\n';
}

TEST_F(BufferTest, test_buffer_and_sstream_write_some_node_with_some_content)
{
	//Writing an some XML node whith some content via buffer
	auto begin1 = std::chrono::steady_clock::now();
	xml::CBufferXml3 outputbuffer;
	CP_XML_WRITER_4(outputbuffer)
	{
		CP_XML_NODE_4(L"NameNode")
		{
			CP_XML_ATTR_4(L"NameString", data);

			CP_XML_NODE_4(L"NameNodeTwo")
			{
				for (size_t i = 0; i < 100; i++)
				{
					CP_XML_CONTENT_4(static_cast<wchar_t>(i));
				}
			}
		}
		CP_XML_NODE_4(L"NameNewNode")
		{
			CP_XML_ATTR_4(L"NameString1", data1);

			CP_XML_NODE_4(L"NameNextNode")
			{
				CP_XML_ATTR_4(L"NameString2", data2);
			}
			CP_XML_NODE_4(L"NameTwoNextNode")
			{
				CP_XML_CONTENT_4(L"1234567890");
			}
		}
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	//Writing an some XML node whith some content via stream
	auto begin2 = std::chrono::steady_clock::now();
	std::wstringstream outputstream;
	CP_XML_WRITER(outputstream)
	{
		CP_XML_NODE(L"NameNode")
		{
			CP_XML_ATTR(L"NameString", data);

			CP_XML_NODE(L"NameNodeTwo")
			{
				for (size_t i = 0; i < 100; i++)
				{
					CP_XML_CONTENT(static_cast<wchar_t>(i));
				}
			}
		}
		CP_XML_NODE(L"NameNewNode")
		{
			CP_XML_ATTR(L"NameString1", data1);

			CP_XML_NODE(L"NameNextNode")
			{
				CP_XML_ATTR(L"NameString2", data2);
			}
			CP_XML_NODE(L"NameTwoNextNode")
			{
				CP_XML_CONTENT(L"1234567890");
			}
		}
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);

	EXPECT_EQ(outputstream.str(), outputbuffer.str());

	std::cout << '\n' << "Writing an XML node via buffer time - " << elapsed_ms1.count() << "(ms)" << '\n' << "Writing an XML node via sstream time - " << elapsed_ms2.count() << "(ms)" << '\n';
}

TEST_F(BufferTest, test_buffer_and_sstream_write_xml)
{
	//Writing an some XML node whith some content via buffer
	xml::CBufferXml3 outputbuffer;
	auto begin1 = std::chrono::steady_clock::now();
	CP_XML_WRITER_4(outputbuffer)
	{
		CP_XML_NODE_4(L"vt:vector")
		{
			CP_XML_ATTR_4(L"size", L"200");
			CP_XML_ATTR_4(L"baseType", L"variant");

			for (size_t i = 0; i < data2.size(); ++i)
			{
				CP_XML_NODE_4(L"vt:variant")
				{
					CP_XML_NODE_4(L"vt:lpstr")
					{
						CP_XML_CONTENT_4(data2[i]);
					}
				}
			}
		}
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	//Writing an some XML node whith some content via stream
	auto begin2 = std::chrono::steady_clock::now();
	std::wstringstream outputstream;
	CP_XML_WRITER(outputstream)
	{
		CP_XML_NODE(L"vt:vector")
		{
			CP_XML_ATTR(L"size", L"200");
			CP_XML_ATTR(L"baseType", L"variant");

			for (size_t i = 0; i < data2.size(); ++i)
			{
				CP_XML_NODE(L"vt:variant")
				{
					CP_XML_NODE(L"vt:lpstr")
					{
						CP_XML_CONTENT(data2[i]);
					}
				}
			}
		}
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);

	EXPECT_EQ(outputstream.str(), outputbuffer.str());

	std::cout << '\n' << "Writing an XML node via buffer time - " << elapsed_ms1.count() << "(ms)" << '\n' << "Writing an XML node via sstream time - " << elapsed_ms2.count() << "(ms)" << '\n';
}