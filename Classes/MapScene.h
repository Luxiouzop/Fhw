#ifndef _Map_Scene_H
#define _Map_SCene_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "cocostudio/CocoStudio.h"

class MapScene :public cocos2d::Scene 
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MapScene);
	void update(float delta);

	void menuBackCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void menuMap1Callback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // !_Map_Scene_H

