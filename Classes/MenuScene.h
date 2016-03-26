//
//  MenuScene.hpp
//  Tetris
//
//  Created by fengyi on 16/3/22.
//
//

#ifndef MenuScene_h
#define MenuScene_h

#include "cocos2d.h"

class MenuScene : cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MenuScene);
private:
    void newGameCallBack();
    void continueCallBack();
};


#endif /* MenuScene_h */
