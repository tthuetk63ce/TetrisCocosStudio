#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <vector>
USING_NS_CC;
class GameScene : public cocos2d::Scene {

public:
    int a[14][13] = { {0,0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}, 
                      {0,0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0,0},
    };
    std::vector <ui::Layout*> _vBlock;
    bool isRemove = false;
    bool isPlay = false;
    bool isBorn = true;
    bool isScore = false;
    int Score = 0;
    bool isLose = false;
    int count = 1;
    int _row = -4, _column = 7, _rolation = 0, _typeBlock = 1;
    bool isMoving = false;
    int _blockType;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    Node* Imagesave = CSLoader::getInstance()->createNode("csb/ImageSave.csb");
    Node* gameScene = CSLoader::getInstance()->createNode("csb/bg_play_scene.csb");

    void tetrisAction(ui::Layout* block);

    void tetrisdoDraw();

    void tetrisdoBorn(ui::ImageView* Small, ui::Layout* block);
    void rolationBlock(ui::Layout* _block, int _blockType, int rolation);
    void changeLeftX(ui::Layout* _block, int column);
    void changeRightX(ui::Layout* _block, int column);
    void checkArr(int arr[14][13], int blockType, int row, int column, int rolation);
    void update(float dt);
    void tetrisStart();
};



#endif // __GAME_SCENE_H__
