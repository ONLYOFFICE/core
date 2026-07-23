/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CCustomView.h"

#include <QPixmap>
Q_GUI_EXPORT QPixmap qt_pixmapFromWinHBITMAP(HBITMAP bitmap, int hbitmapFormat=0);

#include <QWheelEvent>

CCustomView::CCustomView(QWidget *parent)
      : QGraphicsView(parent),
        m_pBitmap(NULL)
{}

CCustomView::~CCustomView()
{
        Clear();
}

unsigned int CCustomView::GetHeightMetafile() const
{
        return m_pBitmap->GetHeight();
}

unsigned int CCustomView::GetWidthMetafile() const
{
        return m_pBitmap->GetWidth();
}

bool CCustomView::DrawMetafile(const std::wstring& wsFilePath)
{
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;

        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        Clear();
        m_pBitmap = Gdiplus::Bitmap::FromFile(wsFilePath.c_str());

        HBITMAP handleToSliceRet = NULL;
        m_pBitmap->GetHBITMAP(Gdiplus::Color::Transparent, &handleToSliceRet);

        if (NULL == handleToSliceRet)
                return false;

        QPixmap oPixmap = qt_pixmapFromWinHBITMAP(handleToSliceRet);

        QGraphicsScene *pScene = new QGraphicsScene();

        pScene->addPixmap(oPixmap);

        setScene(pScene);

        return true;
}

void CCustomView::Clear()
{
        if (NULL != m_pBitmap)
        {
                delete m_pBitmap;
                m_pBitmap = NULL;
        }

        QGraphicsScene *pScene = scene();
        if (NULL != pScene)
        {
                delete pScene;
                setScene(NULL);
        }
}

void CCustomView::wheelEvent(QWheelEvent *event)
{
        double delta = event->delta();
        double sf = 1.0 + delta / 1000;
        scale(sf, sf);
}
