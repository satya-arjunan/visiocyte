/* inter_node_pruning_plugin.cpp
 * This is a test plugin, you can use it as a demo.
 * 2014-09-03 : by Zhi Zhou
 */
 
#include "visiocyte_message.h"
#include <vector>
#include "inter_node_pruning_plugin.h"
#include "../sort_neuron_swc/openSWCDialog.h"

#include "my_surf_objs.h"
using namespace std;
Q_EXPORT_PLUGIN2(inter_node_pruning, inter_node_pruning);
 
QStringList inter_node_pruning::menulist() const
{
	return QStringList() 
        <<tr("inter-node pruning")
		<<tr("about");
}

QStringList inter_node_pruning::funclist() const
{
	return QStringList()
        <<tr("pruning")
		<<tr("func2")
		<<tr("help");
}

template <class T> T pow2(T a)
{
    return a*a;

}

void inter_node_pruning::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
    if (menu_name == tr("inter-node pruning"))
	{
        OpenSWCDialog * openDlg = new OpenSWCDialog(0, &callback);
        if (!openDlg->exec())
            return;

        QString fileOpenName = openDlg->file_name;

        if (fileOpenName.toUpper().endsWith(".SWC") || fileOpenName.toUpper().endsWith(".ESWC"))
        {
            NeuronTree nt = openDlg->nt;
            if(nt.listNeuron[0].pn >0)
            {
                visiocyte_msg("Please sort the swc file first before using this plugin.");
                return;
            }
            QVector<QVector<VISIOCYTELONG> > childs;
            VISIOCYTELONG neuronNum = nt.listNeuron.size();
            childs = QVector< QVector<VISIOCYTELONG> >(neuronNum, QVector<VISIOCYTELONG>() );
            for (VISIOCYTELONG i=0;i<neuronNum;i++)
            {
                VISIOCYTELONG par = nt.listNeuron[i].pn;
                if (par<0) continue;
                childs[nt.hashNeuron.value(par)].push_back(i);
            }

            vector<MyMarker*> final_out_swc = readSWC_file(fileOpenName.toStdString());
            vector<MyMarker*> final_out_swc_updated;
           // final_out_swc_updated.push_back(final_out_swc[0]);


            for(int j = 0; j < final_out_swc.size(); j++)
            {
                if(final_out_swc[j]->parent != 0)
                {
                    int flag_prun = 0;
                    int par_x = final_out_swc[j]->parent->x;
                    int par_y = final_out_swc[j]->parent->y;
                    int par_z = final_out_swc[j]->parent->z;
                    int par_r = final_out_swc[j]->parent->radius;

                    int dis_prun = sqrt(pow2(final_out_swc[j]->x - par_x) + pow2(final_out_swc[j]->y - par_y) + pow2(final_out_swc[j]->z - par_z));
                    if( (final_out_swc[j]->radius + par_r - dis_prun)/dis_prun > 0.3)
                    {
                        if(childs[j].size() > 0)
                        {
                            for(int jj = 0; jj < childs[j].size(); jj++)
                                final_out_swc[childs[j].at(jj)]->parent = final_out_swc[j]->parent;
                        }
                        flag_prun = 1;
                    }

                    if(flag_prun == 0)
                    {
                        final_out_swc_updated.push_back(final_out_swc[j]);
                    }
                }
                else
                    final_out_swc_updated.push_back(final_out_swc[j]);

            }


            QString fileDefaultName = fileOpenName+QString("_pruned.swc");
            QString fileSaveName = QFileDialog::getSaveFileName(0, QObject::tr("Save File"),
                    fileDefaultName,
                    QObject::tr("Supported file (*.swc)"
                        ";;Neuron structure	(*.swc)"
                        ));

            saveSWC_file(fileSaveName.toStdString(), final_out_swc_updated);
        }
	}
	else
	{
        visiocyte_msg(tr("This is a plugin to prun inter-nodes.. "
			"Developed by Zhi Zhou, 2014-09-03"));
	}
}

bool inter_node_pruning::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

    if (func_name == tr("pruning"))
	{
        cout<<"Welcome to swc inter-node pruning processing plugin"<<endl;
        if(infiles.empty())
        {
            cerr<<"Need input swc file"<<endl;
            return false;
        }

        QString  inswc_file =  infiles[0];
        NeuronTree nt = readSWC_file(inswc_file);
        if(nt.listNeuron[0].pn >0)
        {
            visiocyte_msg("Please sort the swc file first before using this plugin.",0);
            return false;
        }
        QVector<QVector<VISIOCYTELONG> > childs;
        VISIOCYTELONG neuronNum = nt.listNeuron.size();
        childs = QVector< QVector<VISIOCYTELONG> >(neuronNum, QVector<VISIOCYTELONG>() );
        for (VISIOCYTELONG i=0;i<neuronNum;i++)
        {
            VISIOCYTELONG par = nt.listNeuron[i].pn;
            if (par<0) continue;
            childs[nt.hashNeuron.value(par)].push_back(i);
        }

        vector<MyMarker*> final_out_swc = readSWC_file(inswc_file.toStdString());
        vector<MyMarker*> final_out_swc_updated;
     //   final_out_swc_updated.push_back(final_out_swc[0]);


        for(int j = 0; j < final_out_swc.size(); j++)
        {
            if(final_out_swc[j]->parent != 0)
            {
                int flag_prun = 0;
                int par_x = final_out_swc[j]->parent->x;
                int par_y = final_out_swc[j]->parent->y;
                int par_z = final_out_swc[j]->parent->z;
                int par_r = final_out_swc[j]->parent->radius;

                int dis_prun = sqrt(pow2(final_out_swc[j]->x - par_x) + pow2(final_out_swc[j]->y - par_y) + pow2(final_out_swc[j]->z - par_z));
                if( (final_out_swc[j]->radius + par_r - dis_prun)/dis_prun > 0.3)
                {
                    if(childs[j].size() > 0)
                    {
                        for(int jj = 0; jj < childs[j].size(); jj++)
                            final_out_swc[childs[j].at(jj)]->parent = final_out_swc[j]->parent;
                    }
                    flag_prun = 1;
                }

                if(flag_prun == 0)
                {
                    final_out_swc_updated.push_back(final_out_swc[j]);
                }
            }
            else
                final_out_swc_updated.push_back(final_out_swc[j]);

        }


        QString outswc_file = inswc_file+QString("_pruned.swc");
        saveSWC_file(outswc_file.toStdString(), final_out_swc_updated);
        return true;

	}
	else if (func_name == tr("help"))
	{
		visiocyte_msg("To be implemented.");
	}
	else return false;

	return true;
}

