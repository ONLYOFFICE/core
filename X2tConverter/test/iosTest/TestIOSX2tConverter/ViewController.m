//
//  ViewController.m
//  TestIOSX2tConverter
//
//  Created by alexey.musinov on 01.04.15.
//  Copyright (c) 2015 Ascensio System SIA. All rights reserved.
//

#import "ViewController.h"

#import "X2tConverter.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self testXLSX];
}

- (void)testDOCX {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"Demo-Hayden-Management-v2" ofType:@"docx"];
    
    NSString* fullFileName = path;
    NSString* fullFileNameTo = [NSString stringWithFormat:@"%@sample.bin", NSTemporaryDirectory()];
    NSString* tempDir = NSTemporaryDirectory();
    NSString* fontsPath = @"/System/Library/Fonts";
    
    X2tConverter* conv = [[X2tConverter alloc]init];
    [conv sdk_docx2doct_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];
    
    NSLog(@"%@",fullFileNameTo);
    
    NSString* docxOut = [NSString stringWithFormat:@"%@document.docx", NSTemporaryDirectory()];
    NSString* outTemp = [NSString stringWithFormat:@"%@OUT_TEMP", NSTemporaryDirectory()];
    
    [self createDirectory:outTemp];
    [self clearDirectory:outTemp];
    
    X2tConverter* conv2 = [[X2tConverter alloc]init];
    [conv2 sdk_doct_bin2docx:fullFileNameTo nsTo:docxOut nsTemp:outTemp nsFontPath:fontsPath fromChanges:@(NO) nsThemeDir:@""];
    
    NSLog(@"%@",fullFileNameTo);
}
- (void)testXLSX {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"price" ofType:@"xlsx"];
    
    NSString* fullFileName = path;
    NSString* fullFileNameTo = [NSString stringWithFormat:@"%@sample.bin", NSTemporaryDirectory()];
    NSString* tempDir = NSTemporaryDirectory();
    NSString* fontsPath = @"/System/Library/Fonts";
    
    X2tConverter* conv = [[X2tConverter alloc]init];
    [conv sdk_xlsx2xlst_bin:fullFileName nsTo:fullFileNameTo nsTemp:tempDir nsFontPath:fontsPath];    
  
    NSLog(@"%@",fullFileNameTo);
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
