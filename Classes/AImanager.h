#ifndef __AIMANAGER_H__
#define __AIMANAGER_H__

#include"GameScene3.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;



class AImanager :public Node
{
public:
	virtual bool init(Node* parent);
	static AImanager* create(Node* parent);
	void setAI(Armature* e_bow,GameScene3::ArrowState AIArrow, Vector<Sprite*> targetVector);
	void update(float delta);
	void findTarget();


private:
	Armature* e_bow;
	GameScene3::ArrowState AIArrow;
	Vec2 targetLoc;
	Vector<Sprite*> targetVector;
};
















#endif