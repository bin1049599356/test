#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgDB/ReadFile>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/IntersectVisitor>
#include <osg/ComputeBoundsVisitor>

#include <osg/MatrixTransform>
#include <osg/Group>
#include <osg/Node>

USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(rgb)
//
//USE_SERIALIZER_WRAPPER_LIBRARY(osg)

USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_GRAPHICSWINDOW()


void GetAllLineSegment(osg::Geometry* pGeometry, osgUtil::IntersectVisitor* pIv, osg::Matrixd& matParent)
{
	if (pGeometry == NULL)
	{
		return;
	}

	osg::ref_ptr<osg::Vec3Array> vertices = dynamic_cast<osg::Vec3Array*>(pGeometry->getVertexArray());
	if (vertices.valid())
	{
		unsigned int uPriNum = pGeometry->getNumPrimitiveSets();
		for (unsigned int i = 0; i < uPriNum; i++)
		{
			const osg::PrimitiveSet* pPriSet = pGeometry->getPrimitiveSet(i);

			for (unsigned int j = 0; j < pPriSet->getNumIndices() - 1; j++)
			{
				unsigned int iIndex1 = pPriSet->index(j);
				unsigned int iIndex2 = pPriSet->index(j + 1);
				osg::ref_ptr<osg::LineSegment> ls = new osg::LineSegment();
				ls->set(vertices->at(iIndex1) * matParent, vertices->at(iIndex2) * matParent);
				pIv->addLineSegment(ls);
			}
		}
	}
}
void GetAllLineSegment(osg::Node* pNode, osgUtil::IntersectVisitor* pIv, osg::Matrixd& matParent)
{
	osg::Geode* pGeode = dynamic_cast<osg::Geode*>(pNode);
	if (pGeode)
	{
		unsigned int iDrawNum = pGeode->getNumDrawables();
		for (unsigned int i = 0; i < iDrawNum; i++)
		{
			osg::Drawable* pDrawable = pGeode->getDrawable(i);
			GetAllLineSegment(pDrawable->asGeometry(), pIv, matParent);
		}
	}
	else
	{
		osg::MatrixTransform* pMatrix = dynamic_cast<osg::MatrixTransform*>(pNode);
		if (pMatrix)
		{
			osg::Matrixd mat = pMatrix->getMatrix();
			osg::Matrixd matNow = mat * matParent;
			unsigned int uNum = pMatrix->getNumChildren();
			for (unsigned int i = 0; i < uNum; i++)
			{
				osg::Node* pChild = pMatrix->getChild(i);
				GetAllLineSegment(pChild, pIv, matNow);
			}
		}
		else
		{
			osg::Group* pGroup = dynamic_cast<osg::Group*>(pNode);
			if (pGroup)
			{
				unsigned int uNum = pGroup->getNumChildren();
				for (unsigned int i = 0; i < uNum; i++)
				{
					osg::Node* pChild = pGroup->getChild(i);
					GetAllLineSegment(pChild, pIv, matParent);
				}
			}
		}
	}
}

