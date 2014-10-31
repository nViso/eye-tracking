//
//  AnnotateFlip.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14/10/17.
//
//

#include "asm_face/ft.hpp"
#include "jzp_lib/jzplib_all.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        cout<<argv[0]<<" user_profile_dir"<<endl;
    }
    fs::path userProfileDirPath(argv[1]);

    
    vector<fs::path> annotateFiles = listFilesWithExtension(userProfileDirPath, "an", "yaml");
    if (annotateFiles.size() >1) {
        cout<<"Error ! multiple annotation files. check it !"<<endl;
        return 0;
    }
    for (int i = 0 ; i < annotateFiles.size(); i ++) {
        cout<<annotateFiles[i].string()<<endl;
        ft_data data = load_ft<ft_data>(annotateFiles[i].string().c_str());
        if (data.isAnnotationFlipped == true) {
            cout<<"this profile has been flipped. shouldn't do again!"<<endl;
            continue;
        }
        for(int perImageIndex = 0 ; perImageIndex < data.points.size() ; perImageIndex ++) {
            fliplr(data.points[perImageIndex], Size(1280,720));
        }
        data.isAnnotationFlipped = true;
        save_ft(annotateFiles[i].string(),data);
        
        vector<fs::path> images = listFilesWithExtension(annotateFiles[i].parent_path(), "", "png");
        for (int i = 0 ; i < images.size(); i++) {
            fs::path filePath = images[i];
            cout<<filePath<<endl;
            Mat image = imread(filePath.string(),CV_LOAD_IMAGE_COLOR);
            flip(image,image,1);
            imwrite(filePath.string(),image);
        }
        cout<<"Annotation flipped , you should re-run the training stage."<<endl;
    }
    
}