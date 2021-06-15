#ifndef CEMFINTERPRETARORXML_H
#define CEMFINTERPRETARORXML_H

//#include "CEmfInterpretarorBase.h"
#include "../../Common/IOutputDevice.h"
#include "../../CXmlOutput.h"

namespace MetaFile
{
    class CEmfInterpretarorXml /*: public IOutputDevice*/
    {
    public:
        CEmfInterpretarorXml();
        virtual ~CEmfInterpretarorXml();

        void Save_EMR_HEADER(const TEmfHeader& oTEmfHeader, const unsigned int &unValue, CDataStream &oDataStream);

    private:
        CXmlOutput *m_pOutputXml;
    };
}

#endif // CEMFINTERPRETARORXML_H
