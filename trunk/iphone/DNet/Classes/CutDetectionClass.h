/*
 * Copyright (c) 2007-2020 Texas A&M University System
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//
//  DNetViewController.h
//  DNet
//
//  Created by Harsha on 2/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AsyncUdpSocket.h"
#import <MessageUI/MFMailComposeViewController.h>

#define MAX_NBR_NUM 3

struct ntableentry
{
	uint8_t idee;
	uint8_t age;
	float state;
};
typedef struct ntableentry NEntry;

struct cutdet 
{
	uint8_t myid;
	CFSwappedFloat32 state;
}__attribute__ ((packed));

@interface CutDetectionClass : UIViewController <MFMailComposeViewControllerDelegate> {
	UITextView *_scrollview;
	UIButton *_startstopButton;
   	UIButton *_emailButton;
	UISegmentedControl * _roleChooser;
	AsyncUdpSocket *listenSocket;
	NSMutableArray *connectedSockets;
	UILabel * _currStateLabel;
	NEntry nbr[MAX_NBR_NUM];
}

@property (nonatomic,retain) IBOutlet UITextView *scrollview;
@property (nonatomic,retain) IBOutlet UIButton *startstopButton;
@property (nonatomic,retain) IBOutlet UIButton *emailButton;
@property (nonatomic,retain) IBOutlet UISegmentedControl *roleChooser;
@property (nonatomic,retain) IBOutlet UILabel *currStateLabel;

- (IBAction) startOrStopAction:(id)sender;
- (IBAction) emailAction:(id)sender;
@end

