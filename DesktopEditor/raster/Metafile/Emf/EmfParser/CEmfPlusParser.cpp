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
#define EMFPLUS_DRAWCURVE               0x4018
#define EMFPLUS_DRAWDRIVERSTRING        0x4036
#define EMFPLUS_DRAWELLIPSE             0x400F
#define EMFPLUS_DRAWIMAGE               0x401A
#define EMFPLUS_DRAWIMAGEPOINTS         0x401B
#define EMFPLUS_DRAWLINES               0x400D
#define EMFPLUS_DRAWPATH                0x4015
#define EMFPLUS_DRAWPIE                 0x4011
#define EMFPLUS_DRAWRECTS               0x400B
#define EMFPLUS_DRAWSTRING              0x401C
#define EMFPLUS_FILLCLOSEDCURVE         0x4016
#define EMFPLUS_FILLELLIPSE             0x400E
#define EMFPLUS_FILLPATH                0x4014
#define EMFPLUS_FILLPIE                 0x4010
#define EMFPLUS_FILLPOLYGON             0x400C
#define EMFPLUS_FILLRECTS               0x400A
#define EMFPLUS_FILLREGION              0x4013

#define EMFPLUS_OBJECT                  0x4008
#define EMFPLUS_SERIALIZABLEOBJECT      0x4038

#define EMFPLUS_SETANTIALIASMODE        0x401E
#define EMFPLUS_SETCOMPOSITINGMODE      0x4023
#define EMFPLUS_SETCOMPOSITINGQUALITY   0x4024
#define EMFPLUS_SETINTERPOLATIONMODE    0x4021
#define EMFPLUS_SETPIXELOFFSETMODE      0x4022
#define EMFPLUS_SETRENDERINGORIGIN      0x401D
#define EMFPLUS_SETTEXTCONTRAST         0x4020
#define EMFPLUS_SETTEXTRENDERINGHINT    0x401F

#define EMFPLUS_BEGINCONTAINER          0x4027
#define EMFPLUS_BEGINCONTAINERNOPARAMS  0x4028
#define EMFPLUS_ENDCONTAINER            0x4029
#define EMFPLUS_RESTORE                 0x4026
#define EMFPLUS_SAVE                    0x4025

#define EMFPLUS_SETTSCLIP               0x403A
#define EMFPLUS_SETTSGRAPHICS           0x4039

#define EMFPLUS_MULTIPLYWORLDTRANSFORM  0x402C
#define EMFPLUS_RESETWORLDTRANSFORM     0x402B
#define EMFPLUS_ROTATEWORLDTRANSFORM    0x402F
#define EMFPLUS_SCALEWORLDTRANSFORM     0x402E
#define EMFPLUS_SETPAGETRANSFORM        0x4030
#define EMFPLUS_SETWORLDTRANSFORM       0x402A
#define EMFPLUS_TRANSLATEWORLDTRANSFORM 0x402D

#include <map>

