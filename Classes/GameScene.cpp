#include "GameScene.h"
#include "MapScene.h"
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

	// UI界面
	auto gameUI = GUIReader::getInstance()->widgetFromJsonFile("gaming/gaming_1.json");
	this->addChild(gameUI, 12);
	gameUI->setName("gameUI");
	gameUI->getChildByName("quitLabel")->setVisible(false);
	gameUI->getChildByName("textbox")->setVisible(false);
	gameUI->getChildByName("quit_back_to_game")->setVisible(false);
	gameUI->getChildByName("pause_back_to_game")->setVisible(false);
	gameUI->getChildByName("sure_to_quit")->setVisible(false);

	auto pauseButton = (Button*)gameUI->getChildByName("pause_button");
	pauseButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menupauseCallback, this));

	auto pausetBackButton = (Button*)gameUI->getChildByName("pause_back_to_game");
	pausetBackButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menubackCallback, this));

	auto quitButton = (Button*)gameUI->getChildByName("quit_button");
	quitButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menuquitCallback, this));

	auto quitBackButton = (Button*)gameUI->getChildByName("quit_back_to_game");
	quitBackButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menubackCallback, this));

	auto quitSureButton = (Button*)gameUI->getChildByName("sure_to_quit");
	quitSureButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menuquitgameCallback, this));

	auto strengthBar = (LoadingBar*)gameUI->getChildByName("strength");
	strengthBar->setPercent(0.0);
	ispause = false;

	// 弓
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Export/bow.ExportJson");
	m_bow = Armature::create("bow");
	this->addChild(m_bow, 10);
	m_bow->setScale(0.05);
	m_bow->setPosition(Vec2(100, visibleSize.height / 2));
	//m_bow->getAnimation()->play("ready");

	// 箭矢
	playerArrow.arrow = Sprite::create("jian.png");
	playerArrow.arrow->setScale(0.1, 0.1);
	playerArrow.islegal = true;
	playerArrow.arrow->setName("arrow");
	playerArrow.arrow->setPosition(m_bow->getPosition());
	this->addChild(playerArrow.arrow, 9);
	playerArrow.arrow->setVisible(false);

	// 风速
	wind = 0;

	// 背景云
	auto cloud1 = Sprite::create("cloud.png");
	cloud1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	cloud1->setName("cloud1");
	this->addChild(cloud1, 1);

	// 得分
	playerScore = 0;
	auto scoreLabel = Label::createWithTTF(StringUtils::format("Score:%d", playerScore), "fonts/arial.ttf", 24);
	scoreLabel->setName("scoreLabel");
	scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 100));
	scoreLabel->setTextColor(Color4B::YELLOW);
	this->addChild(scoreLabel, 11);

	// 箭靶
	addTarget();
	addTarget();
	addTarget();
	addTarget();

	// 鼠标回调
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnd, this);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegin, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMove, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// update
	this->scheduleUpdate();

	return true;
}

void GameScene::update(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (playerArrow.islegal == false&&!ispause)
	{
		// 箭矢移动、旋转
		playerArrow.arrow->setPosition(playerArrow.arrow->getPosition() + 0.1 * playerArrow.velocity);
		playerArrow.arrow->setRotation(-CC_RADIANS_TO_DEGREES(playerArrow.velocity.getAngle()));
		// 每次更新根据 风向、重力 修正速度
		playerArrow.velocity.y -= 1;
		if (playerArrow.velocity.x - 0.01-wind > 0) {
			playerArrow.velocity.x -= 0.05 + wind;
		}

		// 检测箭矢与箭靶碰撞
		for (auto target : targetVector)
		{
			if (playerArrow.arrow->getBoundingBox().intersectsRect(target->getBoundingBox()))
			{
				// 箭矢重置
				playerArrow.arrow->setVisible(false);
				playerArrow.arrow->setPosition(m_bow->getPosition());
				playerArrow.islegal = true;
				// 得分增加
				auto scoreLabel = (Label*)this->getChildByName("scoreLabel");
				playerScore += 1;
				scoreLabel->setString(StringUtils::format("Score:%d", playerScore));
				// 箭靶命中后消失
				auto actionDown = CallFunc::create(CC_CALLBACK_0(GameScene::removeTarget, this, target));
				target->runAction(actionDown);
				// 箭靶增加
				auto actionAdd = CallFunc::create(CC_CALLBACK_0(GameScene::addTarget, this));
				auto sequence = Sequence::create(DelayTime::create(0.75), actionAdd, nullptr);
				this->runAction(sequence);

			}
		}

		// 检测箭矢越界
		if (playerArrow.arrow->getPositionX() > visibleSize.width||playerArrow.arrow->getPositionX()<0
			||playerArrow.arrow->getPositionY()<0||playerArrow.arrow->getPositionY()>1500)
		{
			playerArrow.arrow->setVisible(false);
			playerArrow.arrow->setPosition(m_bow->getPosition());
			playerArrow.islegal = true;
		}
	}
		
}

