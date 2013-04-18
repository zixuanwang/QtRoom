#pragma once
#include <boost/circular_buffer.hpp>
#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include "GlobalConfig.h"
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QString>
#include <QThread>
#include <queue>
#include <stdexcept>
#include <string>
#include <sstream>
#include <thread>

class Camera : public QThread
{
    Q_OBJECT
signals:
    void image_changed();
public:
	// construct the camera by passing the ip address, username and password.
    Camera(const std::string& ip_address, const std::string& username = "", const std::string& password = "");
    virtual ~Camera(void);
    void run();
    std::string get_ip_address(){return m_ip_address;}
	static size_t invoke_frame_producer(void* ptr, size_t size, size_t nmemb, void* p_instance);
	size_t frame_producer(void* ptr, size_t size, size_t nmemb);
    cv::Mat generate_frame(unsigned char* c_ptr, size_t len);
    void fetch_frame(cv::Mat& image); // the consumer call this function to fetch image
	CURL* get_curl_handle(){return m_curl_handle;}
    void start_recording(const std::string& output_dir);
    void stop_recording();
private:
	std::string m_ip_address;
	std::string m_username;
	std::string m_password;
    CURL* m_curl_handle;
    volatile bool m_is_recording;
    boost::circular_buffer<cv::Mat> m_buffer_queue; // maintain the producer consumer queue.
    std::vector<uchar> m_buffer;
    std::mutex m_queue_mutex;
    cv::VideoWriter m_video_writer;
    std::ofstream m_timestamp_stream;
    static std::mutex s_av_mutex;
};

