#ifndef ZFILETYPE_H
#define ZFILETYPE_H

#include <string>

class ZFileType
{
public:
  ZFileType();

  enum EFileType {
    UNIDENTIFIED_FILE, SWC_FILE, SWC_NETWORK_FILE,
    LOCSEG_CHAIN_FILE, TIFF_FILE, LSM_FILE, PNG_FILE, VISIOCYTE_RAW_FILE,
    SYNAPSE_ANNOTATON_FILE, FLYEM_NETWORK_FILE, XML_FILE, JSON_FILE,
    VISIOCYTE_APO_FILE, VISIOCYTE_MARKER_FILE,
    RAVELER_BOOKMARK, VISIOCYTE_PBD_FILE, MYERS_NSP_FILE, OBJECT_SCAN_FILE,
    JPG_FILE, DVID_OBJECT_FILE
  };

  static EFileType fileType(const std::string &filePath);
  static std::string typeName(EFileType type);

  static bool isImageFile(const std::string &filePath);
  static bool isObjectFile(const std::string &filePath);
  static bool isImageFile(EFileType type);
  static bool isObjectFile(EFileType type);
  static bool isNeutubeOpenable(EFileType type);
  static bool isNeutubeOpenable(const std::string &filePath);
};

#endif // ZFILETYPE_H
