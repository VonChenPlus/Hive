#include "LogoLayer.h"
#include "../UI/Background.h"
#include "../UI/UIAtlas.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UI/CONTROLS/UILabel.h"
#include "GRAPH/UNITY3D/TextureAtlas.h"
#include "GRAPH/UNITY3D/Renderer.h"
#include "GRAPH/UNITY3D/ShaderCache.h"
#include "GRAPH/UNITY3D/TextureCache.h"
#include "MATH/Size.h"
#include <QString>

extern const AtlasImage *getUIAtlas();
extern const uint8 *getUIAtlasData(uint64 &size);
namespace GRAPH
{
    extern HData StringToTexture(void *loaderOwner, const char * text, uint32 &width, uint32 &height, bool& hasPremultipliedAlpha);
}

LogoLayer::~LogoLayer() {
    SAFE_RELEASE(uiLabel_);
    SAFE_RELEASE(uiAtlas_);
}

bool LogoLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    frames_ = 0;

    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();
    MATH::Sizef center(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

    uiLabel_ = GRAPH::UI::Label::createWithCustomLoader("Created by Feng Chen", GRAPH::StringToTexture, nullptr);
    uiLabel_->retain();
    // position the label on the center of the screen
    uiLabel_->setPosition(center.width,
        center.height - 40);
    uiLabel_->updateDisplayedOpacity(0.0f);
    this->addChild(uiLabel_, 1);

    IMAGE::ImageObject *image = new IMAGE::ImageObject();
    uint64 uiAtlasSize = 0;
    const uint8 *uiAtlas = getUIAtlasData(uiAtlasSize);
    image->initWithImageData(uiAtlas, uiAtlasSize);
    setU3DShader(GRAPH::ShaderCache::getInstance().getU3DShader(GRAPH::Unity3DShader::SHADER_NAME_POSITION_TEXTURE_COLOR));
    uiAtlas_ = GRAPH::TextureAtlas::createWithTexture(GRAPH::TextureCache::getInstance().addImage(image, "UIAtlas"), 2 * 4);
    uiAtlas_->retain();
    SAFE_RELEASE(image);

    auto background = new Background();
    background->init();
    this->addChild(background);

    scheduleUpdate();

    return true;
}

void LogoLayer::draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) {
    customCommand_.init(globalZOrder_);
    customCommand_.func = std::bind(&LogoLayer::onDraw, this, transform, flags);
    renderer->addCommand(&customCommand_);
}

void LogoLayer::update(float) {
    frames_++;
    frames_ = MATH::MATH_MIN(180, frames_);

    float rate = (float) frames_ / 60.0f;
    float alpha = rate;
    if (rate > 1.0f)
        alpha = 1.0f;
    if (rate > 2.0f)
        alpha = 3.0f - rate;

    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();
    MATH::Sizef center(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    GRAPH::Color4B color = 0xFFFFFFFF;
    color = GRAPH::Color4B::ColorAlpha(color, alpha);

    uiLabel_->updateDisplayedOpacity(color.alpha);

    GRAPH::V3F_C4B_T2F_Quad quads [] =
    {
        {
            { MATH::Vector3f(center.width - 130, center.height - 20, 0), color, GRAPH::Tex2F(getUIAtlas()[I_ICON].u1, getUIAtlas()[I_ICON].v2), },
            { MATH::Vector3f(center.width - 130, center.height + 60, 0), color, GRAPH::Tex2F(getUIAtlas()[I_ICON].u1, getUIAtlas()[I_ICON].v1), },
            { MATH::Vector3f(center.width - 50, center.height - 20, 0), color, GRAPH::Tex2F(getUIAtlas()[I_ICON].u2, getUIAtlas()[I_ICON].v2), },
            { MATH::Vector3f(center.width - 50, center.height + 60, 0), color, GRAPH::Tex2F(getUIAtlas()[I_ICON].u2, getUIAtlas()[I_ICON].v1), },
        },
        {
            { MATH::Vector3f(center.width - 50, center.height - 20, 0), color, GRAPH::Tex2F(getUIAtlas()[I_LOGO].u1, getUIAtlas()[I_LOGO].v2), },
            { MATH::Vector3f(center.width - 50, center.height + 23, 0), color, GRAPH::Tex2F(getUIAtlas()[I_LOGO].u1, getUIAtlas()[I_LOGO].v1), },
            { MATH::Vector3f(center.width + 78, center.height - 20, 0), color, GRAPH::Tex2F(getUIAtlas()[I_LOGO].u2, getUIAtlas()[I_LOGO].v2), },
            { MATH::Vector3f(center.width + 78, center.height + 23, 0), color, GRAPH::Tex2F(getUIAtlas()[I_LOGO].u2, getUIAtlas()[I_LOGO].v1), },
        },
    };

    for (uint64 index = 0; index < sizeof(quads) / sizeof(GRAPH::V3F_C4B_T2F_Quad); ++index) {
        uiAtlas_->updateQuad(&quads[index], index);
    }
}

void LogoLayer::onDraw(const MATH::Matrix4& transform, uint32_t) {
    getU3DShader()->apply();
    getU3DShader()->setUniformsForBuiltins(transform);
    uiAtlas_->drawQuads();
}

GRAPH::Scene* LogoLayer::scene() {
    // 'scene' is an autorelease object
    auto scene = GRAPH::Scene::create();

    // 'layer' is an autorelease object
    LogoLayer *layer = LogoLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

LogoLayer* LogoLayer::create() {
    LogoLayer *pRet = new(std::nothrow) LogoLayer(); \
        if (pRet && pRet->init()) {
            pRet->autorelease();
            return pRet;
        }
        else {
            delete pRet;
            return nullptr;
        }
}
