/* Opencv_example_plugin.cpp
 * This plugin will load image and swc in domenu and dofunc
 * 2012-02-21 : by Hang Xiao
 */

#include "visiocyte_message.h"

#include "LCM_boost_plugin.h"
#include "LCM_boost_func.h"

Q_EXPORT_PLUGIN2(Opencv_example, LoadImageAndSWCPlugin);

QStringList LoadImageAndSWCPlugin::menulist() const
{
	return QStringList()
		<<tr("LCM_boost(MultiScale Ehancement based)")
		<<tr("LCM_boost(Fast Marching based)")
		<<tr("LCM_boost(regression_tubularity based)")
		<<tr("LCM_boost(mostVesselTracer based)")
		<<tr("LCM_boost(neuTube based)")
		<<tr("LCM_boost(SimpleTracing based)")
		<<tr("LCM_boost(APP2 based)")
		<<tr("LCM_boost(APP1 based)")
		<<tr("LCM_boost(fastmarching_spanningtree based)")
		<<tr("LCM_boost(NeuroGPSTree based)")
		<<tr("LCM_boost(neurontracing_mst based)")
		<<tr("about");
}

QStringList LoadImageAndSWCPlugin::funclist() const
{
	return QStringList()
		<<tr("LCM_boost");
}

void LoadImageAndSWCPlugin::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{



  //  cout << menu_name.toStdString() << endl;

    //   cin.get();

	if (menu_name == tr("LCM_boost(MultiScale Ehancement based)"))
	{
		General_Boost1(callback,parent,1,0);
	}

	if (menu_name == tr("LCM_boost(Fast Marching based)"))
	{
		General_Boost1(callback,parent,2,0);
	}


	if (menu_name == tr("LCM_boost(RegressionTubularityAC based)"))
	{
		General_Boost1(callback,parent,4,0);
	}


	if (menu_name == tr("LCM_boost(mostVesselTracer based)"))
	{
		General_Boost1(callback,parent,3,1);
	}


	if (menu_name == tr("LCM_boost(neuTube based)"))
	{
		General_Boost1(callback,parent,3,2);
	}

	if (menu_name == tr("LCM_boost(SimpleTracing based)"))
	{
		General_Boost1(callback,parent,3,3);
	}

	if (menu_name == tr("LCM_boost(APP2 based)"))
	{
		General_Boost1(callback,parent,3,4);
	}

	if (menu_name == tr("LCM_boost(APP1 based)"))
	{
		General_Boost1(callback,parent,3,5);
	}

	if (menu_name == tr("LCM_boost(fastmarching_spanningtree based)"))
	{
		General_Boost1(callback,parent,3,6);
	}

	if (menu_name == tr("LCM_boost(NeuroGPSTree based)"))
	{
		General_Boost1(callback,parent,3,7);
	}


	if (menu_name == tr("LCM_boost(neurontracing_mst based)"))
	{
		General_Boost1(callback,parent,3,8);
	}


	if(menu_name == tr("about"))
	{
		visiocyte_msg(tr("This plugin will boost the performance of the existing methods"));
	}
}

bool LoadImageAndSWCPlugin::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	if (func_name == tr("LCM_boost"))
	{
		return General_Boost1(callback,input,output,1,0);
	}

	if (func_name == tr("Batch"))
	{
		return Batch_Process(callback, input, output);
	}

    if (func_name == tr("PreTraining"))
	{

		return Batch_Base_Method(callback, input, output);
	}

    if (func_name == tr("TrainLCM"))
	{

		return train_LCM(callback, input, output);
	}


//	if (func_name == tr("TrainLCM_BigNeuron"))
//	{

