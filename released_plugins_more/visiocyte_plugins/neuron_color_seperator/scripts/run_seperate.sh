#Hanbo Chen
#2015.7
#script to seperate neuron from brainbow data
#list of input images
listfile=list_rawimage.txt
#extract neuron
while read fname
do
	fname_raw=${fname##*/}
	echo ${fname_raw}
	visiocyte -x neuron_color_seperator -f auto_seperate_neurons -i ${fname} -o ${fname_raw}.extract -p 2 11 10 128 10 1500 15 0.7 0.2 &> ${fname_raw}.extract.log
done < ${listfile}
#generate maximum projection of extraction for later proofreading
while read sub #for fname in *.visiocytepbd
do
	subname=${sub##*/}
	echo ${subname}
	ls ${subname}*.visiocytepbd | while read fname
	do
		visiocyte -x mipZ -f mip_zslices -i ${fname} -p 1:1:e -o ../proofread_3/mip/${fname}.mip.tiff
	done
done < ${listfile}
