#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// resolution size for this game is 960 * 640 (in pixel)
static cocos2d::Size designResolutionSize = cocos2d::Size(960, 640);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(960, 640);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

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
		glview = GLViewImpl::createWithRect("BombingAdventure", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));

#else
        glview = GLViewImpl::create("BombingAdventure");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    // create the first scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

	// run scene 
    director->runWithScene(scene);

	// background music and effect preloaded
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music&effect/HelloMusic.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/BubbleSet.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/BubbleBoom.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/pickItem.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/monster_dead.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/hero_injured.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/game_over.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect("music&effect/win_game.mp3");

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
}
