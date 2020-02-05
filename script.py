import sys

ob=bpy.data.objects['awp']
mesh=bpy.data.meshes['Cylinder']

fl="/home/dominykas/c++/vb01/kek.vb";
file=open(fl,"w")

file.write("numFaces:"+str(len(ob.data.polygons))+"\n")
file.write("numVertices:"+str(len(ob.data.vertices))+"\n")
file.write("vertices:\n")

for vert in ob.data.vertices:
        pos=vert.co
        norm=vert.normal
        file.write(str(pos.x)+" "+str(pos.z)+" "+str(pos.y)+" "+str(norm.x)+" "+str(norm.z)+" "+str(norm.y)+"\n")

file.write("uv:\n")

for face in ob.data.polygons:
    for vert,loop in zip(face.vertices,face.loop_indices):
        uv=mesh.uv_layers[0].data[loop].uv
        file.write(str(vert)+" "+str(uv.x)+" "+str(uv.y)+"\n")

file.write("numGroups:"+str(len(ob.vertex_groups))+"\n")
file.write("groups:\n")
for group in ob.vertex_groups:
    file.write(group.name+":\n")
    for vert in ob.data.vertices:
        for g in vert.groups:
            if g.group==group.index:
                file.write(str(vert.index)+" "+str(g.weight)+"\n")

if ob.parent.name=="AWP":
    file.write("bones:\n")
    armature=ob.parent
    for bone in armature.data.bones:
        tail=bone.tail
        head=bone.head
        file.write(bone.name+":\n"+str(head.x)+" "+str(head.y)+" "+str(head.z)+" "+str(tail.x)+" "+str(tail.y)+" "+str(tail.z)+"\n")

    file.write("boneHierarchy:\n")
    for bone in armature.data.bones:
        file.write(str(len(bone.children))+" ")
        for ch in bone.children:
            file.write(ch.name+" ")
        file.write("\n")
file.close()
