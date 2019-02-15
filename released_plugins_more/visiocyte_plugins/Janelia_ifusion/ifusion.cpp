/* ifusion.cpp
 * 2011-08-31: the program is created by Yang Yu
 */

#include <QtGui>

#include <cmath>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <iostream>


#include "volimg_proc.h"
#include "img_definition.h"
#include "basic_landmark.h"

#include "basic_landmark.h"
#include "basic_4dimage.h"

#include "../istitch/y_imglib.h"

#include "ifusion.h"

using namespace std;

//
// image normalization and image blending
//
// inormalizer: normalizing and linear blending
// iblender: linear blending
//

//
Q_EXPORT_PLUGIN2(ifusion, ImageFusionPlugin);

//plugin
const QString title = "Image Fusion";

//
class AdjustPara
{
public:
    void init(VISIOCYTELONG n)
    {
        a=NULL;
        b=NULL;

        try
        {
            a = new float [n];
            b = new float [n];

            memset(a, 0, sizeof(float)*n);
            memset(b, 0, sizeof(float)*n);
        }
        catch (...)
        {
            printf("Fail to allocate memory for a and b!\n");

            clean();

            return;
        }
    }

    void clean()
    {
        if(a){delete []a; a=NULL;}
        if(b){delete []b; b=NULL;}
    }

public:
    float *a, *b; // r, g, b, ref
    VISIOCYTELONG num_tile;
};

// normalizing func
template<class Tdata>
bool computeAdjustPara(Tdata *f, Tdata *g, VISIOCYTELONG szimg, float &a, float &b)
{
    //
    a=0.0; b=0.0;

    //
    //    float sumfg=0.0;
    //    float sumgg=0.0;

    float meanf=0.0;
    float meang=0.0;

    float N = (float) szimg;
    float coef = sqrt( (N-1.0)/N );

    for (VISIOCYTELONG i=0; i<szimg; i++)
    {
        float valf=(float)f[i];
        float valg=(float)g[i];

        meanf += valf;
        meang += valg;

        //        sumfg += valf*valg;
        //        sumgg += valg*valg;
    }
    meanf /= N;
    meang /= N;

    // least square
    // a might be negative, this will introduce some errors, will fix it later
    //    a = (sumfg - (float)szimg*meanf*meang)/(sumgg - (float)szimg*meang*meang);
    //    b = meanf - a*meang;


    // use mean and standard deviation to solve a and b
    // in this way, a will always be positive
    float stdf=0.0, stdg=0.0;
    for (VISIOCYTELONG i=0; i<szimg; i++)
    {
        float valf=(float)f[i];
        float valg=(float)g[i];

        stdf += (valf-meanf)*(valf-meanf);
        stdg += (valg-meang)*(valg-meang);
    }
    stdf /= (N-1.0);
    stdg /= (N-1.0);

    stdf = sqrt(stdf);
    stdg = sqrt(stdg);

    a = stdf/stdg*coef;
    b = meanf - a*meang;

    return true;
}

// normalizing func
template<class Tdata>
bool findHighestIntensityLevelTile(Tdata *f, Tdata *g, VISIOCYTELONG szimg, VISIOCYTELONG &tilen)
{
    float meanf=0.0;
    float meang=0.0;

    float N = (float) szimg;
    float coef = sqrt( (N-1.0)/N );

    for (VISIOCYTELONG i=0; i<szimg; i++)
    {
        float valf=(float)f[i];
        float valg=(float)g[i];

        meanf += valf;
        meang += valg;
    }
    meanf /= N;
    meang /= N;


    qDebug()<<"mean value ..."<<meanf<<meang;

    return true;
}

