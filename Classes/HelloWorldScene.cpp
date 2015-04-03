// HelloWorldScene.cpp

#include "HelloWorldScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();  // 创建有物理世界的 Scene
	//scene->getPhysicsWorld()->
	//	setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	// 设置物理世界重力
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

	// 添加精灵时要注意添加顺序！

	initBG();  // 加载图片，初始化背景

    addGround();    // 添加物理世界地面
	addBird();    // 添加小鸟精灵

	addContactTestListeners();    // 添加碰撞检测事件监听
	addTouchListeners();    // 添加触摸事件监听

	scheduleUpdate();    // 启动定时器

	// 设置 HelloWorld::addBars() 方法每 1.5 秒执行一次
	schedule(schedule_selector(HelloWorld::addBars), 1.5f);

    return true;
}

void HelloWorld::initBG()
{
	// 从图片中创建地面精灵
	_groundSprite = Sprite::create("ground.png");
	_groundSprite->setPosition(_visibleSize.width / 2, 
		_groundSprite->getContentSize().height / 2);
	addChild(_groundSprite);    // 添加到当前场景中
}

void HelloWorld::addBird()
{
	_birdSprite = Sprite::create("bird.png");  // 从图片中创建小鸟精灵
	// 为小鸟精灵设置物理刚体
	_birdSprite->setPhysicsBody(
		PhysicsBody::createBox(_birdSprite->getContentSize()));
	_birdSprite->setPosition(_visibleSize/2);
	// 禁止小鸟精灵的刚体旋转
	_birdSprite->getPhysicsBody()->setRotationEnable(false);
	// 设置回复力为0
	_birdSprite->getPhysicsBody()->getShape(0)->setRestitution(0.0f);
	// 设置摩擦力为0
	_birdSprite->getPhysicsBody()->getShape(0)->setFriction(0.0f);

	// 设置碰撞检测标识（这里必须设为1，后面的地面和水管也是）
	_birdSprite->getPhysicsBody()->setContactTestBitmask(1);

	addChild(_birdSprite);
}

void HelloWorld::addGround()
{
	// 创建地面刚体
	auto groundBody = PhysicsBody::createEdgeSegment(_origin, 
		Vec2(_visibleSize.width, 0), PHYSICSBODY_MATERIAL_DEFAULT, 1);

	//  创建地面 Node 对象并绑定地面刚体
	auto groundNode = Node::create();
	groundNode->setPhysicsBody(groundBody);
	groundNode->setPosition(0, _groundSprite->getContentSize().height);

	// 为地面刚体设置碰撞检测标识
	groundNode->getPhysicsBody()->setContactTestBitmask(1);

	addChild(groundNode);
}

void HelloWorld::addBars(float dt)
{	
	// 生成一个水管垂直方向的位置偏移
	float offset = -50 - rand() % 95;

	// 创建下方的水管精灵
	auto downBar = Sprite::create("down_bar.png");
	auto downBarSize = downBar->getContentSize();
	downBar->setPhysicsBody(
		PhysicsBody::createBox(downBarSize));
	// 水管不受物理重力影响（不会受小鸟碰撞影响）
	downBar->getPhysicsBody()->setDynamic(false);

	downBar->setPositionX(_visibleSize.width + 2);
	downBar->setPositionY(downBarSize.height / 2 + offset);

	downBar->getPhysicsBody()->setContactTestBitmask(1);

	addChild(downBar);
	_barVec.pushBack(downBar);  // 将水管精灵添加到数组中

	// 创建上方的水管精灵
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
	// 所有水管精灵的位置都向左侧移动
	for (auto &bar : _barVec)
	{
		bar->setPositionX(bar->getPositionX()-2);
	}

	// 判断是否有水管位置离开窗口范围
	// 弱有，将其从父层中移除，同时从数组中移除
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

// 添加触摸事件监听
void HelloWorld::addTouchListeners()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [this](Touch *t, Event *e)
	{
		// 给小鸟精灵一个向上的速度
		_birdSprite->getPhysicsBody()->setVelocity(Vec2(0, 300));
		return false;
	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(touchListener, this);
}

// 添加碰撞检测事件监听
void HelloWorld::addContactTestListeners()
{
	auto contactTestListener = EventListenerPhysicsContact::create();
	contactTestListener->onContactBegin = [this](PhysicsContact &contact)
	{
		// 停止定时器
		this->unscheduleUpdate();
		this->unschedule(schedule_selector(HelloWorld::addBars));
		Director::getInstance()->replaceScene(GameOver::createScene());
		return false;
	};
	Director::getInstance()->getEventDispatcher()->
		addEventListenerWithSceneGraphPriority(contactTestListener, this);
}
