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

    numAnims = 0
    if(ob.animation_data is not None and len(ob.animation_data.nla_tracks) > 0):
        numAnims = len(ob.animation_data.nla_tracks[0].strips)

    if(ob.type == 'ARMATURE'):
        file.write('bones: ' + str(len(ob.data.bones)) + ' ' + str(numAnims) + ' \n')

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

            file.write(bone.name + ': ' + ('None' if bone.parent is None else bone.parent.name) + " " + str(bone.length) + " " + str(head.x) + " " + str(head.y) + " " + str(head.z) + " " + str(xAxis.x) + " " + str(xAxis.y) + " " + str(xAxis.z) + " " + str(yAxis.x) + " " + str(yAxis.y) + " " + str(yAxis.z) + " " + ikTarget + ' ' + str(chainLength) + ' \n')


    
    elif(ob.type == 'MESH'):
        skeletonName = '-'
        skeleton = None
        if(len(ob.modifiers) > 0 and ob.modifiers['Armature'] and ob.modifiers['Armature'].object):
            skeleton = ob.modifiers['Armature'].object
            skeletonName = skeleton.name
        file.write('skeleton: ' + skeletonName + '\n')
        mesh = ob.data

        numFaces = len(mesh.polygons)
        numVerts = len(mesh.vertices)
        numGroups = len(ob.vertex_groups)
        numShapeKeys = 0
        if mesh.shape_keys:
            numShapeKeys = len(mesh.shape_keys.key_blocks) - 1

        file.write('numElements: ' + str(numVerts) + ' ' + str(numFaces) + ' ' + str(numGroups) + ' ' + str(numShapeKeys) + ' \n')

        file.write('groups:\n')
        if numGroups > 0:
            for group in ob.vertex_groups:
                file.write(group.name + '\n')

        file.write('vertices:\n')
        print('Exporting vertices...\n')
        for vert in mesh.vertices:
            pos = vert.co
            norm = vert.normal
            weights = []
            for i in range(0, numGroups):
                weights.append(0.0)
            for g in vert.groups:
                weights[g.group] = g.weight
            file.write(str(pos.x) + ' ' + str(pos.y) + ' ' + str(pos.z) + ' ' + str(norm.x) + ' ' + str(norm.y) + ' ' + str(norm.z) + ' ')
            for w in weights:
                file.write(str(w) + ' ')
            file.write('\n')

        file.write('faces:\n')
        print('Exporting faces...\n')
        mesh.calc_tangents()
        for face in mesh.polygons:
            for vert, loopInd in zip(face.vertices, face.loop_indices):
                uv = mesh.uv_layers[0].data[loopInd].uv
                loop = mesh.loops[loopInd]
                tan = loop.tangent
                bitan = loop.bitangent
                file.write(str(vert) + ' ' + str(uv.x) + ' ' + str(uv.y) + ' ' + str(tan.x) + ' ' + str(tan.y) + ' ' + str(tan.z) + ' ' + str(bitan.x) + ' ' + str(bitan.y) + ' ' + str(bitan.z) + ' \n')
        mesh.free_tangents()


        file.write('shapeKeys:\n')
        if numShapeKeys > 0:
            print('Exporting shape keys...\n')
            for shape_key in mesh.shape_keys.key_blocks:
                if shape_key.name != 'Basis':
                    file.write(shape_key.name + '\n')
                    for vert in shape_key.data:
                        pos = vert.co
                        file.write(str(pos.x) + ' ' + str(pos.y) + ' ' + str(pos.z) + ' \n')

    file.write('animations: ' + str(numAnims) + '\n')
    if numAnims > 0:
        readAnimations(ob, numAnims)

def getChannelType(channel):
    channelType = ''

    dotId = channel.data_path.rfind('.') + 1;
    coordType = channel.data_path[dotId :]
    arrInd = channel.array_index
    coords = 'wxyz'

    if coordType == 'location':
        channelType = 'pos_' + coords[arrInd + 1]
    elif coordType == 'rotation_quaternion':
        channelType = 'rot_' + coords[arrInd]
    elif coordType == 'scale':
        channelType = 'scale_' + coords[arrInd + 1]
    return channelType

def readAnimations(ob, numAnims):
    for nlaStrip in ob.animation_data.nla_tracks[0].strips:
        start = nlaStrip.frame_start
        end = nlaStrip.frame_end
        animName = nlaStrip.name
        action = bpy.data.actions[animName]
        numAnimBones = len(action.groups)
        file.write('animation: ' + animName)
        file.write('\ngroups: ' + str(numAnimBones))
    
        print('Exporting animation ' + animName + '...\n')

        file.write('\n')

        for group in action.groups:
            file.write((ob.name if group.name == 'Object Transforms' else group.name) + ' ' + str(len(group.channels)) + ' ')

            for channel in group.channels:
                numKeyframes = len(channel.keyframe_points)
                channelType = getChannelType(channel)
                file.write(channelType + ' ' + str(numKeyframes) + ' ')

            file.write('\n')

            for channel in group.channels:
                channelType = getChannelType(channel)

                dotId = channel.data_path.rfind('.') + 1
                arrInd = channel.array_index
                coordType = channel.data_path[dotId :]

                for keyframe in channel.keyframe_points:
                    keyframeId = keyframe.co[0]
                    interp = keyframe.interpolation
                    bpy.context.scene.frame_set(int(start) + int(keyframeId))
                    interpInt = - 1

                    if interp == 'CONSTANT':
                        interpInt = 0
                    elif interp == 'LINEAR':
                        interpInt = 1
                    elif interp == 'BEZIER':
                        interpInt = 2

                    keyframeValue = keyframe.co[1]
                    file.write(str(keyframeValue) + ' ' + str(keyframeId) + ' ' + str(interpInt))
                    if interp == 'BEZIER':
                        file.write(' ' + str(keyframe.handle_left_type) + ' ' + str(keyframe.handle_left.x) + ' ' + str(keyframe.handle_left.y) + ' ')
                        file.write(str(keyframe.handle_right_type) + ' ' + str(keyframe.handle_right.x) + ' ' + str(keyframe.handle_right.y))
                    file.write('\n')

fl = bpy.data.filepath
dotId = 0
for i in range(len(fl)):
    if(fl[i] == '.'):
        dotId = i
        break
fl = fl[0 : dotId] + '.vb'

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

file=open(fl, 'w')
for ob in selectedObjects:
    exportData(ob)
    file.write('}\n')
file.close()
