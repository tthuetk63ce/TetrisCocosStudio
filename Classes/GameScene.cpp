#include "GameScene.h"
#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <time.h>  
#include <string> 
#include <iostream>
#include <vector>


USING_NS_CC;
using namespace std;
using namespace cocostudio::timeline;


Scene* GameScene::createScene()
{
	return GameScene::create();
};

bool GameScene::init() {

	if (!Scene::init())
	{
		return false;
	}


	this->addChild(gameScene);
    Imagesave->retain();
    tetrisdoDraw();

    GameScene::tetrisStart();

    tetrisAction(_vBlock.at(count));
    _vSmall.at(_typeBlock % 5)->setPosition(Vec2(98, 165));
    auto backButton = gameScene->getChildByName<ui::Button*>("btn_back");
    backButton->setPressedActionEnabled(true);
    backButton->addClickEventListener([=](Ref*) {
        auto gameMenuScene = GameMenuScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameMenuScene, Color3B(0, 255, 255)));
        });
    
    ActionTimeline* animTimeline = CSLoader::createTimeline("csb/bg_play_scene.csb");
    animTimeline->setTimeSpeed(0.5f);
    animTimeline->gotoFrameAndPlay(0);
    gameScene->runAction(animTimeline);
    this->scheduleUpdate();
	return true;
}

void GameScene::tetrisdoBorn(ui::ImageView* _Small) {
        srand(time(NULL));
        switch (rand() % 6)
        {
        case 0:
            _Small = Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(1));
            _typeBlock = 1;
            break;
        case 1:
            _Small = Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(2));
            _typeBlock = 2;
            break;
        case 2:
            _Small = Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(3));
            _typeBlock = 3;
            break;
        case 3:
            _Small = Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(4));
            _typeBlock = 4;
            break;
        case 4:
            _Small = Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(5));
            _typeBlock = 5;
            break;
        case 5:
            _Small = Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(6));
            _typeBlock = 6;
            break;
        default:
            break;
        }
        _Small->setPosition(Vec2(98, 165));
        _Small->removeFromParentAndCleanup(false);
        this->addChild(_Small);
    }

