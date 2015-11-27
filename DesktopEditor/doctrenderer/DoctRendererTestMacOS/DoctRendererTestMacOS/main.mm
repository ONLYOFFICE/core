//
//  main.m
//  DoctRendererTestMacOS
//
//  Created by Oleg Korshul on 23.11.15.
//  Copyright © 2015 Ascensio System. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "../../DoctRenderer.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {

        std::wstring strXml = L"<Settings>\
        <SrcFileType>0</SrcFileType>\
        <DstFileType>0</DstFileType>\
        <SrcFilePath>/Users/Oleg/Desktop/activex/TEST_FILES/build_doc</SrcFilePath>\
        <DstFilePath>/Users/Oleg/Desktop/activex/TEST_FILES/build_doc/EditorWithChanges.bin</DstFilePath>\
        <FontsDirectory>/Users/Oleg/Desktop/activex/TEST_FILES/activex/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Fonts/native</FontsDirectory>\
        <ImagesDirectory>/Users/Oleg/Desktop/activex/TEST_FILES/build_doc\\media</ImagesDirectory>\
        <ThemesDirectory>/Users/Oleg/Desktop/activex/TEST_FILES/themes</ThemesDirectory>\
        <Changes TopItem=\"-1\">\
        <Change>/Users/Oleg/Desktop/activex/TEST_FILES/build_doc/changes/changes0.json</Change>\
        </Changes>\
        </Settings>";

        NSDoctRenderer::CDoctrenderer oRenderer;
        
        std::wstring sError;
        bool bError = oRenderer.Execute(strXml, sError);
        
        NSLog(bError ? @"OK" : @"ERROR");
    }
    return 0;
}
