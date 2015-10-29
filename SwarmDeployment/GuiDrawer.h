#pragma once
#include "LoggerInterface.h"
#include <QtWidgets/QGraphicsView>

namespace Ui
{

	class GuiDrawer : public App::LoggerInterface
	{
	public:
		GuiDrawer(QGraphicsScene* scene);
		~GuiDrawer();
		virtual void logSelectedMap(App::Map* map) override;

	protected:
		QGraphicsScene* scene;
		void clear();
		void drawGrid();
	};

}
