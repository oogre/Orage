//
//  Mosher.cpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-20.
//
//

#include "Mosher.h"



using namespace ogre;
using namespace cinder;
using namespace cinder::gl;
using namespace std;
using namespace reza::ui;

namespace ogre {
    
    int Mosher::COUNT = 0;
    
    Mosher::Mosher( std::string name, JsonTree jsonData, vec2 origin, vec2 size, gl::Context * mMainWinCtx ) : ModuleVideo(name+" "+ tools.to_roman(Mosher::COUNT), origin, size, 2, 1, true){
        if(jsonData.getNumChildren()!=0){
            data = DATA(jsonData);
        }
        this->mMainWinCtx = mMainWinCtx;
    }
    
    void Mosher::setup(){
        ModuleVideo::setup();
        mFbo = gl::Fbo::create(FBO_WIDTH,
                               FBO_HEIGHT,
                               gl::Fbo::Format()
                               .attachment(GL_COLOR_ATTACHMENT0,
                                           gl::Texture2d::create(FBO_WIDTH,
                                                                 FBO_HEIGHT)));
        
        mFbo2 = gl::Fbo::create(FBO_WIDTH,
                                FBO_HEIGHT,
                                gl::Fbo::Format()
                                .attachment(GL_COLOR_ATTACHMENT0,
                                            gl::Texture2d::create(FBO_WIDTH,
                                                                  FBO_HEIGHT)));
        
        outputs['A'] = mFbo2->getColorTexture();
        
        setupShader();
        setupUI();
        
        // allocate our UBO
        dataUbo = gl::Ubo::create( sizeof( data ), &data, GL_DYNAMIC_DRAW );
        // and bind it to buffer base 0; this is analogous to binding it to texture unit 0
        dataUbo->bindBufferBase( id );
        // and finally tell the shaders that their uniform buffer 'FormulaParams' can be found at buffer base 0
        mShader->uniformBlock("mosher", id );
        
        
    }
    
    void Mosher::update(){
        
        if(mMainWinCtx != gl::Context::getCurrent()){
            return;
        }
        ModuleVideo::update();
        
        gl::pushMatrices();
        gl::ScopedViewport scpVp( ivec2( 0 ), mFbo->getSize() );
        gl::setMatrices( ModuleVideo::CAM );
        
        
        mFbo->bindFramebuffer();
        {
            gl::clear( ColorA(0, 0, 0, 0));
            dataUbo->bufferSubData( 0, sizeof( data ), &data );
            gl::ScopedGlslProg glslProg( mShader );
            
            if(inputs['A']){
                inputs['A']->bind(0);
                mShader->uniform( "tex0", 0 );  // texunit 0
            }
            if(inputs['B']){
                inputs['B']->bind(1);
                mShader->uniform( "tex1", 1 );  // texunit
            }
            
            gl::color(Color::white());
            gl::drawSolidRect(Area( 0, 0, mFbo->getWidth(), mFbo->getHeight() ));
            if(inputs['A']){
                inputs['A']->unbind(0);
            }
            if(inputs['B']){
                inputs['B']->unbind(1);
            }
        }
        mFbo->unbindFramebuffer();
        
        
        mFbo2->bindFramebuffer();
        {
            gl::enableAlphaBlendingPremult();
            gl::color( ColorA(1.0f,1.0f,1.0f,1.0f));
            gl::draw(mFbo->getColorTexture(), Area(0, 0, mFbo2->getWidth(), mFbo2->getHeight()));
        }
        mFbo2->unbindFramebuffer();
        
        gl::popMatrices();
    }
    
    void Mosher::setupUI(){
        ModuleVideo::setupUI();
        mUi->setColorBack(ColorAT<float>(vec4(.1f, .7f, .3f, .4f)));
        //mUi->setColorFill(ColorAT<float>(vec4(.8f, .9f, 1.f, .6f)));
        mUi->setColorFillHighlight(ColorAT<float>(vec4(.3f, .9f, 1.f, 1.f)));
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "gain", this->id, &(data.gain), -1.f, 1.0f);
        
        mUi->addSpacer(false);
        mUi->addSpacer(false);
        
        tools.addSlider(mUi, "x", this->id, &(data.x), -1.f, 1.0f);
        tools.addSlider(mUi, "y", this->id, &(data.y), -1.f, 1.0f);
        
        
        mUi->addToggle("Mirror", &data.mirror);
        
        mUi->setMinified(true);
    }
    
    void Mosher::setupShader(){
        
        ModuleVideo::setupShader();
        mShader = gl::GlslProg::create(
                                       loadAsset( "shaders/mosher.vert"),
                                       loadAsset( "shaders/mosher.frag")
                                       );
    }
}
