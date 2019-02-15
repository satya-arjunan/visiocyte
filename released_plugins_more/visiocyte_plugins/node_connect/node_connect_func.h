#ifndef NODE_CONNECT_FUNC_H
#define NODE_CONNECT_FUNC_H

#include <visiocyte_interface.h>

bool export_list2file_v2(QList<NeuronSWC> & lN, QString fileSaveName, QString FileSWCOpenName);
int connect_swc(VISIOCYTEPluginCallback2 &callback, QList<ImageMarker> tmp_list);
QList<ImageMarker> get_markers(VISIOCYTEPluginCallback2 &callback);
//void final_typeset_save();
void reload_SWC(VISIOCYTEPluginCallback2 &callback); //to be implimented
bool swc_toolbox(const VISIOCYTEPluginArgList & input);

#endif // NODE_CONNECT_FUNC_H
