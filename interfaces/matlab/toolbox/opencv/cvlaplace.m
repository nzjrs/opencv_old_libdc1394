function varargout = cvlaplace(varargin)
%CVLAPLACE           Calculate Laplacian of the image
%   IMAGE dst = cvLaplace(IMAGE src, apertureSize);
%   src          - source image
%   apertureSize - default 3. size of applied aperture
%
%   dst - destination image

if nargin < 1 | nargin > 2
    error 'Invalid number of parameters';
    return;
end

if nargin < 2
    varargin{2} = 3; % default aperture size
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1:out}] = feval('cvwrap', 'Laplace', varargin{:});

return;