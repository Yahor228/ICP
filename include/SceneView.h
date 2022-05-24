/**
 * @file SceneView.h
 * @author Yahor Senichak (xsenic00)
 * @brief declaration a scene view class
 */


#pragma once
#include <QGraphicsView>

class SceneView:public QGraphicsView
{
public:
	SceneView(QGraphicsScene * scene);
};