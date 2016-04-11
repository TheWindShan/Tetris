//
//  GameOver.h
//  Tetris
//
//  Created by fengyi on 16/4/11.
//
//

#ifndef GameOver_h
#define GameOver_h

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(GameOver);
    
private:
    void btnCallBack();
};


#endif /* GameOver_h */
