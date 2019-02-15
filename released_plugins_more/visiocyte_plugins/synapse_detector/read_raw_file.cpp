#include "read_raw_file.h"

#include "visiocyte_message.h"
#include <vector>
#include <iostream>
#include "stackutil.h"


#define b_VERBOSE_PRINT 0
#define DEFINE_NBYTE2G \
  VISIOCYTELONG nBytes2G = (VISIOCYTELONG(1024)*VISIOCYTELONG(1024)*VISIOCYTELONG(1024)-1)*VISIOCYTELONG(2);

typedef int BIT32_UNIT;

//char checkMachineEndian();
//void swap2bytes(void *targetp);
//void swap4bytes(void *targetp);


int loadRawRegion(char * filename, unsigned char * & img, VISIOCYTELONG * & sz, VISIOCYTELONG * & region_sz,int & datatype,
                  VISIOCYTELONG startx, VISIOCYTELONG starty, VISIOCYTELONG startz,
                  VISIOCYTELONG endx, VISIOCYTELONG endy, VISIOCYTELONG endz)
{
    FILE * fid = fopen(filename, "rb");

    if (!fid)
    {
        printf("Fail to open file for reading.\n");
        return 0;
    }

    fseek (fid, 0, SEEK_END);
    VISIOCYTELONG fileSize = ftell(fid);
    rewind(fid);
/*
#endif
*/
    /* Read header */

    char formatkey[] = "raw_image_stack_by_hpeng";
    VISIOCYTELONG lenkey = strlen(formatkey);


#ifndef _MSC_VER //added by PHC, 2010-05-21
    if (fileSize<lenkey+2+4*4+1) // datatype has 2 bytes, and sz has 4*4 bytes and endian flag has 1 byte.
    {
        printf("The size of your input file is too small and is not correct, -- it is too small to contain the legal header.\n");
        printf("The fseek-ftell produces a file size = %ld.", fileSize);
        return 1;
    }
#endif

    char * keyread = new char [lenkey+1];
    if (!keyread)
    {
        printf("Fail to allocate memory.\n");

        return 0;
    }

    VISIOCYTELONG nread = fread(keyread, 1, lenkey, fid);
    if (nread!=lenkey) {
        printf("File unrecognized or corrupted file.\n");

        if (keyread) {delete []keyread; keyread=0;}
        return 0;
    }

    keyread[lenkey] = '\0';

    VISIOCYTELONG i;
    if (strcmp(formatkey, keyread)) /* is non-zero then the two strings are different */
    {
        printf("Unrecognized file format.\n");
        if (keyread) {delete []keyread; keyread=0;}
        return 0;
    }

    char endianCodeData;
    int dummy = fread(&endianCodeData, 1, 1, fid);
    printf("The data endian code is [%c]\n", endianCodeData);
    if (endianCodeData!='B' && endianCodeData!='L')
    {
        if (keyread) {delete []keyread; keyread=0;}
        return 0;    }

    char endianCodeMachine;
    endianCodeMachine = checkMachineEndian();
    printf("The machine endian code is [%c]\n", endianCodeMachine);
    if (endianCodeMachine!='B' && endianCodeMachine!='L')
    {
        printf("This program only supports big- or little- endian but not other format. Check your data endian.\n");
        if (keyread) {delete []keyread; keyread=0;}
        return 0;
       }

    int b_swap = (endianCodeMachine==endianCodeData)?0:1;

    short int dcode = 0;
    dummy = fread(&dcode, 2, 1, fid); /* because I have already checked the file size to be bigger than the header, no need to check the number of actual bytes read. */
    if (b_swap)
        swap2bytes((void *)&dcode);

    switch (dcode)
    {
        case 1:
            datatype = 1; /* temporarily I use the same number, which indicates the number of bytes for each data point (pixel). This can be extended in the future. */
            break;

        case 2:
            datatype = 2;
            break;

        case 4:
            datatype = 4;
            break;

        default:
            if (keyread) {delete []keyread; keyread=0;}
            return  0;
    }

    VISIOCYTELONG unitSize = datatype; // temporarily I use the same number, which indicates the number of bytes for each data point (pixel). This can be extended in the future.

    BIT32_UNIT mysz[4];
    mysz[0]=mysz[1]=mysz[2]=mysz[3]=0;
    int tmpn=(int)fread(mysz, 4, 4, fid); // because I have already checked the file size to be bigger than the header, no need to check the number of actual bytes read.
    if (tmpn!=4) {
        if (keyread) {delete []keyread; keyread=0;}
        return 0;
    }

    if (b_swap)
    {
        for (i=0;i<4;i++)
        {
            //swap2bytes((void *)(mysz+i));
            if (b_VERBOSE_PRINT)
                printf("mysz raw read unit[%ld]: [%d] ", i, mysz[i]);
            swap4bytes((void *)(mysz+i));
            if (b_VERBOSE_PRINT)
                printf("swap unit: [%d][%0x] \n", mysz[i], mysz[i]);
        }
    }
    if (sz) {delete []sz; sz=0;}
    sz = new VISIOCYTELONG [4]; // reallocate the memory if the input parameter is non-null. Note that this requests the input is also an NULL point, the same to img.
    if (!sz)
    {
        if (keyread) {delete []keyread; keyread=0;}
        return 0;
    }

    VISIOCYTELONG totalUnit = 1;

    for (i=0;i<4;i++)
    {
        sz[i] = (VISIOCYTELONG)mysz[i];
        totalUnit *= sz[i];
    }

    VISIOCYTELONG tmpw = endx - startx;
    VISIOCYTELONG tmph = endy - starty;
    VISIOCYTELONG tmpz = endz - startz;

    VISIOCYTELONG head = 4*4+2+1+lenkey; // header_len ?
    VISIOCYTELONG pgsz1=sz[2]*sz[1]*sz[0], pgsz2=sz[1]*sz[0], pgsz3=sz[0];
    VISIOCYTELONG cn = tmpw*tmph*tmpz;
    VISIOCYTELONG kn = tmpw*tmph;
    VISIOCYTELONG total = tmpw*tmph*tmpz*sz[3];

    if (region_sz) {delete []region_sz; region_sz=0;}
    region_sz = new VISIOCYTELONG [4]; // reallocate the memory if the input parameter is non-null. Note that this requests the input is also an NULL point, the same to img.
    if (!region_sz)
    {
        if (keyread) {delete []keyread; keyread=0;}
        return 0;
    }

    region_sz[0] = tmpw;
    region_sz[1] = tmph;
    region_sz[2] = tmpz;
    region_sz[3] = sz[3];

    if (img) {delete []img; img=0;}
    VISIOCYTELONG totalBytes = VISIOCYTELONG(unitSize)*VISIOCYTELONG(total);
    try
    {
        img = new unsigned char [totalBytes];
    }
    catch (...)
    {
        if (keyread) {delete []keyread; keyread=0;}
        if (sz) {delete []sz; sz=0;}
        return 0;
    }

    //VISIOCYTELONG count=0; // unused
    VISIOCYTELONG c,j,k;
    for (c = 0; c < sz[3]; c++)
    {
        for (k = startz; k < endz; k++)
        {
            for (j = starty; j< endy; j++)
            {
                rewind(fid);
                fseek(fid, (long)(head+(c*pgsz1 + k*pgsz2 + j*pgsz3 + startx)*unitSize), SEEK_SET);
                int dummy = ftell(fid);
                dummy = fread(img+(c*cn+(k-startz)*kn + (j-starty)*tmpw)*unitSize,unitSize,tmpw,fid);
            }
        }
    }
    // swap the data bytes if necessary

    if (b_swap==1)
    {
        if (unitSize==2)
        {
            for (i=0;i<total; i++)
            {
                swap2bytes((void *)(img+i*unitSize));
            }
        }
        else if (unitSize==4)
        {
            for (i=0;i<total; i++)
            {
                swap4bytes((void *)(img+i*unitSize));
            }
        }
    }

    // clean and return

    if (keyread) {delete [] keyread; keyread = 0;}
    fclose(fid); //bug fix on 060412
    return 1;

}


bool writeRawRegion(char * filename, unsigned char * & img, VISIOCYTELONG * & sz, VISIOCYTELONG * & region_sz,int & datatype,
      VISIOCYTELONG startx, VISIOCYTELONG starty, VISIOCYTELONG startz,
      VISIOCYTELONG endx, VISIOCYTELONG endy, VISIOCYTELONG endz)
{
    FILE * fid = fopen(filename, "ab");

    if (!fid)
    {
        printf("Fail to open file for updating.\n");
        return 0;
    }

    fseek (fid, 0, SEEK_END);
    VISIOCYTELONG fileSize = ftell(fid);
    rewind(fid);
}
