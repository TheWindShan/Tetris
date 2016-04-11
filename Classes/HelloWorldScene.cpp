#include "HelloWorldScene.h"
#include "AudioEngine.h"
#include "GameOver.h"
USING_NS_CC;
USING_NS_CC_EXT;

const float SCALE = 40.0f/63;

struct Tetris
{
    int box[4][4];
    int color;
    int next_type;
};

struct Tetris tetris[CELL_TYPE] =
{
    {{
        {1,0,0,0},          //  口       2 type 0
        {1,0,0,0},          //  口       2
        {1,0,0,0},          //  口     1 3 1 1
        {1,0,0,0}}, 1, 1    //  口       2
    },
    {{
        {0,0,0,0},          //          type 1
        {0,0,0,0},          //
        {0,0,0,0},          //  口口口口
        {1,1,1,1}}, 1, 0    //
    },
    {{
        {0,0,0,0},          //          type 2
        {0,0,0,0},          //
        {1,0,0,0},          //  口
        {1,1,1,0}}, 2, 3    //  口 口 口
    },
    {{
        {0,0,0,0},          //          type 3
        {1,1,0,0},          //  口 口
        {1,0,0,0},          //  口
        {1,0,0,0}}, 2, 4    //  口
    },
    {{
        {0,0,0,0},          //          type 4  next 17
        {0,0,0,0},          //
        {1,1,1,0},          //  口 口 口
        {0,0,1,0}}, 2, 5    //        口
    },
    {{
        {0,0,0,0},          //          type 5
        {0,1,0,0},          //     口
        {0,1,0,0},          //     口
        {1,1,0,0}}, 2, 2    //  口 口
    },
    {{
        {0,0,0,0},          //          type 6
        {0,0,0,0},          //
        {0,0,1,0},          //        口
        {1,1,1,0}}, 3, 7    //  口 口 口
    },
    {{
        {0,0,0,0},          //          type 7
        {1,0,0,0},          //  口
        {1,0,0,0},          //  口
        {1,1,0,0}}, 3, 8    //  口 口
    },
    {{
        {0,0,0,0},          //          type 8
        {0,0,0,0},          //
        {1,1,1,0},          //  口 口 口
        {1,0,0,0}}, 3, 9    //  口
    },
    {{
        {0,0,0,0},          //          type 9
        {1,1,0,0},          //  口 口
        {0,1,0,0},          //     口
        {0,1,0,0}}, 3, 6    //     口
    },
    {{
        {0,0,0,0},          //          type 10
        {0,0,0,0},          //
        {0,1,1,0},          //    口口
        {1,1,0,0}}, 4, 11   //  口口
    },
    {{
        {0,0,0,0},          //          type 11
        {1,0,0,0},          //  口
        {1,1,0,0},          //  口口
        {0,1,0,0}}, 4, 10   //    口
    },
    {{
        {0,0,0,0},          //          type 12
        {0,0,0,0},          //
        {1,1,0,0},          //  口口
        {0,1,1,0}}, 5, 13   //    口口
    },
    {{
        {0,0,0,0},          //          type 13
        {0,1,0,0},          //    口
        {1,1,0,0},          //  口口
        {1,0,0,0}}, 5, 12   //  口
    },
    
    {{
        {0,0,0,0},          //          type 14
        {0,0,0,0},          //
        {0,1,0,0},          //    口
        {1,1,1,0}}, 6, 15   //  口口口
    },
    {{
        {0,0,0,0},          //          type 15
        {1,0,0,0},          //  口
        {1,1,0,0},          //  口口
        {1,0,0,0}}, 6, 16   //  口
    },
    {{
        {0,0,0,0},          //          type 16
        {0,0,0,0},          //
        {1,1,1,0},          //  口口口
        {0,1,0,0}}, 6, 17   //    口
    },
    {{
        {0,0,0,0},          //          type 17
        {0,1,0,0},          //    口
        {1,1,0,0},          //  口口
        {0,1,0,0}}, 6, 14   //    口
    },
    {{
        {0,0,0,0},          //          type 18
        {0,0,0,0},          //
        {1,1,0,0},          //  口口
        {1,1,0,0}}, 1, 18   //  口口
    },
};

