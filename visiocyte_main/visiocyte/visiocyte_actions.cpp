#include "visiocyte_actions.h"

#include <QUrl>
#include <QDesktopServices>
#include <QTextEdit>
#include "visiocyte_version_info.h"

namespace visiocyte {

BrowseToWebPageAction::BrowseToWebPageAction(
        const QString& text,
        const QUrl& url,
        QObject* parent)
    : QAction(text, parent) , _url(url)
{
    connect(this, SIGNAL(triggered()),
            this, SLOT(openWebPage()));
}

void BrowseToWebPageAction::openWebPage() {
    bool success = QDesktopServices::openUrl(_url);
    if (! success )
        emit page_open_failed();
}


OpenVisiocyteWebPageAction::OpenVisiocyteWebPageAction(QObject* parent)
        : BrowseToWebPageAction(tr("Go to Visiocyte web site..."),
                QUrl("http://visiocyte.org"),
                parent)
{}


ShowVisiocyteAboutDialogAction::ShowVisiocyteAboutDialogAction(QWidget* parent)
        : QAction(QIcon(":/pic/help.png"), tr("Help Info and &About"), parent)
{
    setStatusTip(tr("Show help and version information"));
    connect(this, SIGNAL(triggered()), this, SLOT(show_dialog()));
}

void ShowVisiocyteAboutDialogAction::show_dialog() {
    visiocyte_aboutinfo();
}

} // namespace visiocyte
