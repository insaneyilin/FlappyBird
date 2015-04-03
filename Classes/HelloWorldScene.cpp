// HelloWorldScene.cpp

#include "HelloWorldScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();  // ��������������� Scene
	//scene->getPhysicsWorld()->
	//	setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// ����������������
    scene->getPhysicsWorld()->setGravity(Vec2(0, -720));

    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // super init first
    if ( !LayerColor::initWithColor(Color4B(243, 248, 241, 255)) )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();

	// ��Ӿ���ʱҪע�����˳��

	initBG();  // ����ͼƬ����ʼ������

    addGround();    // ��������������
	addBird();    // ���С����

	addContactTestListeners();    // �����ײ����¼�����
	addTouchListeners();    // ��Ӵ����¼�����

	scheduleUpdate();    // ������ʱ��

	// ���� HelloWorld::addBars() ����ÿ 1.5 ��ִ��һ��
	schedule(schedule_selector(HelloWorld::addBars), 1.5f);

    return true;
}

void HelloWorld::initBG()
{
	// ��ͼƬ�д������澫��
	_groundSprite = Sprite::create("ground.png");
	_groundSprite->setPosition(_visibleSize.width / 2, 
		_groundSprite->getContentSize().height / 2);
	addChild(_groundSprite);    // ��ӵ���ǰ������
}

void HelloWorld::addBird()
{
	_birdSprite = Sprite::create("bird.png");  // ��ͼƬ�д���С����
	// ΪС���������������
	_birdSprite->setPhysicsBody(
		PhysicsBody::createBox(_birdSprite->getContentSize()));
	_birdSprite->setPosition(_visibleSize/2);
	// ��ֹС����ĸ�����ת
	_birdSprite->getPhysicsBody()->setRotationEnable(false);
	// ���ûظ���Ϊ0
	_birdSprite->getPhysicsBody()->getShape(0)->setRestitution(0.0f);
	// ����Ħ����Ϊ0
	_birdSprite->getPhysicsBody()->getShape(0)->setFriction(0.0f);

	// ������ײ����ʶ�����������Ϊ1������ĵ����ˮ��Ҳ�ǣ�
	_birdSprite->getPhysicsBody()->setContactTestBitmask(1);

	addChild(_birdSprite);
}

void HelloWorld::addGround()
{
	// �����������
	auto groundBody = PhysicsBody::createEdgeSegment(_origin, 
		Vec2(_visibleSize.width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);

	//  �������� Node ���󲢰󶨵������
	auto groundNode = Node::create();
	groundNode->setPhysicsBody(groundBody);
	groundNode->setPosition(0, _groundSprite->getContentSize().height);

	// Ϊ�������������ײ����ʶ
	groundNode->getPhysicsBody()->setContactTestBitmask(1);

	addChild(groundNode);
}

void HelloWorld::addBars(float dt)
{	
	// ����һ��ˮ�ܴ�ֱ�����λ��ƫ��
	float offset = -50 - rand() % 95;

	// �����·���ˮ�ܾ���
	auto downBar = Sprite::create("down_bar.png");
	auto downBarSize = downBar->getContentSize();
	downBar->setPhysicsBody(
		PhysicsBody::createBox(downBarSize));
	// ˮ�ܲ�����������Ӱ�죨������С����ײӰ�죩
	downBar->getPhysicsBody()->setDynamic(false);

	downBar->setPositionX(_visibleSize.width + 2);
	downBar->setPositionY(downBarSize.height / 2 + offset);

	downBar->getPhysicsBody()->setContactTestBitmask(1);

	addChild(downBar);
	_barVec.pushBack(downBar);  // ��ˮ�ܾ�����ӵ�������

	// �����Ϸ���ˮ�ܾ���
	auto upBar = Sprite::create("up_bar.png");
	auto upBarSize = upBar->getContentSize();
	upBar->setPhysicsBody(
		PhysicsBody::createBox(upBarSize));
	upBar->getPhysicsBody()->setDynamic(false);

	upBar->setPositionX(_visibleSize.width + 2);
	upBar->setPositionY(downBar->getPositionY() + 
		upBarSize.height / 2 + downBarSize.height / 2 + 130);

	upBar->getPhysicsBody()->setContactTestBitmask(1);

	addChild(upBar);
	_barVec.pushBack(upBar);
}

void HelloWorld::update(float delta)
{
	// ����ˮ�ܾ����λ�ö�������ƶ�
	for (auto &bar : _barVec)
	{
		bar->setPositionX(bar->getPositionX()-2);
	}

	// �ж��Ƿ���ˮ��λ���뿪���ڷ�Χ
	// ���У�����Ӹ������Ƴ���ͬʱ���������Ƴ�
	for (auto &bar : _barVec)
	{
		if (bar->getPositionX() < -_visibleSize.width / 2)
		{
			bar->removeFromParent();
			_barVec.eraseObject(bar);
			break;
		}
	}
}

// ��Ӵ����¼�����
void HelloWorld::addTouchListeners()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [this](Touch *t, Event *e)
	{
		// ��С����һ�����ϵ��ٶ�
		_birdSprite->getPhysicsBody()->setVelocity(Vec2(0, 300));
		return false;
	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(touchListener, this);
}

// �����ײ����¼�����
void HelloWorld::addContactTestListeners()
{
	auto contactTestListener = EventListenerPhysicsContact::create();
	contactTestListener->onContactBegin = [this](PhysicsContact &contact)
	{
		// ֹͣ��ʱ��
		this->unscheduleUpdate();
		this->unschedule(schedule_selector(HelloWorld::addBars));
		Director::getInstance()->replaceScene(GameOver::createScene());
		return false;
	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(contactTestListener, this);
}
