/* mipImageSeries_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2014-06-26 : by Zhi Zhou
 */
 
#include "visiocyte_message.h"
#include <vector>
#include "mipImageSeries_plugin.h"
#include "../../../released_plugins/visiocyte_plugins/istitch/y_imglib.h"

using namespace std;

// Open a series of inputs
QStringList importSeriesFileList_addnumbersort(const QString & curFilePath)
{
    QStringList myList;
    myList.clear();

    // get the image files namelist in the directory
    QStringList imgSuffix;
    imgSuffix<<"*.tif"<<"*.raw"<<"*.visiocyteraw"<<"*.lsm"
            <<"*.TIF"<<"*.RAW"<<"*.VISIOCYTERAW"<<"*.LSM";

    QDir dir(curFilePath);
    if (!dir.exists())
    {
        qWarning("Cannot find the directory");
        return myList;
    }

    foreach (QString file, dir.entryList(imgSuffix, QDir::Files, QDir::Name))
    {
        myList += QFileInfo(dir, file).absoluteFilePath();
    }

    // print filenames
    foreach (QString qs, myList)  qDebug() << qs;

    return myList;
}

Q_EXPORT_PLUGIN2(mipImageSeries, mipImageSeries);
 
QStringList mipImageSeries::menulist() const
{
	return QStringList() 
        <<tr("calculate the MIP of image series")
		<<tr("about");
}

QStringList mipImageSeries::funclist() const
{
	return QStringList()
        <<tr("mipImageSeries")
		<<tr("help");
}

void mipImageSeries::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("calculate the MIP of image series"))
	{
        QString m_InputfolderName = QFileDialog::getExistingDirectory(parent, QObject::tr("Choose the directory including all images "),
                                              QDir::currentPath(),
                                              QFileDialog::ShowDirsOnly);

        QStringList imgList = importSeriesFileList_addnumbersort(m_InputfolderName);

        Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim;

        VISIOCYTELONG count=0;
        foreach (QString img_str, imgList)
        {
            VISIOCYTELONG offset[3];
            offset[0]=0; offset[1]=0; offset[2]=0;

            indexed_t<VISIOCYTELONG, REAL> idx_t(offset);

            idx_t.n = count;
            idx_t.ref_n = 0; // init with default values
            idx_t.fn_image = img_str.toStdString();
            idx_t.score = 0;

            vim.tilesList.push_back(idx_t);
            count++;
        }

        int NTILES  = vim.tilesList.size();

        unsigned char * data1d_mip = 0;
        VISIOCYTELONG in_sz[4];
        int datatype;

        if (!simple_loadimage_wrapper(callback,const_cast<char *>(vim.tilesList.at(0).fn_image.c_str()), data1d_mip, in_sz, datatype))
        {
            fprintf (stderr, "Error happens in reading the subject file [%0]. Exit. \n",vim.tilesList.at(0).fn_image.c_str());
            return;
        }

        VISIOCYTELONG pagesz = in_sz[0]*in_sz[1]*in_sz[2]*in_sz[3];

        for(VISIOCYTELONG ii = 1; ii < NTILES; ii++)
        {
            unsigned char * data1d = 0;
            VISIOCYTELONG in_sz[4];
            int datatype;

            if (!simple_loadimage_wrapper(callback,const_cast<char *>(vim.tilesList.at(ii).fn_image.c_str()), data1d, in_sz, datatype))
            {
                fprintf (stderr, "Error happens in reading the subject file [%0]. Exit. \n",vim.tilesList.at(ii).fn_image.c_str());
                return;
            }
            for(VISIOCYTELONG j = 0; j < pagesz; j++)
            {
                if(data1d_mip[j] < data1d[j])
                    data1d_mip[j] = data1d[j];
            }
            if(data1d) {delete []data1d; data1d = 0;}
        }

        ImagePixelType pixeltype;
        switch (datatype)
        {
            case 1: pixeltype = VISIOCYTE_UINT8; break;
            case 2: pixeltype = VISIOCYTE_UINT16; break;
            case 4: pixeltype = VISIOCYTE_FLOAT32;break;
            default: visiocyte_msg("Invalid data type. Do nothing."); return;
        }

        Image4DSimple * new4DImage = new Image4DSimple();
        new4DImage->setData((unsigned char *)data1d_mip, in_sz[0], in_sz[1], in_sz[2], in_sz[3], pixeltype);
        visiocytehandle newwin = callback.newImageWindow();
        callback.setImage(newwin, new4DImage);
        callback.setImageName(newwin, "maximum intensity projection image");
        callback.updateImageWindow(newwin);
	}
	else
	{
        visiocyte_msg(tr("This is a plugin to caculate the mip from image series"
			"Developed by Zhi Zhou, 2014-06-26"));
	}
}

