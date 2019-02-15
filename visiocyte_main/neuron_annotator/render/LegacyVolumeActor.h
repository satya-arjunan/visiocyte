/*
 * LegacyVolumeActor.h
 *
 *  Created on: Nov 14, 2012
 *      Author: Christopher M. Bruns
 */

#ifndef LEGACYVOLUMEACTOR_H_
#define LEGACYVOLUMEACTOR_H_

#include "ActorGL.h"
#include "boost/shared_ptr.hpp"
#include "../gui/RendererNeuronAnnotator.h"

/**
 * Encapsulates traditional Visiocyte Renderer class volume rendering
 * into an ActorGL.
 */
class LegacyVolumeActor : public ActorGL
{
public:
    typedef boost::shared_ptr<RendererNeuronAnnotator> RendererPtr;

    LegacyVolumeActor(RendererPtr renderer);
    virtual ~LegacyVolumeActor();
    virtual void initGL();
    virtual void paintGL();

protected:
    RendererPtr renderer;
};

#endif /* LEGACYVOLUMEACTOR_H_ */
