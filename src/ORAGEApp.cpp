#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "OrageMenu.h"
#include "ModuleManager.h"

using namespace ci;
using namespace ci::gl;
using namespace ci::app;
using namespace ORAGE::COMMON;

class ORAGEApp : public App {
    gl::Context * mMainWinCtx;
    ModuleManagerRef modules;
    OrageMenuRef menu;
    std::string orageFilePath = getDocumentsDirectory().generic_string() + "/ORAGE";
    
    
public:
    static void prepare( Settings *settings ){
        
    #if defined(CINDER_MAC)
        std::string logFile = "/Users/ogre/Documents/Orage/orage.log";
        std::string errorFile = "/Users/ogre/Documents/Orage/orage.err.log";
        
        std::freopen( &logFile[0], "w", stdout );
        std::freopen( &errorFile[0], "w", stderr );
    #elif defined(CINDER_MSW)
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    #endif
        
        settings->setTitle("ORAGE - VISUAL MODULAR SYNTHESIS");
        
        auto args = settings->getCommandLineArgs();
        for(auto arg : args){
            std::cout << arg << std::endl;
        }
//        settings->setWindowSize(1280, 720);
    }
    void setup() override {
       
        
        
        if(!fs::exists( orageFilePath )){
            fs::create_directories(orageFilePath);
        }
        
        
        
        mMainWinCtx = Context::getCurrent();
        modules = ModuleManager::create();
        menu = OrageMenu::create();
        menu->addEventListener([&](EvtMenu evt){
            if(evt.is("menu")){
                modules->add(evt.target, evt.origin, evt.moduleType);
            }
        });
        enableVerticalSync( false );
        disableAlphaBlending();
    };
    void update() override {
        modules->update();
        menu->update();
    };
    void draw() override {
        if(mMainWinCtx != gl::Context::getCurrent()) return;
        clear( Color( 0, 0, 0 ) );
        modules->draw();
        menu->draw();
    };
    void fileDrop(FileDropEvent evt) override {
    };
    
    void keyDown( KeyEvent evt ) override {
        
        if(evt.getChar() == 's' && evt.isMetaDown()){
            modules->save(orageFilePath);
        }
    }
};

CINDER_APP( ORAGEApp, RendererGl, &ORAGEApp::prepare )
