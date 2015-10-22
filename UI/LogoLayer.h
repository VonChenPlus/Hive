#ifndef LOGOLAYER_H
#define LOGOLAYER_H

#include "GRAPH/Scene.h"
#include "GRAPH/UI/UILayer.h"
#include "GRAPH/UNITY3D/RenderCommand.h"

namespace GRAPH
{
    class GLTextureAtlas;
    namespace UI
    {
        class Label;
    }
}

class LogoLayer : public GRAPH::UI::Layer
{
public:
    virtual bool init() override;
    virtual void draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) override;
    virtual void update(float delta) override;

    static GRAPH::Scene* scene();
    static LogoLayer* create();

protected:
    virtual ~LogoLayer();
    void onDraw(const MATH::Matrix4& transform, uint32_t flags);


private:
    GRAPH::GLTextureAtlas *uiAtlas_;
    GRAPH::CustomCommand customCommand_;
    GRAPH::UI::Label *uiLabel_;
    int frames_;
};

#endif // LOGOLAYER_H
