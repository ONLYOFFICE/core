//
//  OfficeOdfConverter.h
//  OfficeOdfConverter
//
//  Created by alexey.musinov on 15/02/2019.
//  Copyright © 2019 Ascensio System SIA. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface OfficeOdfConverter : NSObject

- (int)docx2odt:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(BOOL)isTemplate;
- (int)xlsx2ods:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(BOOL)isTemplate;
- (int)pptx2odp:(NSString*)nsFrom nsTo:(NSString*)nsTo nsTemp:(NSString*)nsTemp nsFontPath:(NSString*)nsFontPath isTemplate:(BOOL)isTemplate;

@property (strong) NSString* password;
@property (assign) BOOL isNoBase64;

@end
