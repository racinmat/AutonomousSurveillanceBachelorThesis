#pragma once
#include "LoggerInterface.h"
#include <QtWidgets/QGraphicsView>

class QMainWindow;

namespace Ui
{

	class GuiDrawer : public App::LoggerInterface
	{
	public:
		GuiDrawer(QGraphicsView* view, QMainWindow* window);
		virtual ~GuiDrawer();
		virtual void logSelectedMap(App::Map* map, int worldWidth, int worldHeight) override;
		virtual void logMapGrid(std::vector<std::vector<App::Grid>> mapGrid) override;
		virtual void logGuidingPaths(std::vector<App::Path*> paths, App::Node* start, std::vector<App::Node*> ends) override;

	protected:
		QGraphicsView* view;
		QGraphicsScene* scene;
		QMainWindow* window;
		void clear();
		void drawGrid();
		QGraphicsTextItem* addText(QString text, double x, double y);
		void addCross(double x, double y, double size, Qt::GlobalColor color);
		Qt::GlobalColor getRandomColor();
	};

}