HelloWorld::HelloWorld()
: m_panle_bg(nullptr)
, m_cur_type(-1)
, m_next_type(-1)
, m_cur_row(-1)
, m_cur_col(-1)
, m_max_row(-1)
, delay_time(1.0f)
, m_score(0)
, m_score_label(nullptr)
, m_high_score_label(nullptr)
, m_is_down(false)
, m_game_over(false)
, m_total_row(0)
, m_effect_volume(1.0f)
, m_high_score(0)
{
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            m_num_map[row][col].var = 0;
            m_num_map[row][col].color = 0;
            m_sprite_map[row][col] = nullptr;
        }
    }
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            m_next_tetris[r][c] = nullptr;
        }
    }
}

HelloWorld::~HelloWorld()
{
}

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
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

    m_panle_bg = Sprite::create("layer_bg.png");
    addChild(m_panle_bg);
    m_panle_bg->setPosition(win_size.width * 0.5f,win_size.height * 0.5f);
    
    auto cur_score = Sprite::create("cur_score.png");
    cur_score->setAnchorPoint(Vec2(1,0.5));
    cur_score->setPosition(Vec2(win_size.width * 0.5f,win_size.height - 120));
    addChild(cur_score);
    
    m_score_label = Label::createWithSystemFont("0", "", 40);
    addChild(m_score_label);
    m_score_label->setAnchorPoint(Vec2(0,0.5));
    m_score_label->setPosition(Vec2(win_size.width * 0.5f,win_size.height - 120));
    m_score_label->setColor(Color3B::YELLOW);
    
    auto high_score = Sprite::create("high_score.png");
    high_score->setAnchorPoint(Vec2(1,0.5));
    high_score->setPosition(Vec2(win_size.width * 0.5f,win_size.height - 60));
    addChild(high_score);

    m_high_score = UserDefault::getInstance()->getIntegerForKey("high_score",0);
    auto strHighScore = __String::createWithFormat("%d",m_high_score);
    m_high_score_label = Label::createWithSystemFont(strHighScore->getCString(), "", 40);
    addChild(m_high_score_label);
    m_high_score_label->setAnchorPoint(Vec2(0,0.5));
    m_high_score_label->setPosition(Vec2(win_size.width * 0.5f,win_size.height - 60));
    m_high_score_label->setColor(Color3B::YELLOW);


    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            m_sprite_map[row][col] = Sprite::create("0.png");
            m_sprite_map[row][col]->setPosition(START_X + col * CELL_SIZE, START_Y + row * CELL_SIZE);
            m_sprite_map[row][col]->setScale(SCALE);
            m_panle_bg->addChild(m_sprite_map[row][col]);
        }
    }
    
    //预览
    m_next_tetris_node = Node::create();
    addChild(m_next_tetris_node);
    m_next_tetris_node->setPosition(Vec2(win_size.width * 0.5f + 110,win_size.height - 50));
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            m_next_tetris[r][c] = Sprite::create("0.png");
            m_next_tetris[r][c]->setPosition(c * CELL_SIZE, -r * CELL_SIZE);
            m_next_tetris[r][c]->setScale(SCALE);
            m_next_tetris_node->addChild(m_next_tetris[r][c]);
        }
    }
    
    auto music = ui::CheckBox::create("btn_sound_open.png", "btn_sound_close.png");
    music->setPosition(Vec2(win_size.width - 65,win_size.height - 100));
    music->addEventListener(CC_CALLBACK_2(HelloWorld::checkBoxCallBack, this));
    addChild(music);
    
    auto btn_restart = ui::Button::create("btn_pause.png");
    btn_restart->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchButtonAction, this));
    addChild(btn_restart,1,11);
    btn_restart->setPosition(Vec2(70,win_size.height - 100));
    
    auto btn_rorate = ui::Button::create("btn_rorate.png");
    btn_rorate->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchButtonAction, this));
    addChild(btn_rorate,1,10);
    btn_rorate->setPosition(Vec2(win_size.width - 65,450));
    
    auto btn_left = ControlButton::create(ui::Scale9Sprite::create("btn_left.png"));
    addChild(btn_left,10,100);
    btn_left->setPosition(Vec2(65,300));
    btn_left->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchAction),
                                                  Control::EventType::TOUCH_DOWN
                                                  | Control::EventType::TOUCH_UP_INSIDE
                                                  | Control::EventType::TOUCH_UP_OUTSIDE
                                                  | Control::EventType::TOUCH_CANCEL
                                                  | Control::EventType::DRAG_OUTSIDE);
    auto btn_right = ControlButton::create(ui::Scale9Sprite::create("btn_right.png"));
    btn_right->setPosition(Vec2(win_size.width - 65, 300));
    addChild(btn_right,20,101);
    btn_right->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchAction),
                                                   Control::EventType::TOUCH_DOWN
                                                   | Control::EventType::TOUCH_UP_INSIDE
                                                   | Control::EventType::TOUCH_UP_OUTSIDE
                                                   | Control::EventType::TOUCH_CANCEL
                                                   | Control::EventType::DRAG_OUTSIDE);

    auto btn_down = ControlButton::create(ui::Scale9Sprite::create("btn_down.png"));
    btn_down->setPosition(Vec2(65,450));
    addChild(btn_down,30,102);
    btn_down->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::touchAction),
                                                  Control::EventType::TOUCH_DOWN
                                                  | Control::EventType::TOUCH_UP_INSIDE
                                                  | Control::EventType::TOUCH_UP_OUTSIDE
                                                  | Control::EventType::TOUCH_CANCEL
                                                  | Control::EventType::DRAG_OUTSIDE);
    

    reStart();

    
    return true;
}

