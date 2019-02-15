/* Authors: Ching-Wei Wang (cweiwang@mail.ntust.edu.tw), M. Hilmil Muchtar Aditya Pradana, Cheng-Ta Huang
 * Institute: Medical Image Research Center, National Taiwan University of Science & Technology
 * Web: http://www-o.ntust.edu.tw/~cweiwang/
 * License: This plugin uses the same license as Visiocyte.
 * This method is called "Ensemble neuron tracer", which combines multiple segmentation results by three simple methods adapted from APP2[citation 1].
 * Citation 1:  Hang Xiao, Hanchuan Peng, APP2: automatic tracing of 3D neuron morphology based on hierarchical pruning of a gray-weighted image distance-tree. Bioinformatics 29(11): 1448-1454 (2013)
 */
 
#include "visiocyte_message.h"
#include <vector>
#include <algorithm>
#include <string>
#include "basic_surf_objs.h"

#include "CWlab_method1_version1_plugin.h"

#include "vn_imgpreprocess.h"
#include "fastmarching_dt.h"
#include "fastmarching_tree.h"
#include "hierarchy_prune.h"
#include "marker_radius.h"


Q_EXPORT_PLUGIN2(APP2_ported, APP2_ported);

using namespace std;

struct input_PARA
{
    QString inimg_file;
    VISIOCYTELONG channel;
};

void reconstruction_func(VISIOCYTEPluginCallback2 &callback, QWidget *parent, input_PARA &PARA, bool bmenu);
 
QStringList APP2_ported::menulist() const
{
	return QStringList() 
		<<tr("tracing_menu")
		<<tr("about");
}

QStringList APP2_ported::funclist() const
{
	return QStringList()
		<<tr("tracing_func")
		<<tr("help");
}

void APP2_ported::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("tracing_menu"))
	{
        bool bmenu = true;
        input_PARA PARA;
        reconstruction_func(callback,parent,PARA,bmenu);

	}
	else
	{
		visiocyte_msg(tr("Authors: Ching-Wei Wang(cweiwang@mail.ntust.edu.tw), M. Hilmil Muchtar Aditya Pradana, Cheng-Ta Huang\nInstitute: Medical Image Research Center, National Taiwan University of Science & Technology\nWeb: http://www-o.ntust.edu.tw/~cweiwang/\nLicense: This plugin uses the same license as Visiocyte."));
	}
}

bool APP2_ported::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("tracing_func"))
	{
        bool bmenu = false;
        input_PARA PARA;

        vector<char*> * pinfiles = (input.size() >= 1) ? (vector<char*> *) input[0].p : 0;
        vector<char*> * pparas = (input.size() >= 2) ? (vector<char*> *) input[1].p : 0;
        vector<char*> infiles = (pinfiles != 0) ? * pinfiles : vector<char*>();
        vector<char*> paras = (pparas != 0) ? * pparas : vector<char*>();

        if(infiles.empty())
        {
            fprintf (stderr, "Need input image. \n");
            return false;
        }
        else
            PARA.inimg_file = infiles[0];
        int k=0;
        PARA.channel = (paras.size() >= k+1) ? atoi(paras[k]) : 1;  k++;
        reconstruction_func(callback,parent,PARA,bmenu);
	}
    else if (func_name == tr("help"))
    {

        ////HERE IS WHERE THE DEVELOPERS SHOULD UPDATE THE USAGE OF THE PLUGIN
		printf("**** Usage of CWlab_method1_version1 tracing **** \n");
		printf("visiocyte -x CWlab_method1_version1 -f tracing_func -i <inimg_file> -p <channel> <other parameters>\n");
        printf("inimg_file       The input image\n");
        printf("channel          Data channel for tracing. Start from 1 (default 1).\n");

        printf("outswc_file      Will be named automatically based on the input image file name, so you don't have to specify it.\n\n");

	}
	else return false;

	return true;
}

