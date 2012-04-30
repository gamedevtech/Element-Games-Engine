#include "Scripting.h"

BOOST_PYTHON_MODULE(pyegengine) {
	using namespace boost::python;

	class_<glm::ivec2>("ivec2", init<int, int>())
		.def_readwrite("x", &glm::ivec2::x)
		.def_readwrite("y", &glm::ivec2::y);
	class_<glm::vec2>("vec2", init<float, float>())
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);
	class_<glm::vec3>("vec3", init<float, float, float>())
		.def_readwrite("x", &glm::vec3::x)
		.def_readwrite("y", &glm::vec3::y)
		.def_readwrite("z", &glm::vec3::z);
	class_<glm::vec4>("vec4", init<float, float, float, float>())
		.def_readwrite("x", &glm::vec4::x)
		.def_readwrite("y", &glm::vec4::y)
		.def_readwrite("z", &glm::vec4::z)
		.def_readwrite("w", &glm::vec4::w);

	class_<EG::Utility::Window>("Window", init<float, float, unsigned int, bool, std::string>())
		.def("IsOpened", &EG::Utility::Window::IsOpened)
		.def("Update", &EG::Utility::Window::Update)
		.def("Display", &EG::Utility::Window::Display)
		.def("Close", &EG::Utility::Window::Close)
		.def("GetResolutionHeight", &EG::Utility::Window::GetResolutionHeight)
		.def("GetResolutionWidth", &EG::Utility::Window::GetResolutionWidth);

	class_<EG::Game::Scene>("Scene", init<>())
		.def("SetCurrentCamera", &EG::Game::Scene::SetCurrentCamera)
        .def("GetObjectManager", &EG::Game::Scene::GetObjectManager, return_value_policy<reference_existing_object>())
        .def("GetMeshManager", &EG::Game::Scene::GetMeshManager, return_value_policy<reference_existing_object>())
        .def("GetTextureManager", &EG::Game::Scene::GetTextureManager, return_value_policy<reference_existing_object>());

	class_<EG::Game::Game>("Game", init<EG::Utility::Window *, EG::Game::Scene *>())
		.def("Update", &EG::Game::Game::Update)
		.def("Render", &EG::Game::Game::Render);

	class_<EG::Graphics::Camera>("Camera", init<float, glm::ivec2, glm::vec2>())
		.def("ComputeProjectionMatrix", &EG::Graphics::Camera::ComputeProjectionMatrix);

	class_<EG::Graphics::RenderingMaterial>("RenderingMaterial", init<>())
		.def("SetLit", &EG::Graphics::RenderingMaterial::SetLit)
		.def("SetTranslucent", &EG::Graphics::RenderingMaterial::SetTranslucent)
		.def("SetCastsShadows", &EG::Graphics::RenderingMaterial::SetCastsShadows)
		.def("SetAmbient", &EG::Graphics::RenderingMaterial::SetAmbient)
		.def("SetDiffuse", &EG::Graphics::RenderingMaterial::SetDiffuse)
		.def("SetSpecular", &EG::Graphics::RenderingMaterial::SetSpecular)
		.def("SetSpecularExponent", &EG::Graphics::RenderingMaterial::SetSpecularExponent)
		.def("SetColor", &EG::Graphics::RenderingMaterial::SetColor)
		.def("SetTexture", &EG::Graphics::RenderingMaterial::SetTexture)
		.def("SetCubeMap", &EG::Graphics::RenderingMaterial::SetCubeMap)
		.def("SetShaderOverride", &EG::Graphics::RenderingMaterial::SetShaderOverride)
		.def("GetLit", &EG::Graphics::RenderingMaterial::GetLit)
		.def("GetTranslucent", &EG::Graphics::RenderingMaterial::GetTranslucent)
		.def("GetCastsShadows", &EG::Graphics::RenderingMaterial::GetCastsShadows)
		.def("GetAmbient", &EG::Graphics::RenderingMaterial::GetAmbient)
		.def("GetDiffuse", &EG::Graphics::RenderingMaterial::GetDiffuse)
		.def("GetSpecular", &EG::Graphics::RenderingMaterial::GetSpecular)
		.def("GetSpecularExponent", &EG::Graphics::RenderingMaterial::GetSpecularExponent)
		.def("GetColor", &EG::Graphics::RenderingMaterial::GetColor)
		.def("GetTexture", &EG::Graphics::RenderingMaterial::GetTexture)
		.def("GetCubeMap", &EG::Graphics::RenderingMaterial::GetCubeMap)
		.def("HasShader", &EG::Graphics::RenderingMaterial::HasShader)
		.def("GetShader", &EG::Graphics::RenderingMaterial::GetShader)
		.def("HasTexture", &EG::Graphics::RenderingMaterial::HasTexture)
		.def("HasCubeMap", &EG::Graphics::RenderingMaterial::HasCubeMap)
		.def("SetBlendingMode", &EG::Graphics::RenderingMaterial::SetBlendingMode)
		.def("GetBlendingMode", &EG::Graphics::RenderingMaterial::GetBlendingMode, return_value_policy<return_by_value>());

	class_<EG::Game::ObjectAttribute>("ObjectAttribute", init<>())
		.def("GetType", &EG::Game::ObjectAttribute::GetType, return_value_policy<return_by_value>());

	class_<EG::Game::ObjectAttributeRenderingMesh, bases<EG::Game::ObjectAttribute> >("ObjectAttributeRenderingMesh", init<std::string, EG::Graphics::RenderingMaterial *>());
	class_<EG::Game::ObjectAttributeRenderingCamera, bases<EG::Game::ObjectAttribute> >("ObjectAttributeRenderingCamera", init<EG::Graphics::Camera *>());

	class_<EG::Game::Object>("Object", init<std::string>())
		.def("GetObjectId", &EG::Game::Object::GetObjectId)
		.def("GetObjectName", &EG::Game::Object::GetObjectName)
		.def("AddAttribute", &EG::Game::Object::AddAttribute);

	class_<EG::Game::ObjectManager>("ObjectManager", init<>())
		.def("AddObject", &EG::Game::ObjectManager::AddObject)
        .def("GetObjectById", &EG::Game::ObjectManager::GetObjectById, return_value_policy<reference_existing_object>())
        .def("GetObjectByName", &EG::Game::ObjectManager::GetObjectByName, return_value_policy<reference_existing_object>())
        .def("GetObjectCount", &EG::Game::ObjectManager::GetObjectCount);
}
