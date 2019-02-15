#include "AnalysisTools.h"

const int AnalysisTools::CUBIFY_TYPE_AVERAGE=1;
const int AnalysisTools::CUBIFY_TYPE_MODE=2;

AnalysisTools::AnalysisTools()
{
}

My4DImage * AnalysisTools::cubifyImageByChannel(My4DImage * sourceImage, int sourceChannel, int cubeSize, int type, VISIOCYTELONG* subregion=0L /* x0 x1 y0 y1 z0 z1 */,
                                                bool skipzeros=false, unsigned char** skipPositions=0L) {

    qDebug() << "AnalysisTools::cubifyImageByChannel : cubeSize=" << cubeSize;

    VISIOCYTELONG s_xmin=0L;
    VISIOCYTELONG s_ymin=0L;
    VISIOCYTELONG s_zmin=0L;
    VISIOCYTELONG s_xmax=sourceImage->getXDim();
    VISIOCYTELONG s_ymax=sourceImage->getYDim();
    VISIOCYTELONG s_zmax=sourceImage->getZDim();

    if (subregion!=0L) {
        s_xmin=subregion[0];
        s_xmax=subregion[1];
        s_ymin=subregion[2];
        s_ymax=subregion[3];
        s_zmin=subregion[4];
        s_zmax=subregion[5];
    }

    VISIOCYTELONG s_cmax=1; /* we are selecting a single channel */

    VISIOCYTELONG c_xmax=(s_xmax-s_xmin)/cubeSize;
    VISIOCYTELONG c_ymax=(s_ymax-s_ymin)/cubeSize;
    VISIOCYTELONG c_zmax=(s_zmax-s_zmin)/cubeSize;

    qDebug() << "cubifyImageByChannel: s_xmin=" << s_xmin << " s_ymin=" << s_ymin << " s_zmin=" << s_zmin << " s_xmax=" << s_xmax << " s_ymax=" << s_ymax << " s_zmax=" << s_zmax <<
                " c_xmax=" << c_xmax << " c_ymax=" << c_ymax << " c_zmax=" << c_zmax;

    My4DImage * cubeImage=new My4DImage();
    qDebug() << "cubifyImage() calling loadImage()";
    cubeImage->loadImage(c_xmax, c_ymax, c_zmax, s_cmax, VISIOCYTE_UINT8);
    qDebug() << "cubifyImage() after loadImage()";

    if (skipzeros) {
        *skipPositions=new unsigned char[c_xmax*c_ymax*c_zmax];
    }

    VISIOCYTELONG sSize=sourceImage->getTotalUnitNumberPerChannel();
    VISIOCYTELONG c=sourceChannel;
    visiocyte_uint8 * cData=cubeImage->getRawDataAtChannel(c);
    visiocyte_uint8 * sData=sourceImage->getRawDataAtChannel(c);
    VISIOCYTELONG totalCubeSize=cubeSize*cubeSize*cubeSize;
    VISIOCYTELONG cubeZeroCount=0L;
    VISIOCYTELONG *cubeData = new VISIOCYTELONG [totalCubeSize]; // by ZJL for windows compile
    for (VISIOCYTELONG z=0;z<c_zmax;z++) {
        VISIOCYTELONG zOffset=z*c_xmax*c_ymax;
        for (VISIOCYTELONG y=0;y<c_ymax;y++) {
            VISIOCYTELONG yOffset=y*c_xmax + zOffset;
            for (VISIOCYTELONG x=0;x<c_xmax;x++) {
                // clear cube data
                for (VISIOCYTELONG cl=0;cl<totalCubeSize;cl++) {
                    cubeData[cl]=0;
                }
                cubeZeroCount=0L;
                // walk through cube and score
                VISIOCYTELONG offset=x+yOffset;
                VISIOCYTELONG zStart=z*cubeSize + s_zmin;
                VISIOCYTELONG zEnd=(zStart+cubeSize<s_zmax?(zStart+cubeSize):s_zmax);
                VISIOCYTELONG yStart=y*cubeSize + s_ymin;
                VISIOCYTELONG yEnd=(yStart+cubeSize<s_ymax?(yStart+cubeSize):s_ymax);
                VISIOCYTELONG xStart=x*cubeSize + s_xmin;
                VISIOCYTELONG xEnd=(xStart+cubeSize<s_xmax?(xStart+cubeSize):s_xmax);
                VISIOCYTELONG cubeDataPosition=0;
                for (VISIOCYTELONG sz=zStart;sz<zEnd;sz++) {
                    VISIOCYTELONG s_zOffset=sz*s_xmax*s_ymax;
                    for (VISIOCYTELONG sy=yStart;sy<yEnd;sy++) {
                        VISIOCYTELONG s_yOffset=sy*s_xmax + s_zOffset;
                        for (VISIOCYTELONG sx=xStart;sx<xEnd;sx++) {
                            VISIOCYTELONG sPosition=sx+s_yOffset;
                            //qDebug() << "cubeDataPosition=" << cubeDataPosition << " sPosition=" << sPosition;
                            cubeData[cubeDataPosition]=sData[sPosition];
                            cubeDataPosition++;
                            if (skipzeros && sData[sPosition]==0) {
                                cubeZeroCount++;
                            }
                        }
                    }
                }
                if (type==CUBIFY_TYPE_AVERAGE) {
                    VISIOCYTELONG avg=0;
                    for (VISIOCYTELONG a=0;a<cubeDataPosition;a++) {
                        avg+=cubeData[a];
                    }
                    avg/=cubeDataPosition;
                    cData[offset]=avg;
                } else if (type==CUBIFY_TYPE_MODE) {
                    VISIOCYTELONG histogram[256];
                    for (int h=0;h<256;h++) {
                        histogram[h]=0;
                    }
                    for (VISIOCYTELONG a=0;a<cubeDataPosition;a++) {
                        histogram[cubeData[a]]++;
                    }
                    VISIOCYTELONG hmax=0;
                    visiocyte_uint8 hval=0;
                    for (int h=0;h<256;h++) {
                        if (histogram[h]>hmax) {
                            hmax=histogram[h];
                            hval=h;
                        }
                    }
                    cData[offset]=hval;
                }
                if (skipzeros) {
                    //if (cubeZeroCount>totalCubeSize/2) {
                    if (cubeZeroCount==cubeDataPosition) {
                        (*skipPositions)[offset]=1;
                    } else {
                        (*skipPositions)[offset]=0;
                    }
                }
            }
        }
    }
    if(cubeData){ delete []cubeData; cubeData=0;}
    qDebug() << "Returning cubeImage";
    return cubeImage;
}