namespace MetaFile
{
        static std::map<unsigned short, std::wstring> ActionNamesEmfPlus =
        {
                {0x4035, L"EMRPLUS_OFFSETCLIP"},
                {0x4031, L"EMRPLUS_RESETCLIP"},
                {0x4033, L"EMFPLUS_SETCLIPPATH"},
                {0x4032, L"EMFPLUS_SETCLIPRECT"},
                {0x4034, L"EMFPLUS_SETCLIPREGION"},

                {0x4003, L"EMFPLUS_COMMENT"},

                {0x4002, L"EMFPLUS_ENDOFFILE"},
                {0x4004, L"EMFPLUS_GETDC"},
                {0x4001, L"EMFPLUS_HEADER"},

                {0x4009, L"EMFPLUS_CLEAR"},
                {0x4012, L"EMFPLUS_DRAWARC"},
                {0x4019, L"EMFPLUS_DRAWBEZIERS"},
                {0x4017, L"EMFPLUS_DRAWCLOSEDCURVE"},
                {0x4018, L"EMFPLUS_DRAWCURVE"},
                {0x4036, L"EMFPLUS_DRAWDRIVERSTRING"},
                {0x400F, L"EMFPLUS_DRAWELLIPSE"},
                {0x401A, L"EMFPLUS_DRAWIMAGE"},
                {0x401B, L"EMFPLUS_DRAWIMAGEPOINTS"},
                {0x400D, L"EMFPLUS_DRAWLINES"},
                {0x4015, L"EMFPLUS_DRAWPATH"},
                {0x4011, L"EMFPLUS_DRAWPIE"},
                {0x400B, L"EMFPLUS_DRAWRECTS"},
                {0x401C, L"EMFPLUS_DRAWSTRING"},
                {0x4016, L"EMFPLUS_FILLCLOSEDCURVE"},
                {0x400E, L"EMFPLUS_FILLELLIPSE"},
                {0x4014, L"EMFPLUS_FILLPATH"},
                {0x4010, L"EMFPLUS_FILLPIE"},
                {0x400C, L"EMFPLUS_FILLPOLYGON"},
                {0x400A, L"EMFPLUS_FILLRECTS"},
                {0x4013, L"EMFPLUS_FILLREGION"},

                {0x4008, L"EMFPLUS_OBJECT"},
                {0x4038, L"EMFPLUS_SERIALIZABLEOBJECT"},

                {0x401E, L"EMFPLUS_SETANTIALIASMODE"},
                {0x4023, L"EMFPLUS_SETCOMPOSITINGMODE"},
                {0x4024, L"EMFPLUS_SETCOMPOSITINGQUALITY"},
                {0x4021, L"EMFPLUS_SETINTERPOLATIONMODE"},
                {0x4022, L"EMFPLUS_SETPIXELOFFSETMODE"},
                {0x401D, L"EMFPLUS_SETRENDERINGORIGIN"},
                {0x4020, L"EMFPLUS_SETTEXTCONTRAST"},
                {0x401F, L"EMFPLUS_SETTEXTRENDERINGHINT"},

                {0x4027, L"EMFPLUS_BEGINCONTAINER"},
                {0x4028, L"EMFPLUS_BEGINCONTAINERNOPARAMS"},
                {0x4029, L"EMFPLUS_ENDCONTAINER"},
                {0x4026, L"EMFPLUS_RESTORE"},
                {0x4025, L"EMFPLUS_SAVE"},

                {0x403A, L"EMFPLUS_SETTSCLIP"},
                {0x4039, L"EMFPLUS_SETTSGRAPHICS"},

                {0x402C, L"EMFPLUS_MULTIPLYWORLDTRANSFORM"},
                {0x402B, L"EMFPLUS_RESETWORLDTRANSFORM"},
                {0x402F, L"EMFPLUS_ROTATEWORLDTRANSFORM"},
                {0x402E, L"EMFPLUS_SCALEWORLDTRANSFORM"},
                {0x4030, L"EMFPLUS_SETPAGETRANSFORM"},
                {0x402A, L"EMFPLUS_SETWORLDTRANSFORM"},
                {0x402D, L"EMFPLUS_TRANSLATEWORLDTRANSFORM"}
        };
}

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

        unsigned short unShType, unShFlags;
        unsigned int unSize, unDataSize;
        do
        {
                unsigned int unRecordPos = m_oDataStream.Tell();

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
                        case EMFPLUS_CLEAR:             Read_EMFPLUS_CLEAR();                           break;
                        case EMFPLUS_DRAWARC:           Read_EMFPLUS_DRAWARC(unShFlags);                break;
                        case EMFPLUS_DRAWBEZIERS:       Read_EMFPLUS_DRAWBEZIERS(unShFlags);            break;
                        case EMFPLUS_DRAWCLOSEDCURVE:   Read_EMFPLUS_DRAWCLOSEDCURVE(unShFlags);        break;
                        case EMFPLUS_DRAWCURVE:         Read_EMFPLUS_DRAWCURVE(unShFlags);              break;
                        case EMFPLUS_DRAWDRIVERSTRING:  Read_EMFPLUS_DRAWDRIVERSTRING(unShFlags);       break;
                        case EMFPLUS_DRAWELLIPSE:       Read_EMFPLUS_DRAWELLIPSE(unShFlags);            break;
                        case EMFPLUS_DRAWIMAGE:         Read_EMFPLUS_DRAWIMAGEPOINTS(unShFlags);        break;
                        case EMFPLUS_DRAWLINES:         Read_EMFPLUS_DRAWLINES(unShFlags);              break;
                        case EMFPLUS_DRAWPATH:          Read_EMFPLUS_DRAWPATH(unShFlags);               break;
                        case EMFPLUS_DRAWPIE:           Read_EMFPLUS_DRAWPIE(unShFlags);                break;
                        case EMFPLUS_DRAWRECTS:         Read_EMFPLUS_DRAWRECTS(unShFlags);              break;
                        case EMFPLUS_DRAWSTRING:        Read_EMFPLUS_DRAWSTRING(unShFlags);             break;
                        case EMFPLUS_FILLCLOSEDCURVE:   Read_EMFPLUS_FILLCLOSEDCURVE(unShFlags);        break;
                        case EMFPLUS_FILLELLIPSE:       Read_EMFPLUS_FILLELLIPSE(unShFlags);            break;
                        case EMFPLUS_FILLPATH:          Read_EMFPLUS_FILLPATH(unShFlags);               break;
                        case EMFPLUS_FILLPIE:           Read_EMFPLUS_FILLPIE(unShFlags);                break;
                        case EMFPLUS_FILLPOLYGON:       Read_EMFPLUS_FILLPOLYGON(unShFlags);            break;
                        case EMFPLUS_FILLRECTS:         Read_EMFPLUS_FILLRECTS(unShFlags);              break;
                        case EMFPLUS_FILLREGION:        Read_EMFPLUS_FILLREGION(unShFlags);             break;

                        //Object Record Types (Типы записей объектов)
                        case EMFPLUS_OBJECT:            Read_EMFPLUS_OBJECT(unShFlags);                 break;
                        case EMFPLUS_SERIALIZABLEOBJECT:Read_EMFPLUS_SERIALIZABLEOBJECT(unShFlags);     break;

                        //Property Record Types (Типы записей свойств)
                        case EMFPLUS_SETANTIALIASMODE:          Read_EMFPLUS_SETANTIALIASMODE(unShFlags);       break;
                        case EMFPLUS_SETCOMPOSITINGMODE:        Read_EMFPLUS_SETCOMPOSITINGMODE(unShFlags);     break;
                        case EMFPLUS_SETCOMPOSITINGQUALITY:     Read_EMFPLUS_SETCOMPOSITINGQUALITY(unShFlags);  break;
                        case EMFPLUS_SETINTERPOLATIONMODE:      Read_EMFPLUS_SETINTERPOLATIONMODE(unShFlags);   break;
                        case EMFPLUS_SETPIXELOFFSETMODE:        Read_EMFPLUS_SETPIXELOFFSETMODE(unShFlags);     break;
                        case EMFPLUS_SETRENDERINGORIGIN:        Read_EMFPLUS_SETRENDERINGORIGIN();              break;
                        case EMFPLUS_SETTEXTCONTRAST:           Read_EMFPLUS_SETTEXTCONTRAST(unShFlags);        break;
                        case EMFPLUS_SETTEXTRENDERINGHINT:      Read_EMRPLUS_SETTEXTRENDERINGHINT(unShFlags);   break;

                        //State Record Types (Типы записей состояния)
                        case EMFPLUS_BEGINCONTAINER:            Read_EMFPLUS_BEGINCONTAINER(unShFlags); break;
                        case EMFPLUS_BEGINCONTAINERNOPARAMS:    Read_EMFPLUS_BEGINCONTAINERNOPARAMS();  break;
                        case EMFPLUS_ENDCONTAINER:              Read_EMFPLUS_ENDCONTAINER();            break;
                        case EMFPLUS_RESTORE:                   Read_EMFPLUS_RESTORE();                 break;
                        case EMFPLUS_SAVE:                      Read_EMFPLUS_SAVE();                    break;

                        //Terminal Server Record (Запись сервера Терминалов)
                        case EMFPLUS_SETTSCLIP:         Read_EMFPLUS_SETTSCLIP(unShFlags);      break;
                        case EMFPLUS_SETTSGRAPHICS:     Read_EMFPLUS_SETTSGRAPHICS(unShFlags);  break;

                        //Transform Record Types (Преобразование Типов записей)
                        case EMFPLUS_MULTIPLYWORLDTRANSFORM:    Read_EMFPLUS_MULTIPLYWORLDTRANSFORM(unShFlags); break;
                        case EMFPLUS_RESETWORLDTRANSFORM:       Read_EMFPLUS_RESETWORLDTRANSFORM();             break;
                        case EMFPLUS_ROTATEWORLDTRANSFORM:      Read_EMFPLUS_ROTATEWORLDTRANSFORM(unShFlags);   break;
                        case EMFPLUS_SCALEWORLDTRANSFORM:       Read_EMFPLUS_SCALEWORLDTRANSFORM(unShFlags);    break;
                        case EMFPLUS_SETPAGETRANSFORM:          Read_EMFPLUS_SETPAGETRANSFORM(unShFlags);       break;
                        case EMFPLUS_SETWORLDTRANSFORM:         Read_EMFPLUS_SETWORLDTRANSFORM();               break;
                        case EMFPLUS_TRANSLATEWORLDTRANSFORM:   Read_EMFPLUS_TRANSLATEWORLDTRANSFORM(unShFlags);break;

                        default: m_oDataStream.Skip(unSize - 12);     break;
                }

                int need_skip = (unRecordPos + unSize) - m_oDataStream.Tell();
                m_oDataStream.Skip(need_skip);

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
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_DRAWARC_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWARC_BASE(unsigned short unShFlags)
{
        char chOgjectIndex = ExpressValue(unShFlags, 0, 7);
        double dStartAngle, dSweepAngle;
        T oRect;

        m_oDataStream >> dStartAngle;
        m_oDataStream >> dSweepAngle;
        m_oDataStream >> oRect;
        //TODO: реализовать
}