void GameScene::onTouchEnd(Touch* touch, Event* unused_event)
{
	if (playerArrow.islegal == true&&!ispause)
	{
		playerArrow.point2 = touch->getLocation();
		playerArrow.velocity = playerArrow.point1 - playerArrow.point2;
		auto len = playerArrow.velocity.getLength() / 2;
		if (len > playerArrow.maxV) {
			len = playerArrow.maxV;
		}
		playerArrow.velocity = playerArrow.velocity.getNormalized() * len;
		CCLOG("%f,%f", playerArrow.velocity.x, playerArrow.velocity.y);
		playerArrow.islegal = false;
		playerArrow.arrow->setVisible(true);
	}
}

bool GameScene::onTouchBegin(Touch* touch, Event* unused_event)
{
	if (playerArrow.islegal == true&&!ispause)
	{
		playerArrow.point1 = touch->getLocation();
		m_bow->getAnimation()->play("ready");
		return true;
	}
	else 
	{
		return false;
	}
	
}

void GameScene::onTouchMove(Touch* touch, Event* unused_event)
{
	if (playerArrow.islegal == true&&!ispause)
	{
		auto point = touch->getLocation();
		auto strength = (playerArrow.point1 - point).getLength();
		strength /= (2 * playerArrow.maxV) / 100;
		if (strength > 100)
			strength = 100;
		auto strengthBar = (LoadingBar*)this->getChildByName("gameUI")->getChildByName("strength");
		strengthBar->setPercent(strength);
		
		m_bow->setRotation(-CC_RADIANS_TO_DEGREES((playerArrow.point1 - point).getAngle()));
	}
}

void GameScene::addTarget()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto target = Sprite::create("target1.png");
	target->setScale(0.1);
	auto randx = random(1, 9);
	auto xx = 0;
	if (randx <= 4) {
		xx = TargetField1;
	}
	else if (randx <= 7) {
		xx = TargetField2;
	}
	else {
		xx = TargetField3;
	}
	auto yy = random(100, 700);

	target->setPosition(xx, yy);
	this->addChild(target, 5);
	this->targetVector.pushBack(target);
}

void GameScene::removeTarget(Sprite* target)
{
	this->removeChild(target);
	targetVector.eraseObject(target);
}

void GameScene::menuquitCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		ispause = true;
		this->getChildByName("gameUI")->getChildByName("quit_back_to_game")->setVisible(true);
		this->getChildByName("gameUI")->getChildByName("quitLabel")->setVisible(true);
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setVisible(true);
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void GameScene::menupauseCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		ispause = true;
		this->getChildByName("gameUI")->getChildByName("pause_back_to_game")->setVisible(true);
		this->getChildByName("gameUI")->getChildByName("textbox")->setVisible(true);
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void GameScene::menubackCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		ispause = false;
		this->getChildByName("gameUI")->getChildByName("pause_back_to_game")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("textbox")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("quit_back_to_game")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("quitLabel")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setVisible(false);
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void GameScene::menuquitgameCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		break;
	case cocos2d::ui::Widget::TouchEventType::MOVED:
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, MapScene::createScene()));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

