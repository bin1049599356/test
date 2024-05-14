#include <osgViewer/Viewer> 
#include <osgGA/TrackballManipulator> 
#include <osg/NodeCallback> 
#include <osg/Camera> 
#include <osg/Group> 
#include <osg/MatrixTransform> 
#include <osgDB/ReadFile> 
#include <osgUtil/IntersectVisitor>


USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(rgb)
//
//USE_SERIALIZER_WRAPPER_LIBRARY(osg)

USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_GRAPHICSWINDOW()

 
//  从NodeCallback 继承一个类，以修改MatrixTransform 对象的矩阵。 
class RotateCB : public osg::NodeCallback 
{ 
public: 
 RotateCB() : _angle( 0. ) {} 
	 virtual void operator()( osg::Node* node, osg::NodeVisitor* nv ) 
	 { 
	  // 通常应该确认一下更新访问器（update visitor）是否存在， 
	  // 不过这个例子中没有必要这样做。 
	  osg::MatrixTransform* mtLeft = 
	   dynamic_cast<osg::MatrixTransform*>( node ); 
	  osg::Matrix mR, mT; 
	  mT.makeTranslate( -6., 0., 0. ); 
	  mR.makeRotate( _angle, osg::Vec3( 0., 0., 1. ) ); 
	  mtLeft->setMatrix( mR * mT ); 
   
	  // 下一次回调时角度就会增大。 
	  _angle += 0.01; 

	  // 指定继续传递参数， 
	  // 这样OSG 可以接着执行其它带有回调的节点。 
	  traverse( node, nv ); 
	 } 
 protected: 
  double _angle; 
}; 
 
//  创建场景图形。 
//  这是一个带有两个MatrixTransform 子节点的Group 根节点， 
//  而MatrixTransform 节点又同时都是一个Geode 的父节点。 
// Geode 叶节点从模型文件cow.osg 中读取数据。 
osg::ref_ptr<osg::Node> createScene() 
{ 
	 // 加载牛的模型。 
	 osg::Node* cow = osgDB::readNodeFile( "D:\\tjb\\opensrc\\OpenSceneGraph\\assess\\OpenSceneGraph-Data-3.0.0\\cow.osg" ); 
	 // 设置数据变量为STATIC，因为程序中不会修改它。 
	 cow->setDataVariance( osg::Object::STATIC ); 
  
	 // 创建MatrixTransform 来显示左边的牛。 
	 osg::ref_ptr<osg::MatrixTransform> mtLeft = 
	 new osg::MatrixTransform; 
	 mtLeft->setName( "Left Cow\nDYNAMIC" ); 
 
	 // 设置数据变量为DYNAMIC， 
	 // 告诉OSG 这个节点将在更新遍历中被修改。 
	 mtLeft->setDataVariance( osg::Object::DYNAMIC ); 
 
	 // 设置更新回调。 
	 mtLeft->setUpdateCallback( new RotateCB ); 
	 osg::Matrix m; 
	 m.makeTranslate( -6.f, 0.f, 0.f ); 
	 mtLeft->setMatrix( m ); 
	 mtLeft->addChild( cow ); 
  
	 // 创建MatrixTransform 来显示右边的牛。 
	 osg::ref_ptr<osg::MatrixTransform> mtRight = 
	  new osg::MatrixTransform; 
	 mtRight->setName( "Right Cow\nSTATIC" ); 
  
	 // 设置数据变量为STATIC，因为程序中不会修改它。 
	 mtRight->setDataVariance( osg::Object::STATIC ); 
	 m.makeTranslate( 6.f, 0.f, 0.f ); 
	 mtRight->setMatrix( m ); 
	 mtRight->addChild( cow ); 
  
	 // 创建Group 根节点。 
	 osg::ref_ptr<osg::Group> root = new osg::Group; 
	 root->setName( "Root Node" ); 
  
	 // 设置数据变量为STATIC，因为程序中不会修改它。 
	 root->setDataVariance( osg::Object::STATIC ); 
	 root->addChild( mtLeft.get() ); 
	 root->addChild( mtRight.get() ); 
	 return root.get(); 
} 
 
int main(int, char **) 
{ 
	 // 创建视口并设置场景数据为以上代码所创建的场景图形。 
	 osgViewer::Viewer viewer; 
	 viewer.setSceneData( createScene().get() ); 
  
	 // 设置清屏颜色不同于以往的淡蓝色。 
	 viewer.getCamera()->setClearColor( osg::Vec4( 1., 1., 1., 1. ) ); 
  
	 // 渲染循环。OSG 将在更新遍历中调用RotateCB::operator()。 
	 viewer.run(); 
}