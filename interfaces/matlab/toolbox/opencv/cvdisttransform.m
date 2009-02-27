function varargout = cvdisttransform(varargin)
%CVDISTTRANSFORM     Calculate distance transform of binary image
%   IMAGE dst = cvDistTransform(IMAGE src, dist, maskSize);
%   src      - source image
%   dist     - 'l1' | {'l2'} | 'c' | 'l12' | 'fair' | 'welsch'
%   maskSize - {3} | 5
%
%   dst - destination image
%
%   IMAGE dst = cvDistTransform(IMAGE src, dist);
%   src      - source image
%   dist     - array of 2 (for 3x3 mask) or 3 numbers (for 5x5 mask)
%              that characterizes metric
%
%   dst - destination image

if nargin < 1 | nargin > 3
    error 'Invalid number of parameters';
    return;
end

if nargin > 1
    if ischar(varargin{2})
		dist_t = {'l1', 'l2', 'c', 'l12', 'fair', 'welsch'};
		dist_i = [1, 2, 3, 4, 5, 6];
        idx = strmatch(varargin{2}, dist_t, 'exact');
        if isempty(idx)
            error 'Invalid parameter ''dist''';
            return;
		end
        varargin{2} = dist_i(idx(1));
    end
else
    varargin{2} = 2; % default 'l2'
end

if nargin < 3
    varargin{3} = 3; % default maskSize
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1:out}] = feval('cvwrap', 'DistTransform', varargin{:});

return;