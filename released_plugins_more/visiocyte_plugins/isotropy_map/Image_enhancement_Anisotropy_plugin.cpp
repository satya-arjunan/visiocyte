/* Image_enhancement_Anisotropy_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2013-01-28 : by Zhi
 */

#include "visiocyte_message.h"
#include <vector>
#include "Image_enhancement_Anisotropy_plugin.h"
#include "../../../visiocyte_main/jba/newmat11/newmatap.h"
#include "../../../visiocyte_main/jba/newmat11/newmatio.h"


using namespace std;
Q_EXPORT_PLUGIN2(image_anisotropy_map, anisotropy_enhancement);

void processImage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int flag);


template <class T> bool compute_Anisotropy_sphere(const T* data1d, VISIOCYTELONG N, VISIOCYTELONG M, VISIOCYTELONG P, VISIOCYTELONG c,
                                                  VISIOCYTELONG x0, VISIOCYTELONG y0, VISIOCYTELONG z0,
                                                  VISIOCYTELONG rs, double &Score, double & avevalue)
{
    if (!data1d || N<=0 || M<=0 || P<=0 || c<0 || x0<0 || x0>N-1 || y0<0 || y0>M-1 || z0<0 || z0>P-1 || rs<1)
        return false;

        //get the boundary
        VISIOCYTELONG offsetc = (c)*(N*M*P);

    VISIOCYTELONG xb = x0-rs;if(xb<0) xb = 0;
    VISIOCYTELONG xe = x0+rs; if(xe>=N-1) xe = N-1;
    VISIOCYTELONG yb = y0-rs;if(yb<0) yb = 0;
    VISIOCYTELONG ye = y0+rs; if(ye>=M-1) ye = M-1;
    VISIOCYTELONG zb = z0-rs;if(zb<0) zb = 0;
    VISIOCYTELONG ze = z0+rs; if(ze>=P-1) ze = P-1;

    VISIOCYTELONG i,j,k;
    double w;

    //first get the average value
    double x2, y2, z2;
    double rx2 = double(rs+1)*(rs+1);
    double ry2 = rx2, rz2 = rx2;
    double tmpd;
    double xm=0,ym=0,zm=0, s=0, mv=0, n=0;

    s = 0; n = 0;
    for(k=zb; k<=ze; k++)
    {
        VISIOCYTELONG offsetkl = k*M*N;
        z2 = k-z0; z2*=z2;
        for(j=yb; j<=ye; j++)
        {
            VISIOCYTELONG offsetjl = j*N;
            y2 = j-y0; y2*=y2;
            tmpd = y2/ry2 + z2/rz2;
            if (tmpd>1.0)
                continue;

            for(i=xb; i<=xe; i++)
            {
                x2 = i-x0; x2*=x2;
                if (x2/rx2 + tmpd > 1.0)
                    continue;

                s += double(data1d[offsetc+offsetkl + offsetjl + i]);
                n = n+1;
            }
        }
    }
    if (n!=0)
        mv = s/n;
    else
        mv = 0;

    //now get the center of mass
    s = 0; n=0;
    for(k=zb; k<=ze; k++)
    {
        VISIOCYTELONG offsetkl = k*M*N;
        z2 = k-z0; z2*=z2;
        for(j=yb; j<=ye; j++)
        {
            VISIOCYTELONG offsetjl = j*N;
            y2 = j-y0; y2*=y2;
            tmpd = y2/ry2 + z2/rz2;
            if (tmpd>1.0)
                continue;

            for(i=xb; i<=xe; i++)
            {
                x2 = i-x0; x2*=x2;
                if (x2/rx2 + tmpd > 1.0)
                    continue;

                w = double(data1d[offsetc+offsetkl + offsetjl + i]) - mv;
                if (w>0)
                {
                xm += w*i;
                ym += w*j;
                zm += w*k;
                s += w;
                n = n+1;
                }
            }
        }
    }

    if(n>0)
    {
        xm /= s; ym /=s; zm /=s;
    }
    else
    {
        xm = x0; ym=y0; zm=z0;
    }

    double spatial_deviation = sqrt(double(xm-x0)*(xm-x0) + double(ym-y0)*(ym-y0) + double(zm-z0)*(zm-z0)) + 1;

    avevalue = mv;

    double cc11=0, cc12=0, cc13=0, cc22=0, cc23=0, cc33=0;
    double dfx, dfy, dfz;
    for(k=zb; k<=ze; k++)
    {
        z2 = k-z0; z2*=z2;
        dfz = double(k)-zm;
        VISIOCYTELONG offsetkl = k*M*N;
        for(j=yb; j<=ye; j++)
        {
            y2 = j-y0; y2*=y2;
            tmpd = y2/ry2 + z2/rz2;
            if (tmpd>1.0)
                continue;

            dfy = double(j)-ym;
            VISIOCYTELONG offsetjl = j*N;
            for(i=xb; i<=xe; i++)
            {
                x2 = i-x0; x2*=x2;
                if (x2/rx2 + tmpd > 1.0)
                    continue;

                dfx = double(i)-xm;
                w = double(data1d[offsetc+offsetkl + offsetjl + i]) - mv;
                if (w>0)
                {
                cc11 += w*dfx*dfx;
                cc12 += w*dfx*dfy;
                cc13 += w*dfx*dfz;
                cc22 += w*dfy*dfy;
                cc23 += w*dfy*dfz;
                cc33 += w*dfz*dfz;
                }

            }
        }
    }
    cc11 /= s; 	cc12 /= s; 	cc13 /= s; 	cc22 /= s; 	cc23 /= s; 	cc33 /= s;

    Score = 1; //the Score should at least be 1.
    try
    {
        //then find the eigen vector
        SymmetricMatrix Cov_Matrix(3);
        Cov_Matrix.Row(1) << cc11;
        Cov_Matrix.Row(2) << cc12 << cc22;
        Cov_Matrix.Row(3) << cc13 << cc23 << cc33;

        DiagonalMatrix DD;
        Matrix VV;
        EigenValues(Cov_Matrix,DD,VV);

        //output the result
        double pc1 = DD(3);
        double pc2 = DD(2);
        // double pc3 = DD(1);
        Score = pc1/pc2/spatial_deviation;
        //Score = sqrt(pc1)/sqrt(pc2)/spatial_deviation;
    }
    catch (...)
    {
        Score = 0;
    }

    return true;

}


