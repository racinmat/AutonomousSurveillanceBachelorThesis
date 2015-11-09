#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GuiDrawer.h"
#include "LoggerInterface.h"
#include "Configuration.h"
#include <memory>

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
	void setCore(std::shared_ptr<App::Core>& core);
	void setConfiguration(std::shared_ptr<App::Configuration>& configuration);

private slots:
	void on_map_currentIndexChanged(int index);

	void on_countUav_valueChanged(int arg1);

	void on_start_clicked();

protected:
	Ui::MainWindow* ui;
	std::shared_ptr<App::Configuration> configuration;
	Ui::GuiDrawer* drawer;
	bool painting;
	std::shared_ptr<App::Core> core;
};

#endif // MAINWINDOW_H
