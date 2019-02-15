//last change: by PHC 20130905. Creating this file actually fix the Windows VC++ build problem of creating dynamic library files. (w/o this, the dll was created but not the .lib file and .exp file), and thus the plugin cannot be instantiated.

#include "swc2mask_plugin.h"

#include <iostream>
#include <vector>

#include "src/swc2mask.h"
#include "swc_convert.h"
#include "my_surf_objs.h"
#include "basic_surf_objs.h"
#include "customary_structs/visiocyte_neurontoolbox_para.h"


Q_EXPORT_PLUGIN2(swc2mask, SWC2MaskPlugin);

using namespace std;

static string basename(string para)
{
    int pos1 = para.find_last_of("/");
    int pos2 = para.find_last_of(".");
    if(pos1 == string::npos) pos1 = -1;
    if(pos2 == string::npos) pos2 = para.size();
    return para.substr(pos1+1, pos2 - pos1 -1);
}

// file_type("test.tif") == ".tif"
string file_type(string para)
{
    int pos = para.find_last_of(".");
    if(pos == string::npos) return string("unknown");
    else return para.substr(pos, para.size() - pos);
}


QStringList SWC2MaskPlugin::menulist() const
{
    return QStringList()
            << tr("swc2mask")
            << tr("about");
}

void SWC2MaskPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if(menu_name == "swc2mask")
    {
        QString fileName = QFileDialog::getOpenFileName(0, QObject::tr("Open Neuron Tree"), QObject::tr("."), QObject::tr("SWC Files (*.swc)"));
        if(fileName == "") {QMessageBox::information(0,"","No swc file open"); return;}

        string inswc_file = fileName.toStdString();
        vector<MyMarker*> inswc = readSWC_file(inswc_file);
        VISIOCYTELONG sz0 = 0, sz1 = 0, sz2 = 0;
        if(1)
        {
            MyMarker * marker = inswc[0];
            VISIOCYTELONG x = marker->x + 0.5;
            VISIOCYTELONG y = marker->y + 0.5;
            VISIOCYTELONG z = marker->z + 0.5;
            VISIOCYTELONG mx = x, my = y, mz = z, Mx = x, My = y, Mz = z;
            VISIOCYTELONG margin = 0;
            for(int i = 1; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                mx = MIN(x, mx);
                my = MIN(y, my);
                mz = MIN(z, mz);
                Mx = MAX(x, Mx);
                My = MAX(y, My);
                Mz = MAX(z, Mz);
                margin = MAX(margin, (VISIOCYTELONG)(marker->radius+0.5));
            }
            mx -= margin;
            my -= margin;
            mz -= margin;
            Mx += margin;
            My += margin;
            Mz += margin;

            sz0 = Mx - mx + 1;
            sz1 = My - my + 1;
            sz2 = Mz - mz + 1;
            for(int i = 0; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                marker->x = x - mx;
                marker->y = y - my;
                marker->z = z - mz;
            }
        }
        cout<<"size : "<<sz0<<"x"<<sz1<<"x"<<sz2<<endl;

        unsigned char * outimg1d = 0;
        if(!swc2mask(outimg1d, inswc, sz0, sz1, sz2))
        {
            QMessageBox::information(0, "", "Failed to do swc2mask !");
            return;
        }
        VISIOCYTELONG out_sz[4] = {sz0, sz1, sz2, 1};

        visiocytehandle newwin = callback.newImageWindow();
        Image4DSimple * new4DImage = new Image4DSimple();
        new4DImage->setData(outimg1d, sz0, sz1, sz2, 1, VISIOCYTE_UINT8);
        callback.setImage(newwin, new4DImage);
        callback.updateImageWindow(newwin);
    }
    else if(menu_name == "about")
    {
        QMessageBox::information(0, "SWC2Mask Plugin", \
                                 QObject::tr("This is swc2mask plugin"));
    }
}

