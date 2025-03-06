#pragma once

#include "../Common.h"

class TestEnv59708 : public DOCX2ODT_ConvertsionEnvironment
{
public:
	TestEnv59708();
};

class Test59708 : public testing::Test
{
public:
	void SetUp() override;

	cpdoccore::odf_reader::odf_document* Odf;
};
