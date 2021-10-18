#include "CEmfPlusParser.h"
#include "../EmfPlusTypes.h"

#define EMRPLUS_OFFSETCLIP              0x4035
#define EMRPLUS_RESETCLIP               0x4031
#define EMFPLUS_SETCLIPPATH             0x4033
#define EMFPLUS_SETCLIPRECT             0x4032
#define EMFPLUS_SETCLIPREGION           0x4034

#define EMFPLUS_COMMENT                 0x4003

#define EMFPLUS_ENDOFFILE               0x4002
#define EMFPLUS_GETDC                   0x4004
#define EMFPLUS_HEADER                  0x4001

#define EMFPLUS_CLEAR                   0x4009
#define EMFPLUS_DRAWARC                 0x4012
#define EMFPLUS_DRAWBEZIERS             0x4019
#define EMFPLUS_DRAWCLOSEDCURVE         0x4017

#define EMFPLUS_SETTEXTRENDERINGHINT    0x401F

MetaFile::CEmfPlusParser::CEmfPlusParser(CEmfParser *pEmfParser) :
        m_pEmfParser(pEmfParser)
{
        m_oDataStream.SetStream(NULL, 0);
}

void MetaFile::CEmfPlusParser::SetStream(BYTE *pBytes, unsigned int unSize)
{
        m_oDataStream.SetStream(pBytes, unSize);
}

void MetaFile::CEmfPlusParser::PlayFile()
{
        if (NULL == m_pEmfParser || m_oDataStream.CanRead() < 28)
                return;

        do
        {
                unsigned short unShType, unShFlags;
                unsigned int unSize, unDataSize;

                m_oDataStream >> unShType;
                m_oDataStream >> unShFlags;

                m_oDataStream >> unSize;
                m_oDataStream >> unDataSize;

                switch (unShType)
                {
                        //Clipping Record Types (Типы записей отсечения)
                        case EMRPLUS_OFFSETCLIP:    Read_EMRPLUS_OFFSETCLIP();              break;
                        case EMRPLUS_RESETCLIP:     Read_EMRPLUS_RESETCLIP();               break;
                        case EMFPLUS_SETCLIPPATH:   Read_EMFPLUS_SETCLIPPATH(unShFlags);    break;
                        case EMFPLUS_SETCLIPRECT:   Read_EMFPLUS_SETCLIPRECT(unShFlags);    break;
                        case EMFPLUS_SETCLIPREGION: Read_EMFPLUS_SETCLIPREGION(unShFlags);  break;

                        //Comment Record Types (Типы записей комментариев)
                        case EMFPLUS_COMMENT: Read_EMFPLUS_COMMENT(unDataSize); break;

                        //Control Record Types (Типы управляющих записей)
                        case EMFPLUS_ENDOFFILE: Read_EMFPLUS_ENDOFFILE();       break;
                        case EMFPLUS_GETDC:     Read_EMFPLUS_GETDC();           break;
                        case EMFPLUS_HEADER:    Read_EMRPLUS_HEADER(unShFlags); break;

                        //Drawing Record Types (Типы записей чертежа)
                        case EMFPLUS_CLEAR:             Read_EMFPLUS_CLEAR();                   break;
                        case EMFPLUS_DRAWARC:           Read_EMFPLUS_DRAWARC(unShFlags);        break;
                        case EMFPLUS_DRAWBEZIERS:       Read_EMFPLUS_DRAWBEZIERS(unShFlags);    break;
                        case EMFPLUS_DRAWCLOSEDCURVE:   Read_EMFPLUS_DRAWCLOSEDCURVE(unShFlags);break;

                        case EMFPLUS_SETTEXTRENDERINGHINT: Read_EMRPLUS_SETTEXTRENDERINGHINT(unShFlags); break;
                        default: m_oDataStream.Skip(unSize - 12);     break;
                }
        }while(m_oDataStream.CanRead() > 4);
}

void MetaFile::CEmfPlusParser::Read_EMRPLUS_HEADER(unsigned short unShFlags)
{
        m_oDataStream.Skip(4); //Version

        unsigned int unEmfPlusFlags, unLogicalDpiX, unLogicalDpiY;

        m_oDataStream >> unEmfPlusFlags;
        m_oDataStream >> unLogicalDpiX;
        m_oDataStream >> unLogicalDpiY;

        //TODO: добавить установление нового Dpi
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_CLEAR()
{
        TEmfPlusARGB oARGB;

        m_oDataStream >> oARGB;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWARC(unsigned short unShFlags)
{
        double dStartAngle, dSweepAngle;
        unsigned short unOgjectIndex;

        m_oDataStream >> dStartAngle;
        m_oDataStream >> dSweepAngle;

        unsigned char* arBytes = (unsigned char*)&unShFlags;

        if (NULL != arBytes)
                unOgjectIndex = (unsigned short)arBytes[0];

        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRect>();
        else
                Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRectF>();
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWARC_BASE()
{
        T oRect;
        m_oDataStream >> oRect;
        //TODO: реализовать
}


void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS(unsigned short unShFlags)
{
        unsigned short unOgjectIndex;

        unsigned char* arBytes = (unsigned char*)&unShFlags;

        if (NULL != arBytes)
                unOgjectIndex = (unsigned short)arBytes[0];

        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointR>(); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPoint>();  // относительное расположение
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointF>(); // абсолютное расположение
        }

}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS_BASE()
{
        unsigned int unCountPoints;

        m_oDataStream >> unCountPoints;

        std::vector<T> arPoints;

        for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
        {

        }

        //TODO: реализовать
        //TODO: для EmfPlusPointR опредилиться с реализацией
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE(unsigned short unShFlags)
{
        unsigned short unOgjectIndex;

        unsigned char* arBytes = (unsigned char*)&unShFlags;

        if (NULL != arBytes)
                unOgjectIndex = (unsigned short)arBytes[0];

        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointR>(); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPoint>();  // абсолютное расположение с 16-разрядными координатами
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointF>(); // абсолютное расположение с 32-разрядными координатами.
        }
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE_BASE()
{
        double dTension;
        unsigned int unCountPoints;

        m_oDataStream >> dTension;
        m_oDataStream >> unCountPoints;

        std::vector<T> arPoints;

        for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
        {

        }
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_ENDOFFILE()
{
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_GETDC()
{
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMRPLUS_OFFSETCLIP()
{
        double dX, dY;

        m_oDataStream >> dX;
        m_oDataStream >> dY;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMRPLUS_RESETCLIP()
{
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETCLIPPATH(unsigned short unShFlags)
{
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETCLIPRECT(unsigned short unShFlags)
{
        TEmfPlusRectF oRect;

        m_oDataStream >> oRect;
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETCLIPREGION(unsigned short unShFlags)
{
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_COMMENT(unsigned int unDataSize)
{
        m_oDataStream.Skip(unDataSize);
}

void MetaFile::CEmfPlusParser::Read_EMRPLUS_SETTEXTRENDERINGHINT(unsigned short unShFlags)
{
        //TODO: реализовать
}
