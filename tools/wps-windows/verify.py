"""Synthetic XLSX photo import and ordinary-format save/reopen checks."""
from pathlib import Path
import base64, json, shutil, subprocess, sys, xml.etree.ElementTree as E, zipfile
S='http://schemas.openxmlformats.org/spreadsheetml/2006/main'
R='http://schemas.openxmlformats.org/package/2006/relationships'
D='http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing'
NS={'s':S,'x':D}
PNG=bytes([137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,2,0,0,0,1,8,2,0,0,0,123,64,232,221,0,0,0,13,73,68,65,84,120,156,99,248,207,0,4,255,1,7,0,1,255,226,35,158,89,0,0,0,0,73,69,78,68,174,66,96,130])


def convert(exe,src,dst,fmt):
    task=E.Element('TaskQueueDataConvert')
    for key,value in {'m_sFileFrom':src,'m_sFileTo':dst,'m_nFormatTo':fmt,'m_bIsNoBase64':'true'}.items(): E.SubElement(task,key).text=str(value)
    params=dst.with_suffix(dst.suffix+'.xml');E.ElementTree(task).write(params,encoding='utf-8',xml_declaration=True)
    result=subprocess.run([str(exe),str(params)],capture_output=True,text=True)
    assert result.returncode==0 and dst.exists(),(src,dst,result.returncode,result.stdout,result.stderr)


def fixture(template,dest):
    with zipfile.ZipFile(template) as z: parts={n:z.read(n) for n in z.namelist()}
    rels=E.fromstring(parts['xl/_rels/workbook.xml.rels'])
    E.SubElement(rels,'{'+R+'}Relationship',Id='wpsPhotos',Type='http://www.wps.cn/officeDocument/2020/cellImage',Target='photos.xml')
    parts['xl/_rels/workbook.xml.rels']=E.tostring(rels)
    pic='<xdr:pic><xdr:nvPicPr><xdr:cNvPr id="42" name="ID_PHOTO"/><xdr:cNvPicPr/></xdr:nvPicPr><xdr:blipFill><a:blip r:embed="rId1"/><a:stretch><a:fillRect/></a:stretch></xdr:blipFill><xdr:spPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="1000" cy="500"/></a:xfrm><a:prstGeom prst="rect"/></xdr:spPr></xdr:pic>'
    namespaces='xmlns:xdr="'+D+'" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"'
    parts['xl/photos.xml']=('<etc:cellImages xmlns:etc="http://www.wps.cn/officeDocument/2017/etCustomData" '+namespaces+'><etc:cellImage>'+pic+'</etc:cellImage></etc:cellImages>').encode()
    parts['xl/_rels/photos.xml.rels']=('<Relationships xmlns="'+R+'"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/image" Target="media/photo.png"/></Relationships>').encode()
    parts['xl/worksheets/sheet1.xml']=('''<worksheet xmlns="'''+S+'''" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"><cols><col min="2" max="2" width="20"/></cols><sheetData><row r="3" ht="80"><c r="A3"><v>17</v></c><c r="B3" t="e"><f>_xlfn.DISPIMG("ID_PHOTO",1)</f><v>#NAME?</v></c><c r="C3" t="e"><f>_xlfn.DISPIMG("MISSING",1)</f><v>#NAME?</v></c><c r="D3"><f>SUM(A3:A3)</f><v>17</v></c></row><row r="8" ht="80"><c r="B8" t="e"><f>_xlfn.DISPIMG("ID_PHOTO",1)</f><v>#NAME?</v></c></row></sheetData><drawing r:id="rId1"/></worksheet>''').encode()
    parts['xl/worksheets/_rels/sheet1.xml.rels']=('<Relationships xmlns="'+R+'"><Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing" Target="../drawings/drawing1.xml"/></Relationships>').encode()
    parts['xl/drawings/drawing1.xml']=('<xdr:wsDr '+namespaces+'><xdr:oneCellAnchor><xdr:from><xdr:col>0</xdr:col><xdr:colOff>0</xdr:colOff><xdr:row>0</xdr:row><xdr:rowOff>0</xdr:rowOff></xdr:from><xdr:ext cx="1000" cy="500"/>'+pic+'<xdr:clientData/></xdr:oneCellAnchor></xdr:wsDr>').encode()
    parts['xl/drawings/_rels/drawing1.xml.rels']=parts['xl/_rels/photos.xml.rels'].replace(b'media/photo.png',b'../media/photo.png')
    parts['xl/media/photo.png']=PNG
    types=E.fromstring(parts['[Content_Types].xml']);ns='http://schemas.openxmlformats.org/package/2006/content-types'
    E.SubElement(types,'{'+ns+'}Override',PartName='/xl/drawings/drawing1.xml',ContentType='application/vnd.openxmlformats-officedocument.drawing+xml')
    E.SubElement(types,'{'+ns+'}Override',PartName='/xl/photos.xml',ContentType='application/xml')
    if not any(x.get('Extension')=='png' for x in types): E.SubElement(types,'{'+ns+'}Default',Extension='png',ContentType='image/png')
    parts['[Content_Types].xml']=E.tostring(types)
    with zipfile.ZipFile(dest,'w',zipfile.ZIP_DEFLATED) as z:
        for name,data in parts.items():z.writestr(name,data)


