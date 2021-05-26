//
//  ColorAdjustement.hpp
//  orage
//
//  Created by Vincent Evrard on 2017-04-23.
//
//

#ifndef ColorAdjustement_hpp
#define ColorAdjustement_hpp



#include "ModuleVideo.h"

using namespace reza::ui;
using namespace ci;
using namespace ci::app;
using namespace std;

namespace ogre {
    class ColorAdjustement : public ModuleVideo{
        gl::Context * mMainWinCtx;
        
        struct DATA {
            float bri  = 1.0f;
            float sat  = 1.0f;
            float con  = 1.0f;
            float red  = 0.0f;
            float green  = 0.0f;
            float blue  = 0.0f;
            float ssm  = 0.0f;
            float ssM  = 1.0f;
            int modifier = 0;
            DATA(){};
            DATA(JsonTree data):
                bri(data.getChild("bri").getValue<float>()),
                sat(data.getChild("sat").getValue<float>()),
                con(data.getChild("con").getValue<float>()),
                red(data.getChild("red").getValue<float>()),
                green(data.getChild("green").getValue<float>()),
                blue(data.getChild("blue").getValue<float>()),
                ssm(data.getChild("ssm").getValue<float>()),
                ssM(data.getChild("ssM").getValue<float>()),
                modifier(data.getChild("modifier").getValue<int>())
            {}
        } ;
        DATA data;
        
        gl::UboRef          dataUbo;
        gl::FboRef			mFbo, mFbo2;
        gl::GlslProgRef     mShader;
        
        ColorAdjustement(string name, JsonTree data, vec2 origin, vec2 size, gl::Context * mMainWinCtx);
        
    public:
        static int COUNT;
        
        virtual ~ColorAdjustement(){
            data.~DATA();
            dataUbo.reset();
            mFbo.reset();
            mShader.reset();
            mMainWinCtx = nullptr;
        }
        
        virtual void setData(int id, int elem, float nValue) override {
            switch(id){
                case 0 : data.bri = lerp(0.0f, 2.0f, nValue); break;
                case 1 : data.sat = lerp(0.0f, 2.0f, nValue); break;
                case 2 : data.con = lerp(0.0f, 2.0f, nValue); break;
                case 3 : data.red = lerp(-1.f, 1.0f, nValue); break;
                case 4 : data.green = lerp(-1.f, 1.0f, nValue); break;
                case 5 : data.blue = lerp(-1.f, 1.0f, nValue); break;
                case 6 : data.ssm = lerp(0.f, 1.0f, nValue); break;
                case 7 : data.ssM = lerp(1.f, 0.0f, nValue); break;
            }
        }
        
        typedef std::shared_ptr<class ColorAdjustement> ColorAdjustementRef;
        
        static ColorAdjustementRef create( const std::string name, vec2 origin, gl::Context * mMainWinCtx, JsonTree data = JsonTree())
        {
            ColorAdjustement::COUNT++;
            return ColorAdjustementRef( new ColorAdjustement( name, data, origin, vec2(WIDTH, 580), mMainWinCtx ) );
        }
        virtual JsonTree getData() override {
            {
                JsonTree obj = ModuleCommon::getData();
                obj.addChild(JsonTree("type", "ColorAdjustement"));
                JsonTree sub = JsonTree::makeObject("data");
                sub.addChild(JsonTree("bri", data.bri));
                sub.addChild(JsonTree("sat", data.sat));
                sub.addChild(JsonTree("con", data.con));
                sub.addChild(JsonTree("red", data.red));
                sub.addChild(JsonTree("green", data.green));
                sub.addChild(JsonTree("blue", data.blue));
                sub.addChild(JsonTree("ssm", data.ssm));
                sub.addChild(JsonTree("ssM", data.ssM));
                sub.addChild(JsonTree("modifier", data.modifier));
                obj.pushBack(sub);
                return obj;
            }
        };
        
        void setupShader() override;
        void setupUI() override;
        void setup() override;
        void update() override;
    };
    
    typedef std::shared_ptr<class ColorAdjustement> ColorAdjustementRef;
}


#endif /* ColorAdjustement_hpp */
