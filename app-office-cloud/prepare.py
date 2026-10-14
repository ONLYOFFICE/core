"""Add MIDIA provider and mandatory local autosave to a verified vendor runtime."""
from pathlib import Path
import base64,json,shutil,sys

def prepare(root):
    root=Path(root)
    resources=root/'Contents/Resources' if (root/'Contents').exists() else root
    providers=resources/'providers/merp';providers.mkdir(parents=True,exist_ok=True)
    (providers/'config.json').write_text(json.dumps({'provider':'merp','name':'MIDIA Cloud','entryPage':'https://gestion.midiashops.com','startPage':'/diseno-v2/office-cloud.html','editorPage':'/diseno-v2/office-cloud-editor/','order':0,'icons':{'themeLight':{'connectionsList':'./cloud.png','buttonLogo':'./button.svg'},'themeDark':{'connectionsList':'./cloud.png','buttonLogo':'./button.svg'}}},indent=2))
    branding=Path(__file__).resolve().parents[1]/'branding/office-mosaic'
    shutil.copy2(branding/'AppIcon.ico',resources/'MIDIA-Office.ico')
    shutil.copy2(branding/'master.png',providers/'cloud.png')
    icon=base64.b64encode((branding/'master.png').read_bytes()).decode()
    (providers/'button.svg').write_text('<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="148" height="38" viewBox="0 0 148 38"><image x="1" y="1" width="36" height="36" xlink:href="data:image/png;base64,'+icon+'"/><text x="40" y="24" font-family="Arial,sans-serif" font-size="16" font-weight="600" fill="#2b2e36">MIDIA Cloud</text></svg>')
    counts={}
    for app in ['documenteditor','spreadsheeteditor','presentationeditor','pdfeditor']:
        p=resources/'editors/web-apps/apps'/app/'main/app.js'
        if not p.exists():continue
        s=p.read_text(encoding="utf-8");needle='this.api.asc_setAutoSaveGap(';start=0;count=0
        while True:
            at=s.find(needle,start)
            if at<0:break
            pos=at+len(needle);depth=1;quote=None;escape=False
            end=pos
            while depth:
                c=s[end]
                if quote:
                    if escape:escape=False
                    elif c=='\\':escape=True
                    elif c==quote:quote=None
                elif c in "\"'`":quote=c
                elif c=='(':depth+=1
                elif c==')':depth-=1
                end+=1
            # The desktop converter keeps editing local; the companion owns server versions.
            s=s[:pos]+'10'+s[end-1:];start=pos+3;count+=1
        if app!='pdfeditor':assert count>0,(app,'autosave anchor missing')
        ready='onDocumentContentReady:function(){'
        marker='/* MIDIA native file autosave */'
        if marker not in s:
            assert s.count(ready)==1,(app,'native autosave ready anchor')
            timer='if(!this.__midiaTimer){var __midia=this;this.__midiaTimer=setInterval(function(){var a=__midia.api,n=window.AscDesktopEditor;try{if(n&&n.IsLocalFile()&&n.LocalFileGetSaved()&&a&&__midia.appOptions&&__midia.appOptions.isEdit&&a.asc_isDocumentCanSave()&&(!a.asc_getLocalRestrictions||a.asc_getLocalRestrictions()===Asc.c_oAscLocalRestrictionType.None))a.asc_Save();}catch(e){console.warn("MIDIA autosave pending");}},10000);}'
            s=s.replace(ready,ready+marker+timer,1)
        p.write_text(s,encoding="utf-8");counts[app]=count
    assert len(counts)>=3,counts
    (resources/'MIDIA-CLOUD.json').write_text(json.dumps({'version':'9.4.0.3','autosave_gap_seconds':10,'sync_poll_seconds':5,'cloud':'https://gestion.midiashops.com/diseno-v2/office-cloud.html','autosave_calls':counts},indent=2))
    return counts
if __name__=='__main__':print(json.dumps(prepare(sys.argv[1])))
