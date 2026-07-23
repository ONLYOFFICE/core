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

#ifndef CSHAREDWIDGET_H
#define CSHAREDWIDGET_H

#include <QFrame>
#include <QBoxLayout>
#include <QStandardItem>

class CSharedWidget : public QFrame
{
        Q_OBJECT
public:
        explicit CSharedWidget(QWidget *pParent = NULL) : QFrame(pParent) {setFrameShape(QFrame::Box);};
        virtual ~CSharedWidget() {};

        virtual QList<QStandardItem*> GetData() const
        {
                QList<QStandardItem*> arData;

                for (const CSharedWidget *pWidget : m_arWidgets)
                        arData.append(pWidget->GetData());

                return arData;
        }
private:
        friend class CPixelFormatDescriptorWidget;
        friend class CGradientRectangleWidget;
        friend class CGradientTriangleWidget;
        friend class CExtSelectClipRgnWidget;
        friend class CUniversalFontIdWidget;
        friend class CPaletteEntriesWidget;
        friend class CLogColorSpaceWidget;
        friend class CSmallTextOutWidget;
        friend class CCIEXYZTripleWidget;
        friend class CGradientFillWidget;
        friend class CDesignVectorWidget;
        friend class CLogFontExDvWidget;
        friend class CPolyTextOutWidget;
        friend class CLogBrushExWidget;
        friend class CLogPaletteWidget;
        friend class CExtTextOutWidget;
        friend class CLinkedUFISWidget;
        friend class CInvertRgnWidget;
        friend class CLogFontExWidget;
        friend class CTriVertexWidget;
        friend class CRectangleWidget;
        friend class CPolyDrawWidget;
        friend class CFrameRgnWidget;
        friend class CPaintRgnWidget;
        friend class CLogFontWidget;
        friend class CFillRgnWidget;
        friend class CEmrTextWidget;
        friend class CLogPenWidget;
        friend class CPointsWidget;
        friend class CXFormWidget;
        friend class CIEXYZWidget;
        friend class CPointWidget;
        friend class CSizeWidget;
        friend class CPolyWidget;
        friend class CDxWidget;

        void AddWidget(CSharedWidget* pWidget, QBoxLayout *pLayout = NULL)
        {
                if (NULL == pWidget)
                        return;

                m_arWidgets.push_back(pWidget);

                if (NULL != pLayout)
                        pLayout->addWidget(pWidget);
        }

        void AddWidgets(std::vector<CSharedWidget*> arWidgets, QBoxLayout *pLayout = NULL)
        {
                for (CSharedWidget* pWidget : arWidgets)
                        AddWidget(pWidget, pLayout);
        }

        std::vector<CSharedWidget*> m_arWidgets;
};

#endif // CSHAREDWIDGET_H
