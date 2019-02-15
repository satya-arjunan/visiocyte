/*
 *  montage_image_sections .cpp
 *  montage_image_sections
 *
 *  Created by Yang, Jinzhu and Hanchuan Peng, on 11/22/10.
 *
 */

// Adapted and upgraded to VISIOCYTEPluginInterface2_1 by Jianlong Zhou, 2012-04-05
// add dofunc() by Jianlong Zhou, 2012-04-13

#include <iostream>
//#include "stackutil.h"
#include "montage_image_sections.h"
#include "visiocyte_message.h"

using namespace std;
//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(Montage, MONTAGEPlugin);

bool do_computation(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, int method_code);
void do_computation(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code);

template <class T>
void montage_image_sections (T *apsInput, T * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer,bool b_draw)
{
	VISIOCYTELONG i, j,k,n,count,m,row,column;

	VISIOCYTELONG mCount = iImageHeight * iImageWidth;

	column = (VISIOCYTELONG)(sqrt((double)iImageLayer)+0.5);
	//visiocyte_msg("1");
	//printf("column1=%d,",column);

	VISIOCYTELONG remainder = iImageLayer - column*column;
	if (remainder == 0)
	{
		row = column;
	}else
	{
		row = column+1;
	}

	for (i=0; i<iImageLayer; i++)
	{
		for (j=0; j<iImageHeight; j++)
		{
			for (k=0; k<iImageWidth; k++)
			{
				if (i < column)
				{
					aspOutput[(j)* column *iImageWidth + (k)  + (i * iImageWidth) ]= apsInput[i * mCount + j*iImageWidth + k];

				}else
				{
					aspOutput[(((i/column)*iImageHeight)+ j )* column* iImageWidth + k + ((i%column) * iImageWidth)]= apsInput[i * mCount + j*iImageWidth + k];
				}
			}
		}
	}
	if(b_draw)
	{
		for (j=0; j<row*iImageHeight; j=j+iImageHeight)
		{
			for( k=0; k<column*iImageWidth; k++)
			{
				aspOutput[j*column*iImageWidth+k] = 255;
			}
		}
		for (j=0; j<row*iImageHeight; j++)
		{
			for(k=0; k<column*iImageWidth; k= k+iImageWidth)
			{
				aspOutput[j*column*iImageWidth+k] = 255;
			}
		}

	}else
		return;

	//visiocyte_msg("2");
}
template <class T>
     void montage_image_stack(T *apsInput, T * aspOutput, VISIOCYTELONG iImageWidth, VISIOCYTELONG iImageHeight, VISIOCYTELONG iImageLayer,VISIOCYTELONG column,VISIOCYTELONG row, VISIOCYTELONG slice,VISIOCYTELONG n, VISIOCYTELONG r)
{
	VISIOCYTELONG i, j,k;
	VISIOCYTELONG mCount = iImageHeight * iImageWidth;
	VISIOCYTELONG mCount1 = column * row;
	for (i=0; i<slice; i++)
	{
		for (j=0; j<row; j++)
		{
			for (k=0; k<column; k++)
			{
				aspOutput[i * mCount1 + j*column + k] = 0;

			}
		}
	}
	//VISIOCYTELONG w = 7;
	for (j=0; j<iImageHeight; j++)
	{
		for (k=0; k<iImageWidth; k++)
		{
               if(j/row < r) // check boundary by Jianlong Zhou
                    aspOutput[((j/row)*(n)+(k/column))*mCount1 + (j%row)*column + (k%column)] = apsInput[j*iImageWidth+k];
		}
	}
}


//plugin funcs
const QString title = "montage_image_sections";
QStringList MONTAGEPlugin::menulist() const
{
    return QStringList()
	<< tr("montage_image_sections")
    << tr("revert a montage image to a stack")
	<< tr("Help");
}

void MONTAGEPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("montage_image_sections"))
	{
          do_computation(callback, parent,1 );
     }
     else if (menu_name == tr("revert a montage image to a stack"))
	{
		do_computation(callback, parent,2);

	}
	else if (menu_name == tr("Help"))
	{
		visiocyte_msg("This plugin produces a montage view of all z slices.");
		return;
	}
}


QStringList MONTAGEPlugin::funclist() const
{
	return QStringList()
		<<tr("mtg")
        <<tr("rmtg")
		<<tr("help");
}