// reconstruct tiles into one stitched image
template <class Tdata>
bool ireconstructingwnorm(Tdata *pVImg, Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim, Tdata intensityrange, QList<AdjustPara> adjparaList,VISIOCYTEPluginCallback2 &callback)
{
    //
    VISIOCYTELONG vx = vim.sz[0];
    VISIOCYTELONG vy = vim.sz[1];
    VISIOCYTELONG vz = vim.sz[2];
    VISIOCYTELONG vc = vim.sz[3];

    VISIOCYTELONG pagesz = vx*vy*vz;
    VISIOCYTELONG imgsz = pagesz*vc;

    float *pTmp = NULL;
    try {
        pTmp = new float [imgsz];
        memset(pTmp, 0.0, sizeof(float)*imgsz);
    } catch (...) {
        printf("Fail to allocate memory for pTmp!\n");
        if(pTmp){delete []pTmp; pTmp=NULL;}
        return false;
    }

    // fusion
    string fn;
    for(VISIOCYTELONG ii=0; ii<vim.number_tiles; ii++)
    {
        // loading relative imagg files
        VISIOCYTELONG sz_relative[4];
        int datatype_relative = 0;
        unsigned char* relative1d = 0;

        //
        if(ii==0)
        {
            fn = vim.lut[ii].fn_img;
        }
        else
        {
            QString curPath = QFileInfo(QString(vim.lut[0].fn_img.c_str())).path();;

            fn = curPath.append("/").append( QString(vim.lut[ii].fn_img.c_str()) ).toStdString();
        }

        //
        if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), relative1d, sz_relative, datatype_relative)!=true)
        {
            fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",vim.lut[ii].fn_img.c_str());
            return false;
        }
        VISIOCYTELONG rx=sz_relative[0], ry=sz_relative[1], rz=sz_relative[2], rc=sz_relative[3];

        //
        VISIOCYTELONG tile2vi_xs = vim.lut[ii].start_pos[0]-vim.min_vim[0];
        VISIOCYTELONG tile2vi_xe = vim.lut[ii].end_pos[0]-vim.min_vim[0];
        VISIOCYTELONG tile2vi_ys = vim.lut[ii].start_pos[1]-vim.min_vim[1];
        VISIOCYTELONG tile2vi_ye = vim.lut[ii].end_pos[1]-vim.min_vim[1];
        VISIOCYTELONG tile2vi_zs = vim.lut[ii].start_pos[2]-vim.min_vim[2];
        VISIOCYTELONG tile2vi_ze = vim.lut[ii].end_pos[2]-vim.min_vim[2];

        VISIOCYTELONG x_start = (0 > tile2vi_xs) ? 0 : tile2vi_xs;
        VISIOCYTELONG x_end = (vx-1 < tile2vi_xe) ? vx-1 : tile2vi_xe;
        VISIOCYTELONG y_start = (0 > tile2vi_ys) ? 0 : tile2vi_ys;
        VISIOCYTELONG y_end = (vy-1 < tile2vi_ye) ? vy-1 : tile2vi_ye;
        VISIOCYTELONG z_start = (0 > tile2vi_zs) ? 0 : tile2vi_zs;
        VISIOCYTELONG z_end = (vz-1 < tile2vi_ze) ? vz-1 : tile2vi_ze;

        x_end++;
        y_end++;
        z_end++;

        //suppose all tiles with same color dimensions
        if(rc>vc)
            rc = vc;

        VISIOCYTELONG itile;
        for(itile=0; itile<adjparaList.size(); itile++)
        {
            if(ii==adjparaList.at(itile).num_tile) break;
        }

        //
        Tdata *prelative = (Tdata *)relative1d;

        for(VISIOCYTELONG c=0; c<rc; c++)
        {
            VISIOCYTELONG o_c = c*vx*vy*vz;
            VISIOCYTELONG o_r_c = c*rx*ry*rz;
            for(VISIOCYTELONG k=z_start; k<z_end; k++)
            {
                VISIOCYTELONG o_k = o_c + k*vx*vy;
                VISIOCYTELONG o_r_k = o_r_c + (k-z_start)*rx*ry;

                for(VISIOCYTELONG j=y_start; j<y_end; j++)
                {
                    VISIOCYTELONG o_j = o_k + j*vx;
                    VISIOCYTELONG o_r_j = o_r_k + (j-y_start)*rx;
                    for(VISIOCYTELONG i=x_start; i<x_end; i++)
                    {
                        VISIOCYTELONG idx = o_j + i;
                        VISIOCYTELONG idx_r = o_r_j + (i-x_start);

                        float val = (float)(prelative[idx_r]);

                        if(itile<adjparaList.size())
                        {
                            val = val*adjparaList.at(itile).a[c] + adjparaList.at(itile).b[c]; // normalizing
                        }

                        //
                        float coef;
                        if(!computeWeights<REAL>(vim, i, j, k, ii, coef) )
                        {
                            printf("Fail to call function computeWeights!\n");
                            return false;
                        }

                        pTmp[idx] += val*coef; // linear blending
                    }
                }
            }
        }

        //de-alloc
        if(relative1d) {delete []relative1d; relative1d=0;}
    }

    float minval, maxval;
    for(VISIOCYTELONG c=0; c<vc; c++)
    {
        VISIOCYTELONG offsets = c*pagesz;

        minval=1e9;
        maxval=-1e9;
        for (VISIOCYTELONG i=0; i<pagesz; i++)
        {
            VISIOCYTELONG idx = offsets+i;

            float val=pTmp[idx];

            if(minval>val) minval = val;
            if(maxval<val) maxval = val;
        }
        maxval -= minval;

        for (VISIOCYTELONG i=0; i<pagesz; i++)
        {
            VISIOCYTELONG idx = offsets+i;

            pVImg[idx] = (Tdata) (intensityrange * (pTmp[idx] - minval)/maxval);
        }
    }

    //de-alloc
    if(pTmp) {delete []pTmp; pTmp=NULL;}

    return true;
}

// reconstruct tiles into one stitched image
template <class Tdata>
bool ireconstructing(Tdata *pVImg, Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim, Tdata intensityrange,VISIOCYTEPluginCallback2 &callback)
{
    //
    VISIOCYTELONG vx = vim.sz[0];
    VISIOCYTELONG vy = vim.sz[1];
    VISIOCYTELONG vz = vim.sz[2];
    VISIOCYTELONG vc = vim.sz[3];

    VISIOCYTELONG pagesz = vx*vy*vz;
    VISIOCYTELONG imgsz = pagesz*vc;

    float *pTmp = NULL;
    try {
        pTmp = new float [imgsz];
        memset(pTmp, 0.0, sizeof(float)*imgsz);
    } catch (...) {
        printf("Fail to allocate memory for pTmp!\n");
        if(pTmp){delete []pTmp; pTmp=NULL;}
        return false;
    }

    // fusion
    string fn;
    for(VISIOCYTELONG ii=0; ii<vim.number_tiles; ii++)
    {
        // loading relative imagg files
        VISIOCYTELONG sz_relative[4];
        int datatype_relative = 0;
        unsigned char* relative1d = 0;

        //
        if(ii==0)
        {
            fn = vim.lut[ii].fn_img;
        }
        else
        {
            QString curPath = QFileInfo(QString(vim.lut[0].fn_img.c_str())).path();;

            fn = curPath.append("/").append( QString(vim.lut[ii].fn_img.c_str()) ).toStdString();
        }

        //
        if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), relative1d, sz_relative, datatype_relative)!=true)
        {
            fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",vim.lut[ii].fn_img.c_str());
            return false;
        }
        VISIOCYTELONG rx=sz_relative[0], ry=sz_relative[1], rz=sz_relative[2], rc=sz_relative[3];

        //
        VISIOCYTELONG tile2vi_xs = vim.lut[ii].start_pos[0]-vim.min_vim[0];
        VISIOCYTELONG tile2vi_xe = vim.lut[ii].end_pos[0]-vim.min_vim[0];
        VISIOCYTELONG tile2vi_ys = vim.lut[ii].start_pos[1]-vim.min_vim[1];
        VISIOCYTELONG tile2vi_ye = vim.lut[ii].end_pos[1]-vim.min_vim[1];
        VISIOCYTELONG tile2vi_zs = vim.lut[ii].start_pos[2]-vim.min_vim[2];
        VISIOCYTELONG tile2vi_ze = vim.lut[ii].end_pos[2]-vim.min_vim[2];

        VISIOCYTELONG x_start = (0 > tile2vi_xs) ? 0 : tile2vi_xs;
        VISIOCYTELONG x_end = (vx-1 < tile2vi_xe) ? vx-1 : tile2vi_xe;
        VISIOCYTELONG y_start = (0 > tile2vi_ys) ? 0 : tile2vi_ys;
        VISIOCYTELONG y_end = (vy-1 < tile2vi_ye) ? vy-1 : tile2vi_ye;
        VISIOCYTELONG z_start = (0 > tile2vi_zs) ? 0 : tile2vi_zs;
        VISIOCYTELONG z_end = (vz-1 < tile2vi_ze) ? vz-1 : tile2vi_ze;

        x_end++;
        y_end++;
        z_end++;

        //suppose all tiles with same color dimensions
        if(rc>vc)
            rc = vc;

        //
        Tdata *prelative = (Tdata *)relative1d;

        for(VISIOCYTELONG c=0; c<rc; c++)
        {
            VISIOCYTELONG o_c = c*vx*vy*vz;
            VISIOCYTELONG o_r_c = c*rx*ry*rz;
            for(VISIOCYTELONG k=z_start; k<z_end; k++)
            {
                VISIOCYTELONG o_k = o_c + k*vx*vy;
                VISIOCYTELONG o_r_k = o_r_c + (k-z_start)*rx*ry;

                for(VISIOCYTELONG j=y_start; j<y_end; j++)
                {
                    VISIOCYTELONG o_j = o_k + j*vx;
                    VISIOCYTELONG o_r_j = o_r_k + (j-y_start)*rx;
                    for(VISIOCYTELONG i=x_start; i<x_end; i++)
                    {
                        VISIOCYTELONG idx = o_j + i;
                        VISIOCYTELONG idx_r = o_r_j + (i-x_start);

                        float val = (float)(prelative[idx_r]);

                        //
                        float coef;
                        if(!computeWeights<REAL>(vim, i, j, k, ii, coef) )
                        {
                            printf("Fail to call function computeWeights!\n");
                            return false;
                        }

                        pTmp[idx] += val*coef; // linear blending

                    }
                }
            }
        }

        //de-alloc
        if(relative1d) {delete []relative1d; relative1d=0;}
    }

    float minval, maxval;
    for(VISIOCYTELONG c=0; c<vc; c++)
    {
        VISIOCYTELONG offsets = c*pagesz;

        minval=1e9;
        maxval=-1e9;
        for (VISIOCYTELONG i=0; i<pagesz; i++)
        {
            VISIOCYTELONG idx = offsets+i;

            float val=pTmp[idx];

            if(minval>val) minval = val;
            if(maxval<val) maxval = val;
        }
        maxval -= minval;

        for (VISIOCYTELONG i=0; i<pagesz; i++)
        {
            VISIOCYTELONG idx = offsets+i;

            pVImg[idx] = (Tdata) (intensityrange * (pTmp[idx] - minval)/maxval);
        }
    }

    //de-alloc
    if(pTmp) {delete []pTmp; pTmp=NULL;}

    return true;
}


