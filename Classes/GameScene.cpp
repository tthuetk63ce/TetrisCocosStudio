#include "GameScene.h"
#include "GameMenuScene.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include <time.h>  
#include <string> 
#include <iostream>
#include <vector>
#include "GameOver.h"


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
    srand(time(NULL));
    _typeBlock = rand() % 6 + 1;
    GameScene::tetrisStart();
   


    tetrisAction(_tblock);
  /*  tetrisAction(_vBlock.at(count));*/


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

void GameScene::tetrisStart() {
    _tblock = (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_t_0")->clone();
    _tblock->setPosition(Vec2(Vec2(414, 1205)));
    this->addChild(_tblock);

    _tBlock = (ui::ImageView*)Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(_typeBlock))->clone();
    _tBlock->setPosition(Vec2(95, 165));
    this->addChild(_tBlock);
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
    
void GameScene::changeLeftX(ui::Layout* _block) {
    
     _block->runAction(MoveBy::create(0.25f, Point(-60, 0))); 

}

void GameScene::changeRightX(ui::Layout* _block) {

    _block->runAction(MoveBy::create(0.25f, Point(60, 0)));

}

void GameScene::update(float dt) {
    score->setString(to_string(_Score));
    if ((_tblock->getPositionY() <= 355) 
        || (a[_row + 1][_column + 1] == 1 && a[_row + 2][_column + 1] == 10)
        || (a[_row + 1][_column] == 1 && a[_row + 2][_column] == 10)
        || (a[_row + 1][_column - 1] == 1 && a[_row + 2][_column - 1] == 10)
        || (a[_row-1][_column-1] == 1 && a[_row][_column-1]==10)
        || (a[_row - 1][_column + 1] == 1 && a[_row][_column + 1] == 10)
        || (a[_row][_column -1] == 1 && a[_row+1][_column - 1] == 10)
        || (a[_row][_column + 1] == 1 && a[_row+1][_column + 1] == 10)
        || (a[_row][_column]==1 && a[_row+1][_column]==10)
        )
    {

        for (int i = _row - 1;i <= _row + 1;i++)
            for (int j = _column - 1;j <= _column + 1;j++)
                if (a[i][j] == 1)
                {
                    a[i][j] = 10;
                    _vblock.push_back((ui::ImageView*)static_cast<ui::ImageView*>(_tblock->getChildren().at(0))->clone());
                    _vblock.at(_index)->setPosition(Vec2(60*j-6,305+50*(14-i)));
                    this->addChild(_vblock.at(_index));
                    _index++;
                }
        for (int i = 1;i <= 12;i++) if (a[1][i] == 10 || a[2][i] == 10) {
            isLose = true;
            break;
        }
        if (!isLose) {
            score->setString(to_string(_score / 11));
            for (int i = 14;i >= 2;i--)
                for (int j = 1;j <= 11;j++)
                    if ((a[i][j] == a[i][j + 1]) && (a[i][j] == 10)) {
                        _score++;
                        
                        if (_score % 11 == 0)
                        {
                            _Score++;
                            isScore = true;
                        }
                        if (isScore) {
                            for (int ii = 0;ii <= _vblock.size() - 1;ii++) {
                                if (this->_vblock.at(ii)!=nullptr) {
                                    if (_vblock.at(ii)->getPositionY() == 305 + 50 * (14 - i)) {
                                        this->removeChild(_vblock.at(ii));
                                        _vblock.at(ii) = nullptr;
                                    }
                                    else if(_vblock.at(ii)->getPositionY() >= 305 + 50 * (15 - i))
                                        _vblock.at(ii)
                                        ->setPosition(_vblock.at(ii)->getPosition() + Vec2(0, -50));
                                }
                            }
                            for (int k = i;k >= 2;k--)
                                for (int j = 1;j <= 12;j++)
                                    a[k][j] = a[k - 1][j];
                            isScore = false;                          
                            break;
                        }

                    }
                    else { _score = 0; break; }
        }
        else {
            auto scene = GameOver::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
        }


        _tblock->stopActionByTag(2);
        this->removeChild(_tBlock);
        this->removeChild(_tblock);
   
        for (int i = 1;i <= 14;i++)
            for (int j = 1;j <= 13;j++) if (a[i][j] == 10) CCLOG("%d %d %d", i, j, a[i][j]); 

        _row = -4;
        _column = 7;
        _rolation = 0;
        isScore = false;

        _tblock = tetrisBorn(_typeBlock);
        _tblock->setPosition(Vec2(414, 1205));
        this->addChild(_tblock);
        tetrisAction(_tblock);

        srand(time(NULL));
        _typeBlock = rand() % 6 +1;
        _tBlock = (ui::ImageView*)Imagesave->getChildByName<ui::ImageView*>("small_" + to_string(_typeBlock))->clone();
        _tBlock->setPosition(Vec2(98, 165));
        this->addChild(_tBlock);
    }
    
    
}

// Rolation
void GameScene::checkArr(int arr[15][13], int blockType, int row, int column, int rolation) {
//O
    if (blockType%6 == 1) {
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
    if (blockType%6 == 2) {
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
    if (blockType%6 == 3) {
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
            a[_row][_column] = 1;
            a[_row][_column-1] = 1;
            a[_row][_column - +1] = 1;
            a[_row+1][_column + 1] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column + 1] ==1) a[_row - 1][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column] ==1) a[_row + 1][_column] = 0;
        }
    }
