import pyegengine as eg

class TestGame(eg.Game):
    def __init__(self, window, scene):
        eg.Game.__init__(self, window, scene)

    def Update(self):
        movement_speed = self.GetTime().GetFrameTime() * 20.0;
        if (self.GetInput().IsMouseDown(eg.InputMouse.right)):
            self.GetScene().GetCurrentCamera().RotateByMouse(self.GetInput().GetMouseDelta())
        if (self.GetInput().IsKeyDown(eg.InputKey.w)):
            self.GetScene().GetCurrentCamera().Move(eg.vec3(0.0, 0.0, -movement_speed))
        if (self.GetInput().IsKeyDown(eg.InputKey.s)):
            self.GetScene().GetCurrentCamera().Move(eg.vec3(0.0, 0.0, movement_speed))
        if (self.GetInput().IsKeyDown(eg.InputKey.a)):
            self.GetScene().GetCurrentCamera().Move(eg.vec3(-movement_speed, 0.0, 0.0))
        if (self.GetInput().IsKeyDown(eg.InputKey.d)):
            self.GetScene().GetCurrentCamera().Move(eg.vec3(movement_speed, 0.0, 0.0))
        if (self.GetInput().IsKeyDown(eg.InputKey.space)):
            self.GetScene().GetCurrentCamera().Move(eg.vec3(0.0, movement_speed, 0.0))
        if (self.GetInput().IsKeyDown(eg.InputKey.c)):
            self.GetScene().GetCurrentCamera().Move(eg.vec3(0.0, -movement_speed, 0.0))

def main():
    # Setup Game Window
    window = eg.Window(800.0, 600.0, 32, False, "Element Games Python Test")
    scene = eg.Scene()
    game = TestGame(window, scene)

    # Setup Camera Object
    camera = eg.Camera(45.0, eg.ivec2(800, 600), eg.vec2(0.01, 150.0))
    camera.ComputeProjectionMatrix()
    camera.Update()
    camera.ComputeViewMatrix()
    scene.SetCurrentCamera(camera)
    camera_object = eg.Object("test_camera")
    camera_attribute = eg.ObjectAttributeRenderingCamera(camera)
    camera_object.AddAttribute(camera_attribute)
    scene.GetObjectManager().AddObject(camera_object)

    # Setup Sky Sphere
    sphere = eg.GenerateSphere(8, 8)
    scene.GetMeshManager().Add("sphere", sphere)
    sky_sphere = eg.Object("SkySphere")
    texture = eg.Texture("Assets/Textures/sky.png", True, 16.0)
    scene.GetTextureManager().AddTexture("starfield_decal", texture)
    trans = eg.mat4()
    trans = eg.scale(trans, eg.vec3(50.0, 50.0, 50.0))
    trans_attr = eg.ObjectAttributeBasicTransformation(trans)
    sky_sphere.AddAttribute(trans_attr)
    material = eg.RenderingMaterial()
    material.SetLit(False)
    material.SetCastsShadows(False)
    material.SetTexture(eg.TextureType.decal, "starfield_decal")
    mesh_attribute = eg.ObjectAttributeRenderingMesh("sphere", material)
    sky_sphere.AddAttribute(mesh_attribute)
    scene.GetObjectManager().AddObject(sky_sphere)

    # Game Loop
    while window.IsOpened():
        game.Update()
        game.Render()
    window.Close()

if __name__ == "__main__":
    main()
