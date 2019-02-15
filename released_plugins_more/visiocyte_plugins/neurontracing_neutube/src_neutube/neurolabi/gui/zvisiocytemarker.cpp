#include "zvisiocytemarker.h"
#include <sstream>
#include <string.h>

#include "tz_error.h"

using namespace std;

ZVisiocyteMarker::ZVisiocyteMarker()
{
  m_r = 1.0;
  m_type = 1;
  m_color[0] = 0;
  m_color[1] = 0;
  m_color[2] = 0;
}

void ZVisiocyteMarker::adjustForBaseOne()
{
  m_center += 1.0;
}

void ZVisiocyteMarker::moveToSurface(Swc_Tree *tree)
{
  Swc_Tree_Node *tn = NULL;
  Swc_Tree_Point_Dist_N(tree, x(), y(), z(), &tn);

  m_center -= ZPoint(tn->node.x, tn->node.y, tn->node.z);
  m_center.normalize();
  m_center *= tn->node.d;

  /*
  if (tn->node.id == 66){
    cout << "debug here" << endl;
  }
  */

  m_center += ZPoint(tn->node.x, tn->node.y, tn->node.z);
}

string ZVisiocyteMarker::toString()
{
  ostringstream stream;

  stream << x() << "," << y() << "," << z() << "," << radius() << ","
         << m_type << "," << m_name << "," << m_comment << ","
         << static_cast<int>(m_color[0]) << ","
         << static_cast<int>(m_color[1]) << ","
         << static_cast<int>(m_color[2]) << ",";

  return stream.str();
}

void ZVisiocyteMarker::set(const string &line)
{
  TZ_ERROR(ERROR_PART_FUNC);
  char *str = strdup(line.c_str());

  free(str);

}
