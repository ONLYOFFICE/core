#ifndef _CPDOCCORE_XML_NODETYPE_H_
#define _CPDOCCORE_XML_NODETYPE_H_

#ifdef _MSC_VER
#pragma once
#endif

namespace cpdoccore {
namespace xml {

enum NodeType
{
    typeNone = 0,
    typeEOF,
    typeElement,
    typeAttribute,
    typeText,
    typeCDATA,
    typeProcessingInstruction,
    typeComment,
    typeDocumentType,
    typeWhitespace,
    typeEndElement,
    typeXmlDeclaration
};

}
}

#endif // #ifndef _CPDOCCORE_XML_NODETYPE_H_
