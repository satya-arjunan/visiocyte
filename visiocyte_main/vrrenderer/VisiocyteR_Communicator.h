#ifndef VISIOCYTER_COMMUNICATOR_H
#define VISIOCYTER_COMMUNICATOR_H

#include <QWidget>
#include <QtGui>
//#include <QtCore/QCoreApplication>
#include <QTcpSocket>
//#include <QRegExpValidator>
//#ifdef _WIN32
//    #include <windows.h>
//#endif

#include "../basic_c_fun/visiocyte_interface.h"


//class CMainApplication;
class My4DImage;
class MainWindow;

class VisiocyteR_Communicator : public QWidget
{
    Q_OBJECT

public:
    explicit VisiocyteR_Communicator(bool *client_flag = 0, QList<NeuronTree>* ntlist=0);
    ~VisiocyteR_Communicator();
	void onReadySend(QString &send_MSG);
	bool SendLoginRequest();
	//void StartVRScene(QList<NeuronTree>* ntlist, My4DImage *i4d, MainWindow *pmain,bool isLinkSuccess);
	void Update3DViewNTList(QString &msg, int type);

public slots:
	//void RunVRMainloop();
	//void SendHMDPosition();
private slots:

    void onReadyRead();
    void onConnected();
    void onDisconnected();

private:
	
    QTcpSocket* socket;
	QString userName;
	QString vr_Port;
	bool CURRENT_DATA_IS_SENT;
	bool * clienton;
	QList<NeuronTree> *NTList_3Dview;	
	int NTNumReceieved;
};



#endif // VISIOCYTER_COMMUNICATOR_H