bool MONTAGEPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
     if (func_name == tr("mtg"))
	{
        return do_computation(callback, input, output, 1);
	}
     else if (func_name == tr("rmtg"))
	{
        return do_computation(callback, input, output, 2);
	}
	else if(func_name == tr("help"))
	{
         cout<<"mtg - montage image slices to one image; rmtg - reverse the montage process. "<<endl;

         cout<<"Usage : visiocyte -x montage -f mtg -i <inimg_file> -o <outimg_file> -p <b_draw> "<<endl;
          cout<<"p_draw      whether to draw lines in the montage image, 0 for not, 1 for draw, default 1"<<endl;
          cout<<"e.g. visiocyte -x montage -f mtg -i input.raw -o output.raw -p 1"<<endl;
		cout<<endl;

          cout<<"Usage : visiocyte -x montage -f rmtg -i <inimg_file> -o <outimg_file> -p <b_draw> <c> <r> "<<endl;
          cout<<"p_draw      whether to draw lines in the montage image, 0 for not, 1 for draw, default 1"<<endl;
          cout<<"c           numbmer of column blocks, default 7"<<endl;
          cout<<"r           numbmer of row blocks, default 7"<<endl;
          cout<<"e.g. visiocyte -x montage -f mtg -i input.raw -o output.raw -p 1 7 7"<<endl;
		cout<<endl;

		return true;
	}
}


