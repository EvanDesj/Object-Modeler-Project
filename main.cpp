/*
Assignment 4:
    - Name: Mark Hutchison
    - Pronouns: he/him/his
    - Student ID: hutchm6
    - Email: [hutchm6@mcmaster.ca](mailto:hutchm6@mcmaster.ca)
    - Operating System: Arch Linux

    - Name: Evan Desjardine
    - Pronouns: he/him/his
    - Student ID: desjarde
    - Email: [desjarde@mcmaster.ca](mailto:desjarde@mcmaster.ca)
    - Operating System: Windows 10
*/
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif

#include "math/Vector3.h"
#include "nodes/GroupNode.h"
#include "scene/SceneObject.h"
#include "scene/SceneGraph.h"
#include "util/Camera.h"
#include "util/Color.h"
#include "util/Light.h"
#include "util/Material.h"
#include "util/Shape.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>

// Define Constants
#define WINDOW_TITLE "Assignment 4 - Scene Modeller"

// Global Variables -- Scene
int windowWidth = 800,
    windowHeight = 800,
    mouseX = 0,
    mouseY = 0,
    rotationMode = 0,
    materialMode = 0;
float angleX = 0.0f,
      angleY = 0.0f; // Scene Rotation Variables
bool shadingOnGround = true,
     light0_on = true,
     light1_on = true,
     deleteOnClick = false;

// Define Camera
Camera cam = Camera(
    Vector3(25.0f, 15.0f, 25.0f),
    Vector3(0.0f, 1.0f, 0.0f),
    Vector3());

// Default Materials recommended by Google
Material defaultMat = Material(
             Color(0.1f, 0.1f, 0.1f),
             Color(0.5f, 0.5f, 0.5f),
             Color(0.99f, 0.91f, 0.81f),
             10.0f),
         lightMat = Material(
             Color(0.33f, 0.22f, 0.03f),
             Color(0.78f, 0.57f, 0.11f),
             Color(0.99f, 0.91f, 0.81f),
             10.0f),
         floorMat = Material(
             Color(0.91f, 0.86f, 0.49f),
             Color(0.7f, 0.8f, 0.8f),
             Color(0.99f, 0.91f, 0.81f),
             3.0f);

// Define Lights
float light0pos[4] = {25.0f, 15.0f, 65.0f, 1.0f},
      light1pos[4] = {5.0f, 1.0f, -25.0f, 1.0f};

Light *light0 = new Light(0, Vector3(light0pos[0], light0pos[1], light0pos[2]), lightMat),
      *light1 = new Light(1, Vector3(light1pos[0], light1pos[1], light1pos[2]), lightMat);

// Showroom Graph related items
int nodeID = 0,
    nextChild = 0,
    currentObjIndex = 0;
SceneGraph *sceneGraph;
SceneObject *currentObject;
std::vector<SceneObject *> *sceneObjects = new std::vector<SceneObject *>();
std::vector<Light *> lights;

// Construct the "ground" plane
float roomSize = 100.0f;

// Textures -- PPM files provided by Google
GLubyte *marble_texture;
GLubyte *crates_texture;
GLubyte *stone_texture;
GLubyte *iron_texture;
GLuint textures[4];
int width1, height1, max1,
    width2, height2, max2,
    width3, height3, max3,
    width4, height4, max4;

// Incrememnt NodeID to traverse SceneGraph
int getID()
{
    return nodeID++;
}

// Delete Object from SceneGraph and SceneObjects
void deleteSceneObject(int id)
{
    // Delete Object from Graph
    sceneGraph->deleteNode(id);
    // Delete Object from Objects list
    int indexInGraph = -1;
    for (int i = 0; i < sceneObjects->size(); i++)
    {
        if (sceneObjects->at(i)->id == id)
        {
            indexInGraph = i;
            break;
        }
    }
    if (sceneObjects->size() - 1 > 0)
    {
        sceneObjects->erase(sceneObjects->begin() + indexInGraph);
        currentObjIndex = sceneObjects->size() - 1;
        currentObject = sceneObjects->at(currentObjIndex);
        currentObject->shape->selected = true;
        nextChild--;
    }
    else
    {
        currentObject = NULL;
        nextChild = 0;
    }
}

// Insert a Premade Object into the SceneGraph and SceneObjects
void insertSceneObject(SceneObject *obj)
{
    // Add Object to Object List
    sceneObjects->push_back(obj);
    if (nextChild > 1)
        currentObject->deselect();
    obj->select();
    currentObject = obj;
    currentObjIndex = sceneObjects->size() - 1;
}

