//------------------------------------------------------------------------------------------------
// Copyright (c) 2012  Alessandro Bria and Giulio Iannello (University Campus Bio-Medico of Rome).  
// All rights reserved.
//------------------------------------------------------------------------------------------------

/*******************************************************************************************************************************************************************************************
*    LICENSE NOTICE
********************************************************************************************************************************************************************************************
*    By downloading/using/running/editing/changing any portion of codes in this package you agree to this license. If you do not agree to this license, do not download/use/run/edit/change
*    this code.
********************************************************************************************************************************************************************************************
*    1. This material is free for non-profit research, but needs a special license for any commercial purpose. Please contact Alessandro Bria at a.bria@unicas.it or Giulio Iannello at 
*       g.iannello@unicampus.it for further details.
*    2. You agree to appropriately cite this work in your related studies and publications.
*
*       Bria, A., et al., (2012) "Stitching Terabyte-sized 3D Images Acquired in Confocal Ultramicroscopy", Proceedings of the 9th IEEE International Symposium on Biomedical Imaging.
*       Bria, A., Iannello, G., "A Tool for Fast 3D Automatic Stitching of Teravoxel-sized Datasets", submitted on July 2012 to IEEE Transactions on Information Technology in Biomedicine.
*
*    3. This material is provided by  the copyright holders (Alessandro Bria  and  Giulio Iannello),  University Campus Bio-Medico and contributors "as is" and any express or implied war-
*       ranties, including, but  not limited to,  any implied warranties  of merchantability,  non-infringement, or fitness for a particular purpose are  disclaimed. In no event shall the
*       copyright owners, University Campus Bio-Medico, or contributors be liable for any direct, indirect, incidental, special, exemplary, or  consequential  damages  (including, but not 
*       limited to, procurement of substitute goods or services; loss of use, data, or profits;reasonable royalties; or business interruption) however caused  and on any theory of liabil-
*       ity, whether in contract, strict liability, or tort  (including negligence or otherwise) arising in any way out of the use of this software,  even if advised of the possibility of
*       such damage.
*    4. Neither the name of University  Campus Bio-Medico of Rome, nor Alessandro Bria and Giulio Iannello, may be used to endorse or  promote products  derived from this software without
*       specific prior written permission.
********************************************************************************************************************************************************************************************/

#ifndef PTABMERGETILES_H
#define PTABMERGETILES_H

#include <QWidget>
#include <QtGui>
#include "CTeraStitcher.h"
#include "QMyTabWidget.h"
#include "S_config.h"

#define N_MAX_RESOLUTIONS   6  // 2017-05-03. Giulio. Introduced a symbolic constant

class terastitcher::PTabMergeTiles : public QWidget
{
    Q_OBJECT

    /*********************************************************************************
    * Singleton design pattern: this class can have one instance only,  which must be
    * instantiated by calling static method "instance(...)"
    **********************************************************************************/
    static PTabMergeTiles* uniqueInstance;

#ifdef VISIOCYTE_TERASTITCHER
    PTabMergeTiles(QMyTabWidget* _container, int _tab_index, VISIOCYTEPluginCallback* _VISIOCYTE_env);
#else
	PTabMergeTiles(QMyTabWidget* _container, int _tab_index);
#endif

    //members
    QMyTabWidget* container;        //tabs container
    int tab_index;                  //tab index
	static const int n_max_resolutions = N_MAX_RESOLUTIONS; // maximum number of resolutions
#ifdef VISIOCYTE_TERASTITCHER
    VISIOCYTEPluginCallback* VISIOCYTE_env;
#endif

    //basic settings panel widgets
    QWidget* basic_panel;
    QLabel* savedir_label;
    QLineEdit* savedir_field;
	QPushButton *outDirButton;			//browse for volume's output directory button

	QPushButton *outFileButton;			//browse for volume's output file button
    QLabel* resolutions_label;
    QLabel* resolutions_size_label;
    QLabel* resolutions_save_label;
    QLabel* outputs_label;
    QLabel* resolutions_fields[S_MAX_MULTIRES];
    QLabel* resolutions_sizes[S_MAX_MULTIRES];
    QButtonGroup* resolutions_save_selection;
	QCheckBox* resolutions_save_cboxs[S_MAX_MULTIRES];
    QButtonGroup* volumeformat_selection;
    QLabel* volumeformat_label;
    QComboBox* vol_format_cbox;
    QSpinBox* block_height_field;
    QSpinBox* block_width_field;
    QSpinBox* block_depth_field;
    QLineEdit* memocc_field;
    QPushButton* showAdvancedButton;
	QCheckBox *libtiff_uncompressed_checkbox;
	QCheckBox *libtiff_bigtiff_checkbox;
	QCheckBox *isotropic_checkbox;
	QLineEdit *mdata_line;
	QPushButton *mdata_browse_button;
	QLineEdit *external_compressor_line;


