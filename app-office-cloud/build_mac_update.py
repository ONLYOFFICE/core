"""Small, reversible Mosaic update for an existing verified MIDIA Cloud Mac app."""
from pathlib import Path
import sys,shutil,subprocess,tempfile,plistlib,hashlib,json,os
root=Path(__file__).resolve().parents[1];out=Path(sys.argv[1]).resolve();source=Path(sys.argv[2]).resolve();out.mkdir(parents=True,exist_ok=True)
assert plistlib.loads((source/'Contents/Info.plist').read_bytes())['CFBundleVersion']=='563.3'
assert (source/'Contents/Resources/AppIcon.icns').read_bytes()==(root/'branding/office-mosaic/AppIcon.icns').read_bytes()
work=Path(tempfile.mkdtemp(prefix='office-mosaic-pkg-'));payload=work/'root';scripts=work/'scripts';scripts.mkdir()
for rel in ['Contents/Resources/AppIcon.icns','Contents/Resources/providers/merp','Contents/Resources/MIDIA-CLOUD.json']:
 target=payload/rel;target.parent.mkdir(parents=True,exist_ok=True)
 if (source/rel).is_dir():shutil.copytree(source/rel,target)
 else:shutil.copy2(source/rel,target)
common='''#!/bin/bash
set -euo pipefail
app="/Applications/ONLYOFFICE WPS.app"
state="/var/db/midia-office-mosaic.state"
'''
pre=common+'''test -d "$app" || { echo "Instala primero ONLYOFFICE WPS + MIDIA Cloud desde MERP." >&2; exit 1; }
version=$(/usr/libexec/PlistBuddy -c 'Print :CFBundleVersion' "$app/Contents/Info.plist")
[[ "$version" == "563.2" || "$version" == "563.3" ]] || { echo "Esta actualización requiere MIDIA Cloud 9.4.0.2 o 9.4.0.3." >&2; exit 1; }
if /usr/bin/pgrep -f "$app/Contents/MacOS/ONLYOFFICE WPS Native" >/dev/null; then echo "Guarda y sal de Office con Comando-Q antes de actualizar." >&2; exit 1; fi
/usr/bin/codesign --verify --deep --strict "$app"
test "$(/usr/bin/shasum -a 256 "$app/Contents/Resources/converter/x2t" | /usr/bin/awk '{print $1}')" = "7e0203dc42b79ad992401fd2f46d6c75914ef276eacd00607f1fd17fc3ce7749"
test ! -e "$state" || { echo "Hay una actualización anterior pendiente; consulta al administrador." >&2; exit 1; }
backup=$(/usr/bin/mktemp -d /var/tmp/midia-office-mosaic.XXXXXX)
for rel in "Contents/Resources/AppIcon.icns" "Contents/Resources/providers/merp" "Contents/Resources/MIDIA-CLOUD.json" "Contents/Info.plist" "Contents/_CodeSignature" "Contents/MacOS/ONLYOFFICE WPS"; do
 mkdir -p "$backup/$(dirname "$rel")"
 /usr/bin/ditto "$app/$rel" "$backup/$rel"
done
printf '%s' "$backup" > "$state"
chmod 600 "$state"
'''
post=common+'''backup=$(cat "$state")
rollback() {
 /bin/rm -rf "$app/Contents/Resources/providers/merp" "$app/Contents/_CodeSignature"
 /usr/bin/ditto "$backup/Contents" "$app/Contents"
 /bin/rm -rf "$backup"
 /bin/rm -f "$state"
 echo "La actualización no terminó; se ha restaurado la versión anterior." >&2
}
trap rollback ERR
/usr/libexec/PlistBuddy -c 'Set :CFBundleVersion 563.3' "$app/Contents/Info.plist"
/usr/bin/codesign --force --sign - "$app"
/usr/bin/codesign --verify --deep --strict "$app"
/usr/bin/touch "$app"
trap - ERR
/bin/rm -rf "$backup"
/bin/rm -f "$state"
'''
for name,text in [('preinstall',pre),('postinstall',post)]:p=scripts/name;p.write_text(text);p.chmod(0o755);subprocess.run(['bash','-n',str(p)],check=True)
pkg=out/'MIDIA-Office-Mosaico-9.4.0.3-macos-arm64.pkg'
subprocess.run(['pkgbuild','--root',str(payload),'--install-location','/Applications/ONLYOFFICE WPS.app','--identifier','com.midiashops.office.mosaic','--version','9.4.0.3','--ownership','recommended','--scripts',str(scripts),str(pkg)],check=True)
report={'filename':pkg.name,'bytes':pkg.stat().st_size,'sha256':hashlib.sha256(pkg.read_bytes()).hexdigest(),'requires_base':'ONLYOFFICE WPS + MIDIA Cloud 9.4.0.2','requires_office_closed':True,'changes':'icon, cloud provider graphics and bundle version only','converter_unchanged':True,'rollback':'backup resources, plist, outer signature and launcher; restore on postinstall error','signed':False,'build_workspace':str(work)}
(out/'mac-update-verification.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
