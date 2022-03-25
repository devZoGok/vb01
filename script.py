import sys
import xml.etree.ElementTree as ET

rootTag = ET.Element('model')

def exportBone(armature, parentBone, parentTag, exportChildren = True):
    head = parentBone.head
    xAxis = parentBone.x_axis
    yAxis = parentBone.y_axis
    ikTarget = None
    chainLength = -1

    boneTag = ET.SubElement(parentTag, 'bone')
    boneTag.set('name', parentBone.name)
    boneTag.set('hx', str(head.x))
    boneTag.set('hy', str(head.y))
    boneTag.set('hz', str(head.z))
    boneTag.set('length', str(parentBone.length))
    boneTag.set('xaxisx', str(xAxis.x))
    boneTag.set('xaxisy', str(xAxis.y))
    boneTag.set('xaxisz', str(xAxis.z))
    boneTag.set('yaxisx', str(yAxis.x))
    boneTag.set('yaxisy', str(yAxis.y))
    boneTag.set('yaxisz', str(yAxis.z))
    
    ikConstraint = None
    
    for constr in armature.pose.bones[parentBone.name].constraints:
        if constr.name == 'IK':
            ikConstraint = constr
    
    if(ikConstraint and ikConstraint.subtarget):
        ikTarget = ikConstraint.subtarget
    
        if ikConstraint.chain_count == 0: 
            chainLength = 1
            bonePar = bone.parent
    
            while(bonePar):
                chainLength = chain_count + 1
                bonePar = bonePar.parent
        else:
            chainLength = ikConstraint.chain_count

        boneTag.set('iktarget', ikTarget)
        boneTag.set('chainlength', str(chainLength))

    if(exportChildren):
        for bone in parentBone.children:
            exportBone(armature, bone, boneTag, exportChildren)


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
    elif coordType == 'color':
        channelType = 'diffuse_color_' + coords[arrInd + 1]
    elif coordType == 'slider_min':
        channelType = 'shape_key_min'
    elif coordType == 'value':
        channelType = 'shape_key_value'
    elif coordType == 'slider_max':
        channelType = 'shape_key_max'

    return channelType

def readChannel(animatableName, channel, tag):
    channelType = getChannelType(channel)
    channelTag = ET.SubElement(tag, 'channel')
    channelTag.set('type', channelType)
    channelTag.set('name', animatableName)

    numKeyframes = len(channel.keyframe_points)

    for i in range(numKeyframes):
        keyframe = channel.keyframe_points[i]
        keyframeId = keyframe.co[0]
        interp = keyframe.interpolation
        keyframeValue = keyframe.co[1]

        leftHandleValue = (keyframe.handle_left[1])
        leftHandleFrame = (keyframe.handle_left[0])
        rightHandleValue = (keyframe.handle_right[1])
        rightHandleFrame = (keyframe.handle_right[0])

        if interp != 'BEZIER':
            rightHandleValue = keyframeValue
            rightHandleFrame = keyframeId

        if i > 0 and channel.keyframe_points[i - 1].interpolation != 'BEZIER':
            leftHandleValue = keyframeValue
            leftHandleFrame = keyframeId

        keyframeTag = ET.SubElement(channelTag, 'keyframe')
        keyframeTag.set('frame', str(keyframeId))
        keyframeTag.set('value', str(keyframeValue))
        keyframeTag.set('interpolation', interp)
        keyframeTag.set('lefthandleframe', str(leftHandleFrame))
        keyframeTag.set('lefthandlevalue', str(leftHandleValue))
        keyframeTag.set('righthandleframe', str(rightHandleFrame))
        keyframeTag.set('righthandlevalue', str(rightHandleValue))

def readAnimations(ob, tag, node):
    for nlaStrip in ob.animation_data.nla_tracks[0].strips:
        objTrans = 'Object Transforms'

        if(nlaStrip.action.fcurves):
            groupName = nlaStrip.action.fcurves[0].group.name

            if((node and groupName != objTrans) or (not node and groupName == objTrans)):
                continue

        animTag = ET.SubElement(tag, 'animation')
        animTag.set('name', nlaStrip.name)
    
        for channel in nlaStrip.action.fcurves:
            readChannel((ob.name if channel.group is None or channel.group.name == objTrans else channel.group.name), channel, animTag)


def readDriver(node, channel, tag):
    driver = channel.driver
    target = driver.variables[0].targets[0];
    
    driverTag = ET.SubElement(tag, 'driver')
    driverTag.set('type', target.transform_type)
    driverTag.set('obj', target.id.name)

    if(target.bone_target):
        driverTag.set('bone', target.bone_target)
    
    numKeyframes = len(channel.keyframe_points)
    channelType = getChannelType(channel)
    
    firstQuoteId = channel.data_path.find('"') + 1
    secondQuoteId = channel.data_path.rfind('"')
    name = (node.name if firstQuoteId == -1 and secondQuoteId == -1  else channel.data_path[firstQuoteId : secondQuoteId])
    readChannel(name, channel, driverTag)

