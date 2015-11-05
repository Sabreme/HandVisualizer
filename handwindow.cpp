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
    vtkSmartPointer<vtkPolyData> inputPolyData ;

    vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetPhiResolution(15);
    sphereSource->SetThetaResolution(15);
    sphereSource->Update();
    inputPolyData = sphereSource->GetOutput();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInput(inputPolyData);

    vtkSmartPointer<vtkActor> sphereActor =
            vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(mapper);
    sphereActor->GetProperty()->SetRepresentationToWireframe();
    sphereActor->GetProperty()->SetColor(0.8900, 0.8100, 0.3400 );

    // Create the RenderWindow
    vtkRenderer * ren1 = vtkRenderer::New();
    global_Renderer = ren1;

    vtkRenderWindow * renWin = vtkRenderWindow::New();
    global_Window = renWin;

    renWin->AddRenderer(ren1);

    ren1->AddActor(sphereActor);

    ren1->SetBackground(0.1, 0.2, 0.4);

    this->ui->widget->SetRenderWindow(renWin);
    this->ui->widget->GetInteractor()->SetRenderWindow(renWin);
    global_Interactor = this->ui->widget->GetInteractor();

    sphereSource->GetCenter(global_SphereCenter);
    global_SphereRadius =  sphereSource->GetRadius();


    global_Sphere = sphereSource;
    global_SphereActor = sphereActor;
    sphereActor->RotateY(-90);

    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();
    global_Interactor->MouseWheelBackwardEvent();


//    vtkLeapScaleActor * leapScaleActor =
//            new vtkLeapScaleActor();


//    //leapScaleActor->BuildRepresentation(global_Renderer->GetViewport());
//    leapScaleActor->BuildRepresentation(global_Renderer);
//    global_LeapLegend = leapScaleActor;
    //    //leapScaleActor->PrintSelf(std::cout);
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

        //        /// Get the most recent frame and report some basic information
                std::cout   << "Frame id: " << frame.id()
                            << ", timestamp: " << frame.timestamp()
                            << ", hands: " << frame.hands().count()
                            << ", fingers: " << frame.fingers().count()
                            << ", Rotation (Y): " << shouldRotate
                            << ", Translate (Y): " << shouldTranslate
                            << ", Scale (Y): " << shouldScale
                            //<< ", tools: " << frame.tools().count()
                            //<< ", gestures: " << frame.gestures().count()
                            //<< ", PalmNormal: " << frame.hands()[0].palmNormal()
                            //<< ", PalmDirect: " << frame.hands()[0].direction()
                            << ", yaw: " << frame.hands()[0].direction().yaw()
                            //<< ", Angle: " << frame.rotationAngle(controller_->frame(1))
                            << std::endl;

