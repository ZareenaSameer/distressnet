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
//  BTagprogrammer-iPad.h
//  DNet
//
//  Created by Harsha on 5/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AsyncUdpSocket.h"
#import "LocationController.h"

@interface BTagprogrammer_iPad : UIViewController <UITextFieldDelegate>

{
   	AsyncUdpSocket *listenSocket;
}
- (IBAction)sliderChanged:(id)sender;
@property (retain, nonatomic) IBOutlet UILabel *livingLabel;
@property (retain, nonatomic) IBOutlet UILabel *deadLabel;

@property (retain, nonatomic) IBOutlet UIView *mainview;
@property (retain, nonatomic) IBOutlet UILabel *yourlocationLabel;
@property (retain, nonatomic) IBOutlet UISlider *deadSlider;
@property (retain, nonatomic) IBOutlet UISlider *livingSlider;
@property (retain, nonatomic) IBOutlet UISegmentedControl *hazardChooser;
- (IBAction)programPressed:(id)sender;
@property (retain, nonatomic) IBOutlet UISegmentedControl *taskforceChooser;
@property (retain, nonatomic) IBOutlet UISlider *idSlider;
@property (retain, nonatomic) IBOutlet UILabel *dentlabel;
@property (retain, nonatomic) IBOutlet UILabel *tentlabel;
@property (retain, nonatomic) IBOutlet UILabel *dexitlabel;
@property (retain, nonatomic) IBOutlet UILabel *texitlabel;
@property (retain, nonatomic) IBOutlet UILabel *idLabel;
@property (retain, nonatomic) IBOutlet UITextField *addressField;


@end
