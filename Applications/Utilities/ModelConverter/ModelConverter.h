#ifndef MODEL_CONVERTER_H
#define MODEL_CONVERTER_H

#include "../../../Engine/Game/Game.h"

class ModelConverter : public EG::Game::Game{
	public:
		ModelConverter(EG::Utility::Window *_window, EG::Game::Scene *_scene);
		~ModelConverter(void);

		void Update(void);
	private:
};

#endif
