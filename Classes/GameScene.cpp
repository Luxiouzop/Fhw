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

	// UI����
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

	// ��
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Export/bow.ExportJson");
	m_bow = Armature::create("bow");
	this->addChild(m_bow, 10);
	m_bow->setScale(0.05);
	m_bow->setPosition(Vec2(100, visibleSize.height / 2));
	//m_bow->getAnimation()->play("ready");

	// ��ʸ
	playerArrow.arrow = Sprite::create("jian.png");
	playerArrow.arrow->setScale(0.1, 0.1);
	playerArrow.islegal = true;
	playerArrow.arrow->setName("arrow");
	playerArrow.arrow->setPosition(m_bow->getPosition());
	this->addChild(playerArrow.arrow, 9);
	playerArrow.arrow->setVisible(false);

	// ����
	wind = 0;

	// ������
	auto cloud1 = Sprite::create("cloud.png");
	cloud1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	cloud1->setName("cloud1");
	this->addChild(cloud1, 1);

	// �÷�
	playerScore = 0;
	auto scoreLabel = Label::createWithTTF(StringUtils::format("Score:%d", playerScore), "fonts/arial.ttf", 24);
	scoreLabel->setName("scoreLabel");
	scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 200));
	scoreLabel->setTextColor(Color4B::YELLOW);
	this->addChild(scoreLabel, 11);
	// ��ʱ
	time = 10;
	auto timeLabel = Label::createWithTTF(StringUtils::format("%d", (int)time), "fonts/arial.ttf", 36);
	timeLabel->setName("timeLabel");
	timeLabel->setTextColor(Color4B::YELLOW);
	timeLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
	this->addChild(timeLabel, 11);

	// ����
	addTarget();
	addTarget();
	addTarget();
	addTarget();

	// ���ص�
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
		// ��ʸ�ƶ�����ת
		playerArrow.arrow->setPosition(playerArrow.arrow->getPosition() + 0.1 * playerArrow.velocity);
		playerArrow.arrow->setRotation(-CC_RADIANS_TO_DEGREES(playerArrow.velocity.getAngle()));
		// ÿ�θ��¸��� �������� �����ٶ�
		auto resisty = 0.00015 * pow(playerArrow.velocity.y, 2);
		playerArrow.velocity.y -= 1.5 + resisty;
		auto resistx = 0.00015 * pow(playerArrow.velocity.x, 2);
		
		CCLOG("velocityx:%f", playerArrow.velocity.x);
		CCLOG("Resistx:%f", resistx);
		if (playerArrow.velocity.x - resistx -wind > 0) {
			playerArrow.velocity.x -= resistx + wind;
		}

		// ����ʸ�������ײ
		for (auto target : targetVector)
		{
			if (playerArrow.arrow->getBoundingBox().intersectsRect(target->getBoundingBox()))
			{
				// ��ʸ����
				playerArrow.arrow->setVisible(false);
				playerArrow.arrow->setPosition(m_bow->getPosition());
				playerArrow.islegal = true;
				// �÷�����
				auto score = 0;
				if (abs(target->getPositionX() - TargetField1)<0.01)
					score = 1;
				else if (abs(target->getPositionX() - TargetField2) < 0.01)
					score = 2;
				else if (abs(target->getPositionX() - TargetField3) < 0.01)
					score = 3;
				showScoreTips(score, target->getPosition());
				auto scoreLabel = (Label*)this->getChildByName("scoreLabel");
				scoreLabel->setString(StringUtils::format("Score:%d", playerScore));
				// �������к���ʧ
				auto actionDown = CallFunc::create(CC_CALLBACK_0(GameScene::removeTarget, this, target));
				target->runAction(actionDown);
				// ��������
				auto actionAdd = CallFunc::create(CC_CALLBACK_0(GameScene::addTarget, this));
				auto sequence = Sequence::create(DelayTime::create(0.75), actionAdd, nullptr);
				this->runAction(sequence);

			}
		}

		// ����ʸԽ��
		if (playerArrow.arrow->getPositionX() > visibleSize.width||playerArrow.arrow->getPositionX()<0
			||playerArrow.arrow->getPositionY()<0||playerArrow.arrow->getPositionY()>1500)
		{
			playerArrow.arrow->setVisible(false);
			playerArrow.arrow->setPosition(m_bow->getPosition());
			playerArrow.islegal = true;
		}
	}
	if (!ispause) 
	{
		time -= delta;
		auto timeLabel = (Label*)this->getChildByName("timeLabel");
		timeLabel->setString(StringUtils::format("%d", (int)time));
		if (time <= 0)
			ispause = true;
	}
	if(time<=0)
	{
		auto gameoverLabel = Label::createWithTTF(StringUtils::format("Game over!"), "fonts/arial.ttf", 64);
		gameoverLabel->setTextColor(Color4B::RED);
		gameoverLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
		this->addChild(gameoverLabel, 12);
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 25));
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setVisible(true);
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
		m_bow->getAnimation()->play("reset");
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

void GameScene::flyout(Label* label) {
	label->setVisible(false);
	label->removeFromParent();
}

void GameScene::showScoreTips(int score,Vec2 position)
{
	playerScore += score;
	auto label = Label::createWithTTF(StringUtils::format("+%d", score), "fonts/arial.ttf", 24);
	label->setTextColor(Color4B::GREEN);
	label->setPosition(position);
	this->addChild(label, 12);
	label->runAction(Sequence::create(
		MoveBy::create(0.75f, Vec2(0, 30)),
		CallFunc::create(CC_CALLBACK_0(GameScene::flyout, this, label)),
		NULL
	));
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

