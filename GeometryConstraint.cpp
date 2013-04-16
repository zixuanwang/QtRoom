#include "GeometryConstraint.h"

GeometryConstraint::GeometryConstraint(float camera_height, float object_height, float view_angle, float theta, int image_width, int image_height) :
    m_camera_height(camera_height), m_object_height(object_height), m_image_width(image_width), m_image_height(image_height){
    m_view_angle = boost::math::constants::pi<float>() * view_angle / 180.f;
    m_theta = boost::math::constants::pi<float>() * theta / 180.f;
    m_image_center_x = static_cast<float>(image_width) / 2.f;
    m_image_center_y = static_cast<float>(image_height) / 2.f;
}

GeometryConstraint::~GeometryConstraint(){

}

int GeometryConstraint::get_size(int bottom){
    float distance = static_cast<float>(bottom) - m_image_center_y;
    float focal_length = 0.5f * static_cast<float>(m_image_height) / tanf(m_view_angle / 2.f);
    float angle = atanf(distance / focal_length); // \alpha - \theta
    float alpha = angle + m_theta;
    float sin_alpha = sinf(alpha);
    float beta = atanf(m_object_height * sin_alpha * cosf(alpha) / (m_camera_height + m_object_height * sin_alpha * sin_alpha));
    return static_cast<int>(focal_length * (tanf(angle) - tanf(angle - beta)));
}
