#ifndef LOGOLAYER_H
#define LOGOLAYER_H

#include "GRAPH/Scene.h"
#include "GRAPH/UI/Layer.h"

class GRAPH::TextureAtlas;

class LogoLayer : public GRAPH::Layer
{
public:
    virtual bool init() override;

    static GRAPH::Scene* scene();
    static LogoLayer* create();

private:
    GRAPH::TextureAtlas *uiAtlas_;
};

#endif // LOGOLAYER_H