bool do_computation(VISIOCYTEPluginCallback2 &callback, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, int method_code)
{
     cout<<"Welcome to Montage image sections"<<endl;
	if (output.size() != 1) return false;

     int b_draw = 1; //Do you want to draw line?
     VISIOCYTELONG c = 7,r = 7; // for method_code=2

     if (input.size()>=2)
     {
          vector<char*> paras = (*(vector<char*> *)(input.at(1).p));
          if(paras.size() >= 1) b_draw = atoi(paras.at(0));

          if(method_code==2)
          {
               if(paras.size() >= 2) c = atoi(paras.at(1));
               if(paras.size() >= 3) r = atoi(paras.at(2));
          }
     }
	char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
	char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);

     cout<<"b_draw = "<<b_draw<<endl;
     if(method_code==2)
     {
          cout<<"c = "<<c<<endl;
          cout<<"r = "<<r<<endl;
     }
	cout<<"inimg_file = "<<inimg_file<<endl;
	cout<<"outimg_file = "<<outimg_file<<endl;

	unsigned char * data1d = 0;
    VISIOCYTELONG in_sz[4];

	int datatype;
    if(!simple_loadimage_wrapper(callback, inimg_file, data1d, in_sz, datatype))
     {
          cerr<<"load image "<<inimg_file<<" error!"<<endl;
          return false;
     }

     VISIOCYTELONG sz0 = in_sz[0];
     VISIOCYTELONG sz1 = in_sz[1];
     VISIOCYTELONG sz2 = in_sz[2];
	VISIOCYTELONG sz3 = in_sz[3];
	VISIOCYTELONG pagesz_sub = sz0*sz1*sz2;


     //----------------------------------------------------------------------------------------------------------------------------------
     VISIOCYTELONG h;
	VISIOCYTELONG d;
	VISIOCYTELONG channelsz,channelsz1;
	VISIOCYTELONG column,row;
	channelsz =channelsz1=column = row = 0;

	if(method_code == 1)
	{

		column = (VISIOCYTELONG)(sqrt((double)sz2)+ 0.5);
		VISIOCYTELONG remainder= sz2 - column*column;
		if (remainder == 0)
		{
			row = column;
			channelsz1 = (sz0)*(sz1)*sz2;
		}else
		{
			row = column+1;
			channelsz1 = (sz0)*(sz1)*column*row;
		}
		channelsz = sz0*sz1*sz2;
		//  printf("column=%d,remainder=%d,row=%d sz3=%d\n",column,remainder,row,sz3);
		void *pData=NULL;
		VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
		switch (datatype)
		{
			case 1:

				try
                    {
                         pData = (void *)(new unsigned char [sz3*channelsz1]());
                    }
				catch (...)
                    {
                         visiocyte_msg("Fail to allocate memory in data combination.");
                         if (pData) {delete []pData; pData=0;}
                         return false;
                    }

                    {
                         unsigned char * pSubtmp_uint8 = data1d; //pSub.begin();

                         for (VISIOCYTELONG ich=0; ich<sz3; ich++)
                              montage_image_sections(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz1, sz0, sz1, sz2,b_draw);
                    }
				break;

			case 2:
				try
                    {
                         pData = (void *)(new unsigned short int [sz3*channelsz1]());
                    }
				catch (...)
                    {
                         visiocyte_msg("Fail to allocate memory in data combination.");
                         if (pData) {delete []pData; pData=0;}
                         return false;
                    }

                    {
                         unsigned short int * pSubtmp_uint16 =(unsigned short int *)data1d; // (short int *)pSub.begin();

                         for (VISIOCYTELONG ich=0; ich<sz3; ich++)
                              montage_image_sections(pSubtmp_uint16+ich*channelsz, (unsigned short int *)pData+ich*channelsz1, sz0, sz1, sz2,b_draw );
                    }
				break;
			case 4:

				try
                    {
                         pData = (void *)(new float [sz3*channelsz1]());
                    }
				catch (...)
                    {
                         visiocyte_msg("Fail to allocate memory in data combination.");
                         if (pData) {delete []pData; pData=0;}
                         return false;
                    }

                    {
                         float * pSubtmp_float32 = (float *)data1d; //(float *)pSub.begin();

                         for (VISIOCYTELONG ich=0; ich<sz3; ich++)
                              montage_image_sections(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2,b_draw );
                    }

				break;
			default:
				break;
		}
		int end_t = clock();


          VISIOCYTELONG out_sz[4];
          out_sz[0]=column*sz0, out_sz[1]=row*sz1, out_sz[2]=1, out_sz[3]=sz3;
          simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)pData, out_sz, datatype);

          if(pData) {delete []pData; pData =0;}

	}else if (method_code ==2)
	{
		if (sz2 >1)
		{
			visiocyte_msg("The image must be a one slice.",0);
			return false;
		}

          //	  column = floor(sz0/n);
		//	  row = column;
		column = floor((double)(sz0/c));
		row = floor((double)(sz1/r));

		//  VISIOCYTELONG scount = n*(n+1);
		VISIOCYTELONG scount = r*c;
		channelsz = sz0*sz1*sz2;
		channelsz1 = c*r*column*row;
		// channelsz1 = scount*column*row;
		//  printf("column=%d,remainder=%d,row=%d sz3=%d\n",column,remainder,row,sz3);
		void *pData=NULL;
		VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
		switch (datatype)
		{
			case 1:
				try
                    {
                         pData = (void *)(new unsigned char [sz3*channelsz1]());
                    }
				catch (...)
                    {
                         visiocyte_msg("Fail to allocate memory in data combination.",0);
                         if (pData) {delete []pData; pData=0;}
                         return false;
                    }

                    {
                         unsigned char * pSubtmp_uint8 = (unsigned char*)data1d; // pSub.begin();

                         for (VISIOCYTELONG ich=0; ich<sz3; ich++)
                         {
                              montage_image_stack(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz1, sz0, sz1, sz2, column, row, scount, c, r);
                              cout<< "channelsz =" << channelsz  <<endl;
                              cout<< "channelsz1 ="<< channelsz1 <<endl;
                         }
                    }
				break;

			case 2:
				try
                    {
                         pData = (void *)(new unsigned short int [sz3*channelsz1]());
                    }
				catch (...)
                    {
                         visiocyte_msg("Fail to allocate memory in data combination.",0);
                         if (pData) {delete []pData; pData=0;}
                         return false;
                    }

                    {
                         unsigned short int * pSubtmp_uint16 = (unsigned short int *)data1d; //(short int *)pSub.begin();

                         for (VISIOCYTELONG ich=0; ich<sz3; ich++)
                              montage_image_stack(pSubtmp_uint16+ich*channelsz, (unsigned short int *)pData+ich*channelsz1,  sz0, sz1, sz2,column,row,scount,c, r);
                    }
				break;
			case 4:

				try
                    {
                         pData = (void *)(new float [sz3*channelsz1]());
                    }
				catch (...)
                    {
                         visiocyte_msg("Fail to allocate memory in data combination.",0);
                         if (pData) {delete []pData; pData=0;}
                         return false;
                    }

                    {
                         float * pSubtmp_float32 = (float *)data1d; //(float *)pSub.begin();

                         for (VISIOCYTELONG ich=0; ich<sz3; ich++)
                              montage_image_stack(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2,column,row,scount,c, r);
                    }

				break;
			default:
				break;
		}
		int end_t = clock();

          VISIOCYTELONG out_sz[4];
          out_sz[0]=column, out_sz[1]=row, out_sz[2]=scount, out_sz[3]=sz3;
          simple_saveimage_wrapper(callback, outimg_file, (unsigned char *)pData, out_sz, datatype);

          if(pData) {delete []pData; pData =0;}
	}

     if(data1d) {delete []data1d; data1d=0;}

     return true;
}



