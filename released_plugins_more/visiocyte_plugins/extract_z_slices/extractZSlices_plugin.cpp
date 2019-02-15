/* extractZSlices_plugin.cpp
 * A program to extract selected Z slices
 * 2013-08-05 : by Hanchuan Peng
 */
 
#include "visiocyte_message.h"
#include <QInputDialog>
#include "extractZSlices_plugin.h"
using namespace std;
Q_EXPORT_PLUGIN2(extractZSlices, extractZSlices);
 

bool parseFormatString(QString t, VISIOCYTELONG & startnum, VISIOCYTELONG & increment, VISIOCYTELONG & endnum, VISIOCYTELONG sz2);
bool extract_z_slices(Image4DSimple * subject, Image4DSimple & outImage,
             VISIOCYTELONG startnum, VISIOCYTELONG increment, VISIOCYTELONG endnum);

QStringList extractZSlices::menulist() const
{
	return QStringList() 
		<<tr("extract a subset of Z slices to a stack")
		<<tr("about");
}

QStringList extractZSlices::funclist() const
{
	return QStringList()
		<<tr("subzslices2stack")
		<<tr("help");
}

void extractZSlices::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("extract a subset of Z slices to a stack"))
	{
        visiocytehandle curwin = callback.currentImageWindow();
        Image4DSimple* subject = callback.getImage(curwin);
        if (!curwin || !subject)
        {
            visiocyte_msg("You don't have any image open in the main window.");
            return;
        }

        VISIOCYTELONG sz0 = subject->getXDim();
        VISIOCYTELONG sz1 = subject->getYDim();
        VISIOCYTELONG sz2 = subject->getZDim();
        VISIOCYTELONG sz3 = subject->getCDim();

        //

        VISIOCYTELONG startnum, increment, endnum;

        bool ok;
        QString text = QInputDialog::getText(0, tr("Enter the range and increment"),
                                             tr("Range (startslice:increment:endslicenum)    (startslice is 1-based, endslicenum can be 'e', increment can be 1,2,...)    (example: 1:2:e)"),
                                             QLineEdit::Normal,
                                             "1:3:e", &ok);
        if (ok && !text.isEmpty())
        {
            if (!parseFormatString(text, startnum, increment, endnum, sz2))
            {
                visiocyte_msg("The format of the string is not valid. Do nothing.");
                return;
            }
        }
        else
            return;

        QString m_InputFileName = callback.getImageName(curwin);

        //copy data
        VISIOCYTELONG sz2_new = ceil(double(endnum-startnum+1)/increment);

        Image4DSimple p4DImage;
        if (!extract_z_slices(subject, p4DImage, startnum, increment, endnum))
            return;

        //==

        visiocytehandle newwin;
        if(QMessageBox::Yes == QMessageBox::question (0, "", 
        	QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
            newwin = callback.currentImageWindow();
        else
            newwin = callback.newImageWindow();

        callback.setImage(newwin, &p4DImage);
        callback.setImageName(newwin, m_InputFileName + "_extracted_image_" + text + ".visiocyteraw");
        callback.updateImageWindow(newwin);
	}
	else
	{
        visiocyte_msg(tr("A program to extract selected Z slices and export to an image stack. "
			"Developed by Hanchuan Peng, 2013-08-05"));
	}
}

bool extractZSlices::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("subzslices2stack"))
	{
        if (infiles.size()<1 || inparas.size()<1 || output.size()<1 )
        {
            visiocyte_msg("No input image or no parameters or no output image is specified.");
            return false;
        }

        Image4DSimple *inimg=0, outimg;

        inimg = callback.loadImage(infiles.at(0));
        if (!inimg || !inimg->valid())
        {
            visiocyte_msg("Fail to load the specified input image.", 0);
            return false;
        }

        VISIOCYTELONG startnum, increment, endnum;
        QString paratext = inparas.at(0);
        if (!paratext.isEmpty())
        {
            if (!parseFormatString(paratext, startnum, increment, endnum, inimg->getZDim()))
            {
                visiocyte_msg("The format of the string is not valid. Do nothing.");
                return false;
            }
        }
        else
            return false;

        if (!extract_z_slices(inimg, outimg, startnum, increment, endnum))
            return false;

        if (!callback.saveImage(&outimg, outfiles.at(0)))
            return false;

        if (inimg) {delete inimg; inimg=0;}
    }
	else if (func_name == tr("help"))
	{
        visiocyte_msg("This plugin extracts a subset of slices and export to a stack (and save to a file)", 0);
        visiocyte_msg("Usage: visiocyte -x dll_name -f subzslices2stack -i input_image_file -p parameters_string -o output_image_file", 0);
        visiocyte_msg("parameters_string: Range (startslice:increment:endslicenum)    (startslice is 1-based, endslicenum can be 'e', increment can be 1,2,...)    (example: 1:2:e)", 0);
    }
	else return false;
	
	return true;
}

