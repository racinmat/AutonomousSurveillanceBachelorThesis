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
	shared_ptr<App::LoggerInterface> getLogger() const;
	void setCore(shared_ptr<App::Core> core);
	void setConfiguration(shared_ptr<App::Configuration> configuration);

private slots:
	void on_map_currentIndexChanged(int index);

	void on_countUav_valueChanged(int arg1);

	void on_start_clicked();

protected:
	Ui::MainWindow* ui;
	shared_ptr<App::Configuration> configuration;
	shared_ptr<Ui::GuiDrawer> drawer;
	bool painting;
	shared_ptr<App::Core> core;
};

#endif // MAINWINDOW_H
