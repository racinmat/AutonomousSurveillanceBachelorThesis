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
		virtual void logSelectedMap(shared_ptr<App::Map> map, int worldWidth, int worldHeight) override;
		virtual void logMapGrid(vector<vector<App::Grid>> mapGrid) override;
		virtual void logGuidingPaths(vector<shared_ptr<App::Path>> paths, shared_ptr<App::Node> start, vector<shared_ptr<App::Node>> ends) override;
		virtual void logText(string string) override;
		virtual void logNewState(shared_ptr<App::State> nearNode, shared_ptr<App::State> newNode) override;
		virtual void logRandomStates(vector<shared_ptr<App::Point>> randomStates) override;
	protected:
		QGraphicsView* view;
		QGraphicsScene* scene;
		QMainWindow* window;
		void clear();
		void drawGrid();
		QGraphicsTextItem* addText(QString text, double x, double y);
		void addCross(double x, double y, double size, Qt::GlobalColor color);
		Qt::GlobalColor getRandomColor();
		vector<Qt::GlobalColor> uavColors;
	};

}