// Create a new SceneObject and insert it into the SceneGraph and SceneObjects
void insertSceneObject(NodeType texturetype)
{
    sceneGraph->toRoot();

    HitBox *newHB = new HitBox(Vector3(), 2.0f);

    GroupNode *group = new GroupNode();
    sceneGraph->insertNode(group);
    sceneGraph->toChild(nextChild++);

    int groupID = sceneGraph->getCurrentNodeID();

    TransformationNode *translate = new TransformationNode(TRANSLATE, Vector3(0.0f, 0.0f, -25.0f));
    sceneGraph->insertNode(translate);
    sceneGraph->toChild(0);

    TransformationNode *rotate = new TransformationNode(ROTATE, Vector3(-45.0f, 0.0f, 45.0f));
    sceneGraph->insertNode(rotate);
    sceneGraph->toChild(0);

    Vector3 scaleVector = Vector3(1.0f, 1.0f, 1.0f);
    TransformationNode *scale = new TransformationNode(SCALE, scaleVector);
    sceneGraph->insertNode(scale);
    sceneGraph->toChild(0);

    MaterialNode *material = new MaterialNode(defaultMat);
    sceneGraph->insertNode(material);
    sceneGraph->toChild(0);

    TextureNode *texture = new TextureNode(textures[0], texturetype);
    sceneGraph->insertNode(texture);
    sceneGraph->toChild(0);

    ShapeNode *shape = new ShapeNode(texturetype);
    sceneGraph->insertNode(shape);

    SceneObject *newObj = new SceneObject(
        groupID,
        translate,
        rotate,
        scale,
        shape,
        material,
        texture,
        newHB);

    insertSceneObject(newObj);
}

// Reset the SceneGraph and SceneObjects to initial state
void resetSceneGraph()
{
    // Clear the scene graph
    sceneGraph = new SceneGraph();
    // Delete all objects from the old scene
    for (int i = 0; i < sceneObjects->size(); i++)
        deleteSceneObject(sceneObjects->at(i)->id);
    if (sceneObjects->size() > 1)
        resetSceneGraph(); // Weird glitch where the scene graph is not cleared
}

// Reset all materials back to Default Material
void resetMaterials()
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultMat.amb.getRGBA());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultMat.diff.getRGBA());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultMat.spec.getRGBA());
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, defaultMat.shininess);
}

// Render Lights
void renderLights()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    lightMat.render();

    if (light0_on)
    {
        light0->enable();
        light0->render();
    }
    if (light1_on)
    {
        light1->enable();
        light1->render();
    }
    glPopMatrix();
}

// Reset Light Location to initial state
void resetLights()
{
    light0pos[0] = 25.0f;
    light0pos[1] = 15.0f;
    light0pos[2] = 65.0f;
    light0pos[3] = 1.0f;
    light1pos[0] = 5.0f;
    light1pos[1] = 1.0f;
    light1pos[2] = -25.0f;
    light1pos[3] = 1.0f;
    light0->position.x = 25;
    light0->position.y = 15;
    light0->position.z = 65;
    light1->position.x = 5;
    light1->position.y = 1;
    light1->position.z = -25;
}

