/* spine_detector_plugin.cpp
 * This tool detects spine
 * 2015-3-11 : by Yujie Li
 */
 
#include "spine_detector_plugin.h"
#include "manual_correct_dialog.h"
#include "file_io_dialog.h"
#include "manual_proofread_dialog.h"
#include "manual_proof_is.h"
//#include "spine_detector_dialog.h"
#include "combiner.h"

static file_io_dialog *file_dialog = 0;
static manual_proof_is *dialog=0;
static manual_proofread_dialog *proof_dialog=0;

using namespace std;
Q_EXPORT_PLUGIN2(spine_detector, spine_detector);

QStringList spine_detector::menulist() const
{
	return QStringList() 
//        <<tr("spine_detector_1")
//        <<tr("skeleton analysis")
//        <<tr("spine_detector_1.0 (proofread by spine)")
//        <<tr("spine_detector_1.1 (proofread by segment)")
//        <<tr("spine_detector_2.0 (for big images)")
//        <<tr("learning_test")
        <<tr("SpineDetector_NewProject")
        <<tr("SpineDetector_ExistingProject")
        <<tr("IS_Quantifier_NewProject")
        <<tr("IS_Quantifier_ExistingProject")
        <<tr("Combiner")
        <<tr("about");
}

QStringList spine_detector::funclist() const
{
	return QStringList()
		<<tr("func1")
		<<tr("func2")
		<<tr("help");
}

void spine_detector::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("spine_detector"))
	{

//        spine_detector_dialog *dialog=new spine_detector_dialog(&callback);
//        dialog->exec();
	}
    else if (menu_name==tr("spine_detector_1"))
    {
       //QString image_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\Gaussian_5_5_4.visiocyteraw";
       //QString image_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\shape_learn\\Test\\Dan_data\\dendrite_segment5_CN3_gausian_4_4_3.visiocyteraw";
       QString image_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\cd1_coh13-96_100x_cell_d1-1_s5_r1_c4_lh.visiocyteraw";
       QString skel_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\skeleton.swc_resampled.swc";
       //QString skel_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\shape_learn\\Test\\Dan_data\\segment5_resampled.swc";
       QString name_out="predict";
       spine_fun * spine_obj=new spine_fun(image_name.toAscii(),skel_name.toAscii()
                              ,name_out.toAscii(),&callback);
       spine_obj->loadData();
       spine_obj->init();
       spine_obj->reverse_dst_grow();
       spine_obj->run_intensityGroup();
       spine_obj->conn_comp_nb6();
       spine_obj->write_spine_center_profile();
       spine_obj->saveResult();
    }
//    else if(menu_name==tr("learning_test"))
//    {
//        QString image_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\svm_training\\training1.visiocyteraw";
//        QString out_name="learning_result.visiocyteraw";
//        QString marker_name="C:\\Users\\Jade\\Documents\\Visiocyte\\spine_dec_test_data\\svm_training\\training1.marker";
//        learning * learning_obj=new learning(&callback,image_name.toAscii(),marker_name.toAscii(),out_name.toAscii());
//        learning_obj->loadData();
//        learning_obj->loadmarker();
//        learning_obj->wavelet_start();
//        qDebug()<<"the end....";
//    }
//    else if(menu_name==tr("spine_detector_1.0 (proofread by spine)"))
//    {
//        manual_correct_dialog *manual_dialog=new manual_correct_dialog(&callback,0);
//        manual_dialog->show();
//    }

//    else if (menu_name==tr("spine_detector_1.1 (proofread by segment)"))
//    {
//        manual_correct_dialog *manual_dialog=new manual_correct_dialog(&callback,1);
//        manual_dialog->show();
//    }
//    else if (menu_name==tr("spine_detector_2.0 (for big images)"))
//    {
//        manual_correct_dialog *manual_dialog=new manual_correct_dialog(&callback);
//        manual_dialog->show();
//    }
    else if (menu_name==tr("SpineDetector_NewProject"))
    {
        file_dialog= new file_io_dialog(&callback,1);
        file_dialog->show();
    }

    else if (menu_name==tr("SpineDetector_ExistingProject"))
    {
        proof_dialog=new manual_proofread_dialog(&callback,true);
        proof_dialog->show();
    }
    else if (menu_name==tr("IS_Quantifier_NewProject"))
    {
        file_dialog=new file_io_dialog(&callback,2);
        file_dialog->show();
    }
    else if (menu_name==tr("IS_Quantifier_ExistingProject"))
    {
        dialog=new manual_proof_is(&callback,true);
        dialog->show();
    }
    else if (menu_name==tr("Combiner"))
    {
        combiner *this_comb = new combiner(&callback);
        this_comb->show();
    }
    else
        visiocyte_msg(tr("This tool is designed for spine morphology reconstructions. "
            "Developed by Yujie Li, 2015-3-11, reviewed 2017-03-07"));
}

bool spine_detector::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
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

