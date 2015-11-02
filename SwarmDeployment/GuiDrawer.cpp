#include "GuiDrawer.h"
#include <QGraphicsLineItem>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <QtCore/qtextstream.h>

namespace Ui
{

	GuiDrawer::GuiDrawer(QGraphicsView* view, QMainWindow* window) :
		view(view), 
		scene(view->scene()),
		window(window)
	{
		LoggerInterface();
	}


	GuiDrawer::~GuiDrawer()
	{
	}

	void GuiDrawer::logSelectedMap(App::Map* map, int worldWidth, int worldHeight)
	{
		QTextStream cout(stdout);
		cout << "logging selected map" << endl;
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

	void GuiDrawer::logMapGrid(std::vector<std::vector<App::Grid>> mapGrid)
	{
		QTextStream cout(stdout);
		cout << "logging map grid" << endl;
		int x = 5;
		for (auto row : mapGrid)
		{
			int y = 35;
			for (auto grid : row)
			{
				std::string gridText;
				switch (grid)
				{
					case App::Grid::Obstacle: gridText = "obstacle"; break;
					case App::Grid::Free: gridText = "free"; break;
					case App::Grid::Goal: gridText = "goal"; break;
					case App::Grid::UAV: gridText = "uav"; break;
				}
				auto text = scene->addText(QString::fromStdString(gridText));
				text->moveBy(x, y);
				text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
				y += 50;
			}
			x += 50;
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
			auto text = scene->addText(QString("%1").arg(i));
			text->moveBy(i, 0);
			text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
		}

		for (int i = 0; i <= width; i += 50)
		{
			scene->addLine(0, i, width, i, QPen(Qt::gray));
			auto text = scene->addText(QString("%1").arg(i));
			text->moveBy(0, i);
			text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
		}

	}
}
