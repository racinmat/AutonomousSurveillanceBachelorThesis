#include "GuiDrawer.h"
#include <QGraphicsLineItem>
#include <QGraphicsWidget>

namespace Ui
{

	GuiDrawer::GuiDrawer(QGraphicsView* view) :
		view(view), 
		scene(view->scene())
	{
		LoggerInterface();
	}


	GuiDrawer::~GuiDrawer()
	{
	}

	void GuiDrawer::logSelectedMap(App::Map* map, int worldWidth, int worldHeight)
	{
		view->resize(worldWidth, worldHeight);

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
		double height = view->height();
		double width = view->width();

		scene->addRect(0, 0, width, height, QPen(Qt::white), QBrush(Qt::white));
	}

	void GuiDrawer::drawGrid()
	{
		double height = view->height();
		double width = view->width();

		for (int i = 0; i <= height; i+= 50)
		{
			scene->addLine(i, 0, i, height, QPen(Qt::gray));
			scene->addText(QString("%1").arg(i), QFont())->moveBy(i, 0);
		}

		for (int i = 0; i <= width; i += 50)
		{
			scene->addLine(0, i, width, i, QPen(Qt::gray));
			scene->addText(QString("%1").arg(i), QFont())->moveBy(0, i);
		}

	}
}