My4DImage * AnalysisTools::cubifyImage(My4DImage * sourceImage, int cubeSize, int type) {
    VISIOCYTELONG s_xmax=sourceImage->getXDim();
    VISIOCYTELONG s_ymax=sourceImage->getYDim();
    VISIOCYTELONG s_zmax=sourceImage->getZDim();
    VISIOCYTELONG s_cmax=sourceImage->getCDim();

    VISIOCYTELONG c_xmax=s_xmax/cubeSize;
    VISIOCYTELONG c_ymax=s_ymax/cubeSize;
    VISIOCYTELONG c_zmax=s_zmax/cubeSize;

    My4DImage * cubeImage=new My4DImage();
    cubeImage->loadImage(c_xmax, c_ymax, c_zmax, s_cmax, VISIOCYTE_UINT8);

    VISIOCYTELONG sSize=sourceImage->getTotalUnitNumberPerChannel();
    VISIOCYTELONG cSize=cubeImage->getTotalUnitNumberPerChannel();

    for (VISIOCYTELONG c=0;c<s_cmax;c++) {
        My4DImage * cImage=AnalysisTools::cubifyImageByChannel(sourceImage, c, cubeSize, type, 0L);
        visiocyte_uint8 * cData=cImage->getRawDataAtChannel(0);
        visiocyte_uint8 * tData=cubeImage->getRawDataAtChannel(c);
        for (VISIOCYTELONG i=0;i<cSize;i++) {
            tData[i]=cData[i];
        }
        delete cImage;
    }
    return cubeImage;
}