void HelloWorld::checkBoxCallBack(Ref *pSender,ui::CheckBox::EventType type)
{
    switch (type)
    {
        case ui::CheckBox::EventType::SELECTED:
        {
            cocos2d::experimental::AudioEngine::pauseAll();
            m_effect_volume = 0;
        }
            break;
        case ui::CheckBox::EventType::UNSELECTED:
        {
            cocos2d::experimental::AudioEngine::resumeAll();
            m_effect_volume = 1.0f;
        }
            break;
        default:
            break;
    }
}

void HelloWorld::touchButtonAction(Ref *pSender, ui::Widget::TouchEventType type)
{
    int tag = static_cast<Node*>(pSender)->getTag();
    if (type == ui::Widget::TouchEventType::BEGAN)
    {
        if (tag == 10)
        {
            cocos2d::experimental::AudioEngine::play2d("sound/s_change.wav",false,m_effect_volume);
            updateNextType();
        }
        else if (tag == 11)
        {
            reStart();
        }
    }
}

void HelloWorld::touchAction(Ref *pSender, cocos2d::extension::Control::EventType type)
{
    int tag = static_cast<Node*>(pSender)->getTag();
    if (type == ControlButton::EventType::TOUCH_DOWN)
    {
        switch (tag)
        {
            case 100:
                schedule(schedule_selector(HelloWorld::updateLeft), 0.1);
                cocos2d::experimental::AudioEngine::play2d("sound/s_move.wav",false,m_effect_volume);
                break;
            case 101:
                schedule(schedule_selector(HelloWorld::updateRight), 0.1);
                cocos2d::experimental::AudioEngine::play2d("sound/s_move.wav",false,m_effect_volume);
                break;
            case 102:
                schedule(schedule_selector(HelloWorld::touchDownCallBack), 0.03);
                break;
            default:
                break;
        }
    }
    else if (type == ControlButton::EventType::TOUCH_UP_INSIDE)
    {
        switch (tag)
        {
            case 100:
                updateLeft(0);
                break;
            case 101:
                updateRight(0);
                break;
            case 102:
//                updateDown(0);
                break;
            default:
                break;
        }
    }
    if (type == ControlButton::EventType::TOUCH_UP_INSIDE
        ||type == ControlButton::EventType::TOUCH_UP_OUTSIDE
        ||type == ControlButton::EventType::TOUCH_CANCEL
        ||type == ControlButton::EventType::DRAG_OUTSIDE)
    {
        switch (tag)
        {
            case 100:
                unschedule(schedule_selector(HelloWorld::updateLeft));
                break;
            case 101:
                unschedule(schedule_selector(HelloWorld::updateRight));
                break;
            case 102:
                unschedule(schedule_selector(HelloWorld::touchDownCallBack));
                break;
            default:
                break;
        }
    }
}

