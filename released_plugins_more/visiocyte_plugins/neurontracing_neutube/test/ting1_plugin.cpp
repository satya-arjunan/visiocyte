/* ting1_plugin.cpp
 * a collaboration with Hanchuan
 * 2012-12-9 : by Ting Zhao and HP
 */
 
#include "visiocyte_message.h"
#include <vector>
#include "ting1_plugin.h"
#include "zstack.hxx"
#include "nvinterface.h"
#include "zstackframe.h"


using namespace std;
Q_EXPORT_PLUGIN2(neutube, neutube);


neutube::neutube() : m_3dApp(QCoreApplication::applicationDirPath())
{
    m_mainWindow = new MainWindow();
    std::cout << "Initializing 3D ..." << std::endl;
    m_3dApp.initialize();

    m_mainWindow->config();
    m_mainWindow->initOpenglContext();
}

neutube::~neutube()
{
    m_3dApp.deinitializeGL();
    m_3dApp.deinitialize();
    delete m_mainWindow;
}


QStringList neutube::menulist() const
{
	return QStringList() 
		<<tr("neutube")
		<<tr("about");

}

QStringList neutube::funclist() const
{
	return QStringList()
		<<tr("neutube")
		<<tr("help");
}

void neutube::domenu(const QString &menu_name, VISIOCYTEPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("neutube"))
	{
        visiocyte_msg(tr("Show window"));

        m_mainWindow->show();

        visiocytehandle curwin = callback.currentImageWindow();
        if(!curwin)
        {
            return;
        }
        Image4DSimple *p4DImage = callback.getImage(curwin);

        if (!p4DImage) {
            return;
        }

        Mc_Stack *stack = NVInterface::makeStack(p4DImage);

        ZStackFrame *newFrame = new ZStackFrame;
        ZStack *stackObject = new ZStack(stack);
        newFrame->loadStack(stackObject);
        m_mainWindow->addStackFrame(newFrame);
        m_mainWindow->presentStackFrame(newFrame);
        m_mainWindow->enableStackActions(true);
        m_mainWindow->updateAction();
        m_mainWindow->updateMenu();
	}
	else
	{
        visiocyte_msg(tr("neuTube plugin v0.2. a collaboration with Hanchuan. "
            "Developed by Ting Zhao and HP, 2013-12-9"));
	}
}


bool neutube::dofunc(const QString & func_name, const VISIOCYTEPluginArgList & input, VISIOCYTEPluginArgList & output, VISIOCYTEPluginCallback2 & callback,  QWidget * parent)
{

	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("neutube"))
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