// Render Showroom Background
void renderShowroom()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Draw floor planar
    glPushMatrix();
    glRotatef(angleX, 0.0f, 0.0f, 1.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    floorMat.render();
    glTranslatef(0.0f, -1.5f, 0.0f);
    glScalef(100.0f, 0.5f, 100.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Save Scene to File
void saveScene()
{
    using namespace std;
    // Get Filename via CLI
    string filename;
    cout << "File name to save: ";
    cin >> filename;
    cout << "Saving scene to: " + filename << endl;
    ofstream fh(filename.c_str());
    // Write to file if not open
    if (fh.is_open())
    {
        //Iterate through the scene list object
        for (int i = 0; i < sceneObjects->size(); ++i)
        {
            //Enter the data of each scene object into the text file
            //Shape
            string shape = getShapeString(sceneObjects->at(i)->shape->type);
            fh << shape << endl; // 0
            // Shape ID
            fh << sceneObjects->at(i)->id << endl; // 1
            // Translations
            fh << sceneObjects->at(i)->translate->transformations.x << endl; // 2
            fh << sceneObjects->at(i)->translate->transformations.y << endl; // 3
            fh << sceneObjects->at(i)->translate->transformations.z << endl; // 4
            // Rotations
            fh << sceneObjects->at(i)->rotation->transformations.x << endl; // 5
            fh << sceneObjects->at(i)->rotation->transformations.y << endl; // 6
            fh << sceneObjects->at(i)->rotation->transformations.z << endl; // 7
            // Scale
            fh << sceneObjects->at(i)->scale->transformations.x << endl; // 8
            fh << sceneObjects->at(i)->scale->transformations.y << endl; // 9
            fh << sceneObjects->at(i)->scale->transformations.z << endl; // 10
            // Material
            fh << sceneObjects->at(i)->material->material.amb.r << endl // 11
               << sceneObjects->at(i)->material->material.amb.g << endl // 12
               << sceneObjects->at(i)->material->material.amb.b << endl // 13
               << sceneObjects->at(i)->material->material.amb.a << endl; // 14
            fh << sceneObjects->at(i)->material->material.diff.r << endl // 15
               << sceneObjects->at(i)->material->material.diff.g << endl // 16
               << sceneObjects->at(i)->material->material.diff.b << endl // 17
               << sceneObjects->at(i)->material->material.diff.a << endl; // 18
            fh << sceneObjects->at(i)->material->material.spec.r << endl // 19
               << sceneObjects->at(i)->material->material.spec.g << endl // 20
               << sceneObjects->at(i)->material->material.spec.b << endl // 21
               << sceneObjects->at(i)->material->material.spec.a << endl; // 22
            fh << sceneObjects->at(i)->material->material.shininess << endl; // 23
            // Texture Mapping Info
            fh << sceneObjects->at(i)->texture->texture << endl; // 24
            fh << sceneObjects->at(i)->texture->applyTexture << endl; // 25
        }
        fh.close();
        cout << "Scene saved successfully!" << endl;
    }
    else
    {
        cout << "Unable to open file..." << endl;
    }
}

// Load scene from file
void loadScene()
{
    using namespace std;
    // Clear current scene
    if (currentObject != NULL)
        resetSceneGraph();
    // Get the file name via CLI
    string file,
        line,
        filename;
    cout << "Load from file: ";
    cin >> filename;
    nextChild = 0;
    currentObjIndex = 0;

    ifstream fh(filename.c_str());
    if (fh.is_open())
    {
        cout << "Loading scene from: " + filename << endl;
        int i = 0,
            id = 0,
            texture,
            textureOn;
        NodeType type;
        TransformationNode *translateNode,
            *rotationNode,
            *scaleNode;
        ShapeNode *shape;
        MaterialNode *materialNode;
        Color amb,
            diff,
            spec;
        TextureNode *textureNode;
        Vector3 pTrans,
            pRotate,
            pScale;
        float x,
            y,
            z,
            a,
            shininess;

        while (getline(fh, line))
        {
            if (i == 0)
            {
                ShapeNode *s = new ShapeNode(getNodeType((string)line));
                type = getNodeType((string)line);
                shape = s;
                // cout << "Convert Shape String into NodeType: " << type << endl;
            }
            else if (i == 1)
            {
                id = stoi(line);
                // cout << "Shape ID: " << id << endl;
            }
            else if (i == 2 || i == 5 || i == 8 || i == 11 || i == 15 || i == 19)
            {
                x = stof(line);
                // cout << "Create X co-ordinate: " << i << ", " << x << endl;
            }
            else if (i == 3 || i == 6 || i == 9 || i == 12 || i == 16 || i == 20)
            {
                y = stof(line);
                // cout << "Create Y co-ordinate: " << i << ", " << y << endl;
            }
            else if (i == 4 || i == 7 || i == 10 || i == 13 || i == 17 || i == 21)
            {
                z = stof(line);
                // cout << "Create Z co-ordinate: " << i << ", " << z << endl;
                switch (i)
                {
                case 4:
                    pTrans = Vector3(x, y, z);
                    // cout << "Applying Shape Translation" << endl;
                    break;
                case 7:
                    pRotate = Vector3(x, y, z);
                    // cout << "Applying Shape Rotation" << endl;
                    break;
                case 10:
                    pScale = Vector3(x, y, z);
                    // cout << "Applying Shape Scale" << endl;
                    break;
                }
            }
            else if (i == 14 || i == 18 || i == 22)
            {
                a = stof(line);
                // cout << "Create Color Alpha: " << i << ", " << a << endl;
                switch (i)
                {
                    case 14:
                        amb = Color(x, y, z, a);
                        break;
                    case 18:
                        diff = Color(x, y, z, a);
                        break;
                    case 22:
                        spec = Color(x, y, z, a);
                        break;
                }
            }
            else if (i == 23)
            {
                shininess = stof(line);
                // cout << "Get Material Shininess: " << shininess << endl;
            }
            else if (i == 24)
            {
                texture = stoi(line);
                // cout << "Get Texture ID: " << texture << endl;
            }
            else if (i == 25)
            {
                textureOn = stoi(line);
                // cout << "Get Texture Status: " << textureOn << endl;
                i = -1;
            }
            if (i == -1)
            {
                // cout << "Create the SceneObject..." << endl;
                sceneGraph->toRoot();
                Material defualtMat = Material(amb, diff, spec, shininess);
                GroupNode *group = new GroupNode();
                sceneGraph->insertNode(group);
                sceneGraph->toChild(nextChild++);
                int groupID = sceneGraph->getCurrentNodeID();
                translateNode = new TransformationNode(TRANSLATE, pTrans);
                sceneGraph->insertNode(translateNode);
                sceneGraph->toChild(0);
                rotationNode = new TransformationNode(ROTATE, pRotate);
                sceneGraph->insertNode(rotationNode);
                sceneGraph->toChild(0);
                scaleNode = new TransformationNode(SCALE, pScale);
                sceneGraph->insertNode(scaleNode);
                sceneGraph->toChild(0);
                materialNode = new MaterialNode(defualtMat);
                sceneGraph->insertNode(materialNode);
                sceneGraph->toChild(0);
                textureNode = new TextureNode(textures[texture], type);
                sceneGraph->insertNode(textureNode);
                sceneGraph->toChild(0);
                sceneGraph->insertNode(shape);
                HitBox *myhitbox = new HitBox(Vector3(), 2);
                SceneObject *newObject = new SceneObject(
                    groupID,
                    translateNode, rotationNode, scaleNode,
                    shape,
                    materialNode,
                    textureNode,
                    myhitbox);
                insertSceneObject(newObject);
                currentObject->texture->applyTexture = textureOn;
            }
            i++;
        }
        fh.close();
        cout << "Scene Loaded!" << endl;
    }
    else
    {
        cout << "Unable to open file..." << endl;
    }
}

// Find closest object intersected by ray
void findMinBoxDistance(double R0[3], std::vector<int> hitList)
{
    // Find closest object
    int minDistance = 1000000;
    int minDistId = NULL;

    SceneObject *finalMinDistObj = NULL;

    Vector3 rayOrigin = Vector3();
    rayOrigin.x = R0[0];
    rayOrigin.y = R0[1];
    rayOrigin.z = R0[2];

    for (int i = 0; i < sceneObjects->size(); i++) {
        SceneObject *minDistObj = sceneObjects->at(i);
        if (std::find(hitList.begin(), hitList.end(), minDistObj->id) != hitList.end()) {
            float dist = rayOrigin.distance(minDistObj->translate->transformations);
            if (dist < minDistance) {
                minDistance = dist;
                minDistId = minDistObj->id;
                finalMinDistObj = minDistObj;
            }
        } else {
            continue;
        }
    }
    std::cout << "min id: " << minDistId << std::endl;

    //has correct id, loop through scene objects to find right object
    if (finalMinDistObj != NULL) {
        if (deleteOnClick) {
            // Delete
            deleteSceneObject(finalMinDistObj->id);
        } else {
            // Select
            currentObject->deselect();
            finalMinDistObj->select();
            currentObject = finalMinDistObj;
        }
    }
}

void findMinLightDistance(double R0[3], std::vector<int> hitList)
{
    // Find closest object
    int minDistance = 1000000;
    int minDistId = NULL;

    Vector3 rayOrigin = Vector3();
    rayOrigin.x = R0[0];
    rayOrigin.y = R0[1];
    rayOrigin.z = R0[2];

    Light *finalMinDistLight = NULL;

    if (hitList.size() < 1) {
        light0->selected = false;
        light1->selected = false;
    } else if (hitList.size() == 1) {
        if (hitList.at(0) == 0) {
            light0->selected = true;
            light1->selected = false;
        } else {
            light1->selected = true;
            light0->selected = false;
        }
    } else {
        for (int i = 0; i < hitList.size(); i++) {
            Light *minDistLight = lights.at(i);
            if (std::find(hitList.begin(), hitList.end(), minDistLight->id) != hitList.end()) {
                float dist = rayOrigin.distance(minDistLight->position);
                if (dist < minDistance) {
                    minDistance = dist;
                    minDistId = minDistLight->id;
                    finalMinDistLight = minDistLight;
                }
            } else {
                continue;
            }
        }
    }

    //has correct id, loop through scene objects to find right object
    if (finalMinDistLight != NULL) {
        finalMinDistLight->selected = true;
    }
}

Vector3 rayPlaneIntersection(Vector3 origin, double R0[3], double Rd[3])
{
    Vector3 p = Vector3();
    Vector3 normal = Vector3(0, 1, 0);
    float D, dot, dotR0, t;
    D = -1 * normal.x * origin.x -
        normal.y * origin.y -
        normal.z * origin.z,
    dot = normal.x * Rd[0] +
          normal.y * Rd[1] +
          normal.z * Rd[2];
    if (abs(dot) < 0.00001f) {
        return Vector3(NULL, NULL, NULL);
    } else {
        dotR0 = normal.x * R0[0] +
                normal.y * R0[1] +
                normal.z * R0[2];
        t = (-1 * (dotR0 + D)) / dot;
        p.x = R0[0] + t * Rd[0];
        p.y = R0[1] + t * Rd[1];
        p.z = R0[2] + t * Rd[2];
        std::cout << "p: " << p.x << ", " << p.y << ", " << p.z << std::endl;
        return p;
    }
}

bool rayBoxIntersection(double R0[3], double Rd[3])
{
    //all id of hit objects
    std::vector<int> objHitBox;
    double tMin, tMax, tmp;

    for (int i = 0; i < sceneObjects->size(); i++) {
        bool hit = false;
        SceneObject *obj = sceneObjects->at(i);
        
        Vector3 origin = obj->translate->transformations;
        std::cout << "origin: " << origin.x << ", " << origin.y << ", " << origin.z << std::endl;
        
        Vector3 scale = obj->scale->transformations;
        std::cout << "scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;

        Vector3 rotation = obj->rotation->transformations;
        std::cout << "rotation: " << rotation.x << ", " << rotation.y << ", " << rotation.z << std::endl;

        float hitboxMag = obj->hitBox->magnitude;
        std::cout << "length: " << hitboxMag << std::endl;

        Vector3 lowPoint = Vector3(),
                highPoint = Vector3();

        lowPoint.x = origin.x - (scale.x * hitboxMag) / 2;
        lowPoint.y = origin.y - (scale.y * hitboxMag) / 2;
        lowPoint.z = origin.z - (scale.z * hitboxMag) / 2;
        highPoint.x = origin.x + (scale.x * hitboxMag) / 2;
        highPoint.y = origin.y + (scale.y * hitboxMag) / 2;
        highPoint.z = origin.z + (scale.z * hitboxMag) / 2;

        //x side
        double txMin = (lowPoint.x - R0[0]) / Rd[0];
        double txMax = (highPoint.x - R0[0]) / Rd[0];

        if (txMin > txMax) {
            txMax = tmp;
            txMax = txMin;
            txMin = tmp;
        }
        
        //y side
        double tyMin = (lowPoint.y - R0[1]) / Rd[1];
        double tyMax = (highPoint.y - R0[1]) / Rd[1];

        if (tyMax < tyMin) {
            tyMax = tmp;
            tyMax = tyMin;
            tyMin = tmp;
        }

        //z side
        double tzMin = (lowPoint.z - R0[2]) / Rd[2];
        double tzMax = (highPoint.z - R0[2]) / Rd[2];

        if (tzMax < tzMin) {
            tzMax = tmp;
            tzMax = tzMin;
            tzMin = tmp;
        }

        if (txMin > tyMin) {
            tMin = txMin;
        } else {
            tMin = tyMin;
        }
        if (txMax < tyMax) {
            tMax = txMax;
        } else {
            tMax = tyMax;
        }

        if (txMin > tyMax || tyMin > txMax || tMin > tzMax || tzMin > tMax) {
            hit = false;
        } else {
            if (tzMin > tMin) {
                tMin = tzMin;
            }
            if (tzMax < tMax) {
                tMax = tzMax;
            }
            std::cout << "hit at: " << tMin << ", " << tMax << std::endl;
            objHitBox.push_back(obj->id);
        }
    }
    //find closest box if any and highlight
    findMinBoxDistance(R0, objHitBox);
}

//Ray Sphere Intersection
void raySphereIntersection(double R0[3], double Rd[3]) {  
    std::vector<int> objHitSphere;
    double A = Rd[0] * Rd[0] + Rd[1] * Rd[1] + Rd[2] * Rd[2];
    double* R0Pc = new double[3];

    // search for all lights hit by ray
    for (int i = 0; i < lights.size(); i++) {
        Light *testLight = lights.at(i);

        // Get light position
        Vector3 origin = testLight->position;

        R0Pc[0] = R0[0] - origin.x;
        R0Pc[1] = R0[1] - origin.y;
        R0Pc[2] = R0[2] - origin.z;

        double B = 2 * (R0Pc[0] * Rd[0] + R0Pc[1] * Rd[1] + R0Pc[2] * Rd[2]);
		double C = (R0Pc[0] * R0Pc[0] + R0Pc[1] * R0Pc[1] + R0Pc[2] * R0Pc[2]) - 1;

        double discriminant = B*B - 4 * A * C;

        if (!(discriminant < 0)) {
            double t0 = (-B + sqrt(discriminant)) / (2 * A);
            double t1 = (-B - sqrt(discriminant)) / (2 * A);
            objHitSphere.push_back(testLight->id);
        }
    }
    findMinLightDistance(R0, objHitSphere);
}

// Calculate and deal with intersection logic
void calculateRay()
{
    GLdouble R0[3], R1[3], Rd[3];
    GLdouble modelMatrix[16], projMatrix[16];
    GLint viewport[4];

    // Populate Matrices
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Calculate Near Point
    gluUnProject(mouseX, mouseY, 0.0, modelMatrix, projMatrix, viewport, &R0[0], &R0[1], &R0[2]);
    
    // Calculate Far Point
    gluUnProject(mouseX, mouseY, 1.0, modelMatrix, projMatrix, viewport, &R1[0], &R1[1], &R1[2]);

    // Find direction
    double xDiff = R1[0] - R0[0];
    double yDiff = R1[1] - R0[1];
    double zDiff = R1[2] - R0[2];

    // Normalize direction
    double mag = sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
    Rd[0] = xDiff / mag;
    Rd[1] = yDiff / mag;
    Rd[2] = zDiff / mag;

    rayBoxIntersection(R0, Rd);
    raySphereIntersection(R0, Rd);
}

// Linearly iterate through scene object vector, selecting the next one each time
void iterateThroughSceneObjects()
{
    if (currentObject != NULL)
    {
        // Deselect current object
        currentObject->deselect();
        int id = currentObject->id;

        // Find objects vector index of current object
        for (int i = 0; i < sceneObjects->size() - 1; i++)
        {
            currentObject = sceneObjects->at(i);
            if (currentObject->id == id)
            {
                id = i;
                break;
            }
        }

        // Find next object
        id = (id > sceneObjects->size() - 1) ? 0 : id + 1;
        currentObject = sceneObjects->at(id);
        currentObject->select();
    }
}

// Display Callback Function
void display()
{
    // Clear and prepare
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Render Camera Settings
    cam.render();
    // Render the Showroom Background
    renderShowroom();
    glLoadIdentity(); // Reset ModelView Matrix
    glRotatef(angleX, 0.0f, 0.0f, 1.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    // Render the SceneGraph
    sceneGraph->render();
    // Redraw lights if needed
    renderLights();
    // Swap Buffers
    glutSwapBuffers();
};

// Reshape Callback Function
void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f, 500.0f);
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
};

// Keyboard Callback Function
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            // Quit the program
            exit(0);
            break;
        case 'r':
        case 'R':
            // Reset the scene
            resetSceneGraph();
            resetLights();
            break;
        case '[':
            // Save current scene
            saveScene();
            break;
        case ']':
            // Load scene to current
            loadScene();
            break;
        case 'p':
        case 'P':
            // Cycle through lights
            if (light0_on && light1_on)
            {
                light1_on = false;
                light1->disable();
            }
            else if (light0_on && !light1_on)
            {
                light0_on = false;
                light0->disable();
            }
            else if (!light0_on && !light1_on)
            {
                light0_on = true;
                light0->enable();
                light1_on = true;
                light1->enable();
            }
            break;
        case '/':
            // Iterate currently selected
            iterateThroughSceneObjects();
            break;
        case '?':
            // Delete currently selected
            if (currentObject != NULL)
            {
                deleteSceneObject(currentObject->id);
            }
            break;
        case 'w':
        case 'W':
            // Translate selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 translation = Vector3();
                translation.z = -1.0f;
                currentObject->translateBy(translation);
            }
            break;
        case 's':
        case 'S':
            // Translate selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 translation = Vector3();
                translation.z = 1.0f;
                currentObject->translateBy(translation);
            }
            break;
        case 'a':
        case 'A':
            // Translate selected object along X axis
            if (currentObject != NULL)
            {
                Vector3 translation = Vector3();
                translation.x = -1.0f;
                currentObject->translateBy(translation);
            }
            break;
        case 'd':
        case 'D':
            // Translate selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 translation = Vector3();
                translation.x = 1.0f;
                currentObject->translateBy(translation);
            }
            break;
        case 'q':
        case 'Q':
            // Translate selected object along Y axis
            if (currentObject != NULL)
            {
                Vector3 translation = Vector3();
                translation.y = -1.0f;
                currentObject->translateBy(translation);
            }
            break;
        case 'e':
        case 'E':
            // Translate selected object along Y axis
            if (currentObject != NULL)
            {
                Vector3 translation = Vector3();
                translation.y = 1.0f;
                currentObject->translateBy(translation);
            }
            break;
        case 'f':
        case 'F':
            // Scale selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 scale = Vector3();
                scale.z = -1.0f;
                currentObject->scaleBy(scale);
            }
            break;
        case 'b':
        case 'B':
            // Scale selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 scale = Vector3();
                scale.z = 1.0f;
                currentObject->scaleBy(scale);
            }
            break;
        case 't':
        case 'T':
            // Scale selected object along X axis
            if (currentObject != NULL)
            {
                Vector3 scale = Vector3();
                scale.x = -1.0f;
                currentObject->scaleBy(scale);
            }
            break;
        case 'g':
        case 'G':
            // Scale selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 scale = Vector3();
                scale.x = 1.0f;
                currentObject->scaleBy(scale);
            }
            break;
        case 'y':
        case 'Y':
            // Scale selected object along Y axis
            if (currentObject != NULL)
            {
                Vector3 scale = Vector3();
                scale.y = 1.0f;
                currentObject->scaleBy(scale);
            }
            break;
        case 'h':
        case 'H':
            // Scale selected object along Y axis
            if (currentObject != NULL)
            {
                Vector3 scale = Vector3();
                scale.y = -1.0f;
                currentObject->scaleBy(scale);
            }
            break;
        case 'u':
        case 'U':
            // Rotate selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 rotation = Vector3();
                rotation.z = 1.0f;
                currentObject->rotateBy(rotation);
            }
            break;
        case 'j':
        case 'J':
            // Rotate selected object along Z axis
            if (currentObject != NULL)
            {
                Vector3 rotation = Vector3();
                rotation.z = -1.0f;
                currentObject->rotateBy(rotation);
            }
            break;
        case 'i':
        case 'I':
            // Rotate selected object along X axis
            if (currentObject != NULL)
            {
                Vector3 rotation = Vector3();
                rotation.x = -1.0f;
                currentObject->rotateBy(rotation);
            }
            break;
        case 'k':
        case 'K':
            // Rotate selected object along X axis
            if (currentObject != NULL)
            {
                Vector3 rotation = Vector3();
                rotation.x = 1.0f;
                currentObject->rotateBy(rotation);
            }
            break;
        case 'o':
        case 'O':
            // Rotate selected object along Y axis
            if (currentObject != NULL)
            {
                Vector3 rotation = Vector3();
                rotation.y = -1.0f;
                currentObject->rotateBy(rotation);
            }
            break;
        case 'l':
        case 'L':
            // Rotate selected object along Y axis
            if (currentObject != NULL)
            {
                Vector3 rotation = Vector3();
                rotation.y = 1.0f;
                currentObject->rotateBy(rotation);
            }
            break;
        case 'c':
        case 'C':
            // Move light -y
            if (light0->selected && light0->position.y > -2) {
                light0->position.y -= 1.0f;
                light0pos[1] -= 1.0f;
                glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
            } else if (light1->selected && light1->position.y > -2) {
                light1->position.y -= 1.0f;
                light1pos[1] -= 1.0f;
                glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
            }
            break;
        case 'v':
        case 'V':
            // Move light +y
            if (light0->selected) {
                light0->position.y += 1.0f;
                light0pos[1] += 1.0f;
                glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
            } else if (light1->selected) {
                light1->position.y += 1.0f;
                light1pos[1] += 1.0f;
                glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
            }
            break;
        case 'n':
        case 'N':
            // Move light -z
            if (light0->selected) {
                light0->position.z -= 1.0f;
                light0pos[2] -= 1.0f;
                glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
            } else if (light1->selected) {
                light1->position.z -= 1.0f;
                light1pos[2] -= 1.0f;
                glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
            }
            break;
        case 'm':
        case 'M':
            // Move light +z
            if (light0->selected) {
                light0->position.z += 1.0f;
                light0pos[2] += 1.0f;
                glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
            } else if (light1->selected) {
                light1->position.z += 1.0f;
                light1pos[2] += 1.0f;
                glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
            }
            break;
        case ',':
            // Move light -x
            if (light0->selected) {
                light0->position.x -= 1.0f;
                light0pos[0] -= 1.0f;
                glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
            } else if (light1->selected) {
                light1->position.x -= 1.0f;
                light1pos[0] -= 1.0f;
                glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
            }
            break;
        case '.':
            // Move light +x
            if (light0->selected) {
                light0->position.x += 1.0f;
                light0pos[0] += 1.0f;
                glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
            } else if (light1->selected) {
                light1->position.x += 1.0f;
                light1pos[0] += 1.0f;
                glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
            }
            break;
        case '<':
            // Toggle texture mapping
            currentObject->texture->applyTexture = !currentObject->texture->applyTexture;
            break;
        default:
            break;
    }
    glutPostRedisplay();
};

