#include "GuiDrawer.h"
#include <QGraphicsLineItem>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <QtCore/qtextstream.h>
#include <QtWidgets/QMessageBox>

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

	void GuiDrawer::logSelectedMap(std::shared_ptr<App::Map> map, int worldWidth, int worldHeight)
	{
		QTextStream cout(stdout);
		cout << "logging selected map" << endl;
		clear();

		view->resize(worldWidth, worldHeight);

		for(auto goal : map->getGoals())
		{
			auto r = goal->rectangle;
			scene->addRect(r->getX(), r->getY(), r->getWidth(), r->getHeight(), QPen(Qt::green), QBrush(Qt::green));
		}

		for (auto obstacle : map->getObstacles())
		{
			auto r = obstacle->rectangle;
			scene->addRect(r->getX(), r->getY(), r->getWidth(), r->getHeight(), QPen(Qt::gray), QBrush(Qt::gray));
		}

		for (auto start : map->getUavsStart())
		{
			auto p = start->getLocation();
			addCross(p->getX(), p->getY(), 3, getRandomColor());
		}

		drawGrid();
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
				addText(QString::fromStdString(gridText), x, y);
				y += 50;
			}
			x += 50;
		}
	}

	void GuiDrawer::logGuidingPaths(std::vector<std::shared_ptr<App::Path>> paths, std::shared_ptr<App::Node> start, std::vector<std::shared_ptr<App::Node>> ends)
	{
		scene->addRect(start->getPoint()->getX() - 25, start->getPoint()->getY() - 25, 50, 50, QPen(Qt::yellow), QBrush(Qt::yellow));
		for(auto end : ends)
		{
			scene->addRect(start->getPoint()->getX() - 25, start->getPoint()->getY() - 25, 50, 50, QPen(Qt::darkCyan), QBrush(Qt::darkCyan));
		}

		for (auto path : paths)
		{
			std::shared_ptr<App::Node> previous = nullptr;
			for (auto node : path->getNodes())
			{
				if (previous != nullptr)
				{
					scene->addLine(previous->getPoint()->getX(), previous->getPoint()->getY(), node->getPoint()->getX(), node->getPoint()->getY(), QPen(Qt::blue));
				}
				previous = node;
			}
		}
	}

	void GuiDrawer::logText(std::string string)
	{
		QMessageBox::information(
			window,
			"Logged text",
			QString::fromStdString(string));
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
			addText(QString("%1").arg(i), i, 0);
		}

		for (int i = 0; i <= width; i += 50)
		{
			scene->addLine(0, i, width, i, QPen(Qt::gray));
			addText(QString("%1").arg(i), 0, i);
		}

	}

	QGraphicsTextItem* GuiDrawer::addText(QString text, double x, double y)
	{
		auto textItem = scene->addText(text);
		textItem->moveBy(x, y);
		textItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
		return textItem;
	}

	void GuiDrawer::addCross(double x, double y, double size, Qt::GlobalColor color)
	{
		scene->addLine(x - size, y - size, x + size, y + size, QPen(color));
		scene->addLine(x - size, y + size, x + size, y - size, QPen(color));
	}

	Qt::GlobalColor GuiDrawer::getRandomColor()
	{
		return Qt::GlobalColor(rand() % Qt::transparent);	//tansparent is last color of enum
	}
}
