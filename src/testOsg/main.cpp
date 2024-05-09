#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(rgb)
//
//USE_SERIALIZER_WRAPPER_LIBRARY(osg)

USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_GRAPHICSWINDOW()

int main()
{
    osgViewer::Viewer viewer;
    //osg::Camera * camera = viewer.getCamera();
    //camera->setViewport(0, 0, 640, 480);
    
    viewer.setSceneData(osgDB::readNodeFile("D:\\tjb\\opensrc\\OpenSceneGraph\\assess\\OpenSceneGraph-Data-3.0.0\\cow.osg"));
    //viewer.run();

    viewer.getCamera()->setProjectionMatrixAsPerspective(40., 1., 1., 100.);

    //  创建矩阵，指定到视点的距离。 
    osg::Matrix trans;
    trans.makeTranslate(3., 3., -12.);
    //  旋转一定角度（弧度值）。 
    double angle(0.);
    //angle = -90.0 / 180.0 * 3.14;


    osg::Vec3d upOrg = osg::Vec3f(0, 0, 1);
    osg::Vec3d dstPoint = osg::Vec3f(0, 0, 0);
    osg::Vec3d eye = osg::Vec3f(10, -13, 12);

    osg::Vec3d direct = eye - dstPoint;
    direct.normalize();
    osg::Vec3d right = upOrg ^ direct;
    right.normalize();
    osg::Vec3d up = direct ^ right;
    up.normalize();

    osg::Matrixd view0 = osg::Matrix(
        right.x(), up.x(), direct.x(), 0,
        right.y(), up.y(), direct.y(), 0,
        right.z(), up.z(), direct.z(), 0,
       0, 0, 0, 1
    );

    view0 = osg::Matrix().translate(-eye) * view0;


    while (!viewer.done()) 
    {
        // 创建旋转矩阵。 
        osg::Matrix rot;
        rot.makeRotate(angle, osg::Vec3(0., 1., 0.));
        angle += 0.1;

        // 设置视口矩阵（旋转矩阵和平移矩阵连乘）。 
        osg::Matrixd view = rot * trans;
        //viewer.getCamera()->setViewMatrix(rot * trans);
        view = view0;
        viewer.getCamera()->setViewMatrix(view);
        //viewer.getCamera()->setViewMatrixAsLookAt(eye, dstPoint, upOrg);

        // 绘制下一帧 
        viewer.frame();
    }
}