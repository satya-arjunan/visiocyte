function m_struct = load_visiocyte_apo_file(filename)
%function m_struct = load_visiocyte_apo_file(filename)
%
% Load the .apo point cloud format data file used in VISIOCYTE
% 
% m_struct will consist of point cloud coordinates and other information
% (e.g. name/comments/types/shape)
%
% VISIOCYTE website: see software page of http://penglab.janelia.org
%
% by Hanchuan Peng
% 20090724


m_struct = load_visiocyte_pointcloud_file(filename);