QStringList SWC2MaskPlugin::funclist() const
{
    return QStringList()
            << tr("swc2mask")
            << tr("swc2maskbb")
            << tr("help");
}

bool SWC2MaskPlugin::dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if(func_name == "swc2mask")
    {
        vector<char*> * pinfiles = (input.size() >= 1) ? (vector<char*> *) input[0].p : 0;
        vector<char*> * poutfiles = (output.size() >= 1) ? (vector<char*> *) output[0].p : 0;
        vector<char*> * pparas = (input.size() >= 2) ? (vector<char*> *) input[1].p : 0;

        vector<char*> infiles = (pinfiles != 0) ? * pinfiles : vector<char*>();
        vector<char*> outfiles = (poutfiles != 0) ? * poutfiles : vector<char*>();
        vector<char*> paras = (pparas != 0) ? * pparas : vector<char*>();

        if(paras.size() != 0 && paras.size() != 3)
        {
            cerr<<"input para error!"<<endl; return false;
        }

        string inimg_file;
        string inswc_file;
        unsigned char * inimg1d = 0; int datatype = 0;
        VISIOCYTELONG in_sz[4];

        VISIOCYTELONG sz0 = 0, sz1 = 0, sz2 = 0;

        if(infiles.size() == 1)
        {
            inswc_file = infiles[0];
            if(file_type(inswc_file) != ".swc")
            {
                cerr<<"Input is not swc file"<<endl;
                return false;
            }
        }
        else if(infiles.size() == 2)
        {
            if(file_type(infiles[0]) == ".swc") inswc_file = infiles[0];
            else inimg_file = infiles[0];
            if(file_type(infiles[1]) == ".swc") inswc_file = infiles[1];
            else inimg_file = infiles[1];
            if(inswc_file == "" || inimg_file == "")
            {
                cerr<<"Input files error!"<<endl;
                return false;
            }
            if(!simple_loadimage_wrapper(callback, (char*)inimg_file.c_str(), inimg1d, in_sz, datatype))
            {cerr<<"Load image "<<inimg_file<<" error!"<<endl; return false;}
            //if(datatype != VISIOCYTE_UINT8){cerr<<"Currently only support 8bit image as input"<<endl; return false;}
            sz0 = in_sz[0]; sz1 = in_sz[1]; sz2 = in_sz[2];
            if(in_sz[3] > 1) {cout<<"Use red channel as input."<<endl;}
        }

        vector<MyMarker*> inswc = readSWC_file(inswc_file);
        string outimg_file = outfiles.empty() ? basename(inswc_file) + "_out.raw" : outfiles[0];
        if(infiles.size() == 1 && paras.empty())
        {
            MyMarker * marker = inswc[0];
            VISIOCYTELONG x = marker->x + 0.5;
            VISIOCYTELONG y = marker->y + 0.5;
            VISIOCYTELONG z = marker->z + 0.5;
            VISIOCYTELONG mx = x, my = y, mz = z, Mx = x, My = y, Mz = z;
            VISIOCYTELONG margin = 0;
            for(int i = 1; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                mx = MIN(x, mx);
                my = MIN(y, my);
                mz = MIN(z, mz);
                Mx = MAX(x, Mx);
                My = MAX(y, My);
                Mz = MAX(z, Mz);
                margin = MAX(margin, (VISIOCYTELONG)(marker->radius+0.5));
            }
            mx -= margin;
            my -= margin;
            mz -= margin;
            Mx += margin;
            My += margin;
            Mz += margin;

            sz0 = Mx - mx + 1;
            sz1 = My - my + 1;
            sz2 = Mz - mz + 1;
            for(int i = 0; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                marker->x = x - mx;
                marker->y = y - my;
                marker->z = z - mz;
            }
        }
        else if(infiles.size() == 1 && paras.size() == 3)
        {
            sz0 = atoi(paras[0]);
            sz1 = atoi(paras[1]);
            sz2 = atoi(paras[2]);
        }
        cout<<"size : "<<sz0<<"x"<<sz1<<"x"<<sz2<<endl;

        unsigned char * outimg1d = 0;
        if(!swc2mask(outimg1d, inswc, sz0, sz1, sz2)) return false;
        if(infiles.size() == 2)
        {
            VISIOCYTELONG tol_sz = sz0 * sz1 * sz2;
            if(datatype == VISIOCYTE_UINT8)
            {
                unsigned char * tmpimg1d = inimg1d;
                for(VISIOCYTELONG i = 0; i < tol_sz; i++)
                {
                    tmpimg1d[i] = (outimg1d[i] > 0) ? tmpimg1d[i] : 0;
                }
            }
            else if(datatype == VISIOCYTE_UINT16)
            {
                unsigned short int * tmpimg1d = (unsigned short int *) inimg1d;
                for(VISIOCYTELONG i = 0; i < tol_sz; i++)
                {
                    tmpimg1d[i] = (outimg1d[i] > 0) ? tmpimg1d[i] : 0;
                }
            }
            else if(datatype == VISIOCYTE_FLOAT32)
            {
                float * tmpimg1d = (float *) inimg1d;
                for(VISIOCYTELONG i = 0; i < tol_sz; i++)
                {
                    tmpimg1d[i] = (outimg1d[i] > 0) ? tmpimg1d[i] : 0;
                }
            }
            VISIOCYTELONG out_sz[4] = {sz0, sz1, sz2, 1};
            if(!simple_saveimage_wrapper(callback, outimg_file.c_str(), inimg1d, out_sz, datatype))
            {
                cerr<<"Unable to save image to file "<<outimg_file<<endl;
                return false;
            }
            if(inimg1d){delete [] inimg1d; inimg1d = 0;}
            //if(in_sz){delete [] in_sz; in_sz = 0;}
        }
        else
        {
            VISIOCYTELONG out_sz[4] = {sz0, sz1, sz2, 1};
            if(!simple_saveimage_wrapper(callback, outimg_file.c_str(), outimg1d, out_sz, VISIOCYTE_UINT8))
            {
                cerr<<"Unable to save image to file "<<outimg_file<<endl;
                return false;
            }
        }
        if(outimg1d){delete [] outimg1d; outimg1d = 0;}
        return true;
    }
    else if(func_name == "swc2maskbb")
    {
        vector<char*> * pinfiles = (input.size() >= 1) ? (vector<char*> *) input[0].p : 0;
        vector<char*> * poutfiles = (output.size() >= 1) ? (vector<char*> *) output[0].p : 0;
        vector<char*> * pparas = (input.size() >= 2) ? (vector<char*> *) input[1].p : 0;

        vector<char*> infiles = (pinfiles != 0) ? * pinfiles : vector<char*>();
        vector<char*> outfiles = (poutfiles != 0) ? * poutfiles : vector<char*>();
        vector<char*> paras = (pparas != 0) ? * pparas : vector<char*>();

        if(paras.size() != 0 && paras.size() != 6)
        {
            cout<<"./visiocyte -x swc2mask -f swc2maskbb -i input.swc -o output.tif -p xmin xmax ymin ymax zmin zmax"<<endl;
            return false;
        }

        string inimg_file;
        string inswc_file;
        unsigned char * inimg1d = 0;
        int datatype = 0;
        VISIOCYTELONG in_sz[4];

        VISIOCYTELONG sz0 = 0, sz1 = 0, sz2 = 0;

        if(infiles.size() == 1)
        {
            inswc_file = infiles[0];
            if(file_type(inswc_file) != ".swc")
            {
                cerr<<"Input is not swc file"<<endl;
                return false;
            }
        }

        vector<MyMarker*> inswc = readSWC_file(inswc_file);
        string outimg_file = outfiles.empty() ? basename(inswc_file) + "_out.tif" : outfiles[0];
        if(paras.empty())
        {
            MyMarker * marker = inswc[0];
            VISIOCYTELONG x = marker->x + 0.5;
            VISIOCYTELONG y = marker->y + 0.5;
            VISIOCYTELONG z = marker->z + 0.5;
            VISIOCYTELONG mx = x, my = y, mz = z, Mx = x, My = y, Mz = z;
            VISIOCYTELONG margin = 0;
            for(int i = 1; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                mx = MIN(x, mx);
                my = MIN(y, my);
                mz = MIN(z, mz);
                Mx = MAX(x, Mx);
                My = MAX(y, My);
                Mz = MAX(z, Mz);
                margin = MAX(margin, (VISIOCYTELONG)(marker->radius+0.5));
            }
            mx -= margin;
            my -= margin;
            mz -= margin;
            Mx += margin;
            My += margin;
            Mz += margin;

            cout<<"bounding box: "<<mx<<" "<<Mx<<" "<<my<<" "<<My<<" "<<mz<<" "<<Mz<<endl;

            sz0 = Mx - mx + 1;
            sz1 = My - my + 1;
            sz2 = Mz - mz + 1;
            for(int i = 0; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                marker->x = x - mx;
                marker->y = y - my;
                marker->z = z - mz;
            }
        }
        else
        {
            VISIOCYTELONG mx, my, mz, Mx, My, Mz;
            mx = atof(paras[0]);
            Mx = atof(paras[1]);
            my = atof(paras[2]);
            My = atof(paras[3]);
            mz = atof(paras[4]);
            Mz = atof(paras[5]);

            sz0 = Mx - mx + 1;
            sz1 = My - my + 1;
            sz2 = Mz - mz + 1;
            MyMarker * marker;
            for(int i = 0; i < inswc.size(); i++)
            {
                marker = inswc[i];
                VISIOCYTELONG x = marker->x + 0.5;
                VISIOCYTELONG y = marker->y + 0.5;
                VISIOCYTELONG z = marker->z + 0.5;
                marker->x = x - mx;
                marker->y = y - my;
                marker->z = z - mz;
            }
        }
        cout<<"size : "<<sz0<<"x"<<sz1<<"x"<<sz2<<endl;

        unsigned char * outimg1d = 0;
        if(!swc2mask(outimg1d, inswc, sz0, sz1, sz2)) return false;
        if(infiles.size() == 2)
        {
            VISIOCYTELONG tol_sz = sz0 * sz1 * sz2;
            if(datatype == VISIOCYTE_UINT8)
            {
                unsigned char * tmpimg1d = inimg1d;
                for(VISIOCYTELONG i = 0; i < tol_sz; i++)
                {
                    tmpimg1d[i] = (outimg1d[i] > 0) ? tmpimg1d[i] : 0;
                }
            }
            else if(datatype == VISIOCYTE_UINT16)
            {
                unsigned short int * tmpimg1d = (unsigned short int *) inimg1d;
                for(VISIOCYTELONG i = 0; i < tol_sz; i++)
                {
                    tmpimg1d[i] = (outimg1d[i] > 0) ? tmpimg1d[i] : 0;
                }
            }
            else if(datatype == VISIOCYTE_FLOAT32)
            {
                float * tmpimg1d = (float *) inimg1d;
                for(VISIOCYTELONG i = 0; i < tol_sz; i++)
                {
                    tmpimg1d[i] = (outimg1d[i] > 0) ? tmpimg1d[i] : 0;
                }
            }
            VISIOCYTELONG out_sz[4] = {sz0, sz1, sz2, 1};
            if(!simple_saveimage_wrapper(callback, outimg_file.c_str(), inimg1d, out_sz, datatype))
            {
                cerr<<"Unable to save image to file "<<outimg_file<<endl;
                return false;
            }
            if(inimg1d){delete [] inimg1d; inimg1d = 0;}
        }
        else
        {
            VISIOCYTELONG out_sz[4] = {sz0, sz1, sz2, 1};
            if(!simple_saveimage_wrapper(callback, outimg_file.c_str(), outimg1d, out_sz, VISIOCYTE_UINT8))
            {
                cerr<<"Unable to save image to file "<<outimg_file<<endl;
                return false;
            }
        }
        if(outimg1d){delete [] outimg1d; outimg1d = 0;}
        return true;
    }
    else if(func_name == "TOOLBOXswc2mask")
    {
        visiocyte_neurontoolbox_paras * toolbox_paras = (visiocyte_neurontoolbox_paras *)input.at(0).p;
        NeuronTree nt = toolbox_paras->nt;
        vector<MyMarker*> inswc = swc_convert(nt);

        string outimg_file = basename(nt.file.toStdString())+"_out.raw";
        cout<<"outimg_file = "<<outimg_file<<endl;

        VISIOCYTELONG sz0 = 0, sz1 = 0, sz2 = 0;
        visiocytehandle curwin = callback.getImageWindowList().empty() ? 0 : callback.currentImageWindow();
        View3DControl * view3d = callback.getView3DControl(curwin);
        if(1)
        {
            MyMarker * marker = inswc[0];
            VISIOCYTELONG x = marker->x + 0.5;
            VISIOCYTELONG y = marker->y + 0.5;
            VISIOCYTELONG z = marker->z + 0.5;
            VISIOCYTELONG mx = x, my = y, mz = z, Mx = x, My = y, Mz = z;
            VISIOCYTELONG margin = 0;
            for(int i = 1; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                mx = MIN(x, mx);
                my = MIN(y, my);
                mz = MIN(z, mz);
                Mx = MAX(x, Mx);
                My = MAX(y, My);
                Mz = MAX(z, Mz);
                margin = MAX(margin, (VISIOCYTELONG)(marker->radius+0.5));
            }
            mx -= margin;
            my -= margin;
            mz -= margin;
            Mx += margin;
            My += margin;
            Mz += margin;

            sz0 = Mx - mx + 1;
            sz1 = My - my + 1;
            sz2 = Mz - mz + 1;
            for(int i = 0; i < inswc.size(); i++)
            {
                marker = inswc[i];
                x = marker->x + 0.5;
                y = marker->y + 0.5;
                z = marker->z + 0.5;
                marker->x = x - mx;
                marker->y = y - my;
                marker->z = z - mz;
            }
        }
        else
        {
            Image4DSimple * p4dImage = callback.getImage(curwin);
            sz0 = p4dImage->getXDim();
            sz1 = p4dImage->getYDim();
            sz2 = p4dImage->getZDim();
        }
        cout<<"size : "<<sz0<<"x"<<sz1<<"x"<<sz2<<endl;

        unsigned char * outimg1d = 0;
        if(!swc2mask(outimg1d, inswc, sz0, sz1, sz2)) return false;

        visiocytehandle newwin = callback.newImageWindow();
        Image4DSimple * new4dImage = new Image4DSimple();
        //NeuronTree new_nt = swc_convert(inswc);
        new4dImage->setData(outimg1d, sz0, sz1, sz2, 1, VISIOCYTE_UINT8);
        callback.setImage(newwin, new4dImage);
        callback.updateImageWindow(newwin);
        callback.open3DWindow(newwin);
        //callback.setSWC(newwin, new_nt);
        for(int i = 0; i < inswc.size(); i++) delete inswc[i];
        return true;
    }
    else if(func_name == "help")
    {
        cout<<"Usage : visiocyte -x dll_name -f swc2mask -i <swc_file> [-p <sz0> <sz1> <sz2>] [-o <outimg_file>]"<<endl;
        cout<<endl;
        cout<<"Produce mask image from swc structure. The outimg size will be the bounding box of swc structure or provided size."<<endl;
    }
    return false;
}



