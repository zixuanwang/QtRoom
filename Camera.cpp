#include "Camera.h"

std::mutex Camera::s_av_mutex;

Camera::Camera(const std::string& ip_address, const std::string& username, const std::string& password) : QThread(), m_ip_address(ip_address), m_username(username), m_password(password), m_curl_handle(NULL), m_is_recording(false), m_buffer_queue(IMAGE_BUFFER_SIZE){
    std::string userpass = m_username + ":" + m_password;
    m_curl_handle = curl_easy_init();
    curl_easy_setopt(m_curl_handle, CURLOPT_USERPWD, userpass.c_str());
}

Camera::~Camera(void){
    m_is_recording = false;
    if(m_timestamp_stream.is_open())
        m_timestamp_stream.close();
    m_video_writer.release();
	curl_easy_cleanup(m_curl_handle);
}

// producer wrapper
size_t Camera::invoke_frame_producer(void* ptr, size_t size, size_t nmemb, void* p_instance){
	return ((Camera*)p_instance)->frame_producer(ptr, size, nmemb);
}

// producer function. put each fetched image to the queue.
size_t Camera::frame_producer(void* ptr, size_t size, size_t nmemb){
	size_t len = size * nmemb;
	unsigned char* c_ptr = (unsigned char*)ptr;
	cv::Mat image = generate_frame(c_ptr, len);
	if(!image.empty()){
		std::unique_lock<std::mutex> lock(m_queue_mutex);
        m_buffer_queue.push_back(image);        
        if(m_is_recording){// save frame if recording
            m_video_writer << image;
            auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
            m_timestamp_stream << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << std::endl;
        }
        emit image_changed(m_ip_address.c_str());
	}	
	return len;
}

// search data block for SOI and EOI.
cv::Mat Camera::generate_frame(unsigned char* c_ptr, size_t len){
	cv::Mat image;
	bool start = false;
	bool end = false;
	unsigned char* soi_ptr = c_ptr; 
	unsigned char* eoi_ptr = c_ptr + len;
	for(size_t i = 0; i < len - 1; ++i){
		if(c_ptr[i] == 0xFF){
			if(c_ptr[i+1] == 0xD8){
				soi_ptr = c_ptr + i;
				start = true;
			}
			if(c_ptr[i+1] == 0xD9){
				eoi_ptr = c_ptr + i;
				end = true;
			}
		}
	}
	if(start && end){
		if(soi_ptr < eoi_ptr){
			m_buffer.assign(soi_ptr, eoi_ptr); // start before end.
		}else{
			std::copy(c_ptr, eoi_ptr, std::back_inserter(m_buffer)); // end before start.
			image = cv::imdecode(m_buffer, 1);
			m_buffer.assign(soi_ptr, c_ptr + len); // start a new frame
		}
	}else if(start){
		m_buffer.assign(soi_ptr, eoi_ptr);
	}else if(!m_buffer.empty()){
		std::copy(soi_ptr, eoi_ptr, std::back_inserter(m_buffer));
		if(end){
			image = cv::imdecode(m_buffer, 1);
		}
	}
	return image;
}

void Camera::run(){
    std::thread producer([&]{
        std::string url = "http://" + m_ip_address + "/nphMotionJpeg?Resolution=640x480&Quality=Clarity";
        curl_easy_setopt(m_curl_handle, CURLOPT_NOSIGNAL, 1); // resolve longjmp error.
        curl_easy_setopt(m_curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(m_curl_handle, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(m_curl_handle, CURLOPT_WRITEFUNCTION, invoke_frame_producer);
        curl_easy_setopt(m_curl_handle, CURLOPT_TIMEOUT, 0); // set timeout
        curl_easy_perform(m_curl_handle);
    });
    producer.join();
}

void Camera::fetch_frame(cv::Mat& image){
    std::unique_lock<std::mutex> lock(m_queue_mutex);
    if(!m_buffer_queue.empty()){
        image = m_buffer_queue.front();
        m_buffer_queue.pop_front();
    }
}

void Camera::start_recording(const std::string& output_dir){
    auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
    std::stringstream ss;
    ss << output_dir << "/" << m_ip_address << "." << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::string video_path = ss.str() + ".avi";
    std::string timestamp_path = ss.str() + ".timestamp";
    s_av_mutex.lock(); // make libav happy in multithreading.
    m_video_writer.open(video_path, CV_FOURCC('M','P','4','2'), 30, cv::Size(640, 480)); // output video format.
    s_av_mutex.unlock();
    m_timestamp_stream.open(timestamp_path);
    m_is_recording = true;
}

void Camera::stop_recording(){
    m_is_recording = false;
    if(m_timestamp_stream.is_open())
        m_timestamp_stream.close();
    m_video_writer.release();
}
