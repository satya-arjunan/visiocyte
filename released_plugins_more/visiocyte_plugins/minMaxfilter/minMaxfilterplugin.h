/* minMaxfilterplugin.h
 * 2009-08-19: create this program by Yang Yu
 */
// upgrade to VISIOCYTEPluginInterface2_1 2012-04-06, by Jianlong Zhou

#ifndef __MINMAXFILTER_H__
#define __MINMAXFILTER_H__

//     ND min Max min-Max Max-min filter
//
//     pImage  - input & output
//     N       - width of the image
//     M       - height of the image
//     P       - depth of the image
//     Wx      - radius size along x not include itself
//     Wy      - radius size along y
//     Wz      - radius size along z
//
//int minMaxfilter(float *pImage, VISIOCYTELONG N, VISIOCYTELONG M, VISIOCYTELONG P, unsigned int Wx, unsigned int Wy, unsigned int Wz);
//e.g. if(!minMaxfilter(pImg, sz[0], sz[1], sz[2], sigmax, sigmay, sigmaz))
//			cout<<"err"<<endl;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <visiocyte_interface.h>

class minMaxFilterPlugin : public QObject, public VISIOCYTEPluginInterface2_1
{
    Q_OBJECT
    Q_INTERFACES(VISIOCYTEPluginInterface2_1)

public:
     float getPluginVersion() const {return 1.11f;}

     QStringList menulist() const;
	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent);

	QStringList funclist() const;
     bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent);
};

#endif



