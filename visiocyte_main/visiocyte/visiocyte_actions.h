#ifndef VISIOCYTE_ACTIONS_H_
#define VISIOCYTE_ACTIONS_H_

// The purpose of this file is to decouple VISIOCYTE action objects from
// the heavily weighted VISIOCYTE MainWindow object.
// There remains a lot of work to do to complete this separation.

#include <QAction>
#include <QUrl>

namespace visiocyte {

/// Opens a web page in the user's web browser
class BrowseToWebPageAction : public QAction
{
    Q_OBJECT
public:
    BrowseToWebPageAction(const QString& text, const QUrl& url, QObject* parent);
signals:
    void page_open_failed();
private slots:
    void openWebPage();
private:
    QUrl _url;
};


/// Opens the main VISIOCYTE web page in the user's web browser
class OpenVisiocyteWebPageAction : public BrowseToWebPageAction
{
public:
    OpenVisiocyteWebPageAction(QObject* parent);
};


/// Show the "About VISIOCYTE" dialog
class ShowVisiocyteAboutDialogAction : public QAction
{
    Q_OBJECT
public:
    ShowVisiocyteAboutDialogAction(QWidget* parent);
private slots:
    void show_dialog();
};


} // namespace visiocyte

#endif /* VISIOCYTE_ACTIONS_H_ */
