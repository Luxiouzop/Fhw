#include "GameScene.h"
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
	return true;
}

void GameScene::update(float delta)
{

}