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
//  BtagAnnotation.m
//  DNet
//
//  Created by Harsha on 5/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "BtagAnnotation.h"

@implementation BtagAnnotation

@synthesize coordinate = _coordinate;

- (id)initWithData:(btagloc_t*)data {
    if ((self = [super init])) {
        
        memcpy(&_btag, &data->the_btag, sizeof(btag_t));
        
        CLLocationCoordinate2D temper;
        CFSwappedFloat32 lala;
        lala.v = data->location.loc_x;
        
        temper.latitude = CFConvertFloat32SwappedToHost(lala);
        lala.v = data->location.loc_y;
        temper.longitude = CFConvertFloat32SwappedToHost(lala);
        
        _coordinate = temper;
     
        _btag.mote_id = ntohs(_btag.mote_id);
        
    }
    return self;
}

- (NSString *)title {
    
    return FORMAT(@"BTAG %d\n", _btag.mote_id);

}

- (NSString *)subtitle {
    
        
    
    return FORMAT(@"%+.6f %+.6f\n #%d %s %s ent: %s %s ext: %s %s haz %s, %d Alive %d Dead", _coordinate.latitude, _coordinate.longitude, _btag.mote_id, _btag.address, _btag.task_force, _btag.date_entered, _btag.time_entered, _btag.date_exited, _btag.time_exited, _btag.hazards, _btag.living, _btag.dead);
    

}
@end
