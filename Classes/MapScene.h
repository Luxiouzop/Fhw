#ifndef _Map_Scene_H
#define _Map_SCene_H

#include "cocos2d.h"

class MapScene :public cocos2d::Scene 
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MapScene);
	void update(float delta);
};

#endif // !_Map_Scene_H

