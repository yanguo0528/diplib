%XX   Creates an image with the x-axis cartesian coordinate
%   XX(SIZE) returns an image of size SIZE with the value of the x-axis
%   coordinate as the pixel values.
%
%   XX(IMG) is the same as XX(SIZE(IMG)).
%
%   XX(...,ORIGIN,OPTIONS) allows specifying the origin and additional
%   options, see COORDINATES for details.
%
%   SEE ALSO: coordinates, ramp, xx1, yy, zz, rr, phiphi, thetatheta

% (c)2017-2020, Cris Luengo.
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

function out = xx(varargin)
out = internal_ramp(1,'full',varargin{:});
