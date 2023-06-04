#ifndef _Game_Scene_H
#define _Game_Scene_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;


struct ArrowState
{
	Sprite* arrow;  // 箭矢精灵
	bool islegal;  // 箭矢是否可用，可用为true
	Vec2 velocity; // 箭矢速度
	Vec2 point1;   // 确定箭矢速度的起始点
	Vec2 point2;   // 确定箭矢速度的终点
	float maxV = 200;  //最大速度
};

#define TargetField1 750
#define TargetField2 850
#define TargetField3 950


class GameScene :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int s);
	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float delta);
	void onTouchEnd(Touch* touch, Event* unused_event);
	bool onTouchBegin(Touch* touch, Event* unused_event);
	void onTouchMove(Touch* touch, Event* unused_event);
	void addTarget();
	void removeTarget(Sprite* target);

private:
	Armature* m_bow;
	ArrowState playerArrow;
	float wind;
	Vector<Sprite*> targetVector;
	int playerScore;
};

#endif // !_Game_Scene_H

