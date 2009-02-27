function varargout = cvupdatemotionhistory(varargin)
%CVUPDATEMOTIONHISTORY Update motion history image
%   IMAGE mhi = cvUpdateMotionHistory(IMAGE silhouette, 
%     timestamp, mhiDuration);
%   silhouette  - silhouette image
%   timestamp   - current system time
%   mhiDuration - maximal duration of motion track before it will be removed
%
%   mhi         - motion history image

if nargin ~= 3
    error 'Invalid number of parameters';
    return;
end

out = nargout;
if out < 1
    out = 1;
end

if out > 1
    error 'Too many output parameters'
    return;
end;

[varargout{1}] = feval('cvwrap', 'UpdateMotionHistory', varargin{:});

return;