    //advanced settings panel widgets
    QWidget* advanced_panel;
    QSpinBox *y0_field, *y1_field, *x0_field, *x1_field;
    QSpinBox *z0_field, *z1_field;
    QLabel* blendingalgo_label;
    QComboBox* blendingalbo_cbox;
    QLabel* restoreSPIM_label;
    QComboBox* restoreSPIM_cbox;
    //QComboBox* imgdepth_cbox;
    QComboBox* channel_selection;


    //other widgets
    QMovie *wait_movie;             //animated wait GIF icon
    QLabel *wait_label;             //label containing animated wait GIF icon

public:

    /**********************************************************************************
    * Singleton design pattern: this class can have one instance only,  which must be
    * instantiated by calling static method "istance(...)"
    ***********************************************************************************/
#ifdef VISIOCYTE_TERASTITCHER
    static PTabMergeTiles* instance(QMyTabWidget* _container, int _tab_index, VISIOCYTEPluginCallback* _VISIOCYTE_env)
	{
		if (uniqueInstance == 0)
			uniqueInstance = new PTabMergeTiles(_container, _tab_index, _VISIOCYTE_env);
		return uniqueInstance;
	}
#else
	static PTabMergeTiles* instance(QMyTabWidget* _container, int _tab_index)
	{
		if (uniqueInstance == 0)
			uniqueInstance = new PTabMergeTiles(_container, _tab_index);
		return uniqueInstance;
	}
#endif
    static PTabMergeTiles* getInstance(){return uniqueInstance;}
    static void uninstance();
    ~PTabMergeTiles();

    /**********************************************************************************
    * Start/Stop methods associated to the current step.
    * They are called by the startButtonClicked/stopButtonClicked methods of <PTeraStitcher>
    ***********************************************************************************/
    void start();
    void stop();

    //reset method
    void reset();

    /**********************************************************************************
    * Overrides QWidget's setEnabled(bool).
    * If the widget is enabled, its fields are filled with the informations provided by
    * the <StackedVolume> object of <CImport> instance.
    ***********************************************************************************/
    void setEnabled(bool enabled);

    //gives PTeraStitcher and CMergeTiles instances public access to this class members
    friend class PTeraStitcher;
    friend class terastitcher::CMergeTiles;

public slots:

    /**********************************************************************************
    * Opens the dialog to select the directory where the stitched volume has to be saved.
    * Called when user clicks on "browse_button".
    ***********************************************************************************/
    void browse_button_clicked();

    /**********************************************************************************
    * Called when <multistack_cbox> or <multistack_cbox> state changed.
    ***********************************************************************************/
    void volumeformat_changed(QString);

    /**********************************************************************************
    * Updates widgets contents
    ***********************************************************************************/
    void updateContent();

    /**********************************************************************************
    * Called when <showAdvancedButton> status changed
    ***********************************************************************************/
    void showAdvancedChanged(bool status);

    /**********************************************************************************
    * Called when the corresponding spinboxes changed.
    * New maximum/minimum values are set according to the status of spinboxes.
    ***********************************************************************************/
    void y0_field_changed(int val);
    void y1_field_changed(int val);
    void x0_field_changed(int val);
    void x1_field_changed(int val);
    void z0_field_changed(int val);
    void z1_field_changed(int val);

	/**********************************************************************************
    * Called when the checkboxes of the channel QComboBox's QStandardItemModel changed.
    ***********************************************************************************/
	void channelSelectionChanged(QStandardItem * item);



    /**********************************************************************************
    * Called by <CMergeTiles> when the associated operation has been performed.
    * If an exception has occurred in the <CMergeTiles> thread,it is propagated and man-
    * aged in the current thread (ex != 0). Otherwise, if a valid  3D image  is passed,
    * it is shown in Visiocyte.
    ***********************************************************************************/
	void merging_done(iom::exception *ex);

	/**********************************************************************************
    * Opens the dialog to select the image metadata file
    * Called when user clicks on "mdata_browse_button".
    ***********************************************************************************/
    void metadata_browse_button_clicked();

	// class members
public:

	static std::string metadata_path_null;
	static std::string external_compressor_line_null;

};

#endif // PTABMERGETILES_H
