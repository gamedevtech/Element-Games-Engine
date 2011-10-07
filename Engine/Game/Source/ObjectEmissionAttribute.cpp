#include "../ObjectEmissionAttribute.h"

namespace EG{
	namespace Game{
		ObjectAttributeEmissionLight::ObjectAttributeEmissionLight(EG::Graphics::Light *_light){
			type = EG::Game::ObjectAttribute::OBJECT_ATTRIBUTE_EMISSION_LIGHT;
			light = _light;
		}

		ObjectAttributeEmissionLight::~ObjectAttributeEmissionLight(void){
			//
		}

		void ObjectAttributeEmissionLight::SetLight(EG::Graphics::Light *_light){
			light = _light;
		}

		EG::Graphics::Light *ObjectAttributeEmissionLight::GetLight(void){
			return light;
		}
	}
}
