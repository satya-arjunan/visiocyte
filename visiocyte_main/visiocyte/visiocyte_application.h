//last change: by Hanchuan Peng, 110828. add conditional compilation of mode menu
//last change: by Hanchuan Peng, 120217. add a new constructor VisiocyteApplication()

#ifndef VISIOCYTE_APPLICATION_H
#define VISIOCYTE_APPLICATION_H

#include "../3drenderer/visiocyter_common.h"

#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"

#ifdef _ALLOW_WORKMODE_MENU_
#include "../neuron_annotator/gui/NaMainWindow.h"
#endif

#include "visiocyte_compile_constraints.h"

class VisiocyteApplication : public QApplication
{
    Q_OBJECT

private:

    static VisiocyteApplication* theApp;
    static MainWindow* mainWindow;
    static bool mainWindowIsActive;

#ifdef _ALLOW_WORKMODE_MENU_
    static NaMainWindow* naMainWindow;
    static bool naMainWindowIsActive;
#endif

    explicit VisiocyteApplication(int & argc, char ** argv);

    static void deactivateMainWindowHelper(QMainWindow* qMainWindow) {
        if (qMainWindow!=0) {
            theApp->removeEventFilter(qMainWindow);
            // Remember the window size and position before deactivating
            QPoint windowPosition = qMainWindow->pos();
            QSize windowSize = qMainWindow->size();
            QSettings settings("HHMI", "Visiocyte");
            settings.setValue("pos", windowPosition);
            settings.setValue("size", windowSize);
            qMainWindow->hide();
        }
    }

    static void activateMainWindowHelper(QMainWindow* qMainWindow) {
        if (qMainWindow!=0) {
#ifdef CGS_AUTOLAUNCH
			qMainWindow->resize(QSize(0, 0));
			qMainWindow->hide();
#endif
			theApp->installEventFilter(qMainWindow);
			QSettings settings("HHMI", "Visiocyte");
            QPoint windowPosition = settings.value("pos", QPoint(10, 10)).toPoint();
            QSize windowSize = settings.value("size", QSize(1000, 700)).toSize();
            qMainWindow->move(windowPosition);
            qMainWindow->resize(windowSize);
            qMainWindow->show();
        }
    }

public:
    ~VisiocyteApplication();

    static bool hasMainWindow() {
        if (mainWindow==0) {
            return false;
        }
        return true;
    }

    static void handleCloseEvent(QCloseEvent* event) {
        mainWindow->handleCoordinatedCloseEvent(event);

#ifdef _ALLOW_WORKMODE_MENU_
        if (naMainWindow!=0) {
            naMainWindow->handleCoordinatedCloseEvent(event);
        }
#endif

        QCoreApplication::postEvent(theApp, new QEvent(QEvent::Quit)); // this more OK
    }

    static MainWindow* getMainWindow() {
        return mainWindow;
    }

#ifdef _ALLOW_WORKMODE_MENU_
    static NaMainWindow* getNaMainWindow() {
        return naMainWindow;
    }
#endif

    static void activateMainWindow() {
        if (mainWindowIsActive==false) {
            activateMainWindowHelper(mainWindow);
            mainWindowIsActive=true;
        }

#ifdef _ALLOW_WORKMODE_MENU_
        if (naMainWindow!=0) {
            naMainWindow->setVISIOCYTEDefaultModeCheck(true);
        }
        mainWindow->setVISIOCYTEDefaultModeCheck(true);
#endif
    }

    static void deactivateMainWindow() {
        if (mainWindowIsActive==true) {
            deactivateMainWindowHelper(mainWindow);
            mainWindowIsActive=false;
        }
#ifdef _ALLOW_WORKMODE_MENU_
        mainWindow->setVISIOCYTEDefaultModeCheck(false);
        if (naMainWindow!=0) {
            naMainWindow->setVISIOCYTEDefaultModeCheck(false);
        }
#endif
    }

#ifdef _ALLOW_WORKMODE_MENU_
    static void activateNaMainWindow() {
        if (naMainWindowIsActive==false) {
            if (naMainWindow==0) {
                naMainWindow = new NaMainWindow();
                // Allow NeuronAnnotator to activate file load in default window
                connect(naMainWindow, SIGNAL(defaultVisiocyteFileLoadRequested(QString)),
                        mainWindow, SLOT(loadVISIOCYTEFile(QString)));
                connect(naMainWindow, SIGNAL(defaultVisiocyteUrlLoadRequested(QUrl)),
                        mainWindow, SLOT(loadVISIOCYTEUrl(QUrl)));
            }
            activateMainWindowHelper(naMainWindow);
            naMainWindowIsActive=true;
        }
        naMainWindow->setNeuronAnnotatorModeCheck(true);
        mainWindow->setNeuronAnnotatorModeCheck(true);
    }

    static void deactivateNaMainWindow() {
        if (naMainWindowIsActive==true) {
            deactivateMainWindowHelper(naMainWindow);
            naMainWindowIsActive=false;
        }
        if (naMainWindow!=0) {
            naMainWindow->setNeuronAnnotatorModeCheck(false);
        }
        mainWindow->setNeuronAnnotatorModeCheck(false);
    }
#endif

    static VisiocyteApplication* getInstance(int & argc, char ** argv) {
        if (theApp==0) {
            theApp = new VisiocyteApplication(argc, argv);
        }
        return theApp;
    }
    static VisiocyteApplication* getInstance() {
        int a=0;
        char ** p=0;
        if (theApp==0) {
            theApp = new VisiocyteApplication(a, p);
        }
        return theApp;
    }

signals:

public slots:


};


#endif // VISIOCYTE_APPLICATION_H
