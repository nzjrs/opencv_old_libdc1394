function varargout = cvsobel(varargin)
%CVSOBEL             Calculate an image derivative
%   IMAGE dst = cvSobel(IMAGE src, xorder, yorder, apertureSize);
%   src          - source image
%   xorder       - order of x derivative
%   yorder       - order of y derivative
%   apertureSize - default 3. size of applied aperture or 'schar'
%                  for Schar kernel
%
%   dst - destination image

if nargin < 3 | nargin > 4
    error 'Invalid number of parameters';
    return;
end

if nargin > 3
    if ischar(varargin{4}) & strcmp(varargin{4}, 'schar')
        varargin{4} = -1; % schar
    end
else
    varargin{4} = 3; % default aperture size
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1:out}] = feval('cvwrap', 'Sobel', varargin{:});

return;