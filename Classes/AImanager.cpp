#include "AImanager.h"

bool AImanager::init(Node* parent)
{
	if (!Node::init())
	{
		return false;
	}
	parent->addChild(this);

	return true;
}

AImanager* AImanager::create(Node* parent)
{
	AImanager* pRet = new(std::nothrow) AImanager();
	if (pRet && pRet->init(parent))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void AImanager::setAI(Armature* e_bow, GameScene3::ArrowState AIArrow,Vector<Sprite*> targetVector)
{
	this->e_bow = e_bow;
	this->AIArrow = AIArrow;
	this->targetVector = targetVector;

	auto ss = Sequence::create(
		CallFunc::create(CC_CALLBACK_0(AImanager::findTarget,this)),
		MoveTo::create(1.5,targetLoc),
		NULL
	);
	auto s = RepeatForever::create(ss);
	this->e_bow->runAction(s);
}

void AImanager::update(float delta)
{
}

void AImanager::findTarget()
{
	targetLoc = targetVector.at(0)->getPosition();
}
