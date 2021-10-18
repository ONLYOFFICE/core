#ifndef CEMFPLUSPARSER_H
#define CEMFPLUSPARSER_H

#include "../../Common/MetaFileUtils.h"

namespace MetaFile
{
        class CEmfParser;
        class CEmfPlusParser
        {
            public:
                CEmfPlusParser(CEmfParser *pEmfParser);
                void SetStream(BYTE *pBytes, unsigned int unSize);
                void PlayFile();
            private:
                void Read_EMRPLUS_OFFSETCLIP();
                void Read_EMRPLUS_RESETCLIP();
                void Read_EMFPLUS_SETCLIPPATH(unsigned short unShFlags);
                void Read_EMFPLUS_SETCLIPRECT(unsigned short unShFlags);
                void Read_EMFPLUS_SETCLIPREGION(unsigned short unShFlags);

                void Read_EMFPLUS_COMMENT(unsigned int unDataSize);

                void Read_EMFPLUS_ENDOFFILE();
                void Read_EMFPLUS_GETDC();
                void Read_EMRPLUS_HEADER(unsigned short unShFlags);

                void Read_EMFPLUS_CLEAR();
                void Read_EMFPLUS_DRAWARC(unsigned short unShFlags);
                template<typename T>void Read_EMFPLUS_DRAWARC_BASE();
                void Read_EMFPLUS_DRAWBEZIERS(unsigned short unShFlags);
                template<typename T>void Read_EMFPLUS_DRAWBEZIERS_BASE();
                void Read_EMFPLUS_DRAWCLOSEDCURVE(unsigned short unShFlags);
                template<typename T>void Read_EMFPLUS_DRAWCLOSEDCURVE_BASE();

                void Read_EMRPLUS_SETTEXTRENDERINGHINT(unsigned short unShFlags);
                CEmfParser *m_pEmfParser;
                CDataStream m_oDataStream;
        };
}

#endif // CEMFPLUSPARSER_H