bool mipImageSeries::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    if (func_name == tr("mip_imageseries"))
	{
        cout<<"Welcome to mipImageSeries plugin"<<endl;
        if (output.size() != 1) return false;
        char * inimg_file = ((vector<char*> *)(input.at(0).p))->at(0);
        char * outimg_file = ((vector<char*> *)(output.at(0).p))->at(0);

        QString m_InputfolderName(inimg_file);

        QStringList imgList = importSeriesFileList_addnumbersort(m_InputfolderName);

        Y_VIM<REAL, VISIOCYTELONG, indexed_t<VISIOCYTELONG, REAL>, LUT<VISIOCYTELONG> > vim;

        VISIOCYTELONG count=0;
        foreach (QString img_str, imgList)
        {
            VISIOCYTELONG offset[3];
            offset[0]=0; offset[1]=0; offset[2]=0;

            indexed_t<VISIOCYTELONG, REAL> idx_t(offset);

            idx_t.n = count;
            idx_t.ref_n = 0; // init with default values
            idx_t.fn_image = img_str.toStdString();
            idx_t.score = 0;

            vim.tilesList.push_back(idx_t);
            count++;
        }

        int NTILES  = vim.tilesList.size();

        unsigned char * data1d_mip = 0;
        VISIOCYTELONG in_sz[4];
        int datatype;

        if (!simple_loadimage_wrapper(callback,const_cast<char *>(vim.tilesList.at(0).fn_image.c_str()), data1d_mip, in_sz, datatype))
        {
            fprintf (stderr, "Error happens in reading the subject file [%0]. Exit. \n",vim.tilesList.at(0).fn_image.c_str());
            return false;
        }

        VISIOCYTELONG pagesz = in_sz[0]*in_sz[1]*in_sz[2]*in_sz[3];

        for(VISIOCYTELONG ii = 1; ii < NTILES; ii++)
        {
            unsigned char * data1d = 0;
            VISIOCYTELONG in_sz[4];
            int datatype;

            if (!simple_loadimage_wrapper(callback,const_cast<char *>(vim.tilesList.at(ii).fn_image.c_str()), data1d, in_sz, datatype))
            {
                fprintf (stderr, "Error happens in reading the subject file [%0]. Exit. \n",vim.tilesList.at(ii).fn_image.c_str());
                return false;
            }
            for(VISIOCYTELONG j = 0; j < pagesz; j++)
            {
                if(data1d_mip[j] < data1d[j])
                    data1d_mip[j] = data1d[j];
            }
            if(data1d) {delete []data1d; data1d = 0;}
        }

        simple_saveimage_wrapper(callback, outimg_file,(unsigned char *)data1d_mip,in_sz,datatype);
        if(data1d_mip) {delete []data1d_mip; data1d_mip = 0;}
        return true;
    }
	else if (func_name == tr("help"))
	{
        cout<<"Usage : visiocyte -x dllname -f mip_imageseries -i <inimg_file> -o <outimg_file>"<<endl;
        cout<<endl;
	}
	else return false;

	return true;
}

