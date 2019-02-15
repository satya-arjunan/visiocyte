#ifndef ZMOVIECAMERA_H
#define ZMOVIECAMERA_H

#include "z3drotation.h"
#include "zpoint.h"
#include "zjsonobject.h"

class Z3DWindow;

/**
 * @brief The ZMovieCamera class
 *
 * ZMovieCamera controls the movement of a camera, which has two controlling
 * points: world position (center) and eye position (eye). The vector from the
 * eye to the center defines the view vector. Rotation only applies on the eye
 * position.
 */
class ZMovieCamera
{
public:
  ZMovieCamera();

  enum EControlPoint {
    EYE, CENTER, UP_VECTOR
  };

  enum EMovingDirection {
    DEFINED_AXIS, VIEW_AXIS
  };

  void loadJsonObject(const ZJsonObject &obj);
  void print() const;

  void reset(Z3DWindow *stage);

  inline const Z3DRotation& getRotation() const { return m_rotation; }
  double getMovingSpeed(EControlPoint ref) const;
  const ZPoint& getMovingVelocity(EControlPoint ref) const;
  EMovingDirection getMovingDirection(EControlPoint ref) const;
  bool hasReset(EControlPoint ref) const;

private:
  Z3DRotation m_rotation;

  EMovingDirection m_movingDirection[2];
  ZPoint m_movingSpeed[3];
  bool m_hasReset[3];
  ZPoint m_resetPosition[3];
};

#endif // ZMOVIECAMERA_H
