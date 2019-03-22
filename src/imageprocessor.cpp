#include "imageprocessor.h"
#include <QDebug>
#include <QApplication>
#include <cmath>
ImageProcessor::ImageProcessor(QObject *parent) : QObject(parent)
{
    normal_bisel_depth = 0;
    normal_bisel_distance = 0;
    normal_depth = 10;
    normal_blur_radius = 1;
    normal_bisel_blur_radius = 1;
    gradient_end = 255;
    normal_bisel_soft = true;
    normalInvertX = normalInvertY = normalInvertZ = 1;
    busy = false;
}

int ImageProcessor::loadImage(QString fileName){
    m_fileName = fileName;
    m_img = imread(fileName.toStdString(),-1);

    if (m_img.channels() < 4){
        if (m_img.channels() == 3){
            cvtColor(m_img,m_img,COLOR_RGB2RGBA);
        }
        else{
            cvtColor(m_img,m_img,COLOR_GRAY2RGBA);
        }
    }

    cvtColor(m_img, m_gray,COLOR_RGBA2GRAY);
    if(m_gray.type() != CV_32FC1)
        m_gray.convertTo(m_gray, CV_32FC1);

    calculate_distance();
    m_distance.copyTo(new_distance);
    m_emboss_normal =(calculate_normal(m_gray,normal_depth,normal_blur_radius));
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                 ,normal_bisel_blur_radius);
    generate_normal_map();
    return 0;
}


void ImageProcessor::calculate_gradient(){


}

void ImageProcessor::calculate_distance(){

    if (!m_img.ptr<int>(0)) return;
    cvtColor(m_img, m_distance,CV_RGBA2GRAY);
    for(int x = 0; x < m_distance.cols; ++x)
    {
        for(int y = 0; y < m_distance.rows; ++y)
        {
            if (x == 0 || y == 0
                    || x == m_distance.cols-1
                    || y == m_distance.rows-1
                    || m_img.at<Vec4b>(y,x)[3] == 0.0)
            {
                m_distance.at<unsigned char>(y,x) = 0;
            }else{
                m_distance.at<unsigned char>(y,x) = 255;
            }
        }
    }
    threshold(m_distance,m_distance,1,255,THRESH_BINARY);

    distanceTransform(m_distance,m_distance,CV_DIST_L2,5);
    m_distance.convertTo(m_distance,CV_32FC1,1.0/255);
}

void ImageProcessor::set_normal_invert_x(bool invert){
    normalInvertX = -invert*2+1;
    m_emboss_normal =(calculate_normal(m_gray,normal_depth,normal_blur_radius));
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                 ,normal_bisel_blur_radius);
    generate_normal_map();

}
void ImageProcessor::set_normal_invert_y(bool invert){
    normalInvertY = -invert*2+1;
    m_emboss_normal =(calculate_normal(m_gray,normal_depth,normal_blur_radius));
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                 ,normal_bisel_blur_radius);
    generate_normal_map();
}
void ImageProcessor::set_normal_invert_z(bool invert){
    normalInvertZ = -invert*2+1;
    m_emboss_normal =(calculate_normal(m_gray,normal_depth,normal_blur_radius));
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                 ,normal_bisel_blur_radius);
    generate_normal_map();
}
void ImageProcessor::set_normal_depth(int depth){
    normal_depth = depth;
    Mat gray;
    m_gray.copyTo(gray);
    m_emboss_normal = calculate_normal(gray,normal_depth,normal_blur_radius);
    generate_normal_map();
}
void ImageProcessor::set_normal_bisel_soft(bool soft){
    normal_bisel_soft = soft;
    new_distance = modify_distance();
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                                                  ,normal_bisel_blur_radius);
    generate_normal_map();
}
void ImageProcessor::set_normal_blur_radius(int radius){
    normal_blur_radius = radius;
    Mat gray;
    m_gray.copyTo(gray);
    m_emboss_normal = calculate_normal(gray,normal_depth,normal_blur_radius);
    generate_normal_map();
}

void ImageProcessor::set_normal_bisel_depth(int depth){
    normal_bisel_depth = depth;
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                                                  ,normal_bisel_blur_radius);
    generate_normal_map();
}

