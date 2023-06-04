#include "GameScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

int mapno = 0;

Scene* GameScene::createScene(int s)
{
	mapno = s;
	return GameScene::create();
}

bool GameScene::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	ArmatureDataManager::getInstance()->addArmatureFileInfo("Export/bow.ExportJson");
	m_bow = Armature::create("bow");
	if (m_bow == NULL)
	{
		CCLOG("bow load error!");
		return false;
	}
	this->addChild(m_bow, 10);
	m_bow->setScale(0.05);
	m_bow->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//m_bow->getAnimation()->play("ready");

	//
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnd, this);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegin, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameScene::update(float delta)
{

}

void GameScene::onTouchEnd(Touch* touch, Event* unused_event)
{
	m_bow->getAnimation()->play("ready");
}

bool GameScene::onTouchBegin(Touch* touch, Event* unused_event)
{
	touch->getLocation();
	return true;
}

