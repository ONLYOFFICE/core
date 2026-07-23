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

#ifndef CLOGFONTEXDV_H
#define CLOGFONTEXDV_H

#include "CSharedWidget.h"

class CLogFontExDvWidget : public CSharedWidget
{
public:
        explicit CLogFontExDvWidget(QWidget *pParent = NULL);
        virtual ~CLogFontExDvWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

class CLogFontExWidget : public CSharedWidget
{
public:
        explicit CLogFontExWidget(QWidget *pParent = NULL);
        virtual ~CLogFontExWidget();
};

class CLogFontWidget : public CSharedWidget
{
public:
        explicit CLogFontWidget(QWidget *pParent = NULL);
        virtual ~CLogFontWidget();

        virtual QList<QStandardItem*> GetData() const override;
};

class CDesignVectorWidget : public CSharedWidget
{
public:
        explicit CDesignVectorWidget(QWidget *pParent = NULL);
        virtual ~CDesignVectorWidget();

        virtual QList<QStandardItem*> GetData() const override;
private slots:
        void on_createValue_clicked();
private:
        unsigned int m_unCounter;
};

class CLinkedUFISWidget  : public CSharedWidget
{
public:
        explicit CLinkedUFISWidget(QWidget *pParent = NULL);
        virtual ~CLinkedUFISWidget();
private slots:
        void on_createValue_clicked();
private:
        unsigned int m_unCounter;
};

class CUniversalFontIdWidget : public CSharedWidget
{
public:
        explicit CUniversalFontIdWidget(unsigned int unIndex = 0, QWidget *pParent = NULL);
        virtual ~CUniversalFontIdWidget();

        virtual QList<QStandardItem*> GetData() const override;
private:
        unsigned int m_unIndex;
};

#endif // CLOGFONTEXDV_H