//        std::cout << "TRACKING: " << frame.id() << endl;

        if (!frame.hands().isEmpty() && !frame.hands()[0].fingers().isEmpty())
        {
            //                    Get the first hand
            const Hand hand = frame.hands().rightmost();
            //                    Get the 2nd hand
            const Hand hand2 = frame.hands().leftmost();



//            if(frame.hands().count() ==1)
//            {
//                ////////////////////////////////////////////////////////////////////////////////////////////////////
//                //////////////////////////    FingerTip Callibration TRACKING  /////////////////////////////////////
//                //////////////////////////////////////////////////////////////////////////////////

//                Vector handPos = hand.palmPosition();

//                double sensitivity = 0.01;
//                double offSetX = 0;
//                double offSetY = 2.5;
//                double offSetZ = 1;

//                double handPosPoint[3] = {handPos.x * sensitivity - offSetX,
//                                          handPos.y * sensitivity - offSetY,
//                                          handPos.z * sensitivity - offSetZ
//                                         };

//                pointWidget->SetPosition(handPosPoint);//handPos.
//                pointWidget->GetProperty()->SetColor(0.0, 1.0, 0.0);

//            }
            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////    Hand TRACKING  /////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////

//            if((frame.hands().count() ==1)       &&
//                    (frame.fingers().count() >= 4))
//            {
//                Vector newNormal = hand.palmNormal();

//                double oldNormal[3] ;
//                global_PlaneWidget->GetNormal(oldNormal);
//                double newNormalD [3];
//                double theta, rotVector[3];

//                double *point1 = global_PlaneWidget->GetPoint1();
//                double *origin = global_PlaneWidget->GetOrigin();
//                double *center = global_PlaneWidget->GetCenter();


//                newNormalD[0] = newNormal.x;
//                newNormalD[1] = newNormal.y;
//                newNormalD[2] = newNormal.z;

//                global_PlaneWidget->SetNormal(newNormal.x, newNormal.y, newNormal.z);
//                global_PlaneWidget->UpdatePlacement();


//                ///Compute the rotation vector using a transformation matrix
//                ///Note that is fnromals are aparelle then te rotation is either 0 or 180 Degrees

//                double dp = vtkMath::Dot(oldNormal, newNormalD);
//                if (dp >= 1.0)
//                {
//                    return;    ///zero rotation
//                }

//                else if (dp <= -1.0)
//                {
//                    theta = 180.0;
//                    rotVector[0] = point1[0] - origin[0];
//                    rotVector[1] = point1[1] - origin[1];
//                    rotVector[2] = point1[2] - origin[2];
//                }
//                else
//                {
//                    vtkMath::Cross(oldNormal, newNormalD,rotVector);
//                    theta = vtkMath::DegreesFromRadians(acos(dp));
//                }

//                global_ArrowActor->RotateWXYZ(theta, rotVector[0], rotVector[1], rotVector[2]);
//                global_SphereActor->RotateWXYZ(theta, rotVector[0], rotVector[1], rotVector[2]);
//                global_SphereActor->GetProperty()->SetColor(0.0, 1.0, 0.0);

//            //    global_PlaneWidget->Print(std::cout);

//                /// Compute the Scale Factor using the leap motion factor

//                /// The following code checks to see if the sensor has regained focus.
//                /// if so, we set the global_CameraPosition to the default value
//                /// Effectively functioning as a reset value.
//                /// We also have a skip value to true to not invert the slider.

//                bool do_Invert = true;
//                if (abs(controller_->frame(1).id() - global_ScaleFactorID) > 15 )
//                {
//                    global_CameraPosition = static_cast<vtkSliderRepresentation3D*>(global_Slider->GetRepresentation())->GetValue();
////                    std::cout << "Return focus" << endl;
//                    do_Invert = false;
//                }

//                global_ScaleFactorID = frame.id();       //Current Frame

//                float scaleFactor = frame.hands()[0].scaleFactor(controller_->frame(2));

//                double oldPosition = global_CameraPosition;

//                global_CameraPosition = oldPosition / (scaleFactor) ;

//                double newPosition = global_CameraPosition;

//                /// We add color chromatic scale to the Slider Widget Propoert to highligh strength

//                double colourRange = (newPosition /  scaling_Max) ;

//                if (colourRange < 0) colourRange = 0;
//                else
//                if(colourRange > 1) colourRange = 1;

//                if (scaleFactor > 1.0000001)            /// EXPANDING .... ColourRange Getting SMALLER - Blue Adjustment
//                {
//                     static_cast<vtkSliderRepresentation3D*>(global_Slider->GetRepresentation())->SetValue(newPosition );
//                    static_cast<vtkSliderRepresentation3D*>(global_Slider->GetRepresentation())->GetTubeProperty()->SetColor(colourRange,colourRange,1);
//                }
//                else                                           /// SCHINKING.... ColourRange Getting BIGGER -- Red Adjustment
//                {
//                    static_cast<vtkSliderRepresentation3D*>(global_Slider->GetRepresentation())->SetValue(newPosition );
//                    static_cast<vtkSliderRepresentation3D*>(global_Slider->GetRepresentation())->GetTubeProperty()->SetColor(1,1-colourRange,1-colourRange);
//                }

////                 std::cout  << "scaleFactor: " << scaleFactor
////                              << "\t, oldPosition: "  << oldPosition
////                              << "\t, newPosition: "  << newPosition
////                              << "\t, colourRange: "  << colourRange
////                              << "\t, CameraPos: "  << global_CameraPosition
////                             <<  "\t, FrameID: "  << frame.id()
////                              << "\t" << endl;


//            } /// Hand tracking


            //////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////    FINGER TRACKING  ///////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////

//            if (this->ui->checkBoxFingers->isChecked() || this->ui->checkBoxFingerTips->isChecked())
//            {
//                ///Set default finger values
//                ///
//                for (int t =0; t < 5; t++)
//                {
//                    global_FingerTip[t]->VisibilityOff();
//                }


//                if((frame.hands().count() ==1)       &&
//                        (frame.fingers().count() >= 1 && frame.fingers().count() < 6))
//                {

//                    const Leap::FingerList fingers = frame.fingers();
//                    double handDropPos = 1;

//                    for (Leap::FingerList::const_iterator f1 = fingers.begin(); f1 != fingers.end(); f1++)
//                    {
//                        Leap::Bone bone;
//                        Leap::Bone::Type boneType;
//                        for(int b = 0; b < 4; b++)
//                        {
//                            boneType = static_cast<Leap::Bone::Type>(b);
//                            bone = (*f1).bone(boneType);
//                            std::cout << "Finger index: " << (*f1).type() << " " << bone <<  "x1" << std::endl;
//                        }
//                    };

//                    for(int i =0; i < fingers.count(); i++)
//                    {
//                        const Leap::Finger finger = fingers[i];
////                        Leap::Vector vStartPos = mtxFrameTransform.transformPoint(finger.stabilizedTipPosition() * frameScale);
////                        Leap::Vector vEndPos = mtxFrameTransform.transformDirection(finger.direction()) * 30;

//                        Leap::Vector vStartPos = mtxFrameTransform.transformPoint(finger.stabilizedTipPosition() * frameScale);
//                        Leap::Vector vEndPos = mtxFrameTransform.transformDirection(finger.direction()) * 30;

//                        //Leap::Vector v

//                        if (this->ui->checkBoxFingerTips->isChecked())
//                        {
//                            //global_FingerTip[i]->SetPosition(vStartPos.x, vStartPos.y -handDropPos, vStartPos.z);
//                            global_FingerTip[i]->SetPosition(vStartPos.x, vStartPos.y, vStartPos.z);
//                            global_FingerTip[i]->GetProperty()->SetColor(global_FingerClr[i]);
//                            global_FingerTip[i]->VisibilityOn();
//                        }

//                        double * output;

//                        /// Optional Lines (Very confusing)
//                        if (this->ui->checkBoxFingers->isChecked())
//                        {
////                            double startPoint [3] = {vStartPos.x, vStartPos.y -handDropPos, vStartPos.z} ;
////                            double endPoint [3] = {vEndPos.x, vEndPos.y -handDropPos, vEndPos.z};
//                            double startPoint [3] = {vStartPos.x, vStartPos.y, vStartPos.z} ;
//                            double endPoint [3] = {vEndPos.x, vEndPos.y, vEndPos.z};
//                            double radius = 25;

//                          //  std::cout  << "endPoint A: " << endPoint[0] << ","  << endPoint[1] << ","  << endPoint[2] << "\t" << endl;


//                            //// SHORT LINE FUNCTION C++ Too Silly for Simple Array Return Function
//                            double dx = endPoint[0] - startPoint[0];
//                            double dy = endPoint[1] - startPoint[1];
//                            double dz = endPoint[2] - startPoint[2];

//                         //   std::cout << "A dx = " << dx << "A dy = " << dy << "A dz = " << dz << endl;

//                            double length = std::sqrt(dx * dx + dy * dy + dz * dz);

//                            if (length > 0)
//                            {
//                                dx /=length;
//                                dy /=length;
//                                dz /=length;
//                            }


//                            dx *= length - radius;
//                            dy *= length - radius;
//                            dz *= length - radius;

//                          //  std::cout << "B dx = " << dx << "B dy = " << dy << "B dz = " << dz << endl;


//                            double newDistance[3] = {
//                                                     startPoint[0] + dx,
//                                                     startPoint[1] + dy,
//                                                     startPoint[2] + dz
//                                                    };

//                          //  std::cout  << "endPointB: " << newDistance[0] << ","  << newDistance[1] << ","  << newDistance[2] << "\t" << endl;

//                            //global_Fingers[i]->SetPoint1(vStartPos.x, vStartPos.y, vStartPos.z);
//                            global_Fingers[i]->SetPoint1(startPoint);
//                            global_Fingers[i]->GetLineProperty()->SetColor(global_FingerClr[i]);
//                            //global_Fingers[i]->SetPoint2(vEndPos.x, vEndPos.y , vEndPos.z);
//                            global_Fingers[i]->SetPoint2(newDistance);

//                            global_Fingers[i]->On();

//                            output = newDistance;
//                        }



//                        /// NEW BONES DISPLAY Lines (SHOULD BE Very NON CONFUSING confusing)
//                        if (this->ui->checkBoxBones->isChecked())
//                        {

////                            const Leap::Finger finger = fingers[i];
//////                            double startPoint [3] = {vStartPos.x, vStartPos.y -handDropPos, vStartPos.z} ;
//////                            double endPoint [3] = {vEndPos.x, vEndPos.y -handDropPos, vEndPos.z};

////                            Leap::Bone bone;
////                            Leap::Bone::Type boneType;

////                            for (int b = 0; b <  4 ; b++)
////                            {
////                                    boneType = static_cast<Leap::Bone::Type>(b);
////                                    bone = finger.bone(boneType);
////                                global_Bones[i][b]->SetPoint1
////                                        (
////                                            bone.prevJoint().x,
////                                            bone.prevJoint().y,
////                                            bone.prevJoint().z
////                                            );

////                                global_Bones[i][b]->SetPoint2
////                                        (
////                                            bone.nextJoint().x,
////                                            bone.nextJoint().y,
////                                            bone.nextJoint().z
////                                            );

////                               global_Bones[i][b]->GetLineProperty()->SetColor(global_FingerClr[i]);
////                                //global_Fingers[i]->SetPoint2(vEndPos.x, vEndPos.y , vEndPos.z);


////                                global_Bones[i][b]->On();

////                            }
//////                            double startPoint [3] = {vStartPos.x, vStartPos.y, vStartPos.z} ;
//////                            double endPoint [3] = {vEndPos.x, vEndPos.y, vEndPos.z};
//////                            double radius = 25;

//////                          //  std::cout  << "endPoint A: " << endPoint[0] << ","  << endPoint[1] << ","  << endPoint[2] << "\t" << endl;


//////                            //// SHORT LINE FUNCTION C++ Too Silly for Simple Array Return Function
//////                            /// Basically generates a line between 2 vectors which are 3D points
//////                            double dx = endPoint[0] - startPoint[0];
//////                            double dy = endPoint[1] - startPoint[1];
//////                            double dz = endPoint[2] - startPoint[2];

//////                         //   std::cout << "A dx = " << dx << "A dy = " << dy << "A dz = " << dz << endl;

//////                            double length = std::sqrt(dx * dx + dy * dy + dz * dz);

//////                            if (length > 0)
//////                            {
//////                                dx /=length;
//////                                dy /=length;
//////                                dz /=length;
//////                            }


//////                            dx *= length - radius;
//////                            dy *= length - radius;
//////                            dz *= length - radius;

//////                          //  std::cout << "B dx = " << dx << "B dy = " << dy << "B dz = " << dz << endl;


//////                            double newDistance[3] = {
//////                                                     startPoint[0] + dx,
//////                                                     startPoint[1] + dy,
//////                                                     startPoint[2] + dz
//////                                                    };

////                          //  std::cout  << "endPointB: " << newDistance[0] << ","  << newDistance[1] << ","  << newDistance[2] << "\t" << endl;

////                            //global_Fingers[i]->SetPoint1(vStartPos.x, vStartPos.y, vStartPos.z);
////                            ///global_Fingers[i]->SetPoint1(startPoint);


//                        }
//                    }
//                }
//            } /// Finger Tracking
        }
    }
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
