#include "handwindow.h"
#include "ui_handwindow.h"

#define HAND_SPHERE_RADIUS 7.0f
#define HAND_SPHERE_DETAIL 32
#define HAND_CYLINDER_RADIUS 4.0f

#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"

#include "vtkMath.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"

#include "QTimer"

#include <sstream>

HandWindow::HandWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HandWindow)
{
    ui->setupUi(this);

    this->controller_ = new Controller;
    //this->listener_ = new SampleListener;

    timer = new QTimer(this);
    timer->setInterval(1000/60);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMe()));


}

HandWindow::~HandWindow()
{
    delete ui;
}

void HandWindow::on_buttonApply_clicked()
{
     timer->start();

     // Create the RenderWindow
    vtkRenderer * ren1 = vtkRenderer::New();
    global_Renderer = ren1;

    vtkRenderWindow * renWin = vtkRenderWindow::New();
    global_Window = renWin;

    renWin->AddRenderer(ren1);

    ren1->SetBackground(0.1, 0.2, 0.4);

    this->ui->widget->SetRenderWindow(renWin);
    this->ui->widget->GetInteractor()->SetRenderWindow(renWin);
    global_Interactor = this->ui->widget->GetInteractor();



    ///// WE INITIALISE THE HAND FIRST /////
    //// THE JOINTS
    vtkSmartPointer<vtkSphereSource> jointSource =
            vtkSmartPointer<vtkSphereSource>::New();
    jointSource->SetRadius(0.05);

      vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(jointSource->GetOutputPort());

//    /// Loop through each of the Fingers
//    /// Then in Each Finger, loop through each of the Joints
//    for (int f = 0; f < 5; f++)
//    {
//        fingerJoints  newJoints;                    /// Create a newJoints Object for each finger
//        jointPositions(newJoints,f);                /// We get the defualt joints for the current finger

//        for(int j = 0 ; j < 5; j++)                     /// We loop through each joint and create actor
//        {
//         global_Joints[f][j] = vtkActor::New();
//         global_Joints[f][j]->SetMapper(mapper);

//        global_Joints[f][j]->GetProperty()->SetColor(2, 2, 2);
//        global_Joints[f][j]->GetProperty()->SetOpacity(0.5);

//                                                        /// We get the position from the newJoints [B][x,y,z]
//        scale_ = 0.01;

//        global_Joints[f][j]->SetPosition(newJoints[j][0] * scale_,
//                                                newJoints[j][1] * scale_,
//                                                newJoints[j][2] * scale_ );
//        ///global_Joints[f][j]->SetPosition((f*.1), (j*.1), (f+j)*.1);
//         ///global_FingerTip[f][j]->SetPosition(tipPos[i]);
//         global_Renderer->AddActor(global_Joints[f][j]);
//        }
//    }


    //// THE BONES
    vtkSmartPointer<vtkCylinderSource> boneSource =
            vtkSmartPointer<vtkCylinderSource>::New();
    boneSource->SetRadius(0.05);

     mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(boneSource->GetOutputPort());

    /// Loop through each of the Fingers
    /// Then in Each Finger, loop through each of the bones
    for (int f = 0; f < 5; f++)
    {
        fingerBones  newBones;                    /// Create a newBones Object for each finger
        bonePositions(newBones,f);                /// We get the defualt joints for the current finger

        for(int j = 0 ; j < 4; j++)                     /// We loop through each Bone and create actor
        {
         global_Bones[f][j] = vtkActor::New();
         global_Bones[f][j]->SetMapper(mapper);

        global_Bones[f][j]->GetProperty()->SetColor(2, 2, 2);
        global_Bones[f][j]->GetProperty()->SetOpacity(0.5);

                                                        /// We get the position from the newJoints [B][x,y,z]
        scale_ = 0.01;

        global_Bones[f][j]->SetPosition(newBones[j][0] * scale_,
                                                newBones[j][1] * scale_,
                                                newBones[j][2] * scale_ );
        ///global_Joints[f][j]->SetPosition((f*.1), (j*.1), (f+j)*.1);
         ///global_FingerTip[f][j]->SetPosition(tipPos[i]);
         global_Renderer->AddActor(global_Bones[f][j]);
        }
    }
}

