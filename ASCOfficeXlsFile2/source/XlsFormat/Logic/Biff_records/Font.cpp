#include "precompiled_xls.h"
#include "Font.h"

#include <simple_xml_writer.h>

namespace XLS
{;

Font::Font()
{
}


Font::~Font()
{
}


BaseObjectPtr Font::clone()
{
	return BaseObjectPtr(new Font(*this));
}


void Font::writeFields(CFRecord& record)
{
	unsigned short flags = 0;
	SETBIT(flags, 1, fItalic);
	SETBIT(flags, 3, fStrikeOut);
	SETBIT(flags, 4, fOutline);
	SETBIT(flags, 5, fShadow);
	SETBIT(flags, 6, fCondense);
	SETBIT(flags, 7, fExtend);

	record << dyHeight << flags;
	record << icv << bls << sss << uls << bFamily << bCharSet;
	record.reserveNunBytes(1, static_cast<unsigned char>(0x5E));
	record << fontName;
}


void Font::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> dyHeight >> flags;
	fItalic = GETBIT(flags, 1);
	fStrikeOut = GETBIT(flags, 3);
	fOutline = GETBIT(flags, 4);
	fShadow = GETBIT(flags, 5);
	fCondense = GETBIT(flags, 6);
	fExtend = GETBIT(flags, 7);

	record >> icv >> bls >> sss >> uls >> bFamily >> bCharSet;
	record.skipNunBytes(1);
	record >> fontName;
}

int Font::serialize(std::wostream & stream)
{
    CP_XML_WRITER(stream)    
    {
        CP_XML_NODE(L"font")
        {
            if ((bls.value()) && (*bls.value() == 700))
            {
                CP_XML_NODE(L"b")
                {
                    CP_XML_ATTR(L"val", true);
                }            
            }

            if (bCharSet.value())
            {
                CP_XML_NODE(L"charset")
                {
                    CP_XML_ATTR(L"val", *bCharSet.value());
                }
            }

            if (icv.value())
			{
                CP_XML_NODE(L"color")
                {
                    CP_XML_ATTR(L"indexed", *icv.value());
                }
			}

            if ((fCondense.value()) && (*fCondense.value()))
            {
                CP_XML_NODE(L"condense")
                {
                    CP_XML_ATTR(L"val", (int)(*fCondense.value()));
                }
            }

			if ((fExtend.value()) && (*fExtend.value()))
            {
                CP_XML_NODE(L"extend")
                {
                    CP_XML_ATTR(L"val", (int)(*fExtend.value()));
                }
            }

            if (bFamily.value())
            {
                CP_XML_NODE(L"family")
                {
                    CP_XML_ATTR(L"val", (int)(*bFamily.value()));
                }
            }

            if ((fItalic.value()) && (*fItalic.value()))
            {
                CP_XML_NODE(L"i")
                {
                    CP_XML_ATTR(L"val", (int)(*fItalic.value()));
                }
            }

			if (!fontName.value().empty())
            {
                CP_XML_NODE(L"name")
                {
                    CP_XML_ATTR(L"val", fontName.value());
                }
            }

			if (fOutline.value())
            {
                CP_XML_NODE(L"outline")
                {
                    CP_XML_ATTR(L"val", (int)(*fOutline.value()));
                }
            }

            //if (font.scheme)
            //{
            //    CP_XML_NODE(L"scheme")
            //    {
            //        CP_XML_ATTR(L"val", *font.scheme);
            //    }
            //}

			if ((fShadow.value()) && (*fShadow.value()))
            {
                CP_XML_NODE(L"shadow")
                {
					CP_XML_ATTR(L"val", (int)(*fShadow.value()));
                }
            }

            if ((fStrikeOut.value()) && (*fStrikeOut.value()))
            {
                CP_XML_NODE(L"strike")
                {
                    CP_XML_ATTR(L"val", (int)(*fStrikeOut.value()));
                }
            }

            if (dyHeight.value())
            {
                CP_XML_NODE(L"sz")
                {
                    CP_XML_ATTR(L"val", *dyHeight.value()/20);
                }
            }

            if ((uls.value()) && (*uls.value() > 0))
            {
                CP_XML_NODE(L"u")
                {
					switch(*uls.value())
					{
						case 1:		CP_XML_ATTR(L"val", "single");break;
						case 2:		CP_XML_ATTR(L"val", "double");break;
						case 33:	CP_XML_ATTR(L"val", "singleAccounting");break;
						case 34:	CP_XML_ATTR(L"val", "doubleAccounting");break;
					}
                }
            }

            if ((sss.value()) && (*sss.value() > 0))
            {
                CP_XML_NODE(L"vertAlign")
                {
 					switch(*sss.value())
					{
						case 1:	CP_XML_ATTR(L"val", L"superscript");break;
						case 2:	CP_XML_ATTR(L"val", L"subscript");break;
					}
                   
                }
            }
			}
            
        }
		return 0;
	}

} // namespace XLS

