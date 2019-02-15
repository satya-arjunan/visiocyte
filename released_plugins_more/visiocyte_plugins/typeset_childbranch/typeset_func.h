#ifndef TYPESET_FUNC_H
#define TYPESET_FUNC_H

#include <visiocyte_interface.h>

//bool export_list2file(QList<NeuronSWC> & lN, QString fileSaveName, QString FileSWCOpenName);
int typeset_swc_func(VISIOCYTEPluginCallback2 &callback, double settype, QList<ImageMarker> tmp_list_in);
QList<ImageMarker> get_markers(VISIOCYTEPluginCallback2 &callback);
void final_typeset_save();
void reload_SWC(VISIOCYTEPluginCallback2 &callback); //to be implimented
bool typeset_swc_toolbox(const VISIOCYTEPluginArgList & input);


#endif // TYPESET_FUNC_H