void HandWindow::updateMe()
{
    this->ui->widget->GetRenderWindow()->Render();

    if(controller_->isConnected())  // controller is a controller object
    {
        /// Get the most recent frame and report some basic information
        const Frame frame = controller_->frame();


        bool shouldTranslate = true;
        bool shouldRotate = true;
        bool shouldScale = true;

        Leap::Matrix mtxFrameTransform ;


//        if (global_Initialised == true)
//        {
//            pointWidget->GetProperty()->SetColor(1.0, 1.0, 1.0);
//            global_SphereActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
//            static_cast<vtkSliderRepresentation3D*>(global_Slider->GetRepresentation())->SetValue(scaling_Start);
//        }


        mtxFrameTransform.origin = Leap::Vector(0.0, 0.0, 0.0);
//        frameScale = 0.0075;

        shouldTranslate = frame.translationProbability(controller_->frame(1)) > 0.40;
        shouldRotate = frame.rotationProbability(controller_->frame(1)) > 0.40;
        shouldScale = frame.scaleProbability(controller_->frame(1)) > 0.40;
        //pointWidget->GetProperty()->SetColor(0.0, 1.0, 0.0);

        //Leap::Finger front = frame.fingers.frontmost();

        Leap::Hand hand  = frame.hands()[0];                        /// Select Hand and get each finger
        for (int f = 0; f < hand.fingers().count(); f++ )               /// For each finger, we get the joints
        {
            Leap::Finger finger = hand.fingers()[f];
            Leap::Bone mcp = finger.bone(Leap::Bone::TYPE_METACARPAL);

            Leap::Vector midpointMETA  = mcp.prevJoint() + mcp.prevJoint() / 2.0;

            /// We Get the location of the joint inside the hand
            std::cout << std::fixed << std::setprecision(1) <<
                          "----FINGER " << f << "-----\n" <<

                            "{" << midpointMETA.x <<
                            "," << midpointMETA.y <<
                            "," << midpointMETA.z  <<
                            "}," ;


            /// We interate through the bones and get the joints between each of them on each finger
            /// REMEMBER: We have 5 Joins for the 4 Bones for EACH of the 5 Fingers
            for (int b = 0; b < 4 ; b++)
            {
                Leap::Bone bone = finger.bone(static_cast<Leap::Bone::Type>(b));
                joints[b] = bone.nextJoint() + bone.nextJoint() / 2.0;

                std::cout << "\n" <<
                                "{" << joints[b].x <<
                                "," << joints[b].y <<
                                "," << joints[b].z <<
                                "}," ;

            }
            std::cout <<  endl;

        }


//        //        /// Get the most recent frame and report some basic information
//                std::cout   << "Frame id: " << frame.id()
////                            << ", timestamp: " << frame.timestamp()
////                            << ", hands: " << frame.hands().count()
////                            << ", fingers: " << frame.fingers().count()
////                            << ", Rotation (Y): " << shouldRotate
////                            << ", Translate (Y): " << shouldTranslate
////                            << ", Scale (Y): " << shouldScale

//                            //<< ", tools: " << frame.tools().count()
//                            //<< ", gestures: " << frame.gestures().count()
//                            //<< ", PalmNormal: " << frame.hands()[0].palmNormal()
//                            //<< ", PalmDirect: " << frame.hands()[0].direction()
//                            //<< ", yaw: " << frame.hands()[0].direction().yaw()
//                            //<< ", Angle: " << frame.rotationAngle(controller_->frame(1))
//                            << std::endl;

//        std::cout << "TRACKING: " << frame.id() << endl;

        if (!frame.hands().isEmpty() && !frame.hands()[0].fingers().isEmpty())
        {
            //                    Get the first hand
            const Hand hand = frame.hands().rightmost();
            //                    Get the 2nd hand
            const Hand hand2 = frame.hands().leftmost();



            if(frame.hands().count() ==1)
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////    Finger Joints  TRACKING  /////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////


                for (int f = 0; f < hand.fingers().count(); f++ )               /// For each finger, we get the joints
                {
                    Leap::Finger finger = hand.fingers()[f];
                    Leap::Bone mcp = finger.bone(Leap::Bone::TYPE_METACARPAL);

                    Leap::Vector midpointMETA  = mcp.prevJoint() + mcp.prevJoint() / 2.0;

                    /// We Get the location of the joint inside the hand
                    double sensitivity = 0.01;
                    double jointPosPoint[3] = {midpointMETA.x * sensitivity ,
                                               midpointMETA.y * sensitivity,
                                               midpointMETA.z * sensitivity
                                              };

                    global_Joints[f][0]->SetPosition(jointPosPoint);    ///joint Position.



                    /// We interate through the bones and get the joints between each of them on each finger
                    /// REMEMBER: We have 5 Joins for the 4 Bones for EACH of the 5 Fingers
                    for (int b = 0; b < 4 ; b++)
                    {
                        Leap::Bone bone = finger.bone(static_cast<Leap::Bone::Type>(b));
                        Leap::Vector bonePosition  = bone.nextJoint() + bone.nextJoint() / 2.0;

                        double sensitivity = 0.01;
                        double jointPosPoint[3] = { bonePosition.x * sensitivity ,
                                                    bonePosition.y * sensitivity,
                                                    bonePosition.z * sensitivity
                                                  };

                        global_Joints[f][b+1]->SetPosition(jointPosPoint);    ///joint Position.
                    }    /// for (int b = 0)
                }   ///  for (int f = 0; )
            }    /// if(frame.hands().count() ==1)
        }   ///    if (!frame.hands().isEmpty()
    }   ///  if(controller_->isConnected())
}

