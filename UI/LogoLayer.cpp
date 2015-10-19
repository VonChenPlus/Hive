#include "LogoLayer.h"
#include "UI/UIAtlas.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UI/CONTROLS/Label.h"
#include "GRAPH/UNITY3D/GLTexture.h"

extern const AtlasImage *UIImages;
extern const uint8 *getUIAtlas(uint64 &size);

bool LogoLayer::init() {
    if (!Layer::init()) {
        return false;
    }

    auto visibleSize = GRAPH::Director::getInstance().getRenderView()->getVisibleSize();
    auto origin = GRAPH::Director::getInstance().getRenderView()->getVisibleOrigin();
    auto label = GRAPH::Label::createWithSystemFont("Hello World", "arial.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height);

    this->addChild(label, 1);

    IMAGE::ImageObject *image = new IMAGE::ImageObject();
    uint64 uiAtlasSize = 0;
    const uint8 *uiAtlas = getUIAtlas(uiAtlasSize);
    image->initWithImageData(uiAtlas, uiAtlasSize);
    uiAtlas_ = GRAPH::TextureAtlas::createWithTexture(GRAPH::Director::getInstance().getTextureCache()->addImage(image, "UIData"), sizeof(UIImages) / sizeof(AtlasImage));

    return true;
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
