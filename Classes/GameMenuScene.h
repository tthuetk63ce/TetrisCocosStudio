#ifndef __GAME_MENU_SCENE_H__
#define __GAME_MENU_SCENE_H__

#include "cocos2d.h"

class GameMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameMenuScene);

private:
    void GotoGameScene(cocos2d::Ref* sender);
};

#endif // __GAME_MENU_SCENE_H__
