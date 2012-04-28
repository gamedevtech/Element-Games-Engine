#include "Scripting.h"

const char * greet() {
	return "Hola Duder!";
}

BOOST_PYTHON_MODULE(eg) {
	using namespace boost::python;
	//class_<EG::Utility::Window>("Window", init<float, float, unsigned int, bool, std::string>())
	//	.def("GetResolutionHeight", &EG::Utility::Window::GetResolutionHeight);
	def("greet", greet);
}
