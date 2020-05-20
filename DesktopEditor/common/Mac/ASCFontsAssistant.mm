/*
 * (c) Copyright Ascensio System SIA 2010-2020
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

#import "ASCFontsAssistant.h"

#import "NSString+StringUtils.h"
#import "NSFileManager+Utils.h"

#import "../../../../core/DesktopEditor/fontengine/ApplicationFontsWorker.h"

@implementation ASCFontsAssistant

- (NSFonts::IApplicationFonts*)load {
    
    CApplicationFontsWorker worker;
    
    worker.m_bIsUseOpenType = true;
    worker.m_bIsUseSystemFonts = true;
    worker.m_bIsNeedThumbnails = false;
    worker.m_sDirectory = self.dataFontsPath.stdwstring;
    
    for (NSString * fontsPath in self.fontsPaths) {
        worker.m_arAdditionalFolders.push_back(fontsPath.stdwstring);
    }
    
    worker.m_arAdditionalFolders.push_back(std::wstring(L"/System/Library/Fonts"));
    
    NSFonts::IApplicationFonts* appFonts = worker.Check();
    
    std::vector<std::wstring> fontsNames = worker.GetFontNames(appFonts);
    const std::string fontsJs = worker.GetAllFonts();
    
    NSMutableArray* fonts = [NSString stringsArray:fontsNames];
    [fonts removeObject:@"ASCW3"];
    [fonts removeObject:@"LastResort"];
    
    NSMutableDictionary* uniqueFonts = [NSMutableDictionary dictionary];
    
    for (int i = (int)fonts.count - 1; i >= 0; --i) {
        NSString *fontName = (NSString *)fonts[i];
        
        if ([fontName rangeOfString:@"Noto Sans"].location != NSNotFound ||
            [fontName rangeOfString:@"OpenSymbol"].location != NSNotFound ||
            uniqueFonts[fontName] != nil) {
            [fonts removeObjectAtIndex:i];
        }
        
        uniqueFonts[fontName] = fontName;
    }
    
    self->_availableFonts = fonts;
    self->_allFontsScriptData = [NSString stringWithAString:fontsJs];
    
    return appFonts;
}

- (void)check {
    CApplicationFontsWorker worker;
    
    worker.m_bIsUseOpenType = true;
    worker.m_bIsUseSystemFonts  = true;
    worker.m_bIsNeedThumbnails  = false;
    worker.m_sDirectory = self.dataFontsPath.stdwstring;
    
    for (NSString * fontsPath in self.fontsPaths) {
        worker.m_arAdditionalFolders.push_back(fontsPath.stdwstring);
    }
    
    worker.m_arAdditionalFolders.push_back(std::wstring(L"/System/Library/Fonts"));
    
    NSFonts::IApplicationFonts* appFonts = worker.Check();
    
    RELEASEINTERFACE(appFonts);
}

@end

