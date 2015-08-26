#ifndef CEFCLIENT_CEFWEBVIEW_H
#define CEFCLIENT_CEFWEBVIEW_H

#include "base.h"
#include "./applicationmanager_events.h"

enum CefViewWrapperType
{
    cvwtSimple      = 0,
    cvwtEditor      = 1
};

enum AscEditorType
{
    etDocument      = 0,
    etPresentation  = 1,
    etSpreadsheet   = 2,
    etUndefined     = 255
};

class CCefView_Private;
class CAscApplicationManager;

class CCefViewWidgetImpl
{
public:
    virtual bool parent_window_is_empty() { return false; }
    virtual int parent_x() { return 0; }
    virtual int parent_y() { return 0; }
    virtual int parent_width() { return 0; }
    virtual int parent_height() { return 0; }
    virtual WindowHandleId parent_wid() { return 0; }
    virtual void child_loaded() { }
};

class Q_DECL_EXPORT CCefView
{
public:

    CCefView(CCefViewWidgetImpl* parent, int nId);
    virtual ~CCefView();

    void load(const std::wstring& url);
    std::wstring GetUrl();

    void focus(bool value = true);

    void resizeEvent(int width = 0, int height = 0);
    void moveEvent();
    bool nativeEvent(const char* data, const int& datalen, void *message, long *result);

    void Apply(NSEditorApi::CAscMenuEvent* );
    NSEditorApi::CAscMenuEvent* ApplySync(NSEditorApi::CAscMenuEvent* );

#if 0
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragMoveEvent(QDragMoveEvent *);
    virtual void dragLeaveEvent(QDragLeaveEvent *);
    virtual void dropEvent(QDropEvent *);
#endif

    CAscApplicationManager* GetAppManager();
    void SetAppManager(CAscApplicationManager* );

    CCefViewWidgetImpl* GetWidgetImpl();

    int GetId();
    CefViewWrapperType GetType();

    void SetParentCef(int nId);
    int GetParentCef();

    void SetModified(bool bIsModified);
    bool GetModified();

protected:
    int m_nId;
    CefViewWrapperType m_eWrapperType;
    CCefView_Private* m_pInternal;

    friend class CAscClientHandler;
    friend class CAscApplicationManager;
};

class Q_DECL_EXPORT CCefViewEditor : public CCefView
{
protected:
    AscEditorType m_eType;

public:
    CCefViewEditor(CCefViewWidgetImpl* parent, int nId);
    virtual ~CCefViewEditor();

    void SetEditorType(AscEditorType eType);
    AscEditorType GetEditorType();
};

#endif  // CEFCLIENT_CEFWEBVIEW_H
