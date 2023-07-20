#pragma comment(lib, "../../../../../build/lib/win_64/debug/kernel.lib")
#pragma comment(lib, "../../../../../build/lib/win_64/debug/UnicodeConverter.lib")
#include <iostream>
#include <random>
#include <sstream>
#include <chrono>
#include "../../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../simple_xml_writer.h"
#include "../../simple_xml_writer2.h"

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
	ReadAndWrite(xml_writer& _xml_wr_, XmlUtils::CXmlLiteReader& oReader, int md) : _xml_wr_(_xml_wr_), oReader(oReader), maxdepth(md) {};
	~ReadAndWrite() {};
	void AddNode(int depth)
	{
		while (oReader.ReadNextSiblingNode(depth))
		{
			int tmp = oReader.GetDepth();
			CP_XML_NODE(oReader.GetName())
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
				if (depth == maxdepth - 1)
				{
					CP_XML_CONTENT(oReader.GetText2());
					if (oReader.MoveToElement()) this->AddNode(tmp);
				}	
				else this->AddNode(tmp);
			}
		}
	}
	
public:
	xml_writer& _xml_wr_;
	XmlUtils::CXmlLiteReader& oReader;
	int maxdepth;
};

class ReadAndWrite2
{
public:
	ReadAndWrite2(xml_writer2& _xml_wr_2, XmlUtils::CXmlLiteReader& oReader, int md) :  _xml_wr_2(_xml_wr_2), oReader(oReader), maxdepth(md) {};
	~ReadAndWrite2() {};
	void AddNode(int depth)
	{
		while (oReader.ReadNextSiblingNode(depth))
		{
			int tmp = oReader.GetDepth();
			CP_XML_NODE_2(oReader.GetName())
			{
				int na = oReader.GetAttributesCount();
				if (na > 0)
				{
					if (oReader.MoveToFirstAttribute())
					{
						CP_XML_ATTR2_2(oReader.GetName(), oReader.GetText());
						for (int i = 1; i < na; i++)
							if (oReader.MoveToNextAttribute())
								CP_XML_ATTR2_2(oReader.GetName(), oReader.GetText());
						oReader.MoveToElement();
					}
				}
				if (depth == maxdepth - 1)
				{
					CP_XML_CONTENT_2(oReader.GetText2());
					if (oReader.MoveToElement()) this->AddNode(tmp);
				}
				else this->AddNode(tmp);
			}
		}
	}
public:
	xml_writer2& _xml_wr_2;
	XmlUtils::CXmlLiteReader& oReader;
	int maxdepth;
};

TEST_F(BufferTest, main_test_read_and_write_xml_with_buffer_and_sstream)
{
	XmlUtils::CXmlLiteReader oReader;
	if (!oReader.FromFile(L"test.xml"))
		return;
	if (!oReader.ReadNextNode())
		return;

	int md = 0;
	while (oReader.ReadNextNode()) if (oReader.GetDepth() > md) md = oReader.GetDepth();
	if (!oReader.MoveToStart())
		return;
	if (!oReader.ReadNextNode())
		return;

	std::wstring sName = oReader.GetName();
	int n = oReader.GetDepth();
	//Reading and writing an XML doc via buffer
	auto begin1 = std::chrono::steady_clock::now();
	xml::CBufferXml<wchar_t> outputbuffer;
	CP_XML_WRITER_2(outputbuffer)
	{
		CP_XML_NODE_2(sName)
		{
			ReadAndWrite2 rnw(_xml_wr_2, oReader, md);
			rnw.AddNode(n);
		}
	}
	auto end1 = std::chrono::steady_clock::now();
	auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

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
			ReadAndWrite rnw(_xml_wr_, oReader, md);
			rnw.AddNode(n);
		}
	}
	auto end2 = std::chrono::steady_clock::now();
	auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);

	EXPECT_EQ(outputstream.str(), outputbuffer.str());

	std::cout << '\n' << "Reading and writing an XML doc via buffer time - " << elapsed_ms1.count() << "(ms)" << '\n' << "Reading and writing an XML doc via sstream time - " << elapsed_ms2.count() << "(ms)" << '\n';
}

TEST_F(BufferTest, test_buffer_and_sstream_write_node)
{
	//Writing an XML node via buffer
	auto begin1 = std::chrono::steady_clock::now();
	xml::CBufferXml<wchar_t> outputbuffer;
	CP_XML_WRITER_2(outputbuffer)
	{
		CP_XML_NODE_2(L"NameNode")
		{
			CP_XML_ATTR_2(L"NameString", data);
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
	xml::CBufferXml<wchar_t> outputbuffer;
	CP_XML_WRITER_2(outputbuffer)
	{
		CP_XML_NODE_2(L"NameNode")
		{
			CP_XML_ATTR_2(L"NameString", L"data");

			CP_XML_NODE_2(L"NameNodeTwo")
			{
				for (size_t i = 0; i < 100; i++)
				{
					CP_XML_CONTENT_2(static_cast<wchar_t>(i));
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
	xml::CBufferXml<wchar_t> outputbuffer;
	CP_XML_WRITER_2(outputbuffer)
	{
		CP_XML_NODE_2(L"NameNode")
		{
			CP_XML_ATTR_2(L"NameString", data);

			CP_XML_NODE_2(L"NameNodeTwo")
			{
				for (size_t i = 0; i < 100; i++)
				{
					CP_XML_CONTENT_2(static_cast<wchar_t>(i));
				}
			}
		}
		CP_XML_NODE_2(L"NameNewNode")
		{
			CP_XML_ATTR_2(L"NameString1", data1);

			CP_XML_NODE_2(L"NameNextNode")
			{
				CP_XML_ATTR_2(L"NameString2", data2);
			}
			CP_XML_NODE_2(L"NameTwoNextNode")
			{
				CP_XML_CONTENT_2(L"1234567890");
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
	xml::CBufferXml<wchar_t> outputbuffer;
	auto begin1 = std::chrono::steady_clock::now();
	CP_XML_WRITER_2(outputbuffer)
	{
		CP_XML_NODE_2(L"vt:vector")
		{
			CP_XML_ATTR_2(L"size", L"200");
			CP_XML_ATTR_2(L"baseType", L"variant");

			for (size_t i = 0; i < data2.size(); ++i)
			{
				CP_XML_NODE_2(L"vt:variant")
				{
					CP_XML_NODE_2(L"vt:lpstr")
					{
						CP_XML_CONTENT_2(data2[i]);
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