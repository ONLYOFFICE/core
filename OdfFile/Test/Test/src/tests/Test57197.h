#pragma once

#include "../Common.h"

class TestEnv57197 : public ODT2DOCX_ConversionEnvironment
{
public:
	TestEnv57197();
};

class Test57197 : public testing::Test
{
public:
	void SetUp() override;

	OOX::CDocx* Docx;
};
