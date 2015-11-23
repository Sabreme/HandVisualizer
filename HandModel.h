#ifndef HANDMODEL_H
#define HANDMODEL_H
#include "Leap.h"

typedef double fingerJoints[5][3];          /// Each finger has 5 joints and 3 pt Vector
typedef double fingerBones[4][4];        /// Each Finger has 4 Bones and  4 pt Vector (Which includes  1 Length)
typedef double fingerLines[4][6];          /// Each finger has 4 Lines and a 2x3  pt Vector taken from  Joints

enum visibleHand  {rightHand, leftHand};

class HandModeller
{

public:
    void drawJoints(visibleHand activeHand);
    void drawBones(visibleHand activeHand);

    void jointRightStartPos(fingerJoints& joints, int finger);
    void jointLeftStartPos(fingerJoints& joints, int finger);
    void boneRightStartPos(fingerBones &bones, int finger);
    void boneLeftStartPos(fingerBones& bones, int finger);
};



#endif // HANDMODEL_H

