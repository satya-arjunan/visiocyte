Last update: 2009-07-28. by Hanchuan Peng

This directory contains a Matlab toolbox to read and write files with 
some formats defined/supported by VISIOCYTE software (penglab.janelia.org).

Since some of the functions are mex functions, you may need to run the 
following command to generate the mex function for your machine (assuming
you have mex compiling setup in your Matlab):

>> makeosmex_rawfile_io

The following is a simple explanation of the interface of functions, both
mex functions and plain Matlab .m functions. 

load_visiocyte_raw_img_file:    load the .raw, .tiff image stacks used in VISIOCYTE (depending on
						  file extension, the program automatically determine the file type)
load_visiocyte_marker_file:     load the .marker file 
load_visiocyte_apo_file:        load the .apo files (point cloud files)
load_visiocyte_pointcloud_file: load the point cloud files used in VISIOCYTE (an overloading
						  function provided for convenience)
load_visiocyte_swc_file:        load the .swc file (for neurons or other graphs)
load_visiocyte_neuron_file:     load the neuron files in the .swc format (an overloading function
                          provided for convenience)

save_visiocyte_raw_img_file:    save the .raw, .tiff image stacks used in VISIOCYTE (depending on
						  file extension, the program automatically determine the file type)
save_visiocyte_marker_file:     save the .marker file
save_visiocyte_apo_file:        save the .apo files (point cloud files)
save_visiocyte_pointcloud_file: save the point cloud files used in VISIOCYTE (an overloading
						  function provided for convenience)
save_visiocyte_swc_file:        save the .swc file (for neurons or other graphs)

