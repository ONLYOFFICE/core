#ifndef QASCPRINTER_H
#define QASCPRINTER_H

#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QPaintEngine>
#include <qmath.h>

#include "./applicationmanager_events.h"

class QAscPrinterContext : public NSEditorApi::CAscPrinterContextBase
{
private:
    QPrinter m_oPrinter;
    QPainter m_oPainter;
    bool m_bIsUsePainter;

public:
    QAscPrinterContext() : NSEditorApi::CAscPrinterContextBase(), m_oPrinter(QPrinter::HighResolution)
    {
        m_bIsUsePainter = false;
    }
    QAscPrinterContext(const QPrinterInfo& pi) : NSEditorApi::CAscPrinterContextBase(),
        m_oPrinter(pi, QPrinter::HighResolution), m_bIsUsePainter(false)
    {
    }

    void BeginPaint()
    {
        m_bIsUsePainter = true;
        //m_oPrinter.setFullPage(true);
        m_oPainter.begin(&m_oPrinter);
    }
    void EndPaint()
    {
        if (m_bIsUsePainter)
            m_oPainter.end();
        m_bIsUsePainter = false;
    }

    QPrinter* getPrinter()
    {
        return &m_oPrinter;
    }

    virtual ~QAscPrinterContext()
    {
    }

    virtual void GetLogicalDPI(int& nDpiX, int& nDpiY)
    {
        nDpiX = m_oPrinter.logicalDpiX();
        nDpiY = m_oPrinter.logicalDpiY();
    }

    virtual void GetPhysicalRect(int& nX, int& nY, int& nW, int& nH)
    {
        QRect rect1 = m_oPrinter.pageRect();
        QRect rect2 = m_oPrinter.paperRect();

        nX = rect1.x();
        nY = rect1.y();
        nW = rect2.width();
        nH = rect2.height();
    }

    virtual void GetPrintAreaSize(int& nW, int& nH)
    {
        QRect rect = m_oPrinter.pageRect();
        nW = rect.width();
        nH = rect.height();
    }

    virtual void BitBlt(BYTE* pBGRA, const int& nRasterX, const int& nRasterY, const int& nRasterW, const int& nRasterH,
                        const double& x, const double& y, const double& w, const double& h, const double& dAngle)
    {
        int nPhysicalX = 0;
        int nPhysicalY = 0;
        int nPhysicalW = 0;
        int nPhysicalH = 0;
        this->GetPhysicalRect(nPhysicalX, nPhysicalY, nPhysicalW, nPhysicalH);

        //m_oPrinter.setFullPage(true);

        // TODO: must be faster!!! (create CBgraFrame as QPixmap???)
        //QPainter painter(&m_oPrinter);

        QPainter* painter = NULL;

        if (m_bIsUsePainter)
            painter = &m_oPainter;
        else
        {
            painter = new QPainter();
            painter->begin(&m_oPrinter);
        }

        painter->save();

        bool bIsPrintToFile = (m_oPrinter.outputFileName().length() != 0);

        bool bIsClip = painter->hasClipping();
        painter->setClipping(false);

        //QImage oImage(pBGRA + 4 * ((nRasterH - 1) * nRasterW), nRasterW, nRasterH, -4 * nRasterW, QImage::Format_ARGB32);
        QImage oImage(pBGRA, nRasterW, nRasterH, 4 * nRasterW, QImage::Format_ARGB32);

        double dAngleDeg = dAngle * 180.0 / M_PI;
        if ((std::abs(dAngleDeg - 90) < 1.0) || (std::abs(dAngleDeg - 270) < 1.0))
        {
            //painter->translate(-nPhysicalX, -nPhysicalY);

            float fCenterX = (float)(x + w / 2.0);
            float fCenterY = (float)(y + h / 2.0);

            painter->translate(fCenterX, fCenterY);
            painter->rotate(90);
            painter->translate(-fCenterX, -fCenterY);

            int nWDst = nRasterW;
            int nHDst = nRasterH;

            int nAreaW = 0;
            int nAreaH = 0;
            this->GetPrintAreaSize(nAreaW, nAreaH);

            int nOldX = nPhysicalX;
            nPhysicalX = nPhysicalY;
            nPhysicalY = nPhysicalW - nAreaW - nOldX;

            if (bIsPrintToFile)
            {
                // обнуляем сдвиги, напечатается и в отрицательных местах
                nPhysicalX = 0;
                nPhysicalY = 0;
            }

            nWDst -= nPhysicalX;
            nHDst -= nPhysicalY;
            QRect rect((int)x + nPhysicalX, (int)y + nPhysicalY, nWDst, nHDst);
            QRect rectSrc(nPhysicalX, nPhysicalY, nWDst, nHDst);

            //QRectF rectF((float)x + nPhysicalX, (float)y, (float)w, (float)h);
            //painter->drawImage(rectF, oImage);

            painter->drawImage(rect, oImage, rectSrc);
        }
        else
        {
            int nWDst = nRasterW;
            int nHDst = nRasterH;

            if (bIsPrintToFile)
            {
                // обнуляем сдвиги, напечатается и в отрицательных местах
                nPhysicalX = 0;
                nPhysicalY = 0;
            }

            nWDst -= nPhysicalX;
            nHDst -= nPhysicalY;
            QRect rect((int)x + nPhysicalX, (int)y + nPhysicalY, nWDst, nHDst);
            QRect rectSrc(nPhysicalX, nPhysicalY, nWDst, nHDst);

            painter->drawImage(rect, oImage, rectSrc);
        }

        painter->restore();

        if (!m_bIsUsePainter)
        {
            painter->end();
            delete painter;
        }

        //m_oPrinter.setFullPage(false);
    }
};

#endif  // QASCPRINTER_H
