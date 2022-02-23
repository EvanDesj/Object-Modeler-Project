#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "HitBox.h"
#include "../nodes/Node.h"
#include "../nodes/TransformationNode.h"
#include "../nodes/ShapeNode.h"
#include "../nodes/MaterialNode.h"
#include "../nodes/TextureNode.h"

class SceneObject
{
public:
    int id, material_id;
    TransformationNode *translate,
        *rotation,
        *scale;
    ShapeNode *shape;
    MaterialNode *material;
    TextureNode *texture;
    HitBox *hitBox;

    SceneObject(int id,
        TransformationNode *trans,
        TransformationNode *rot,
        TransformationNode *scale,
        ShapeNode *shape,
        MaterialNode *m,
        TextureNode *t,
        HitBox *hit);

    void translateBy(Vector3 pointTranslate);
    void scaleBy(Vector3 newScale);
    void rotateBy(Vector3 newRotate);
    void select();
    void deselect();
};

#endif