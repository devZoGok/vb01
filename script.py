import sys

def exportData(ob):
    par = ob.parent
    if(par is not None and par.type == 'ARMATURE'):
        while(par is not None):
            par = par.parent

    file.write('{\n' + ob.type + ": " + ob.name + '\n')
    file.write('pos: ' + str(ob.location.x) + ' ' + str(ob.location.y) + ' ' + str(ob.location.z) + '\n')
    file.write('rot: ' + str(ob.rotation_quaternion.w) + ' ' + str(ob.rotation_quaternion.x) + ' ' + str(ob.rotation_quaternion.y) + ' ' + str(-ob.rotation_quaternion.z) + '\n')
    file.write('scale: ' + str(ob.scale.x) + ' ' + str(ob.scale.y) + ' ' + str(ob.scale.z) + '\n')
    file.write('parent: ' + ('-' if par is None else par.name) + '\n')
    if(ob.type == "ARMATURE"):
        numAnims = len(ob.animation_data.nla_tracks[0].strips)
        file.write("bones: "+str(len(ob.data.bones)) + ' ' + str(numAnims) + ' ')

        for bone in ob.data.bones:
            head = bone.head
            tail = bone.tail
            xAxis = bone.x_axis
            yAxis = bone.y_axis
            ikTarget = '-'
            chainLength = -1

            ikConstraint = None
            for constr in ob.pose.bones[bone.name].constraints:
                if constr.name == 'IK':
                    ikConstraint = constr
            if(ikConstraint is not None and ikConstraint.subtarget):
                ikTarget = ikConstraint.subtarget
                if ikConstraint.chain_count == 0: 
                    chainLength = 1
                    bonePar = bone.parent
                    while(bonePar is not None):
                        chainLength = chain_count + 1
                        bonePar = bonePar.parent
                else:
                    chainLength = ikConstraint.chain_count

            file.write('\n' + bone.name + ': ' + ('None' if bone.parent is None else bone.parent.name) + " " + str(bone.length) + " " + str(head.x) + " " + str(head.y) + " " + str(head.z) + " " + str(xAxis.x) + " " + str(xAxis.y) + " " + str(xAxis.z) + " " + str(yAxis.x) + " " + str(yAxis.y) + " " + str(yAxis.z) + " " + ikTarget + ' ' + str(chainLength) + ' ')

        if numAnims > 0:
            numGroups = 0
            for nlaStrip in ob.animation_data.nla_tracks[0].strips:
                animName = nlaStrip.name
                action = bpy.data.actions[animName]
                for group in action.groups:
                    numGroups += 1;


            file.write('\nanimations: ' + str(numAnims))
            file.write('\ngroups: ' + str(numGroups))
            for nlaStrip in ob.animation_data.nla_tracks[0].strips:
                animName = nlaStrip.name
                action = bpy.data.actions[animName]
                numAnimBones = len(action.groups)
                file.write('\nanimation: ' + animName + ' ' + str(numAnimBones) + ' ')
    
                print('Exporting animation '+animName+'...\n')

                file.write('\n')

                for group in action.groups:
                    bone = ob.data.bones[group.name]
                    start = nlaStrip.frame_start
                    end = nlaStrip.frame_end

                    file.write(animName + ' ' + group.name + ' ' + str(len(group.channels)) + ' ')

                    for channel in group.channels:
                        numKeyframes = len(channel.keyframe_points)
                        channelType = getChannelType(channel)
                        file.write(channelType + ' ')

                    file.write('\n')


                for group in action.groups:
                    bone = ob.data.bones[group.name]
                    start = nlaStrip.frame_start
                    end = nlaStrip.frame_end
                    for channel in group.channels:
                        channelType = getChannelType(channel)

                        dotId = channel.data_path.rfind('.') + 1
                        arrInd = channel.array_index
                        coordType = channel.data_path[dotId :]

                        for keyframe in channel.keyframe_points:
                            keyframeId = keyframe.co[0]
                            interp = keyframe.interpolation
                            bpy.context.scene.frame_set(int(start) + int(keyframeId))
                            poseBone = ob.pose.bones[group.name]
                            interpInt = - 1

                            if interp == 'CONSTANT':
                                interpInt = 0
                            elif interp == 'LINEAR':
                                interpInt = 1
                            elif interp == 'BEZIER':
                                interpInt = 2

                            file.write(animName + ' ' + bone.name + ' ' + channelType + ' ')

                            if bpy.context.scene.frame_current > end:
                                    bpy.context.scene.frame_current = bpy.context.scene.frame_current - 1
                            if coordType == "location":
                                file.write(str(poseBone.location[arrInd]))
                            elif coordType == "rotation_quaternion":
                                file.write(str(poseBone.rotation_quaternion[arrInd]))
                            elif coordType == "scale":
                                file.write(str(poseBone.scale[arrInd]))
                            file.write(" " + str(keyframeId) + " " + str(interpInt))
                            if interp == 'BEZIER':
                                file.write(" " + str(keyframe.handle_left_type) + " " + str(keyframe.handle_left.x) + " " + str(keyframe.handle_left.y) + " ")
                                file.write(str(keyframe.handle_right_type) + " " + str(keyframe.handle_right.x) + " " + str(keyframe.handle_right.y))
                            file.write("\n")

    
    elif(ob.type=="MESH"):
        skeletonName = '-'
        skeleton = ob.modifiers['Armature'].object
        if(skeleton):
            skeletonName = skeleton.name
        file.write('skeleton: ' + skeletonName + '\n')
        mesh = ob.data

        numFaces = len(mesh.polygons)
        numVerts = len(mesh.vertices)
        numGroups = len(ob.vertex_groups)
        numShapeKeys = 0
        #if mesh.shape_keys is not NoneType:
            #numShapeKeys=len(mesh.shape_keys[0].key_blocks)-1


        file.write("numElements: " + str(numVerts) + " " + str(numFaces) + " " + str(numGroups) + " " + str(numShapeKeys) + " \n")

        if numGroups > 0:
            file.write('groups:\n')
            for group in ob.vertex_groups:
                file.write(group.name + '\n')

        file.write("vertices:\n")
        print('Exporting vertices...\n')
        for vert in mesh.vertices:
            pos = vert.co
            norm = vert.normal
            weights = []
            for i in range(0, numGroups):
                weights.append(0.0)
            for g in vert.groups:
                weights[g.group] = g.weight
            file.write(str(pos.x) + " " + str(pos.y) + " " + str(pos.z) + " " + str(norm.x) + " " + str(norm.y) + " " + str(norm.z) + " ")
            for w in weights:
                file.write(str(w)+" ")
            file.write('\n')

        file.write("faces:\n")
        print('Exporting faces...\n')
        mesh.calc_tangents()
        for face in mesh.polygons:
            for vert, loopInd in zip(face.vertices, face.loop_indices):
                uv = mesh.uv_layers[0].data[loopInd].uv
                loop = mesh.loops[loopInd]
                tan = loop.tangent
                bitan = loop.bitangent
                file.write(str(vert) + " " + str(uv.x) + " " + str(uv.y) + " " + str(tan.x) + " " + str(tan.y) + " " + str(tan.z) + " " + str(bitan.x) + " " + str(bitan.y) + " " + str(bitan.z) + ' \n')
        mesh.free_tangents()


        if numShapeKeys > 0:
            file.write('shapeKeys:\n')
            for shape_key in mesh.shape_keys.key_blocks:
                if shape_key.name == 'Basis':
                    continue
                file.write(shape_key.name + '\n')
        '''
        for face in mesh.polygons:
            for vert,loop in zip(face.vertices,mesh.loops):
                pos=mesh.vertices[vert].co
                norm=mesh.vertices[vert].normal
                uv=mesh.uv_layers[0].data[loop.index].uv
                tan=loop.tangent
                bitan=loop.bitangent
                file.write(str(pos.x)+" "+str(pos.y)+" "+str(pos.z)+" "+str(norm.x)+" "+str(norm.y)+" "+str(norm.z)+str(uv.x)+" "+str(uv.y)+" "+str(tan.x)+" "+str(tan.y)+" "+str(tan.z)+" "+str(bitan.x)+" "+str(bitan.y)+" "+str(bitan.z)+" \n")

        if numGroups>0:
            groupVerts=[]
            file.write("groups: "+str(numGroups)+"\n")
            print('Exporting vertex groups...\n')
            for i in range(0,numGroups):
                groupVerts.append([])
                for vert in mesh.vertices:
                    for g in vert.groups:
                        if g.group==ob.vertex_groups[i].index:
                            groupVerts[i].append(vert.index)

            for i in range(0,numGroups):
                group=ob.vertex_groups[i]
                file.write(group.name+": "+str(len(groupVerts[i]))+"\n")
                for vert in mesh.vertices:
                    for g in vert.groups:
                        if g.group==group.index:
                            file.write(str(vert.index)+" "+str(g.weight)+" \n")

        if numShapeKeys>0:
            file.write('shapeKeys:\n')
            print('Exporting shape keys...\n')
            for shape_key in mesh.shape_keys.key_blocks:
                if shape_key.name=='Basis':
                    continue
                for i in range(0,numVerts-1):
                    newPos=shape_key.data[i].co
                    if(newPos!=mesh.vertices[i].co):
                        file.write('\n'+str(i)+" "+str(newPos.x)+' '+str(newPos.y)+' '+str(newPos.z))
        '''

def getChannelType(channel):
    channelType = ''

    dotId = channel.data_path.rfind('.') + 1;
    coordType = channel.data_path[dotId :]
    arrInd = channel.array_index
    coords = 'wxyz'

    if coordType == "location":
        channelType = "pos_" + coords[arrInd + 1]
    elif coordType == "rotation_quaternion":
        channelType = "rot_" + coords[arrInd]
    elif coordType == "scale":
        channelType = "scale_" + coords[arrInd + 1]
    return channelType

fl = bpy.data.filepath
dotId = 0
for i in range(len(fl)):
    if(fl[i] == '.'):
        dotId = i
        break
fl = fl[0 : dotId] + ".vb"

objects = []
selectedObjects = bpy.context.selected_objects

for s in selectedObjects:
    sPar = s
    while(sPar):
        sPar = sPar.parent
    inObjects = False
    for so in objects:
        if(so == sPar):
            inObjects = True
            break
    if inObjects == False:
        objects.append(sPar)

file=open(fl, "w")
for ob in selectedObjects:
    exportData(ob)
    file.write("}\n")
file.close()
