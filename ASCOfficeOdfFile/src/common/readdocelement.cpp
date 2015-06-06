
#include "../../include/cpdoccore/common/readdocelement.h"
#include "../../include/cpdoccore/xml/attributes.h"
#include "../../include/cpdoccore/xml/sax.h"

namespace cpdoccore { 
namespace common {

bool read_doc_element::read_sax( xml::sax * Reader )
{
	const unsigned int currentDepth = Reader->depth();
   
	xml::attributes_wc_ptr attrb = xml::read_attributes( Reader );
    add_attributes( attrb );
    
    if (!attrb->check())
    {
        //std::wcerr << " -- attr check error (" << Reader->nodeQualifiedName() << ")\n";
    }

	//const std::wstring value = Reader->value();

	//if (value.length() > 0)
	//{
	//	add_text(value);
	//}

    xml::NodeType nodeType = Reader->nodeType();
	
	if (Reader->isEmptyElement())
	{
		return false;
	}


    while (true)
	{
		nodeType = Reader->next(currentDepth);
		if (nodeType == xml::typeEOF || nodeType == xml::typeNone ) break;
       
		if (Reader->depth() <= currentDepth + 1 && nodeType == xml::typeEndElement) break;
        
		switch( nodeType )
        {
			case xml::typeElement:
				{
					const std::wstring namespacePrefix	= Reader->namespacePrefix();
					const std::wstring localName		= Reader->nodeLocalName();
					
					add_child_element(Reader, namespacePrefix, localName);
				}
				break;
			case xml::typeWhitespace:
			case xml::typeText:
				{
					const std::wstring value = Reader->value();
					add_text(value);
				}
				break;
        }
    }
    return true;
}

}
}