void HelloWorld::touchDownCallBack(float dt)
{
    updateDown(0);
}

void HelloWorld::newTetris()
{
    switch (m_cur_type)
    {
        case 0:
            m_max_row = MAX(m_max_row,m_cur_row + 3);
            break;
        case 1:
            m_max_row = MAX(m_max_row,m_cur_row);
            break;
        case 2:
        case 4:
        case 6:
        case 8:
        case 10:
        case 12:
        case 14:
        case 16:
        case 18:
            m_max_row = MAX(m_max_row,m_cur_row + 1);
            break;
        case 3:
        case 5:
        case 7:
        case 9:
        case 11:
        case 13:
        case 15:
        case 17:
            m_max_row = MAX(m_max_row,m_cur_row + 2);
            break;
        default:
            break;
    }
    if (m_max_row >= ROW)
    {
        gameOver();
        return;
    }
    removeFullRow();
    unschedule(schedule_selector(HelloWorld::touchDownCallBack));
    m_cur_type = m_next_type;
    m_next_type = random(0, 18);
    m_cur_row = ROW;
    m_cur_col = 4;
    updateDown(0);
}

void HelloWorld::updateUI()
{
    char pic_name[64];
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            sprintf(pic_name, "%d.png",m_num_map[row][col].color);
            m_sprite_map[row][col]->setTexture(pic_name);
        }
    }
    auto strScore = __String::createWithFormat("%d",m_score);
    m_score_label->setString(strScore->getCString());
    if (m_score > m_high_score)
    {
        m_high_score_label->setString(strScore->getCString());
    }
    
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if(tetris[m_next_type].box[r][c])
            {
                sprintf(pic_name, "%d.png",tetris[m_next_type].color);
                m_next_tetris[r][c]->setTexture(pic_name);
            }
            else
            {
                m_next_tetris[r][c]->setTexture("0.png");
            }
        }
    }
}

void HelloWorld::reStart()
{
    for (int row = 0; row < ROW; row++)
    {
        for (int col = 0; col < COL; col++)
        {
            m_num_map[row][col].var = 0;
            m_num_map[row][col].color = 0;
        }
    }
    m_cur_type = -1;
    m_max_row = -1;
    delay_time = 1.0f;
    m_total_row = 0;
    m_score = 0;
    m_next_type = random(0, 18);
    updateUI();
    setAutoDownSpeed(delay_time);
    newTetris();
    static_cast<ControlButton*>(this->getChildByTag(100))->setEnabled(true);
    static_cast<ControlButton*>(this->getChildByTag(101))->setEnabled(true);
    static_cast<ControlButton*>(this->getChildByTag(102))->setEnabled(true);
    static_cast<ui::Button*>(this->getChildByTag(10))->setEnabled(true);
    cocos2d::experimental::AudioEngine::stopAll();
    cocos2d::experimental::AudioEngine::play2d("sound/m_game_bg.mp3",true);
}


