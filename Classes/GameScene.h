#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <vector>
USING_NS_CC;
class GameScene : public cocos2d::Scene {

public:
    int a[15][13] = { {0,0,0,0,0,0,0,0,0,0,0,0,0},
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
                      {0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
    /*std::vector <ui::Layout*> _vBlock;
    std::vector <ui::ImageView*> _vSmall;*/
    std::vector<ui::ImageView*> _vblock;
    bool isRemove = false;
    bool isPlay = false;
    bool isBorn = true;
    bool isScore = false;
    int Score = 0;
    bool isLose = false;
    int count = 1;
    int _row = -4, _column = 7, _rolation = 0, _typeBlock, type_Block=4, _score = 0, _index = 0,_Score =0, z=13;
    bool isMoving = false;

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    Node* Imagesave = CSLoader::getInstance()->createNode("csb/ImageSave.csb");
    Node* gameScene = CSLoader::getInstance()->createNode("csb/bg_play_scene.csb");
    ui::Text* score = (ui::Text*)gameScene->getChildByName<ui::Text*>("Score");
    ui::ImageView* _tBlock;
    ui::Layout* _tblock;
    void tetrisAction(ui::Layout* block);

    void tetrisdoDraw();

    ui::Layout* tetrisBorn(int typeBlock);
    void rolationBlock(ui::Layout* _block, int _blockType, int rolation);
    void changeLeftX(ui::Layout* _block);
    void changeRightX(ui::Layout* _block);
    void checkArr(int arr[15][13], int blockType, int row, int column, int rolation);
    void update(float dt);
    void tetrisStart();
    bool looped = false;
};



#endif // __GAME_SCENE_H__
