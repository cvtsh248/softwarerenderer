import pywavefront
scene = pywavefront.Wavefront('teapot.obj')
print(scene.vertices)
out = []
for i in scene.vertices:
    out.append("Point3D{"+str(i[0])+","+str(i[1])+","+str(i[2])+"}")

print (','.join(map(str, out)))