//Z
    if (blockType%6 == 5) {
        if (rolation % 2 == 0) {
            a[_row+1][_column - 1] = 1;
            a[_row][_column-1] = 1;
            a[_row ][_column] = 1;
            a[_row-1][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column+1] ==1) a[_row - 1][_column+1] = 0;
            if (a[_row ][_column + 1] ==1) a[_row ][_column + 1] = 0;
            if (a[_row+1][_column ] ==1) a[_row+1][_column] = 0;
            if (a[_row + 1][_column + 1] ==1) a[_row + 1][_column + 1] = 0;
        }
        else if (rolation % 2 == 1) {
            a[_row][_column] = 1;
            a[_row][_column - 1] = 1;;
            a[_row + 1][_column + 1] = 1;
            a[_row + 1][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column] ==1) a[_row - 1][_column] = 0;
            if (a[_row - 1][_column+1] == 1) a[_row - 1][_column+1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
        }
    }
//T
    if (blockType%6 == 4) {
        if (rolation % 4 == 3) {
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
        else if (rolation % 4 == 0) {

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
        else if (rolation % 4 == 1) {

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
        else if (rolation % 4 == 2) {

            a[_row-1][_column] = 1;
            a[_row][_column ] = 1;
            a[_row ][_column-1] = 1;
            a[_row + 1][_column] = 1;
            if (a[_row - 1][_column - 1] ==1) a[_row - 1][_column - 1] = 0;
            if (a[_row - 1][_column+1] ==1) a[_row - 1][_column+1] = 0;
            if (a[_row][_column + 1] ==1) a[_row][_column + 1] = 0;
            if (a[_row + 1][_column - 1] ==1) a[_row + 1][_column - 1] = 0;
            if (a[_row + 1][_column+1] ==1) a[_row + 1][_column+1] = 0;
        }
    }
//I
    if (blockType%6 == 0) {
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

    if (_blockType%6 == 1) {
        if (rolation % 1 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 75));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(30, 75));
            _block->getChildren().at(3)->setPosition(Vec2(30, 25));
        }
    }
    //L 
    if (_blockType%6 == 2) {
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
    if (_blockType%6 == 5) {
        if (rolation % 2 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 75));
            _block->getChildren().at(1)->setPosition(Vec2(90, 75));
            _block->getChildren().at(2)->setPosition(Vec2(90, 25));
            _block->getChildren().at(3)->setPosition(Vec2(150, 25));
        }
        else if (rolation % 2 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 25));
            _block->getChildren().at(1)->setPosition(Vec2(30, 75));
            _block->getChildren().at(2)->setPosition(Vec2(90, 75));
            _block->getChildren().at(3)->setPosition(Vec2(90, 125));
        }
    }
    //T
    if (_blockType%6 == 4) {
        if (rolation % 4 == 1) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 75));
            _block->getChildren().at(1)->setPosition(Vec2(90, 75));
            _block->getChildren().at(2)->setPosition(Vec2(150, 75));
            _block->getChildren().at(3)->setPosition(Vec2(90, 25));
        }
        else if (rolation % 4 == 2) {
            _block->getChildren().at(0)->setPosition(Vec2(90, 125));
            _block->getChildren().at(1)->setPosition(Vec2(90, 75));
            _block->getChildren().at(2)->setPosition(Vec2(90, 25));
            _block->getChildren().at(3)->setPosition(Vec2(30, 75));
        }
        else if (rolation % 4 == 3) {
            _block->getChildren().at(0)->setPosition(Vec2(150, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(30, 25));
            _block->getChildren().at(3)->setPosition(Vec2(90, 75));
        }
        else if (rolation % 4 == 0) {
            _block->getChildren().at(0)->setPosition(Vec2(30, 25));
            _block->getChildren().at(1)->setPosition(Vec2(30, 75));
            _block->getChildren().at(2)->setPosition(Vec2(30, 125));
            _block->getChildren().at(3)->setPosition(Vec2(90, 75));
        }
    }
    //J
    if (_blockType%6 == 3) {
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
            _block->getChildren().at(0)->setPosition(Vec2(30, 25));
            _block->getChildren().at(1)->setPosition(Vec2(90, 25));
            _block->getChildren().at(2)->setPosition(Vec2(90, 75));
            _block->getChildren().at(3)->setPosition(Vec2(90, 125));
        }
    }
    //I
    if (_blockType%6 == 0) {
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
    int indexL = 0, indexR = 0;
    if (a[_row - 1][_column + 1] == 0 && a[_row][_column + 1] == 0 && a[_row + 1][_column + 1] == 0)
        indexR = 11;
    else indexR = 10;
    auto seq = Sequence::create(
        MoveBy::create(0.25f, Point(0, -50)),
        CallFunc::create([=] {
            _row += 1;
            if (_row >= 2) checkArr(a, type_Block, _row, _column, _rolation);
            if (_row >= 3 && _row <= 13) {
                if (a[_row - 2][_column] != 10)a[_row - 2][_column] = 0;
                if (a[_row - 2][_column - 1] != 10)a[_row - 2][_column - 1] = 0;
                if (a[_row - 2][_column + 1] != 10)a[_row - 2][_column + 1] = 0;
            }
            CCLOG("%d %d %d\n%d %d %d\n%d %d %d",
                a[_row - 1][_column - 1],
                a[_row - 1][_column],
                a[_row - 1][_column + 1],
                a[_row][_column - 1],
                a[_row][_column],
                a[_row][_column + 1],
                a[_row + 1][_column - 1],
                a[_row + 1][_column],
                a[_row + 1][_column + 1]
            );
            }),
        DelayTime::create(0.25f),
        nullptr
                );
    auto rSeq = RepeatForever::create(seq);
    rSeq->setTag(2);
    seq->setTag(1);
    block->runAction(rSeq);

    ActionTimeline* animTimeline = CSLoader::createTimeline("csb/bg_play_scene.csb");
    animTimeline->setTimeSpeed(0.5f);
    animTimeline->gotoFrameAndPlay(0);

    auto rolationBtn = gameScene->getChildByName<ui::Button*>("btn_rolation");
    rolationBtn->setPressedActionEnabled(true);
    rolationBtn->addClickEventListener([=](Ref*) {
        if (_column >= 2 && _row >= 2 && _column<=11
            &&a[_row-1][_column-1] !=10 &&a[_row-1][_column]!=10 &&a[_row-1][_column+1]!=10
            && a[_row][_column - 1] != 10 && a[_row][_column] != 10 && a[_row][_column + 1] != 10
            && a[_row + 1][_column - 1] != 10 && a[_row + 1][_column] != 10 && a[_row + 1][_column + 1] != 10
            ) {
            _rolation++;
            CCLOG("%d", _rolation);
            checkArr(a, type_Block, _row, _column, _rolation);
            rolationBlock(block, type_Block, _rolation);
        }
        });
    auto leftBtn = gameScene->getChildByName<ui::Button*>("btn_left");
    leftBtn->setPressedActionEnabled(true);
    leftBtn->addClickEventListener([=](Ref*) {
        if (_row >= 2) {
            if (block->getChildren().size() == 3)
                if (_column >= 2 && a[_row - 1][_column - 1] == 0 && a[_row][_column - 1] == 0 && a[_row + 1][_column - 1] ==0 ) {
                    _column -= 1;
                    CCLOG("%d", _column);
                    a[_row - 1][_column + 2] = 0;
                    a[_row][_column + 2] = 0;
                    a[_row + 1][_column + 2] = 0;
                    changeLeftX(block);
                }
            if (block->getChildren().size() == 4 && _column>=3)
                /*if (    (a[_row-1][_column-1] == 1 && a[_row-1][_column-2]!=10
                        && a[_row][_column-1] == 1 && a[_row][_column - 2] != 10
                        && a[_row+1][_column - 1] == 1 && a[_row+1][_column - 2] !=10)
                    ||
                      (
                       a[_row + 1][_column - 1] == 1 && a[_row + 1][_column - 2] != 10
                       && a[_row][_column - 1] == 1 && a[_row][_column - 2] != 10                          
                      )
                    ||
                    (
                        a[_row + 1][_column] == 1 && a[_row + 1][_column - 1] != 10
                        && a[_row][_column] == 1 && a[_row][_column - 1] != 10
                        )
                    ||
                    (a[_row][_column] == 1 && a[_row ][_column - 1] != 10
                        && a[_row-1][_column - 1] == 1 && a[_row-1][_column - 2] != 10
                        )
                    ||
                    (a[_row][_column] == 1 && a[_row][_column - 1] != 10
                        && a[_row + 1][_column - 1] == 1 && a[_row + 1][_column - 2] != 10
                        )
                    )*/
                if (a[_row-1][_column-2] !=10  && a[_row][_column-2]!=10 && a[_row+1][_column-2]!=10)
            {
                _column -= 1;
                CCLOG("%d", _column);
                a[_row - 1][_column + 2] = 0;
                a[_row][_column + 2] = 0;
                a[_row + 1][_column + 2] = 0;
                changeLeftX(block);
            }
        }});
    auto rightBtn = gameScene->getChildByName<ui::Button*>("btn_right");
    rightBtn->setPressedActionEnabled(true);
    rightBtn->addClickEventListener([=](Ref*) {
        if (_row >= 2) {
            if (block->getChildren().size() == 3 && _column <= 11 && a[_row - 1][_column + 1] == 0 && a[_row][_column + 1] == 0 && a[_row + 1][_column + 1] == 0)
            {
                _column += 1;
                a[_row - 1][_column - 2] = 0;
                a[_row][_column - 2] = 0;
                a[_row + 1][_column - 2] = 0;
                changeRightX(block);
            }
            if (block->getChildren().size() == 4 && _column <= indexR)
                /*if ((a[_row - 1][_column + 1] == 1 && a[_row - 1][_column + 2] != 10
                    && a[_row][_column + 1] == 1 && a[_row][_column + 2] != 10
                    && a[_row + 1][_column + 1] == 1 && a[_row + 1][_column + 2] != 10)
                    ||
                    (
                        a[_row - 1][_column + 1] == 1 && a[_row - 1][_column + 2] != 10
                        && a[_row][_column + 1] == 1 && a[_row][_column + 2] != 10
                        )
                    ||
                    (
                        a[_row + 1][_column + 1] == 1 && a[_row + 1][_column + 2] != 10
                        && a[_row][_column + 1] == 1 && a[_row][_column + 2] != 10
                        )
                    ||
                    (
                        a[_row][_column] == 1 && a[_row][_column + 1] != 10
                        && a[_row - 1][_column + 1] == 1 && a[_row - 1][_column + 2] != 10
                        )
                    ||
                    (
                        a[_row][_column] == 1 && a[_row][_column + 1] != 10
                        && a[_row + 1][_column + 1] == 1 && a[_row + 1][_column + 2] != 10
                        )
                    ||
                    (a[_row - 1][_column + 1] == 0 && a[_row - 1][_column + 2] != 10
                        && a[_row][_column + 1] == 0 && a[_row][_column + 2] != 10
                        && a[_row + 1][_column + 1] == 0 && a[_row + 1][_column + 2] != 10)
                    ||
                    (
                        a[_row - 1][_column + 1] == 0 && a[_row - 1][_column + 2] != 10
                        && a[_row][_column + 1] == 0 && a[_row][_column + 2] != 10
                        )
                    ||
                    (
                        a[_row + 1][_column + 1] == 0 && a[_row + 1][_column + 2] != 10
                        && a[_row][_column + 1] == 0 && a[_row][_column + 2] != 10
                        )
                    ||
                    (
                        a[_row][_column] == 0 && a[_row][_column + 1] != 10
                        && a[_row - 1][_column + 1] == 0 && a[_row - 1][_column + 2] != 10
                        )
                    ||
                    (
                        a[_row][_column] == 0 && a[_row][_column + 1] != 10
                        && a[_row + 1][_column + 1] == 0 && a[_row + 1][_column + 2] != 10
                        )
                    ||
                    (a[_row][_column - 1] == 1 && a[_row][_column] == 1 && a[_row][_column + 1] == 1
                        && a[_row][_column + 2] != 10
                        )
                    )*/
                if ((a[_row][_column+2]!=10 &&a[_row][_column+1]== 0 
                    && a[_row-1][_column+2]!=10 && a[_row-1][_column+1] == 0
                    && a[_row+1][_column + 2] != 10 && a[_row+1][_column + 1] == 0
                    )
                    ||
                    (
                        a[_row][_column + 2] != 10 && a[_row][_column + 1] == 1
                        && a[_row + 1][_column + 2] != 10 && a[_row + 1][_column + 1] == 1
                        )
                    || 
                    (
                        a[_row][_column + 2] != 10 && a[_row][_column + 1] == 1
                        && a[_row - 1][_column + 2] != 10 && a[_row - 1][_column + 1] == 1
                        )
                    || 
                    (   
                        a[_row][_column+2]!=10 && a[_row][_column+1] == 1
                        && a[_row+1][_column+2]!=10 && a[_row + 1][_column + 1] == 0
                        )
                    ||
                    (
                        a[_row+1][_column + 2] != 10 && a[_row+1][_column + 1] == 1
                        && a[_row ][_column + 2] != 10 && a[_row ][_column + 1] == 0
                        )
                    )
                {
                    changeRightX(block);
                    _column += 1;
                    CCLOG("%d", _column);
                    a[_row - 1][_column - 2] = 0;
                    a[_row][_column - 2] = 0;
                    a[_row + 1][_column - 2] = 0;
                }
        }});
    auto downBtn = gameScene->getChildByName<ui::Button*>("btn_down");
    downBtn->setPressedActionEnabled(true);
    downBtn->addClickEventListener([=](Ref*) {
                    block->stopAction(rSeq);
                    block->runAction(RepeatForever::create(
                        Sequence::create(
                            MoveBy::create(0.00000001f, Point(0, -50)),
                            CallFunc::create([=] {
                                _row += 1;
                                if (_row >= 2) checkArr(a, type_Block, _row, _column, _rolation);
                                if (_row >= 3 && _row <= 13) {
                                    if (a[_row - 2][_column] != 10)a[_row - 2][_column] = 0;
                                    if (a[_row - 2][_column - 1] != 10)a[_row - 2][_column - 1] = 0;
                                    if (a[_row - 2][_column + 1] != 10)a[_row - 2][_column + 1] = 0;
                                }
                                }),
                            nullptr)));
    });
}

ui::Layout* GameScene::tetrisBorn(int typeBlock) {
    switch (typeBlock)
               {
                case 1:
                    type_Block = 1;
                    return (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_o_0")->clone();
                    break;
                case 2:
                    type_Block = 2;
                    return (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_l_0")->clone();
                    break;
                case 3:
                    type_Block = 3;
                    return (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_j_0")->clone();
                    break;
                case 4:
                    type_Block = 4;
                    return (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_t_0")->clone();
                    break;
                case 5:
                    type_Block = 5;
                    return (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_z_0")->clone();
                    break;
                case 6:
                    type_Block = 6;
                    return (ui::Layout*)Imagesave->getChildByName<ui::Layout*>("block_i_0")->clone();
                    break;
                default:
                    break;
                }
}
