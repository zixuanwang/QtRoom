#ifndef GEOMETRYCONSTRAINT_H
#define GEOMETRYCONSTRAINT_H

#include <boost/geometry.hpp>
#include <boost/math/constants/constants.hpp>

class GeometryConstraint
{
public:
    GeometryConstraint(float camera_height, float object_height, float view_angle, float theta, int image_width = 640, int image_height = 480); // angles are in degree.
    ~GeometryConstraint();
    //void set_image_center(float image_center_x, float image_center_y){m_image_center_x = image_center_x; m_image_center_y = image_center_y;} // load data from the calibration.
    int get_size(int bottom); // pass the y coordinate of the bottom
private:
    float m_camera_height;
    float m_object_height;
    float m_view_angle;
    float m_theta;
    int m_image_width;
    int m_image_height;
    float m_image_center_x;
    float m_image_center_y;
};

#endif // GEOMETRYCONSTRAINT_H
