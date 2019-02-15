#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void produce_simplest_plugin(string dir)
{
	ofstream ofs;
	string filename;

	filename = dir + "/test.pro";
	cout<<"output "<<filename<<endl;

	ofs.open((char*)filename.c_str());
	ofs<<""<<endl;
	ofs<<"TEMPLATE	= lib"<<endl;
	ofs<<"CONFIG	+= qt plugin warn_off"<<endl;
	ofs<<"#CONFIG	+= x86_64"<<endl;
	ofs<<"VISIOCYTE_DIR = /Users/xiaoh10/work/visiocyte_external"<<endl;
	ofs<<"INCLUDEPATH	+= $$VISIOCYTE_DIR/visiocyte_main/basic_c_fun"<<endl;
	ofs<<""<<endl;
	ofs<<"HEADERS	= test_plugin.h"<<endl;
	ofs<<"SOURCES	+= $$VISIOCYTE_DIR/visiocyte_main/basic_c_fun/visiocyte_message.cpp"<<endl;
	ofs<<""<<endl;
	ofs<<"TARGET	= $$qtLibraryTarget(test)"<<endl;
	ofs<<"DESTDIR	= $$VISIOCYTE_DIR/bin/plugins/test/"<<endl;
	ofs.close();

	filename = dir + "/test_plugin.h";
	cout<<"output "<<filename<<endl;

	ofs.open((char*)filename.c_str());
	ofs<<"#ifndef __TEST_PLUGIN_H__"<<endl;
	ofs<<"#define __TEST_PLUGIN_H__"<<endl;
	ofs<<""<<endl;
	ofs<<"#include <QtGui>"<<endl;
	ofs<<"#include <visiocyte_interface.h>"<<endl;
	ofs<<""<<endl;
	ofs<<"class TestPlugin : public QObject, public VISIOCYTEPluginInterface2_1"<<endl;
	ofs<<"{"<<endl;
	ofs<<"	Q_OBJECT"<<endl;
	ofs<<"	Q_INTERFACES(VISIOCYTEPluginInterface2_1);"<<endl;
	ofs<<""<<endl;
	ofs<<"public:"<<endl;
	ofs<<"	float getPluginVersion() const {return 1.1f;}"<<endl;
	ofs<<""<<endl;
	ofs<<"	QStringList menulist() const"<<endl;
	ofs<<"	{"<<endl;
	ofs<<"		return QStringList()"<<endl;
	ofs<<"			<< tr(\"test\")"<<endl;
	ofs<<"			<< tr(\"about\");"<<endl;
	ofs<<"	}"<<endl;
	ofs<<""<<endl;
	ofs<<"	void domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)"<<endl;
	ofs<<"	{"<<endl;
	ofs<<"		if(menu_name == \"test\")"<<endl;
	ofs<<"		{"<<endl;
	ofs<<"		}"<<endl;
	ofs<<"		else if(menu_name == \"about\")"<<endl;
	ofs<<"		{"<<endl;
	ofs<<"			QMessageBox::information(0, \"Raw_split Plugin About\", \\"<<endl;
	ofs<<"					QObject::tr(\"This is test plugin\"));"<<endl;
	ofs<<"		}"<<endl;
	ofs<<"	}"<<endl;
	ofs<<""<<endl;
	ofs<<"	QStringList funclist() const "<<endl;
	ofs<<"	{"<<endl;
	ofs<<"		return QStringList();"<<endl;
	ofs<<"	}"<<endl;
	ofs<<""<<endl;
	ofs<<"	bool dofunc(const QString &func_name, const VISIOCYTEPluginArgList &input, VISIOCYTEPluginArgList &output, VISIOCYTEPluginCallback2 &callback, QWidget *parent)"<<endl;
	ofs<<"	{"<<endl;
	ofs<<"		return false;"<<endl;
	ofs<<"	}"<<endl;
	ofs<<"};"<<endl;
	ofs<<""<<endl;
	ofs<<"Q_EXPORT_PLUGIN2(test, TestPlugin);"<<endl;
	ofs<<""<<endl;
	ofs<<"#endif"<<endl;
	ofs<<""<<endl;
	ofs.close();
}