void do_computation(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int method_code)
{
	visiocytehandle curwin = callback.currentImageWindow();

     if (!curwin)
	{
		visiocyte_msg("You don't have any image open in the main window.");
		return;
	}
	int start_t = clock(); // record time point

	Image4DSimple* subject = callback.getImage(curwin);
	QString m_InputFileName = callback.getImageName(curwin);

	if (!subject)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}
	Image4DProxy<Image4DSimple> pSub(subject);

	VISIOCYTELONG sz0 = subject->getXDim();
     VISIOCYTELONG sz1 = subject->getYDim();
     VISIOCYTELONG sz2 = subject->getZDim();
	VISIOCYTELONG sz3 = subject->getCDim();
	VISIOCYTELONG pagesz_sub = sz0*sz1*sz2;
	bool b_draw;
	if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to draw line?"), QMessageBox::Yes, QMessageBox::No))
	{
		b_draw = true;
	}else
	{
		b_draw = false;

	}
	//----------------------------------------------------------------------------------------------------------------------------------


     VISIOCYTELONG h;
	VISIOCYTELONG d;
	VISIOCYTELONG channelsz,channelsz1;
	VISIOCYTELONG column,row;
	channelsz =channelsz1=column = row = 0;

	if(method_code == 1)
	{

		column = (VISIOCYTELONG)(sqrt((double)sz2)+ 0.5);
		VISIOCYTELONG remainder= sz2 - column*column;
		if (remainder == 0)
		{
			row = column;
			channelsz1 = (sz0)*(sz1)*sz2;
		}else
		{
			row = column+1;
			channelsz1 = (sz0)*(sz1)*column*row;
		}
		channelsz = sz0*sz1*sz2;
		//  printf("column=%d,remainder=%d,row=%d sz3=%d\n",column,remainder,row,sz3);
		void *pData=NULL;
		VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
		switch (subject->getDatatype())
		{
			case VISIOCYTE_UINT8:

				try
			{
				pData = (void *)(new unsigned char [sz3*channelsz1]());
			}
				catch (...)
			{
				visiocyte_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}

			{
				unsigned char * pSubtmp_uint8 = pSub.begin();

				for (VISIOCYTELONG ich=0; ich<sz3; ich++)
					montage_image_sections(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz1, sz0, sz1, sz2,b_draw);
			}
				break;

			case VISIOCYTE_UINT16:
				try
			{
				pData = (void *)(new short int [sz3*channelsz1]());
			}
				catch (...)
			{
				visiocyte_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}

			{
				short int * pSubtmp_uint16 = (short int *)pSub.begin();

				for (VISIOCYTELONG ich=0; ich<sz3; ich++)
					montage_image_sections(pSubtmp_uint16+ich*channelsz, (short int *)pData+ich*channelsz1, sz0, sz1, sz2,b_draw );
			}
				break;
			case VISIOCYTE_FLOAT32:

				try
			{
				pData = (void *)(new float [sz3*channelsz1]());
			}
				catch (...)
			{
				visiocyte_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}

			{
				float * pSubtmp_float32 = (float *)pSub.begin();

				for (VISIOCYTELONG ich=0; ich<sz3; ich++)
					montage_image_sections(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2,b_draw );
			}

				break;
			default:
				break;
		}
		int end_t = clock();
		//	printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);
		///visiocyte_msg("2");
		//printf("column=%d,remainder=%d,row=%d\n",column,remainder,row);
		Image4DSimple p4DImage;

		p4DImage.setData((unsigned char*)pData, (VISIOCYTELONG)column*sz0, (VISIOCYTELONG)row*sz1, 1, sz3, subject->getDatatype());
		//p4DImage.setData((unsigned char*)pData, (VISIOCYTELONG)column*(sz0), (VISIOCYTELONG)row*(sz1), 1, sz3, subject->getDatatype());
		//	printf("sz0= %d sz1=%d sz2=%d dd=%d vv=%d\n", sz0,sz1,sz2,dd,vv);
		visiocytehandle newwin;
		if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
			newwin = callback.currentImageWindow();
		else
			newwin = callback.newImageWindow();

		callback.setImage(newwin, &p4DImage);
		callback.setImageName(newwin, QString("montage_image_sections"));
		callback.updateImageWindow(newwin);
	}else if (method_code ==2)
	{
		if (sz2 >1)
		{
			QMessageBox::information(0, title, QObject::tr("The image must be a one slice."));
			return;
		}
		VISIOCYTELONG c,r;
		SetsizeDialog dialog(callback, parent);
		if (dialog.exec()!=QDialog::Accepted)
			return;
		else
		{
			c = dialog.coord_y->text().toLong();
			r = dialog.coord_x->text().toLong();
		}
		//	  column = floor(sz0/n);
		//	  row = column;
		column = floor((double)(sz0/c));
		row = floor((double)(sz1/r));

		//  VISIOCYTELONG scount = n*(n+1);
		VISIOCYTELONG scount = r*c;
		channelsz = sz0*sz1*sz2;
		channelsz1 = c*r*column*row;
		// channelsz1 = scount*column*row;
		//  printf("column=%d,remainder=%d,row=%d sz3=%d\n",column,remainder,row,sz3);
		void *pData=NULL;
		VISIOCYTELONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
		switch (subject->getDatatype())
		{
			case VISIOCYTE_UINT8:
				try
			{
				pData = (void *)(new unsigned char [sz3*channelsz1]());
			}
				catch (...)
			{
				visiocyte_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}

			{
				unsigned char * pSubtmp_uint8 = pSub.begin();

				for (VISIOCYTELONG ich=0; ich<sz3; ich++)
					montage_image_stack(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz1, sz0, sz1, sz2,column,row,scount,c, r);
			}
				break;

			case VISIOCYTE_UINT16:
				try
			{
				pData = (void *)(new short int [sz3*channelsz1]());
			}
				catch (...)
			{
				visiocyte_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}

			{
				unsigned short int * pSubtmp_uint16 = (unsigned short int *)pSub.begin();

				for (VISIOCYTELONG ich=0; ich<sz3; ich++)
					montage_image_stack(pSubtmp_uint16+ich*channelsz, (unsigned short int *)pData+ich*channelsz1,  sz0, sz1, sz2,column,row,scount,c, r);
			}
				break;
			case VISIOCYTE_FLOAT32:

				try
			{
				pData = (void *)(new float [sz3*channelsz1]());
			}
				catch (...)
			{
				visiocyte_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}

			{
				float * pSubtmp_float32 = (float *)pSub.begin();

				for (VISIOCYTELONG ich=0; ich<sz3; ich++)
					montage_image_stack(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2,column,row,scount,c, r);
			}

				break;
			default:
				break;
		}
		int end_t = clock();
		//	printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);
		///visiocyte_msg("2");
		//printf("column=%d,remainder=%d,row=%d\n",column,remainder,row);
		Image4DSimple p4DImage;

		p4DImage.setData((unsigned char*)pData, (VISIOCYTELONG)column, (VISIOCYTELONG)row, scount, sz3, subject->getDatatype());
		//p4DImage.setData((unsigned char*)pData, (VISIOCYTELONG)column*(sz0), (VISIOCYTELONG)row*(sz1), 1, sz3, subject->getDatatype());
		//	printf("sz0= %d sz1=%d sz2=%d dd=%d vv=%d\n", sz0,sz1,sz2,dd,vv);
		visiocytehandle newwin;
		if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
			newwin = callback.currentImageWindow();
		else
			newwin = callback.newImageWindow();

		callback.setImage(newwin, &p4DImage);
		callback.setImageName(newwin, QString("revert a montage image to a stack"));
		callback.updateImageWindow(newwin);

	}
	//VISIOCYTELONG width= column*sz0;
	//VISIOCYTELONG Height = row*sz1;

	//----------------------------------------------------------------------------------------------------------------------------------

}
void SetsizeDialog::update()
{
	//get current data

	NX = coord_x->text().toLong()-1;
	NY = coord_y->text().toLong()-1;
	//NZ = coord_z->text().toLong()-1;


}
