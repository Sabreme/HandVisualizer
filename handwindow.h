#ifndef HANDWINDOW_H
#define HANDWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include "QVTKWidget.h"

#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "vtkSphereSource.h"
#include "vtkActor.h"

#include "Leap.h"


#define HAND_TRANSLATION (Vec3f(0.0f, -400.0f, -100.0f))

typedef std::vector<std::pair<Leap::Vector, float> > pinch_list;

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

    void drawHands();

private:
    Ui::HandWindow *ui;

    vtkRenderer                 * global_Renderer;
    vtkRenderWindow             * global_Window;
    vtkRenderWindowInteractor   * global_Interactor;

    vtkSphereSource             * global_Sphere;
    vtkActor                    * global_SphereActor;


    Controller                  * controller_;
    QTimer*                timer;

    double global_SphereRadius;
    double global_SphereCenter[3];

    /////////////////////////////////////////////////////
    void drawJoint(const Leap::Vector& joint_position);
    void drawBone(const Leap::Bone& bone);

    Leap::Vector translation_;
    float scale_;
};

#endif // HANDWINDOW_H
