//
//  TestDocs3ViewController.m
//  TestDocs3
//
//  Created by naceka on 24.04.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "TestDocs3ViewController.h"

@implementation TestDocs3ViewController


/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://html5.teamlab.com"]]];
	
	////NSString *path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html" inDirectory:@"v143mobile/apps/documenteditortouch"];
	//NSString *path = [[NSBundle mainBundle] pathForResource:@"apps/documenteditortouch/index" ofType:@"html" inDirectory:@"v143mobile"];	NSURL *url = [NSURL fileURLWithPath:path];
	//NSURLRequest *request = [NSURLRequest requestWithURL:url];
	//[webView loadRequest:request];
	 
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

@end
