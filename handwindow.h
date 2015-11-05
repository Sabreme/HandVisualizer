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

    vtkSphereSource             * global_Sphere;
    vtkActor                    * global_SphereActor;


    Controller                  * controller_;
    QTimer*                timer;

    double global_SphereRadius;
    double global_SphereCenter[3];
};

#endif // HANDWINDOW_H
