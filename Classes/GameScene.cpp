#include "GameScene.h"
#include "MapScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;



Scene* GameScene::createScene(int s)
{
	return GameScene::create();
}

bool GameScene::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 音乐音效预加载
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/click2.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/pull_bow.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/shoot2.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/hit.wav");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/wind.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/bonus.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/error.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/win.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("audio/lose.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/frontMusic.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/inGame.mp3");
	// UI界面
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/frontMusic.mp3", true);
	auto gameUI = GUIReader::getInstance()->widgetFromJsonFile("gaming/gaming_1.json");
	this->addChild(gameUI, 12);
	gameUI->setName("gameUI");
	gameUI->getChildByName("quitLabel")->setVisible(false);
	gameUI->getChildByName("quit_back_to_game")->setVisible(false);
	//gameUI->getChildByName("pause_back_to_game")->setVisible(false);
	//gameUI->getChildByName("textbox")->setVisible(false);
	gameUI->getChildByName("sure_to_quit")->setVisible(false);
	// 暂停按钮
	auto pauseButton = (Button*)gameUI->getChildByName("pause_button");
	pauseButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menupauseCallback, this));
	// 暂停界面的返回按钮
	auto pausetBackButton = (Button*)gameUI->getChildByName("pause_back_to_game");
	pausetBackButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menubackCallback, this));
	// 退出按钮
	auto quitButton = (Button*)gameUI->getChildByName("quit_button");
	quitButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menuquitCallback, this));
	// 退出界面的返回按钮
	auto quitBackButton = (Button*)gameUI->getChildByName("quit_back_to_game");
	quitBackButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menubackCallback, this));
	// 退出界面的确定按钮
	auto quitSureButton = (Button*)gameUI->getChildByName("sure_to_quit");
	quitSureButton->addTouchEventListener(CC_CALLBACK_2(GameScene::menuquitgameCallback, this));
	// 力度条
	auto strengthBar = (LoadingBar*)gameUI->getChildByName("strength");
	strengthBar->setPercent(0.0);
	// 游戏暂停标志
	ispause = true;
	// 游戏结束标签
	auto gameoverLabel = Label::createWithTTF(StringUtils::format("Game over!"), "fonts/Marker Felt.ttf", 64);
	gameoverLabel->setTextColor(Color4B::RED);
	gameoverLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
	gameoverLabel->setName("gameoverLabel");
	gameoverLabel->setVisible(false);
	this->addChild(gameoverLabel, 12);
	// 游戏帮助
	auto tips = Label::createWithTTF(StringUtils::format("W/S: control bow up and down movement\nLeft mouse button pressed : the bow starts to build up power\nMouse movement : control the bow accumulation strength and shooting direction\nLeft mouse button release : shoot arrow\n\nThe arrow target will be fixed in this level"),
		"fonts/arial.ttf", 20);
	tips->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	tips->setName("tips");
	this->addChild(tips, 12);

	// 弓
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Export/bow.ExportJson");
	m_bow = Armature::create("bow");
	this->addChild(m_bow, 10);
	m_bow->setScale(0.05);
	m_bow->setPosition(Vec2(100, 82));
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
	auto windLabel = Label::createWithTTF(StringUtils::format("no wind"), "fonts/arial.ttf", 30);
	windLabel->setTextColor(Color4B::YELLOW);
	windLabel->setPosition(visibleSize.width / 2, visibleSize.height - 100);
	windLabel->setName("windLabel");
	this->addChild(windLabel, 11);
	randomWind();
	windtime = 10;

	// 背景云
	auto cloud1 = Sprite::create("cloud.png");
	cloud1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	cloud1->setName("cloud1");
	this->addChild(cloud1, 1);
	auto cloud2 = Sprite::create("cloud.png");
	cloud2->setPosition(cloud1->getPosition());
	cloud2->setPositionX(cloud2->getPositionX() + cloud1->getContentSize().width - 1);
	cloud2->setName("cloud2");
	this->addChild(cloud2, 1);

	// 得分
	playerScore = 0;
	maxScore = 20;
	auto scoreLabel = Label::createWithTTF(StringUtils::format("Score:%d (Target:%d)", playerScore, maxScore), "fonts/arial.ttf", 24);
	scoreLabel->setName("scoreLabel");
	scoreLabel->setPosition(Vec2(150, visibleSize.height - 150));
	scoreLabel->setTextColor(Color4B::YELLOW);
	this->addChild(scoreLabel, 11);
	// 计时
	time = 60;
	auto timeLabel = Label::createWithTTF(StringUtils::format("%d", (int)time), "fonts/arial.ttf", 36);
	timeLabel->setName("timeLabel");
	timeLabel->setTextColor(Color4B::YELLOW);
	timeLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
	this->addChild(timeLabel, 11);

	// 箭靶
	addTarget();
	addTarget();
	addTarget();
	addTarget();
	addTarget();
	addTarget();

	// 地图
	auto map = TMXTiledMap::create("background/backgroundMap.tmx");
	map->setPosition(Vec2(0, 0));
	this->addChild(map, 1);


	// 鼠标回调
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnd, this);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegin, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMove, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	// 键盘回调
	bow_move = 0;
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [&](EventKeyboard::KeyCode code, Event* e) {
		if (!ispause) {
			switch (code)
			{
			case cocos2d::EventKeyboard::KeyCode::KEY_W:
				bow_move = 1;
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_S:
				bow_move = -1;
				break;
			default:
				break;
			}
		}
	};
	keyListener->onKeyReleased= [&](EventKeyboard::KeyCode code, Event* e) {
		if (!ispause) {
			switch (code)
			{
			case cocos2d::EventKeyboard::KeyCode::KEY_W:
				bow_move = 0;
				break;
			case cocos2d::EventKeyboard::KeyCode::KEY_S:
				bow_move = 0;
				break;
			default:
				break;
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	// update
	this->scheduleUpdate();



	return true;
}

void GameScene::update(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 背景云移动
	if (!ispause)
	{
		auto cloud1 = this->getChildByName("cloud1");
		auto cloud2 = this->getChildByName("cloud2");
		//CCLOG("cloud1x:%f,cloud2x:%f", cloud1->getPositionX(), cloud2->getPositionX());
		if (wind == 1)//<<<<<
		{
			cloud1->setPositionX(cloud1->getPositionX() - 0.25);
			cloud2->setPositionX(cloud2->getPositionX() - 0.25);
			if (cloud1->getPositionX() + cloud1->getContentSize().width/2 <= 0)
			{
				cloud1->setPositionX(cloud2->getPositionX() + cloud2->getContentSize().width - 1);
			}
			if (cloud2->getPositionX() + cloud2->getContentSize().width/2 <= 0)
			{
				cloud2->setPositionX(cloud1->getPositionX() + cloud1->getContentSize().width - 1);
			}
		}
		else if (wind == -1)//>>>>>
		{
			cloud1->setPositionX(cloud1->getPositionX() + 0.25);
			cloud2->setPositionX(cloud2->getPositionX() + 0.25);
			if (cloud1->getPositionX() - cloud1->getContentSize().width/2 >= visibleSize.width)
			{
				cloud1->setPositionX(cloud2->getPositionX() - cloud2->getContentSize().width + 1);
			}
			if (cloud2->getPositionX() - cloud2->getContentSize().width/2 >= visibleSize.width)
			{
				cloud2->setPositionX(cloud1->getPositionX() - cloud1->getContentSize().width + 1);
			}
		}
		// 风速 
		windtime -= delta;
		if (windtime <= 0)
		{
			randomWind();
			windtime = 10;
		}
	}

	// 箭靶运动暂停开始
	if (ispause) {
		for (auto target : targetVector)
		{
			target->pause();
		}
	}
	else
	{
		for (auto target : targetVector)
		{
			target->resume();
		}
	}
	
	// 弓移动
	if (!ispause)
	{
		if (bow_move == 1&&m_bow->getPositionY()+4<448)
		{
			m_bow->setPositionY(m_bow->getPositionY() + 4);
		}
		else if (bow_move == -1 && m_bow->getPositionY() - 4 > 82)
		{
			m_bow->setPositionY(m_bow->getPositionY() - 4);
		}
	}

	// 箭矢移动
	if (playerArrow.islegal == false&&!ispause)
	{
		// 箭矢移动、旋转
		playerArrow.arrow->setPosition(playerArrow.arrow->getPosition() + 0.1 * playerArrow.velocity);
		playerArrow.arrow->setRotation(-CC_RADIANS_TO_DEGREES(playerArrow.velocity.getAngle()));
		// 每次更新根据 风向、重力 修正速度
		auto resisty = 0.00015 * pow(playerArrow.velocity.y, 2);
		playerArrow.velocity.y -= 1.5 + resisty;
		auto resistx = 0.00015 * pow(playerArrow.velocity.x, 2);
		
		//CCLOG("velocityx:%f", playerArrow.velocity.x);
		//CCLOG("Resistx:%f", resistx);
		if (playerArrow.velocity.x - resistx -wind > 0&&wind==1) 
		{
			playerArrow.velocity.x -= 1.25 * resistx;
		}
		else if (wind == -1)
		{
			playerArrow.velocity.x -= 0.75 * resistx;
		}
		else if (wind == 0)
		{
			playerArrow.velocity.x -= resistx;
		}

		for (auto target : targetVector)
		{
			if (playerArrow.arrow->getBoundingBox().intersectsRect(target->getBoundingBox()))
			{
				//
				auto tmp = ParticleSystem::create("shoot.plist");
				tmp->setPosition(target->getPosition());
				tmp->setScale(3);
				this->addChild(tmp, 13);

				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/hit.wav");
				// 箭矢重置
				if (playerArrow.bonus == 0||playerArrow.bonus%3!=0)
				{
					playerArrow.arrow->setVisible(false);
					playerArrow.arrow->setPosition(m_bow->getPosition());
					playerArrow.islegal = true;
					playerArrow.bonus++;
					if (playerArrow.bonus % 3 == 0) {
						auto bonusLabel = Label::createWithTTF(StringUtils::format("Bonus!"), "fonts/Marker Felt.ttf", 30);
						bonusLabel->setPosition(m_bow->getPosition() + Vec2(50, 25));
						this->addChild(bonusLabel, 11);
						bonusLabel->runAction(MoveBy::create(1.0f, Vec2(0, 50)));
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/bonus.mp3");
						bonusLabel->runAction(Sequence::create(
							ScaleBy::create(1.0f, 2),
							CallFunc::create(CC_CALLBACK_0(GameScene::flyout, this, bonusLabel)),
							NULL
						));
					}
				}
				
				// 得分增加
				auto score = 0;
				if (abs(target->getPositionX() - TargetField1)<0.01)
					score = 1;
				else if (abs(target->getPositionX() - TargetField2) < 0.01)
					score = 2;
				else if (abs(target->getPositionX() - TargetField3) < 0.01)
					score = 3;
				showScoreTips(score, target->getPosition(),Color4B::GREEN);
				auto scoreLabel = (Label*)this->getChildByName("scoreLabel");
				scoreLabel->setString(StringUtils::format("Score:%d (Target:%d)", playerScore, maxScore));
				// 箭靶命中后消失
				auto actionDown = CallFunc::create(CC_CALLBACK_0(GameScene::removeTarget, this, target));
				target->runAction(actionDown);
				tmp->removeFromParent();

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
			playerArrow.bonus++;
			if (playerArrow.bonus % 3 == 0) {
				auto bonusLabel = Label::createWithTTF(StringUtils::format("Bonus!"), "fonts/Marker Felt.ttf", 30);
				bonusLabel->setPosition(m_bow->getPosition() + Vec2(50, 25));
				this->addChild(bonusLabel, 11);
				bonusLabel->runAction(MoveBy::create(1.0f, Vec2(0, 50)));
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/bonus.mp3");
				bonusLabel->runAction(Sequence::create(
					ScaleBy::create(1.0f, 2),
					CallFunc::create(CC_CALLBACK_0(GameScene::flyout, this, bonusLabel)),
					NULL
				));
			}
		}
	}
	// 倒计时
	if (!ispause) 
	{
		time -= delta;
		auto timeLabel = (Label*)this->getChildByName("timeLabel");
		timeLabel->setString(StringUtils::format("%d", (int)time));
	}
	// 游戏结束
	if((time<=0|| playerScore >= maxScore)&&!ispause)
	{
		this->getChildByName("gameUI")->getChildByName("pause_button")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("quit_button")->setVisible(false);
		auto gameoverLabel = (Label*)this->getChildByName("gameoverLabel");
		if (playerScore >= maxScore)
		{
			CCLOG("WIN");
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("audio/frontMusic.mp3");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/win.mp3");
			gameoverLabel->setString("Success!");
			gameoverLabel->setTextColor(Color4B::YELLOW);
		}
		else 
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("audio/frontMusic.mp3");
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/lose.mp3");
		}
		this->stopActionByTag(114);
		gameoverLabel->setVisible(true);
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 25));
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setVisible(true);
		ispause = true;
	}

}

