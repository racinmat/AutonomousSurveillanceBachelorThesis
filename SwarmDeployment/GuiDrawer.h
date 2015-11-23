#pragma once
#include "LoggerInterface.h"
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QTextBrowser>

namespace App{
	class Configuration;
}

class QMainWindow;
class MainWindow;

namespace Ui
{

	class GuiDrawer : public App::LoggerInterface
	{
	public:
		GuiDrawer(QGraphicsView* view, QMainWindow* window, ::MainWindow* mainWindow, QTextBrowser* text);
		virtual ~GuiDrawer();
		virtual void logSelectedMap(shared_ptr<App::Map> map, int worldWidth, int worldHeight) override;
		virtual void logMapGrid(vector<vector<App::Grid>> mapGrid) override;
		virtual void logMapNodes(vector<shared_ptr<App::Node>> nodes) override;
		virtual void logAStarNode(shared_ptr<AStar::NodeWrapper> node) override;
		virtual void logGuidingPaths(vector<shared_ptr<App::Path>> paths, shared_ptr<App::Node> start, vector<shared_ptr<App::Node>> ends) override;
		virtual void logText(string string) override;
		virtual void logText(char const string[]) override;
		virtual void logNewState(shared_ptr<App::State> nearNode, shared_ptr<App::State> newNode) override;
		virtual void logRandomStates(unordered_map<App::Uav, shared_ptr<App::Point>, App::UavHasher> randomStates) override;
		virtual void logRandomStatesCenter(shared_ptr<App::Point> center) override;
		virtual void setConfiguration(shared_ptr<App::Configuration> configuration);

	protected:
		QGraphicsView* view;
		QGraphicsScene* scene;
		QMainWindow* window;
		::MainWindow* mainWindow;
		QTextBrowser* text;
		shared_ptr<App::Configuration> configuration;
		void clear();
		void drawGrid();
		QGraphicsTextItem* addText(QString text, double x, double y);
		void addCross(double x, double y, double size, Qt::GlobalColor color);
		Qt::GlobalColor getRandomColor();
		unordered_map<App::Uav, Qt::GlobalColor, App::UavHasher> uavColors;
		void showPopup(string text);
	};

}