My4DImage * AnalysisTools::getChannelSubImageFromMask(My4DImage* sourceImage, My4DImage* indexImage, int sourceChannel, int index, BoundingBox3D bb,
                                                               bool normalize, double normalizationCutoff /* 0.0 - 1.0 */) {

    VISIOCYTELONG xmax=indexImage->getXDim();
    VISIOCYTELONG ymax=indexImage->getYDim();
    VISIOCYTELONG zmax=indexImage->getZDim();

    visiocyte_uint8 * iData=indexImage->getRawData();
    visiocyte_uint8 * rData=sourceImage->getRawDataAtChannel(sourceChannel);

    My4DImage * subImage = new My4DImage();
    VISIOCYTELONG s_xlen=(bb.x1-bb.x0)+1;
    VISIOCYTELONG s_ylen=(bb.y1-bb.y0)+1;
    VISIOCYTELONG s_zlen=(bb.z1-bb.z0)+1;
    subImage->loadImage( s_xlen, s_ylen, s_zlen , 1 /* single channel */, VISIOCYTE_UINT8 /* datatype */);
    visiocyte_uint8 * sRaw=subImage->getRawData();
    for (VISIOCYTELONG i=0;i<subImage->getTotalBytes();i++) {
        sRaw[i]=0;
    }

    visiocyte_uint8 * sData=subImage->getRawDataAtChannel(0);

    double min=256.0;
    double max=-1.0;

    // First pass is to get accurate min/max for bounding box, which is tricky because we want to use a histogram to
    // apply the normalizationCutoff, so that outliers don't dominate the normalized range.
    VISIOCYTELONG histogram[256];
    int h=0;
    for (h=0;h<256;h++) {
        histogram[h]=0;
    }

    if (normalize) {
        VISIOCYTELONG hcount=0;
        for (int z=bb.z0;z<=bb.z1;z++) {
            VISIOCYTELONG zoffset=z*ymax*xmax;
            VISIOCYTELONG s_zoffset=(z-bb.z0)*s_ylen*s_xlen;
            for (int y=bb.y0;y<=bb.y1;y++) {
                VISIOCYTELONG yoffset = y*xmax + zoffset;
                VISIOCYTELONG s_yoffset = (y-bb.y0)*s_xlen + s_zoffset;
                for (int x=bb.x0;x<=bb.x1;x++) {
                    VISIOCYTELONG position = yoffset + x;
                    VISIOCYTELONG s_position = s_yoffset + (x-bb.x0);
                    if (iData[position]==index) {
                        histogram[rData[position]]++;
                        hcount++;
                    }
                }
            }
        }
        double lowerThreshold=normalizationCutoff*hcount;
        qDebug() << "hcount=" << hcount << " lowerThreshold=" << lowerThreshold;
        VISIOCYTELONG lowerCount=0;
        for (h=0;h<256;h++) {
            lowerCount+=histogram[h];
            if (lowerCount>=lowerThreshold) {
                break;
            }
        }
        qDebug() << "lowerCount=" << lowerCount;
        if (h==256) {
            qDebug() << "ERROR: could not find lower threshold for histogram";
            min=0;
        } else {
            min=h*1.0;
        }
        double higherThreshold=(1.0-normalizationCutoff)*hcount;
        qDebug() << "higherThreshold=" << higherThreshold;
        VISIOCYTELONG higherCount=hcount;
        for (h=255;h>-1;h--) {
            higherCount-=histogram[h];
            if (higherCount<=higherThreshold) {
                break;
            }
        }
        qDebug() << "higherCount=" << higherCount;
        if (h==-1) {
            qDebug() << "ERROR: could not find upper threshold for histogram";
            max=255.0;
        } else {
            max=h*1.0;
        }
        qDebug() << " Using normalization min/max of " << min << " " << max;
    }

    for (int z=bb.z0;z<=bb.z1;z++) {
        VISIOCYTELONG zoffset=z*ymax*xmax;
        VISIOCYTELONG s_zoffset=(z-bb.z0)*s_ylen*s_xlen;
        for (int y=bb.y0;y<=bb.y1;y++) {
            VISIOCYTELONG yoffset = y*xmax + zoffset;
            VISIOCYTELONG s_yoffset = (y-bb.y0)*s_xlen + s_zoffset;
            for (int x=bb.x0;x<=bb.x1;x++) {
                VISIOCYTELONG position = yoffset + x;
                VISIOCYTELONG s_position = s_yoffset + (x-bb.x0);
                if (iData[position]==index) {
                    if (normalize) {
                        double nvalue=0.0;
                        if ((max-min)!=0.0) {
                            nvalue=(255.0*((1.0*rData[position])-min))/(max-min);
                        }
                        visiocyte_uint8 nv=0;
                        if (nvalue>255.0) {
                            nv=255;
                        } else if (nvalue<0.0) {
                            nv=0;
                        } else {
                            nv=nvalue;
                        }
                        sData[s_position]=nv;
                    } else {
                        sData[s_position]=rData[position];
                    }
                }
            }
        }
    }
    return subImage;
}

