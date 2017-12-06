#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;
using namespace chrono;

int main( int argc, char** argv )
{
  //从参数中读出图像
  Mat image1 = imread( argv[1], 6 );
  Mat image2 = imread( argv[2], 6 );
  
  //定义存储关键点的容器
  vector<KeyPoint> keypoints1,keypoints2;
  
  //定义存储描述子的容器
  Mat descriptors1,descriptors2;
  
  //定义SIFT特征点提取对象并进行特征提取
  SiftFeatureDetector sift_detector;
  steady_clock::time_point t1 = steady_clock::now();
  sift_detector.detect(image1,keypoints1);
  sift_detector.detect(image2,keypoints2);
  steady_clock::time_point t2 = steady_clock::now();
  duration<double> time_detect = duration_cast<duration<double>>(t2-t1);
  cout<<"提取两张图片的特征点共用了"<<time_detect.count()<<"s."<<endl;
  
  //绘制所提取出的特征点
  Mat image1_out;
  Mat image2_out;
  drawKeypoints(image1,keypoints1,image1_out,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
  drawKeypoints(image2,keypoints2,image2_out,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
  imshow("图一提取到的SIFT特征点",image1_out);
  imshow("图二提取到的SIFT特征点",image2_out);
  waitKey(0);
  destroyWindow("图一提取到的SIFT特征点");
  destroyWindow("图二提取到的SIFT特征点");
  
  //定义SIFT特征点描述子的计算对象并计算描述子
  SiftDescriptorExtractor sift_descriptor;
  steady_clock::time_point t3 = steady_clock::now();
  sift_descriptor.compute(image1,keypoints1,descriptors1);
  sift_descriptor.compute(image2,keypoints2,descriptors2);
  steady_clock::time_point t4 = steady_clock::now();
  duration<double> time_descriptor_compute = duration_cast<duration<double>>(t4-t3);
  cout<<"计算两张图片的描述子共用了"<<time_descriptor_compute.count()<<"s."<<endl;
  
  //定义描述子匹配对象
  BFMatcher sift_matcher(NORM_L2);
  
  //定义存储匹配结果的容器
  vector<DMatch> matches;
  vector<DMatch> good_matches;
  
  //特征点通过计算描述子进行匹配
  steady_clock::time_point t5 = steady_clock::now();
  sift_matcher.match(descriptors1,descriptors2,matches);
  steady_clock::time_point t6 = steady_clock::now();
  duration<double> time_match = duration_cast<duration<double>>(t6-t5);
  cout<<"特征点匹配共用了"<<time_match.count()<<"s."<<endl;
  
  //根据距离进行剔除
  double max_dist=0,min_dist=1000;
  for( int i=0;i<matches.size();i++)
  {
    double distance = matches[i].distance;
    if(distance<min_dist)
      min_dist=distance;
    if(distance>max_dist)
      max_dist=distance;
  }
  
  cout<<"最大距离："<<max_dist<<endl<<"最小距离："<<min_dist<<endl;
  
  for( int j=0;j<matches.size();j++)
  {
    if(matches[j].distance <= max( 2*min_dist,30.0))
      good_matches.push_back(matches[j]);
  }
  
  //绘制匹配的点对
  Mat image_matches;
  drawMatches(image1,keypoints1,image2,keypoints2,good_matches,image_matches);
  imshow("图一和图二的SIFT匹配结果",image_matches);
  waitKey(0);
  destroyWindow("图一和图二的SIFT匹配结果");
  
  return 0;
}