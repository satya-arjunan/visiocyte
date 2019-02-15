function a = load_visiocyte_swc_file(filename, b_minusFirst)
%function a = load_visiocyte_swc_file(filename, b_minusFirst)
% Load the swc file as a neuron structure
% by Hanchuan Peng
%
% an overaloading function for convenience
%
% 2009-07-24

if nargin<2,
    b_minusFirst=0;
end;

a = load_visiocyte_neuron_file(filename, b_minusFirst);

return;

