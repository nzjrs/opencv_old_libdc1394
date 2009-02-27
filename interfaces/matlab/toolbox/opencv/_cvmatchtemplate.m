function varargout = cvmatchtemplate(varargin)
%CVMATCHTEMPLATE     Measures similarity
%   IMAGE result = cvMatchTemplate(IMAGE array, IMAGE templ, method);
%   array  - source image
%   templ  - template to find
%   method - 'sqdiff' | 'sqdiff_normed' | 'ccorr' | 'ccorr_normed' | 
%            'ccoeff' | 'ccoeff_normed'
%
%   result - resultant image

if nargin ~= 3
    error 'Invalid number of parameters';
    return;
end

if ~ischar(varargin{3})
    error 'Invalid parameter ''method''';
    return;
end;

method_t = {'sqdiff', 'sqdiff_normed', 'ccorr', 'ccorr_normed', ... 
            'ccoeff', 'ccoeff_normed'};
method_i = [0, 1, 2, 3, 4, 5];
idx = strmatch(varargin{3}, method_t, 'exact');
if isempty(idx)
    error 'Invalid parameter ''method''';
    return;
end
varargin{3} = method_i(idx(1));

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1}] = feval('cvwrap', 'MatchTemplate', varargin{:});

return;