svn co https://subversion.int.janelia.org/penglab/projects/VISIOCYTE/neurolabi
cd neurolabi
rm -rf doc
cd ..
tar -zcf neurolabi.tar neurolabi
rm -rf neurolabi