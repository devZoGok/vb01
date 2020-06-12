import sys

def exportData(ob):
    par=ob.parent
    file.write(ob.name+"\n")
    file.write("pos: "+str(ob.location.x)+" "+str(ob.location.y)+" "+str(ob.location.z)+"\n")
    file.write("rot: "+str(ob.rotation_quaternion.w)+" "+str(ob.rotation_quaternion.x)+" "+str(ob.rotation_quaternion.y)+" "+str(-ob.rotation_quaternion.z)+"\n")
    file.write("scale: "+str(ob.scale.x)+" "+str(ob.scale.y)+" "+str(ob.scale.z)+"\n")
    file.write("parent: "+('None' if par is None else par.name)+"\n")
    if(ob.type=="ARMATURE"):
        file.write("bones: "+str(len(ob.data.bones)))
        for bone in ob.data.bones:
            tail=bone.tail
            head=bone.head
            file.write("\n"+bone.name+": "+str(head.x)+" "+str(head.y)+" "+str(head.z)+" "+str(tail.x)+" "+str(tail.y)+" "+str(tail.z))
    
        file.write("\nboneHierarchy:")
        for bone in ob.data.bones:
            numChildren=len(bone.children)
            if numChildren>0:
                file.write('\n'+bone.name+' '+str(numChildren)+':')
                for ch in bone.children:
                    file.write(' '+ch.name)

        numAnims=len(ob.animation_data.nla_tracks[0].strips)
        if numAnims>0:
            file.write("\nanimations: "+str(numAnims))
            for nlaStrip in ob.animation_data.nla_tracks[0].strips:
                animName=nlaStrip.name
                action=bpy.data.actions[animName]
                numAnimBones=len(action.groups)
                file.write("\n"+animName+": "+str(numAnimBones)+"\n")
    
                print('Exporting animation '+animName+'...\n')
                for group in action.groups:
                    bone=ob.data.bones[group.name]
                    start=nlaStrip.frame_start
                    end=nlaStrip.frame_end
                    file.write(group.name+":\n")
                    for channel in group.channels:
                        numKeyframes=len(channel.keyframe_points)
                        dotId=-1
                        typeLen=len(channel.data_path)-1
                        i=typeLen
                        while(i>=0):
                            if channel.data_path[i]=='.':
                                dotId=i
                                break
                            i=i-1
                        coordType=channel.data_path[dotId+1:typeLen+1]
                        arrInd=channel.array_index
                        coords="wxyz"
                        if coordType=="location":
                            file.write("pos_"+coords[arrInd+1])
                        elif coordType=="rotation_quaternion":
                            file.write("rot_"+coords[arrInd])
                        elif coordType=="scale":
                            file.write("scale_"+coords[arrInd+1])
                        file.write(' '+str(numKeyframes)+':\n')
    
                        for keyframe in channel.keyframe_points:
                            keyframeId=keyframe.co[0]
                            interp=keyframe.interpolation
                            bpy.context.scene.frame_set(int(start)+int(keyframeId))
                            poseBone=ob.pose.bones[group.name]
                            if bpy.context.scene.frame_current>end:
                                    bpy.context.scene.frame_current=bpy.context.scene.frame_current-1
                            if coordType=="location":
                                file.write(str(poseBone.location[arrInd]))
                            elif coordType=="rotation_quaternion":
                                file.write(str(poseBone.rotation_quaternion[arrInd]))
                            elif coordType=="scale":
                                file.write(str(poseBone.scale[arrInd]))
                            file.write(" "+str(keyframeId)+" "+interp)
                            if interp=='BEZIER':
                                file.write(" "+str(keyframe.handle_left_type)+" "+str(keyframe.handle_left.x)+" "+str(keyframe.handle_left.y)+" ")
                                file.write(str(keyframe.handle_right_type)+" "+str(keyframe.handle_right.x)+" "+str(keyframe.handle_right.y))
                            file.write("\n")
    elif(ob.type=="MESH"):
        mesh=ob.data

        numFaces=len(mesh.polygons)
        numVerts=len(mesh.vertices)
        numGroups=len(ob.vertex_groups)
        numShapeKeys=0
        #if mesh.shape_keys is not NoneType:
            #numShapeKeys=len(mesh.shape_keys[0].key_blocks)-1

        file.write("vertices: "+str(numVerts)+","+str(numFaces)+"\n")
        print('Exporting vertices...\n')
        for vert in mesh.vertices:
            pos=vert.co
            norm=vert.normal
            file.write(str(pos.x)+" "+str(pos.y)+" "+str(pos.z)+" "+str(norm.x)+" "+str(norm.y)+" "+str(norm.z)+"\n")
        
        file.write("uv:\n")
        print('Exporting texture coordinates...\n')
        for face in mesh.polygons:
            for vert,loop in zip(face.vertices,face.loop_indices):
                uv=mesh.uv_layers[0].data[loop].uv
                file.write(str(vert)+" "+str(uv.x)+" "+str(uv.y)+"\n")
        
        if numGroups>0:
            file.write("groups: "+str(numGroups)+"\n")
            print('Exporting vertex groups...\n')
            for group in ob.vertex_groups:
                file.write(group.name+":\n")
                for vert in mesh.vertices:
                    for g in vert.groups:
                        if g.group==group.index:
                            file.write(str(vert.index)+" "+str(g.weight)+"\n")
        if numShapeKeys>0:
            file.write('shape keys:\n')
            print('Exporting shape keys...\n')
            for shape_key in mesh.shape_keys.key_blocks:
                if shape_key.name=='Basis':
                    continue
                for i in range(0,numVerts-1):
                    newPos=shape_key.data[i].co
                    if(newPos!=mesh.vertices[i].co):
                        file.write('\n'+str(i)+" "+str(newPos.x)+' '+str(newPos.y)+' '+str(newPos.z))

fl="/home/dominykas/c++/vb01/kek.vb";
objects=[]
selectedObjects=bpy.context.selected_objects

for s in selectedObjects:
    sPar=s
    while(sPar):
        sPar=sPar.parent
    inObjects=False
    for so in objects:
        if(so==sPar):
            inObjects=True
            break
    if inObjects==False:
        objects.append(sPar)

file=open(fl,"w")
for ob in selectedObjects:
    exportData(ob)
    file.write("\n")
file.close()
