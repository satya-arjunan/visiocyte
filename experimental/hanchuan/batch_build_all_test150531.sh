cd ~/work/visiocyte_external/
rm -fr bin/*
svn up
build.macx -m -j4 -B

cd ../visiocyte_tools/hackathon/zhi/neuron_comparison_script/
sh batch_build_tracing_methods.sh

cd ~/work/visiocyte_tools/bigneuron_ported
sh build_bigneuron_plugins_v2.940.sh

cd ~/work/visiocyte_external
visiocyte -h

