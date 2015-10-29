#pragma once
#include "LoggerInterface.h"
#include <QtWidgets/QGraphicsView>

namespace Ui
{

	class GuiDrawer : public App::LoggerInterface
	{
	public:
		GuiDrawer(QGraphicsView* view);
		~GuiDrawer();
		virtual void logSelectedMap(App::Map* map, int worldWidth, int worldHeight) override;

	protected:
		QGraphicsView* view;
		QGraphicsScene* scene;
		void clear();
		void drawGrid();
	};

}
