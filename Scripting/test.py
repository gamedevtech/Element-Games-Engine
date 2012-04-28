import pyegengine as eg

w = eg.Window(800.0, 600.0, 32, False, "Element Games Python Test")
s = eg.Scene()
c = eg.Camera(45.0, eg.ivec2(800, 600), eg.vec2(0.01, 1000.0))
s.SetCurrentCamera(c)
g = eg.Game(w, s)

while w.IsOpened():
    g.Update()
    g.Render()
w.Close()