bool parseFormatString(QString t, VISIOCYTELONG & startnum, VISIOCYTELONG & increment, VISIOCYTELONG & endnum, VISIOCYTELONG sz2)
{
    if (sz2<=0)
        return false;

    QStringList list = t.split(":");
    if (list.size()<2)
        return false;

    bool ok;

    startnum = list.at(0).toLong(&ok)-1;
    if (!ok)
        startnum = 0;

    if (list.size()==2)
    {
        increment = 1;
        endnum = list.at(1).toLong(&ok);
        if (!ok)
            endnum = sz2-1;
    }
    else
    {
        increment = list.at(1).toLong(&ok);
        if (!ok) increment = 1;
        endnum = list.at(2).toLong(&ok);
        if (!ok) endnum = sz2-1;
    }

    if (increment<0) //this will not reverse the order of all z slices in a stack. This can be enhanced later.
        increment = -increment;
    if (endnum>=sz2)
        endnum = sz2-1;
    if (startnum<0)
        startnum = 0;
    if (startnum>endnum)
    {
        VISIOCYTELONG tmp=endnum; endnum=startnum; startnum=tmp;
    }

    qDebug() << " start=" << startnum << " increment=" << increment << " end=" << endnum;
    return true;
}


bool extract_z_slices(Image4DSimple * subject, Image4DSimple & outImage,
             VISIOCYTELONG startnum, VISIOCYTELONG increment, VISIOCYTELONG endnum)
{
    if (!subject || !subject->valid())
        return false;

    VISIOCYTELONG sz0 = subject->getXDim();
    VISIOCYTELONG sz1 = subject->getYDim();
    VISIOCYTELONG sz2 = subject->getZDim();
    VISIOCYTELONG sz3 = subject->getCDim();

    //copy data
    VISIOCYTELONG sz2_new = ceil(double(endnum-startnum+1)/increment);
    //qDebug() << "sz2_new=" << sz2_new << "end=" << endnum << "start=" << startnum << "incre=" << increment;

    outImage.createBlankImage(sz0, sz1, sz2_new, sz3, subject->getDatatype());
    if (!outImage.valid())
        return false;

    VISIOCYTELONG k=0, c, pagesz;
    for (VISIOCYTELONG i=startnum, k=0; i<=endnum, k<sz2_new; i+=increment, k++)
    {
        switch (subject->getDatatype())
        {
        case VISIOCYTE_UINT8:
            pagesz = sz0*sz1*subject->getUnitBytes();
            for (c=0;c<sz3;c++)
            {
                //printf("c=%d i=%d k=%d\n", c, i, k);
                unsigned char *dst = outImage.getRawDataAtChannel(c) + k*pagesz;
                unsigned char *src = subject->getRawDataAtChannel(c) + i*pagesz;
                memcpy(dst, src, pagesz);
            }
            break;
        case VISIOCYTE_UINT16:
            pagesz = sz0*sz1*subject->getUnitBytes();
            for (c=0;c<sz3;c++)
            {
                unsigned char *dst = outImage.getRawDataAtChannel(c) + k*pagesz;
                unsigned char *src = subject->getRawDataAtChannel(c) + i*pagesz;
                memcpy(dst, src, pagesz);
            }
            break;
        case VISIOCYTE_FLOAT32:
            pagesz = sz0*sz1*subject->getUnitBytes();
            for (c=0;c<sz3;c++)
            {
                unsigned char *dst = outImage.getRawDataAtChannel(c) + k*pagesz;
                unsigned char *src = subject->getRawDataAtChannel(c) + i*pagesz;
                memcpy(dst, src, pagesz);
            }
            break;
        default: visiocyte_msg("You should never see this. The data is not returned meaningfully. Check your data and code.");
            return false;
            break;
        }
    }

    return true;
}
