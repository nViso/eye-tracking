//
//  ft.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-16.
//
//

#include "opencv_hotshots/ft/ft.hpp"




ft_data load_ft_jzp(string fname) {
    bool withFileName = fname.find("annotations.yaml") != string::npos ;
    if (withFileName) {
        if (access(fname.c_str(),F_OK) == -1) {
            cout<<"Cannot find the file annotations.yaml in "<<fname<<endl;
            return ft_data();
        }
        ft_data data = load_ft<ft_data>(fname.c_str());
        data.baseDir = fname.substr(0, fname.find_last_of("/\\")+1);
        return data;
    } else {
        string::size_type sepExist = fname.find_last_of("/\\");
        if (sepExist == string::npos || sepExist <fname.length()-1) {
            fname +="/";
        }
        const char* fileName = string(fname+"annotations.yaml").c_str();
        if (access(fileName,F_OK) == -1) {
            cout<<"Cannot find the file annotations.yaml in "<<fname<<endl;
            return ft_data();
        }
        ft_data data = load_ft<ft_data>(fileName);
        data.baseDir = fname;
        return data;
    }
}