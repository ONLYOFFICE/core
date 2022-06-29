#include "ElementShape.h"
#include <limits.h>
#include "../resources/Constants.h"
#include "../resources/utils.h"

namespace NSDocxRenderer
{
    CShape::CShape() : CBaseItem(etShape)
    {
        m_dLeft = 0;
        m_dTop = 0;
        m_dWidth = 0;
        m_dHeight = 0;

        m_bIsNoFill = false;
        m_bIsNoStroke = false;
        m_bIsBehindDoc = true;

        m_lCoordSizeX = 100000;
        m_lCoordSizeY = 100000;

        m_lTxId = -1;
    }

    CShape::CShape(const CShape &oSrc) : CBaseItem(etShape)
    {
        *this = oSrc;
    }

    CShape::CShape(const CShape& oSrc1, const CShape& oSrc2) : CBaseItem(etShape)
    {
        //todo добавить логику объединения двух похожих шейпов в один новый
        //todo добавить метод ConverPathToVectorGraphics
    }

    CShape::~CShape()
    {
        Clear();
    }

    void CShape::Clear()
    {
        for (size_t i = 0; i <  m_arParagraphs.size(); ++i)
        {
            m_arParagraphs[i]->Clear();
        }
        m_arParagraphs.clear();
    }

    CShape& CShape::operator=(const CShape &oSrc)
    {
        if (this == &oSrc)
        {
            return *this;
        }

        CBaseItem::operator=(oSrc);

        m_strPath = oSrc.m_strPath;

        m_oBrush = oSrc.m_oBrush;
        m_oPen = oSrc.m_oPen;

        m_dLeft = oSrc.m_dLeft;
        m_dTop = oSrc.m_dTop;
        m_dWidth = oSrc.m_dWidth;
        m_dHeight = oSrc.m_dHeight;

        m_bIsNoFill = oSrc.m_bIsNoFill;
        m_bIsNoStroke = oSrc.m_bIsNoStroke;
        m_bIsBehindDoc = oSrc.m_bIsBehindDoc;

        m_lTxId = oSrc.m_lTxId;

        m_bIsNotNecessaryToUse = oSrc.m_bIsNotNecessaryToUse;

        return *this;
    }

    void CShape::GetDataFromVector(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize)
    {
        m_dLeft = oVector.m_dLeft;
        m_dTop = oVector.m_dTop;
        m_dWidth = oVector.m_dRight - m_dLeft;
        m_dHeight = oVector.m_dBottom - m_dTop;

        if (m_dWidth < 0.0001)
            m_dWidth = 0.0001;
        if (m_dHeight < 0.0001)
            m_dHeight = 0.0001;

        m_lCoordSizeX = lCoordSize;
        m_lCoordSizeY = lCoordSize;

        if (0x00 == (lType & 0x01))
        {
            m_bIsNoStroke = true;
        }
        if (0x00 == (lType >> 8))
        {
            m_bIsNoFill = true;
        }

        WritePath(oVector, lType, lCoordSize);
    }

