"""Add MIDIA provider and mandatory local autosave to a verified vendor runtime."""
from pathlib import Path
import json,sys

def prepare(root):
    root=Path(root)
    resources=root/'Contents/Resources' if (root/'Contents').exists() else root
    providers=resources/'providers/merp';providers.mkdir(parents=True,exist_ok=True)
    (providers/'config.json').write_text(json.dumps({'provider':'merp','name':'MIDIA Cloud','entryPage':'https://gestion.midiashops.com/diseno-v2/office-cloud.html','editorPage':'/diseno-v2/office-cloud-editor/','order':0,'icons':{'themeLight':{'connectionsList':'./cloud.svg','buttonLogo':'./button.svg'},'themeDark':{'connectionsList':'./cloud.svg','buttonLogo':'./button.svg'}}},indent=2))
    (providers/'cloud.svg').write_text('<svg xmlns="http://www.w3.org/2000/svg" width="36" height="36" viewBox="0 0 36 36"><rect width="36" height="36" rx="8" fill="#0b747a"/><path d="M10 25h16a5 5 0 0 0 0-10 8 8 0 0 0-15-2A6 6 0 0 0 10 25Z" fill="white"/><path d="M18 16v7m-3-4 3-3 3 3" stroke="#0b747a" fill="none" stroke-width="2"/></svg>')
    (providers/'button.svg').write_text('<svg xmlns="http://www.w3.org/2000/svg" width="148" height="38" viewBox="0 0 148 38"><rect x="3" y="4" width="30" height="30" rx="7" fill="#0b747a"/><path d="M11 25h15a4 4 0 0 0-1-8 7 7 0 0 0-13-1 5 5 0 0 0-1 9Z" fill="white"/><text x="40" y="24" font-family="Arial,sans-serif" font-size="16" font-weight="600" fill="#2b2e36">MIDIA Cloud</text></svg>')
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
        p.write_text(s,encoding="utf-8");counts[app]=count
    assert len(counts)>=3,counts
    (resources/'MIDIA-CLOUD.json').write_text(json.dumps({'version':'9.4.0.2','autosave_gap_seconds':10,'sync_poll_seconds':5,'cloud':'https://gestion.midiashops.com/diseno-v2/office-cloud.html','autosave_calls':counts},indent=2))
    return counts
if __name__=='__main__':print(json.dumps(prepare(sys.argv[1])))
