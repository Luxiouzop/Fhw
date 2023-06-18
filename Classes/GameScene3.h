#pragma once
#ifndef _Game_Scene3_H
#define _Game_Scene3_H

//#include"AImanager.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;




#define TargetField1 782
#define TargetField2 878
#define TargetField3 974


class GameScene3 :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int s);
	virtual bool init();
	CREATE_FUNC(GameScene3);
	void update(float delta);
	void onTouchEnd(Touch* touch, Event* unused_event);
	bool onTouchBegin(Touch* touch, Event* unused_event);
	void onTouchMove(Touch* touch, Event* unused_event);
	void addTarget();
	void removeTarget(Sprite* target);
	void flyout(Label* label);
	void randomWind();
	void showScoreTips(int score, Vec2 position, Color4B color);
	void menuquitCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void menupauseCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void menubackCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void menuquitgameCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void findTarget();
	void shootArrow();
	void shootEnd();


struct ArrowState
{
	Sprite* arrow;  // ��ʸ����
	bool islegal;  // ��ʸ�Ƿ���ã�����Ϊtrue
	Vec2 velocity; // ��ʸ�ٶ�
	Vec2 point1;   // ȷ����ʸ�ٶȵ���ʼ��
	Vec2 point2;   // ȷ����ʸ�ٶȵ��յ�
	float maxV = 180;  //����ٶ�
	int bonus = 1;
};
private:
	Armature* m_bow;
	Armature* e_bow;
	
	ArrowState playerArrow;
	ArrowState AIArrow;
	int wind;
	Vector<Sprite*> targetVector;
	int playerScore;
	int maxScore;
	bool ispause;
	double time;
	int bow_move;
	double windtime;
	Vec2 targetLoc;
	
};

#endif // !_Game_Scene_H

