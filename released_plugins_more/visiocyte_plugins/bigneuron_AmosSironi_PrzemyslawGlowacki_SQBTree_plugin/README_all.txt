# In this folder you find a visiocyte plugin implementing the method described in [1] (./SQBTree_plugin)
# and a plugin using the method [1] in combination with a Minimum Spanning Tree (MST) algorithm [2] to reconstruct a neuron in an 3D image (./RegMST_tracing_plugin)
#
# The plugin under ./RegMST_tracing_plugin will be used in the BigNeuron bench-testing [3]
 

#  ***1. compile dependent libraries ( static libs included for Linux 64 bits) ***
# cd ./libs/libconfig-1.4.9
#  ./configure && make
#
# cd ./libs/liblbfgs-1.10
# ./configure --disable-shared --enable-static --enable-sse2 && make 



 ***2. compile the two plugins ***
cd ./SQBTree_plugin
qmake && make
cd ../RegMST_tracing_plugin
qmake && make
cd ..

 *** 3. run the tracing plugin on a small image ***
cd ../../../visiocyte_external/bin

./visiocyte -x RegMST -f tracing_func -i ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/cropped_OP_9.tif -p ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/filter_banks/oof_fb_3d_scale_1_2_3_5_size_13_sep_cpd_rank_49.txt ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/filter_banks/oof_fb_3d_scale_1_2_3_5_size_13_weigths_cpd_rank_49.txt ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/filter_banks/proto_filter_AC_lap_633_822_sep_cpd_rank_49.txt ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/filter_banks/proto_filter_AC_lap_633_822_weigths_cpd_rank_49.txt 1 2 ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/SQBTree_plugin/trained_models/model_S/Regressor_ac_0.cfg ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/SQBTree_plugin/trained_models/model_S/OPF_debug/Regressor_ac_1.cfg 21 200

 *** 4. visualize results ***
load in visiocyte the original image : ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/cropped_OP_9.tif
          and  the reconstruction swc file : ../../visiocyte_tools/bigneuron_ported/AmosSironi_PrzemyslawGlowacki/data/cropped_OP_9.tif_tubularity_model_S.visiocyteraw_MST_Tracing_Ws_21_th_200.swc


[1] A. Sironi, E. Türetken, V. Lepetit and P. Fua. Multiscale Centerline Detection, submitted to IEEE Transactions on Pattern Analysis and Machine Intelligence.
[2] H. Peng, Z. Ruan, D. Atasoy and S. Sternson Automatic reconstruction of 3D neuron structures using a graph-augmented deformable model. Bioinformatics.
[3] http://alleninstitute.org/bigneuron/about/