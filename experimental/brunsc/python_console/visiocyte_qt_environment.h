/*
 * visiocyte_qt_environment.h
 *
 *  Created on: Jan 11, 2011
 *      Author: brunsc
 */

#ifndef VISIOCYTE_QT_ENVIRONMENT_H_
#define VISIOCYTE_QT_ENVIRONMENT_H_

class QThread;
class QWidget;
class VISIOCYTEPluginCallback2;

namespace visiocyte {

    // Expose Qt symbols for use by python plugins
    QThread* get_qt_gui_thread();
    QWidget* get_qt_gui_parent();

    VISIOCYTEPluginCallback2* get_plugin_callback();

} // namespace visiocyte

#endif /* VISIOCYTE_QT_ENVIRONMENT_H_ */