void ImageProcessor::set_normal_bisel_distance(int distance){
    normal_bisel_distance = distance;
    new_distance = modify_distance();
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                                                  ,normal_bisel_blur_radius);
    generate_normal_map();
}

Mat ImageProcessor::modify_distance(){
    Mat m;
    m_distance.copyTo(m);
    for(int x = 0; x < m.cols; ++x)
    {
        for(int y = 0; y < m.rows; ++y)
        {
            if (normal_bisel_distance == 0) m.at<float>(y,x) = 0;
            else{
                m.at<float>(y,x) *= 255.0/normal_bisel_distance;
                if (m.at<float>(y,x) > 1) m.at<float>(y,x) = 1;
                if (normal_bisel_soft){
                    double d = m.at<float>(y,x);
                    //m.at<float>(y,x) = -d*(d-2);
                    m.at<float>(y,x) = sqrt(1-pow((d-1),2));
                }
            }
        }
    }
    return m;
}

void ImageProcessor::set_normal_bisel_blur_radius(int radius){
    normal_bisel_blur_radius = radius;
    m_distance_normal = calculate_normal(new_distance,normal_bisel_depth*normal_bisel_distance
                                                                  ,normal_bisel_blur_radius);
    generate_normal_map();
}

void ImageProcessor::generate_normal_map(){
    if (!m_img.ptr<int>(0) || busy) return;
    busy = true;
    Mat normals;
    normals =(m_emboss_normal
              + m_distance_normal);
    for(int x = 0; x < normals.cols; ++x)
    {
        for(int y = 0; y < normals.rows; ++y)
        {
            Vec3f n = normalize(normals.at<Vec3f>(y,x));
            normals.at<Vec3f>(y,x) = n*0.5+Vec3f(0.5,0.5,0.5);
        }
    }

    normals.convertTo(normals,CV_8UC3,255);
    normals.copyTo(m_normal);
    QImage p =QImage(static_cast<unsigned char *>(m_normal.data),
                                         m_normal.cols,m_normal.rows,m_normal.step,QImage::Format_RGB888);
    processed(p, ProcessedImage::Normal);
    busy = false;
    on_idle();
}

Mat ImageProcessor::calculate_normal(Mat mat, int depth, int blur_radius){
    Mat normals(mat.size(), CV_32FC3);
    Mat aux;
    Mat mdx,mdy,mg;
    float dx, dy;
    GaussianBlur(mat,aux,Size(blur_radius*2+1,blur_radius*2+1),0);
//    mat.copyTo(mg);
//    Sobel(mg,mdx,CV_32F, 1, 0);
//    Sobel(mg,mdy,CV_32F, 0, 1);
    //imshow("asdf",mdx);
    for(int x = 0; x < aux.cols; ++x)
    {
        for(int y = 0; y < aux.rows; ++y)
        {
            if (m_img.at<Vec4b>(y,x)[3] == 0.0){
                normals.at<Vec3f>(y,x) = Vec3f(0,0,1);
                continue;
            }
            if (x==0)
                dx = -3*aux.at<float>(y,x) + 4*aux.at<float>(y,x+1) - aux.at<float>(y,x+2);
            else if (x == aux.cols -1 )
                dx = aux.at<float>(y,x-2) + -4*aux.at<float>(y,x-1) +3*aux.at<float>(y,x);
            else
                dx = -aux.at<float>(y,x-1) + aux.at<float>(y,x);
            if (y==0)
                dy = -3*aux.at<float>(y,x) + 4*aux.at<float>(y,x+1) - aux.at<float>(y,x+2);
            else if (y == aux.rows -1 )
                dy = aux.at<float>(y-2,x) + -4*aux.at<float>(y-1,x) +3*aux.at<float>(y,x);
            else
                dy = -aux.at<float>(y-1,x) + aux.at<float>(y+1,x);
//            Vec3f n = Vec3f(-mdx.at<float>(y,x)*(depth/1000.0)*normalInvertX,
//                            -mdy.at<float>(y,x)*(depth/1000.0)*normalInvertY,
//                            1*normalInvertZ);
            Vec3f n = Vec3f(-dx*(depth/1000.0)*normalInvertX,
                            dy*(depth/1000.0)*normalInvertY,
                            1*normalInvertZ);
            normals.at<Vec3f>(y,x) = n;

        }
    }
    return normals;
}
