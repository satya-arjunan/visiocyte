#ifndef __PLUGIN_CREATOR_GUI_H__
#define __PLUGIN_CREATOR_GUI_H__

#include <string>
#include <QtGui>
#include "visiocyte_message.h"
#include "plugin_creator_func.h"

using namespace std;

class GuidingDialog : public QDialog
{
	Q_OBJECT

public:
	GuidingDialog(QWidget * parent) : QDialog(parent)
	{
		label_plugin_name = new QLabel(tr("Plugin Name :"));
		editor_plugin_name = new QLineEdit(tr("test"));

		label_plugin_class = new QLabel(tr("Plugin Class Name :"));
		editor_plugin_class = new QLineEdit(tr("TestPlugin"));

		label_win_title = new QLabel(tr("Window Title :"));
		editor_win_title = new QLineEdit(tr("This is Test Plugin"));

		label_plugin_description = new QLabel(tr("Plugin Description :"));
		editor_plugin_description = new QLineEdit(tr("This is a test plugin, you can use it as a demo."));

		label_plugin_date = new QLabel(tr("Plugin Date :"));
        QDate date = QDate::currentDate();
        QString date_format = QString("%1-%2-%3").arg(date.year()).arg(date.month()).arg(date.day());
        editor_plugin_date = new QLineEdit(date_format);

		label_plugin_author = new QLabel(tr("Plugin Author :"));
		editor_plugin_author = new QLineEdit(tr("YourName"));

		label_visiocyte_path = new QLabel(tr("Visiocyte whole-project path :"));
		editor_visiocyte_path = new QLineEdit(tr(""));
        editor_visiocyte_path->setText(getVisiocytePath());
		button_visiocyte_path = new QPushButton(tr("..."));

        label_menu_list = new QLabel(tr("Menu List : "));
		editor_menu_list = new QLineEdit(tr("menu1 menu2"));

		label_func_list = new QLabel(tr("Func List : "));
		editor_func_list = new QLineEdit(tr("func1 func2"));

		label_save_folder = new QLabel(tr("Save Folder : "));
		editor_save_folder = new QLineEdit(tr(""));
		button_save_folder = new QPushButton(tr("..."));
        button_save_folder->setBackgroundRole(QPalette::Highlight);

		ok = new QPushButton(tr("ok"));
		cancel = new QPushButton(tr("cancel"));

		gridLayout = new QGridLayout();
		gridLayout->addWidget(label_plugin_name,         0, 0, 1, 1);
		gridLayout->addWidget(editor_plugin_name,        0, 1, 1, 9);
		gridLayout->addWidget(label_plugin_class,        1, 0, 1, 1);
		gridLayout->addWidget(editor_plugin_class,       1, 1, 1, 9);
		gridLayout->addWidget(label_win_title,           2, 0, 1, 1);
		gridLayout->addWidget(editor_win_title,          2, 1, 1, 9);
		gridLayout->addWidget(label_plugin_description,  3, 0, 1, 1);
		gridLayout->addWidget(editor_plugin_description, 3, 1, 1, 9);
		gridLayout->addWidget(label_plugin_date,         4, 0, 1, 1);
		gridLayout->addWidget(editor_plugin_date,        4, 1, 1, 9);
		gridLayout->addWidget(label_plugin_author,       5, 0, 1, 1);
		gridLayout->addWidget(editor_plugin_author,      5, 1, 1, 9);
		gridLayout->addWidget(label_visiocyte_path,       6, 0, 1, 1);
		gridLayout->addWidget(editor_visiocyte_path,      6, 1, 1, 8);
		gridLayout->addWidget(button_visiocyte_path,      6, 9, 1, 1);
		gridLayout->addWidget(label_menu_list,           7, 0, 1, 1);
		gridLayout->addWidget(editor_menu_list,          7, 1, 1, 9);
		gridLayout->addWidget(label_func_list,           8, 0, 1, 1);
		gridLayout->addWidget(editor_func_list,          8, 1, 1, 9);
		gridLayout->addWidget(label_save_folder,         9, 0, 1, 1);
		gridLayout->addWidget(editor_save_folder,        9, 1, 1, 8);
		gridLayout->addWidget(button_save_folder,        9, 9, 1, 1);
		gridLayout->addWidget(cancel, 10, 0, 1, 5, Qt::AlignRight);
		gridLayout->addWidget(ok, 10, 5, 1, 5, Qt::AlignRight);

		connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
		connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
		connect(button_visiocyte_path, SIGNAL(clicked()), this, SLOT(setFolderPath()));
		connect(button_save_folder, SIGNAL(clicked()), this, SLOT(setFolderPath()));

		connect(ok, SIGNAL(clicked()), this, SLOT(update()));

		this->setLayout(gridLayout);
		this->setWindowTitle("Plugin Creator Dialog");

	}

	~GuidingDialog(){}

public slots:
	void accept()
	{
		return QDialog::accept();
	}

	void reject()
	{
		return QDialog::reject();
	}

	void setFolderPath()
	{
		QPushButton * button = (QPushButton*) sender();
		QLineEdit * editor;
		QString title;
		if(button == button_visiocyte_path) 
		{
			editor = editor_visiocyte_path;
			title = "Open VISIOCYTE_MAIN Directory";
		}
		else if(button == button_save_folder) 
		{
			editor = editor_save_folder;
			title = "Save to Directory";
		}
        QString current_path = editor->text();

        QString dir = QFileDialog::getExistingDirectory(this, title,
                                                 "~/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
        if(dir.isEmpty())
            editor->setText(current_path);
        else
            editor->setText(dir);
	}

	void update()
	{
        if(editor_save_folder->text().isEmpty() || editor_visiocyte_path->text().isEmpty())
        {
            visiocyte_msg("Please select the correct path!");
            return QDialog::reject();
        }

        plugin_name =  editor_plugin_name->text().toStdString();
		plugin_class =  editor_plugin_class->text().toStdString();
		win_title =  editor_win_title->text().toStdString();
		plugin_desp =  editor_plugin_description->text().toStdString();
		plugin_date =  editor_plugin_date->text().toStdString();
		plugin_author =  editor_plugin_author->text().toStdString();
		visiocyte_path =  editor_visiocyte_path->text().toStdString();
		menulst =  editor_menu_list->text().toStdString();
		funclst =  editor_func_list->text().toStdString();
		save_folder =  editor_save_folder->text().toStdString();

	}

public:
	string plugin_name;
	string plugin_class;
	string win_title;
	string plugin_desp;
	string plugin_date;
	string plugin_author;
	string visiocyte_path;
	string menulst;
	string funclst;
	string save_folder;

	QLabel * label_plugin_name;
	QLineEdit * editor_plugin_name;

	QLabel * label_plugin_class;
	QLineEdit * editor_plugin_class;

	QLabel * label_win_title;
	QLineEdit * editor_win_title;

	QLabel * label_plugin_description;
	QLineEdit * editor_plugin_description;

	QLabel * label_plugin_date;
	QLineEdit * editor_plugin_date;

	QLabel * label_plugin_author;
	QLineEdit * editor_plugin_author;

	QLabel * label_visiocyte_path;
	QLineEdit * editor_visiocyte_path;
	QPushButton * button_visiocyte_path;

	QLabel * label_menu_list;
	QLineEdit * editor_menu_list;

	QLabel * label_func_list;
	QLineEdit * editor_func_list;

	QLabel * label_save_folder;
	QLineEdit * editor_save_folder;
	QPushButton * button_save_folder;

	QPushButton * ok;
	QPushButton * cancel;

	QGridLayout * gridLayout;

};

#endif
