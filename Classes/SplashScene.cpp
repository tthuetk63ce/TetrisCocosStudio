#include "SplashScene.h"
#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;


Scene* SplashScene::createScene()
{
	return SplashScene::create();
};

bool SplashScene::init() {
	if (!Scene::init())
	{
		return false;
	}
	auto splashScene = CSLoader::getInstance()->createNode("csb/bg_splash_scene.csb");
	this->addChild(splashScene);
	this->scheduleOnce(schedule_selector(SplashScene::GotoGameMenuScene), 1.0f);
	return true;
}

void SplashScene::GotoGameMenuScene(float dt) {
	auto scene = GameMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}
