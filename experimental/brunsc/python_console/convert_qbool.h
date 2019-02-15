#ifndef VISIOCYTE_PYTHON_CONVERT_QBOOL_H_
#define VISIOCYTE_PYTHON_CONVERT_QBOOL_H_

/*
 * convert_qbool.h
 *
 *  Created on: Dec 23, 2010
 *      Author: cmbruns
 *
 *  Interconvert between QString and python string.
 *  Warning: use of this technique might be incompatible with
 *  PyQt or PySide
 */

// Call this from within BOOST_PYTHON_MODULE block
void register_qbool_conversion();

#endif /* VISIOCYTE_PYTHON_CONVERT_QBOOL_H_ */
