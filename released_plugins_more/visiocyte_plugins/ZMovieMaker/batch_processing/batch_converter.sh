#!/bin/bash
cd /Users/zhiz/data/20151030_rhea_reconstructions_for_allen300_silver_set/movie
for folder in `ls -d *`
do
	/Users/zhiz/work/visiocyte/visiocyte_external/bin/visiocyte64.app/Contents/MacOS/visiocyte64 -x Movie_Converter -f convert_frames_to_movie -i $folder 
        mv ${folder}/movie.avi /Users/zhiz/data/20151030_rhea_reconstructions_for_allen300_silver_set/movie_final/${folder}.avi
done
