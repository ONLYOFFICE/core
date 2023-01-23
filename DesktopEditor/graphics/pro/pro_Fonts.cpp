/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "Fonts.h"
#include "../../common/File.h"
#include "../../common/StringExt.h"
#include "../../fontengine/ApplicationFonts.h"
#include "./pro_base.cpp"

NSFonts::IFontsMemoryStorage* g_global_fonts_memory_storage = NULL;

namespace NSFonts
{
    CLibrary::CLibrary()
    {
        m_internal = new CLibrary_private();
        m_internal->m_library = NULL;
    }
    CLibrary::~CLibrary()
    {
        RELEASEOBJECT(m_internal);
    }
}

namespace NSFonts
{
    IFontPath::IFontPath() : NSBase::CBaseRefCounter() {}
    IFontPath::~IFontPath() {}
    namespace NSFontPath
    {
        IFontPath* Create()
        {
            return new CFontPath();
        }
    }

    IFontStream::IFontStream() : NSBase::CBaseRefCounter() {}
    IFontStream::~IFontStream() {}
    namespace NSStream
    {
        IFontStream* Create()
        {
            return new CFontStream();
        }
    }

    IApplicationFontStreams::IApplicationFontStreams() : NSBase::CBaseRefCounter() {}
    IApplicationFontStreams::~IApplicationFontStreams() {}

    IFontsMemoryStorage::IFontsMemoryStorage() {}
    IFontsMemoryStorage::~IFontsMemoryStorage() {}

    namespace NSApplicationFontStream
    {
        IApplicationFontStreams* Create()
        {
            return new CApplicationFontStreams();
        }

        // default global memory storage
        class CDefaultGlobalFontsMemoryStorage : public IFontsMemoryStorage
        {
        private:
            std::map<std::wstring, IFontStream*> m_mapStreams;
            int m_nInternalCounter;

        public:
            CDefaultGlobalFontsMemoryStorage()
            {
                m_nInternalCounter = 1;
            }
            virtual ~CDefaultGlobalFontsMemoryStorage()
            {
                Clear();
            }

            virtual bool Add(const std::wstring& id, BYTE* data, LONG size, bool bClear = false)
            {
                std::wstring sFile = id;
                NSStringExt::Replace(sFile, L"\\", L"/");
                std::map<std::wstring, IFontStream*>::iterator it = m_mapStreams.find(sFile);
                if (it != m_mapStreams.end())
                    return false;

                IFontStream* pStream = NSFonts::NSStream::Create();
                pStream->CreateFromMemory(data, size, bClear);
                m_mapStreams.insert(std::pair<std::wstring, IFontStream*>(sFile, pStream));
                return true;
            }
            virtual bool Remove(const std::wstring& id)
            {
                std::wstring sFile = id;
                NSStringExt::Replace(sFile, L"\\", L"/");
                std::map<std::wstring, IFontStream*>::iterator it = m_mapStreams.find(sFile);
                if (it == m_mapStreams.end())
                    return false;

                RELEASEINTERFACE(it->second);
                m_mapStreams.erase(it);
                return true;
            }
            virtual void Clear()
            {
                for (std::map<std::wstring, IFontStream*>::iterator it = m_mapStreams.begin(); it != m_mapStreams.end(); it++)
                    RELEASEINTERFACE(it->second);
                m_mapStreams.clear();
            }

            virtual IFontStream* Get(const std::wstring& id)
            {
                std::wstring sFile = id;
                NSStringExt::Replace(sFile, L"\\", L"/");
                std::map<std::wstring, IFontStream*>::iterator it = m_mapStreams.find(sFile);
                return it != m_mapStreams.end() ? it->second : NULL;
            }

            virtual std::wstring GenerateId()
            {
                std::wstring sKey;
                do
                {
                    sKey = L"storage_internal_" + std::to_wstring(m_nInternalCounter++);
                } while (m_mapStreams.find(sKey) != m_mapStreams.end());
                return sKey;
            }
        };

        GRAPHICS_DECL IFontsMemoryStorage* CreateDefaultGlobalMemoryStorage()
        {
            return new CDefaultGlobalFontsMemoryStorage();
        }

        GRAPHICS_DECL IFontsMemoryStorage* GetGlobalMemoryStorage()
        {
            return g_global_fonts_memory_storage;
        }
        GRAPHICS_DECL void SetGlobalMemoryStorage(IFontsMemoryStorage* pStorage)
        {
            if (g_global_fonts_memory_storage != pStorage)
                RELEASEINTERFACE(g_global_fonts_memory_storage);

            g_global_fonts_memory_storage = pStorage;
        }
    }

    IFontFile::IFontFile() : NSBase::CBaseRefCounter() {}
    IFontFile::~IFontFile() {}
    namespace NSFontFile
    {
        IFontFile* Create()
        {
            return new CFontFile();
        }
    }

    IFontsCache::IFontsCache() : NSBase::CBaseRefCounter() {}
    IFontsCache::~IFontsCache() {}
    namespace NSFontCache
    {
        IFontsCache* Create()
        {
            return new CFontsCache();
        }
    }

    IFontManager::IFontManager() : NSBase::CBaseRefCounter() {}
    IFontManager::~IFontManager() {}

    void IFontManager::CreateOwnerCache(const int& nCacheSize)
    {
        NSFonts::IFontsCache* pCache = NSFonts::NSFontCache::Create();
        pCache->SetStreams(GetApplication()->GetStreams());
        pCache->SetCacheSize(nCacheSize);
        SetOwnerCache(pCache);
    }

    namespace NSFontManager
    {
        IFontManager* Create()
        {
            return new CFontManager();
        }
    }

    IFontList::IFontList() : NSBase::CBaseRefCounter() {}
    IFontList::~IFontList() {}

    IApplicationFonts::IApplicationFonts() : NSBase::CBaseRefCounter() {}
    IApplicationFonts::~IApplicationFonts() {}
    namespace NSApplication
    {
        IApplicationFonts* Create()
        {
            return new CApplicationFonts();
        }
    }
}
