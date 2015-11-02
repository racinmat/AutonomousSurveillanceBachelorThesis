#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GuiDrawer.h"
#include "LoggerInterface.h"
#include "Configuration.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	virtual void paintEvent(QPaintEvent *e) override;
	~MainWindow() override;
	App::LoggerInterface* getLogger() const;
	void setCore(App::Core* core);
	void setConfiguration(App::Configuration* configuration);

private slots:
	void on_map_currentIndexChanged(int index);

	void on_countUav_valueChanged(int arg1);

	void on_start_clicked();

protected:
	Ui::MainWindow* ui;
	App::Configuration* configuration;
	Ui::GuiDrawer* drawer;
	bool painting;
	App::Core* core;
};

#endif // MAINWINDOW_H
