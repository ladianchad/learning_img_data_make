#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace cv;
using namespace std; 
pair<double,double> find_data(Mat& angle);
int main(int argc,char** argv){
	
	ifstream file;
	ofstream data_file;
	data_file.open("learning_data/data.txt");
	int img_num;
	file.open("img_data/camera1.txt");
	if(file.is_open()){
		string txt;
		getline(file,txt);
    	img_num = stoi(txt);
    	cout<<"IMG_NUM : "<<img_num<<endl;
    	file.close();
	}
	else{
		cout<<"CAN'T OPEN FILE"<<endl;
		return -1;
	}
	Mat img;
	string img_name = "img_data/camera1_";
	for(int i=0;i<img_num;i++){
		img = imread(img_name + to_string(i)+".jpg");
		if(img.empty()){
			cout<<"IMG_READ_ERROR"<<endl;
			return -1;
		}
		else{
			Mat temp_img;
			cvtColor(img, temp_img, COLOR_BGR2GRAY);
			Mat dx, dy; // 1st derivative in x,y
			Sobel(temp_img, dx, CV_64F, 1,0);
			Sobel(temp_img, dy, CV_64F, 0,1);
			Mat angle, mag;
			cartToPolar(dx, dy, mag, angle);
			pair<double,double> temp;
			temp = find_data(angle);
			resize(img, img, Size(280, 280), 0, 0, CV_INTER_LINEAR);
			imshow("now_img",img);
			waitKey(10);
			cout<<"================================="<<endl;
			cout<<"================================="<<endl;
			cout<<"[ANGLE]"<<endl;
			cout<<"Gradient average : "<<temp.first<<endl;
			cout<<"Deviation : "<<temp.second<<endl<<endl;
			cout<<"!! PLEASE TYPE !!"<<endl;
			cout<<"0 : nothing"<<endl;
			cout<<"1 : circle"<<endl;
			cout<<"<<";
			string write_data;
			cin>>write_data;
			write_data += " ";
			write_data += to_string(temp.first);
			write_data += " ";
			write_data += to_string(temp.second);
			data_file<<write_data<<endl;
		}
	}
	data_file.close();
	return 0;
}

pair<double,double> find_data(Mat& angle){
	double* p_data = (double*)angle.data; // v+1행 첫 칸의 주소를 불러온다. 
	double aver =0;
	for(int v= 0;v<angle.rows ; v++){
  		for(int u = 0 ; u<angle.cols; u++){
    		aver += p_data[v*angle.cols + u]; // v+1행 u+1열에 대한 연산을 수행한다. 
  		}
	}
	aver /= angle.rows*angle.rows;
	double err =0;
	for(int v= 0;v<angle.rows ; v++){
  		for(int u = 0 ; u<angle.cols; u++){
    		err += (aver - p_data[v*angle.cols + u])*(aver - p_data[v*angle.cols + u]); // v+1행 u+1열에 대한 연산을 수행한다. 
  		}
	}
	err /=angle.rows*angle.rows;
	return make_pair(aver,err);
}