def export(node, parentTag):
    nodeTag = ET.SubElement(parentTag, 'node')
    nodeTag.set('name', node.name)

    if node.type == 'MESH':
        mesh = node.data
        numFaces = len(mesh.polygons)
        numVerts = len(mesh.vertices)
        numGroups = len(node.vertex_groups)
        numShapeKeys = 0
        
        if mesh.shape_keys:
            numShapeKeys = len(mesh.shape_keys.key_blocks) - 1
        
        meshTag = ET.SubElement(nodeTag, 'mesh')
        meshTag.set('num_faces', str(numFaces))
        meshTag.set('num_vertex_groups', str(numGroups))
        meshTag.set('num_shape_keys', str(numShapeKeys))
        
        for vert in mesh.vertices:
            pos = vert.co
            norm = vert.normal
            vertDataTag = ET.SubElement(meshTag, 'vertdata')
            vertDataTag.set('px', str(pos.x))
            vertDataTag.set('py', str(pos.z))
            vertDataTag.set('pz', str(-pos.y))
            vertDataTag.set('nx', str(norm.x))
            vertDataTag.set('ny', str(-norm.z))
            vertDataTag.set('nz', str(norm.y))
        
            weights = []
        
            for i in range(0, numGroups):
                weights.append(0.0)
        
            for g in vert.groups:
                weights[g.group] = g.weight
        
            for w in weights:
                weightDataTag = ET.SubElement(vertDataTag, 'weight')
                weightDataTag.set('value', str(w))
        
        if numGroups > 0:
            for group in node.vertex_groups:
                vertGroupTag = ET.SubElement(meshTag, 'vertexgroup')
                vertGroupTag.set('name', group.name)
        
        mesh.calc_tangents()
        
        for face in mesh.polygons:
            for vert, loopInd in zip(face.vertices, face.loop_indices):
                uv = mesh.uv_layers[0].data[loopInd].uv
                loop = mesh.loops[loopInd]
                tan = loop.tangent
                bitan = loop.bitangent
                vertTag = ET.SubElement(meshTag, 'vert')
                vertTag.set('id', str(vert))
                vertTag.set('uvx', str(uv.x))
                vertTag.set('uvy', str(uv.y))
                vertTag.set('tx', str(tan.x))
                vertTag.set('ty', str(tan.z))
                vertTag.set('tz', str(-tan.y))
                vertTag.set('bx', str(bitan.x))
                vertTag.set('by', str(bitan.z))
                vertTag.set('bz', str(-bitan.y))
        
        mesh.free_tangents()

        i = 0
        
        if numShapeKeys > 1:
            for shape_key in mesh.shape_keys.key_blocks:
                if shape_key.name != 'Basis':
                    shapeKeyTag = ET.SubElement(meshTag, 'shapekey')
                    shapeKeyTag.set('name', str(shape_key.name))
                    shapeKeyTag.set('min', str(shape_key.slider_min))
                    shapeKeyTag.set('max', str(shape_key.slider_max))

                    readDriver(node, mesh.shape_keys.animation_data.drivers[i], shapeKeyTag)
            
                    for vert in shape_key.data:
                        pos = vert.co
                        vertTag = ET.SubElement(shapeKeyTag, 'vert')
                        vertTag.set('px', str(pos.x))
                        vertTag.set('py', str(pos.z))
                        vertTag.set('pz', str(-pos.y))

                    i = i + 1
        
        armatureMod = 'Armature'
        
        if(node.modifiers and node.modifiers[armatureMod] and node.modifiers[armatureMod].object):
            skeleton = node.modifiers[armatureMod].object
            meshTag.set('skeleton', skeleton.name + '.' + skeleton.data.name_full)
    elif node.type == 'ARMATURE':
        skeletonTag = ET.SubElement(nodeTag, 'skeleton')
        skeletonTag.set('name', node.data.name_full)
        exportBone(node, node.data.bones[0], skeletonTag, True)
        readAnimations(node, skeletonTag, False)
    elif node.type == 'LIGHT':
        lightTag = ET.SubElement(nodeTag, 'light')

        lightData = node.data
        color = lightData.color
        lighTag.set('cr', str(color[0]))
        lighTag.set('cg', str(color[1]))
        lighTag.set('cb', str(color[2]))

        type = lightData.type
        lightTag.set('type', type)

        outerAngle = 0
        innerAngle = 0

        if type == 'SPOT':
            outerAngle = ob.data.spot_size
            innerAngle = outerAngle * ob.data.spot_blend

        lightTag.set('innerangle', str(innerAngle))
        lightTag.set('outerangle', str(outerAngle))

    if node.animation_data and node.animation_data.nla_tracks:
        readAnimations(node, nodeTag, True)

    for child in node.children:
        export(child, nodeTag)

objects = []
selectedObjects = bpy.context.selected_objects

for s in selectedObjects:
    sPar = s

    while(sPar.parent and sPar.parent in selectedObjects):
        sPar = sPar.parent

    if sPar not in objects:
        objects.append(sPar)

for obj in objects:
    export(obj, rootTag)
        
fl = bpy.data.filepath
fl = fl[0 : fl.rfind('.')] + '.xml'

ET.indent(rootTag)
binXml = ET.tostring(rootTag)

with open(fl, 'wb') as f:
    f.write(binXml)