My4DImage * AnalysisTools::createMIPFromImage(My4DImage * image) {

    if (image->getDatatype()!=VISIOCYTE_UINT8) {
        qDebug() << "image has datatype=" << image->getDatatype() << " , createMIPFromImage only supports datatype 1";
        return 0;
    }
    Image4DProxy<My4DImage> stackProxy(image);
    My4DImage * mip = new My4DImage();
    mip->loadImage( stackProxy.sx, stackProxy.sy, 1 /* z */, stackProxy.sc, VISIOCYTE_UINT8 );
    memset(mip->getRawData(), 0, mip->getTotalBytes());
    Image4DProxy<My4DImage> mipProxy(mip);

    for (int y=0;y<stackProxy.sy;y++) {
        for (int x=0;x<stackProxy.sx;x++) {
            VISIOCYTELONG maxIntensity=0;
            int maxPosition=0;
            for (int z=0;z<stackProxy.sz;z++) {
                VISIOCYTELONG currentIntensity=0;
                for (int c=0;c<stackProxy.sc;c++) {
                    currentIntensity+=(*stackProxy.at(x,y,z,c));
                }
                if (currentIntensity>maxIntensity) {
                    maxIntensity=currentIntensity;
                    maxPosition=z;
                }
            }
            for (int c=0;c<stackProxy.sc;c++) {
                mipProxy.put_at(x,y,0,c,(*stackProxy.at(x,y,maxPosition,c)));
            }
        }
    }
    return mip;
}

