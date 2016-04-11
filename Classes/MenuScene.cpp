//
//  MenuScene.cpp
//  Tetris
//
//  Created by fengyi on 16/3/22.
//
//

#include "MenuScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    Size win_size = Director::getInstance()->getWinSize();
    auto game_bg = Sprite::create("scene_bg.jpg");
    addChild(game_bg);
    game_bg->setPosition(win_size * 0.5f);
    Size bg_size = game_bg->getContentSize();
    float scaleHeight = win_size.height/bg_size.height;
    float scaleWidth = win_size.width/bg_size.width;
    game_bg->setScale(MAX(scaleHeight,scaleWidth));
    
    auto newItem = MenuItemImage::create("menu_newgame_nor.png","menu_newgame_sel.png",CC_CALLBACK_0(MenuScene::newGameCallBack, this));
    auto continueItem = MenuItemImage::create("menu_continue_nor.png","menu_continue_sel.png",CC_CALLBACK_0(MenuScene::continueCallBack, this));
    
    auto menu = Menu::create(newItem,continueItem,nullptr);
    menu->setPosition(_director->getWinSize() * 0.5f);
    menu->alignItemsVerticallyWithPadding(50);
    addChild(menu);
    
    return true;
}

void MenuScene::newGameCallBack()
{
    auto scene = HelloWorld::createScene();
    _director->replaceScene(scene);
}

void MenuScene::continueCallBack()
{
    auto scene = HelloWorld::createScene();
    _director->replaceScene(scene);
}
