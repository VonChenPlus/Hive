#include "Background.h"
#include "../UI/UIAtlas.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UNITY3D/GLShader.h"
#include "GRAPH/UNITY3D/GLTexture.h"
#include "GRAPH/UNITY3D/Renderer.h"

extern const AtlasImage *getUIAtlas();
extern const uint8 *getUIAtlasData(uint64 &size);

bool Background::init() {
    if (!Node::init()) {
        return false;
    }

    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();
    MATH::Sizef center(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

    setGLShader(GRAPH::GLShaderCache::getInstance().getGLShader(GRAPH::GLShader::SHADER_NAME_POSITION_TEXTURE_COLOR));
    uiAtlas_ = GRAPH::TextureAtlas::createWithTexture(GRAPH::Director::getInstance().getTextureCache()->getTextureForKey("UIAtlas"), 101 * 4);
    uiAtlas_->retain();

    GRAPH::Color4B color = 0xFFFFFFFF;
    GRAPH::V3F_C4B_T2F_Quad quads [] =
    {
        {
            { MATH::Vector3f(origin.x, origin.y, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u1, getUIAtlas()[I_BG].v2), },
            { MATH::Vector3f(origin.x, origin.y + visibleSize.height, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u1, getUIAtlas()[I_BG].v1), },
            { MATH::Vector3f(origin.x + visibleSize.width, origin.y, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u2, getUIAtlas()[I_BG].v2), },
            { MATH::Vector3f(origin.x + visibleSize.width, origin.y + visibleSize.height, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u2, getUIAtlas()[I_BG].v1), },
        },
    };

    uiAtlas_->updateQuad(&quads[0], 0);

    scheduleUpdate();

    return true;
}

void Background::draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) {
    customCommand_.init(_globalZOrder - 1);
    customCommand_.func = std::bind(&Background::onDraw, this, transform, flags);
    renderer->addCommand(&customCommand_);
}

void Background::update(float delta) {
    
}

void Background::onDraw(const MATH::Matrix4& transform, uint32_t flags) {
    getGLShader()->use();
    getGLShader()->setUniformsForBuiltins(transform);
    uiAtlas_->drawQuads();
}