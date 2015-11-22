#pragma once
#include "LoggerInterface.h"
#include <QtWidgets/QGraphicsView>

class QMainWindow;
class MainWindow;

namespace Ui
{

	class GuiDrawer : public App::LoggerInterface
	{
	public:
		GuiDrawer(QGraphicsView* view, QMainWindow* window, ::MainWindow* mainWindow);
		virtual ~GuiDrawer();
		virtual void logSelectedMap(shared_ptr<App::Map> map, int worldWidth, int worldHeight) override;
		virtual void logMapGrid(vector<vector<App::Grid>> mapGrid) override;
		virtual void logGuidingPaths(vector<shared_ptr<App::Path>> paths, shared_ptr<App::Node> start, vector<shared_ptr<App::Node>> ends) override;
		virtual void logText(string string) override;
		virtual void logNewState(shared_ptr<App::State> nearNode, shared_ptr<App::State> newNode) override;
		virtual void logRandomStates(unordered_map<App::Uav, shared_ptr<App::Point>, App::UavHasher> randomStates) override;
		virtual void logRandomStatesCenter(shared_ptr<App::Point> center) override;
	protected:
		QGraphicsView* view;
		QGraphicsScene* scene;
		QMainWindow* window;
		MainWindow* mainWindow;
		void clear();
		void drawGrid();
		QGraphicsTextItem* addText(QString text, double x, double y);
		void addCross(double x, double y, double size, Qt::GlobalColor color);
		Qt::GlobalColor getRandomColor();
		unordered_map<App::Uav, Qt::GlobalColor, App::UavHasher> uavColors;
		void showPopup(string text);
	};

}
