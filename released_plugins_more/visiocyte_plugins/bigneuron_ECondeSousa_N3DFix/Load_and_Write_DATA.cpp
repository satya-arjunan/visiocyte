/*
* N3DFix - automatic removal of swelling artifacts in neuronal 2D/3D reconstructions
* last update: Mar 2016
* VERSION 2.0
*
* Authors: Eduardo Conde-Sousa <econdesousa@gmail.com>
*          Paulo de Castro Aguiar <pauloaguiar@ineb.up.pt>
* Date:    Mar 2016
*
* N3DFix v2.0 is described in the following publication (!UNDER REVIEW!)
* Conde-Sousa E, Szucs P, Peng H, Aguiar P - Neuroinformatics, 2016
*
*    Disclaimer
*    ----------
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You can view a copy of the GNU General Public License at
*    <http://www.gnu.org/licenses/>.
*/



#include <stdio.h>
#include <iostream>
#include <vector>
#include "myHeader.h"
#include "N3DFix_plugin.h"
#include "visiocyte_message.h"
#include "Dend_Section.h"
#include <cerrno>

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
void load_data(std::vector<float>& x, std::vector<float>& y, std::vector<float>& z,
               std::vector<long>& tree_id, std::vector<float>& r, std::vector<long>& ppid,
               std::vector<long> &pid, QString &path){
//    printf("\n\n###################################################################\n");
//    printf("######################## loading data #############################\n");
//    printf("###################################################################\n\n\n\n");


	// declare variables and vectors
	FILE * fin;
	float tmp_x, tmp_y, tmp_z, tmp_r;
    long tmp_id, tmp_tid, tmp_ppid;
    char mystring [100000];

    //open file
    fin = fopen(path.toStdString().c_str(), "r");//fopen_s(&fin,path.toStdString().c_str(), "r");
    // fopen_s(&fin,"C:\\Users\\Eduardo\\Dropbox\\PauloAguiar\\ArtifactRemoval\\reconstructions\\L395_no_axon.swc", "r");
	if (fin == NULL) {
        visiocyte_msg("Impossible to read data\n",1);
		//return -1;
	}else{
//		printf("\n########################################################################## \n");
//		printf("############################### Commnents: ############################### \n");
//		printf("########################################################################## \n");
        while ( fgets (mystring , 100000, fin) != NULL )
		{
			//pch=strchr(mystring,'#');
			//printf("pch = %s\n",pch);
			//if (pch != NULL){
			if (mystring[0] == '#' || mystring[1] == '#' || mystring[0] == '\n'|| mystring[1] == '\n'){
				//printf("%s",pch);
				//printf("pch = %s\t\t",pch);printf ("found at %d\n\n\n\n",pch-mystring+1);
				//pch=strchr(pch+1,'#');		
//				printf("%s",mystring);
			}else{
                sscanf(mystring, "%ld %ld %f %f %f %f %ld\n", &tmp_id, &tmp_tid, &tmp_x, &tmp_y, &tmp_z, &tmp_r, &tmp_ppid );
                //sscanf_s(mystring, "%d %d %f %f %f %f %d\n", &tmp_id, &tmp_tid, &tmp_x, &tmp_y, &tmp_z, &tmp_r, &tmp_ppid );
                pid.push_back(tmp_id);
                tree_id.push_back( tmp_tid);
				x.push_back( tmp_x );
				y.push_back( tmp_y );
				z.push_back( tmp_z );
                r.push_back( 2 * tmp_r );
				ppid.push_back( tmp_ppid );
			}
		}
		
	}
	fclose( fin );	
}
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_data(std::vector<float>& x, std::vector<float>& y, std::vector<float>& z,std::vector<long>& tree_id,
                std::vector<float>& r, std::vector<long>& ppid,std::vector<long> &pid){

	printf("########################################################################## \n");
	printf("############################## Data Matrix: ############################## \n");
	printf("########################################################################## \n");
	printf("tree_id\tx\t\ty\t\tz\t\tr\t\tppid\n");
	for (unsigned it=0; it<x.size(); it++){
        printf("%d %d\t%f\t%f\t%f\t%f\t%d\n",pid.at(it), tree_id.at(it), x.at(it), y.at(it), z.at(it), r.at(it), ppid.at(it));
	}

	printf("\n\n\n########################################################################## \n");
	printf("############################## # of Points: ############################## \n");
	printf("########################################################################## \n");
	printf("\t\t\tTotal Number of Points = %d\n",x.size());
	printf("########################################################################## \n");
}
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
void write_data(std::vector< std::vector<struct RawPoints > > &dend_original,std::vector< std::vector<struct RawPoints > > &dend,
                QString &path, QString &fileSaveName,std::vector<float>& x, std::vector<float>& y, std::vector<float>& z,
                std::vector<long>& tree_id, std::vector<float>& r, std::vector<long>& ppid, std::vector<long> &pid){

//    printf("\n\n###################################################################\n");
//    printf("######################## writing data #############################\n");
//    printf("###################################################################\n\n\n\n");

    // declare variables and vectors
    FILE * fout;
    //errno_t err;
    //open file

    //QString name = "_N3DFix";
    QString dataFile = fileSaveName;//QString dataFile = path.remove(path.size()-4,4)  + name + QLatin1String(".swc");
    fout = fopen(dataFile.toStdString().c_str(), "w");//err = fopen_s(&fout,dataFile.toStdString().c_str(), "w");

    std::vector<long> pid_tmp;
    long point = 0;
    if (fout == NULL) {//if (err != 0) {
        visiocyte_msg("The file was not opened\n",1);
        //return -1;
    }else{
        fprintf (fout, "# This file was generated using N3Dfix plugin\n");
        fprintf (fout, "#\n");
        fprintf (fout, "# Eduardo Conde-Sousa <econdesousa@gmail.com>\n");
        fprintf (fout, "# Paulo Aguiar        <pauloaguiar@ineb.up.pt>\n");
        fprintf (fout, "#\n");
        fprintf (fout, "# N3DFix v2.0 is described in the following publication (!UNDER REVIEW!)\n");
        fprintf (fout, "# Conde-Sousa E, Szucs P, Peng H, Aguiar P - Neuroinformatics, 2016\n");
        fprintf (fout, "#\n");
        // go through all compartments in neuron and then to all points of the compartment

        //first compartment is different
        for (unsigned ii =0; ii<1; ii++){
            for (unsigned i=0;i<dend[ii].size();i++){
                point = point +1;
                pid_tmp.push_back(dend[ii][i].pid);
                fprintf(fout,"%d %d %g %g %g %g %d\n",dend[ii][i].pid,dend[ii][i].tid,dend[ii][i].x,dend[ii][i].y,
                        dend[ii][i].z,dend[ii][i].r/2,dend[ii][i].ppid);
            }
            printf("\n\n");
        }
        //in all other compartments the first point is a duplicate, so as to be removed
        for (unsigned ii =1; ii<dend.size(); ii++){
            for (unsigned i=1;i<dend[ii].size();i++){
                point = point +1;
                pid_tmp.push_back(dend[ii][i].pid);
                fprintf(fout,"%d %d %g %g %g %g %d\n",dend[ii][i].pid,dend[ii][i].tid,dend[ii][i].x,dend[ii][i].y,
                        dend[ii][i].z,dend[ii][i].r/2,dend[ii][i].ppid);
            }
        }
    }
//    std::cout<<"original size = "<<x.size()<<std::endl;
//    std::cout<<" new size     = "<<point<<std::endl;
    int print_flag;
    if(point != pid.size()){
        for(long ii = 0;ii<pid.size();ii++){
            print_flag =1;
            for(long jj =0;jj<pid_tmp.size();jj++){
                if( pid.at(ii) == pid_tmp.at(jj)){
                    print_flag =0;
                    break;
                }
            }
            if(print_flag == 1){
                fprintf(fout,"%d %d %g %g %g %g %d\n",pid.at(ii),tree_id.at(ii),x.at(ii),y.at(ii),
                        z.at(ii),r.at(ii)/2,ppid.at(ii));
            }
        }
    }
    fclose( fout );
}
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
