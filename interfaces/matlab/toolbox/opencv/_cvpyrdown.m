function varargout = cvpyrdown(varargin)
%CVPYRDOWN           Perform factor-2 downsampling of the image
%   IMAGE dst = cvPyrDown(IMAGE src);
%   src - source image
%
%   dst - destination image

if nargin ~= 1
    error 'Invalid number of parameters';
    return;
end

varargin{2} = 7; % IPL_GAUSSIAN_5x5

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1:out}] = feval('cvwrap', 'PyrDown', varargin{:});

return;