void GameScene::tetrisStart() {
    for (int i = 0;i <= 1000;i++)
    {
        if (i % 6 == 0)_vBlock.push_back((ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_i_0")->clone());
        if (i % 6 == 1)_vBlock.push_back((ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_o_0")->clone());
        if (i % 6 == 2)_vBlock.push_back((ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_l_0")->clone());
        if (i % 6 == 3)_vBlock.push_back((ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_j_0")->clone());
        if (i % 6 == 4)_vBlock.push_back((ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_t_0")->clone());
        if (i % 6 == 5)_vBlock.push_back((ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_z_0")->clone());
        _vBlock.at(i)->setPosition(Vec2(414, 1205));
        this->addChild(_vBlock.at(i));
    }
    for (int i = 0;i <= 5;i++) {
        _vSmall.push_back((ui::ImageView*)Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(i+1))->clone());
        _vSmall.at(i)->setPosition(Vec2(414, 1205));
        this->addChild(_vSmall.at(i));
    }
}

void GameScene::tetrisdoDraw() {
        for (int i = 0; i <= 13; i++)
        {
            auto draw = DrawNode::create();
            draw->drawLine(
                Vec2(24, 280 + i * 50),
                Vec2(744, 280 + i * 50),
                Color4F(Color3B::BLACK, 1));
            this->addChild(draw);
        }
    
        for (int i = 0; i <= 12; i++)
        {
            auto draw = DrawNode::create();
            draw->drawLine(
                Vec2(24 + i * 60, 280),
                Vec2(24 + i * 60, 930),
                Color4F(Color3B::BLACK, 2));
            this->addChild(draw);
        }
    }
    
    
void GameScene::changeLeftX(ui::Layout* _block, int column) {
    
     _block->runAction(MoveBy::create(0.5f, Point(-60, 0))); 

}

void GameScene::changeRightX(ui::Layout* _block, int column) {

    _block->runAction(MoveBy::create(0.5f, Point(60, 0)));

}

void GameScene::update(float dt) {   
    if ((_vBlock.at(count)->getPositionY() <= 355) 
        || (a[_row + 1][_column + 1] == 1 && a[_row + 2][_column + 1] == 10)
        || (a[_row + 1][_column] == 1 && a[_row + 2][_column] == 10)
        || (a[_row + 1][_column - 1] == 1 && a[_row + 2][_column - 1] == 10))
    {
        _vSmall.at(_typeBlock % 5)->setPosition(Vec2(414, 1205));
        for (int i = _row - 1;i <= _row + 1;i++)
            for (int j = _column - 1;j <= _column + 1;j++)
                if (a[i][j] == 1) a[i][j] = 10;  
        for (int i = 1;i <= 13;i++)
            for (int j = 1;j <= 12;j++) if (a[i][j] != 0) CCLOG("%d %d %d", i, j, _row);
        _vBlock.at(count)->stopActionByTag(2);
        _row = -4;
        _column = 7;
        _rolation = 0;
        count++;
        _typeBlock++;
        tetrisAction(_vBlock.at(count));
        _vSmall.at(_typeBlock % 5)->setPosition(Vec2(98, 165));
    }
              
    //for (int i = 1;i <= 12;i++) if (a[2][i] == 10) isLose = true;
    //if (!isLose){
    //    for (int i = 12;i>=1;i--)
    //      for (int j = 1;j <= 11;j++)
    //          if ((a[i][j] != a[i][j + 1]) && (a[i][j] != 0)) {
    //            isScore = false; break;
    //          }
    //    if (isScore) {
    //        Score++;
    //        // _vBlock(at(i)) setPosition(-50);
    //        // a[i+1][j] = a[i][j]
    //          for(auto it = _vBlock.begin(); it != _vBlock.end(); ++it) 
    //              {
    //                  _v.Block.at(it) -> removeChild();
    // }
    // }
    //    }
    //}
}

// Rolation
void GameScene::checkArr(int arr[14][13], int blockType, int row, int column, int rolation) {
//O
    if (blockType == 1) {
        if (rolation % 1 == 0) {
            a[row][column - 1] = 1;
            a[row][column] = 1;
            a[row + 1][column - 1] = 1;
            a[row + 1][column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
    }
//L
    if (blockType == 2) {
        if (rolation % 4 == 0) {
            a[row -1][column-1] = 1;
            a[row ][column-1] = 1;
            a[row+1][column - 1] = 1;
            a[row +1][column] = 1;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column] ==1) a[_row][_column] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 1) {
            a[_row][_column - 1] = 1;
            a[_row][_column] = 1;
            a[_row][_column + 1] = 1;
            a[_row + 1][_column - 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row + 1][_column] ==1) a[_row + 1][_column] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 2) {
            a[_row][_column] = 1;
            a[_row - 1][_column - 1] = 1;
            a[_row - 1][_column] = 1;;
            a[_row + 1][_column] = 1;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 3) {
            a[_row][_column + 1] = 1;
            a[_row + 1][_column - 1] =1;
            a[_row + 1][_column] = 1;
            a[_row + 1][_column + 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            if (a[_row][_column] ==1) a[_row][_column] = 0;
        }
    }
//J
    if (blockType == 3) {
        if (rolation % 4 == 0) {
            a[_row][_column] = 1;
            a[_row - 1][_column] =1;
            a[_row + 1][_column - 1] = 1;
            a[_row + 1][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 1) {
            a[_row][_column - 1] = 1;
            a[_row + 1][_column - 1] = 1;
            a[_row + 1][_column] = 1;
            a[_row + 1][_column + 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column] ==1) a[_row][_column] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
        }
        else if (rolation % 4 == 2) {
            a[_row][_column - 1] = 1;
            a[_row - 1][_column - 1] = 1;
            a[_row - 1][_column] = 1;
            a[_row + 1][_column - 1] = 1;
            if (a[_row - 1][_column + 1] == 1) a[_row - 1][_column] = 0;
            if (a[_row][_column] ==1) a[_row][_column] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column] ==1) a[_row + 1][_column] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 3) {
            if(a[_row][_column - 1]!=10) a[_row][_column - 1] = 1;
            if(a[_row][_column] ==1) a[_row][_column] = 1;
            if(a[_row][_column + 1] ==1) a[_row][_column] = 1;
            if(a[_row + 1][_column + 1]==1) a[_row][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column] ==1) a[_row + 1][_column] = 0;
        }
    }
//Z
    if (blockType == 5) {
        if (rolation % 2 == 0) {
            a[_row][_column - 1] = 1;
            a[_row][_column] = 1;
            a[_row + 1][_column] = 1;
            a[_row + 1][_column + 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
        }
        else if (rolation % 2 == 1) {
            a[_row][_column] = 1;
            a[_row][_column + 1] = 1;;
            a[_row - 1][_column + 1] = 1;
            a[_row + 1][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
    }
//T
    if (blockType == 4) {
        if (rolation % 4 == 0) {
            a[_row][_column] = 1;
            a[_row + 1][_column - 1] = 1;
            a[_row + 1][_column] = 1;
            a[_row + 1][_column + 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column-1] ==1) a[_row][_column-1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
        }
        else if (rolation % 4 == 1) {

            a[_row][_column - 1] = 1;
            a[_row][_column] = 1;
            a[_row - 1][_column - 1] = 1;
            a[_row + 1][_column - 1] = 1;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column] ==1) a[_row + 1][_column] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 2) {

            a[_row][_column - 1] = 1;
            a[_row][_column] = 1;
            a[_row][_column + 1] =1;
            a[_row + 1][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 4 == 3) {

            a[_row][_column] = 1;
            a[_row][_column + 1] = 1;
            a[_row - 1][_column + 1] = 1;
            a[_row + 1][_column + 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column] ==1) a[_row + 1][_column] = 0;
        }
    }
//I
    if (blockType == 0) {
        if (rolation % 2 == 0) {
            a[row-1][column] = 1;
            a[row][column] = 1;
            a[row + 1][column] = 1;

            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;

            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;

            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
           
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 2 == 1) {
            a[row+1][column-1] = 1;
            a[row+1][column] = 1;
            a[row+1][column +1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;

            if (a[_row][_column - 1] ==1) a[_row][_column - 1] = 0;
            if (a[_row][_column] ==1) a[_row][_column] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;

        }
    }
}

void GameScene::rolationBlock(ui::Layout* _block, int _blockType, int rolation) {

    if (_blockType == 1) {
        if (rolation % 1 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 75));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(30, 75));
            _block->getChildren().at(3)->setPosition(Vec2(30, 25));
        }
    }
    //L 
    if (_blockType == 2) {
        if (rolation % 4 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 25));
            _block->getChildren().at(1)->setPosition(Vec2(30, 75));
            _block->getChildren().at(2)->setPosition(Vec2(90, 75));
            _block->getChildren().at(3)->setPosition(Vec2(150, 75));
        }
        else if (rolation % 4 == 2) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 125));
            _block->getChildren().at(1)->setPosition(Vec2(90, 125));
            _block->getChildren().at(2)->setPosition(Vec2(90, 75));
            _block->getChildren().at(3)->setPosition(Vec2(90, 25));
        }
        else if (rolation % 4 == 3) {
            _block->getChildren().at(0)->setPosition(Vec2(150, 75));
            _block->getChildren().at(1)->setPosition(Vec2(150, 25));
            _block->getChildren().at(2)->setPosition(Vec2(90, 25));
            _block->getChildren().at(3)->setPosition(Vec2(30, 25));
        }
        else if (rolation % 4 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 25));
            _block->getChildren().at(1)->setPosition(Vec2(30, 25));
            _block->getChildren().at(2)->setPosition(Vec2(30, 75));
            _block->getChildren().at(3)->setPosition(Vec2(30, 125));
        }
    }
    //Z
    if (_blockType == 5) {
        if (rolation % 2 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 75));
            _block->getChildren().at(2)->setPosition(Vec2(150, 75));
            _block->getChildren().at(3)->setPosition(Vec2(150, 125));
        }
        else if (rolation % 2 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(150, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(90, 75));
            _block->getChildren().at(3)->setPosition(Vec2(30, 75));
        }
    }
    //T
    if (_blockType == 4) {
        if (rolation % 4 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 25));
            _block->getChildren().at(1)->setPosition(Vec2(30, 75));
            _block->getChildren().at(2)->setPosition(Vec2(30, 125));
            _block->getChildren().at(3)->setPosition(Vec2(90, 75));
        }
        else if (rolation % 4 == 2) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 75));
            _block->getChildren().at(1)->setPosition(Vec2(90, 75));
            _block->getChildren().at(2)->setPosition(Vec2(150, 75));
            _block->getChildren().at(3)->setPosition(Vec2(90, 25));
        }
        else if (rolation % 4 == 3) {
            _block->getChildren().at(0)->setPosition(Vec2(150, 125));
            _block->getChildren().at(1)->setPosition(Vec2(150, 75));
            _block->getChildren().at(2)->setPosition(Vec2(150, 25));
            _block->getChildren().at(3)->setPosition(Vec2(90, 75));
        }
        else if (rolation % 4 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(150, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(30, 25));
            _block->getChildren().at(3)->setPosition(Vec2(90, 75));
        }
    }
    //J
    if (_blockType == 3) {
        if (rolation % 4 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 75));
            _block->getChildren().at(1)->setPosition(Vec2(30, 25));
            _block->getChildren().at(2)->setPosition(Vec2(90, 25));
            _block->getChildren().at(3)->setPosition(Vec2(150, 25));
        }
        else if (rolation % 4 == 2) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 125));
            _block->getChildren().at(1)->setPosition(Vec2(30, 125));
            _block->getChildren().at(2)->setPosition(Vec2(30, 75));
            _block->getChildren().at(3)->setPosition(Vec2(30, 25));
        }
        else if (rolation % 4 == 3) {
            _block->getChildren().at(0)->setPosition(Vec2(150, 25));
            _block->getChildren().at(1)->setPosition(Vec2(150, 75));
            _block->getChildren().at(2)->setPosition(Vec2(90, 75));
            _block->getChildren().at(3)->setPosition(Vec2(30, 75));
        }
        else if (rolation % 4 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 25));
            _block->getChildren().at(1)->setPosition(Vec2(150, 25));
            _block->getChildren().at(2)->setPosition(Vec2(150, 75));
            _block->getChildren().at(3)->setPosition(Vec2(150, 125));
        }
    }
    //I
    if (_blockType == 0) {
        if (rolation % 2 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(150, 25));
        }
        else if (rolation % 2 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 75));
            _block->getChildren().at(2)->setPosition(Vec2(90, 125));
        }
    }
}