void HelloWorld::removeFullRow()
{
    do{
        CC_BREAK_IF(m_max_row < 0);
        int rowNum = 0;
        int firstRow = -1;
        for (int row = m_cur_row; row <= MIN(m_max_row,m_cur_row + 3); )
        {
            CC_BREAK_IF(m_cur_row >= ROW || m_cur_row < 0);
            bool bRemove = false;
            for (int col = 0; col < COL; col++)
            {
                if(!m_num_map[row][col].var)
                {
                    break;
                }
                if(col == COL - 1)
                {
                    if (!rowNum)
                    {
                        firstRow = row;
                    }
                    rowNum++;
                    bRemove = true;
                    for (int r = row; r <= m_max_row; r++)
                    {
                        for (int c = 0; c < COL ; c ++)
                        {
                            m_num_map[r][c].var = m_num_map[r + 1][c].var;
                            m_num_map[r][c].color = m_num_map[r + 1][c].color;
                        }
                    }
                }
            }
            if (!bRemove)
            {
                row++;
            }
        }
        if (rowNum)
        {
            auto cleanRow = Sprite::create("ico_clean_row.png");
            m_panle_bg->addChild(cleanRow);
            cleanRow->setPosition(START_X + 2 * CELL_SIZE, START_Y + firstRow * CELL_SIZE);
            cleanRow->setScale(0.5f);
            cleanRow->runAction(Sequence::create(MoveBy::create(0.2f, Vec2(CELL_SIZE*8,0)),RemoveSelf::create(0.1f), NULL));
            cocos2d::experimental::AudioEngine::play2d("sound/s_clean.mp3",false,m_effect_volume);
            int add_score;
            switch (rowNum)
            {
                case 1:
                    add_score = 10;
                    break;
                case 2:
                    add_score = 30;
                    break;
                case 3:
                    add_score = 60;
                    break;
                case 4:
                    add_score = 100;
                default:
                    break;
            }
            m_total_row += rowNum;
            m_score += add_score;
            auto strAdd = __String::createWithFormat("+ %d",add_score);
            auto add_label = Label::createWithSystemFont(strAdd->getCString(), "", 30);
            addChild(add_label);
            add_label->setColor(Color3B::RED);
            add_label->setPosition(_director->getWinSize()/2);
            add_label->runAction(Sequence::create(ScaleTo::create(0.5f, 5.0f),RemoveSelf::create(0.1f), NULL));
            if (m_total_row > 1000)
            {
                delay_time = 0.1f;
            }
            else if(m_total_row > 700)
            {
                delay_time = 0.2f;
            }
            else if(m_total_row > 400)
            {
                delay_time = 0.3f;
            }
            else if(m_total_row > 200)
            {
                delay_time = 0.4f;
            }
            else if(m_total_row > 100)
            {
                delay_time = 0.6f;
            }
            else if(m_total_row > 50)
            {
                delay_time = 0.8f;
            }
            setAutoDownSpeed(delay_time);
        }
        
    }while(0);
}
bool HelloWorld::checkNextTepDown()
{
    bool bDown = true;
    do
    {
        if(m_cur_row <= 0)
        {
            bDown = false;
            break;
        }
        switch (m_cur_type)
        {
            case 0:
            {
                if(m_num_map[m_cur_row - 1][m_cur_col].var)
                {
                    bDown = false;
                }
            }
                break;
            case 1:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 3].var)
                {
                    bDown = false;
                }
            }
                break;
            case 2:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var)
                {
                    bDown = false;
                }
            }
                break;
            case 3:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var))
                {
                    bDown = false;
                }
            }
                break;
            case 4:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 2].var
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col].var)
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col + 1].var))
                {
                    bDown = false;
                }
            }
                break;
            case 5:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var)
                {
                    bDown = false;
                }
            }
                break;
            case 6:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var)
                {
                    bDown = false;
                }
            }
                break;
            case 7:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_num_map[m_cur_row - 1][m_cur_col + 1].var))
                {
                    bDown = false;
                }
            }
                break;
            case 8:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col + 1].var)
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col + 2].var))
                {
                    bDown = false;
                }
            }
                break;
            case 9:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var))
                {
                    bDown = false;
                }
            }
                break;
            case 10:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 2].var))
                {
                    bDown = false;
                }
            }
                break;
            case 11:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var))
                {
                    bDown = false;
                }
            }
                break;
            case 12:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var))
                {
                    bDown = false;
                }
            }
                break;
            case 13:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 1].var))
                {
                    bDown = false;
                }
            }
                break;
            case 14:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var)
                {
                    bDown = false;
                }
            }
                break;
            case 15:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 1].var))
                {
                    bDown = false;
                }
            }
                break;
            case 16:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var)
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 2].var))
                {
                    bDown = false;
                }
            }
                break;
            case 17:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var))
                {
                    bDown = false;
                }
            }
                break;
            case 18:
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var)
                {
                    bDown = false;
                }
                break;
            default:
                break;
        }
    }while(0);
    return bDown;
}
void HelloWorld::updateDown(float dt)
{
    do{
        m_is_down = false;
        m_game_over = false;
        if (m_cur_row <= 0)
        {
            newTetris();
            break;
        }
        switch (m_cur_type)
        {
            case 0:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var)
                {
                    if (m_cur_row >= ROW - 4)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                if (m_cur_row > ROW - 4)
                {
                    for (int i = m_cur_row - 1; i < ROW; i++)
                    {
                        m_num_map[i][m_cur_col].var = 1;
                        m_num_map[i][m_cur_col].color = tetris[m_cur_type].color;
                    }
                }
                else
                {
                    m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                    m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                    m_num_map[m_cur_row + 3][m_cur_col].var = 0;
                    m_num_map[m_cur_row + 3][m_cur_col].color = 0;
                }
            }
                break;
            case 1:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 3].var)
                {
                    m_is_down = false;
                    if (m_cur_row >= ROW - 1)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 3].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                m_num_map[m_cur_row][m_cur_col + 3].var = 0;
                m_num_map[m_cur_row][m_cur_col + 3].color = 0;
            }
                break;
            case 2:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var)
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 3:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                if (m_cur_row >= ROW - 2)
                {
                    for (int i = m_cur_row - 1; i < ROW; i++)
                    {
                        m_num_map[i][m_cur_col].var = 1;
                        m_num_map[i][m_cur_col].color = tetris[m_cur_type].color;
                        if(m_cur_row == ROW - 2 && i == ROW - 1)
                        {
                            m_num_map[i][m_cur_col + 1].var = 1;
                            m_num_map[i][m_cur_col + 1].color = tetris[m_cur_type].color;
                        }
                    }
                }
                else
                {
                    m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                    m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                    m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                    m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                    
                    m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                    m_num_map[m_cur_row + 2][m_cur_col].color = 0;
                    m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                    m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
                }
            }
                break;
            case 4:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 2].var
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col].var)
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col + 1].var))
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 5:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var)
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row >= ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 6:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var)
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 7:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_num_map[m_cur_row - 1][m_cur_col + 1].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row >= ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 8:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col + 1].var)
                    ||(m_cur_row < ROW &&m_num_map[m_cur_row][m_cur_col + 2].var))
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 9:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                if (m_cur_row >= ROW - 2)
                {
                    for (int i = m_cur_row - 1; i < ROW; i++)
                    {
                        m_num_map[i][m_cur_col + 1].var = 1;
                        m_num_map[i][m_cur_col + 1].color = tetris[m_cur_type].color;
                        if(m_cur_row == ROW - 2 && i == ROW - 1)
                        {
                            m_num_map[i][m_cur_col].var = 1;
                            m_num_map[i][m_cur_col].color = tetris[m_cur_type].color;
                        }
                    }
                }
                else
                {
                    m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                    m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                    m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                    m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                    
                    m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                    m_num_map[m_cur_row + 2][m_cur_col].color = 0;
                    m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                    m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
                }
        }
                break;
            case 10:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 2].var))
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 11:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 12:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var))
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
        }
                break;
            case 13:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 1].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 14:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 2].var)
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
            }
                break;
            case 15:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 1].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 16:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var)
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col + 2].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 17:
            {
                if (m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW && m_num_map[m_cur_row][m_cur_col].var))
                {
                    if (m_cur_row >= ROW - 3)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW);
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 18:
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var)
                {
                    if (m_cur_row >= ROW - 2)
                    {
                        m_game_over = true;
                    }
                    break;
                }
                m_is_down = true;
                m_num_map[m_cur_row - 1][m_cur_col].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row >= ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                break;
            default:
                break;
        }
        
        if (m_game_over)
        {
            gameOver();
            break;
        }
        if (!m_is_down)
        {
            newTetris();
            break;
        }
        m_cur_row--;
        updateUI();
        if (isScheduled(schedule_selector(HelloWorld::touchDownCallBack))
            && !checkNextTepDown())
        {
            cocos2d::experimental::AudioEngine::play2d("sound/s_fast_down.wav",false,m_effect_volume);
            m_panle_bg->runAction(Sequence::create(MoveBy::create(0.05f, Vec2(0,10)),
                                                   MoveBy::create(0.05f, Vec2(0,-10)),nullptr));
        }
    }while (0);

}

