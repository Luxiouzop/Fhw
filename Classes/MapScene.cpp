#include "MapScene.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "GameScene3.h"
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
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/click2.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/map_choose.wav");

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto mapUI = GUIReader::getInstance()->widgetFromJsonFile("Map/Map.json");
	this->addChild(mapUI, 1);
	auto backButton = (Button*)mapUI->getChildByName("back_button");
	backButton->addTouchEventListener(CC_CALLBACK_2(MapScene::menuBackCallback, this));

	auto map1Button = (Button*)mapUI->getChildByName("map1_button");
	map1Button->addTouchEventListener(CC_CALLBACK_2(MapScene::menuMap1Callback, this));
	auto map2Button = (Button*)mapUI->getChildByName("map2_button");
	map2Button->addTouchEventListener(CC_CALLBACK_2(MapScene::menuMap2Callback, this));
	auto map3Button = (Button*)mapUI->getChildByName("map3_button");
	map3Button->addTouchEventListener(CC_CALLBACK_2(MapScene::menuMap3Callback, this));

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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.wav");
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/map_choose.wav");
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, GameScene::createScene(1)));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void MapScene::menuMap2Callback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/map_choose.wav");
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, GameScene2::createScene(1)));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void MapScene::menuMap3Callback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/map_choose.wav");
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, GameScene3::createScene(1)));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}