void HitDetection(osg::Node* root)
{
	osg::Group* m_root = dynamic_cast<osg::Group*>(root);
	if (m_root == nullptr)
		return;

	osg::Matrix rootMatrix = osg::Matrix::identity();
	osg::MatrixTransform* pMatrix = dynamic_cast<osg::MatrixTransform*>(root);
	if (pMatrix)
		rootMatrix = pMatrix->getMatrix();

	unsigned int uNum = m_root->getNumChildren();
	unsigned int i = 0;
	for (i = 0; i < uNum - 1; i++)
	{
		osg::Node* pNode1 = m_root->getChild(i);
		if (pNode1)
		{
			// 获取包围盒
			osg::ComputeBoundsVisitor boundvisitor;
			pNode1->accept(boundvisitor);
			osg::BoundingBox& bb1 = boundvisitor.getBoundingBox();
			osg::ref_ptr<osgUtil::IntersectVisitor> iv = new osgUtil::IntersectVisitor();
			GetAllLineSegment(pNode1, iv, rootMatrix);
			//iv->addLineSegment(new osg::LineSegment(osg::Vec3(50.0,20.0, 0.0), osg::Vec3(50, 50,50)));
			unsigned int j = i + 1;
			for (; j < uNum; j++)
			{
				osg::Node* pNode2 = m_root->getChild(j);
				if (pNode2)
				{
					osg::ComputeBoundsVisitor boundvisitor2;
					pNode2->accept(boundvisitor2);
					osg::BoundingBox& bb2 = boundvisitor2.getBoundingBox();
					if (bb1.intersects(bb2))
					{
						// 包围盒有相交
						// 判断两个第一个节点的所有线是否和第二个节点有相交
						pNode2->accept(*iv);
						if (iv->hits())
						{
							//  有相交，下面的代码是获取相交点
							osgUtil::IntersectVisitor::LineSegmentHitListMap& mapList = iv->getSegHitList();
							osgUtil::IntersectVisitor::LineSegmentHitListMap::iterator mapIter = mapList.begin();
							for (mapIter = mapList.begin(); mapIter != mapList.end(); mapIter++)
							{
								osgUtil::IntersectVisitor::HitList hitList = mapIter->second;
								osgUtil::IntersectVisitor::HitList::iterator iter = hitList.begin();
								for (; iter != hitList.end(); iter++)
								{
									osg::Vec3 ptInter = iter->getWorldIntersectPoint();
									int cc = 0;
								}
							}
						}
						else
						{
							// 没有相交，用第二个节点的线段与第一个节点进行检查
							osg::ref_ptr<osgUtil::IntersectVisitor> iv2 = new osgUtil::IntersectVisitor();
							GetAllLineSegment(pNode2, iv2, rootMatrix);
							pNode1->accept(*iv2);
							if (iv2->hits())
							{
								// 有相交
							}
						}

					}
				}
			}
		}
	}
}

int main()
{
	 osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("D:\\tjb\\opensrc\\OpenSceneGraph\\assess\\OpenSceneGraph-Data-3.0.0\\cow.osg");//第一个节点
	 osg::ref_ptr<osg::Node> axes = osgDB::readNodeFile("D:\\tjb\\opensrc\\OpenSceneGraph\\assess\\OpenSceneGraph-Data-3.0.0\\cow.osg");//第二个节点

	 osg::ref_ptr<osg::MatrixTransform> mt_cow = new osg::MatrixTransform;
	 mt_cow->setMatrix(osg::Matrix::scale(osg::Vec3(0.5,0.5,0.5))*osg::Matrix::translate(osg::Vec3(-10,0,0)));
	 mt_cow->addChild(cow.get());

	 osg::ref_ptr<osg::MatrixTransform> mt_axes = new osg::MatrixTransform;
	 mt_axes->addChild(axes.get());

	 osg::ref_ptr<osg::Group> root = new osg::Group;
	 root->addChild(mt_cow.get());
	 root->addChild(mt_axes.get());

	 osgViewer::Viewer * viewer = new osgViewer::Viewer;
	 viewer->addEventHandler(new osgGA::StateSetManipulator(viewer->getCamera()->getOrCreateStateSet()));
	 /************************************************************************/
	 /* 碰撞检测                                                             */
	 /************************************************************************/
	 osg::Vec3 p_start(-100,0,0);
	 osg::Vec3 p_end(100,0,0);
	 osgUtil::LineSegmentIntersector::Intersections _intersections;
	 osg::ref_ptr< osgUtil::LineSegmentIntersector > _lineSegmentIntersector = new osgUtil::LineSegmentIntersector(p_start,p_end);
	 osgUtil::IntersectionVisitor _iv(_lineSegmentIntersector.get());
	 cow->accept(_iv);
	 _intersections=_lineSegmentIntersector->getIntersections();
	 int _intersectionNumber=_intersections.size();       
	 if (_intersectionNumber!=0)
	 {        
	  osgUtil::LineSegmentIntersector::Intersections::iterator hitr = _intersections.begin();
	  osg::Vec3 vec3_p1 =hitr->getWorldIntersectPoint()*mt_cow->getMatrix();//这样得到的交点就是绝对的世界坐标了。
	  mt_axes->setMatrix(osg::Matrix::translate(vec3_p1));//把坐标系原点移动到交点，可以测试现在的坐标系原点是不是刚好和牛相交！
	  hitr++;
	  osg::Vec3 vec3_p2 =hitr->getWorldIntersectPoint()*mt_cow->getMatrix();
	 }
	 viewer->setSceneData(root.get());

	 return viewer->run();
 
}