void HelloWorld::updateLeft(float dt)
{
    do
    {
        CC_BREAK_IF(m_cur_row == ROW);
        CC_BREAK_IF(m_cur_col < 1);
        bool b_moved = false;
        switch (m_cur_type)
        {
            case 0:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 3 && m_num_map[m_cur_row + 3][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
                m_num_map[m_cur_row + 2][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col - 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 3);
                m_num_map[m_cur_row + 3][m_cur_col].var = 0;
                m_num_map[m_cur_row + 3][m_cur_col].color = 0;
                m_num_map[m_cur_row + 3][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 3][m_cur_col - 1].color = tetris[m_cur_type].color;
            }
                break;
            case 1:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var)
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 3].var = 0;
                m_num_map[m_cur_row][m_cur_col + 3].color = 0;
            }
                break;
            case 2:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 3:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 4:
            {
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 5:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 6:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 7:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 8:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 9:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 10:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 11:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 12:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
            }
                break;
            case 13:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 14:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
            }
                break;
            case 15:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 16:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 17:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 18:
            {
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col - 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
            }
                break;
            default:
                break;
        }
        if (b_moved)
        {
            m_cur_col--;
            updateUI();
        }
    }while(0);
}
void HelloWorld::updateRight(float dt)
{
    do
    {
        CC_BREAK_IF(m_cur_row == ROW);
        CC_BREAK_IF(m_cur_col >= COL - 1);
        bool b_moved = false;
        switch (m_cur_type)
        {
            case 0:
            {
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 3 && m_num_map[m_cur_row + 3][m_cur_col + 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 3);
                m_num_map[m_cur_row + 3][m_cur_col].var = 0;
                m_num_map[m_cur_row + 3][m_cur_col].color = 0;
                m_num_map[m_cur_row + 3][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 3][m_cur_col + 1].color = tetris[m_cur_type].color;
            }
                break;
            case 1:
            {
                CC_BREAK_IF(m_cur_col >= COL - 4);
                if (m_num_map[m_cur_row][m_cur_col + 4].var)
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 4].var = 1;
                m_num_map[m_cur_row][m_cur_col + 4].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
            }
                break;
            case 2:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 3].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 3:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 4:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 3].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 3].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 5:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 6:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 3].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 3].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
            }
                break;
            case 7:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 8:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 3].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 9:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 10:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 3].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
            }
                break;
            case 11:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 12:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 3].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 13:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 14:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 3].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
            }
                break;
            case 15:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 16:
            {
                CC_BREAK_IF(m_cur_col >= COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 3].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 3].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 3].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            case 17:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 18:
            {
                CC_BREAK_IF(m_cur_col >= COL - 2);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var))
                {
                    break;
                }
                b_moved = true;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
            }
                break;
            default:
                break;
        }
        if (b_moved)
        {
            m_cur_col++;
            updateUI();
        }
    }while(0);
}

