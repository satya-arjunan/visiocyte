/* imageIO_bioformat.cpp
 * 2010-06-01: create this program by Yang Yu
 */

#include "imageIO_bioformat.h"

// .raw or .tif IO
#include "../../basic_c_fun/stackutil.h"

//
void printHelp();
void printHelp()
{
	//pirnt help messages
	printf("\nUsage: imageIO_bioformat <input_file>\n");
	printf(" Image IO using Bio-format. \n");
	printf(" [-h]	HELP message.\n");
	return;
}


//read image file and save as .tif/.raw file
int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printHelp ();
		return 0;
	}
	
	char *dfile_input = argv[1];
	//char *dfile_output = argv[2];
	
	try 
	{
		string filename = dfile_input;
		
		createJVM();
		
		vector<IMAGE> pImgList;

		//
		ImageReader* imageReader = new ImageReader; // read all image formats
		ChannelFiller* channelFiller = new ChannelFiller(*imageReader); // expand 8-bit indexed color images to full RGB
		ChannelSeparator* reader = new ChannelSeparator(*channelFiller); // split interleaved RGB channels into separate image planes
		//FileStitcher* fileStitcher = new FileStitcher(channelSeparator);	
		
		// attach OME metadata object
		IMetadata omeMeta = MetadataTools::createOMEXMLMetadata();
		reader->setMetadataStore(omeMeta);
		
		bool isType = reader->isThisType(filename);
		printf("Is this kind of image recognized by Bio-Format: %s \n",(isType)?"Yes":"No");
		
		reader->setId(filename);
		
		String id = imageReader->getFormat(filename);
		//std::cout<< "Format is "<< id<<endl;
		
		int seriesCount = reader->getSeriesCount();
		std::cout << "How many series: " << seriesCount << endl;
		
		MetadataStore ms = reader->getMetadataStore();
		MetadataRetrieve mr = MetadataTools::asRetrieve(ms);
		
		//save temp images' names to text file
		FILE *pTIFile=0;
		string tmpimage = "names_tmp_images.txt";
		pTIFile = fopen(tmpimage.c_str(),"wt");

		// for all series
		for (int js=0; js<seriesCount; js++) 
		{
			reader->setSeries(js);
			
			std::cout << "Series #" << js;
			if (js < mr.getImageCount()) {
				std::cout << " -- " << mr.getImageName(js) << endl;
			}
			
			int pixelType = reader->getPixelType();
			std::cout << "pixelType is " << pixelType << " -- ";
			
			ImagePixelType visiocyteComponentType;
			
			if (pixelType == FormatTools::UINT8())
			{
				visiocyteComponentType = VISIOCYTE_UINT8;
				std::cout << "UINT8" << endl;
			}
			else if (pixelType == FormatTools::INT8())
			{
				visiocyteComponentType = VISIOCYTE_UINT8;
				std::cout << "INT8" << endl;
			}
			else if (pixelType == FormatTools::UINT16())
			{
				visiocyteComponentType = VISIOCYTE_UINT16;
				std::cout << "UINT16" << endl;
			}
			else if (pixelType == FormatTools::INT16())
			{
				visiocyteComponentType = VISIOCYTE_UINT16;
				std::cout << "INT16" << endl;
			}
			else if (pixelType == FormatTools::UINT32())
			{
				visiocyteComponentType = VISIOCYTE_FLOAT32;
				std::cout << "UINT32" << endl;
			}
			else if (pixelType == FormatTools::INT32())
			{
				visiocyteComponentType = VISIOCYTE_FLOAT32;
				std::cout << "INT32" << endl;
			}
			else if (pixelType == FormatTools::FLOAT())
			{
				visiocyteComponentType = VISIOCYTE_FLOAT32;
				std::cout << "FLOAT" << endl;
			}
			else if (pixelType == FormatTools::DOUBLE())
			{
				visiocyteComponentType = VISIOCYTE_FLOAT32;
				std::cout << "DOUBLE" << endl;
			}
			else
				return -1;
			
			// get pixel resolution and dimensional extents
			//change datatype to VISIOCYTELONG, 2010-06-01, by PHC
			VISIOCYTELONG sizeX = reader->getSizeX();
			VISIOCYTELONG sizeY = reader->getSizeY();
			VISIOCYTELONG sizeZ = reader->getSizeZ();
			VISIOCYTELONG sizeC = reader->getSizeC();
			VISIOCYTELONG sizeT = reader->getSizeT();
			VISIOCYTELONG effSizeC = reader->getEffectiveSizeC(); // always equals sizeC (because of ChannelSeparator)
			VISIOCYTELONG rgbChannelCount = reader->getRGBChannelCount(); // always 1 (because of ChannelSeparator)
			VISIOCYTELONG imageCount = reader->getImageCount(); // imageCount = sizeZ*sizeT*effSizeC;
			VISIOCYTELONG bpp = FormatTools::getBytesPerPixel(pixelType);
			
			bool little = reader->isLittleEndian(); // for 16-bit data Endianness (intel little, motorola big)
			printf("Endianness = %s \n", (little ? "intel (little)" : "motorola (big)"));
			
			printf("sizeX %d sizeY %d sizeZ %d sizeC %d sizeT %d effSizeC %d rgbChannelCount %d imageCount %d bpp %d \n", sizeX, sizeY, sizeZ, sizeC, sizeT, effSizeC, rgbChannelCount, imageCount, bpp);
			
			// size of image
			VISIOCYTELONG N = sizeX*sizeY*sizeZ*sizeC*sizeT*bpp; //
			unsigned char* data1d_8 = 0;
			short int* data1d_16 = 0;
			float* data1d_32 = 0;
			
			IMAGE pImg(sizeX, sizeY, sizeZ, sizeC, sizeT, visiocyteComponentType, mr.getImageName(js));
			
			if(visiocyteComponentType == VISIOCYTE_UINT8)
			{
				data1d_8 = pImg.data1d_8;
				printf("VISIOCYTE_UINT8 \n");
			}
			else if (visiocyteComponentType == VISIOCYTE_UINT16)
			{
				data1d_16 = pImg.data1d_16;
				printf("VISIOCYTE_UINT16 \n");
			}
			else if (visiocyteComponentType == VISIOCYTE_FLOAT32)
			{
				data1d_32 = pImg.data1d_32;
				printf("VISIOCYTE_FLOAT32 \n");
			}
			
			// check image info
			//change the following data types from int to VISIOCYTELONG, by PHC, 100601
			VISIOCYTELONG pIndex = 0, pCount = imageCount;
			VISIOCYTELONG bytesPerPlane = sizeX * sizeY * bpp * rgbChannelCount;
			
			VISIOCYTELONG pagesz = sizeX * sizeY * bpp;
			VISIOCYTELONG channelsz = sizeX * sizeY * sizeZ*sizeT * bpp;
			
			jbyte* jData;
			
			if(visiocyteComponentType == VISIOCYTE_UINT8)
				jData = (jbyte*) data1d_8;
			else if (visiocyteComponentType == VISIOCYTE_UINT16)
				jData = (jbyte*) data1d_16;
			else if (visiocyteComponentType == VISIOCYTE_FLOAT32)
				jData = (jbyte*) data1d_32;

			ByteArray buf(bytesPerPlane); // pre-allocate buffer
			
			jbyte* p = jData;
			for (VISIOCYTELONG no=pIndex; no<pCount; no++)
			{
				jData=p;
				for(VISIOCYTELONG c=0; c<sizeC; c++)
				{
					reader->openBytes(no++, buf, 0, 0, sizeX, sizeY);

					// copy raw byte array
					JNIEnv* env = jace::helper::attach();
					jbyteArray jArray = static_cast<jbyteArray>(buf.getJavaJniArray());
					env->GetByteArrayRegion(jArray, 0, bytesPerPlane, jData);
					jData += channelsz;
				}
				no--;
				p += bytesPerPlane;
			}
			
			pImgList.push_back(pImg);

			//temprary image
			char tmp[5];
			int ten = js/10;
			int dig = js - ten*10;
			tmp[0] = 's'; tmp[1] = ten + '0'; tmp[2] = dig + '0'; tmp[3] = '\0';

			string tmp_filename = filename + "_" + tmp + ".raw";

			VISIOCYTELONG sz_tmp[4];
			
			sz_tmp[0] = sizeX; sz_tmp[1] = sizeY; sz_tmp[2] = sizeZ*sizeT; sz_tmp[3] = sizeC; 

			if(visiocyteComponentType == VISIOCYTE_UINT8)
			{
				if (saveImage(tmp_filename.c_str(), (const unsigned char *)data1d_8, sz_tmp, VISIOCYTE_UINT8)!=true)
				{
					fprintf(stderr, "Error happens in file writing. Exit. \n");
					return -1;
				}
			}
			else if (visiocyteComponentType == VISIOCYTE_UINT16)
			{
				if (saveImage(tmp_filename.c_str(), (const unsigned char *)data1d_16, sz_tmp, VISIOCYTE_UINT16)!=true)
				{
					fprintf(stderr, "Error happens in file writing. Exit. \n");
					return -1;
				}
			}
			else if (visiocyteComponentType == VISIOCYTE_FLOAT32)
			{
				if (saveImage(tmp_filename.c_str(), (const unsigned char *)data1d_32, sz_tmp, VISIOCYTE_FLOAT32)!=true)
				{
					fprintf(stderr, "Error happens in file writing. Exit. \n");
					return -1;
				}
			}
			
			//
			fprintf(pTIFile, "%s \n", tmp_filename.c_str() );
		
		}
		// de-alloc
		imageReader->close();
		delete imageReader;
		delete channelFiller;
		delete reader;
		fclose(pTIFile);

		return 0;
		
	}
	catch (FormatException& fe) 
	{
		fe.printStackTrace();
		return -2;
	}
	catch (IOException& ioe) 
	{
		ioe.printStackTrace();
		return -3;
	}
	catch (JNIException& jniException)
	{
		std::cout << "An unexpected JNI error occurred. " << jniException.what() << endl;
		return -4;
	}
	catch (std::exception& e) 
	{
		std::cout << "An unexpected C++ error occurred. " << e.what() << endl;
		return -5;
	}
	
	
}
