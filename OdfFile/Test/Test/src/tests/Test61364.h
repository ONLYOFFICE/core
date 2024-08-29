#pragma once

#include "../Common.h"

class TestEnv61364 : public ODT2DOCX_ConversionEnvironment
{
public:
	TestEnv61364();
};

class Test61364 : public testing::Test
{
public:
	void SetUp() override;

	OOX::CDocx* Docx;
};
