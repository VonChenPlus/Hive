#include "LogoLayer.h"
#include "UI/UIAtlas.h"
#include "GRAPH/Director.h"
#include "GRAPH/UI/CONTROLS/Label.h"
#include "GRAPH/UNITY3D/GLTexture.h"
#include "GRAPH/UNITY3D/Renderer.h"
#include "GRAPH/UNITY3D/GLShader.h"

extern const AtlasImage *UIImages;
extern const uint8 *getUIAtlas(uint64 &size);

bool LogoLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();
    auto label = GRAPH::Label::createWithSystemFont("Created by Feng Chen", "arial.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 + label->getContentSize().height);

    this->addChild(label, 1);

    IMAGE::ImageObject *image = new IMAGE::ImageObject();
    uint64 uiAtlasSize = 0;
    const uint8 *uiAtlas = getUIAtlas(uiAtlasSize);
    image->initWithImageData(uiAtlas, uiAtlasSize);
    setGLShader(GRAPH::GLShaderCache::getInstance().getGLShader(GRAPH::GLShader::SHADER_NAME_POSITION_TEXTURE));
    uiAtlas_ = GRAPH::TextureAtlas::createWithTexture(GRAPH::Director::getInstance().getTextureCache()->addImage(image, "UIData"), 10);
    uiAtlas_->retain();
    SAFE_RELEASE(image);

    GRAPH::Color4B color = 0xFFFFFFFF;
    GRAPH::V3F_C4B_T2F_Quad quads [] =
    {
        {
            { MATH::Vector3f(0, 0, 0), color, GRAPH::Tex2F(0.957031f, 0.158203f), },                // bottom left
            { MATH::Vector3f(origin.x + visibleSize.width / 2, 0, 0), color, GRAPH::Tex2F(0.957031f, 0.292968f), },            // bottom right
            { MATH::Vector3f(0, origin.y + visibleSize.height / 2, 0), color, GRAPH::Tex2F(0.824218f, 0.158203f), },            // top left
            { MATH::Vector3f(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2, 0), color, GRAPH::Tex2F(0.824218f, 0.292968f), },    // top right
        },
    };

    uiAtlas_->updateQuad(&quads[0], 0);

    return true;
}

void LogoLayer::draw(GRAPH::Renderer* renderer, const MATH::Matrix4& transform, uint32_t flags) {
    customCommand_.init(_globalZOrder);
    customCommand_.func = std::bind(&LogoLayer::onDraw, this, transform, flags);
    renderer->addCommand(&customCommand_);
}

void LogoLayer::onDraw(const MATH::Matrix4& transform, uint32_t flags) {
    getGLShader()->use();
    getGLShader()->setUniformsForBuiltins(transform);
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
            pRet = NULL;
            return NULL;
        }
}