    void CShape::WritePath(const CVectorGraphics& oVector, const LONG& lType, const LONG& lCoordSize)
    {
        size_t nCount = oVector.GetCurSize();
        double *pData = oVector.m_pData;

        double dWidth = (oVector.m_dRight - oVector.m_dLeft) * c_dMMToEMU;
        double dHeight = (oVector.m_dBottom - oVector.m_dTop) * c_dMMToEMU;

        NSStringUtils::CStringBuilder oWriter;

        oWriter.WriteString(L"<a:path w=\"");
        oWriter.AddInt(static_cast<int>(dWidth));
        oWriter.WriteString(L"\" h=\"");
        oWriter.AddInt(static_cast<int>(dHeight));
        oWriter.WriteString(L"\">");

        while (nCount > 0)
        {
            CVectorGraphics::VectorGraphicsType eType = static_cast<CVectorGraphics::VectorGraphicsType>(*pData++);

            switch (eType)
            {
            case CVectorGraphics::vgtMove:
            {
                LONG lX = (*pData - m_dLeft) * c_dMMToEMU;
                ++pData;
                LONG lY = (*pData - m_dTop) * c_dMMToEMU;
                ++pData;

                oWriter.WriteString(L"<a:moveTo><a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY));
                oWriter.WriteString(L"\"/></a:moveTo>");

                nCount -= 3;
                break;
            }
            case CVectorGraphics::vgtLine:
            {
                LONG lX = (*pData - m_dLeft)* c_dMMToEMU;
                ++pData;
                LONG lY = (*pData - m_dTop)* c_dMMToEMU;
                ++pData;

                oWriter.WriteString(L"<a:lnTo><a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY));
                oWriter.WriteString(L"\"/></a:lnTo>");

                nCount -= 3;
                break;
            }
            case CVectorGraphics::vgtCurve:
            {
                LONG lX1 = (*pData - m_dLeft)* c_dMMToEMU;
                ++pData;
                LONG lY1 = (*pData - m_dTop)* c_dMMToEMU;
                ++pData;
                LONG lX2 = (*pData - m_dLeft)* c_dMMToEMU;
                ++pData;
                LONG lY2 = (*pData - m_dTop)* c_dMMToEMU;
                ++pData;
                LONG lX3 = (*pData - m_dLeft)* c_dMMToEMU;
                ++pData;
                LONG lY3 = (*pData - m_dTop)* c_dMMToEMU;
                ++pData;

                oWriter.WriteString(L"<a:cubicBezTo>");

                oWriter.WriteString(L"<a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX1));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY1));
                oWriter.WriteString(L"\"/>");
                oWriter.WriteString(L"<a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX2));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY2));
                oWriter.WriteString(L"\"/>");
                oWriter.WriteString(L"<a:pt x=\"");
                oWriter.AddInt(static_cast<int>(lX3));
                oWriter.WriteString(L"\" y=\"");
                oWriter.AddInt(static_cast<int>(lY3));
                oWriter.WriteString(L"\"/>");

                oWriter.WriteString(L"</a:cubicBezTo>");

                nCount -= 7;
                break;
            }
            case CVectorGraphics::vgtClose:
            default:
                --nCount;
                break;
            }
        }
        oWriter.WriteString(L"<a:close/>");
        oWriter.WriteString(L"</a:path>");

        m_strPath = oWriter.GetData();
        oWriter.ClearNoAttack();
    }

    void CShape::ToXml(NSStringUtils::CStringBuilder &oWriter)
    {
        //todo для уменьшения размера каждого шейпа ипользовавать только то, что необходимо - для графики, текста, графика+текст
        //todo добавить все возможные параметры/атрибуты

        //note Если обрабатывается много документов за раз, то iNumber сохраняется.
        static UINT iNumber = 1;
        oWriter.WriteString(L"<w:r>");

        oWriter.WriteString(L"<w:rPr><w:noProof/></w:rPr>"); //отключение проверки орфографии

        oWriter.WriteString(L"<mc:AlternateContent>");
        oWriter.WriteString(L"<mc:Choice Requires=\"wps\">");
        oWriter.WriteString(L"<w:drawing>");

        oWriter.WriteString(L"<wp:anchor");
            oWriter.WriteString(L" distT=\"0\""); //Определяет минимальное расстояние, которое должно сохраняться между краем
            oWriter.WriteString(L" distB=\"0\""); //этого объекта рисования и любым последующим текстом в документе, когда
            oWriter.WriteString(L" distL=\"0\""); //этот графический объект объект отображается в содержимом документа.
            oWriter.WriteString(L" distR=\"0\"");
            oWriter.WriteString(L" simplePos=\"0\""); //true/1 Указывает, что этот объект должен быть позиционирован с использованием информации о позиционировании в дочернем элементе simplePos
            oWriter.WriteString(L" relativeHeight=\""); //Определяет относительное упорядочивание по Z всех объектов DrawingML в этом документе.
            oWriter.AddUInt(static_cast<UINT>(UINT_MAX - iNumber));
            oWriter.WriteString(L"\"");
            oWriter.WriteString(L" behindDoc=\""); //позади текста - 1, перед текстом - 0
            oWriter.AddUInt(static_cast<UINT>(m_bIsBehindDoc));
            oWriter.WriteString(L"\"");
            oWriter.WriteString(L" locked=\"0\""); //true/1 Указывает, что местоположение привязки для этого объекта не должно быть изменено во время выполнения, когда приложение редактирует содержимое этого документа.
            oWriter.WriteString(L" layoutInCell=\"0\""); //объект будет позиционирован, как указано, но таблица будет изменена в размерах и/или перемещена в документе, как это необходимо для размещения объекта.
            oWriter.WriteString(L" allowOverlap=\"1\""); //разрешается перекрывать содержимое другого объекта
            oWriter.WriteString(L" hidden=\"0\""); //Определяет, будет ли отображаться данный плавающий объект DrawingML.
        oWriter.WriteString(L">");

        oWriter.WriteString(L"<wp:simplePos x=\"0\" y=\"0\"/>");

        oWriter.WriteString(L"<wp:positionH relativeFrom=\"page\">");
            oWriter.WriteString(L"<wp:posOffset>");
            oWriter.AddInt(static_cast<int>(m_dLeft * c_dMMToEMU));
            oWriter.WriteString(L"</wp:posOffset>");
        oWriter.WriteString(L"</wp:positionH>");

        oWriter.WriteString(L"<wp:positionV relativeFrom=\"page\">");
            oWriter.WriteString(L"<wp:posOffset>");
            oWriter.AddInt(static_cast<int>(m_dTop * c_dMMToEMU));
            oWriter.WriteString(L"</wp:posOffset>");
        oWriter.WriteString(L"</wp:positionV>");

        //координаты конца границы шейпа
        oWriter.WriteString(L"<wp:extent");
        oWriter.WriteString(L" cx=\"");
        oWriter.AddInt(static_cast<int>(m_dWidth * c_dMMToEMU));
        oWriter.WriteString(L"\" cy=\"");
        oWriter.AddInt(static_cast<int>(m_dHeight * c_dMMToEMU));
        oWriter.WriteString(L"\"/>");

        oWriter.WriteString(L"<wp:effectExtent l=\"0\" t=\"0\" r=\"0\" b=\"0\"/>"); //Этот элемент определяет дополнительное расстояние, которое должно быть добавлено к каждому краю изображения, чтобы компенсировать любые эффекты рисования, применяемые к объекту DrawingML

        oWriter.WriteString(L"<wp:wrapNone/>");

        oWriter.WriteString(L"<wp:docPr id=\"");
        oWriter.AddUInt(iNumber);
        oWriter.WriteString(L"\" name=\"Freeform: Shape ");
        oWriter.AddUInt(iNumber);
        oWriter.WriteString(L"\"/>");
        iNumber++;

        oWriter.WriteString(L"<wp:cNvGraphicFramePr/>");

        oWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">");

        oWriter.WriteString(L"<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");

        oWriter.WriteString(L"<wps:wsp>");

        oWriter.WriteString(L"<wps:cNvSpPr/>"); //non-visual shape properties. http://officeopenxml.com/drwSp-nvSpPr.php

        oWriter.WriteString(L"<wps:spPr>"); //shape properties. http://officeopenxml.com/drwSp-SpPr.php
            //отвечает за размеры прямоугольного фрейма шейпа
            oWriter.WriteString(L"<a:xfrm>");
                oWriter.WriteString(L"<a:off x=\"0\" y=\"0\"/>");
                oWriter.WriteString(L"<a:ext");
                oWriter.WriteString(L" cx=\"");
                oWriter.AddInt(static_cast<int>(m_dWidth * c_dMMToEMU));
                oWriter.WriteString(L"\" cy=\"");
                oWriter.AddInt(static_cast<int>(m_dHeight * c_dMMToEMU));
                oWriter.WriteString(L"\"/>");
            oWriter.WriteString(L"</a:xfrm>");

            //Если просто текст без графики
            if (m_strPath.empty())
            {
                oWriter.WriteString(L"<a:prstGeom prst=\"rect\">");
                    oWriter.WriteString(L"<a:avLst/>");
                oWriter.WriteString(L"</a:prstGeom>");
            }
            else
            {
                //Рисуем сложный путь
                oWriter.WriteString(L"<a:custGeom>");
                oWriter.WriteString(L"<a:avLst/>");
                oWriter.WriteString(L"<a:gdLst/>");
                oWriter.WriteString(L"<a:ahLst/>");
                oWriter.WriteString(L"<a:cxnLst/>");
                oWriter.WriteString(L"<a:rect l=\"l\" t=\"t\" r=\"r\" b=\"b\"/>");
                oWriter.WriteString(L"<a:pathLst>");
                oWriter.WriteString(m_strPath);
                oWriter.WriteString(L"</a:pathLst>");
                oWriter.WriteString(L"</a:custGeom>");
            }

            if (m_bIsNoFill)
            {
                //Нет заливки
                oWriter.WriteString(L"<a:noFill/>");
            }
            else
            {
                //Есть заливка
                oWriter.WriteString(L"<a:solidFill>");
                oWriter.WriteString(L"<a:srgbClr val=\"");
                oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oBrush.Color1)));
                if (0xFF != m_oBrush.TextureAlpha)
                {
                    oWriter.WriteString(L"\"><a:alpha val=\"");
                    oWriter.AddInt(static_cast<int>(m_oPen.Alpha / 255.0 * 100.0));
                    oWriter.WriteString(L"%\"/></a:srgbClr>");
                }
                else
                {
                    oWriter.WriteString(L"\"/>");
                }
                oWriter.WriteString(L"</a:solidFill>");
            }

            if (m_bIsNoStroke)
            {
                oWriter.WriteString(L"<a:ln>"); // w - width по умолчанию 0.75pt = 9525
                    oWriter.WriteString(L"<a:noFill/>");
                oWriter.WriteString(L"</a:ln>");
            }
            else
            {
                oWriter.WriteString(L"<a:ln w=\"");
                oWriter.AddInt(static_cast<int>(m_oPen.Size * c_dMMToEMU)); //note можно писать в мм
                oWriter.WriteString(L"\">");

                oWriter.WriteString(L"<a:solidFill>");

                oWriter.WriteString(L"<a:srgbClr val=\"");
                oWriter.WriteHexInt3(static_cast<int>(ConvertColorBGRToRGB(m_oPen.Color))); //note можно вместо цвета использовать слова типа "black"
                if (0xFF != m_oPen.Alpha)
                {
                    oWriter.WriteString(L"\"><a:alpha val=\"");
                    oWriter.AddInt(static_cast<int>(m_oPen.Alpha / 255.0 * 100.0));
                    oWriter.WriteString(L"%\"/></a:srgbClr>");
                }
                else
                {
                    oWriter.WriteString(L"\"/>");
                }

                oWriter.WriteString(L"</a:solidFill>");
                oWriter.WriteString(L"</a:ln>");
            }

        oWriter.WriteString(L"</wps:spPr>");

        //не работает
        //oWriter.WriteString(L"<wps:style/>"); //shape styles. http://officeopenxml.com/drwSp-styles.php

        if (!m_arParagraphs.empty())
        {
            oWriter.WriteString(L"<wps:txbx>"); //text within the shape. http://officeopenxml.com/drwSp-text.php
            oWriter.WriteString(L"<w:txbxContent>");
            for (size_t i = 0; i < m_arParagraphs.size(); ++i)
            {
                CParagraph* pParagraph = m_arParagraphs[i];
                pParagraph->ToXml(oWriter);
            }
            oWriter.WriteString(L"</w:txbxContent>");
            oWriter.WriteString(L"</wps:txbx>");
        }

        oWriter.WriteString(L"<wps:bodyPr"); //определяет свойства тела текста внутри фигуры
            oWriter.WriteString(L" rot=\"0\""); //Определяет поворот, который применяется к тексту в пределах ограничивающей рамки.
            oWriter.WriteString(L" spcFirstLastPara=\"0\""); //должен ли соблюдаться интервал между абзацами до и после, заданный пользователем.
            oWriter.WriteString(L" vertOverflow=\"overflow\""); //может ли текст выходить за пределы ограничительной рамки по вертикали
            oWriter.WriteString(L" horzOverflow=\"overflow\""); //может ли текст выходить за пределы ограничительной рамки по горизонтали.
            oWriter.WriteString(L" vert=\"horz\"");
            //oWriter.WriteString(L" wrap=\"none\""); //граница шейпа по ширине текста
            oWriter.WriteString(L" wrap=\"square\""); //Определяет параметры обертки, которые будут использоваться для данного текстового тела.
            //на сколько граница текста отступает от границы шейпа
            oWriter.WriteString(L" lIns=\"0\""); //left   по умолчанию 0.25см = 91440
            oWriter.WriteString(L" tIns=\"0\""); //top    по умолчанию 0.13см = 45720
            oWriter.WriteString(L" rIns=\"0\""); //right  по умолчанию 0.25см
            oWriter.WriteString(L" bIns=\"0\""); //bottom по умолчанию 0.13см
            oWriter.WriteString(L" numCol=\"1\""); //Определяет количество колонок текста в ограничивающем прямоугольнике.
            oWriter.WriteString(L" spcCol=\"0\""); //Определяет пространство между колонками текста в текстовой области (только если numCol >1)
            oWriter.WriteString(L" rtlCol=\"0\""); //используются ли столбцы в порядке справа налево (true) или слева направо (false).
            oWriter.WriteString(L" fromWordArt=\"0\""); //true/1 текст в этом текстовом поле является преобразованным текстом из объекта WordArt.
            oWriter.WriteString(L" anchor=\"t\""); //Вертикальное выравнивание текста в шейпе (t - top, b - bottom, ctr - middle) по умолчанию top
            oWriter.WriteString(L" anchorCtr=\"0\""); //true/1 Определяет центрирование текстового поля.
            oWriter.WriteString(L" forceAA=\"0\""); //true/1 Заставляет текст отображаться сглаженным независимо от размера шрифта.
            oWriter.WriteString(L" compatLnSpc=\"1\""); //межстрочный интервал для данного текста определяется упрощенным способом с помощью сцены шрифта.
        oWriter.WriteString(L">");

        oWriter.WriteString(L"<a:prstTxWarp prst=\"textNoShape\">");
            oWriter.WriteString(L"<a:avLst/>");
        oWriter.WriteString(L"</a:prstTxWarp>");
        oWriter.WriteString(L"<a:noAutofit/>");
        oWriter.WriteString(L"</wps:bodyPr>");

        oWriter.WriteString(L"</wps:wsp>");
        oWriter.WriteString(L"</a:graphicData>");
        oWriter.WriteString(L"</a:graphic>");
        oWriter.WriteString(L"</wp:anchor>");
        oWriter.WriteString(L"</w:drawing>");
        oWriter.WriteString(L"</mc:Choice>");

        oWriter.WriteString(L"<mc:Fallback/>");
        //здесь можно вставить COldShape без <w:r>
        //
        //oWriter.WriteString(L"</mc:Fallback>");

        oWriter.WriteString(L"</mc:AlternateContent>");
        oWriter.WriteString(L"</w:r>");
    }
}; // namespace NSDocxRenderer
