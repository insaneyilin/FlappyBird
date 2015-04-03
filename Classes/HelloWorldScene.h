#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	void initBG();        // 初始化背景
	void addBird();     // 添加小鸟精灵
	void addGround();    // 添加地面精灵

	void addTouchListeners();              // 添加触摸事件监听
	void addContactTestListeners();     // 添加碰撞检测事件监听

	void addBars(float dt);    // 定时器回调函数，每隔1.5秒添加一对水管
	virtual void update(float delta);

private:
	cocos2d::Size _visibleSize;    // 窗口大小
	cocos2d::Vec2 _origin;    // 窗口原点
	cocos2d::Sprite *_birdSprite;    // 小鸟精灵
	cocos2d::Sprite *_groundSprite;    // 地面精灵
	cocos2d::Vector<cocos2d::Sprite*> _barVec;    // 存放水管精灵的数组
};

#endif // __HELLOWORLD_SCENE_H__
