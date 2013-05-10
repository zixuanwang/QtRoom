#include "Geometry.h"


Geometry::Geometry(void){
	m_mode = ANNOTATE;
}


Geometry::~Geometry(void)
{
}

void Geometry::annotate(const std::string& image_path1, const std::string& image_path2){
	cv::Mat image1 = cv::imread(image_path1);
	cv::Mat image2 = cv::imread(image_path2);
	m_correspond = cv::Mat(image1.rows, image1.cols + image2.cols, CV_8UC3, cv::Scalar(0));
	image1.copyTo(m_correspond(cv::Rect(0, 0, image1.cols, image1.rows)));
	image2.copyTo(m_correspond(cv::Rect(image1.cols, 0, image2.cols, image2.rows)));
	m_image_width1 = image1.cols;
	m_image_width2 = image2.cols;
}

void Geometry::load_fundamental_matrix(const std::string& filepath){
	cv::FileStorage f(filepath, cv::FileStorage::READ);
	f["f"] >> m_f; 
	f.release();
}

void Geometry::paintEvent(QPaintEvent*){
	QPainter painter(this);
	painter.drawImage(0, 0, Utility::mat2QImage(m_correspond));
	if(m_mode == ANNOTATE){
		for(size_t i = 0; i < m_point_vector2.size(); ++i){
			painter.setPen(QPen(m_color_vector[i], 2));
			painter.drawLine(m_point_vector1[i].x, m_point_vector1[i].y, m_point_vector2[i].x + m_image_width1, m_point_vector2[i].y);
		}
		if(m_point_vector1.size() > m_point_vector2.size()){
			painter.setPen(QPen(Qt::red, 2));
			painter.drawPoint(m_point_vector1.back().x, m_point_vector1.back().y);
		}
	}
	if(m_mode == VALIDATE){
		if(!m_point_vector.empty()){
			std::vector<cv::Vec3f> lines;
			int which_image = m_point_vector[0].x < m_image_width1 ? 1 : 2;
			cv::computeCorrespondEpilines(m_point_vector, which_image, m_f, lines);
			for(size_t i = 0; i < m_point_vector.size(); ++i){
				painter.setPen(QPen(m_color_vector[i], 2));
				painter.drawPoint(m_point_vector[i].x, m_point_vector[i].y);
				int y1 = -1.0 * lines[i][2] / lines[i][1];
				int y2 = -1.0 * lines[i][0] * m_image_width2 / lines[i][1] + y1;
				painter.drawLine(m_image_width1, y1, m_image_width1 + m_image_width2, y2);
			}
		}
	}
}

void Geometry::mousePressEvent(QMouseEvent* event){
	if(m_mode == ANNOTATE){
		if(m_point_vector1.size() == m_point_vector2.size()){
			m_point_vector1.push_back(cv::Point2f(event->pos().x(), event->pos().y()));
		}else{
			m_point_vector2.push_back(cv::Point2f(event->pos().x() % m_image_width1, event->pos().y()));
			QColor color(rand() % 256, rand() % 256, rand() % 256);
			m_color_vector.push_back(color);
		}
	}
	if(m_mode == VALIDATE){
		m_point_vector.push_back(cv::Point2f(event->pos().x(), event->pos().y()));
		QColor color(rand() % 256, rand() % 256, rand() % 256);
		m_color_vector.push_back(color);
	}
	repaint();
}

void Geometry::keyPressEvent(QKeyEvent *event){
	if(event->key() == Qt::Key_R){
		if(m_point_vector1.size() < 8 || m_point_vector2.size() < 8){
			QMessageBox::information(this, "Insufficient correspondences", "Please find more than 7 correspondences.");
		}else{
			// compute and save fundamental matrix.
			m_f = cv::findFundamentalMat(m_point_vector1, m_point_vector2);
			QString filepath = QFileDialog::getSaveFileName(this, tr("Save Fundamental Matrix"), QDir::homePath(), tr("Data File (*.xml)"));
			cv::FileStorage file(filepath.toStdString(), cv::FileStorage::WRITE);
			file << "f" << m_f; 
			file.release();
		}
	}
	if(event->key() == Qt::Key_C){
		m_point_vector1.clear();
		m_point_vector2.clear();
		m_color_vector.clear();
		m_point_vector.clear();
	}
	if(event->key() == Qt::Key_L){
		QString filepath = QFileDialog::getOpenFileName(this, tr("Load Fundamental Matrix"), QDir::homePath(), tr("Data File (*.xml)"));
		load_fundamental_matrix(filepath.toStdString());
		m_mode = VALIDATE;
		m_point_vector1.clear();
		m_point_vector2.clear();
		m_color_vector.clear();
		m_point_vector.clear();
	}
	repaint();
}