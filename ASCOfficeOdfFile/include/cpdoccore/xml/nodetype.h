#pragma once

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

