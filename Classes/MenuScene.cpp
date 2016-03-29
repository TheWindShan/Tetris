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
    auto menuBackground = Sprite::create("scene_bg.jpg");
    menuBackground->setPosition(_director->getWinSize() * 0.5f);
    addChild(menuBackground);
    
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
