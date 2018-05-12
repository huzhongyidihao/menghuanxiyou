#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "GameSystem.h"
// #define USE_AUDIO_ENGINE 1
 #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);//设计大小,同sd尺寸
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);//标准尺寸sd     sd尺寸资源
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);//中等尺寸 md   md尺寸资源
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);//大屏尺寸 hd   hd尺寸资源

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		log("need creat window");
        glview = GLViewImpl::createWithRect("WuXiaGame", cocos2d::Rect(0, 0, 800/*designResolutionSize.width*/,600/* designResolutionSize.height*/));
		log("have creat window");
#else
		CCLOG("VisibleSize: width=%f,height=%f");
		log("there is not windows system,so create app use other");
		auto tempsize = glview->getFrameSize();
		log("width=%f,height=%f", tempsize.width, tempsize.height);
        glview = GLViewImpl::create("WuXiaGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);


	
    
	//屏幕大小
	auto frameSize = glview->getFrameSize();
	/*-----------------屏幕适配-----------*/
	//根据屏幕尺寸 选择合适的资源尺寸
    // if the frame's height is larger than the height of medium size.
  //  if (frameSize.height > mediumResolutionSize.height)//如果屏幕尺寸大于md 则采用hd尺寸 使用hd资源
  //  {        
  //      director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
		//log("use hd");
  //  }
  //  // if the frame's height is larger than the height of small size.
  //  else if (frameSize.height > smallResolutionSize.height)
  //  {        
  //      director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
		//log("use md");
  //  }
  //  // if the frame's height is smaller than the height of medium size.
  //  else
  //  {        
  //      director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
		//log("use sd");
  //  }

	// Set the design resolution

	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);


    register_all_packages();

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	auto scene = GameSystem::CreateScene();
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
