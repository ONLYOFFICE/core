#pragma once

#include "../Common.h"

class TestEnv44363 : public ODT2DOCX_ConversionEnvironment
{
public:
	TestEnv44363();
};

class Test44363 : public testing::Test
{
public:
	void SetUp() override;

	OOX::CDocx* Docx;
};
