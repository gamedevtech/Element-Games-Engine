#include "Scripting.h"

BOOST_PYTHON_MODULE(pyegengine) {
	using namespace boost::python;
	class_<EG::Utility::Window>("Window", init<float, float, unsigned int, bool, std::string>())
		.def("IsOpened", &EG::Utility::Window::IsOpened)
		.def("Update", &EG::Utility::Window::Update)
		.def("Display", &EG::Utility::Window::Display)
		.def("Close", &EG::Utility::Window::Close)
		.def("GetResolutionHeight", &EG::Utility::Window::GetResolutionHeight)
		.def("GetResolutionWidth", &EG::Utility::Window::GetResolutionWidth);

	class_<EG::Game::Scene>("Scene", init<>())
		.def("SetCurrentCamera", &EG::Game::Scene::SetCurrentCamera);

	class_<EG::Game::Game>("Game", init<EG::Utility::Window *, EG::Game::Scene *>())
		.def("Update", &EG::Game::Game::Update)
		.def("Render", &EG::Game::Game::Render);

	class_<glm::ivec2>("ivec2", init<int, int>());
	class_<glm::vec2>("vec2", init<float, float>());

	class_<EG::Graphics::Camera>("Camera", init<float, glm::ivec2, glm::vec2>());
}