My4DImage* AnalysisTools::createMIPFromImageByLUT(My4DImage * image, visiocyte_uint8 * lut) {
    if (image->getDatatype()!=VISIOCYTE_UINT8) {
        qDebug() << "AnalysisTools::createMIPFromImage only supports datatype 1";
        return 0;
    }

    VISIOCYTELONG xsize=image->getXDim();
    VISIOCYTELONG ysize=image->getYDim();
    VISIOCYTELONG zsize=image->getZDim();

    My4DImage * mip = new My4DImage();
    mip->loadImage( xsize, ysize, 1 /* z */, image->getCDim(), VISIOCYTE_UINT8 );
    memset(mip->getRawData(), 0, mip->getTotalBytes());

    visiocyte_uint8 * s_r = image->getRawDataAtChannel(0);
    visiocyte_uint8 * s_g = image->getRawDataAtChannel(1);
    visiocyte_uint8 * s_b = image->getRawDataAtChannel(2);

    visiocyte_uint8 * t_r = mip->getRawDataAtChannel(0);
    visiocyte_uint8 * t_g = mip->getRawDataAtChannel(1);
    visiocyte_uint8 * t_b = mip->getRawDataAtChannel(2);

    VISIOCYTELONG zslice=xsize*ysize;
    for (int y=0;y<ysize;y++) {
      for (int x=0;x<xsize;x++) {
    visiocyte_uint8 maxIndex=0;
    VISIOCYTELONG xy_offset = y*xsize+x;
    for (int z=0;z<zsize;z++) {
      VISIOCYTELONG total_offset = z*zslice+xy_offset;
      visiocyte_uint8 r=s_r[total_offset];
      visiocyte_uint8 g=s_g[total_offset];
      visiocyte_uint8 b=s_b[total_offset];
      visiocyte_uint8 lutIndex = getReverse16ColorLUT(lut, r, g, b);
      if (lutIndex>maxIndex) {
        maxIndex=lutIndex;
      }
    }
    visiocyte_uint8 max_r=lut[maxIndex];
    visiocyte_uint8 max_g=lut[maxIndex+256];
    visiocyte_uint8 max_b=lut[maxIndex+512];
    t_r[xy_offset]=max_r;
    t_g[xy_offset]=max_g;
    t_b[xy_offset]=max_b;
      }
    }
    return mip;
}

visiocyte_uint8 AnalysisTools::getReverse16ColorLUT(visiocyte_uint8 * lut, visiocyte_uint8 r, visiocyte_uint8 g, visiocyte_uint8 b)
{
  for (int i=0;i<256;i++) {
    int i2=i+256;
    int i3=i+512;
    if (lut[i]==r &&
    lut[i2]==g &&
    lut[i3]==b) {
      visiocyte_uint8 v=i;
      return v;
    }
  }
  return 0;
}

visiocyte_uint8 * AnalysisTools::create16Color8BitLUT_fiji()
{
    visiocyte_uint8 * lut16 = new visiocyte_uint8[256*3];

    for (int i=0;i<16;i++) {
      for (int j=0;j<16;j++) {
    int index=i*16+j;
    if (i==0) {
      lut16[index]    = 0;
      lut16[index+256]= 0;
      lut16[index+512]= 0;
    } else if (i==1) {
      lut16[index]    = 1;
      lut16[index+256]= 1;
      lut16[index+512]= 171;
    } else if (i==2) {
      lut16[index]    = 1;
      lut16[index+256]= 1;
      lut16[index+512]= 224;
    } else if (i==3) {
      lut16[index]    = 0;
      lut16[index+256]= 110;
      lut16[index+512]= 255;
    } else if (i==4) {
      lut16[index]    = 1;
      lut16[index+256]= 171;
      lut16[index+512]= 254;
    } else if (i==5) {
      lut16[index]    = 1;
      lut16[index+256]= 224;
      lut16[index+512]= 254;
    } else if (i==6) {
      lut16[index]    = 1;
      lut16[index+256]= 254;
      lut16[index+512]= 1;
    } else if (i==7) {
      lut16[index]    = 190;
      lut16[index+256]= 255;
      lut16[index+512]= 0;
    } else if (i==8) {
      lut16[index]    = 255;
      lut16[index+256]= 255;
      lut16[index+512]= 0;
    } else if (i==9) {
      lut16[index]    = 255;
      lut16[index+256]= 224;
      lut16[index+512]= 0;
    } else if (i==10) {
      lut16[index]    = 255;
      lut16[index+256]= 141;
      lut16[index+512]= 0;
    } else if (i==11) {
      lut16[index]    = 250;
      lut16[index+256]= 94;
      lut16[index+512]= 0;
    } else if (i==12) {
      lut16[index]    = 245;
      lut16[index+256]= 0;
      lut16[index+512]= 0;
    } else if (i==13) {
      lut16[index]    = 245;
      lut16[index+256]= 0;
      lut16[index+512]= 185;
    } else if (i==14) {
      lut16[index]    = 222;
      lut16[index+256]= 180;
      lut16[index+512]= 222;
    } else if (i==15) {
      lut16[index]    = 255;
      lut16[index+256]= 255;
      lut16[index+512]= 255;
    }
      }
    }
    return lut16;
}

