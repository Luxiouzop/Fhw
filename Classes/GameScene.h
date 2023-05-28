#ifndef _Game_Scene_H
#define _Game_Scene_H

#include "cocos2d.h"

class GameScene :public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene(int s);
	virtual bool init();
	CREATE_FUNC(GameScene);
	void update(float delta);
private:




};

#endif // !_Game_Scene_H