// funcs
QStringList ImageFusionPlugin::funclist() const
{
    return QStringList() << "iblender"
                         <<"inormalizer"
                        <<"help";
}

bool ImageFusionPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback, QWidget * parent)
{
    if (func_name == tr("help"))
    {
        printf("\nUsage: visiocyte -x ifusion -f iblender -i <folder> -o <output_image> -p \"#s <save_blending_result zero(false)/nonzero(true)>\"\n");
        printf("\nUsage: visiocyte -x ifusion -f inormalizer -i <folder> -o <output_image> -p \"#s <save_blending_result zero(false)/nonzero(true)>\"\n");
    }
    else if (func_name == tr("iblender")) // linear blending
    {
        // parsing parameters
        if(input.size()<1) return false; // no inputs

        vector<char*> * infilelist = (vector<char*> *)(input.at(0).p);
        vector<char*> * paralist;
        vector<char*> * outfilelist;
        if(infilelist->empty())
        {
            //print Help info
            printf("\nUsage: visiocyte -x ifusion -f iblender -i <folder> -o <output_image> -p \"#s <save_blending_result zero(false)/nonzero(true)>\"\n");

            return true;
        }

        char * infile = infilelist->at(0); // input images
        char * paras = NULL; // parameters
        char * outfile = NULL; // outputs

        if(output.size()>0) { outfilelist = (vector<char*> *)(output.at(0).p); outfile = outfilelist->at(0);}  // specify output
        if(input.size()>1) { paralist = (vector<char*> *)(input.at(1).p); paras =  paralist->at(0);} // parameters

        bool b_saveimage = true; // save the blended image by default

        if(paras)
        {
            int argc = 0;
            int len = strlen(paras);
            int posb[200];
            char * myparas = new char[len];
            strcpy(myparas, paras);
            for(int i = 0; i < len; i++)
            {
                if(i==0 && myparas[i] != ' ' && myparas[i] != '\t')
                {
                    posb[argc++] = i;
                }
                else if((myparas[i-1] == ' ' || myparas[i-1] == '\t') && (myparas[i] != ' ' && myparas[i] != '\t'))
                {
                    posb[argc++] = i;
                }
            }
            char ** argv = new char* [argc];
            for(int i = 0; i < argc; i++)
            {
                argv[i] = myparas + posb[i];
            }
            for(int i = 0; i < len; i++)
            {
                if(myparas[i]==' ' || myparas[i]=='\t')
                    myparas[i]='\0';
            }

            char* key;
            for(int i=0; i<argc; i++)
            {
                if(i+1 != argc) // check that we haven't finished parsing yet
                {
                    key = argv[i];

                    qDebug()<<">>key ..."<<key;

                    if (*key == '#')
                    {
                        while(*++key)
                        {
                            if (!strcmp(key, "s"))
                            {
                                b_saveimage = (atoi( argv[i+1] ))?true:false;
                                i++;
                            }
                            else
                            {
                                cout<<"parsing ..."<<key<<" "<<i<<" "<<"Unknown command. Type 'visiocyte -x plugin_name -f function_name' for usage"<<endl;
                                return false;
                            }

                        }
                    }
                    else
                    {
                        cout<<"parsing ..."<<key<<" "<<i<<" "<<"Unknown command. Type 'visiocyte -x plugin_name -f function_name' for usage"<<endl;
                        return false;
                    }

                }
            }
        }

        QString blendImageName;

        // get stitch configuration
        QDir myDir(infile);
        QStringList list = myDir.entryList(QStringList("*.tc"));

        if(list.size()!=1)
        {
            printf("Must have only one stitching configuration file!\n");
            return false;
        }

        if(!outfile)
            blendImageName = QString(infile).append("/stitched.visiocyteraw");
        else
            blendImageName = QString(outfile);

        if(QFileInfo(blendImageName).suffix().toUpper() != "VISIOCYTERAW")
        {
            blendImageName.append(".visiocyteraw"); // force to save as .visiocyteraw file
        }

        // load config
        Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim;

        QString tcfile = QString(infile).append("/").append(list.at(0));

        if( !vim.y_load(tcfile.toStdString()) )
        {
            printf("Wrong stitching configuration file to be load!\n");
            return false;
        }

        // do blending
        VISIOCYTELONG vx, vy, vz, vc;

        vx = vim.sz[0]; //
        vy = vim.sz[1];
        vz = vim.sz[2];
        vc = vim.sz[3];

        VISIOCYTELONG pagesz_vim = vx*vy*vz*vc;

        int datatype_tile = 0; // assume all tiles with the same datatype
        for(VISIOCYTELONG ii=0; ii<vim.number_tiles; ii++)
        {
            if(ii>0) break; // get datatype

            // load tile
            VISIOCYTELONG sz_relative[4];
            unsigned char* relative1d = 0;

            QString curPath = QFileInfo(tcfile).path();;

            string fn = curPath.append("/").append( QString(vim.lut[ii].fn_img.c_str()) ).toStdString();

            vim.lut[ii].fn_img = fn; // absolute path

            if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), relative1d, sz_relative, datatype_tile)!=true)
            {
                fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",vim.lut[ii].fn_img.c_str());
                return -1;
            }

            // de-alloca
            if(relative1d) {delete []relative1d; relative1d=NULL;}
        }
        if(datatype_tile == VISIOCYTE_UINT8)
        {
            // init
            unsigned char *pVImg = NULL;

            try
            {
                pVImg = new unsigned char [pagesz_vim];

                memset(pVImg, 0, sizeof(unsigned char)*pagesz_vim);
            }
            catch (...)
            {
                printf("Fail to allocate memory for pVImg.\n");
                if(pVImg) {delete []pVImg; pVImg=NULL;}
                return false;
            }

            //
            bool success = ireconstructing<unsigned char>((unsigned char *)pVImg, vim, 255,callback);
            if(!success)
            {
                printf("Fail to call function ireconstructing! \n");
                return false;
            }

            // output
            if(b_saveimage)
            {
                //save
                if (simple_saveimage_wrapper(callback,blendImageName.toStdString().c_str(), (unsigned char *)pVImg, vim.sz, 1)!=true)
                {
                    fprintf(stderr, "Error happens in file writing. Exit. \n");
                    return false;
                }

                //de-alloc
                if(pVImg) {delete []pVImg; pVImg=NULL;}
            }
            else
            {
                VISIOCYTEPluginArgItem arg;

                arg.type = "data"; arg.p = (void *)(pVImg); output << arg;

                VISIOCYTELONG metaImg[5]; // xyzc datatype
                metaImg[0] = vim.sz[0];
                metaImg[1] = vim.sz[1];
                metaImg[2] = vim.sz[2];
                metaImg[3] = vim.sz[3];
                metaImg[4] = datatype_tile;

                arg.type = "metaImage"; arg.p = (void *)(metaImg); output << arg;
            }
        }
        else if(datatype_tile == VISIOCYTE_UINT16)
        {
            // init
            unsigned short *pVImg = NULL;

            try
            {
                pVImg = new unsigned short [pagesz_vim];

                memset(pVImg, 0, sizeof(unsigned short)*pagesz_vim);
            }
            catch (...)
            {
                printf("Fail to allocate memory for pVImg.\n");
                if(pVImg) {delete []pVImg; pVImg=NULL;}
                return false;
            }

            //
            bool success = ireconstructing<unsigned short>((unsigned short *)pVImg, vim, 4095,callback);
            if(!success)
            {
                printf("Fail to call function ireconstructing! \n");
                return false;
            }

            // output
            if(b_saveimage)
            {
                //save
                if (simple_saveimage_wrapper(callback,blendImageName.toStdString().c_str(), (unsigned char *)pVImg, vim.sz, 2)!=true)
                {
                    fprintf(stderr, "Error happens in file writing. Exit. \n");
                    return false;
                }

                //de-alloc
                if(pVImg) {delete []pVImg; pVImg=NULL;}
            }
            else
            {
                VISIOCYTEPluginArgItem arg;

                arg.type = "data"; arg.p = (void *)(pVImg); output << arg;

                VISIOCYTELONG metaImg[5]; // xyzc datatype
                metaImg[0] = vim.sz[0];
                metaImg[1] = vim.sz[1];
                metaImg[2] = vim.sz[2];
                metaImg[3] = vim.sz[3];
                metaImg[4] = datatype_tile;

                arg.type = "metaImage"; arg.p = (void *)(metaImg); output << arg;
            }

        }
        else if(datatype_tile == VISIOCYTE_FLOAT32)
        {
            // current not supported
        }
        else
        {
            printf("Currently this program only support UINT8, UINT16, and FLOAT32 datatype.\n");
            return false;
        }
    }
    else if (func_name == tr("inormalizer")) // normalizing and linear blending
    {
        // parsing parameters
        if(input.size()<1) return false; // no inputs

        vector<char*> * infilelist = (vector<char*> *)(input.at(0).p);
        vector<char*> * paralist;
        vector<char*> * outfilelist;
        if(infilelist->empty())
        {
            //print Help info
            printf("\nUsage: visiocyte -x ifusion.dylib -f inormalizer -i <folder> -o <output_image> -p \"#s <save_blending_result zero(false)/nonzero(true)>\"\n");

            return true;
        }

        char * infile = infilelist->at(0); // input images
        char * paras = NULL; // parameters
        char * outfile = NULL; // outputs

        if(output.size()>0) { outfilelist = (vector<char*> *)(output.at(0).p); outfile = outfilelist->at(0);}  // specify output
        if(input.size()>1) { paralist = (vector<char*> *)(input.at(1).p); paras =  paralist->at(0);} // parameters

        bool b_saveimage = true; // save the blended image by default

        if(paras)
        {
            int argc = 0;
            int len = strlen(paras);
            int posb[200];
            char * myparas = new char[len];
            strcpy(myparas, paras);
            for(int i = 0; i < len; i++)
            {
                if(i==0 && myparas[i] != ' ' && myparas[i] != '\t')
                {
                    posb[argc++] = i;
                }
                else if((myparas[i-1] == ' ' || myparas[i-1] == '\t') && (myparas[i] != ' ' && myparas[i] != '\t'))
                {
                    posb[argc++] = i;
                }
            }
            char ** argv = new char* [argc];
            for(int i = 0; i < argc; i++)
            {
                argv[i] = myparas + posb[i];
            }
            for(int i = 0; i < len; i++)
            {
                if(myparas[i]==' ' || myparas[i]=='\t')
                    myparas[i]='\0';
            }

            char* key;
            for(int i=0; i<argc; i++)
            {
                if(i+1 != argc) // check that we haven't finished parsing yet
                {
                    key = argv[i];

                    qDebug()<<">>key ..."<<key;

                    if (*key == '#')
                    {
                        while(*++key)
                        {
                            if (!strcmp(key, "s"))
                            {
                                b_saveimage = (atoi( argv[i+1] ))?true:false;
                                i++;
                            }
                            else
                            {
                                cout<<"parsing ..."<<key<<" "<<i<<" "<<"Unknown command. Type 'visiocyte -x plugin_name -f function_name' for usage"<<endl;
                                return false;
                            }

                        }
                    }
                    else
                    {
                        cout<<"parsing ..."<<key<<" "<<i<<" "<<"Unknown command. Type 'visiocyte -x plugin_name -f function_name' for usage"<<endl;
                        return false;
                    }

                }
            }
        }

        QString blendImageName;

        // get stitch configuration
        QDir myDir(infile);
        QStringList list = myDir.entryList(QStringList("*.tc"));

        if(list.size()!=1)
        {
            printf("Must have only one stitching configuration file!\n");
            return false;
        }

        if(!outfile)
            blendImageName = QString(infile).append("/normalized_stitched.visiocyteraw");
        else
            blendImageName = QString(outfile);

        if(QFileInfo(blendImageName).suffix().toUpper() != "VISIOCYTERAW")
        {
            blendImageName.append(".visiocyteraw"); // force to save as .visiocyteraw file
        }

        // load config
        Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim;

        QString tcfile = QString(infile).append("/").append(list.at(0));

        if( !vim.y_load(tcfile.toStdString()) )
        {
            printf("Wrong stitching configuration file to be load!\n");
            return false;
        }

        // do blending
        VISIOCYTELONG vx, vy, vz, vc;

        vx = vim.sz[0];
        vy = vim.sz[1];
        vz = vim.sz[2];
        vc = vim.sz[3];

        VISIOCYTELONG pagesz_vim = vx*vy*vz*vc;

        int datatype_tile = 0; // assume all tiles with the same datatype
        for(VISIOCYTELONG ii=0; ii<vim.number_tiles; ii++)
        {
            if(ii>0) break; // get datatype

            // load tile
            VISIOCYTELONG sz_relative[4];
            unsigned char* relative1d = 0;

            QString curPath = QFileInfo(tcfile).path();

            string fn = curPath.append("/").append( QString(vim.lut[ii].fn_img.c_str()) ).toStdString();

            vim.lut[ii].fn_img = fn; // absolute path

            if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), relative1d, sz_relative, datatype_tile)!=true)
            {
                fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",vim.lut[ii].fn_img.c_str());
                return false;
            }

            // de-alloca
            if(relative1d) {delete []relative1d; relative1d=NULL;}
        }

        // the first tile chosen to be a reference image, all other tile will be normalized to this reference image
        QList<VISIOCYTELONG> normList, tileList;
        QList<AdjustPara> adjparaList;

        //normList<<0; // choose the first tile as the reference to normalize
        // find the highest mean value of overlap region as the reference tile
        VISIOCYTELONG reftile=0;
        QList<VISIOCYTELONG> aList, bList;
        aList<<0;
        for(VISIOCYTELONG ii=1; ii<vim.number_tiles; ii++)
        {
            bList.append(ii);
        }
        while (!bList.isEmpty())
        {
            // pick one from tileList
            VISIOCYTELONG j=bList.at(0);
            bList.pop_front();

            VISIOCYTELONG i;

            VISIOCYTELONG jx_s, jy_s, jz_s, jx_e, jy_e, jz_e;
            VISIOCYTELONG ix_s, iy_s, iz_s, ix_e, iy_e, iz_e;

            jx_s = vim.lut[j].start_pos[0] + vim.min_vim[0];
            jy_s = vim.lut[j].start_pos[1] + vim.min_vim[1];
            jz_s = vim.lut[j].start_pos[2] + vim.min_vim[2];

            jx_e = vim.lut[j].end_pos[0] + vim.min_vim[0];
            jy_e = vim.lut[j].end_pos[1] + vim.min_vim[1];
            jz_e = vim.lut[j].end_pos[2] + vim.min_vim[2];

            bool findoverlap = false;
            for (VISIOCYTELONG ii=0; ii<aList.size(); ii++)
            {
                // pick one from normList
                i=aList.at(ii);

                ix_s = vim.lut[i].start_pos[0] + vim.min_vim[0];
                iy_s = vim.lut[i].start_pos[1] + vim.min_vim[1];
                iz_s = vim.lut[i].start_pos[2] + vim.min_vim[2];

                ix_e = vim.lut[i].end_pos[0] + vim.min_vim[0];
                iy_e = vim.lut[i].end_pos[1] + vim.min_vim[1];
                iz_e = vim.lut[i].end_pos[2] + vim.min_vim[2];

                // find whether they have common overlap region
                if(  ( (ix_s>=jx_s && ix_s<=jx_e) || (ix_e>=jx_s && ix_e<=jx_e) || (jx_s>=ix_s && jx_s<=ix_e) || (jx_e>=ix_s && jx_e<=ix_e) )
                     && ( (iy_s>=jy_s && iy_s<=jy_e) || (iy_e>=jy_s && iy_e<=jy_e) || (jy_s>=iy_s && jy_s<=iy_e) || (jy_e>=iy_s && jy_e<=iy_e) )
                     && ( (iz_s>=jz_s && iz_s<=jz_e) || (iz_e>=jz_s && iz_e<=jz_e) || (jz_s>=iz_s && jz_s<=iz_e) || (jz_e>=iz_s && jz_e<=iz_e) ) )
                {
                    findoverlap = true;
                    break;
                }
            }

            // find a and b
            if(findoverlap)
            {
                // load image f
                VISIOCYTELONG sz_f[4];
                int datatype_f = 0;
                unsigned char* f1d = 0;

                string fn;

                // j <-> f
                if(j==0)
                {
                    fn = vim.lut[j].fn_img;
                }
                else
                {
                    QString curPath = QFileInfo(QString(vim.lut[0].fn_img.c_str())).path();;

                    fn = curPath.append("/").append( QString(vim.lut[j].fn_img.c_str()) ).toStdString();
                }

                //
                if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), f1d, sz_f, datatype_f)!=true)
                {
                    fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",fn.c_str());
                    return false;
                }
                VISIOCYTELONG fx=sz_f[0], fy=sz_f[1], fz=sz_f[2], fc=sz_f[3];

                // load image g
                VISIOCYTELONG sz_g[4];
                int datatype_g = 0;
                unsigned char* g1d = 0;

                // i <-> g
                if(i==0)
                {
                    fn = vim.lut[i].fn_img;
                }
                else
                {
                    QString curPath = QFileInfo(QString(vim.lut[0].fn_img.c_str())).path();;

                    fn = curPath.append("/").append( QString(vim.lut[i].fn_img.c_str()) ).toStdString();
                }

                //
                if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), g1d, sz_g, datatype_g)!=true)
                {
                    fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",fn.c_str());
                    return false;
                }
                VISIOCYTELONG gx=sz_g[0], gy=sz_g[1], gz=sz_g[2], gc=sz_g[3];

                // overlap image from f and g
                VISIOCYTELONG start_x = qMax(jx_s, ix_s);
                VISIOCYTELONG start_y = qMax(jy_s, iy_s);
                VISIOCYTELONG start_z = qMax(jz_s, iz_s);

                VISIOCYTELONG end_x = qMin(jx_e, ix_e);
                VISIOCYTELONG end_y = qMin(jy_e, iy_e);
                VISIOCYTELONG end_z = qMin(jz_e, iz_e);

                VISIOCYTELONG dimxol= end_x-start_x+1;
                VISIOCYTELONG dimyol= end_y-start_y+1;
                VISIOCYTELONG dimzol= end_z-start_z+1;

                VISIOCYTELONG pagesz_ol = dimxol*dimyol*dimzol;

                float *fol1d = NULL;
                float *gol1d = NULL;

                try
                {
                    // suppose fc = gc = vc
                    fol1d = new float [fc*pagesz_ol];
                    gol1d = new float [gc*pagesz_ol];
                }
                catch (...)
                {
                    printf("Fail to allocate memory for fol1d and gol1d.\n");

                    if (fol1d) {delete []fol1d; fol1d=NULL;}
                    if (gol1d) {delete []gol1d; gol1d=NULL;}

                    return false;
                }

                for(VISIOCYTELONG c=0; c<fc; c++)
                {
                    VISIOCYTELONG offsets_fc = c*fx*fy*fz;
                    VISIOCYTELONG offsets_gc = c*gx*gy*gz;

                    VISIOCYTELONG offsets_olc = c*pagesz_ol;

                    for(VISIOCYTELONG z=start_z; z<end_z; z++)
                    {
                        VISIOCYTELONG offsets_fz = offsets_fc + (z-jz_s)*fx*fy;
                        VISIOCYTELONG offsets_gz = offsets_gc + (z-iz_s)*gx*gy;

                        VISIOCYTELONG offsets_olz = offsets_olc + (z-start_z)*dimxol*dimyol;

                        for(VISIOCYTELONG y=start_y; y<end_y; y++)
                        {
                            VISIOCYTELONG offsets_fy = offsets_fz + (y-jy_s)*fx;
                            VISIOCYTELONG offsets_gy = offsets_gz + (y-iy_s)*gx;

                            VISIOCYTELONG offsets_oly = offsets_olz + (y-start_y)*dimxol;

                            for(VISIOCYTELONG x=start_x; x<end_x; x++)
                            {
                                VISIOCYTELONG idx = offsets_oly + x - start_x;

                                fol1d[idx] = f1d[offsets_fy + (x-jx_s)];
                                gol1d[idx] = g1d[offsets_gy + (x-ix_s)];
                            }
                        }
                    }
                }
                // de-alloc
                if (f1d) {delete []f1d; f1d=NULL;}
                if (g1d) {delete []g1d; g1d=NULL;}

                //
                for(VISIOCYTELONG c=0; c<fc; c++)
                {
                    VISIOCYTELONG offsets_olc = c*pagesz_ol;

                    float a,b;
                    VISIOCYTELONG tilen;

                    if(findHighestIntensityLevelTile<float>(fol1d+offsets_olc, gol1d+offsets_olc, pagesz_ol, tilen)!=true)
                    {
                        printf("Fail to call function computeAdjustPara! \n");
                        return false;
                    }

                    qDebug()<<"color ..."<<c;
                }

                // de-alloc
                if (fol1d) {delete []fol1d; fol1d=NULL;}
                if (gol1d) {delete []gol1d; gol1d=NULL;}

                //
                aList.push_back(j);
            }
            else
            {
                bList.push_back(j);
            }
        }

        normList.append(reftile);

        for(VISIOCYTELONG ii=1; ii<vim.number_tiles; ii++)
        {
            tileList.append(ii);

            AdjustPara adjPara;

            adjPara.init(vc);
            adjPara.num_tile=ii;

            adjparaList.push_back(adjPara);
        }

        // consider linear model f=a*g+b find a and b
        while (!tileList.isEmpty())
        {
            // pick one from tileList
            VISIOCYTELONG j=tileList.at(0);
            tileList.pop_front();

            VISIOCYTELONG i;

            VISIOCYTELONG jx_s, jy_s, jz_s, jx_e, jy_e, jz_e;
            VISIOCYTELONG ix_s, iy_s, iz_s, ix_e, iy_e, iz_e;

            jx_s = vim.lut[j].start_pos[0] + vim.min_vim[0];
            jy_s = vim.lut[j].start_pos[1] + vim.min_vim[1];
            jz_s = vim.lut[j].start_pos[2] + vim.min_vim[2];

            jx_e = vim.lut[j].end_pos[0] + vim.min_vim[0];
            jy_e = vim.lut[j].end_pos[1] + vim.min_vim[1];
            jz_e = vim.lut[j].end_pos[2] + vim.min_vim[2];

            bool findoverlap = false;
            for (VISIOCYTELONG ii=0; ii<normList.size(); ii++)
            {
                // pick one from normList
                i=normList.at(ii);

                ix_s = vim.lut[i].start_pos[0] + vim.min_vim[0];
                iy_s = vim.lut[i].start_pos[1] + vim.min_vim[1];
                iz_s = vim.lut[i].start_pos[2] + vim.min_vim[2];

                ix_e = vim.lut[i].end_pos[0] + vim.min_vim[0];
                iy_e = vim.lut[i].end_pos[1] + vim.min_vim[1];
                iz_e = vim.lut[i].end_pos[2] + vim.min_vim[2];

                // find whether they have common overlap region
                if(  ( (ix_s>=jx_s && ix_s<=jx_e) || (ix_e>=jx_s && ix_e<=jx_e) || (jx_s>=ix_s && jx_s<=ix_e) || (jx_e>=ix_s && jx_e<=ix_e) )
                     && ( (iy_s>=jy_s && iy_s<=jy_e) || (iy_e>=jy_s && iy_e<=jy_e) || (jy_s>=iy_s && jy_s<=iy_e) || (jy_e>=iy_s && jy_e<=iy_e) )
                     && ( (iz_s>=jz_s && iz_s<=jz_e) || (iz_e>=jz_s && iz_e<=jz_e) || (jz_s>=iz_s && jz_s<=iz_e) || (jz_e>=iz_s && jz_e<=iz_e) ) )
                {
                    findoverlap = true;
                    break;
                }
            }

            // find a and b
            if(findoverlap)
            {
                // load image f
                VISIOCYTELONG sz_f[4];
                int datatype_f = 0;
                unsigned char* f1d = 0;

                string fn;

                // j <-> f
                if(j==0)
                {
                    fn = vim.lut[j].fn_img;
                }
                else
                {
                    QString curPath = QFileInfo(QString(vim.lut[0].fn_img.c_str())).path();;

                    fn = curPath.append("/").append( QString(vim.lut[j].fn_img.c_str()) ).toStdString();
                }

                //
                if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), f1d, sz_f, datatype_f)!=true)
                {
                    fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",fn.c_str());
                    return false;
                }
                VISIOCYTELONG fx=sz_f[0], fy=sz_f[1], fz=sz_f[2], fc=sz_f[3];

                // load image g
                VISIOCYTELONG sz_g[4];
                int datatype_g = 0;
                unsigned char* g1d = 0;

                // i <-> g
                if(i==0)
                {
                    fn = vim.lut[i].fn_img;
                }
                else
                {
                    QString curPath = QFileInfo(QString(vim.lut[0].fn_img.c_str())).path();;

                    fn = curPath.append("/").append( QString(vim.lut[i].fn_img.c_str()) ).toStdString();
                }

                //
                if (simple_loadimage_wrapper(callback,const_cast<char *>(fn.c_str()), g1d, sz_g, datatype_g)!=true)
                {
                    fprintf (stderr, "Error happens in reading the subject file [%s]. Exit. \n",fn.c_str());
                    return false;
                }
                VISIOCYTELONG gx=sz_g[0], gy=sz_g[1], gz=sz_g[2], gc=sz_g[3];

                // overlap image from f and g
                VISIOCYTELONG start_x = qMax(jx_s, ix_s);
                VISIOCYTELONG start_y = qMax(jy_s, iy_s);
                VISIOCYTELONG start_z = qMax(jz_s, iz_s);

                VISIOCYTELONG end_x = qMin(jx_e, ix_e);
                VISIOCYTELONG end_y = qMin(jy_e, iy_e);
                VISIOCYTELONG end_z = qMin(jz_e, iz_e);

                VISIOCYTELONG dimxol= end_x-start_x+1;
                VISIOCYTELONG dimyol= end_y-start_y+1;
                VISIOCYTELONG dimzol= end_z-start_z+1;

                VISIOCYTELONG pagesz_ol = dimxol*dimyol*dimzol;

                float *fol1d = NULL;
                float *gol1d = NULL;

                try
                {
                    // suppose fc = gc = vc
                    fol1d = new float [fc*pagesz_ol];
                    gol1d = new float [gc*pagesz_ol];
                }
                catch (...)
                {
                    printf("Fail to allocate memory for fol1d and gol1d.\n");

                    if (fol1d) {delete []fol1d; fol1d=NULL;}
                    if (gol1d) {delete []gol1d; gol1d=NULL;}

                    return false;
                }

                for(VISIOCYTELONG c=0; c<fc; c++)
                {
                    VISIOCYTELONG offsets_fc = c*fx*fy*fz;
                    VISIOCYTELONG offsets_gc = c*gx*gy*gz;

                    VISIOCYTELONG offsets_olc = c*pagesz_ol;

                    for(VISIOCYTELONG z=start_z; z<end_z; z++)
                    {
                        VISIOCYTELONG offsets_fz = offsets_fc + (z-jz_s)*fx*fy;
                        VISIOCYTELONG offsets_gz = offsets_gc + (z-iz_s)*gx*gy;

                        VISIOCYTELONG offsets_olz = offsets_olc + (z-start_z)*dimxol*dimyol;

                        for(VISIOCYTELONG y=start_y; y<end_y; y++)
                        {
                            VISIOCYTELONG offsets_fy = offsets_fz + (y-jy_s)*fx;
                            VISIOCYTELONG offsets_gy = offsets_gz + (y-iy_s)*gx;

                            VISIOCYTELONG offsets_oly = offsets_olz + (y-start_y)*dimxol;

                            for(VISIOCYTELONG x=start_x; x<end_x; x++)
                            {
                                VISIOCYTELONG idx = offsets_oly + x - start_x;

                                fol1d[idx] = f1d[offsets_fy + (x-jx_s)];
                                gol1d[idx] = g1d[offsets_gy + (x-ix_s)];
                            }
                        }
                    }
                }
                // de-alloc
                if (f1d) {delete []f1d; f1d=NULL;}
                if (g1d) {delete []g1d; g1d=NULL;}

                //
                VISIOCYTELONG ii;
                for (ii=0; ii<adjparaList.size(); ii++)
                {
                    if(j==adjparaList.at(ii).num_tile) break;
                }

                for(VISIOCYTELONG c=0; c<fc; c++)
                {
                    VISIOCYTELONG offsets_olc = c*pagesz_ol;

                    float a,b;

                    if(computeAdjustPara<float>(fol1d+offsets_olc, gol1d+offsets_olc, pagesz_ol, a, b)!=true)
                    {
                        printf("Fail to call function computeAdjustPara! \n");
                        return false;
                    }

                    adjparaList.at(ii).a[c] = a;
                    adjparaList.at(ii).b[c] = b;
                }

                // de-alloc
                if (fol1d) {delete []fol1d; fol1d=NULL;}
                if (gol1d) {delete []gol1d; gol1d=NULL;}

                //
                normList.push_back(j);
            }
            else
            {
                tileList.push_back(j);
            }
        }
        // de-alloc
        //        while (!adjparaList.isEmpty())
        //        {
        //            adjparaList.at(0).clean();
        //            adjparaList.pop_front();
        //        }

        if(datatype_tile == VISIOCYTE_UINT8)
        {
            // init
            unsigned char *pVImg = NULL;

            try
            {
                pVImg = new unsigned char [pagesz_vim];

                memset(pVImg, 0, sizeof(unsigned char)*pagesz_vim);
            }
            catch (...)
            {
                printf("Fail to allocate memory for pVImg.\n");

                if(pVImg) {delete []pVImg; pVImg=NULL;}

                return false;
            }

            //
            if(ireconstructingwnorm<unsigned char>((unsigned char *)pVImg, vim, 255, adjparaList,callback)!=true)
            {
                printf("Fail to call function ireconstructing! \n");
                return false;
            }

            // output
            if(b_saveimage)
            {
                //save
                if (simple_saveimage_wrapper(callback,blendImageName.toStdString().c_str(), (unsigned char *)pVImg, vim.sz, 1)!=true)
                {
                    fprintf(stderr, "Error happens in file writing. Exit. \n");
                    return false;
                }

                //de-alloc
                if(pVImg) {delete []pVImg; pVImg=NULL;}
            }
            else
            {
                VISIOCYTEPluginArgItem arg;

                arg.type = "data"; arg.p = (void *)(pVImg); output << arg;

                VISIOCYTELONG metaImg[5]; // xyzc datatype
                metaImg[0] = vim.sz[0];
                metaImg[1] = vim.sz[1];
                metaImg[2] = vim.sz[2];
                metaImg[3] = vim.sz[3];
                metaImg[4] = datatype_tile;

                arg.type = "metaImage"; arg.p = (void *)(metaImg); output << arg;
            }
        }
        else if(datatype_tile == VISIOCYTE_UINT16)
        {
            // init
            unsigned short *pVImg = NULL;

            try
            {
                pVImg = new unsigned short [pagesz_vim];

                memset(pVImg, 0, sizeof(unsigned short)*pagesz_vim);
            }
            catch (...)
            {
                printf("Fail to allocate memory for pVImg.\n");

                if(pVImg) {delete []pVImg; pVImg=NULL;}

                return false;
            }

            //
            if(ireconstructingwnorm<unsigned short>((unsigned short *)pVImg, vim, 4095, adjparaList,callback)!=true)
            {
                printf("Fail to call function ireconstructing! \n");
                return false;
            }

            // output
            if(b_saveimage)
            {
                //save
                if (simple_saveimage_wrapper(callback,blendImageName.toStdString().c_str(), (unsigned char *)pVImg, vim.sz, 2)!=true)
                {
                    fprintf(stderr, "Error happens in file writing. Exit. \n");
                    return false;
                }

                //de-alloc
                if(pVImg) {delete []pVImg; pVImg=NULL;}
            }
            else
            {
                VISIOCYTEPluginArgItem arg;

                arg.type = "data"; arg.p = (void *)(pVImg); output << arg;

                VISIOCYTELONG metaImg[5]; // xyzc datatype
                metaImg[0] = vim.sz[0];
                metaImg[1] = vim.sz[1];
                metaImg[2] = vim.sz[2];
                metaImg[3] = vim.sz[3];
                metaImg[4] = datatype_tile;

                arg.type = "metaImage"; arg.p = (void *)(metaImg); output << arg;
            }

        }
        else if(datatype_tile == VISIOCYTE_FLOAT32)
        {
            // current not supported
        }
        else
        {
            printf("Currently this program only support UINT8, UINT16, and FLOAT32 datatype.\n");
            return false;
        }

    }
    else
    {
        printf("\nWrong function specified.\n");
        return false;
    }

    return true;
}