//		return train_LCM(callback, input, output);
//	}


    if (func_name == tr("BatchTest"))
	{

        return   Batch_Test1(callback, input, output,1,0);

        //Batch_Test1(callback, input, output,2,0);

     //  return Batch_Test2000(callback, input, output,1,0);
	}

    if (func_name == tr("BigNSWC2IMG"))
	{

        return   BigN_swc2img(callback, input, output);

        //Batch_Test1(callback, input, output,2,0);

     //  return Batch_Test2000(callback, input, output,1,0);
	}



    if (func_name == tr("BigNScore"))
	{

       //Batch_Test1(callback, input, output,1,0);

      // Dist_Score_BigN(callback, input, output,1,0);

       // Dist_Score_BigN(callback, input, output,2,0);

        Rank_Score_BigN(callback,input,output,1,0);

        return Rank_Score_BigN(callback,input,output,2,0);

     //  return Dist_Score_BigN(callback, input, output,2,0);

     //  return Batch_Test2000(callback, input, output,1,0);
	}

    if (func_name == tr("OPFScore"))
	{

       //Batch_Test1(callback, input, output,1,0);

      // Dist_Score_OPF(callback, input, output,1,0);

       //return Dist_Score_OPF(callback, input, output,2,0);

       Rank_Score_OPF(callback, input, output,1,0);

       return Rank_Score_OPF(callback, input, output,2,0);



     //  return Batch_Test2000(callback, input, output,1,0);
	}


    if (func_name == tr("BatchTest2000"))
	{

		return Batch_Test2000(callback, input, output,1,0);
	}





    if (func_name == tr("OPF"))
	{

        Batch_Test_OPF(callback, input, output,1,0);

		//return Batch_Test_OPF(callback, input, output,2,0);

		return Batch_Test_OPF(callback, input, output,2,0);

		//return Batch_Test2000(callback, input, output,1,0);
	}

	if (func_name == tr("Retinal"))
	{

        //Batch_Trace_Retinal(callback,input,output,1);

        return Batch_Trace_Retinal(callback,input,output);

	}

	if (func_name == tr("Batch_Test_BigNeuron_GT"))
	{

        return   Batch_Test_BigNeuron(callback, input, output,4,0);

	}


	if (func_name == tr("Batch_PreProcess_BigNeuron_GT"))
	{

        return   Batch_PreProcess_BigNeuron(callback, input, output,4,0);

	}


	if (func_name == tr("Batch_Evaluate_BigNeuron_GT"))
	{

        return   Batch_Evaluate_BigNeuron(callback, input, output);

	}




//    if (func_name == tr("BatchTest1"))
//	{

//		return Batch_Test1(callback, input, output);
//	}


	if (func_name == tr("LCM_boost_1"))
	{
		return General_Boost1(callback,input,output,1,0);
	}

	if (func_name == tr("LCM_boost_2"))
	{
		return General_Boost1(callback,input,output,2,0);
	}

	if (func_name == tr("LCM_boost_3"))
	{
		return General_Boost1(callback,input,output,4,0);
	}

	if (func_name == tr("LCM_boost_4"))
	{
		return General_Boost1(callback,input,output,3,1);
	}

	if (func_name == tr("LCM_boost_5"))
	{
		return General_Boost1(callback,input,output,3,3);
	}

	if (func_name == tr("LCM_boost_6"))
	{
		return General_Boost1(callback,input,output,3,4);
	}

	if (func_name == tr("LCM_boost_7"))
	{
		return General_Boost1(callback,input,output,3,5);
	}

	if (func_name == tr("LCM_boost_8"))
	{
		return General_Boost1(callback,input,output,3,6);
	}

	if (func_name == tr("LCM_boost_9"))
	{
		return General_Boost1(callback,input,output,3,7);
	}

	if (func_name == tr("LCM_boost_10"))
	{
		return General_Boost1(callback,input,output,3,8);
	}

		if (func_name == tr("help"))
	{

		cout << "**** Usage of LCM_boost tracing **** " << endl;
		printf("./visiocyte -x LCM_boost -f LCM_boost -i <inimg_file> -o  <outswc_file>\n");

		printf("e.g.    ./visiocyte -x LCM_boost -f LCM_boost -i B4.visiocyteraw -o tmp_tst.swc \n");

		printf("inimg_file       The input image\n");

        printf("outswc_file      The file name of output swc.\n");

        printf("The default base method is multiScale enhancement. Users could also denote alternative base method \n");

        printf(" The function name LCM_boost_x (x = 1,.., 10) correponds to different base method.\n");

        printf(" The function name LCM_boost_1 corresponds to multiScale enhancement base method (default). \n");

        printf(" The function name LCM_boost_2 corresponds to fast marching base method. \n");

        printf(" The function name LCM_boost_3 corresponds to RegressionTubularityAC base method. \n");

        printf(" The function name LCM_boost_4 corresponds to mostVesselTracer base method. \n");

        printf(" The function name LCM_boost_5 corresponds to SimpleTracing base method. \n");

        printf(" The function name LCM_boost_6 corresponds to APP2 base method. \n");

        printf(" The function name LCM_boost_7 corresponds to APP1 base method. \n");

        printf(" The function name LCM_boost_8 corresponds to fastmarching_spanningtree base method. \n");

        printf(" The function name LCM_boost_9 corresponds to NeuroGPSTree base method. \n");

        printf(" The function name LCM_boost_10 corresponds to neurontracing_mst base method. \n");

	}

}




