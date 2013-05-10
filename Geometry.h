#pragma once
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include "Utility.h"
class Geometry : public QWidget
{
	Q_OBJECT
public:
	enum{ANNOTATE, VALIDATE};
	Geometry(void);
	~Geometry(void);
	void annotate(const std::string& image_path1, const std::string& image_path2);
	void load_fundamental_matrix(const std::string& filepath);
protected:
	void mousePressEvent(QMouseEvent*);
	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
	cv::Mat m_correspond;
	std::vector<cv::Point2f> m_point_vector1;
	std::vector<cv::Point2f> m_point_vector2;
	std::vector<QColor> m_color_vector;
	int m_image_width1;
	int m_image_width2;
	cv::Mat m_f;
	int m_mode;
	std::vector<cv::Point2f> m_point_vector;
};

