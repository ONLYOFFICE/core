#pragma once
#include "../AVSOfficeHtmlFile/src/stdafx.h"

#pragma warning(push, 4)

#define SAFE_DELETE_POINTER(_P) \
(void)(_P != NULL ? delete _P, _P = NULL : 0) \

#pragma warning(pop)

