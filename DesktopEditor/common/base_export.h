#ifndef _BUILD_BASE_EXPORTIMPORT_H
#define _BUILD_BASE_EXPORTIMPORT_H

#ifndef Q_DECL_EXPORT

//#define Q_BUILDCUSTOM_VISIBILITY_AVAILABLE

#ifndef Q_BUILDCUSTOM_VISIBILITY_AVAILABLE
#ifdef QT_VISIBILITY_AVAILABLE
#define Q_BUILDCUSTOM_VISIBILITY_AVAILABLE
#endif
#endif

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
#define Q_DECL_EXPORT     __declspec(dllexport)
#define Q_DECL_IMPORT     __declspec(dllimport)
#elif defined(Q_BUILDCUSTOM_VISIBILITY_AVAILABLE)
#define Q_DECL_EXPORT     __attribute__((visibility("default")))
#define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#ifndef Q_DECL_EXPORT
#define Q_DECL_EXPORT
#endif
#ifndef Q_DECL_IMPORT
#define Q_DECL_IMPORT
#endif

#endif

#endif // _BUILD_BASE_EXPORTIMPORT_H
