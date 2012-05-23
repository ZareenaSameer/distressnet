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
function dist = getdistance(p1,p2)

% returns in miles...
% so this is in latitude and longitude....so....
% dist = sqrt((p1.x-p2.x)^2 + (p1.y-p2.y)^2);

lat1 = p1.y;
lon1 = p1.x;
lat2 = p2.y;
lon2 = p2.x;

%Convert to radians

latrad1 = lat1*pi/180;
lonrad1 = lon1*pi/180;
latrad2 = lat2*pi/180;
lonrad2 = lon2*pi/180;

londif = abs(lonrad2-lonrad1);

raddis = acos(sin(latrad2)*sin(latrad1)+cos(latrad2)*cos(latrad1)*cos(londif));

nautdis = raddis * 3437.74677;
statdis = nautdis * 1.1507794;
stdiskm = nautdis * 1.852;

dist = statdis;
% disp(' ')
% fprintf('Distance in radians: = %7.4f \n', raddis);
% fprintf('Distance in nautical miles: = %7.2f \n', nautdis);
% fprintf('Distance in statute miles: = %7.2f \n', statdis);
% fprintf('Distance in kilometers: = %7.2f \n', stdiskm);