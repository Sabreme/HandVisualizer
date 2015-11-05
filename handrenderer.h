#ifndef HANDRENDERER_H
#define HANDRENDERER_H

#include <vector>
#include "Leap.h"





typedef std::vector<std::pair<Leap::Vector, float> > pinch_list;

class HandRenderer
{
public:
    HandRenderer() ;

    void setScale(float scale)
    {
        scale_ = scale;
    }

    void setTranslation(Leap::Vector translation)
    {
        translation_ = translation;
    }

    void drawHands();


private:
    void drawJoint(const Leap::Vector& joint_position);
    void drawBone(const Leap::Bone& bone);

    Leap::Controller controller_;
    Leap::Vector translation_;
    float scale_;
};

#endif // HANDRENDERER_H
