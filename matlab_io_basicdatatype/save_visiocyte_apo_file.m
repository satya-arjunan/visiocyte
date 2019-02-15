function save_visiocyte_apo_file(m_struct, filename)
%function save_visiocyte_apo_file(m_struct, filename)
%
% Save the .apo point cloud format data file used in VISIOCYTE
% 
% m_struct will consist of point cloud coordinates and other information
% (e.g. name/comments/types/shape)
%
% VISIOCYTE website: see software page of http://penglab.janelia.org
%
% by Hanchuan Peng
% 20090724


save_visiocyte_pointcloud_file(m_struct, filename);
