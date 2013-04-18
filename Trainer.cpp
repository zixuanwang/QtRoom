#include "Trainer.h"

Trainer::Trainer()
{
}

void Trainer::train_hog(const std::string& train_directory){
    SVMClassifier classifier;
    HoGDescriptor hog_descriptor;
    std::string positive_directory = train_directory + "/positive";
    std::string negative_directory = train_directory + "/negative";
    std::vector<std::string> positive_image_vector;
    std::vector<std::string> negative_image_vector;
    Utility::get_files(positive_image_vector, positive_directory, false, ".jpg");
    Utility::get_files(negative_image_vector, negative_directory, false, ".jpg");
    int positive_sample_count = 0;
    // add positive samples
    for(size_t i = 0; i < positive_image_vector.size(); ++i){
        cv::Mat image = cv::imread(positive_image_vector[i]);
        std::string label_path = Utility::get_stem_path(positive_image_vector[i]) + ".txt";
        std::ifstream label_stream(label_path);
        std::string line;
        while(getline(label_stream, line)){
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of(","));
            cv::Rect rect(boost::lexical_cast<int>(strs[0]), boost::lexical_cast<int>(strs[1]), boost::lexical_cast<int>(strs[2]), boost::lexical_cast<int>(strs[3]));
            cv::Mat sub_image = image(rect);
            cv::Mat patch;
            cv::resize(sub_image, patch, cv::Size(GlobalConfig::TRAIN_PATCH_WIDTH, GlobalConfig::TRAIN_PATCH_HEIGHT));
            std::vector<float> sample;
            hog_descriptor.compute(patch, cv::Rect(0, 0, GlobalConfig::TRAIN_PATCH_WIDTH, GlobalConfig::TRAIN_PATCH_HEIGHT), sample);
            classifier.add_sample(sample, 1);
            ++positive_sample_count;
        }
        qDebug() << "Processing " << positive_image_vector[i].c_str();
        label_stream.close();
    }
    // add negative samples
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> length_dist(48, 80);
    int negative_image_vector_size = static_cast<int>(negative_image_vector.size());
    for(int i = 0; i < negative_image_vector_size; ++i){
        cv::Mat image = cv::imread(negative_image_vector[i]);
        int width = image.cols;
        int height = image.rows;
        for(int j = 0; j < positive_sample_count / negative_image_vector_size; ++j){
            int length = length_dist(gen);
            cv::Rect rect(rand() % (width - length -1), rand() % (height - length -1), length, length);
            cv::Mat sub_image = image(rect);
            cv::Mat patch;
            cv::resize(sub_image, patch, cv::Size(GlobalConfig::TRAIN_PATCH_WIDTH, GlobalConfig::TRAIN_PATCH_HEIGHT));
            std::vector<float> sample;
            hog_descriptor.compute(patch, cv::Rect(0, 0, GlobalConfig::TRAIN_PATCH_WIDTH, GlobalConfig::TRAIN_PATCH_HEIGHT), sample);
            classifier.add_sample(sample, -1);
        }
    }
    classifier.build();
    classifier.save(train_directory + "/hog.svm");
}

void Trainer::train_cascade(const std::string& train_directory){
    // generate info.dat
    std::string positive_directory = train_directory + "/positive";
    std::vector<std::string> positive_image_vector;
    Utility::get_files(positive_image_vector, positive_directory, false, ".jpg");
    std::ofstream info_stream(train_directory + "/info.dat");
    for(size_t i = 0; i < positive_image_vector.size(); ++i){
        info_stream << "positive/" << Utility::get_name(positive_image_vector[i]) << "\t";
        std::vector<cv::Rect> rect_vector;
        std::string label_path = Utility::get_stem_path(positive_image_vector[i]) + ".txt";
        std::ifstream label_stream(label_path);
        std::string line;
        while(getline(label_stream, line)){
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of(","));
            cv::Rect rect(boost::lexical_cast<int>(strs[0]), boost::lexical_cast<int>(strs[1]), boost::lexical_cast<int>(strs[2]), boost::lexical_cast<int>(strs[3]));
            rect_vector.push_back(rect);
        }
        label_stream.close();
        info_stream << rect_vector.size() << "\t";
        for(size_t j = 0; j < rect_vector.size(); ++j){
            info_stream << rect_vector[j].x << " " << rect_vector[j].y << " " << rect_vector[j].width << " " << rect_vector[j].height << "\t";
        }
        info_stream << std::endl;
    }
    info_stream.close();
    // generate bg.txt
    std::string negative_directory = train_directory + "/negative";
    std::vector<std::string> negative_image_vector;
    Utility::get_files(negative_image_vector, negative_directory, false, ".jpg");
    std::ofstream bg_stream(train_directory + "/bg.txt");
    for(size_t i = 0; i < negative_image_vector.size(); ++i){
        bg_stream << "negative/" << Utility::get_name(negative_image_vector[i]) << std::endl;
    }
    bg_stream.close();
}
