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
    class_<glm::mat4>("mat4", init<>())
        .def(init<glm::vec4, glm::vec4, glm::vec4, glm::vec4>());
        //.def(init<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>());

    /*def("translate", glm::gtc::matrix_transform::translate<float>);
    def("translate", glm::gtc::matrix_transform::translate<double>);
    def("scale", glm::gtc::matrix_transform::scale<float>);
    def("scale", glm::gtc::matrix_transform::scale<double>);
    def("rotate", glm::gtc::matrix_transform::rotate<float>);
    def("rotate", glm::gtc::matrix_transform::rotate<double>);*/
    // Those were broken again, since the glm upgrade to 0.9.3

    // Finish
    enum_<EG::Input::Key>("InputKey")
        .value("w", EG::Input::w)
        .value("a", EG::Input::a)
        .value("s", EG::Input::s)
        .value("d", EG::Input::d)
        .value("space", EG::Input::space)
        .value("c", EG::Input::c);

    enum_<EG::Input::Mouse>("InputMouse")
        .value("left", EG::Input::mouse_left)
        .value("right", EG::Input::mouse_right)
        .value("middle", EG::Input::mouse_middle);

    // Done
    class_<EG::Input::Input>("Input", init<>())
        .def("Update", &EG::Input::Input::Update)
        .def("KeyPressed", &EG::Input::Input::KeyPressed)
        .def("KeyReleased", &EG::Input::Input::KeyReleased)
        .def("MouseButtonPressed", &EG::Input::Input::MouseButtonPressed)
        .def("MouseButtonReleased", &EG::Input::Input::MouseButtonReleased)
        .def("MouseMoved", &EG::Input::Input::MouseMoved)
        .def("SetTextEntered", &EG::Input::Input::SetTextEntered)
        .def("IsKeyDown", &EG::Input::Input::IsKeyDown)
        .def("IsKeyToggled", &EG::Input::Input::IsKeyToggled)
        .def("IsKeyPressed", &EG::Input::Input::IsKeyPressed)
        .def("IsKeyReleased", &EG::Input::Input::IsKeyReleased)
        .def("IsMouseDown", &EG::Input::Input::IsMouseDown)
        .def("IsMouseToggled", &EG::Input::Input::IsMouseToggled)
        .def("IsMousePressed", &EG::Input::Input::IsMousePressed)
        .def("IsMouseReleased", &EG::Input::Input::IsMouseReleased)
        .def("GetMousePosition", &EG::Input::Input::GetMousePosition)
        .def("GetMouseDelta", &EG::Input::Input::GetMouseDelta)
        .def("GetTextEntered", &EG::Input::Input::GetTextEntered);

    class_<EG::Graphics::Mesh>("Mesh", init<EG::Graphics::TriangleMesh *>()); // TODO: Expose More

    // Done
    def("GenerateCube", &EG::Graphics::GenerateCube, return_value_policy<reference_existing_object>());
    def("GeneratePlane", &EG::Graphics::GeneratePlane, return_value_policy<reference_existing_object>());
    def("GenerateSphere", &EG::Graphics::GenerateSphere, return_value_policy<reference_existing_object>());
    def("GenerateCubeSphere", &EG::Graphics::GenerateCubeSphere, return_value_policy<reference_existing_object>());
    def("GenerateQuad", &EG::Graphics::GenerateQuad, return_value_policy<reference_existing_object>());

    // Done
    class_<EG::Utility::Window>("Window", init<float, float, unsigned int, bool, std::string>())
        .def("IsOpened", &EG::Utility::Window::IsOpened)
        .def("Update", &EG::Utility::Window::Update)
        .def("Display", &EG::Utility::Window::Display)
        .def("Close", &EG::Utility::Window::Close)
        .def("GetResolutionHeight", &EG::Utility::Window::GetResolutionHeight)
        .def("GetResolutionWidth", &EG::Utility::Window::GetResolutionWidth);

    // Done
    class_<EG::Game::Scene>("Scene", init<>())
        .def("SetCurrentCamera", &EG::Game::Scene::SetCurrentCamera)
        .def("GetCurrentCamera", &EG::Game::Scene::GetCurrentCamera, return_value_policy<reference_existing_object>())
        .def("GetObjectManager", &EG::Game::Scene::GetObjectManager, return_value_policy<reference_existing_object>())
        .def("GetMeshManager", &EG::Game::Scene::GetMeshManager, return_value_policy<reference_existing_object>())
        .def("GetTextureManager", &EG::Game::Scene::GetTextureManager, return_value_policy<reference_existing_object>());

    // Done
    class_<EG::Game::Game>("Game", init<EG::Utility::Window *, EG::Game::Scene *>())
        .def("Update", &EG::Game::Game::Update)
        .def("Render", &EG::Game::Game::Render)
        .def("GetInput", &EG::Game::Game::GetInput, return_value_policy<reference_existing_object>())
        .def("GetScene", &EG::Game::Game::GetScene, return_value_policy<reference_existing_object>())
        .def("GetTime", &EG::Game::Game::GetTime, return_value_policy<reference_existing_object>())
        .def("GetWindow", &EG::Game::Game::GetWindow, return_value_policy<reference_existing_object>())
        .def("GetRenderer", &EG::Game::Game::GetRenderer, return_value_policy<reference_existing_object>());

    // Done
    class_<EG::Graphics::Camera>("Camera", init<float, glm::ivec2, glm::vec2>())
        .def("ComputeProjectionMatrix", &EG::Graphics::Camera::ComputeProjectionMatrix)
        .def("ComputeViewMatrix", &EG::Graphics::Camera::ComputeViewMatrix)
        .def("SetPosition", &EG::Graphics::Camera::SetPosition)
        .def("RotateByMouse", &EG::Graphics::Camera::RotateByMouse)
        .def("Update", &EG::Graphics::Camera::Update)
        .def("Move", &EG::Graphics::Camera::Move)
        .def("SetOffset", &EG::Graphics::Camera::SetOffset)
        .def("SetOrientation", &EG::Graphics::Camera::SetOrientation)
        .def("SetViewSize", &EG::Graphics::Camera::SetViewSize)
        .def("SetNearFar", &EG::Graphics::Camera::SetNearFar)
        .def("SetFOV", &EG::Graphics::Camera::SetFOV)
        .def("SetCameraType", &EG::Graphics::Camera::SetCameraType)
        .def("SetViewMatrix", &EG::Graphics::Camera::SetViewMatrix)
        .def("Rotate", &EG::Graphics::Camera::Rotate)
        .def("GetPosition", &EG::Graphics::Camera::GetPosition)
        .def("GetOffset", &EG::Graphics::Camera::GetOffset)
        .def("GetOrientation", &EG::Graphics::Camera::GetOrientation)
        .def("GetViewSize", &EG::Graphics::Camera::GetViewSize)
        .def("GetNearFar", &EG::Graphics::Camera::GetNearFar)
        .def("GetFOV", &EG::Graphics::Camera::GetFOV)
        .def("GetViewMatrix", &EG::Graphics::Camera::GetViewMatrix)
        .def("GetInverseViewMatrix", &EG::Graphics::Camera::GetInverseViewMatrix)
        .def("GetProjectionMatrix", &EG::Graphics::Camera::GetProjectionMatrix)
        .def("GetInverseProjectionMatrix", &EG::Graphics::Camera::GetInverseProjectionMatrix);

    // Done
    enum_<EG::Graphics::RenderingMaterial::RenderingMaterialTextureType>("TextureType")
        .value("decal", EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_DECAL)
        .value("normal", EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_NORMAL)
        .value("height", EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_HEIGHT)
        .value("specular", EG::Graphics::RenderingMaterial::RENDERING_MATERIAL_TEXTURE_SPECULAR);

    // Done
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

    // Object Attributes TODO: Getters and setters
    class_<EG::Game::ObjectAttribute>("ObjectAttribute", init<>())
        .def("GetType", &EG::Game::ObjectAttribute::GetType, return_value_policy<return_by_value>());
    class_<EG::Game::ObjectAttributeBasicTransformation, bases<EG::Game::ObjectAttribute> >("ObjectAttributeBasicTransformation", init<glm::mat4>());
    class_<EG::Game::ObjectAttributeBasicString, bases<EG::Game::ObjectAttribute> >("ObjectAttributeBasicString", init<std::string, std::string>());
    class_<EG::Game::ObjectAttributeBasicInteger, bases<EG::Game::ObjectAttribute> >("ObjectAttributeBasicInteger", init<std::string, int>());
    class_<EG::Game::ObjectAttributeBasicFloat, bases<EG::Game::ObjectAttribute> >("ObjectAttributeBasicFloat", init<std::string, float>());
    class_<EG::Game::ObjectAttributeRenderingMesh, bases<EG::Game::ObjectAttribute> >("ObjectAttributeRenderingMesh", init<std::string, EG::Graphics::RenderingMaterial *>());
    class_<EG::Game::ObjectAttributeRenderingCamera, bases<EG::Game::ObjectAttribute> >("ObjectAttributeRenderingCamera", init<EG::Graphics::Camera *>());
    class_<EG::Game::ObjectAttributeEmissionLight, bases<EG::Game::ObjectAttribute> >("ObjectAttributeEmissionLight", init<EG::Graphics::Light *>());
    class_<EG::Game::ObjectAttributeEmissionParticleSystem, bases<EG::Game::ObjectAttribute> >("ObjectAttributeEmissionParticleSystem", init<EG::Graphics::ParticleSystem *>());
    class_<EG::Game::ObjectAttributeControlRigidBody, bases<EG::Game::ObjectAttribute> >("ObjectAttributeControlRigidBody", init<EG::Dynamics::RigidBody *>());

    // Done
    class_<EG::Game::Object>("Object", init<std::string>())
        .def("GetObjectId", &EG::Game::Object::GetObjectId)
        .def("GetObjectName", &EG::Game::Object::GetObjectName)
        .def("AddAttribute", &EG::Game::Object::AddAttribute)
        .def("GetAttributes", &EG::Game::Object::GetAttributes, return_value_policy<reference_existing_object>())
        .def("GetAttributesByType", &EG::Game::Object::GetAttributesByType, return_value_policy<reference_existing_object>())
        .def("HasAttributesOfType", &EG::Game::Object::HasAttributesOfType);

    // Done
    class_<EG::Game::ObjectManager>("ObjectManager", init<>())
        .def("AddObject", &EG::Game::ObjectManager::AddObject)
        .def("GetObjectById", &EG::Game::ObjectManager::GetObjectById, return_value_policy<reference_existing_object>())
        .def("GetObjectByName", &EG::Game::ObjectManager::GetObjectByName, return_value_policy<reference_existing_object>())
        .def("GetObjectCount", &EG::Game::ObjectManager::GetObjectCount)
        .def("GetObjects", &EG::Game::ObjectManager::GetObjects, return_value_policy<reference_existing_object>());

    // Done
    class_<EG::Graphics::MeshManager>("MeshManager", init<>())
        .def("Add", &EG::Graphics::MeshManager::Add)
        .def("Get", &EG::Graphics::MeshManager::Get, return_value_policy<reference_existing_object>());

    // Done
    class_<EG::Graphics::Light>("Light", init<>())
        .def("SetColor", &EG::Graphics::Light::SetColor)
        .def("SetAttenuation", &EG::Graphics::Light::SetAttenuation)
        .def("SetPosition", &EG::Graphics::Light::SetPosition)
        .def("SetDirection", &EG::Graphics::Light::SetDirection)
        .def("SetCastsShadows", &EG::Graphics::Light::SetCastsShadows)
        .def("SetRadius", &EG::Graphics::Light::SetRadius)
        .def("SetSpotAngle", &EG::Graphics::Light::SetSpotAngle)
        .def("GetColor", &EG::Graphics::Light::GetColor)
        .def("GetAttenuation", &EG::Graphics::Light::GetAttenuation)
        .def("GetPosition", &EG::Graphics::Light::GetPosition)
        .def("GetDirection", &EG::Graphics::Light::GetDirection)
        .def("GetCastsShadows", &EG::Graphics::Light::GetCastsShadows)
        .def("GetRadius", &EG::Graphics::Light::GetRadius)
        .def("GetSpotAngle", &EG::Graphics::Light::GetSpotAngle)
        .def("GetShadowMapResolution", &EG::Graphics::Light::GetShadowMapResolution)
        .def("Update", &EG::Graphics::Light::Update)
        .def("GetProjectionMatrix", &EG::Graphics::Light::GetProjectionMatrix)
        .def("GetViewMatrix", &EG::Graphics::Light::GetViewMatrix)
        .def("GetShadowBuffer", &EG::Graphics::Light::GetShadowBuffer, return_value_policy<reference_existing_object>());

    // Done
    class_<EG::Utility::Time>("Time", init<>())
        .def("Update", &EG::Utility::Time::Update)
        .def("GetFrameTime", &EG::Utility::Time::GetFrameTime)
        .def("GetElapsedTime", &EG::Utility::Time::GetElapsedTime)
        .def("GetFPS", &EG::Utility::Time::GetFPS);

    // Need to figure out default params, and also overloaded load function, which I neglected so far
    class_<EG::Graphics::Texture>("Texture", init<std::string, bool, float>())
        .def("GetId", &EG::Graphics::Texture::GetId)
        .def("GetFilePath", &EG::Graphics::Texture::GetFilePath)
        .def("GetWidth", &EG::Graphics::Texture::GetWidth)
        .def("GetHeight", &EG::Graphics::Texture::GetHeight)
        .def("UpdateImage", &EG::Graphics::Texture::UpdateImage);

    // Done
    class_<EG::Graphics::TextureManager>("TextureManager", init<>())
        .def("AddTexture", &EG::Graphics::TextureManager::AddTexture)
        .def("AddCubeMap", &EG::Graphics::TextureManager::AddCubeMap)
        .def("HasTexture", &EG::Graphics::TextureManager::HasTexture)
        .def("HasCubeMap", &EG::Graphics::TextureManager::HasCubeMap)
        .def("GetTexture", &EG::Graphics::TextureManager::GetTexture, return_value_policy<reference_existing_object>())
        .def("GetCubeMap", &EG::Graphics::TextureManager::GetCubeMap, return_value_policy<reference_existing_object>());
}
