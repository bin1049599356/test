
GET_FILENAME_COMPONENT(CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
PROJECT(${CURRENT_FOLDER})
SET(TARGETNAME ${CURRENT_FOLDER})
#SET(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGETNAME} INTERFACE)


SET(PROJECT_DIR D:/tjb/opensrc/install/OpenSceneGraph)

TARGET_INCLUDE_DIRECTORIES(${TARGETNAME} 
    INTERFACE ${PROJECT_DIR}/include
)

target_link_libraries(${TARGETNAME} 
    INTERFACE optimized ${PROJECT_DIR}/lib/osg162-osg.lib
   	INTERFACE optimized ${PROJECT_DIR}/lib/osg162-osgUtil.lib
  	INTERFACE optimized ${PROJECT_DIR}/lib/osg162-osgDB.lib
 	INTERFACE optimized ${PROJECT_DIR}/lib/osg162-osgViewer.lib

     INTERFACE debug ${PROJECT_DIR}/lib/osg162-osgd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osg162-osgUtild.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osg162-osgDBd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osg162-osgViewerd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osg162-osgGAd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/ot21-OpenThreadsd.lib

     INTERFACE debug ${PROJECT_DIR}/lib/osgPlugins-3.6.5/osgdb_osgd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osgPlugins-3.6.5/osgdb_rgbd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osgPlugins-3.6.5/osgdb_serializers_osgd.lib
     INTERFACE debug ${PROJECT_DIR}/lib/osgPlugins-3.6.5/osgdb_deprecated_osgd.lib
     
     INTERFACE opengl32.lib
)