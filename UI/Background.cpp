#include "Background.h"
#include "../UI/UIAtlas.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UNITY3D/ShaderCache.h"
#include "GRAPH/UNITY3D/TextureCache.h"
#include "GRAPH/UNITY3D/TextureAtlas.h"
#include "GRAPH/UNITY3D/Renderer.h"
#include "UTILS/TIME/TimeUtils.h"
#include "UTILS/RANDOM/RandomUtils.h"

extern const AtlasImage *getUIAtlas();
extern const uint8 *getUIAtlasData(uint64 &size);

bool Background::init() {
    if (!Node::init()) {
        return false;
    }

    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();

    setU3DShader(GRAPH::ShaderCache::getInstance().getU3DShader(GRAPH::Unity3DShader::SHADER_NAME_POSITION_TEXTURE_COLOR));
    uiAtlas_ = GRAPH::TextureAtlas::createWithTexture(GRAPH::TextureCache::getInstance().getTextureForKey("UIAtlas"), 101 * 4);
    uiAtlas_->retain();

    GRAPH::Color4B color = 0xFFFFFFFF;
    GRAPH::V3F_C4B_T2F_Quad quads =
    {
        { MATH::Vector3f(origin.x, origin.y, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u1, getUIAtlas()[I_BG].v2), },
        { MATH::Vector3f(origin.x, origin.y + visibleSize.height, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u1, getUIAtlas()[I_BG].v1), },
        { MATH::Vector3f(origin.x + visibleSize.width, origin.y, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u2, getUIAtlas()[I_BG].v2), },
        { MATH::Vector3f(origin.x + visibleSize.width, origin.y + visibleSize.height, 0), 0xFFFFFFFF, GRAPH::Tex2F(getUIAtlas()[I_BG].u2, getUIAtlas()[I_BG].v1), },
    };

    uiAtlas_->updateQuad(&quads, 0);

    scheduleUpdate();

    return true;
}

void Background::draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) {
    customCommand_.init(globalZOrder_ - 1);
    customCommand_.func = std::bind(&Background::onDraw, this, transform, flags);
    renderer->addCommand(&customCommand_);
}

static inline void Rotated(float *v, float angle, float xc, float yc) {
    const float x = v[0] - xc;
    const float y = v[1] - yc;
    const float sa = sinf(angle);
    const float ca = cosf(angle);
    v[0] = x * ca + y * -sa + xc;
    v[1] = x * sa + y *  ca + yc;
}

void Background::update(float) {
    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();

    static float xbase[100] = { 0 };
    static float ybase[100] = { 0 };
    static int last_xres = 0;
    static int last_yres = 0;

    if (xbase[0] == 0.0f || last_xres != visibleSize.width || last_yres != visibleSize.height) {
        UTILS::RANDOM::GMRandom rng;
        for (int i = 0; i < 100; i++) {
            xbase[i] = rng.randFloat() * visibleSize.width;
            ybase[i] = rng.randFloat() * visibleSize.height;
        }
        last_xres = visibleSize.width;
        last_yres = visibleSize.height;
    }

    static const int symbols[4] = {
        I_H,
        I_I,
        I_V,
        I_E
    };
    static const GRAPH::Color4B colors[4] = {
        0xC0FFFFFF,
        0xC0FFFFFF,
        0xC0FFFFFF,
        0xC0FFFFFF,
    };

    auto curTime = UTILS::TIME::FetchCurrentTime();
    for (int i = 0; i < 100; i++) {
        float x = xbase[i] + origin.x;
        float y = ybase[i] + origin.y + 40 * cosf(i * 7.2f + curTime * 1.3f);
        float angle = sinf(i + curTime);
        int n = i & 3;
        float width = (float) getUIAtlas()[symbols[n]].w * 1.2;
        float height = (float) getUIAtlas()[symbols[n]].h * 1.2;
        GRAPH::Color4B color = GRAPH::Color4B::ColorAlpha(colors[n], 0.1f);
        float _v[4][2] = {
            { x - width * 0.5, y - height * 0.5 },
            { x - width * 0.5, y + height * 0.5 },
            { x + width * 0.5, y - height * 0.5 },
            { x + width * 0.5, y + height * 0.5 }, };
        for (auto &v : _v) {
            Rotated(v, angle, x, y);
        }

        GRAPH::V3F_C4B_T2F_Quad quads =
        {
            { MATH::Vector3f(_v[0][0], _v[0][1], 0), color, GRAPH::Tex2F(getUIAtlas()[symbols[n]].u1, getUIAtlas()[symbols[n]].v2), },
            { MATH::Vector3f(_v[1][0], _v[1][1], 0), color, GRAPH::Tex2F(getUIAtlas()[symbols[n]].u1, getUIAtlas()[symbols[n]].v1), },
            { MATH::Vector3f(_v[2][0], _v[2][1], 0), color, GRAPH::Tex2F(getUIAtlas()[symbols[n]].u2, getUIAtlas()[symbols[n]].v2), },
            { MATH::Vector3f(_v[3][0], _v[3][1], 0), color, GRAPH::Tex2F(getUIAtlas()[symbols[n]].u2, getUIAtlas()[symbols[n]].v1), },
        };
        uiAtlas_->updateQuad(&quads, i + 1);
    }
}

void Background::onDraw(const MATH::Matrix4& transform, uint32_t) {
    getU3DShader()->apply();
    getU3DShader()->setUniformsForBuiltins(transform);
    uiAtlas_->drawQuads();
}
