#ifndef ZFILEPARSER_H
#define ZFILEPARSER_H

#include <map>
#include <string>
#include <vector>
#include "zvisiocytemarker.h"
#include "zvisiocyteapo.h"

namespace FlyEm {

class ZFileParser
{
public:
  ZFileParser();

public:
  static std::map<int, std::string> loadBodyList(std::string filePath,
                                                 std::string workDir = "");
  static bool writeVisiocyteMakerFile(std::string filePath,
                                  std::vector<ZVisiocyteMarker> markerArray);
  static bool writeVisiocyteApoFile(std::string filePath,
                                std::vector<ZVisiocyteApo> markerArray);

  static std::vector<ZVisiocyteMarker> readVisiocyteMarkerFile(
      const std::string &filePath);

  static std::string bodyNameToFileName(const std::string bodyName);
};

}

#endif // ZFILEPARSER_H
