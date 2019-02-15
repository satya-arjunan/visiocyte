#include "toolbox_gui.h"
#include <iostream>
#include "customary_structs/visiocyte_neurontoolbox_para.h"
using namespace std;

static QString toolboxRootPath = QObject::tr("/Users/xiaoh10/Applications/visiocyte/plugins");
bool setPluginRootPathAutomaticly()
{
	QDir testPluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
	if (testPluginsDir.dirName().toLower() == "debug" || testPluginsDir.dirName().toLower() == "release")
		testPluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	// In a Mac app bundle, plugins directory could be either
	//  a - below the actual executable i.e. visiocyte.app/Contents/MacOS/plugins/
	//  b - parallel to visiocyte.app i.e. foo/visiocyte.app and foo/plugins/
	if (testPluginsDir.dirName() == "MacOS") {
		QDir testUpperPluginsDir = testPluginsDir;
		testUpperPluginsDir.cdUp();
		testUpperPluginsDir.cdUp();
		testUpperPluginsDir.cdUp(); // like foo/plugins next to foo/visiocyte.app
		testPluginsDir = testUpperPluginsDir;
	//	if (testUpperPluginsDir.cd("plugins")) testPluginsDir = testUpperPluginsDir;

	}
#endif
	if (testPluginsDir.cd("plugins")==false)
		return false;
	if (testPluginsDir.cd("neuron_utilities")==false)
		return false;
	toolboxRootPath=testPluginsDir.absolutePath();

	return true;
}

void getAllFiles(QString dirname, QStringList & fileList)
{
	QDir dir(dirname);
	QStringList dirlist = dir.entryList(QDir::Dirs);
	if(dirlist.size() == 2) 
	{
		QStringList files = dir.entryList(QDir::Files);
		QStringList::iterator fit = files.begin();
		while(fit != files.end())
		{
			fileList.append(dir.absoluteFilePath(*fit));
			fit++;
		}
		return;
	}

	for(QStringList::iterator it = dirlist.begin(); it != dirlist.end(); it++)
	{
		if(((*it) == ".") || ((*it) == "..")) continue;
		getAllFiles(dir.absoluteFilePath(*it), fileList);
	}
}

static QStringList visiocyte_getInterfaceMenuList(QObject *plugin)
{
	QStringList qslist;

	VISIOCYTESingleImageInterface2_1 *iFilter2_1 = qobject_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
	if (iFilter2_1 )  return (qslist = iFilter2_1->menulist());

	VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(plugin);
	if (iFilter )  return (qslist = iFilter->menulist());

	VISIOCYTEPluginInterface *iface = qobject_cast<VISIOCYTEPluginInterface *>(plugin);
	if (iface )  return (qslist = iface->menulist());

	VISIOCYTEPluginInterface2 *iface2 = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
	if (iface2 )  return (qslist = iface2->menulist());

	VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
	if (iface2_1 )  return (qslist = iface2_1->menulist());

	return qslist;
}

SelectPluginDlg::SelectPluginDlg(QWidget * parent, const VISIOCYTEPluginCallback2 & _callback)
	: QDialog(parent)
{
	setMinimumWidth(500);
	parent = parent;
	callback = (VISIOCYTEPluginCallback2 *) (&(_callback));
	
	QVBoxLayout * layout = new QVBoxLayout;
	
	//tree widget GUI
	pluginTreeWidget = new QTreeWidget();
	pluginTreeWidget->setColumnCount(1);
	pluginTreeWidget->header()->hide();
	pluginTreeWidget->setSortingEnabled(false);

	if (!setPluginRootPathAutomaticly())
		visiocyte_msg("You don't have any plugins on neuron utilities");
	QStringList fileList;
	getAllFiles(toolboxRootPath, fileList);
	root_path = toolboxRootPath;

	for (int i=0;i<fileList.size();i++)
	{
		QString file = fileList[i];
		QPluginLoader* loader = new QPluginLoader(file);
		if(!loader) 
		{
			cerr<<"unable to load plugin: "<<qPrintable(file)<<endl;
			continue;
		}

		QObject * plugin = loader->instance();

		if (plugin)
		{
			//lib - top level item
			QTreeWidgetItem *pluginItem = new QTreeWidgetItem(pluginTreeWidget);
			QString tmp = file.remove(0, root_path.size()+1);
			tmp.chop(file.section("/", -1).size()+1);
			pluginItem->setText(0, tmp);
			pluginTreeWidget->addTopLevelItem(pluginItem);
			name_table.insert(pluginItem,fileList[i]);

			QStringList menulist = visiocyte_getInterfaceMenuList(plugin);
			foreach(QString menu_name, menulist)
			{
				//menu - second level item
				QTreeWidgetItem * menuItem = new QTreeWidgetItem(pluginItem);
				menuItem->setText(0, menu_name);
			}
			
		}
		loader->unload();
		delete loader;
	}
	layout->addWidget(pluginTreeWidget);

	//run button
	QPushButton * button = new QPushButton("run");
	connect(button, SIGNAL(clicked()), this, SLOT(runMenu()));
	connect(pluginTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(runMenu()));

	layout->addWidget(button);

	setLayout(layout);
}

