import pyegengine as eg

class TestGame(eg.Game):
    def __init__(self, window, scene):
        eg.Game.__init__(self, window, scene)
        print "Setting Up"

    def Update(self):
        movement_speed = self.GetTime().GetFrameTime() * 2.0;
        if (self.GetInput().IsMouseDown(eg.InputMouse.left)):
            self.GetScene().GetCurrentCamera().RotateByMouse(self.GetInput().GetMouseDelta())
        #if (self.GetInput().IsKeyDown(eg.InputKey.q)):
            #self.GetScene().GetCurrentCamera().Rotate(eg.vec3(0.0, 0.0, -movement_speed))
        #if (self.GetInput().IsKeyDown(eg.InputKey.e)):
            #self.GetScene().GetCurrentCamera().Rotate(eg.vec3(0.0, 0.0, movement_speed))
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
    camera = eg.Camera(45.0, eg.ivec2(800, 600), eg.vec2(0.01, 1000.0))
    camera.ComputeProjectionMatrix()
    camera.SetPosition(eg.vec3(0.0, 0.0, 10.0))
    #camera.RotateByMouse(eg.vec2(0.0, -200.0))
    camera.Update()
    camera.ComputeViewMatrix()
    scene.SetCurrentCamera(camera)
    camera_object = eg.Object("test_camera")
    camera_attribute = eg.ObjectAttributeRenderingCamera(camera)
    camera_object.AddAttribute(camera_attribute)
    scene.GetObjectManager().AddObject(camera_object)

    # Setup Simple Sphere Object
    sphere_object = eg.Object("test_sphere")
    sphere_mesh = eg.GenerateSphere(16, 16)
    scene.GetMeshManager().Add("sphere", sphere_mesh)
    sphere_mat = eg.RenderingMaterial()
    sphere_mat.SetLit(False)
    sphere_mat.SetColor(eg.vec4(1.0, 1.0, 0.0, 1.0))
    mesh_attribute = eg.ObjectAttributeRenderingMesh("sphere", sphere_mat)
    sphere_object.AddAttribute(mesh_attribute)
    trans = eg.mat4()
    sphere_object.AddAttribute(eg.ObjectAttributeBasicTransformation(trans))
    scene.GetObjectManager().AddObject(sphere_object)

    # Game Loop
    while window.IsOpened():
        game.Update()
        game.Render()
    window.Close()

if __name__ == "__main__":
    main()
