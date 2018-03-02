/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
//
//  ViewController.m
//  TestIOSX2tConverter
//
//  Created by alexey.musinov on 01.04.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import "ViewController.h"

#import "X2tConverter.h"
#import "OfficeFileErrorDescription.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self testDOCX];
    [self testXLSX];
}

- (void)testDOCX
{
    {
        NSLog(@"==================== OPEN EMPTY DOCX ====================");
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"empty" ofType:@"docx"];
        
        NSString* fullFileName = path;
        NSString* fullFileNameTo = [NSString stringWithFormat:@"%@empty.bin", NSTemporaryDirectory()];
        NSString* tempDir = NSTemporaryDirectory();
        NSString* fontsPath = @"/System/Library/Fonts";
        
        X2tConverter* conv = [[X2tConverter alloc]init];
        conv.isNoBase64 = YES;
        int result = [conv sdk_docx2doct_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];
        if (result != 0) {
            NSLog(@"ERROR OPEN EMPTY DOCX : %d",result);
        } else {
            NSLog(@"doct output : %@", fullFileNameTo);
        }
    }
    
    {
        NSLog(@"==================== OPEN DOCX ====================");
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"demo" ofType:@"docx"];
        
        NSString* fullFileName = path;
        NSString* fullFileNameTo = [NSString stringWithFormat:@"%@demo.bin", NSTemporaryDirectory()];
        NSString* tempDir = NSTemporaryDirectory();
        NSString* fontsPath = @"/System/Library/Fonts";
        
        X2tConverter* conv = [[X2tConverter alloc]init];
        conv.isNoBase64 = YES;
        int result = [conv sdk_docx2doct_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];
        if (result != 0) {
            NSLog(@"ERROR OPEN DOCX : %d",result);
        } else {
            NSLog(@"doct output : %@", fullFileNameTo);
        }
    }
    
    {
        NSLog(@"==================== OPEN DOCT ====================");
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"demo" ofType:@"docx"];
        
        NSString* fullFileName = path;
        NSString* fullFileNameTo = [NSString stringWithFormat:@"%@demo.bin", NSTemporaryDirectory()];
        NSString* tempDir = NSTemporaryDirectory();
        NSString* fontsPath = @"/System/Library/Fonts";
        
        X2tConverter* conv = [[X2tConverter alloc]init];
        conv.isNoBase64 = YES;
        [conv sdk_docx2doct_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];
        
        NSLog(@"%@",fullFileNameTo);
        
        NSString* docxOut = [NSString stringWithFormat:@"%@document.docx", NSTemporaryDirectory()];
        NSString* outTemp = [NSString stringWithFormat:@"%@OUT_TEMP", NSTemporaryDirectory()];
        
        [self createDirectory:outTemp];
        [self clearDirectory:outTemp];
        
        X2tConverter* conv2 = [[X2tConverter alloc]init];
        conv.isNoBase64 = YES;
        [conv2 sdk_doct_bin2docx:fullFileNameTo nsTo:docxOut nsTemp:outTemp nsFontPath:fontsPath fromChanges:@(NO) nsThemeDir:@""];
        
        NSLog(@"doct output: %@",fullFileNameTo);
    }
}

- (void)testXLSX
{
    {
        NSLog(@"==================== OPEN EMPTY XLSX ====================");
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"empty" ofType:@"xlsx"];
        
        NSString* fullFileName = path;
        NSString* fullFileNameTo = [NSString stringWithFormat:@"%@empty.bin", NSTemporaryDirectory()];
        NSString* tempDir = NSTemporaryDirectory();
        NSString* fontsPath = @"/System/Library/Fonts";
        
        X2tConverter* conv = [[X2tConverter alloc]init];
        conv.isNoBase64 = YES;
        int result = [conv sdk_xlsx2xlst_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];
        if (result != 0) {
            NSLog(@"ERROR OPEN EMPTY XLSX : %d",result);
        } else {
            NSLog(@"xlst output : %@", fullFileNameTo);
        }
    }
    
    {
        NSLog(@"==================== OPEN XLSX ====================");
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"price" ofType:@"xlsx"];
        
        NSString* fullFileName = path;
        NSString* fullFileNameTo = [NSString stringWithFormat:@"%@price.bin", NSTemporaryDirectory()];
        NSString* tempDir = NSTemporaryDirectory();
        NSString* fontsPath = @"/System/Library/Fonts";
        
        X2tConverter* conv = [[X2tConverter alloc]init];
        conv.isNoBase64 = YES;
        int result = [conv sdk_xlsx2xlst_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];
        if (result != 0) {
            NSLog(@"ERROR OPEN XLSX : %d",result);
        } else {
            NSLog(@"xlst output : %@", fullFileNameTo);
        }
    }
    
    {
        NSLog(@"==================== OPEN CRYPTED XLSX ====================");
        
        NSString *path = [[NSBundle mainBundle] pathForResource:@"crypted" ofType:@"xlsx"];
        
        NSString* fullFileName = path;
        NSString* fullFileNameTo = [NSString stringWithFormat:@"%@crypted.bin", NSTemporaryDirectory()];
        NSString* tempDir = NSTemporaryDirectory();
        NSString* fontsPath = @"/System/Library/Fonts";
        
        X2tConverter* conv = [[X2tConverter alloc]init];
        conv.password = @"555";
        conv.isNoBase64 = YES;
        if ((int)AVS_FILEUTILS_ERROR_CONVERT_PASSWORD == [conv sdk_xlsx2xlst_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath]) {
            NSLog(@"Error password : %@",conv.password);
        }
        
        NSLog(@"xlst output : %@", fullFileNameTo);
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

#pragma mark -
#pragma Utils

- (void)clearDirectory:(NSString*)directory
{
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *error = nil;
    for (NSString *file in [fm contentsOfDirectoryAtPath:directory error:&error]) {
        BOOL success = [fm removeItemAtPath:[NSString stringWithFormat:@"%@/%@", directory, file] error:&error];
        if (!success || error) {
            NSLog(@"Failed to clear directory \"%@\". Error: %@", directory, error);
        }
    }
}

- (void)createDirectory:(NSString*)directory
{
    NSFileManager *fileManager= [NSFileManager defaultManager];
    NSError *error = nil;
    if(![fileManager createDirectoryAtPath:directory withIntermediateDirectories:YES attributes:nil error:&error]) {
        // An error has occurred, do something to handle it
        NSLog(@"Failed to create directory \"%@\". Error: %@", directory, error);
    }
}

@end
