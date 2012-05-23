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
function sumthruput = getinvaggthruput(x,G,selector_thresh)

% x is 1x2 vector
selector = [1 2 3]; % 2 sources, 1 dest

for i = 1:length(x)
    if (x(i) > selector_thresh)
        selector = [selector i+3];
    end
end

% flow is from 1-3 and 2-3 and 1-2

sumthruput = 300000/graphshortestpath(sparse(G(selector,selector)),1,3);
sumthruput = sumthruput + 90/graphshortestpath(sparse(G(selector,selector)),2,3);
sumthruput = sumthruput + 300000/graphshortestpath(sparse(G(selector,selector)),1,2);

% % this is the shortest delay, so invert it for thruput
% sumthruput = 1/sumthruput;

%optimizer will minimize and not maximize, so invert
sumthruput = 1/sumthruput;

% disconnections
if (sumthruput == Inf)
    sumthruput = 100*rand + 0.10;
end