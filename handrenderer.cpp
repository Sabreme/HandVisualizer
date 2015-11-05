#include "handrenderer.h"

#define HAND_SPHERE_RADIUS 7.0f
#define HAND_SPHERE_DETAIL 32
#define HAND_CYLINDER_RADIUS 4.0f

//namespace  {
//Matrix44f GetBoneMatrix(const Leap::Bone& bone){

//}
//}

HandRenderer::HandRenderer()
{
    scale_ = 1.0f;
}

void HandRenderer::drawHands()
{
//    glPushMatrix();
//    glTranslatef(translation_);
//    glScalef(scale_, scale_, scale_);

//    Leap::Frame frame = controller_.frame();
//    for (int h = 0; h < frame.hands().count(); ++h)
//    {
//        Leap::Hand hand = frame.hands()[h];

//        for (int f = 0; f < hand.fingers().count(); ++f)
//        {
//            Leap::Finger finger = hand.fingers()[f];

//            // Draw first joint inside hand.
//            Leap::Bone mcp = finger.bone(Leap::Bone::Type::TYPE_METACARPAL);
//            drawJoint(mcp.prevJoint());

//            for (int b = 0; b < 4; ++b)
//            {
//                Leap::Bone bone = finger.bone(static_cast<Leap::Bone::Type>(b));
//                drawJoint(bone.nextJoint());
//                drawBone(bone);
//            }
//        }
//    }
//    glPopMatrix();
}

void HandRenderer::drawJoint(const Leap::Vector &joint_position)
{
//    static const ColorA joint_color(CM_HSV, 0.6, 0.5f, 1.0f, 0.5f);

//    glpushMatrix()
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, joint_color);
//    gl::drawSphere(Vec3f::zero(), HAND_SPHERE_RADIUS, HAND_SPHERE_DETAIL);
//    glPopMatrix();


}

void HandRenderer::drawBone(const Leap::Bone &bone)
{
//    static const ColorA bone_color(CM_HSV, 0.5f, 0.0f, 1.0f, 0.5f);

//      Leap::Vector midpoint = (bone.prevJoint() + bone.prevJoint()) / 2.0;

//      glPushMatrix();
//      glTranslatef(midpoint.x, midpoint.y, midpoint.z);
//      glMultMatrixf(GetBoneMatrix(bone));
//      glRotatef(90, -1, 0, 0);

//      glMaterialfv(GL_FRONT, GL_DIFFUSE, bone_color);
//      gl::drawCylinder(HAND_CYLINDER_RADIUS, HAND_CYLINDER_RADIUS, bone.length(), 12, 1);
//      glPopMatrix();
}