SelectPluginDlg::SelectPluginDlg(QWidget * parent, const VISIOCYTEPluginCallback2 & _callback, const VISIOCYTEPluginArgList & _input)
	: QDialog(parent)
{
	setMinimumWidth(500);
	input = new VISIOCYTEPluginArgList;
	*input = _input;
	const char* test_str4 = ((visiocyte_neurontoolbox_paras *)(input->at(0).p))->nt.file.toStdString().c_str();

	parent = parent;
	callback = (VISIOCYTEPluginCallback2 *) (&(_callback));
	
	QVBoxLayout * layout = new QVBoxLayout;
	
	//tree widget GUI
	pluginTreeWidget = new QTreeWidget();
	pluginTreeWidget->setColumnCount(1);
	pluginTreeWidget->header()->hide();
	pluginTreeWidget->setSortingEnabled(true);

	if (!setPluginRootPathAutomaticly())
		visiocyte_msg("You don't have any plugins on neuron utilities");
	QStringList fileList;
	getAllFiles(toolboxRootPath, fileList);
	root_path = toolboxRootPath;

	for(int i=0;i<fileList.size();i++)
	{
		QString file = fileList[i];
		QPluginLoader* loader = new QPluginLoader(file);
		if(!loader) 
		{
			cerr<<"unable to load plugin: "<<qPrintable(file)<<endl;
			continue;
		}

		//pluginLoaderList.push_back(loader);

		QObject * plugin = loader->instance();

		if (plugin)
		{
			//lib - top level item
			QTreeWidgetItem *pluginItem = new QTreeWidgetItem(pluginTreeWidget);
			QString tmp = file.remove(0, root_path.size()+1);
			tmp.chop(file.section("/", -1).size()+1);
			pluginItem->setText(0, tmp);
			pluginTreeWidget->addTopLevelItem(pluginItem);
			name_table.insert(pluginItem, fileList[i]);

			QStringList menulist = visiocyte_getInterfaceMenuList(plugin);
			foreach(QString menu_name, menulist)
			{
				//menu - second level item
				QTreeWidgetItem * menuItem = new QTreeWidgetItem(pluginItem);
				menuItem->setText(0, menu_name);
			}
		}
		loader->unload();
		delete loader;
	}
	layout->addWidget(pluginTreeWidget);

	//run button
	QPushButton * button = new QPushButton("run");
	connect(button, SIGNAL(clicked()), this, SLOT(runFunc()));
	connect(pluginTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(runFunc()));

	layout->addWidget(button);

	setLayout(layout);

	connect(this, SIGNAL(rejected()), this, SLOT(freeMem()));
}

bool SelectPluginDlg::runMenu()
{
	QTreeWidgetItem * menuItem = pluginTreeWidget->currentItem();
	if (!menuItem)
	{
		visiocyte_msg("Please select an item to run");
		return false;
	}
	QString menu_name = menuItem->text(0);
	QTreeWidgetItem * pluginItem = menuItem->parent();
	if (!pluginItem)
	{
		visiocyte_msg("Please select a menu name");
		return false;
	}
	//QString plugin_name = root_path + "/" + pluginItem->text(0);
	QString plugin_name = name_table[pluginItem];

	QPluginLoader* loader = new QPluginLoader(plugin_name);
	cout<<"plugin_file = "<<plugin_name.toStdString()<<endl;
	if(!loader)
	{
		visiocyte_msg("Unable to load this plugin!");
		return true;
	}

	loader->unload();
	QObject * plugin = loader->instance();
	VISIOCYTESingleImageInterface2_1 *iFilter2_1 = qobject_cast<VISIOCYTESingleImageInterface2_1 *>(plugin);
	if (iFilter2_1 )
	{
		QMessageBox::information(0,"","This is VISIOCYTESingleImageInterface2_1 plugin , only VISIOCYTEPluginInterface2 and VISIOCYTEPluginInterface2_1 is supported!");
		return false;
	}
	VISIOCYTESingleImageInterface *iFilter = qobject_cast<VISIOCYTESingleImageInterface *>(plugin);
	if (iFilter )
	{
		QMessageBox::information(0,"","This is VISIOCYTESingleImageInterface plugin , only VISIOCYTEPluginInterface2 and VISIOCYTEPluginInterface2_1 is supported!");
		return false;
	}
	VISIOCYTEPluginInterface2_1 *iface2_1 = qobject_cast<VISIOCYTEPluginInterface2_1 *>(plugin);
	if (iface2_1 )
	{
		iface2_1->domenu(menu_name, *callback, parent);
	}
	VISIOCYTEPluginInterface2 *iface2 = qobject_cast<VISIOCYTEPluginInterface2 *>(plugin);
	if (iface2 )
	{
		iface2->domenu(menu_name, *callback,parent);
	}
	VISIOCYTEPluginInterface *iface = qobject_cast<VISIOCYTEPluginInterface *>(plugin);
	if (iface )
	{
		iface->domenu(menu_name, *callback, parent);
	}
	//while(loader->isLoaded()) 
	loader->unload();
	delete loader;

	return true;
}

bool SelectPluginDlg::runFunc()
{
	VISIOCYTEPluginArgList output;
	QTreeWidgetItem * menuItem = pluginTreeWidget->currentItem();
	if (!menuItem)
	{
		visiocyte_msg("Please select an item to run");
		return false;
	}
	QString menu_name = menuItem->text(0);
	QTreeWidgetItem * pluginItem = menuItem->parent();
	if (!pluginItem)
	{
		visiocyte_msg("Please select a menu name");
		return false;
	}
	//QString plugin_name = root_path + "/" + pluginItem->text(0);
	QString plugin_name = name_table[pluginItem];

	if (!callback->callPluginFunc(plugin_name, "TOOLBOX" + menu_name, *input, output))
	{
		runMenu();
	}
	
	return true;
}

bool SelectPluginDlg::freeMem()
{
	visiocyte_neurontoolbox_paras * paras = (visiocyte_neurontoolbox_paras *)(input->at(0).p);
	if (paras) {delete paras; paras=NULL;}
	
	accept();
    return true;
}

