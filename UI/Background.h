#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H

#include "GRAPH/Node.h"
#include "GRAPH/UNITY3D/RenderCommand.h"

class Background : public GRAPH::Node
{
public:
    ~Background();
    virtual bool init() override;
    virtual void draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) override;
    virtual void update(float delta) override;

protected:
    void onDraw(const MATH::Matrix4& transform, uint32_t flags);

private:
    GRAPH::TextureAtlas *uiAtlas_;
    GRAPH::CustomCommand customCommand_;
};

#endif // BACKGROUNDLAYER_H