void reconstruction_func(VISIOCYTEPluginCallback2 &callback, QWidget *parent, input_PARA &PARA, bool bmenu)
{
    unsigned char* data1d = 0;
	Image4DSimple* p4DImage;
    VISIOCYTELONG N,M,P,sc,c;
    VISIOCYTELONG in_sz[4];
    if(bmenu)
    {
        visiocytehandle curwin = callback.currentImageWindow();
        if (!curwin)
        {
            QMessageBox::information(0, "", "You don't have any image open in the main window.");
            return;
        }

        p4DImage = callback.getImage(curwin);

        if (!p4DImage)
        {
            QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
            return;
        }

        if(p4DImage->getDatatype()!=VISIOCYTE_UINT8)
        {
            QMessageBox::information(0, "", "Please convert the image to be UINT8 and try again!");
            return;
        }
        data1d = p4DImage->getRawData();
        N = p4DImage->getXDim();
        M = p4DImage->getYDim();
        P = p4DImage->getZDim();
        sc = p4DImage->getCDim();
		unsigned char * inimg1d = p4DImage->getRawDataAtChannel(-1);
		
        bool ok1;

        if(sc==1)
        {
            c=1;
            ok1=true;
        }
        else
        {
            c = QInputDialog::getInteger(parent, "Channel", "Enter channel NO:", 1, 1, sc, 1, &ok1);
        }

        if(!ok1) return;

        in_sz[0] = N;
        in_sz[1] = M;
        in_sz[2] = P;
        in_sz[3] = sc;

        PARA.inimg_file = p4DImage->getFileName();
    }
    else
    {
        int datatype = 0;
        if (!simple_loadimage_wrapper(callback,PARA.inimg_file.toStdString().c_str(), data1d, in_sz, datatype))
        {
            fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",PARA.inimg_file.toStdString().c_str());
            return;
        }
        if(PARA.channel < 1 || PARA.channel > in_sz[3])
        {
            fprintf (stderr, "Invalid channel number. \n");
            return;
        }

        if(datatype !=1)
        {
            fprintf (stderr, "Please convert the image to be UINT8 and try again!\n");
            return;
        }
        N = in_sz[0];
        M = in_sz[1];
        P = in_sz[2];
        sc = in_sz[3];
        c = PARA.channel;
    }

    //main neuron reconstruction code

    //// THIS IS WHERE THE DEVELOPERS SHOULD ADD THEIR OWN NEURON TRACING CODE
	
	int se;
	
    VISIOCYTELONG pagesz = N*M*P;
    unsigned char *data1d_1ch;
    try {data1d_1ch = new unsigned char [pagesz];}
    catch(...)  {visiocyte_msg("cannot allocate memory for data1d_1ch."); return;}

    for(VISIOCYTELONG i = 0; i < pagesz; i++)
        data1d_1ch[i] = data1d[i+(c-1)*pagesz];

    Image4DSimple * p4dImageNew = 0;
    p4dImageNew = new Image4DSimple;

    if(!p4dImageNew->createImage(N,M,P,1, VISIOCYTE_UINT8))
        return;

    memcpy(p4dImageNew->getRawData(), data1d_1ch, pagesz);

    unsigned char * indata1d = p4dImageNew->getRawDataAtChannel(0);
	
//	fstream file;
//	file.open("file.swc", ios::out); 
	fstream filelog;
	filelog.open("filelog.swc", ios::out);  

/*	if (!file)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BW Function
	
	VISIOCYTELONG tb = in_sz[0]*in_sz[1]*in_sz[2]*p4DImage->getUnitBytes();
	
	unsigned char * nm = NULL;
	try {
		nm = new unsigned char [tb];
	}
	catch (...) {
		throw("Fail to allocate memory in Image Thresholding plugin.");
	}
	
	unsigned char * imdilate = NULL;
	try {
		imdilate = new unsigned char [tb];
	}
	catch (...) {
		throw("Fail to allocate memory in Image Thresholding plugin.");
	}
	
	vector<MyMarker*> outswc[3];
	float * phi = 0;
	vector<MyMarker *> outtree[3];
	VISIOCYTELONG max_loc = 0;
	double max_val = -1;
	
	for (VISIOCYTELONG i=0;i<tb;i++)
	{
		if (indata1d[i]>=1) nm[i] = 255;
		else nm[i] = 0;
	}
	
	in_sz[0] = N;
	in_sz[1] = M;
	in_sz[2] = P;
	in_sz[3] = sc;
	int j = 0;
	
	QString swc_name = PARA.inimg_file + "_Cwlab_ver1.swc";
	QByteArray temp = swc_name.toLatin1();
	char* path = temp.data();
	ofstream file(path);
	
	file << "#Supervisor: Prof Ching-Wei Wang (cweiwang@mail.ntust.edu.tw)\n#Authors: Ching-Wei Wang, M. Hilmil Muchtar Aditya Pradana (hilmil.041@gmail.com), Cheng-Ta Huang (chengta.huang@mail.ntust.edu.tw)\n#Institute: Medical Image Research Center, National Taiwan University of Science & Technology\n#Web: http://www-o.ntust.edu.tw/~cweiwang/\n#License: This plugin uses the same license as Visiocyte.\n";
	
	
	for (int iteration=0;iteration<3;iteration++)
	{
		in_sz[0] = N;
		in_sz[1] = M;
		in_sz[2] = P;
		in_sz[3] = sc;
		se = 1; // strell

		if (iteration!=0)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// Closing
			// Create 3D image
			// dilate
			for (VISIOCYTELONG i=0;i<tb;i++)
			{
				if (nm[i]==255)
				{
					imdilate[i] = 255;
					for (VISIOCYTELONG j=0;j<se;j++)
					{
						for (VISIOCYTELONG k=0;k<se;k++)
						{
							for (VISIOCYTELONG l=0;l<se;l++)
							{
								if ((((i+(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))-(1*(l+1)))>=0 && (((i+(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))+(1*(l+1)))>=0 && ((i+(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))-(1*(l+1)))>=0 && ((i+(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))+(1*(l+1)))>=0 && (((i-(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))-(1*(l+1)))>=0 && (((i-(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))+(1*(l+1)))>=0 && ((i-(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))-(1*(l+1)))>=0 && ((i-(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))+(1*(l+1)))>=0)
								{
									//back  //bottom mid //left
									imdilate[((i+(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))-(1*(l+1))] = 255;
									//back  //bottom mid //right
									imdilate[((i+(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))+(1*(l+1))] = 255;
									//back  //top mid //left
									imdilate[(i+(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))-(1*(l+1))] = 255;
									//back  //top mid //right
									imdilate[(i+(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))+(1*(l+1))] = 255;
									
									//front //bottom mid //left
									imdilate[((i-(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))-(1*(l+1))] = 255;
									//front //bottom mid //right
									imdilate[((i-(in_sz[0]*in_sz[1]*j))+(in_sz[0]*(k+1)))+(1*(l+1))] = 255;
									//from //top mid //left
									imdilate[(i-(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))-(1*(l+1))] = 255;
									//from //top mid //right
									imdilate[(i-(in_sz[0]*in_sz[1]*(j+1))-(in_sz[0]*(k+1)))+(1*(l+1))] = 255;
								}
							}
						}
					}
				}
				else {
					imdilate[i] = 0;
				}
			}
		}
		else if (iteration==2)
		{
			//erode
			for (VISIOCYTELONG i=0;i<tb;i++)
			{
				//front //right //top
		//		imdilate[i-(in_sz[0]*in_sz[1]*se)-se-(se*in_sz[0])]
				//front //right //bottom
		//		imdilate[i-(in_sz[0]*in_sz[1]*se)-se+(se*in_sz[0])]
				
				//front //left //top
		//		imdilate[i-(in_sz[0]*in_sz[1]*se)+se-(se*in_sz[0])]
				//front //left //bottom
		//		imdilate[i-(in_sz[0]*in_sz[1]*se)+se+(se*in_sz[0])]
				
				//back //right //top
		//		imdilate[i+(in_sz[0]*in_sz[1]*se)-se-(se*in_sz[0])]
				//back //right //bottom
		//		imdilate[i+(in_sz[0]*in_sz[1]*se)-se+(se*in_sz[0])]
				
				//back //left //top
		//		imdilate[i+(in_sz[0]*in_sz[1]*se)+se-(se*in_sz[0])]
				//back //left //bottom
		//		imdilate[i+(in_sz[0]*in_sz[1]*se)+se+(se*in_sz[0])]
		
				if (nm[i]!=imdilate[i])
				{
					if ((i-(in_sz[0]*in_sz[1]*(se-1))-(se-1)-((se-1)*in_sz[0]))>=0 && (i-(in_sz[0]*in_sz[1]*se)-se+(se*in_sz[0]))>=0 && (i-(in_sz[0]*in_sz[1]*se)+se-(se*in_sz[0]))>=0 && (i-(in_sz[0]*in_sz[1]*se)+se+(se*in_sz[0]))>=0 && (i+(in_sz[0]*in_sz[1]*se)-se-(se*in_sz[0]))>=0 && (i+(in_sz[0]*in_sz[1]*se)-se+(se*in_sz[0]))>=0 && (i+(in_sz[0]*in_sz[1]*se)+se-(se*in_sz[0]))>=0 && (i+(in_sz[0]*in_sz[1]*se)+se+(se*in_sz[0])))
					{
						if(imdilate[i-(in_sz[0]*in_sz[1]*(se-1))-(se-1)-((se-1)*in_sz[0])]!=255 || imdilate[i-(in_sz[0]*in_sz[1]*se)-se+(se*in_sz[0])]!=255 || imdilate[i-(in_sz[0]*in_sz[1]*se)+se-(se*in_sz[0])]!=255 || imdilate[i-(in_sz[0]*in_sz[1]*se)+se+(se*in_sz[0])]!=255 || imdilate[i+(in_sz[0]*in_sz[1]*se)-se-(se*in_sz[0])]!=255 || imdilate[i+(in_sz[0]*in_sz[1]*se)-se+(se*in_sz[0])]!=255 || imdilate[i+(in_sz[0]*in_sz[1]*se)+se-(se*in_sz[0])]!=255 || imdilate[i+(in_sz[0]*in_sz[1]*se)+se+(se*in_sz[0])]!=255)
						{
							nm[i] = 0;
						}
						else
						{
							nm[i] = 255;
						}
					}
				}
			}
		}
		if (iteration==0 || iteration==2)
		{
			indata1d = nm;
		}
		else if (iteration==1)
		{
			indata1d = imdilate;
		}
		
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		in_sz[3] = 1;
		double dfactor_xy = 1, dfactor_z = 1;

		if (in_sz[0]<=256 && in_sz[2]<=256 && in_sz[2]<=256)
		{
			dfactor_z = dfactor_xy = 1;
		}
		else if (in_sz[0] >= 2*in_sz[2] || in_sz[1] >= 2*in_sz[2])
		{
			if (in_sz[2]<=256)
			{
				double MM = in_sz[0];
				if (MM<in_sz[1]) MM=in_sz[1];
				dfactor_xy = MM / 256.0;
				dfactor_z = 1;
			}
			else
			{
				double MM = in_sz[0];
				if (MM<in_sz[1]) MM=in_sz[1];
				if (MM<in_sz[2]) MM=in_sz[2];
				dfactor_xy = dfactor_z = MM / 256.0;
			}
		}
		else
		{
			double MM = in_sz[0];
			if (MM<in_sz[1]) MM=in_sz[1];
			if (MM<in_sz[2]) MM=in_sz[2];
			dfactor_xy = dfactor_z = MM / 256.0;
		}
		
		printf("dfactor_xy=%5.3f\n", dfactor_xy);
		printf("dfactor_z=%5.3f\n", dfactor_z);

		if (dfactor_z>1 || dfactor_xy>1)
		{
			visiocyte_msg("enter ds code", 0);

			VISIOCYTELONG out_sz[4];
			
			unsigned char * outimg=0;
			if (!downsampling_img_xyz( indata1d, in_sz, dfactor_xy, dfactor_z, outimg, out_sz))
				return;
			
			Image4DSimple * p4dImageNew2 = 0;
			p4dImageNew2 = new Image4DSimple;
			
			
			p4dImageNew2->setData(outimg, out_sz[0], out_sz[1], out_sz[2], out_sz[3], VISIOCYTE_UINT8);

			indata1d = p4dImageNew2->getRawDataAtChannel(0);
			in_sz[0] = p4dImageNew2->getXDim();
			in_sz[1] = p4dImageNew2->getYDim();
			in_sz[2] = p4dImageNew2->getZDim();
			in_sz[3] = p4dImageNew2->getCDim();
		}

		cout<<"Start detecting cellbody"<<endl;
		float * phi = 0;
		vector<MyMarker> inmarkers;
		
		fastmarching_dt_XY(indata1d, phi, in_sz[0], in_sz[1], in_sz[2],2, 10);
		VISIOCYTELONG sz0 = in_sz[0];
		VISIOCYTELONG sz1 = in_sz[1];
		VISIOCYTELONG sz2 = in_sz[2];
		VISIOCYTELONG sz01 = sz0 * sz1;
		VISIOCYTELONG tol_sz = sz01 * sz2;
	
		for(VISIOCYTELONG i = 0; i < tol_sz; i++)
		{
			if(phi[i] > max_val)
			{
				max_val = phi[i];
				max_loc = i;
			}
		}

		MyMarker max_marker(max_loc % sz0, max_loc % sz01 / sz0, max_loc / sz01);
		inmarkers.push_back(max_marker);

		cout<<"======================================="<<endl;
		cout<<"Construct the neuron tree"<<endl;

		visiocyte_msg("8bit", 0);

		fastmarching_tree(inmarkers[0], indata1d, outtree[iteration], in_sz[0], in_sz[1], in_sz[2], 2, 10, false);
		cout<<"======================================="<<endl;

		//save a copy of the ini tree
		cout<<"Save the initial unprunned tree"<<endl;
		vector<MyMarker*> & inswc = outtree[iteration];
		
		if (1)
		{
			VISIOCYTELONG tmpi;

			vector<MyMarker*> tmpswc;
			for (tmpi=0; tmpi<inswc.size(); tmpi++)
			{
				MyMarker * curp = new MyMarker(*(inswc[tmpi]));
				tmpswc.push_back(curp);

				if (dfactor_xy>1) inswc[tmpi]->x *= dfactor_xy;
				inswc[tmpi]->x += (0);
				if (dfactor_xy>1) inswc[tmpi]->x += dfactor_xy/2;

				if (dfactor_xy>1) inswc[tmpi]->y *= dfactor_xy;
				inswc[tmpi]->y += (0);
				if (dfactor_xy>1) inswc[tmpi]->y += dfactor_xy/2;

				if (dfactor_z>1) inswc[tmpi]->z *= dfactor_z;
				inswc[tmpi]->z += (0);
				if (dfactor_z>1)  inswc[tmpi]->z += dfactor_z/2;
			}
			
/*			if (iteration==0)
			{
				saveSWC_file(QString(PARA.inimg_file).append("_ini1.swc").toStdString(), inswc);
			}
			else if (iteration==1)
			{
				saveSWC_file(QString(PARA.inimg_file).append("_ini2.swc").toStdString(), inswc);
			}
			else
			{
				saveSWC_file(QString(PARA.inimg_file).append("_ini3.swc").toStdString(), inswc);
			}*/
			
			for (tmpi=0; tmpi<inswc.size(); tmpi++)
			{
				inswc[tmpi]->x = tmpswc[tmpi]->x;
				inswc[tmpi]->y = tmpswc[tmpi]->y;
				inswc[tmpi]->z = tmpswc[tmpi]->z;
			}

			for(tmpi = 0; tmpi < tmpswc.size(); tmpi++)
				delete tmpswc[tmpi];
			tmpswc.clear();
		}

		cout<<"Pruning neuron tree"<<endl;

		visiocyte_msg("start to use happ.\n", 0);
		happ(inswc, outswc[iteration], indata1d, in_sz[0], in_sz[1], in_sz[2],10, 5, 0.3333);

		if (p4dImageNew) {delete p4dImageNew; p4dImageNew=0;} //free buffe

		inmarkers[0].x *= dfactor_xy;
		inmarkers[0].y *= dfactor_xy;
		inmarkers[0].z *= dfactor_z;

		for(VISIOCYTELONG i = 0; i < outswc[iteration].size(); i++)
		{
			if (dfactor_xy>1) outswc[iteration][i]->x *= dfactor_xy;
			outswc[iteration][i]->x += 0;
			if (dfactor_xy>1) outswc[iteration][i]->x += dfactor_xy/2;

			if (dfactor_xy>1) outswc[iteration][i]->y *= dfactor_xy;
			outswc[iteration][i]->y += 0;
			if (dfactor_xy>1) outswc[iteration][i]->y += dfactor_xy/2;

			if (dfactor_z>1) outswc[iteration][i]->z *= dfactor_z;
			outswc[iteration][i]->z += 0;
			if (dfactor_z>1)  outswc[iteration][i]->z += dfactor_z/2;

			outswc[iteration][i]->radius *= dfactor_xy; //use xy for now
		}

		//re-estimate the radius using the original image
		double real_thres = 40;

		VISIOCYTELONG szOriginalData[4] = {N,M,P, 1};

		int method_radius_est = 2;
		

		map<MyMarker*,int>ind;

		for(VISIOCYTELONG i = 0; i < outswc[iteration].size(); i++)
		{
			ind[outswc[iteration][i]] = i+1;
		}

		for(VISIOCYTELONG i = 0; i < outswc[iteration].size(); i++)
		{
			//printf(" node %ld of %ld.\n", i, outswc.size());
			outswc[iteration][i]->radius = markerRadius(data1d_1ch, szOriginalData, *(outswc[iteration][i]), real_thres, method_radius_est);
			
			int parentId = (outswc[iteration][i]->parent == 0) ? -1 : ind[outswc[iteration][i]->parent];
			
			if(iteration==1)
			{
				if (parentId!=-1) parentId = parentId + outswc[0].size();
			}
			if(iteration==2)
			{
				if (parentId!=-1) parentId = parentId + outswc[0].size() + outswc[1].size();
			}
				
			j++;
			file << j << " " <<  outswc[iteration][i]->type << " " <<  outswc[iteration][i]->x << " " <<  outswc[iteration][i]->y << " " <<  outswc[iteration][i]->z << " " << outswc[iteration][i]->radius << " " << parentId << endl ;			
		}
	}

    //Output
