#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GemeDefine.h"
#include "ui/CocosGui.h"
#include "cocos-ext.h"
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
private:
    HelloWorld();
    ~HelloWorld();
    virtual bool init();
    void updateDown(float dt);
    void updateLeft(float dt);
    void updateRight(float dt);
    void updateNextType();
    void updateUI();
    void newTetris();
    void reStart();
    void touchAction(Ref *pSender, cocos2d::extension::Control::EventType type);
    void touchButtonAction(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void touchDownCallBack(float dt);
    void removeFullRow();
    void setAutoDownSpeed(float speed);
    bool checkNextTepDown();
    void checkBoxCallBack(Ref *pSender,cocos2d::ui::CheckBox::EventType type);
    void gameOver();
    CREATE_FUNC(HelloWorld);
private:

    struct BOARD
    {
        int var;
        int color;
    }m_num_map[ROW][COL];
    
    int     m_cur_type;
    int     m_next_type;
    int     m_cur_row;      //4x4方块左下角位置
    int     m_cur_col;      //
    int     m_max_row;
    bool    m_is_down;      //自动下落是否成功
    bool    m_game_over;
    float   delay_time;
    int     m_score;
    int     m_high_score;
    int     m_total_row;
    float   m_effect_volume;
    cocos2d::Node*          m_next_tetris_node;
    cocos2d::Label*         m_score_label;
    cocos2d::Label*         m_high_score_label;
    cocos2d::Sprite*        m_panle_bg;
    cocos2d::Sprite*        m_sprite_map[ROW][COL];
    cocos2d::Sprite*        m_next_tetris[4][4];
    
};

#endif // __HELLOWORLD_SCENE_H__
