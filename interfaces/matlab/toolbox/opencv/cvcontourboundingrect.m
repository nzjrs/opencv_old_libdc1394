function varargout = cvcontourboundingrect(varargin)
%CVCONTOURBOUNDINGRECT Calculate contour bounding rectangle.
%   rect = cvContourBoundingRect(contour);
%   contour - m by 2 matrix of contour's points
%
%   rect = [x y width height]

if nargin ~= 1
    error 'Invalid number of parameters';
    return;
end

contSize = size(varargin{1});
if ~isnumeric(varargin{1}) | ndims(varargin{1}) ~= 2 | ...
    contSize(1) < 1 | contSize(2) ~= 2
    error 'Invalid parameter ''contour''';
    return;
end;

m = min(varargin{1});
M = max(varargin{1});
varargout{1} = [m M-m+1];

return;