//    QString swc_name1 = PARA.inimg_file + "_APP2_ported1.swc";
//    QString swc_name2 = PARA.inimg_file + "_APP2_ported2.swc";
//    QString swc_name3 = PARA.inimg_file + "_APP2_ported3.swc";
    // NeuronTree nt;
    // nt.name = "tracing method";
    // writeSWC_file(swc_name.toStdString().c_str(),nt);
//	saveSWC_file(swc_name1.toStdString(), outswc[0]);
//	saveSWC_file(swc_name2.toStdString(), outswc[1]);
//	saveSWC_file(swc_name3.toStdString(), outswc[2]);
	
	//Combine image
	
/*	VISIOCYTELONG size_of_neuron[3], max_size, num_neuron;
	for (int iteration=0;iteration<3;iteration++)
	{
		size_of_neuron[iteration] = outswc[iteration].size();
	}
	max_size = *max_element(size_of_neuron,size_of_neuron+3);
	
	for (int iteration=0;iteration<3;iteration++)
	{
		if (max_size==size_of_neuron[iteration]) num_neuron = iteration;
	} */
	
	
	
	
	if(phi){delete [] phi; phi = 0;}
	//for(VISIOCYTELONG i = 0; i < outtree.size(); i++) delete outtree[i];
	//outtree.clear();
	if(data1d_1ch){delete []data1d_1ch; data1d_1ch = 0;}

	if(!bmenu)
	{
		if(data1d) {delete []data1d; data1d = 0;}
	}

	visiocyte_msg(QString("Now you can drag and drop the generated swc fle [%1] into Visiocyte.").arg(swc_name.toStdString().c_str()),bmenu);

	return;
}
