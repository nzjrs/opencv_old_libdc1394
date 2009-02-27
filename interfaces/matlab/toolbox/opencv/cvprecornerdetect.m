function varargout = cvprecornerdetect(varargin)
%CVPRECORNERDETECT   Calculate constraint image for corner detection
%   IMAGE dst = cvPreCornerDetect(IMAGE src, apertureSize);
%   src           - source image
%   apertureSize  - default 3. Size of Sobel operator aperture
%
%   dst - destination image
%

if nargin < 1 | nargin > 2
    error 'Invalid number of parameters';
    return;
end

if nargin < 2
    varargin{2} = 3; % default apertureSize
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1}] = feval('cvwrap', 'PreCornerDetect', varargin{:});

return;