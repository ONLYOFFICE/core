#include "../odf/precompiled_cpodf.h"
#include "namespaces.h"

namespace cpdoccore { 
namespace oox {
namespace xmlns {

const office_xmlns rels = {L"", L"http://schemas.openxmlformats.org/package/2006/relationships"};
const office_xmlns types = {L"", L"http://schemas.openxmlformats.org/package/2006/content-types"};

const office_xmlns ve = {L"ve", L"http://schemas.openxmlformats.org/markup-compatibility/2006"};
const office_xmlns o = {L"o", L"urn:schemas-microsoft-com:office:office"};
const office_xmlns r = {L"r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"};
const office_xmlns m = {L"m", L"http://schemas.openxmlformats.org/officeDocument/2006/math"};
const office_xmlns v = {L"v", L"urn:schemas-microsoft-com:vml"};
const office_xmlns wp = {L"wp", L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"};
const office_xmlns w10 = {L"w10", L"urn:schemas-microsoft-com:office:word"};
const office_xmlns w = {L"w", L"http://schemas.openxmlformats.org/wordprocessingml/2006/main"};
const office_xmlns wne = {L"wne", L"http://schemas.microsoft.com/office/word/2006/wordml"};

office_xmlns const * const word_ns[] = { &ve, &o, &r, &m, &v, &wp, &w10, &w, &wne };

}
}
}