// Special Keyboard Callback Function
void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            angleY += 1.0f;
            cam.position.z -= 1.0f;
            break;
        case GLUT_KEY_DOWN:
            angleY -= 1.0f;
            cam.position.z += 1.0f;
            break;
        case GLUT_KEY_LEFT:
            angleX -= 1.0f;
            cam.position.x -= 1.0f;
            break;
        case GLUT_KEY_RIGHT:
            angleX += 1.0f;
            cam.position.x += 1.0f;
            break;
    }
    glutPostRedisplay();
};

// Mouse Callback Function
void mouse(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = windowHeight - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        deleteOnClick = false;
        calculateRay();
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        deleteOnClick = true;
        calculateRay();
    }

    glutPostRedisplay();
};

// Load textures -- Provided by Google
GLubyte *LoadPPM(char *file, int *width, int *height, int *max)
{
    GLubyte *img;
    FILE *fd;
    int n, m, k, nm, i, red, green, blue;
    char c, b[100];
    float s;
    fd = fopen(file, "r");
    fscanf(fd, "%[^\n] ", b);
    if (b[0] != 'P' || b[1] != '3')
    {
        printf("%s is not a PPM file!\n", file);
        exit(0);
    }
    // printf("%s is a PPM file\n", file);
    fscanf(fd, "%c", &c);
    while (c == '#')
    {
        fscanf(fd, "%[^\n] ", b);
        // printf("%s\n",b);
        fscanf(fd, "%c", &c);
    }
    ungetc(c, fd);
    fscanf(fd, "%d %d %d", &n, &m, &k);

    // printf("%d rows  %d columns  max value= %d\n",n,m,k);

    nm = n * m;

    img = (GLubyte *)malloc(3 * sizeof(GLuint) * nm);

    s = 255.0 / k;

    for (i = 0; i < nm; i++)
    {
        fscanf(fd, "%d %d %d", &red, &green, &blue);
        img[3 * nm - 3 * i - 3] = red * s;
        img[3 * nm - 3 * i - 2] = green * s;
        img[3 * nm - 3 * i - 1] = blue * s;
    }

    *width = n;
    *height = m;
    *max = k;

    return img;
}

