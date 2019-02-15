function img = load_visiocyte_raw_img_file(filename)
%function img = load_visiocyte_raw_img_file(filename)
%
% Load the .RAW or .TIF or (even .LSM) image stack files that are supported by VISIOCYTE
% 
%
% VISIOCYTE website: see software page of http://penglab.janelia.org
%
% by Hanchuan Peng
% 20090724

img = loadRaw2Stack(filename);
