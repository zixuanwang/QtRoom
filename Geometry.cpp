#include "Geometry.h"


Geometry::Geometry(void){
	m_mode = ANNOTATE;
}


Geometry::~Geometry(void)
{
}

void Geometry::init_geometry(){
	cv::FileStorage f1("C:/Users/Zixuan/Dropbox/data/iroom/calibrate/171.67.83.73/camera.xml", cv::FileStorage::READ);
	cv::FileStorage f2("C:/Users/Zixuan/Dropbox/data/iroom/calibrate/171.67.83.75/camera.xml", cv::FileStorage::READ);
	cv::Mat camera1, camera2, dist1, dist2, R1, R2, t1, t2;
	f1["camera"] >> m_m1;
	f1["dist"] >> dist1;
	f1["r"] >> R1;
	f1["t"] >> m_t1;
	f2["camera"] >> m_m2;
	f2["dist"] >> dist2;
	f2["r"] >> R2;
	f2["t"] >> m_t2;
	f1.release();
	f2.release();
	cv::Mat image1 = cv::imread("C:/Users/Zixuan/Dropbox/data/iroom/annotate/171.67.83.73/fusion/empty.jpg");
	cv::Mat image2 = cv::imread("C:/Users/Zixuan/Dropbox/data/iroom/annotate/171.67.83.75/fusion/empty.jpg");
	m_correspond = cv::Mat(image1.rows, image1.cols + image2.cols, CV_8UC3, cv::Scalar(0));
	image1.copyTo(m_correspond(cv::Rect(0, 0, image1.cols, image1.rows)));
	image2.copyTo(m_correspond(cv::Rect(image1.cols, 0, image2.cols, image2.rows)));
	cv::Rodrigues(R1, m_r1);
	cv::Rodrigues(R2, m_r2);
	m_mode = HEIGHT;
	//cv::namedWindow("test");
	//cv::imshow("test", correspond);
	//cv::waitKey(0);
}

cv::Point2f Geometry::compute_correspondence(const cv::Point2f& point){
	cv::Mat n1 = m_m1.inv();
	cv::Mat q1 = m_r1.inv();
	std::stringstream ss;
	ss << m_r1 << std::endl;
	ss << q1 << std::endl;
	qDebug() << ss.str().c_str();
	double* n1_ptr = (double*)n1.data;
	double* q1_ptr = (double*)q1.data;
	double* t1_ptr = (double*)m_t1.data;
	double coeff = q1_ptr[6] * (n1_ptr[0] * point.x + n1_ptr[1] * point.y + n1_ptr[2]);
	coeff += q1_ptr[7] * (n1_ptr[3] * point.x + n1_ptr[4] * point.y + n1_ptr[5]);
	coeff += q1_ptr[8] * (n1_ptr[6] * point.x + n1_ptr[7] * point.y + n1_ptr[8]);
	double h = 1.0;
	double b = h + q1_ptr[6] * t1_ptr[0] + q1_ptr[7] * t1_ptr[1] + q1_ptr[8] * t1_ptr[2];
	double s = b / coeff;
	cv::Mat p1 = (cv::Mat_<double>(3, 1) << s * point.x, s * point.y , s);
	cv::Mat P = q1 * (n1 * p1 - m_t1);
	cv::Mat p2 = m_m2 * (m_r2 * P + m_t2);
	double* p2_ptr = (double*)p2.data;
	double s2 = p2_ptr[2];
	cv::Point2f point2(p2_ptr[0] / s2, p2_ptr[1] / s2);
	return point2;
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

bool Geometry::line_intersect_line(const cv::Point& l1p1, const cv::Point& l1p2, const cv::Point& l2p1, const cv::Point& l2p2){
    float q = (l1p1.y - l2p1.y) * (l2p2.x - l2p1.x) - (l1p1.x - l2p1.x) * (l2p2.y - l2p1.y);
    float d = (l1p2.x - l1p1.x) * (l2p2.y - l2p1.y) - (l1p2.y - l1p1.y) * (l2p2.x - l2p1.x);
    if( d == 0 ){
        return false;
    }
    float r = q / d;
    q = (l1p1.y - l2p1.y) * (l1p2.x - l1p1.x) - (l1p1.x - l2p1.x) * (l1p2.y - l1p1.y);
    float s = q / d;
    if( r < 0 || r > 1 || s < 0 || s > 1 ){
        return false;
    }
    return true;
}

bool Geometry::line_intersect_rect(const cv::Point& p1, const cv::Point& p2, const cv::Rect& r){
    return line_intersect_line(p1, p2, cv::Point(r.x, r.y), cv::Point(r.x + r.width, r.y)) ||
           line_intersect_line(p1, p2, cv::Point(r.x + r.width, r.y), cv::Point(r.x + r.width, r.y + r.height)) ||
           line_intersect_line(p1, p2, cv::Point(r.x + r.width, r.y + r.height), cv::Point(r.x, r.y + r.height)) ||
           line_intersect_line(p1, p2, cv::Point(r.x, r.y + r.height), cv::Point(r.x, r.y));
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
			cv::computeCorrespondEpilines(m_point_vector, m_which_image, m_f, lines);
			for(size_t i = 0; i < m_point_vector.size(); ++i){
				painter.setPen(QPen(m_color_vector[i], 2));
				if(m_which_image == 1){
					int y1 = -1.0 * lines[i][2] / lines[i][1];
					int y2 = -1.0 * lines[i][0] * m_image_width2 / lines[i][1] + y1;
					painter.drawPoint(m_point_vector[i].x, m_point_vector[i].y);
					painter.drawLine(m_image_width1, y1, m_image_width1 + m_image_width2, y2);
				}else{
					int y1 = -1.0 * lines[i][2] / lines[i][1];
					int y2 = -1.0 * lines[i][0] * m_image_width1 / lines[i][1] + y1;
					painter.drawPoint(m_point_vector[i].x + m_image_width1, m_point_vector[i].y);
					painter.drawLine(0, y1, m_image_width1, y2);
				}
			}
		}
	}
	if(m_mode == HEIGHT){
		if(!m_point_vector.empty()){
			for(size_t i = 0; i < m_point_vector.size(); ++i){
				painter.setPen(QPen(m_color_vector[i], 7));
				painter.drawPoint(m_point_vector[i].x, m_point_vector[i].y);
				cv::Point2f point = compute_correspondence(m_point_vector[i]);
				painter.drawPoint(point.x + 640, point.y);
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
		m_point_vector.push_back(cv::Point2f(event->pos().x() % m_image_width1, event->pos().y()));
		if(event->pos().x() < m_image_width1){
			m_which_image = 1;
		}else{
			m_which_image = 2;
		}
		QColor color(rand() % 256, rand() % 256, rand() % 256);
		m_color_vector.push_back(color);
	}
	if(m_mode == HEIGHT){
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
