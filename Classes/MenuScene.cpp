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
    
    auto tetrisItem = MenuItemImage::create("menu_tetris.png","menu_tetris.png",CC_CALLBACK_0(MenuScene::tetrisCallBack, this));
    auto cleanItem = MenuItemImage::create("menu_clean.png","menu_clean.png",CC_CALLBACK_0(MenuScene::tetrisCallBack, this));
    auto colorItem = MenuItemImage::create("menu_color.png","menu_color.png",CC_CALLBACK_0(MenuScene::tetrisCallBack, this));
    auto dragItem = MenuItemImage::create("menu_drag.png","menu_drag.png",CC_CALLBACK_0(MenuScene::tetrisCallBack, this));
    auto hexItem = MenuItemImage::create("menu_hex.png","menu_hex.png",CC_CALLBACK_0(MenuScene::tetrisCallBack, this));
    auto menu = Menu::create(tetrisItem,cleanItem,colorItem,dragItem,hexItem,nullptr);
    menu->setPosition(_director->getWinSize() * 0.5f);
    menu->alignItemsVerticallyWithPadding(50);
    addChild(menu);
    return true;
}

void MenuScene::tetrisCallBack()
{
    auto scene = HelloWorld::createScene();
    _director->replaceScene(scene);
}
