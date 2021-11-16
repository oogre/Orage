//
//  ParameterBase.h
//  CinderProject
//
//  Created by Vincent Evrard on 12/10/21.
//

#ifndef ParameterBase_h
#define ParameterBase_h



#include "EventTemplate.h"
#include "ISFAttr.h"

namespace reza {
    namespace ui {
        
        using namespace ORAGE::COMMON;
        using namespace ISF;
        
        typedef ORAGE::COMMON::Event<class ISFAttr> Evt;
        typedef EventTemplate<Evt> EvtHandler;
        typedef boost::signals2::signal<void(Evt)>::slot_type EvtSlot;
        
        enum PARAMETER_TYPE {
            NONE = 0x00,
            TEXTURE = 0x01,
            FLOAT = 0x03,
            CLOCK = 0x07
        };
        enum PLUG_TYPE {
            _IN = 0x00,
            _OUT = 0x10
        };
        
        struct Conf{
            ci::ColorA cableColorNormal;
            ci::ColorA cableColorOver;
        };
        
        class Config {
            ci::Rand r;
            static std::map<PARAMETER_TYPE, Conf> configs;
            public :
            static Conf getConfig(int type){
                return getConfig(static_cast<PARAMETER_TYPE>(type));
            }
            static Conf getConfig(PARAMETER_TYPE type){
                if(configs.count(type) == 0){
                    ci::Rand r = ci::Rand((int)type);
                    float tint = r.nextFloat();
                    configs[type] = {
                        ci::ColorA(CM_HSV, tint, 1.0, 0.5, 0.85),
                        ci::ColorA(CM_HSV, tint, 1.0, 0.75, 0.95),
                    };
                }
                return configs[type];
            }
        };
        std::map<PARAMETER_TYPE, Conf> Config::configs;
        
        class ParameterBase : public View, public EvtHandler{
            typedef std::shared_ptr<ParameterBase> ParameterBaseRef;
            
        public:
            uint8_t type = PARAMETER_TYPE::NONE | PLUG_TYPE::_IN;
            ParameterBase( std::string name):
                EvtHandler()
            {
                setName(name);
            }
            ColorA getCableColor(bool over){
                Conf conf = Config::getConfig(type & 0x0F);
                return over ? conf.cableColorOver : conf.cableColorNormal;
            }
            virtual void plugTo(ParameterBaseRef other){}
            virtual void unplugTo(ParameterBaseRef other){}
            virtual ~ParameterBase(){
            }
            virtual void beginDraw(){}
            virtual void endDraw(){}
            virtual void setVisible( bool visible ) override{
                View::setVisible(visible);
            }
            
        };//ParameterBase
        typedef std::shared_ptr<ParameterBase> ParameterBaseRef;
    }//ui {
}//reza {


#endif /* ParameterBase_h */
