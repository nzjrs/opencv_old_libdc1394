function varargout = cvadaptivethreshold(varargin)
%CVADAPTIVETHRESHOLD Apply adaptive threshold
%   IMAGE dst = cvAdaptiveThreshold(IMAGE src,
%     DOUBLE maxVal, type, neighborhoodSize, minVariance);
%   src              - source image
%   maxValue         - the maximum value of the image pixel
%   type:
%     'binary'       - val = (val>thresh? maxValue:0)
%     'binary_inv'   - val = (val>thresh? 0:maxValue)
%     'trunc'        - val = (val>thresh? thresh:val)
%     'tozero'       - val = (val>thresh? val:0)
%     'tozero_inv'   - val = (val>thresh? 0:val)
%   neighborhoodSize - 3, 5 or 7
%   minVariance      - value of the minimum variance
%
%   dst - destination image

if nargin ~= 5
    error 'Invalid number of parameters';
    return;
end

varargin{5} = [(varargin{4} - 1) / 2, varargin{5}]; %parameters

if ~ischar(varargin{3})
    error 'Invalid parameter ''type''';
    return;
end;

type_t = {'binary', 'binary_inv', 'trunc', 'tozero', 'tozero_inv'};
type_i = [0, 1, 2, 3, 4];
idx = strmatch(varargin{3}, type_t, 'exact');
if isempty(idx)
    error 'Invalid parameter ''type''';
    return;
end
varargin{4} = type_i(idx(1)); % type
varargin{3} = 0; % method. CV_STDDEV_ADAPTIVE_THRESH

if nargout > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1}] = feval('cvwrap', 'AdaptiveThreshold', varargin{:});

return;