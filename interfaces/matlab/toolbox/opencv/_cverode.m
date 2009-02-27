function varargout = cverode(varargin)
%CVERODE             Apply minimum filter to the image
%   IMAGE dst = cvErode(IMAGE src, elementValues, elementSize, iterations);
%   src           - source image
%   elementValues - structuring element matrix
%   elementSize   - [anchorRow, anchorCol]
%   iterations    - default 1.
%
%   dst - destination image
%
%   IMAGE dst = cvErode(IMAGE src, elementType, elementSize, iterations);
%   src         - source image
%   elementType - {'rect'} | 'cross' | 'ellipse'
%   elementSize - [nRows, nCols, anchorRow, anchorCol]. Default [3, 3, 2, 2].
%   iterations  - default 1.
%
%   dst - destination image

if nargin < 1 | nargin > 4
    error 'Invalid number of parameters';
    return;
end

if nargin > 1
    if ischar(varargin{2})
		elem_t = {'rect', 'cross', 'ellipse'};
		elem_i = [0, 1, 2];
        idx = strmatch(varargin{2}, elem_t, 'exact');
        if isempty(idx)
            error 'Invalid parameter ''elementType''';
            return;
		end
        varargin{2} = elem_i(idx(1));
    end
else
    varargin{2} = [];
end

if nargin < 3
    varargin{3} = [3, 3, 2, 2]; % default elementSize
end

if nargin < 4
    varargin{4} = 1; % default iterations
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1:out}] = feval('cvwrap', 'Erode', varargin{:});

return;