QStringList anisotropy_enhancement::menulist() const
{
    return QStringList()
            <<tr("anisotropy map of a fixed window")
           <<tr("anisotropy map of using adaptive windows")
          <<tr("about");
}

QStringList anisotropy_enhancement::funclist() const
{
    return QStringList()
            <<tr("func1")
           <<tr("func2")
          <<tr("help");
}

void anisotropy_enhancement::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("anisotropy map of a fixed window"))
    {
        processImage(callback,parent,0);
    }
    else if (menu_name == tr("anisotropy map of using adaptive windows"))
    {
        processImage(callback,parent,1);
    }
    else
    {
        visiocyte_msg(tr("This plugin calculates the PCA-based anisotropy map of an image. by Hanchuan Peng & Zhi Zhou."));
    }
}

bool anisotropy_enhancement::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
    vector<char*> infiles, inparas, outfiles;
    if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
    if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
    if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    if (func_name == tr("func1"))
    {
        visiocyte_msg("To be implemented.");
    }
    else if (func_name == tr("func2"))
    {
        visiocyte_msg("To be implemented.");
    }
    else if (func_name == tr("help"))
    {
        visiocyte_msg("To be implemented.");
    }
    else return false;

    return true;
}

void processImage(VISIOCYTEPluginCallback2 &callback, QWidget *parent, int flag)
{
    visiocytehandle curwin = callback.currentImageWindow();
    if (!curwin)
    {
        QMessageBox::information(0, "", "You don't have any image open in the main window.");
        return;
    }

    Image4DSimple* p4DImage = callback.getImage(curwin);

    if (!p4DImage)
    {
        QMessageBox::information(0, "", "The image pointer is invalid. Ensure your data is valid and try again!");
        return;
    }

    unsigned char* data1d = p4DImage->getRawData();
    VISIOCYTELONG pagesz = p4DImage->getTotalUnitNumberPerChannel();

    VISIOCYTELONG N = p4DImage->getXDim();
    VISIOCYTELONG M = p4DImage->getYDim();
    VISIOCYTELONG P = p4DImage->getZDim();
    VISIOCYTELONG sc = p4DImage->getCDim();

    ImagePixelType pixeltype = p4DImage->getDatatype();

    //input
    bool ok1,ok2;
    int c = 1;
    double rs = 0;

    if(flag ==0)
        rs = QInputDialog::getDouble(parent, "Radius of the window for computing anisotropy map",
                                     "Window radius:",
                                     1, 1, 30, 1, &ok1);
    else
        ok1=true;
    if(ok1)
    {
        if(sc==1)
        {
            c=1;
            ok2=true;
        }
        else
        {
            c = QInputDialog::getInteger(parent, "Channel",
                                         "Enter channel NO:",
                                         1, 1, sc, 1, &ok2);
        }
    }

    if(!ok2)
        return;
    c = c-1;

    VISIOCYTELONG offsetc = (c)*pagesz;
    float* datald_output = 0;
    datald_output = new float[pagesz];
    for(VISIOCYTELONG i = 0; i<pagesz;i++)
        datald_output[i] = 0;

    double score_max = 0, ave_last=0;
    double score_each = 0, ave_v=0;

    for(VISIOCYTELONG iz = 0; iz < P; iz++)
    {
        printf("\r Evaluation : %d %% completed ", int(float(iz)/P*100)); fflush(stdout);
        VISIOCYTELONG offsetk = iz*M*N;
        for(VISIOCYTELONG iy = 0; iy < M; iy++)
        {
            VISIOCYTELONG offsetj = iy*N;
            for(VISIOCYTELONG ix = 0; ix < N; ix++)
            {
                VISIOCYTELONG PixelValue = data1d[offsetc+offsetk + offsetj + ix];
                if(flag == 0) //for fixed window
                {
                    if (rs > 0 && PixelValue > 0)
                    {
                        compute_Anisotropy_sphere(data1d, N, M, P, c, ix, iy, iz, rs, score_each, ave_v);
                        if(score_each>0)
                        {
                            datald_output[offsetk + offsetj + ix] = score_each;
                        }
                    }
                }
                else
                {
                    if(PixelValue > 0)
                    {
                        score_max = 0;
                        ave_last = 0;
                        for(rs = 2; rs < 31; rs++)
                        {
                            compute_Anisotropy_sphere(data1d, N, M, P, c, ix, iy, iz, rs, score_each, ave_v);
                            if(rs==2) ave_last = ave_v;

                            //if(score_each > score_max && ave_v < ave_last*1.1 && ave_v>ave_last*0.9)
                            if (score_each > score_max)
                            {
                                score_max = score_each;
                                ave_last = ave_v;
                            }
                            else
                            {
                                break;
                            }
                        }
                        datald_output[offsetk + offsetj + ix] = score_max;
                    }

                }
            }
        }
    }


    // display
    Image4DSimple * new4DImage = new Image4DSimple();
    new4DImage->setData((unsigned char *)datald_output,N, M, P, 1, VISIOCYTE_FLOAT32);
    visiocytehandle newwin = callback.newImageWindow();
    callback.setImage(newwin, new4DImage);
    callback.setImageName(newwin, "Local_adaptive_enhancement_result");
    callback.updateImageWindow(newwin);
    return;
}
