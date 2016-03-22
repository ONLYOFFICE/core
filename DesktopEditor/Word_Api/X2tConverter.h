//
//  X2tConverter.h
//  X2tConverter
//
//  Created by alexey.musinov on 25.03.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface X2tConverter : NSObject

- (int)sdk_docx2doct_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_docx2doct:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_doct_bin2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;
- (int)sdk_doct2docx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;

- (int)sdk_xlsx2xlst_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlsx2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlst_bin2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;
- (int)sdk_xlst2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;

- (int)sdk_pptx2pptt_bin:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_pptx2pptt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_pptt_bin2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;
- (int)sdk_pptt2pptx:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath fromChanges:(NSNumber*)fromChanges nsThemeDir:(NSString*)nsThemeDir;

- (int)sdk_csv2xlst:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_csv2xlsx:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlst2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;
- (int)sdk_xlsx2csv:(NSString*)nsFrom nsTo:(NSString*)nsTo xmlOptions:(NSString*)xmlOptions nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath;

- (int)sdk_dir2zip:(NSString*)nsFrom nsTo:(NSString*)nsTo;
- (int)sdk_zip2dir:(NSString*)nsFrom nsTo:(NSString*)nsTo;

@end
