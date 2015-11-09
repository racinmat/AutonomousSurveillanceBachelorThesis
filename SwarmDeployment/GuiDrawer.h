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
		virtual void logSelectedMap(std::shared_ptr<App::Map> map, int worldWidth, int worldHeight) override;
		virtual void logMapGrid(std::vector<std::vector<App::Grid>> mapGrid) override;
		virtual void logGuidingPaths(std::vector<std::shared_ptr<App::Path>> paths, std::shared_ptr<App::Node> start, std::vector<std::shared_ptr<App::Node>> ends) override;
		virtual void logText(std::string string) override;

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
