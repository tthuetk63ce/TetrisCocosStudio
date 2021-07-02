#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameScene.h"

USING_NS_CC;

Scene* GameMenuScene::createScene()
{
	return GameMenuScene::create();
};

bool GameMenuScene::init() {

	if (!Scene::init())
	{
		return false;
	}

	auto gameMenuScene = CSLoader::getInstance()->createNode("csb/bg_menu_scene.csb");
	this->addChild(gameMenuScene);

	auto playButton = gameMenuScene->getChildByName<ui::Button*>("btn_play");
	playButton->setPressedActionEnabled(true);
	playButton->addClickEventListener([=](Ref*) {
		auto gameScene = GameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene, Color3B(0, 255, 255)));
	});

	return true;
}
