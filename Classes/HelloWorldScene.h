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
	void initBG();        // ��ʼ������
	void addBird();     // ���С����
	void addGround();    // ��ӵ��澫��

	void addTouchListeners();              // ��Ӵ����¼�����
	void addContactTestListeners();     // �����ײ����¼�����

	void addBars(float dt);    // ��ʱ���ص�������ÿ��1.5�����һ��ˮ��
	virtual void update(float delta);

private:
	cocos2d::Size _visibleSize;    // ���ڴ�С
	cocos2d::Vec2 _origin;    // ����ԭ��
	cocos2d::Sprite *_birdSprite;    // С����
	cocos2d::Sprite *_groundSprite;    // ���澫��
	cocos2d::Vector<cocos2d::Sprite*> _barVec;    // ���ˮ�ܾ��������
};

#endif // __HELLOWORLD_SCENE_H__