void GameScene::tetrisAction(ui::Layout* block) {
    auto seq = Sequence::create(
        MoveBy::create(0.5f, Point(0, -50)),
        DelayTime::create(0.5f),
        CallFunc::create([=] {
            _row += 1;
            if (_row >= 2) checkArr(a, _typeBlock, _row, _column, _rolation);
            if (_row >= 3&&_row<=13) {
                if(a[_row - 2][_column]!=10)a[_row - 2][_column] = 0;
                if(a[_row - 2][_column - 1]!=10)a[_row - 2][_column-1] = 0;
                if(a[_row - 2][_column + 1]!=10)a[_row - 2][_column+1] = 0;
            }
            }),
        nullptr
    );
    auto rSeq = RepeatForever::create(seq);
    rSeq->setTag(2);
    block->runAction(rSeq);

    ActionTimeline* animTimeline = CSLoader::createTimeline("csb/bg_play_scene.csb");
    animTimeline->setTimeSpeed(0.5f);
    animTimeline->gotoFrameAndPlay(0);

    auto rolationBtn = gameScene->getChildByName<ui::Button*>("btn_rolation");
    rolationBtn->setPressedActionEnabled(true);
    rolationBtn->addClickEventListener([=](Ref*) {
        if (_column >= 2 && _row >= 2) {
            _rolation++;
            CCLOG("%d", _rolation);
            checkArr(a, _typeBlock, _row, _column, _rolation);
            rolationBlock(block, _typeBlock, _rolation);
        }
    });
    auto leftBtn = gameScene->getChildByName<ui::Button*>("btn_left");
    leftBtn->setPressedActionEnabled(true);
    leftBtn->addClickEventListener([=](Ref*) {  
        if (_column >= 3 && _row >= 2 
            && a[_row][_column-2] ==0 
            && a[_row-1][_column-2] == 0 
            && a[_row+1][_column-2] == 0
            )
        {
            _column -= 1;
            CCLOG("%d", _column);
            a[_row - 1][_column + 2] = 0;
            a[_row][_column + 2] = 0;
             a[_row + 1][_column + 2] = 0;
            changeLeftX(block, _column);
        }
    });
    auto rightBtn = gameScene->getChildByName<ui::Button*>("btn_right");
    rightBtn->setPressedActionEnabled(true);
    rightBtn->addClickEventListener([=](Ref*) {
        if (_column <= 11 && _row >= 2 
            && a[_row][_column + 2] == 0
            && a[_row - 1][_column + 2] == 0
            && a[_row + 1][_column + 2] == 0) {
            changeRightX(block, _column);
            _column += 1;
            CCLOG("%d", _column);
            a[_row - 1][_column - 2] = 0;
            a[_row][_column - 2] = 0;
            a[_row + 1][_column - 2] = 0;
        }
    });
}