void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS(unsigned short unShFlags)
{
        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPoint>(unShFlags);  // относительное расположение
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_DRAWBEZIERS_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение
        }

}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWBEZIERS_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unCountPoints;

        m_oDataStream >> unCountPoints;

        std::vector<T> arPoints(unCountPoints);

        for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
        {

        }

        //TODO: реализовать
        //TODO: для EmfPlusPointR опредилиться с реализацией
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE(unsigned short unShFlags)
{
        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными координатами
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_DRAWCLOSEDCURVE_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами.
        }
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWCLOSEDCURVE_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        double dTension;
        unsigned int unCount;

        m_oDataStream >> dTension;
        m_oDataStream >> unCount;

        std::vector<T> arPoints(unCount);

        for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
        {
                //TODO: реализовать
        }
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWCURVE(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWCURVE_BASE<TEmfPlusPoint>(unShFlags);
        else
                Read_EMFPLUS_DRAWCURVE_BASE<TEmfPlusPointF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWCURVE_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        double dTension;
        unsigned int unOffset, unNumSegments, unCountPoints;

        m_oDataStream >> dTension;
        m_oDataStream >> unOffset;
        m_oDataStream >> unNumSegments;
        m_oDataStream >> unCountPoints;

        std::vector<T> arPoints(unCountPoints);

        for (unsigned int unIndex = 0; unIndex < unCountPoints; ++unIndex)
        {
                //TODO: реализовать
        }
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWDRIVERSTRING(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unBrushId, unDriverStringOptionsFlags, unMatrixPresent, unGlyphCount;

        m_oDataStream >> unBrushId;
        m_oDataStream >> unDriverStringOptionsFlags;
        m_oDataStream >> unMatrixPresent;
        m_oDataStream >> unGlyphCount;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWELLIPSE(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWELLIPSE_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_DRAWELLIPSE_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWELLIPSE_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);

        T oRect;

        m_oDataStream >> oRect;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWIMAGE(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWIMAGE_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_DRAWIMAGE_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWIMAGE_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unImageAttributesID;
        int nSrcUnit;
        TEmfPlusRectF oSrcRect;

        m_oDataStream >> unImageAttributesID;
        m_oDataStream >> nSrcUnit;
        m_oDataStream >> oSrcRect;

        T oRectData;

        m_oDataStream >> oRectData;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWIMAGEPOINTS(unsigned short unShFlags)
{
        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_DRAWIMAGEPOINTS_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
        }
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWIMAGEPOINTS_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unImageAttributesID, unCount;
        int nSrcUnit;
        TEmfPlusRectF oSrcRect;

        m_oDataStream >> unImageAttributesID;
        m_oDataStream >> nSrcUnit;
        m_oDataStream >> oSrcRect;
        m_oDataStream >> unCount;

        std::vector<T> arPoints(unCount);

        for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
        {
                //TODO: реализовать
        }
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWLINES(unsigned short unShFlags)
{
        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_DRAWLINES_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
        }
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWLINES_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unCount;

        m_oDataStream >> unCount;

        std::vector<T> arPoints(unCount);

        for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
        {
                //TODO: реализовать
        }
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWPATH(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unPenId;

        m_oDataStream >> unPenId;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWPIE(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWPIE_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_DRAWPIE_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWPIE_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        double dStartAngle, dSweepAngle;
        T oRect;

        m_oDataStream >> dStartAngle;
        m_oDataStream >> dSweepAngle;
        m_oDataStream >> oRect;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWRECTS(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_DRAWRECTS_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_DRAWRECTS_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWRECTS_BASE(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unCount;

        m_oDataStream >> unCount;

        std::vector<T> arRects(unCount);

        for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
        {
                T oRect;

                m_oDataStream >> oRect;

                arRects.push_back(oRect);
        }
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_DRAWSTRING(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unBrushId, unFormatID, unLength;
        TEmfPlusRectF oRect;

        m_oDataStream >> unBrushId;
        m_oDataStream >> unFormatID;
        m_oDataStream >> unLength;
        m_oDataStream >> oRect;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLCLOSEDCURVE(unsigned short unShFlags)
{
        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_FILLCLOSEDCURVE_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
        }
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLCLOSEDCURVE_BASE(unsigned short unShFlags)
{
        unsigned int unBrushId, unCount;
        double dTension;

        m_oDataStream >> unBrushId;
        m_oDataStream >> dTension;
        m_oDataStream >> unCount;

        std::vector<T> arPoints(unCount);

        for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
        {

        }

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLELLIPSE(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_FILLELLIPSE_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_FILLELLIPSE_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLELLIPSE_BASE(unsigned short unShFlags)
{
        unsigned int unBrushId;
        T oRect;

        m_oDataStream >> unBrushId;
        m_oDataStream >> oRect;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLPATH(unsigned short unShFlags)
{
        unsigned int unBrushId;

        m_oDataStream >> unBrushId;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLPIE(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_FILLPIE_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_FILLPIE_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLPIE_BASE(unsigned short unShFlags)
{
        unsigned int unBrushId;
        double dStartAngle, dSweepAngle;
        T oRect;

        m_oDataStream >> unBrushId;
        m_oDataStream >> dStartAngle;
        m_oDataStream >> dSweepAngle;
        m_oDataStream >> oRect;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLPOLYGON(unsigned short unShFlags)
{
        if ((unShFlags >>(11)) & 1 )
        {
                //Определен флаг P (С игнорируется)
                Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPointR>(unShFlags); // относительное расположение
        }
        else if ((unShFlags >>(14)) & 1 )
        {
                //Не определен флаг P, но определен флаг С
                Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPoint>(unShFlags);  // абсолютное расположение с 16-разрядными целочисленными координатами
        }
        else
        {
                //Оба флага не определены
                Read_EMFPLUS_FILLPOLYGON_BASE<TEmfPlusPointF>(unShFlags); // абсолютное расположение с 32-разрядными координатами с плавующей запятой
        }
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLPOLYGON_BASE(unsigned short unShFlags)
{
        unsigned int unBrushId, unCount;

        m_oDataStream >> unBrushId;
        m_oDataStream >> unCount;

        std::vector<T> arPoints(unCount);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLRECTS(unsigned short unShFlags)
{
        if ((unShFlags >>(14)) & 1 )
                Read_EMFPLUS_FILLRECTS_BASE<TEmfPlusRect>(unShFlags);
        else
                Read_EMFPLUS_FILLRECTS_BASE<TEmfPlusRectF>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLRECTS_BASE(unsigned short unShFlags)
{
        unsigned int unBrushId, unCount;

        m_oDataStream >> unBrushId;
        m_oDataStream >> unCount;

        std::vector<T> arRects(unCount);

        for (unsigned int unIndex = 0; unIndex < unCount; ++unIndex)
        {
                T oRect;

                m_oDataStream >> oRect;

                arRects.push_back(oRect);
        }
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_FILLREGION(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        unsigned int unBrushId;

        m_oDataStream >> unBrushId;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_OBJECT(unsigned short unShFlags)
{
        short shOgjectIndex = ExpressValue(unShFlags, 0, 7);
        short shOgjectType  = ExpressValue(unShFlags, 8, 14);
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SERIALIZABLEOBJECT(unsigned short unShFlags)
{
        TGUID oGUID;
        unsigned int unBufferSize;

        m_oDataStream >> oGUID;
        m_oDataStream >> unBufferSize;

        m_oDataStream.Skip(unBufferSize);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETANTIALIASMODE(unsigned short unShFlags)
{
        short shSmoothingMode = ExpressValue(unShFlags, 1,  7);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETCOMPOSITINGMODE(unsigned short unShFlags)
{
        short shCompositingMode = ExpressValue(unShFlags, 0, 7);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETCOMPOSITINGQUALITY(unsigned short unShFlags)
{
        short shCompositingQuality = ExpressValue(unShFlags, 0, 7);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETINTERPOLATIONMODE(unsigned short unShFlags)
{
        short shInterpolationMode = ExpressValue(unShFlags, 0, 7);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETPIXELOFFSETMODE(unsigned short unShFlags)
{
        short shPixelOffsetMode = ExpressValue(unShFlags, 0, 7);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETRENDERINGORIGIN()
{
        int nX, nY;

        m_oDataStream >> nX;
        m_oDataStream >> nY;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETTEXTCONTRAST(unsigned short unShFlags)
{
        short shTextContrast  = ExpressValue(unShFlags, 0, 11);

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMRPLUS_SETTEXTRENDERINGHINT(unsigned short unShFlags)
{
        short shTextRenderingHint = ExpressValue(unShFlags, 0, 7);
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_BEGINCONTAINER(unsigned short unShFlags)
{
        short shPageUnit = ExpressValue(unShFlags, 8, 15);
        TEmfPlusRectF oDestRect, oSrcRect;
        unsigned int unStackIndex;

        m_oDataStream >> oDestRect;
        m_oDataStream >> oSrcRect;
        m_oDataStream >> unStackIndex;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_BEGINCONTAINERNOPARAMS()
{
        unsigned int unStackIndex;

        m_oDataStream >> unStackIndex;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_ENDCONTAINER()
{
        unsigned int unStackIndex;

        m_oDataStream >> unStackIndex;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_RESTORE()
{
        unsigned int unStackIndex;

        m_oDataStream >> unStackIndex;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SAVE()
{
        unsigned int unStackIndex;

        m_oDataStream >> unStackIndex;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETTSCLIP(unsigned short unShFlags)
{
        if ((unShFlags >>(15)) & 1 )
                Read_EMFPLUS_SETTSCLIP_BASE<TEmfPlusRectR>(unShFlags);
        else
                Read_EMFPLUS_SETTSCLIP_BASE<TEmfPlusRect>(unShFlags);
}

template<typename T>
void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETTSCLIP_BASE(unsigned short unShFlags)
{
        short shNumRects = ExpressValue(unShFlags, 0, 14);

        std::vector<T> arRects(shNumRects);

        for (unsigned int unIndex = 0; unIndex < shNumRects; ++unIndex)
        {
                T oRect;

                m_oDataStream >> oRect;

                arRects.push_back(oRect);
        }

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETTSGRAPHICS(unsigned short unShFlags)
{
        unsigned char unChAntiAliasMode, unChTextRenderHint, unChCompositingMode, unChCompositingQuality, unChFilterType, unChPixelOffset;
        short shRenderOriginX, shRenderOriginY;
        unsigned short unShTextContrast;
        TEmfPlusTransformMatrix oMatrix;

        m_oDataStream >> unChAntiAliasMode;
        m_oDataStream >> unChTextRenderHint;
        m_oDataStream >> unChCompositingMode;
        m_oDataStream >> unChCompositingQuality;
        m_oDataStream >> shRenderOriginX;
        m_oDataStream >> shRenderOriginY;
        m_oDataStream >> unShTextContrast;
        m_oDataStream >> unChFilterType;
        m_oDataStream >> unChPixelOffset;
        m_oDataStream >> oMatrix;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_MULTIPLYWORLDTRANSFORM(unsigned short unShFlags)
{
        TEmfPlusTransformMatrix oMatrix;

        m_oDataStream >> oMatrix;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_RESETWORLDTRANSFORM()
{
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_ROTATEWORLDTRANSFORM(unsigned short unShFlags)
{
        double dAngle;

        m_oDataStream >> dAngle;
        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SCALEWORLDTRANSFORM(unsigned short unShFlags)
{
        double dSx, dSy;

        m_oDataStream >> dSx;
        m_oDataStream >> dSy;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETPAGETRANSFORM(unsigned short unShFlags)
{
        short shPageUnit = ExpressValue(unShFlags, 0, 7);
        double dPageScale;

        m_oDataStream >> dPageScale;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_SETWORLDTRANSFORM()
{
        TEmfPlusTransformMatrix oMatrix;

        m_oDataStream >> oMatrix;

        //TODO: реализовать
}

void MetaFile::CEmfPlusParser::Read_EMFPLUS_TRANSLATEWORLDTRANSFORM(unsigned short unShFlags)
{
        double dX, dY;

        m_oDataStream >> dX;
        m_oDataStream >> dY;

        //TODO: реализовать
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

short MetaFile::CEmfPlusParser::ExpressValue(unsigned short unShFlags, unsigned int unStartIndex, unsigned int unEndIndex) const
{
        if (unStartIndex >= unEndIndex || unStartIndex > 14 || unEndIndex > 15)
                return 0;

        short shValue = 0;
        unsigned int unValueIndex = 0;

        for (unsigned int unIndex = unStartIndex; unIndex <= unEndIndex; ++unIndex, ++unValueIndex)
                shValue |= (((unShFlags >> unIndex) & 1) << unValueIndex);

        return shValue;
}