def main():
    converter=Path(sys.argv[1]).resolve();work=Path(sys.argv[2]).resolve();work.mkdir(parents=True,exist_ok=True)
    exe=converter/'x2t.exe';templates=converter/'empty/en-US'
    source=work/'WPS-fotos-prueba.xlsx';fixture(templates/'new.xlsx',source)
    binary=work/'Editor.bin';saved=work/'WPS-fotos-guardadas.xlsx'
    convert(exe,source,binary,8194);convert(exe,binary,saved,257);convert(exe,saved,work/'Reopened.bin',8194)
    with zipfile.ZipFile(saved) as z:
        sheet=E.fromstring(z.read('xl/worksheets/sheet1.xml'));cells={c.get('r'):c for c in sheet.findall('.//s:c',NS)}
        for address in ['B3','B8']:assert cells.get(address) is None or cells[address].find('s:f',NS) is None,address
        assert cells['D3'].find('s:f',NS).text=='SUM(A3:A3)'
        assert cells['A3'].find('s:v',NS).text=='17'
        assert 'MISSING' in cells['C3'].find('s:f',NS).text
        anchors=[a for name in z.namelist() if name.startswith('xl/drawings/drawing') and name.endswith('.xml') for a in E.fromstring(z.read(name)) if a.find('x:pic',NS) is not None]
        assert len(anchors)==3,len(anchors)
        positions={(int(a.find('x:from/x:row',NS).text),int(a.find('x:from/x:col',NS).text)) for a in anchors}
        assert positions=={(0,0),(2,1),(7,1)},positions
        ids=[a.find('x:pic/x:nvPicPr/x:cNvPr',NS).get('id') for a in anchors];assert len(set(ids))==3,ids
    for ext,binfmt,savefmt in [('docx',8193,65),('pptx',8195,129),('xlsx',8194,257)]:
        folder=work/ext;folder.mkdir(exist_ok=True);src=folder/('Normal.'+ext);shutil.copy2(templates/('new.'+ext),src)
        convert(exe,src,folder/'Editor.bin',binfmt);convert(exe,folder/'Editor.bin',folder/('Saved.'+ext),savefmt);convert(exe,folder/('Saved.'+ext),folder/'Reopened.bin',binfmt)
    report={'WPS_photos':'passed','existing_drawing':'preserved','row_coordinates':'preserved','unrelated_formula_and_value':'preserved','missing_image_formula':'preserved','save_reopen':'passed','ordinary_DOCX_PPTX_XLSX':'passed'}
    (work/'verification.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))

if __name__=='__main__':main()