// menu
QStringList ImageFusionPlugin::menulist() const
{
    return QStringList() << tr("Image Fusion")
                         << tr("About");
}

void ImageFusionPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("Image Fusion"))
    {
        ImageFusionDialog dialog(callback, parent, NULL);
        if (dialog.exec()!=QDialog::Accepted)
            return;

        QString m_InputFolder = dialog.foldername;

        if ( !QFile::exists(m_InputFolder) )
        {
            cout<<"Folder does not exist!"<<endl;
            return;
        }

        // call fusion dofunc
        VISIOCYTEPluginArgItem arg;
        VISIOCYTEPluginArgList pluginfunc_input;
        VISIOCYTEPluginArgList pluginfunc_output;

        vector<char*> fileList;
        vector<char*> paraList;

        fileList.clear();
        paraList.clear();

        QByteArray bytes = m_InputFolder.toAscii();

        fileList.push_back(bytes.data());

        paraList.push_back("#s 0");

        arg.type = ""; arg.p = (void *)(&fileList); pluginfunc_input << arg;
        arg.type = ""; arg.p = (void *)(&paraList); pluginfunc_input << arg;

        bool success = dofunc("iblender", pluginfunc_input, pluginfunc_output, callback, parent);

        if(!success)
        {
            QMessageBox::information(parent, "Warning: Image Fusion", QString("Fail to run image fusion program."));
            return;
        }

        // output
        VISIOCYTELONG *metaImg = (VISIOCYTELONG *)(pluginfunc_output.at(1).p);

        VISIOCYTELONG sx = metaImg[0];
        VISIOCYTELONG sy = metaImg[1];
        VISIOCYTELONG sz = metaImg[2];

        VISIOCYTELONG colordim = metaImg[3];
        VISIOCYTELONG datatype = metaImg[4];

        // show result in visiocyte
        if(datatype == VISIOCYTE_UINT8)
        {
            //
            unsigned char* data1d = (unsigned char *)(pluginfunc_output.at(0).p);

            //display
            Image4DSimple p4DImage;
            p4DImage.setData((unsigned char*)data1d, sx, sy, sz, colordim, VISIOCYTE_UINT8); //

            visiocytehandle newwin = callback.newImageWindow();
            callback.setImage(newwin, &p4DImage);
            callback.setImageName(newwin, "Stitched Image");
            callback.updateImageWindow(newwin);

        }
        else if(datatype == VISIOCYTE_UINT16)
        {
            //
            unsigned short* data1d = (unsigned short *)(pluginfunc_output.at(0).p);

            //display
            Image4DSimple p4DImage;
            p4DImage.setData((unsigned char*)data1d, sx, sy, sz, colordim, VISIOCYTE_UINT16); //

            visiocytehandle newwin = callback.newImageWindow();
            callback.setImage(newwin, &p4DImage);
            callback.setImageName(newwin, "Stitched Image");
            callback.updateImageWindow(newwin);
        }
        else if(datatype == VISIOCYTE_FLOAT32)
        {
            // current not support
        }
        else
        {
            printf("Currently this program only support UINT8, UINT16, and FLOAT32 datatype.\n");
            return;
        }

    }
    else if (menu_name == tr("About"))
    {
        QMessageBox::information(parent, "Version info", QString("Image fusion. Version %1 (August 31, 2011) developed by Yang Yu and Sean Murphy. (Janelia Research Farm Campus, HHMI)").arg(getPluginVersion()));
        return;
    }
}

