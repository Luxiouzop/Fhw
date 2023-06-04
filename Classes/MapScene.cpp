#include "MapScene.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

Scene* MapScene::createScene()
{

	return MapScene::create();
}

bool MapScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto mapUI = GUIReader::getInstance()->widgetFromJsonFile("Map/Map.json");
	this->addChild(mapUI, 1);
	auto backButton = (Button*)mapUI->getChildByName("back_button");
	backButton->addTouchEventListener(CC_CALLBACK_2(MapScene::menuBackCallback, this));

	auto map1Button = (Button*)mapUI->getChildByName("map1_button");
	map1Button->addTouchEventListener(CC_CALLBACK_2(MapScene::menuMap1Callback, this));

	return true;
}

void MapScene::update(float delta) {

}

void MapScene::menuBackCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, HelloWorld::createScene()));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void MapScene::menuMap1Callback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, GameScene::createScene(1)));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