void HandWindow::on_buttonStop_clicked()
{
//    this->tim
    timer->stop();
//    if (controller_ != NULL)
//    {
//        if(controller_->isConnected())
//            controller_->~Controller();
    //    }
}

void HandWindow::drawHands()
{

}

void HandWindow::drawJoint(const Vector &joint_position)
{

    ///    static const ColorA joint_color(CM_HSV, 0.6, 0.5f, 1.0f, 0.5f);       //  The Colour

        vtkSmartPointer<vtkSphereSource> joint =
                vtkSmartPointer<vtkSphereSource>::New();
        joint->SetRadius(HAND_SPHERE_RADIUS);

        vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(joint->GetOutputPort());

            vtkActor * jointActor = vtkActor::New();
            jointActor->SetMapper(mapper);

             jointActor->GetProperty()->SetColor(2, 2, 2);
             jointActor->GetProperty()->SetOpacity(0.5);

             jointActor->SetPosition(joint_position.x, joint_position.y, joint_position.z);
             global_Renderer->AddActor(jointActor);


    ///    glPushMatrix();                                                                   //
    ///     glTranslatef(joint_position.x, joint_position.y, joint_position.z);

    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, joint_color);
    //    gl::drawSphere(Vec3f::zero(), HAND_SPHERE_RADIUS, HAND_SPHERE_DETAIL);
    //    glPopMatrix();


}

void HandWindow::drawBone(const Bone &bone)
{

}
void HandWindow::jointPositions(fingerJoints& joints, int finger)
{
    /// FINGER 5, JOINTS 5, 3 Point Vector
    switch (finger)
    {
    case 0:
    {

        fingerJoints finger1 =
        {
            {-0.6,165.9,182.9},
            {-0.6,165.9,182.9},
            {-35.2,172.2,118.1},
            {-61.6,178.5,75.7},
            {-81.4,184.3,48.1},
        };
        memcpy(joints,finger1,sizeof(joints));
        break;
    }

    case 1:
    {

        fingerJoints finger2 =
        {
            {21.0,192.9,177.8},
            {5.8,222.3,74.4},
            {-0.3,259.3,23.3},
            {-9.7,263.2,-10.9},
            {-18.7,253.1,-32.2},
        };
        memcpy(joints,finger2,sizeof(joints));
        break;
    }
    case 2:
    {
        fingerJoints finger3 =
        {
            {38.5,190.8,173.7},
            {37.5,214.6,73.5},
            {42.8,260.5,19.4},
            {36.5,272.5,-20.3},
            {27.1,268.7,-46.1},
        };
        memcpy(joints,finger3,sizeof(joints));
        break;
    }

    case 3:
    {
        fingerJoints finger4 =
        {
            {55.1,184.2,170.7},
            {67.4,200.8,80.6},
            {93.9,232.4,29.2},
            {101.7,237.8,-10.6},
            {99.9,231.0,-37.2},
        };
        memcpy(joints,finger4,sizeof(joints));
        break;
    }

    case 4:
    {
        fingerJoints finger5 =
        {
            {67.9,167.6,169.0},
            {91.4,181.7,87.9},
            {122.8,195.9,48.7},
            {132.8,194.9,21.6},
            {133.5,186.5,-2.3},
        };
        memcpy(joints,finger5,sizeof(joints));
        break;
    }

    }   /// switch()
}

void HandWindow::bonePositions(fingerBones& bones, int finger)
{
    /// FINGER 5, BONES 4, 3 Point Vector
    switch (finger)
    {
    case 0:
    {

        fingerBones finger1 =
        {
            {-0.6,165.9,182.9},
            {-0.6,165.9,182.9},
            {-35.2,172.2,118.1},
            {-61.6,178.5,75.7},
        };
        memcpy(bones,finger1,sizeof(bones));
        break;
    }

    case 1:
    {

        fingerBones finger2 =
        {
            {21.0,192.9,177.8},
            {5.8,222.3,74.4},
            {-0.3,259.3,23.3},
            {-9.7,263.2,-10.9},
        };
        memcpy(bones,finger2,sizeof(bones));
        break;
    }
    case 2:
    {
        fingerBones finger3 =
        {
            {38.5,190.8,173.7},
            {37.5,214.6,73.5},
            {42.8,260.5,19.4},
            {36.5,272.5,-20.3},
        };
        memcpy(bones,finger3,sizeof(bones));
        break;
    }

    case 3:
    {
        fingerBones finger4 =
        {
            {55.1,184.2,170.7},
            {67.4,200.8,80.6},
            {93.9,232.4,29.2},
            {101.7,237.8,-10.6},
        };
        memcpy(bones,finger4,sizeof(bones));
        break;
    }

    case 4:
    {
        fingerBones finger5 =
        {
            {67.9,167.6,169.0},
            {91.4,181.7,87.9},
            {122.8,195.9,48.7},
            {132.8,194.9,21.6},
        };
        memcpy(bones,finger5,sizeof(bones));
        break;
    }

    }   /// switch()
}
