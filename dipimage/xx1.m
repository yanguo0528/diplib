%XX1   Creates an image with the x-axis cartesian coordinate
%   XX1(SIZE) returns an image of size SIZE(1) along the x-axis,
%   and size 1 along other dimensions, with the x-coordinate of each
%   pixel as the pixel values.
%
%   Singleton expansion of XX1(SIZE,DIM) to SIZE yields the same
%   result as XX(SIZE,DIM). Because of implicit singleton expansion
%   in arithmetic, it is often faster and more memory-efficient to
%   use XX1 over XX.
%
%   XX1(IMG) is the same as XX1(SIZE(IMG)).
%
%   XX1(...,ORIGIN,OPTIONS) allows specifying the origin and additional
%   options, see COORDINATES for details.
%
%   SEE ALSO: coordinates, ramp1, xx, yy1, zz1, rr, phiphi, thetatheta

% (c)2017, Cris Luengo.
% Based on original DIPlib code: (c)1995-2014, Delft University of Technology.
% Based on original DIPimage code: (c)1999-2014, Delft University of Technology.
%
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
%
%    http://www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.

function out = xx1(sz,varargin)
if nargin<1
   sz = [256,256];
elseif ischar(sz)
   varargin = [{sz},varargin];
   sz = [256,256];
end
out = ramp1(sz,1,varargin{:});
