#include "Utility.h"

Utility::Utility()
{
}

QImage Utility::mat2QImage(const cv::Mat3b& src){
    QImage dest(src.cols, src.rows, QImage::Format_ARGB32);
    for(int y = 0; y < src.rows; ++y){
        const cv::Vec3b* src_row = src[y];
        QRgb* dest_row = (QRgb*)dest.scanLine(y);
        for(int x = 0; x < src.cols; ++x){
            dest_row[x] = qRgba(src_row[x][2], src_row[x][1], src_row[x][0], 255);
        }
    }
    return dest;
}

float Utility::l1_norm(const std::vector<float>& v){
    float sum = 0.f;
    std::for_each(v.begin(), v.end(), [&sum](float v){sum += v;});
    return sum;
}

float Utility::l2_norm(const std::vector<float>& v){
    float sum = 0.f;
    std::for_each(v.begin(), v.end(), [&sum](float v){sum += v * v;});
    return sqrtf(sum);
}

void Utility::l1_normalize(std::vector<float>& v){
    float norm = l1_norm(v);
    if(norm == 0.f){
        std::cerr << "0 norm" << std::endl;
    }else{
        std::for_each(v.begin(), v.end(), [&](float& v){v /= norm;});
    }
}

void Utility::l2_normalize(std::vector<float>& v){
    float norm = l2_norm(v);
    if(norm == 0.f){
        std::cerr << "0 norm" << std::endl;
    }else{
        std::for_each(v.begin(), v.end(), [&](float& v){v /= norm;});
    }
}

void Utility::normalize_image(const cv::Mat& image, cv::Mat& output){
    double min_value;
    double max_value;
    cv::minMaxLoc(image, &min_value, &max_value);
    if(image.type() == CV_32FC1 || image.type() == CV_64FC1)
        image.convertTo(output, CV_32FC1, 1.0 / max_value);
    if(image.type() == CV_8UC1)
        image.convertTo(output, CV_8UC1, 255.0/ max_value);
}

void Utility::get_files(std::vector<std::string>& file_vector,
        const std::string& directory, bool recursive, const std::string& extension) {
    if (!boost::filesystem::exists(directory)) {
        return;
    }
    boost::filesystem::directory_iterator end_itr;
    for (boost::filesystem::directory_iterator itr(directory); itr != end_itr;
            ++itr) {
        if (recursive && boost::filesystem::is_directory(itr->status())) {
            get_files(file_vector, itr->path().string(), recursive);
        } else {
            if(extension.empty() || (!extension.empty() && itr->path().extension().string() == extension))
                file_vector.push_back(itr->path().string());
        }
    }
}

std::string Utility::get_parent_path(const std::string& filepath){
    boost::filesystem::path bst_filepath = filepath;
    return bst_filepath.parent_path().string();
}

std::string Utility::get_parent_name(const std::string& filepath){
    boost::filesystem::path bst_filepath = filepath;
    return bst_filepath.filename().string();
}

std::string Utility::get_stem(const std::string& filepath){
    boost::filesystem::path bst_filepath = filepath;
    return bst_filepath.stem().string();
}

std::string Utility::get_extension(const std::string& filepath){
    boost::filesystem::path bst_filepath = filepath;
    return bst_filepath.extension().string();
}

std::string Utility::get_stem_path(const std::string& filepath){
    boost::filesystem::path bst_filepath = filepath;
    return (bst_filepath.parent_path() / bst_filepath.stem()).string();
}

std::string Utility::get_name(const std::string& filepath){
    boost::filesystem::path bst_filepath = filepath;
    return bst_filepath.filename().string();
}

int64_t Utility::get_timestamp(){
    auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
