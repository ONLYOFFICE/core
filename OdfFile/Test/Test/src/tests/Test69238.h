#pragma once

#include "../Common.h"

class TestEnv69238 : public PPTX2ODP_ConversionEnvironment
{
public:
	TestEnv69238();
};

class Test69238 : public testing::Test
{
public:
	void SetUp() override;

	cpdoccore::odf_reader::odf_document* Odf;
};