void HelloWorld::updateNextType()
{
    do
    {
        CC_BREAK_IF(m_cur_row == ROW);
        bool is_next_type = false;
        switch (m_cur_type)
        {
            case 0:
            {
                CC_BREAK_IF(m_cur_row == ROW - 1);
                CC_BREAK_IF(m_cur_col == 0 || m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col - 1].var
                    ||m_num_map[m_cur_row + 1][m_cur_col - 1].var
                    ||m_num_map[m_cur_row + 1][m_cur_col + 1].var
                    ||m_num_map[m_cur_row + 1][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 3 && m_num_map[m_cur_row + 3][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 3 && m_num_map[m_cur_row + 3][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row + 1][m_cur_col - 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col - 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_cur_row++;
                m_cur_col--;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 1:
            {
                CC_BREAK_IF(m_cur_row < 1);
                if (m_num_map[m_cur_row - 1][m_cur_col].var
                    ||m_num_map[m_cur_row - 1][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 3].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                
                m_num_map[m_cur_row - 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row - 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                m_num_map[m_cur_row][m_cur_col + 3].var = 0;
                m_num_map[m_cur_row][m_cur_col + 3].color = 0;
                m_cur_row--;
                m_cur_col++;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 3);
                m_num_map[m_cur_row + 3][m_cur_col].var = 1;
                m_num_map[m_cur_row + 3][m_cur_col].color = tetris[m_cur_type].color;
            }
                break;
            case 2:
            {
                if ((m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row >= ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
            }
                break;
            case 3:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 4:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
            }
                break;
            case 5:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var)
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 6:
            {
                if ((m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
            }
                break;
            case 7:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if ((m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 8:
            {
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
            }
                break;
            case 9:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 10:
            {
                if ((m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
            }
                break;
            case 11:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 12:
            {
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row >= ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
            }
                break;
            case 13:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row >= ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            case 14:
            {
                if ((m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row][m_cur_col + 1].color = 0;
                m_num_map[m_cur_row][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col].color = tetris[m_cur_type].color;
            }
                break;
            case 15:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col + 1].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 0;
                m_num_map[m_cur_row][m_cur_col].color = 0;
                m_num_map[m_cur_row][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row][m_cur_col + 1].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col].color = 0;
            }
                break;
            case 16:
            {
                if (m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 1].var)
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col + 2].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col + 2].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 1;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = tetris[m_cur_type].color;
            }
                break;
            case 17:
            {
                CC_BREAK_IF(m_cur_col > COL - 3);
                if (m_num_map[m_cur_row][m_cur_col].var
                    ||m_num_map[m_cur_row][m_cur_col + 2].var
                    ||(m_cur_row < ROW - 1 && m_num_map[m_cur_row + 1][m_cur_col + 2].var)
                    ||(m_cur_row < ROW - 2 && m_num_map[m_cur_row + 2][m_cur_col + 2].var))
                {
                    break;
                }
                is_next_type = true;
                m_cur_type = tetris[m_cur_type].next_type;
                m_num_map[m_cur_row][m_cur_col].var = 1;
                m_num_map[m_cur_row][m_cur_col].color = tetris[m_cur_type].color;
                m_num_map[m_cur_row][m_cur_col + 2].var = 1;
                m_num_map[m_cur_row][m_cur_col + 2].color = tetris[m_cur_type].color;
                CC_BREAK_IF(m_cur_row == ROW - 1);
                m_num_map[m_cur_row + 1][m_cur_col].var = 0;
                m_num_map[m_cur_row + 1][m_cur_col].color = 0;
                CC_BREAK_IF(m_cur_row == ROW - 2);
                m_num_map[m_cur_row + 2][m_cur_col + 1].var = 0;
                m_num_map[m_cur_row + 2][m_cur_col + 1].color = 0;
            }
                break;
            default:
                break;
        }
        if (is_next_type)
        {
            updateUI();
        }
    }while(0);
}

void HelloWorld::setAutoDownSpeed(float speed)
{
    delay_time = speed;
    unschedule(schedule_selector(HelloWorld::updateDown));
    schedule(schedule_selector(HelloWorld::updateDown), delay_time);
}

void HelloWorld::gameOver()
{
    this->unschedule(schedule_selector(HelloWorld::touchDownCallBack));
    this->unschedule(schedule_selector(HelloWorld::updateDown));
    static_cast<ControlButton*>(this->getChildByTag(100))->setEnabled(false);
    static_cast<ControlButton*>(this->getChildByTag(101))->setEnabled(false);
    static_cast<ControlButton*>(this->getChildByTag(102))->setEnabled(false);
    static_cast<ui::Button*>(this->getChildByTag(10))->setEnabled(false);
    cocos2d::experimental::AudioEngine::stopAll();
    cocos2d::experimental::AudioEngine::play2d("sound/s_gameover.wav",false,m_effect_volume);
    if (m_score > m_high_score)
    {
        UserDefault::getInstance()->setIntegerForKey("high_score", m_score);
    }
    log("game over");
    
    auto gameOver = GameOver::create();
    this->addChild(gameOver,200);
}