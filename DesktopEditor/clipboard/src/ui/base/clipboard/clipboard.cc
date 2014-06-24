// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/base/clipboard/clipboard.h"

#include <iterator>

//#include "base/lazy_instance.h"
//#include "base/logging.h"
//#include "base/memory/scoped_ptr.h"
//#include "base/synchronization/lock.h"
#include "ui/gfx/size.h"

#include "libxml2/libxml/parser.h"
#include "libxml2/libxml/HTMLparser.h"
#include "base/strings/utf_string_conversions.h"

namespace ui {

namespace {

// A compromised renderer could send us bad data, so validate it.
// This function only checks that the size parameter makes sense, the caller
// is responsible for further validating the bitmap buffer against
// |bitmap_bytes|.
//
// |params| - Clipboard bitmap contents to validate.
// |bitmap_bytes| - On return contains the number of bytes needed to store
// the bitmap data or -1 if the data is invalid.
// returns: true if the bitmap size is valid, false otherwise.
bool IsBitmapSafe(const Clipboard::ObjectMapParams& params,
                  uint32* bitmap_bytes) {
  *bitmap_bytes = -1;
  if (params[1].size() != sizeof(gfx::Size))
    return false;
  const gfx::Size* size =
      reinterpret_cast<const gfx::Size*>(&(params[1].front()));
  uint32 total_size = size->width();
  // Using INT_MAX not SIZE_T_MAX to put a reasonable bound on things.
  if (INT_MAX / size->width() <= size->height())
    return false;
  total_size *= size->height();
  if (INT_MAX / total_size <= 4)
    return false;
  total_size *= 4;
  *bitmap_bytes = total_size;
  return true;
}

// Validates a plain bitmap on the clipboard.
// Returns true if the clipboard data makes sense and it's safe to access the
// bitmap.
bool ValidatePlainBitmap(const Clipboard::ObjectMapParams& params) {
  uint32 bitmap_bytes = -1;
  if (!IsBitmapSafe(params, &bitmap_bytes))
    return false;
  if (bitmap_bytes != params[0].size())
    return false;
  return true;
}

/*
// Valides a shared bitmap on the clipboard.
// Returns true if the clipboard data makes sense and it's safe to access the
// bitmap.
bool ValidateAndMapSharedBitmap(const Clipboard::ObjectMapParams& params,
                                base::SharedMemory* bitmap_data) {
  using base::SharedMemory;
  uint32 bitmap_bytes = -1;
  if (!IsBitmapSafe(params, &bitmap_bytes))
    return false;

  if (!bitmap_data || !SharedMemory::IsHandleValid(bitmap_data->handle()))
    return false;

  if (!bitmap_data->Map(bitmap_bytes)) {
    return false;
  }
  return true;
}
*/

/*
// A list of allowed threads. By default, this is empty and no thread checking
// is done (in the unit test case), but a user (like content) can set which
// threads are allowed to call this method.
typedef std::vector<base::PlatformThreadId> AllowedThreadsVector;
static base::LazyInstance<AllowedThreadsVector> g_allowed_threads =
    LAZY_INSTANCE_INITIALIZER;

// Mapping from threads to clipboard objects.
typedef std::map<base::PlatformThreadId, Clipboard*> ClipboardMap;
static base::LazyInstance<ClipboardMap> g_clipboard_map =
    LAZY_INSTANCE_INITIALIZER;

// Mutex that controls access to |g_clipboard_map|.
static base::LazyInstance<base::Lock>::Leaky
    g_clipboard_map_lock = LAZY_INSTANCE_INITIALIZER;

*/

const std::size_t kSourceTagSize = sizeof(SourceTag);

// The union serves to easily convert SourceTag into its binary representation
// and vice versa.
union SourceTag2BinaryHelper {
  SourceTag tag;
  uint8 bytes[kSourceTagSize];
};

}  // namespace

const char Clipboard::kMimeTypeText[] = "text/plain";
const char Clipboard::kMimeTypeURIList[] = "text/uri-list";
const char Clipboard::kMimeTypeDownloadURL[] = "downloadurl";
const char Clipboard::kMimeTypeHTML[] = "text/html";
const char Clipboard::kMimeTypeRTF[] = "text/rtf";
const char Clipboard::kMimeTypePNG[] = "image/png";

// static
Clipboard::ObjectMapParam Clipboard::SourceTag2Binary(SourceTag tag) {
  SourceTag2BinaryHelper helper;
  helper.tag = tag;
  std::vector<char> bytes(kSourceTagSize);
  memcpy(&bytes[0], helper.bytes, kSourceTagSize);
  return bytes;
}

// static
SourceTag Clipboard::Binary2SourceTag(const std::string& binary) {
  if (binary.size() != kSourceTagSize)
    return SourceTag();
  SourceTag2BinaryHelper helper;
  memcpy(helper.bytes, binary.c_str(), kSourceTagSize);
  return helper.tag;
}

/*
// static
void Clipboard::SetAllowedThreads(
    const std::vector<base::PlatformThreadId>& allowed_threads) {
  base::AutoLock lock(g_clipboard_map_lock.Get());

  g_allowed_threads.Get().clear();
  std::copy(allowed_threads.begin(), allowed_threads.end(),
            std::back_inserter(g_allowed_threads.Get()));
}
*/

// static
Clipboard* Clipboard::GetForCurrentThread() {

    return new Clipboard ();

    /*
  base::AutoLock lock(g_clipboard_map_lock.Get());

  base::PlatformThreadId id = base::PlatformThread::CurrentId();

  AllowedThreadsVector* allowed_threads = g_allowed_threads.Pointer();
  if (!allowed_threads->empty()) {
    bool found = false;
    for (AllowedThreadsVector::const_iterator it = allowed_threads->begin();
         it != allowed_threads->end(); ++it) {
      if (*it == id) {
        found = true;
        break;
      }
    }

  }

  ClipboardMap* clipboard_map = g_clipboard_map.Pointer();
  ClipboardMap::iterator it = clipboard_map->find(id);
  if (it != clipboard_map->end())
    return it->second;

  Clipboard* clipboard = new ui::Clipboard;
  clipboard_map->insert(std::make_pair(id, clipboard));
  return clipboard;
  */
}


void Clipboard::DestroyClipboardForCurrentThread(Clipboard* clipboard) {

	if (clipboard)
		delete clipboard;
	/*
  base::AutoLock lock(g_clipboard_map_lock.Get());

  ClipboardMap* clipboard_map = g_clipboard_map.Pointer();
  base::PlatformThreadId id = base::PlatformThread::CurrentId();
  ClipboardMap::iterator it = clipboard_map->find(id);
  if (it != clipboard_map->end()) {
    delete it->second;
    clipboard_map->erase(it);
  }
  */
}

void Clipboard::ReadDoct(Buffer buffer, base::string16* content) const
{
    // read HTML
    base::string16 markup;
    std::string src_url;
    uint32 fragment_start;
    uint32 fragment_end;

    ReadHTML(buffer, &markup, &src_url, &fragment_start, &fragment_end);
    xmlDocPtr pDoc = NULL;
    xmlNodePtr pRoot = NULL;
    //String filepath = App::GetInstance()->GetAppRootPath() + L"data/tizen.html";
    //ByteBuffer* pBuf = null;
    //pBuf = Tizen::Base::Utility::StringUtil::StringToUtf8N(filepath);

    //pDoc = htmlParseFile((const char*)pBuf->GetPointer(), (const char*)"utf-8");

    base::string16 markupFragment = markup.substr(fragment_start, fragment_end - fragment_start);
    std::string markupUtf8 = base::UTF16ToUTF8(markupFragment);

    pDoc = htmlParseDoc		((xmlChar*) markupUtf8.c_str(),  (const char*)"utf-8");

    if (pDoc)
    {
       pRoot = xmlDocGetRootElement(pDoc);
    }

    xmlNodePtr pCurrentElement = NULL;
    for (pCurrentElement = pRoot; pCurrentElement; pCurrentElement = pCurrentElement->next)
    {
       if (pCurrentElement->type == XML_ELEMENT_NODE)
       {
          // Insert your code to extract required contents from the parsed HTML document
           content->assign (base::UTF8ToUTF16(base::StringPiece((const char *) pCurrentElement->name)));
       }
    }
}


void Clipboard::WriteObjects(Buffer buffer,
                             const ObjectMap& objects,
                             SourceTag tag) {
  WriteObjectsImpl(buffer, objects, tag);
  /*
  if (!write_objects_callback_.is_null())
    write_objects_callback_.Run(buffer);
	*/
}

void Clipboard::DispatchObject(ObjectType type, const ObjectMapParams& params) {
  // All types apart from CBF_WEBKIT need at least 1 non-empty param.
  if (type != CBF_WEBKIT && (params.empty() || params[0].empty()))
    return;
  // Some other types need a non-empty 2nd param.
  if ((type == CBF_BOOKMARK || type == CBF_BITMAP ||
       /*type == CBF_SMBITMAP ||*/ type == CBF_DATA) &&
      (params.size() != 2 || params[1].empty()))
    return;
  switch (type) {
    case CBF_TEXT:
      WriteText(&(params[0].front()), params[0].size());
      break;

    case CBF_HTML:
      if (params.size() == 2) {
        if (params[1].empty())
          return;
        WriteHTML(&(params[0].front()), params[0].size(),
                  &(params[1].front()), params[1].size());
      } else if (params.size() == 1) {
        WriteHTML(&(params[0].front()), params[0].size(), NULL, 0);
      }
      break;

    case CBF_RTF:
      WriteRTF(&(params[0].front()), params[0].size());
      break;

    case CBF_BOOKMARK:
      WriteBookmark(&(params[0].front()), params[0].size(),
                    &(params[1].front()), params[1].size());
      break;

    case CBF_WEBKIT:
      WriteWebSmartPaste();
      break;

    case CBF_BITMAP:
      if (!ValidatePlainBitmap(params))
        return;

      WriteBitmap(&(params[0].front()), &(params[1].front()));
      break;

    case CBF_SMBITMAP: 
		{
			/*
      using base::SharedMemory;
      using base::SharedMemoryHandle;

      if (params[0].size() != sizeof(SharedMemory*))
        return;

      // It's OK to cast away constness here since we map the handle as
      // read-only.
      const char* raw_bitmap_data_const =
          reinterpret_cast<const char*>(&(params[0].front()));
      char* raw_bitmap_data = const_cast<char*>(raw_bitmap_data_const);
      scoped_ptr<SharedMemory> bitmap_data(
          *reinterpret_cast<SharedMemory**>(raw_bitmap_data));

      if (!ValidateAndMapSharedBitmap(params, bitmap_data.get()))
        return;
      WriteBitmap(static_cast<const char*>(bitmap_data->memory()),
                  &(params[1].front()));
				  */
      break;
    }

    case CBF_DATA:
      WriteData(
          FormatType::Deserialize(
              std::string(&(params[0].front()), params[0].size())),
          &(params[1].front()),
          params[1].size());
      break;

    default:
		break;
      //NOTREACHED();
  }
}

/*
// static
void Clipboard::ReplaceSharedMemHandle(ObjectMap* objects,
                                       base::SharedMemoryHandle bitmap_handle,
                                       base::ProcessHandle process) {
  using base::SharedMemory;
  bool has_shared_bitmap = false;

  for (ObjectMap::iterator iter = objects->begin(); iter != objects->end();
       ++iter) {
    if (iter->first == CBF_SMBITMAP) {
      // The code currently only accepts sending a single bitmap over this way.
      // Fail hard if we ever encounter more than one shared bitmap structure to
      // fill.
      //CHECK(!has_shared_bitmap);

#if defined(OS_WIN)
      SharedMemory* bitmap = new SharedMemory(bitmap_handle, true, process);
#else
      SharedMemory* bitmap = new SharedMemory(bitmap_handle, true);
#endif

      // We store the shared memory object pointer so it can be retrieved by the
      // UI thread (see DispatchObject()).
      iter->second[0].clear();
      for (size_t i = 0; i < sizeof(SharedMemory*); ++i)
        iter->second[0].push_back(reinterpret_cast<char*>(&bitmap)[i]);
      has_shared_bitmap = true;
    }
  }
}
*/

}  // namespace ui