// Load Textures into Memory
void initTexture()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    char *marble_file = "textures/marble.ppm";
    char *crates_file = "textures/crate.ppm";
    char *stone_file = "textures/stone.ppm";
    char *iron_file = "textures/iron.ppm";

    marble_texture = LoadPPM(marble_file, &width1, &height1, &max1);
    crates_texture = LoadPPM(crates_file, &width2, &height2, &max2);
    stone_texture = LoadPPM(stone_file, &width3, &height3, &max3);
    iron_texture = LoadPPM(iron_file, &width4, &height4, &max4);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(4, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, marble_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, crates_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, stone_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width4, height4, 0, GL_RGB, GL_UNSIGNED_BYTE, iron_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glDisable(GL_TEXTURE_2D);
}

// Glut Initialization Function
void init()
{
    // Prepare the window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(WINDOW_TITLE);
    glutInitWindowPosition(100, 100);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);
    // Viewport settings
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
    // Lights!
    lights.push_back(light0);
    lights.push_back(light1);
    renderLights();
    // Instantiate the scene graph
    sceneGraph = new SceneGraph();
    // Textures Please!
    initTexture();
};

// Print Program Instructions
void printInstructions()
{
    using namespace std;
    cout << "Program controls:" << endl;
    cout << ">> [Quit Program] - Esc" << endl;
    cout << ">> [Reset Scene Graph] - r/R" << endl;
    // Save and Load
    cout << ">> [Save Scene Graph] - [" << endl;
    cout << ">> [Load Scene Graph] - ]" << endl;
    // Program interaction
    cout << ">> [Toggle Active Lights] - p/P" << endl;
    cout << ">> [Change Selected Item] - Left Click/'/' " << endl;
    cout << ">> [Delete Scene Object] - Right Click/?" << endl;
    // Shape Menu
    cout << ">> [Model Control Menu] - Middle Click" << endl;
    // Shape Translation Controls
    cout << ">> [Shape Translation - Z-Axis] - w/W|s/S" << endl;
    cout << ">> [Shape Translation - X-Axis] - a/A|d/D" << endl;
    cout << ">> [Shape Translation - Y-Axis] - q/Q|e/E" << endl;
    // Shape Scale Controls
    cout << ">> [Shape Scale - Z-Axis] - f/F|b/B" << endl;
    cout << ">> [Shape Scale - X-Axis] - t/T|g/G" << endl;
    cout << ">> [Shape Scale - Y-Axis] - y/Y|h/H" << endl;
    // Shape Rotation Controls
    cout << ">> [Shape Rotation - Z-Axis] - u/U|j/J" << endl;
    cout << ">> [Shape Rotation - X-Axis] - i/I|k/K" << endl;
    cout << ">> [Shape Rotation - Y-Axis] - o/O|l/L" << endl;
    // Light controls
    cout << ">> [Selected Light - Z-Axis] - n/N|m/M" << endl;
    cout << ">> [Selected Light - X-Axis] - ,|." << endl;
    cout << ">> [Selected Light - Y-Axis] - c/C|v/V" << endl;
    // Texture Toggle
    cout << ">> [Toggle Texture] - <" << endl;
    // Camera Controls
    cout << ">> [Camera - Z-Axis] - Up/Down" << endl;
    cout << ">> [Camera - X-Axis] - Left/Right" << endl;
};

