%SEPARABLESUM Combines separable functions into their sum
%
%   SEPARABLESUM(fs, idx, sizes) where fs is a cell array of function
%   objects, while idx and sizes are integer vectors of the same length.
%   If length(idx) = length(sizes) = k, then SEPARABLESUM returns the
%   function object correspondent to the sum
%       
%       f(x) = sum_i=1...k fs{idx(i)}(x_i)
%
%   i.e., the sum of k functions, the ith being idx(i) and applied to a
%   block of size(i) variables.
%

function obj = separableSum(objs, idx, sizes)
    obj.makeprox = @() make_separableSum_prox(objs, idx, sizes);
end

function op = make_separableSum_prox(objs, idx, sizes)
    proxes = {};
    for i=1:length(objs)
        proxes{end+1} = objs{i}.makeprox();
    end
    op = @(x, gam) call_separableSum_prox(x, gam, proxes, idx, sizes);
end

function [prox, val] = call_separableSum_prox(x, gam, proxes, idx, sizes)
    n = sum(sizes);
    prox = zeros(n, 1);
    val = 0;
    baseidx = 0;
    for i=1:length(idx)
        endcurr = baseidx+sizes(i);
        xcurr = x(baseidx+1:endcurr);
        [prox(baseidx+1:endcurr), val1] = proxes{idx(i)}(xcurr, gam);
        val = val+val1;
        baseidx = endcurr;
    end
end