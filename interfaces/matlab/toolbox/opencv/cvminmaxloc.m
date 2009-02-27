function varargout = cvminmaxloc(varargin)
%CVMINMAXLOC         minimum and maximum pixel values in the image region
%   [minVal, maxVal, minLoc, maxLoc] = cvMinMaxLoc(IMAGE src);
%   [minVal, maxVal, minLoc, maxLoc] = cvMinMaxLoc(IMAGE src, IMAGE mask);
%   src  - source image
%   mask - mask image
%
%   minVal - minimum pixel value
%   maxVal - maximum pixel value
%   minLoc - minimum pixel location
%   maxLoc - maximum pixel location

if nargin < 1 | nargin > 2
    error 'Invalid number of parameters';
    return;
end

if nargin < 2
    varargin{2} = []; % default mask
end

out = nargout;
if out < 1
    out = 1;
end

if out > 4
    error 'Too many output parameters'
    return;
end;
for idx = out+1:4
    varargout{idx} = [];
end

[varargout{1:4}] = feval('cvwrap', 'MinMaxLoc', varargin{:});

return;