visiocyte_uint8 * AnalysisTools::create16Color8BitLUT()
{
    visiocyte_uint8 * lut16 = new visiocyte_uint8[256*3];

    for (int i=0;i<16;i++) {
        for (int j=0;j<16;j++) {
            int index=i*16+j;
            if (i==0) {
                lut16[index]    = 0;
                lut16[index+256]= 0;
                lut16[index+512]= 0 + ((171*j)/16);
            } else if (i==1) {
                lut16[index]    = 1;
                lut16[index+256]= 1;
                lut16[index+512]= 171 + (((224-171)*j)/16);
            } else if (i==2) {
                lut16[index]    = 1;
                lut16[index+256]= 1 + (((110-1)*j)/16);
                lut16[index+512]= 224;
            } else if (i==3) {
                lut16[index]    = 0;
                lut16[index+256]= 110 + (((171-110)*j)/16);
                lut16[index+512]= 255;
            } else if (i==4) {
                lut16[index]    = 1;
                lut16[index+256]= 171 + (((224-171)*j)/16);
                lut16[index+512]= 254;
            } else if (i==5) {
                lut16[index]    = 1;
                lut16[index+256]= 224;
                lut16[index+512]= 254 - (((254-1)*j)/16);
            } else if (i==6) {
                lut16[index]    = 1 + (((190-1)*j)/16);
                lut16[index+256]= 254;
                lut16[index+512]= 1;
            } else if (i==7) {
                lut16[index]    = 190 + (((255-190)*j)/16);
                lut16[index+256]= 255;
                lut16[index+512]= 0;
            } else if (i==8) {
                lut16[index]    = 255;
                lut16[index+256]= 255 - (((255-224)*j)/16);
                lut16[index+512]= 0;
            } else if (i==9) {
                lut16[index]    = 255;
                lut16[index+256]= 224 - (((224-141)*j)/16);
                lut16[index+512]= 0;
            } else if (i==10) {
                lut16[index]    = 255 - (((255-250)*j)/16);
                lut16[index+256]= 141 - (((141-94)*j)/16);
                lut16[index+512]= 0;
            } else if (i==11) {
                lut16[index]    = 250 - (((250-245)*j)/16);
                lut16[index+256]= 94 - (((94-0)*j)/16);
                lut16[index+512]= 0;
            } else if (i==12) {
                lut16[index]    = 245;
                lut16[index+256]= 0;
                lut16[index+512]= 0 + (((185-0)*j)/16);
            } else if (i==13) {
                lut16[index]    = 245 - (((245-222)*j)/16);
                lut16[index+256]= 0 + (((180-0)*j)/16);
                lut16[index+512]= 185 + (((222-185)*j)/16);
            } else if (i==14) {
                lut16[index]    = 222 + (((237-222)*j)/16);
                lut16[index+256]= 180 + (((215-180)*j)/16);
                lut16[index+512]= 222 + (((237-222)*j)/16);
            } else if (i==15) {
                lut16[index]    = 237 + (((255-237)*j)/16);
                lut16[index+256]= 215 + (((255-215)*j)/16);
                lut16[index+512]= 237 + (((255-237)*j)/16);
            }
            if (index==255) {
                lut16[index]    = 255;
                lut16[index+256]= 255;
                lut16[index+512]= 255;
            }
        }
    }
    return lut16;
}

