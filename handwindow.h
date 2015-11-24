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

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkTextActor.h>

#include <handRenderer.h>

#include "Leap.h"


#define HAND_TRANSLATION (Vec3f(0.0f, -400.0f, -100.0f))


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


private slots:
    void on_buttonApply_clicked();

    void updateMe();

    void on_buttonStop_clicked();

private:
    Ui::HandWindow *ui;

    vtkRenderer                 * global_Renderer;
    vtkRenderWindow             * global_Window;
    vtkRenderWindowInteractor   * global_Interactor;


    HandRenderer        *handRenderer;


    Controller                  * controller_;
    QTimer*                timer;        

    /////////////////////////////////////////////////////


    double boxBounds[6] = {-3, 3, 1, 5, -2,3};
};

#endif // HANDWINDOW_H
