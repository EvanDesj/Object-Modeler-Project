// Include Dependencies
#include "SceneObject.h"

SceneObject::SceneObject(
    int id,
    TransformationNode *trans,
    TransformationNode *rot,
    TransformationNode *scale,
    ShapeNode *shape,
    MaterialNode *m,
    TextureNode *t,
    HitBox *hit)
{
    this->id = id;
    this->translate = trans;
    this->rotation = rot;
    this->scale = scale;
    this->shape = shape;
    this->material_id = 0;
    this->material = m;
    this->texture = t;
    this->hitBox = hit;
};

void SceneObject::translateBy(Vector3 pointTranslate)
{
    this->translate->transformations.x += pointTranslate.x;
    this->translate->transformations.y += pointTranslate.y;
    this->translate->transformations.z += pointTranslate.z;
};

void SceneObject::scaleBy(Vector3 newScale)
{
    this->scale->transformations.x += newScale.x;
    this->scale->transformations.y += newScale.y;
    this->scale->transformations.z += newScale.z;
};

void SceneObject::rotateBy(Vector3 newRotate)
{
    this->rotation->transformations.x += newRotate.x;
    this->rotation->transformations.y += newRotate.y;
    this->rotation->transformations.z += newRotate.z;
};

void SceneObject::select()
{
    this->shape->selected = true;
};

void SceneObject::deselect()
{
    this->shape->selected = false;
};