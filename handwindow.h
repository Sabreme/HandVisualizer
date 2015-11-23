#ifndef HANDWINDOW_H
#define HANDWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include "QVTKWidget.h"

#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "vtkSphereSource.h"
#include "vtkCylinderSource.h"
#include "vtkLineSource.h"
#include "vtkCubeSource.h"
#include "vtkActor.h"

#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>
#include <vtkPoints.h>
#include <vtkPyramid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPyramid.h>
#include <vtkSelectEnclosedPoints.h>

#include <vtkFrustumSource.h>
#include <vtkPlanes.h>
#include <vtkPlane.h>
#include <vtkTransformFilter.h>
#include <vtkTransformPolyDataFilter.h>

#include <vtkInteractorStyleJoystickActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkTextActor.h>

#include <handRenderer.h>

#include "Leap.h"


#define HAND_TRANSLATION (Vec3f(0.0f, -400.0f, -100.0f))


//typedef double fingerJoints[5][3];          /// Each finger has 5 joints and 3 pt Vector
//typedef double fingerBones[4][4];        /// Each Finger has 4 Bones and  4 pt Vector (Which includes  1 Length)
//typedef double fingerLines[4][6];          /// Each finger has 4 Lines and a 2x3  pt Vector taken from  Joints

//enum visibleHand  {rightHand, leftHand};

using namespace Leap;

namespace Ui {
class HandWindow;
}

class HandWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HandWindow(QWidget *parent = 0);
    ~HandWindow();

    void setScale(float scale)
    {
        scale_ = scale;
    }

    void setTranslation(Leap::Vector translation)
    {
        translation_ = translation;
    }


private slots:
    void on_buttonApply_clicked();

    void updateMe();

    void on_buttonStop_clicked();

private:
    Ui::HandWindow *ui;

    vtkRenderer                 * global_Renderer;
    vtkRenderWindow             * global_Window;
    vtkRenderWindowInteractor   * global_Interactor;


    Leap::Vector            joints[4];
    Leap::Vector            bones[4];
    vtkActor                    *global_Joints[2][5][5];        /// 2 Hands,  5 FINGERS / 5 Joints
    vtkLineSource        *global_Bones[2][5][4];            /// 2 Hands, 5 FINGERS / 4 Bones
    vtkActor                    *global_Bone_Actor[2][5][4];            /// 5 Fingers / 4 Bones

    HandRenderer        *handRenderer;


    Controller                  * controller_;
    QTimer*                timer;        

    /////////////////////////////////////////////////////
    //void drawJoints(visibleHand activeHand);
    void drawBones(visibleHand activeHand);  

    Leap::Vector translation_;
    float scale_;

    double fingerColourNormal [3] =  {2, 2, 2};
    double fingerColourWarning [3] =  {255, 0, 0};
    double boxBounds[6] = {-3, 3, 1, 5, -2,3};
    double jointSize = 0.15;
    double fingerSize = 20;
};

#endif // HANDWINDOW_H
