#include "AppDelegate.h"
#include "Global/Marcos.h"
#include "View/GameScene.h"
#include "Model/GameModel.h"
#include "Global/VisibleRect.h"


USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) 
	{
#if (IS_WIN32)
	    Rect size(0.0f, 0.0f, WIN_WIDTH, WIN_HEIGHT);
        glview = GLViewImpl::createWithRect("match-three-game", size, FRAME_ZOOM_FACTOR);
#else
		glview = GLViewImpl::create("match-three-game");
#endif
		director->setOpenGLView(glview);
    }
	setResouceSearchPaths();
	Rect viewRect = VisibleRect::getRect();
	CCLOG("scr: %f,%f ", viewRect.size.width, viewRect.size.height);
	if (viewRect.size.width / 640.0f < viewRect.size.height / 960.0f)
		glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::FIXED_WIDTH);
	else
		glview->setDesignResolutionSize(DESIGN_WIDTH, DESIGN_HEIGHT, ResolutionPolicy::FIXED_HEIGHT);

	// set 30 fps for mobile
	director->setAnimationInterval(1.0 / LOGIC_FPS);

	GameModel::loadLevelsConstant();
	auto gameScene = GameScene::create();
	director->runWithScene(gameScene);
	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::setResouceSearchPaths()
{
	std::vector<std::string> paths;
	paths.push_back("fonts");
	paths.push_back("Grid.atlas");
	paths.push_back("Images");
	paths.push_back("Levels");
	paths.push_back("Sounds");
	paths.push_back("Sprites.atlas");
	FileUtils::getInstance()->setSearchResolutionsOrder(paths);
}
