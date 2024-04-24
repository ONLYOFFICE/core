/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#ifndef JNI_EXCEPTIONS_UTILS_H
#define JNI_EXCEPTIONS_UTILS_H

#include <iostream>
#include <jni.h>
#include <JniLogUtils.h>

using namespace std;

#define THROW(ENV, CLASS, MESSAGE)                                    \
    jclass jc = ENV->FindClass(CLASS);                                \
    if (jc != NULL) {                                                 \
        ENV->ThrowNew(jc, MESSAGE);                                   \
    }

#define TRY_JNI(ENV, STUFF)                                           \
    try {                                                             \
        STUFF;                                                        \
    } catch (const std::bad_alloc& e){                                \
        THROW(ENV, "java/lang/OutOfMemoryError", e.what())            \
    } catch (const std::ios_base::failure& e) {                       \
        THROW(ENV, "java/io/IOException", e.what())                   \
    } catch (const std::exception& e) {                               \
        THROW(ENV, "java/lang/Exception", e.what())                   \
    } catch (...) {                                                   \
        THROW(ENV, "java/lang/Error", "Unidentified exception")       \
    }

#endif
