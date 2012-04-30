import pyegengine as eg

def main():
	# Setup Game Window
	w = eg.Window(800.0, 600.0, 32, False, "Element Games Python Test")
	s = eg.Scene()
	g = eg.Game(w, s)

	# Setup Camera Object
	c = eg.Camera(45.0, eg.ivec2(800, 600), eg.vec2(0.01, 1000.0))
	o = eg.Object("test_camera")
	ca = eg.ObjectAttributeRenderingCamera(c)
	o.AddAttribute(ca)
	s.GetObjectManager().AddObject(o)
	#print s.GetObjectManager().GetObjectById(o.GetObjectId()).GetObjectId()
	#print "DOngle"
	c.ComputeProjectionMatrix()
	s.SetCurrentCamera(c)

	# Game Loop
	while w.IsOpened():
		g.Update()
		g.Render()
	w.Close()

if __name__ == "__main__":
	main()
