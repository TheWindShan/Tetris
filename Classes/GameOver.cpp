//
//  GameOver.cpp
//  Tetris
//
//  Created by fengyi on 16/4/11.
//
//

#include "GameOver.h"
#include "HelloWorldScene.h"

USING_NS_CC;

bool GameOver::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    auto background = LayerColor::create(Color4B::BLACK, winSize.width, winSize.height);
    background->setOpacity(128);
    this->addChild(background);
    
    auto panelBG = Sprite::create("ico_gameover.png");
    panelBG->setAnchorPoint(Vec2(0.5, 0.5));
    panelBG->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));
    this->addChild(panelBG);
    
    auto newGame = MenuItemImage::create("menu_newgame_nor.png", "menu_newgame_sel.png", CC_CALLBACK_0(GameOver::btnCallBack, this));
    auto menu = Menu::create(newGame, nullptr);
    panelBG->addChild(menu);
    Size bgSize = panelBG->getContentSize();
    menu->setPosition(Vec2(bgSize.width * 0.5f, bgSize.height * 0.3f));
    
    
    return true;
}


void GameOver::btnCallBack()
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}