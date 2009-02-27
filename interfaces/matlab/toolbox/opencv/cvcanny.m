function varargout = cvcanny(varargin)
%CVCANNY             Canny edge detection
%   IMAGE dst = cvCanny(IMAGE src, lowThreshold, highThreshold, apertureSize);
%   src           - source image
%   lowThreshold,
%   highThreshold - tresholds, applied in hysteresis thresholding
%   apertureSize  - default 3. Size of Sobel operator aperture
%
%   dst - destination image
%

if nargin < 3 | nargin > 4
    error 'Invalid number of parameters';
    return;
end

if nargin < 4
    varargin{4} = 3; % default apertureSize
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1:out}] = feval('cvwrap', 'Canny', varargin{:});

return;