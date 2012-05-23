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
//  BTagProgrammer.h
//  DNet
//
//  Created by Harsha Chenji on 3/5/11.
//  Copyright 2011 Texas A&M. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "Common.h"
#import "AsyncUdpSocket.h"
#import "BuildingTagClass.h"

@interface BTagProgrammer : UIViewController <UITextFieldDelegate> {
    
	UILabel * _idLabel;
	UISlider * _idSlider;
	UITextField * _addr1Field;
	UITextField * _addr2Field;
	UISegmentedControl * _teamField;
	UILabel * _livingLabel;
	UISlider * _livingSlider;
	UILabel * _deadLabel;
	UISlider * _deadSlider;
	UILabel * _searchLabel;
	UISlider * _searchSlider;
	
    UILabel * _locLabel;
    
	UIButton * _sendButton;
	
	AsyncUdpSocket *listenSocket;
	NSMutableArray *connectedSockets;
}

- (void) doResocket;

@property (nonatomic,retain) IBOutlet UILabel * idLabel;
@property (nonatomic,retain) IBOutlet UILabel * livingLabel;
@property (nonatomic,retain) IBOutlet UILabel * deadLabel;
@property (nonatomic,retain) IBOutlet UILabel * searchLabel;

@property (nonatomic,retain) IBOutlet UISlider * idSlider;
@property (nonatomic,retain) IBOutlet UISlider * livingSlider;
@property (nonatomic,retain) IBOutlet UISlider * deadSlider;
@property (nonatomic,retain) IBOutlet UISlider * searchSlider;

@property (nonatomic,retain) IBOutlet UISegmentedControl * teamField;
@property (nonatomic,retain) IBOutlet UITextField * addr1Field;
@property (nonatomic,retain) IBOutlet UITextField * addr2Field;
@property (nonatomic,retain) IBOutlet UIButton * sendButton;
@property (retain, nonatomic) IBOutlet UILabel *loclabel;
@property (retain, nonatomic) IBOutlet UISegmentedControl *hazardField;

- (IBAction) sliderAction:(id)sender;
- (IBAction) sendAction:(id)sender;

@end