// Menu callback function
void menu(int value)
{
    // Call appropriate event command
    switch (value)
    {
    case 1:
        insertSceneObject(CUBE);
        break;
    case 2:
        insertSceneObject(SPHERE);
        break;
    case 3:
        insertSceneObject(TEAPOT);
        break;
    case 4:
        insertSceneObject(TORUS);
        break;
    case 5:
        insertSceneObject(TETRAHEDRON);
        break;
    case 6:
        if (currentObject != NULL)
        {
            currentObject->material_id = 1;
            currentObject->material->material.setMaterial(CHROME);
        }
        break;
    case 7:
        if (currentObject != NULL)
        {
            currentObject->material_id = 2;
            currentObject->material->material.setMaterial(GOLD);
        }
        break;
    case 8:
        if (currentObject != NULL)
        {
            currentObject->material_id = 3;
            currentObject->material->material.setMaterial(TIN);
        }
        break;
    case 9:
        if (currentObject != NULL)
        {
            currentObject->material_id = 4;
            currentObject->material->material.setMaterial(PERL);
        }
        break;
    case 10:
        if (currentObject != NULL)
        {
            currentObject->material_id = 5;
            currentObject->material->material.setMaterial(RUBBER);
        }
        break;
    case 11:
        if (currentObject != NULL)
        {
            currentObject->material_id = 6;
            currentObject->material->material.setMaterial(DEFAULT);
        }
        break;
    case 12:
        if (currentObject != NULL)
        {
            currentObject->texture->applyTexture = true;
            currentObject->texture->texture = textures[0];
        }
        break;
    case 13:
        if (currentObject != NULL)
        {
            currentObject->texture->applyTexture = true;
            currentObject->texture->texture = textures[1];
        }
        break;
    case 14:
        if (currentObject != NULL)
        {
            currentObject->texture->applyTexture = true;
            currentObject->texture->texture = textures[2];
        }
        break;
    case 15:
        if (currentObject != NULL)
        {
            currentObject->texture->applyTexture = true;
            currentObject->texture->texture = textures[3];
        }
        break;
    }
    // redraw the scene
    glutPostRedisplay();
}

// Quick Menu
void menuHandler()
{
    int shapeMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Cube", 1);
    glutAddMenuEntry("Sphere", 2);
    glutAddMenuEntry("Teapot", 3);
    glutAddMenuEntry("Torus", 4);
    glutAddMenuEntry("Tetrahedron", 5);
    glutAddMenuEntry("Chrome", 6);
    glutAddMenuEntry("Gold", 7);
    glutAddMenuEntry("Tin", 8);
    glutAddMenuEntry("Perl", 9);
    glutAddMenuEntry("Rubber", 10);
    glutAddMenuEntry("Default", 11);
    glutAddMenuEntry("Marble", 12);
    glutAddMenuEntry("Crate", 13);
    glutAddMenuEntry("Iron", 14);
    glutAddMenuEntry("Stone", 15);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

// Main program
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    // Initialize the program
    init();
    // Print instructions
    printInstructions();
    // Bind callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouse);
    // Bind Menus
    menuHandler();
    // Culling and 3D
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    // Start the main loop
    glutMainLoop();
    return 0;
};
