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
function distance = getpathdistance(source,destination,vp)


% divide up vehicles path into segments, then see if destination lies on
% the segments....
% disp('>>> finding segment for source')
seg_source = findlinesegment(source,vp);
% disp('>>> finding segment for dest')
seg_dest = findlinesegment(destination,vp);

% fprintf('segments are %d to %d\n', seg_source, seg_dest);

% segment 1 is from point 1 to point 2
% segment 2 is from 2 to 3, so segment n is from point n to n+1

numsegs = length(vp) - 1; % since last point = first point

distance = 0;

if (seg_source == seg_dest)
    % now see which comes first
    dist1 = getdistance(source, vp(seg_source));
    dist2 = getdistance(destination, vp(seg_source));
    if (dist2 > dist1)
        % O-----source-------dest------O
        distance = dist2 - dist1;
    else
        % O-----dest-------source------O
        curr_seg = seg_source;
        distance = distance + getdistance(source, vp(curr_seg+1));

        curr_seg = curr_seg + 1;
        while (curr_seg ~= seg_dest)
            distance = distance + getdistance(vp(curr_seg),vp(curr_seg+1));
            curr_seg = curr_seg + 1;
            if (curr_seg > numsegs)
                curr_seg = 1;
            end
        end
        distance = distance + getdistance(vp(seg_dest), destination);
    end
else
    curr_seg = seg_source;
    distance = distance + getdistance(source, vp(curr_seg+1));

    curr_seg = curr_seg + 1;
    while (curr_seg ~= seg_dest)
        distance = distance + getdistance(vp(curr_seg),vp(curr_seg+1));
        curr_seg = curr_seg + 1;
        if (curr_seg > numsegs)
            curr_seg = 1;
        end
    end
    distance = distance + getdistance(vp(seg_dest), destination);
end



% TODO: go in a loop, cross end point etc
% curr_seg = seg_source+1

% for i=seg_source+1:seg_dest-1
%     distance = distance + getdistance(vp(i),vp(i+1));
% end
% distance = distance + getdistance(vp(seg_dest), destination);

