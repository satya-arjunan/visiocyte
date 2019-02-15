#ifndef ZPUNCTUMIO_H
#define ZPUNCTUMIO_H

#include <QList>
#include "QsLog.h"
#include "zpunctum.h"
#include <QFile>

class ZPunctum;

class ZPunctumIO
{
public:
  ZPunctumIO();

  static QList<ZPunctum*> load(const QString &file);
  static bool save(const QString &filename, const QList<ZPunctum*> &puncta);
  // InputIterator should point to ZPunctum* type
  template <typename InputIterator>
  static bool save(const QString &filename, InputIterator begin, InputIterator end);

private:
  static void readVISIOCYTEApoFile(const QString &file, QList<ZPunctum*> &punctaList);
  template <typename InputIterator>
  static bool writeVISIOCYTEApoFile(const QString &file, InputIterator begin, InputIterator end);
  static void readVISIOCYTEMarkerFile(const QString &file, QList<ZPunctum*> &punctaList);
  static void readRavelerBookmarkFile(const QString &file, QList<ZPunctum*> &punctaList);
  static void punctaFprint(FILE *fp, const ZPunctum &p, int id);
};

// template
template <typename InputIterator>
bool ZPunctumIO::save(const QString &filename, InputIterator begin, InputIterator end)
{
  if (filename.endsWith(".apo", Qt::CaseInsensitive)) {
    return writeVISIOCYTEApoFile(filename, begin, end);
  } else {
    QString file = filename;
    file += ".apo";
    return writeVISIOCYTEApoFile(file, begin, end);
  }
}

template <typename InputIterator>
bool ZPunctumIO::writeVISIOCYTEApoFile(const QString &filename, InputIterator begin, InputIterator end)
{
  FILE *fp = fopen(filename.toLocal8Bit().data(), "w");
  if (fp != NULL) {
    int idx = 1;
    for (InputIterator it = begin; it != end; ++it) {
      punctaFprint(fp, *(*it), idx++);
    }
    fclose(fp);
    LINFO() << "Wrote puncta file" << filename;
    return true;
  } else {
    LERROR() << "Can not open" << filename << "to write puncta.";
    return false;
  }
}

#endif // ZPUNCTUMIO_H