void GameScene::onTouchEnd(Touch* touch, Event* unused_event)
{
	if (playerArrow.islegal == true&&!ispause)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/shoot2.wav");
		playerArrow.point2 = touch->getLocation();
		playerArrow.velocity = playerArrow.point1 - playerArrow.point2;
		auto len = playerArrow.velocity.getLength() / 2;
		if (len > playerArrow.maxV) {
			len = playerArrow.maxV;
		}
		playerArrow.velocity = playerArrow.velocity.getNormalized() * (len+20);
		CCLOG("%f,%f", playerArrow.velocity.x, playerArrow.velocity.y);
		playerArrow.islegal = false;
		playerArrow.arrow->setPosition(m_bow->getPosition());
		playerArrow.arrow->setVisible(true);
		m_bow->getAnimation()->play("reset");
		auto strengthBar = (LoadingBar*)this->getChildByName("gameUI")->getChildByName("strength");
		strengthBar->setPercent(0);
	}
}

bool GameScene::onTouchBegin(Touch* touch, Event* unused_event)
{
	if (playerArrow.islegal == true&&!ispause)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/pull_bow.wav");
		playerArrow.point1 = touch->getLocation();
		m_bow->getAnimation()->play("ready");
		return true;
	}
	else if (playerArrow.islegal == false && !ispause)
	{
		//CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/error.mp3");
		auto label = Label::createWithTTF("X", "fonts/arial.ttf", 36);
		label->setTextColor(Color4B::RED);
		this->addChild(label, 11);
		label->setPosition(touch->getLocation());
		label->runAction(Sequence::create(
			DelayTime::create(0.5),
			CallFunc::create(CC_CALLBACK_0(GameScene::flyout, this, label)),
			NULL
		));
		return false;
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
	double yy = random(100, 675);

	target->setPosition(xx, yy);
	this->addChild(target, 5);
    /*// 箭靶随机运动
	double vy1 = random(100, 675);
	double vy2 = random(100, 675);
	double t1 = random(0.75, 1.75);
	double t2 = random(0.75, 1.75);
	auto act = RepeatForever::create(
		Sequence::create(
			MoveTo::create(t1, Vec2(xx, vy1)),
			MoveTo::create(t2, Vec2(xx, vy2)),
			NULL
		));
	target->runAction(act);*/

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

void GameScene::randomWind()
{
	wind = random(-1, 1);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/wind.mp3");
	auto windLabel = (Label*)this->getChildByName("windLabel");
	if (wind == 1)
	{
		windLabel->setString("<<<<<<");
	}
	else if (wind == 0)
	{
		windLabel->setString("<<O>>");
	}
	else if (wind == -1)
	{
		windLabel->setString(">>>>>>");
	}
	CCLOG("wind:%d", wind);
}

void GameScene::showScoreTips(int score,Vec2 position,Color4B color)
{
	playerScore += score;
	auto label = Label::createWithTTF(StringUtils::format("+%d", score), "fonts/Marker Felt.ttf", 24);
	label->setTextColor(color);
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.wav");
		this->getChildByName("tips")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("pause_back_to_game")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("textbox")->setVisible(true);
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.wav");
		this->getChildByName("gameUI")->getChildByName("quit_back_to_game")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("quitLabel")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("sure_to_quit")->setVisible(false);
		this->getChildByName("gameUI")->getChildByName("pause_back_to_game")->setVisible(true);
		this->getChildByName("gameUI")->getChildByName("textbox")->setVisible(true);
		this->getChildByName("tips")->setVisible(true);
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.wav");
		this->getChildByName("tips")->setVisible(false);
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
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/click2.wav");
		Director::getInstance()->replaceScene(TransitionFade::create(0.75, MapScene::createScene()));
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

