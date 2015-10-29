#include "GuiDrawer.h"
#include <QGraphicsLineItem>

namespace Ui
{

	GuiDrawer::GuiDrawer(QGraphicsScene* scene) :
		scene(scene)
	{
		LoggerInterface();
	}


	GuiDrawer::~GuiDrawer()
	{
	}

	void GuiDrawer::logSelectedMap(App::Map* map)
	{
		clear();
		drawGrid();

		for(App::Goal* goal : map->getGoals())
		{
			App::Rectangle* r = goal->rectangle;
			scene->addRect(r->getX(), r->getY(), r->getWidth(), r->getHeight(), QPen(Qt::green), QBrush(Qt::green));
		}

		for (App::Obstacle* obstacle : map->getObstacles())
		{
			App::Rectangle* r = obstacle->rectangle;
			scene->addRect(r->getX(), r->getY(), r->getWidth(), r->getHeight(), QPen(Qt::gray), QBrush(Qt::gray));
		}

	}

	void GuiDrawer::clear()
	{
		scene->addRect(0, 0, 1000, 1000, QPen(Qt::white), QBrush(Qt::white));
	}

	void GuiDrawer::drawGrid()
	{
		for (int i = 0; i <= 1000; i+= 50)
		{
			scene->addLine(i, 0, i, 1000, QPen(Qt::gray));
			scene->addText(QString("%1").arg(i), QFont())->moveBy(i, 0);
		}

	//	scene->addText("test");
	//	scene->add

		for (int i = 0; i <= 1000; i += 50)
		{
			scene->addLine(0, i, 1000, i, QPen(Qt::gray));
			scene->addText(QString("%1").arg(i), QFont())->moveBy(0, i);
		}

	}
}
