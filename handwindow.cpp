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

#include "QTimer"

#include <sstream>


HandWindow::HandWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HandWindow)
{
    ui->setupUi(this);

    this->controller_ = new Controller;    

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

     /// Create the RenderWindow
    vtkRenderer * ren1 = vtkRenderer::New();
    global_Renderer = ren1;

    vtkRenderWindow * renWin = vtkRenderWindow::New();
    global_Window = renWin;

    renWin->AddRenderer(ren1);

    ren1->SetBackground(0.1, 0.2, 0.4);

    this->ui->widget->SetRenderWindow(renWin);
    this->ui->widget->GetInteractor()->SetRenderWindow(renWin);
    global_Interactor = this->ui->widget->GetInteractor();


    vtkInteractorStyleTrackballCamera *istyle = vtkInteractorStyleTrackballCamera::New();
    global_Interactor->SetRenderWindow(renWin);
       global_Interactor->SetInteractorStyle(istyle);
    ///// WE Create a Centre Point Object for the camera


    //// THE VTK Interaction Box
    ///
    vtkSmartPointer<vtkCubeSource> centerBox =
            vtkSmartPointer<vtkCubeSource>::New();
    centerBox->SetCenter(0,0,0);
    centerBox->SetBounds(boxBounds);

    vtkSmartPointer<vtkPolyDataMapper> centerMapper =
          vtkSmartPointer<vtkPolyDataMapper>::New();
  centerMapper->SetInputConnection(centerBox->GetOutputPort());

  vtkSmartPointer<vtkActor>  boxActor =
          vtkSmartPointer<vtkActor>::New();
  boxActor->GetProperty()->SetOpacity(0.2);
   boxActor->SetMapper(centerMapper);

   global_Renderer->AddActor(boxActor);


    ///// WE INITIALISE THE HAND FIRST /////
    ////    THEN JOINTS
    ///     THEN BONES
    ///     RIGHT THEN LEFT
    ///

   handRenderer = new HandRenderer();

   handRenderer->drawJoints(rightHand,global_Renderer);
   handRenderer->drawBones(rightHand, global_Renderer);

    handRenderer->drawJoints(leftHand, global_Renderer);
    handRenderer->drawBones(leftHand, global_Renderer);

}

void HandWindow::updateMe()
{
    this->ui->widget->GetRenderWindow()->Render();

    if(controller_->isConnected())  // controller is a controller object
    {
        /// Get the most recent frame and report some basic information
        const Frame frame = controller_->frame();

        Device device = controller_->devices()[0];      

        Leap::Matrix mtxFrameTransform ;

        mtxFrameTransform.origin = Leap::Vector(0.0, 0.0, 0.0);

         Leap::Hand hand  = frame.hands()[0];                        /// Select Hand and get each finger


        if (!frame.hands().isEmpty() && !frame.hands()[0].fingers().isEmpty())
        {
            //                    Get the first hand
            const Hand rightHandMoving = frame.hands().rightmost();
            //                    Get the 2nd hand
            const Hand leftHandMoving = frame.hands().leftmost();

            bool leftHandActive = true;
            bool rightHandActive = true;

            /// If we have a single hand, we must determine which 1 it is
            /// If not the Right hand, then reverse outcome
            /// Otherwise we swap hands accordingly
            if(frame.hands().count() == 1)
            {
                if (frame.hands().frontmost().isRight())
                    leftHandActive = false;
                else
                    rightHandActive = false;
            }

            /// The Leap MOTION Interaction Box
            InteractionBox leapBox = frame.interactionBox();


            if (rightHandActive)
            {                    
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////    Right Finger Joints  TRACKING  /////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////

                Vector normalPos = leapBox.normalizePoint(rightHandMoving.palmPosition(),true);


                 bool outsideBounds = ( (normalPos.x  == 0) || (normalPos.x == 1)) ||
                                                              ( (normalPos.y  == 0)  || (normalPos.y == 1)) ||
                                                              ( (normalPos.z  == 0)  || (normalPos.z == 1)) ;


                  handRenderer->translateHand(rightHand,rightHandMoving,outsideBounds);
            }/// if(hand.isLeft())

            if (leftHandActive)
            {
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                //////////////////////////    Right Finger Joints  TRACKING  /////////////////////////////////////
                //////////////////////////////////////////////////////////////////////////////////

                Vector normalPos = leapBox.normalizePoint(leftHandMoving.palmPosition(),true);


                 bool outsideBounds = ( (normalPos.x  == 0) || (normalPos.x == 1)) ||
                                                              ( (normalPos.y  == 0)  || (normalPos.y == 1)) ||
                                                              ( (normalPos.z  == 0)  || (normalPos.z == 1)) ;


                  handRenderer->translateHand(leftHand,leftHandMoving,outsideBounds);
            }   /// if(hand.isLeft())
        }   ///    if (!frame.hands().isEmpty()
    }   ///  if(controller_->isConnected())
}

void HandWindow::on_buttonStop_clicked()
{
    timer->stop();
}


