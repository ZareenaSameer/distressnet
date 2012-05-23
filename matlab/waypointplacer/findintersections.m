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
%% preliminaries and loading data
clear

numvehs = 3;
veh = [];

% make sure last point of veh trace is first point also
for i=1:numvehs
    filen = sprintf('./temp/veh%d.kml', i);
    tempveh = kml_shapefile(filen);
    tempveh.X = [tempveh.X; tempveh.X(1)];
    tempveh.Y = [tempveh.Y; tempveh.Y(1)];
    tempveh.pointarray = getpointarray(tempveh);
    tempveh.speed = 20;
    tempveh.pointsonpath = [];
    veh = [veh tempveh];
end

% make points coincide
veh(2).X(28) = veh(1).X(7);
veh(2).Y(28) = veh(1).Y(7);

veh(3).X(8) = veh(1).X(7);
veh(3).Y(8) = veh(1).Y(7);


veh(3).X(13) = veh(2).X(23);
veh(3).Y(13) = veh(2).Y(23);

veh(1).pointarray = getpointarray(veh(1));
veh(2).pointarray = getpointarray(veh(2));
veh(3).pointarray = getpointarray(veh(3));

dest.x = veh(1).X(27);
dest.y = veh(1).Y(27);

source(1).x = veh(2).X(23);
source(1).y = veh(2).Y(23);

source(2).x = veh(3).X(18);
source(2).y = veh(3).Y(18);



%% main algo, first find unique intersection points, build master list

disp('--------------------------------------')

% first, find intersection points between vehicles
% store points as [x y; x y] so that unique can be applied
tempints = [];

for i=1:numvehs-1
    for j=i+1:numvehs
        [xints,yints] = intersections(veh(i).X',veh(i).Y',veh(j).X',veh(j).Y');        
        tempints = [tempints; [xints yints]];
    end
end

% keep only unique intersection points
temper = unique(tempints, 'rows');

%kluge
temper(2,:) = [];

tempintersecns.X = temper(:,1);
tempintersecns.Y = temper(:,2);
intersecns = getpointarray(tempintersecns);

% we now have our intersections. 
% but first, add source and dest to a master point list

allpoints = [source(1) source(2) dest intersecns];
allpoints(1).vehs = [];

for i=1:length(allpoints)
    for j=1:numvehs
        if (findlinesegment(allpoints(i),veh(j).pointarray) ~= 0)
            allpoints(i).vehs = [allpoints(i).vehs j];
            veh(j).pointsonpath = [veh(j).pointsonpath i];
        end
    end
%     fprintf('point %d has vehs %d\n', i, allpoints(i).vehs);
end




%% plot all this to visually verify

figure
hold on
plot(veh(1).X',veh(1).Y', 'r')
plot(veh(2).X',veh(2).Y', 'b')
plot(veh(3).X',veh(3).Y', 'y')
plot(temper(:,1)',temper(:,2)', 'gd',   'MarkerEdgeColor','k','MarkerFaceColor','g','MarkerSize',10)

plot(source(1).x,source(1).y, 'm^',   'MarkerEdgeColor','m','MarkerFaceColor','m','MarkerSize',10)
plot(source(2).x,source(2).y, 'm^',   'MarkerEdgeColor','m','MarkerFaceColor','m','MarkerSize',10)
plot(dest.x, dest.y, 'ro',   'MarkerEdgeColor','r','MarkerFaceColor','r','MarkerSize',10)

for i=1:length(allpoints)
    text(allpoints(i).x, allpoints(i).y, int2str(i), 'FontSize',16);
end

for i=1:length(veh)
    for j=1:length(veh(i).X')
%         text(veh(i).X(j), veh(i).Y(j), int2str(j), 'FontSize',16);
    end
end

print -dpng all.png

%% now to build the graph

fprintf('source lies on veh%d, dest on veh%d\n', allpoints(1).vehs, allpoints(2).vehs);
G = ones(length(allpoints), length(allpoints)).*Inf;
Gv = G;

% for every vehicle, see all points on it, do exhaustive pairing

for i=1:numvehs
    temparr = veh(i).pointsonpath;
    for j=1:length(temparr)-1
        for k=j+1:length(temparr)
            jt = temparr(j);
            kt = temparr(k);
            
%             fprintf('doing veh %d p%d p%d\n', i, jt, kt);
            % points jt and kt on vehicle i...go for it
            riddhi = getpathdistance(allpoints(jt), allpoints(kt), veh(i).pointarray)/veh(i).speed;
            fprintf('p%d to p%d, veh %d time is %f\n', jt, kt, i, riddhi);
            
            if (riddhi < G(jt,kt))
                G(jt,kt) = riddhi;
                Gv(jt,kt) = i;
            end
            
            % directional graph, so do k and j
            riddhi = getpathdistance(allpoints(kt), allpoints(jt), veh(i).pointarray)/veh(i).speed;
            if (riddhi < G(kt,jt))
                G(kt,jt) = riddhi;
                Gv(kt,jt) = i;
            end
            fprintf('p%d to p%d, veh %d time is %f\n', kt, jt, i, riddhi);
        end
    end
end

G
Gv


%% now for the optimization

max_waypoints = 2;
possib_waypoints = length(intersecns);
selector_thresh = 0.999; % dont reduce this to 0.5 for patternsearch


x0 = zeros(1, possib_waypoints);
Aineq = ones(1, possib_waypoints);
bineq = selector_thresh*(max_waypoints + 1) - 0.1;
lb = zeros(1, possib_waypoints);
ub = ones(1, possib_waypoints);



% Start with the default options
options = psoptimset;
% Modify options setting
options = psoptimset(options,'Display', 'off');
options = psoptimset(options,'SearchMethod', @GPSPositiveBasis2N);
options = psoptimset(options,'CompleteSearch', 'on');

[x,fval,exitflag,output] = ...
patternsearch(@(x)getinvaggthruput(x,G,selector_thresh),x0,Aineq,bineq,[],[],lb,ub,[],options);



% figure
% [x, y] = meshgrid(0:0.01:1);
% z = arrayfun(@(x,y)getaggthruput([x y],G),x,y);
% surf(x,y,z)

fprintf('selection vector for waypoints is [%s]\n', num2str(x));
fprintf('agg thruput is %f B per hour, %f B per sec\n', 1/fval, 1/(3600*fval));

x = [1 1 0 0];

selector = [1 2 3];
for i = 1:length(x)
    if (x(i) > selector_thresh)
        selector = [selector i+3];
    end
end

[dist,path] = graphshortestpath(sparse(G(selector,selector)),1,3);
fprintf('path route is [%s]\n', num2str(path));
[dist,path] = graphshortestpath(sparse(G(selector,selector)),2,3);
fprintf('path route is [%s]\n', num2str(path));
[dist,path] = graphshortestpath(sparse(G(selector,selector)),1,2);
fprintf('path route is [%s]\n', num2str(path));


figure
gplot(G(selector,selector), [[allpoints.x]' [allpoints.y]'], 'g-')

for i=1:length(allpoints)
    text(allpoints(i).x, allpoints(i).y, int2str(i), 'FontSize',16);
end

print -dpng chosen.png

disp('--------------------------------------')

