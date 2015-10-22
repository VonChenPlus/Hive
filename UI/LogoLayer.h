#ifndef LOGOLAYER_H
#define LOGOLAYER_H

#include "GRAPH/Scene.h"
#include "GRAPH/UI/UILayer.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UNITY3D/RenderCommand.h"

class GRAPH::TextureAtlas;

class LogoLayer : public GRAPH::UI::Layer
{
public:
    virtual bool init() override;
    virtual void draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) override;

    static GRAPH::Scene* scene();
    static LogoLayer* create();

protected:
    void onDraw(const MATH::Matrix4& transform, uint32_t flags);

private:
    GRAPH::TextureAtlas *uiAtlas_;
    GRAPH::CustomCommand customCommand_;
};

#endif // LOGOLAYER_H
