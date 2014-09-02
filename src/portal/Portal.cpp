//
//  Portal.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-9-1.
//
//
#include "stdlib.h"
#include <jzp_lib/jzplib_all.h>

void invoke_annotate(fs::path userProfileDir) {
    cout<<"------- Invoking ./annotate --------------------"<<endl;
    string cmd("./annotate "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_shape_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_shape_model -----------"<<endl;
    string cmd("./train_shape_model "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_patch_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_patch_model -----------"<<endl;
    string cmd("./train_patch_model "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_face_detector(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_face_detector ---------"<<endl;
    string cmd("./train_face_detector "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_face_tracker(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_face_tracker ----------"<<endl;
    string cmd("./train_face_tracker "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_shape_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_shape_model -------"<<endl;
    string cmd("./visualize_shape_model "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_patch_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_patch_model -------"<<endl;
    string cmd("./visualize_patch_model "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_face_detector(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_face_detector -----"<<endl;
    string cmd("./visualize_face_detector "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_face_tracker(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_face_tracker ------"<<endl;
    string cmd("./visualize_face_tracker "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_PupilTracker(fs::path userProfileDir) {
    cout<<"------- Invoking ./CurveDrawer -----------------"<<endl;
    string cmd("./PupilTracker "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_HeadPoseEstimation(fs::path userProfileDir) {
    cout<<"------- Invoking ./HeadPoseEstimation ----------"<<endl;
    string cmd("./HeadPoseEstimation "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_CurveDrawer(fs::path curvesDir) {
    cout<<"------- Invoking ./CurveDrawer -----------------"<<endl;
    string cmd("./CurveDrawer "+curvesDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_CurveGazingRecorder(fs::path curvesDir, fs::path userProfileDir, fs::path outputDir) {
    cout<<"------- Invoking ./CurveGazingRecorder ---------"<<endl;
    string cmd("./CurveGazingRecorder "+curvesDir.string()+" "+userProfileDir.string()+" "+outputDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}


void trainASMModel(fs::path userProfilePath) {
    invoke_annotate(userProfilePath);
    invoke_train_shape_model(userProfilePath);
    invoke_train_patch_model(userProfilePath);
    invoke_train_face_detector(userProfilePath);
    invoke_train_face_tracker(userProfilePath);
}

void visualizeASMModel(fs::path userProfilePath) {
    invoke_visualize_shape_model(userProfilePath);
    invoke_visualize_patch_model(userProfilePath);
    invoke_visualize_face_detector(userProfilePath);
    invoke_visualize_face_tracker(userProfilePath);
}

vector<fs::path> listExistingUserProfiles(fs::path userBasePath) {
    vector<fs::path> userProfilePaths;
    fs::directory_iterator iterator(userBasePath);
    
    for (; iterator != fs::directory_iterator(); iterator++) {
        if (fs::exists(iterator->path()/ "annotations.yaml")) {
            userProfilePaths.push_back(iterator->path());
        }
    }
    return userProfilePaths;
}

fs::path chooseUserProfile(fs::path userBasePath, bool withNew) {
    vector<fs::path> existingProfiles = listExistingUserProfiles(userBasePath);
    int index = 0;
    while (true) {
        
        cout<<"There are "<<existingProfiles.size()<<" user profiles:"<<endl;
        for (index = 0; index < existingProfiles.size(); index++) {
            cout<<"("<<index<<") "<<existingProfiles[index].string()<<endl;
        }
        if (withNew) {
            cout<<"([n]ew) to create new profile"<<endl;
        }
        cout<<"which one do you choose ? ";
        string input;
        cin >> input;
        
        if (is_number(input) && boost::lexical_cast<int>(input) >=0 && boost::lexical_cast<int>(input) < existingProfiles.size()) {
            return existingProfiles[boost::lexical_cast<int>(input)];
        } else if (boost::starts_with(input, "n") && withNew ){
            cout<<"input new user profile name: ";
            string newname;
            cin >> newname;
            fs::path newProfilePath = userBasePath / newname;
            fs::create_directories(newProfilePath);
            return newProfilePath;
        } else {
            cout<<"error input"<<endl;
        }
    }
}





int main(int argc, const char * argv[])
{
    fs::path  basePath, userBasePath, curvesPath, resultsPath;
    if (argc >=2) {
        basePath = fs::path(argv[1]);
    } else {
        cout<<"enter the base dir path:";
        string path;
        cin >> path;
        if ( fs::is_directory(fs::path(path))) {
            basePath = path;
        }
        cout<<"your base folder path is :"<<basePath.string()<<endl;
    }
    userBasePath = basePath/"user_profile";
    curvesPath = basePath/"curves";
    resultsPath = basePath/"results";
    fs::create_directories(userBasePath);
    fs::create_directories(curvesPath);
    fs::create_directories(resultsPath);
    
    while (true) {
        cout<<"\nAvaliable Choices:"<<endl;
        cout<<"1. train or modify ASM face models."<<endl;
        cout<<"2. visualize existing ASM face models"<<endl;
        cout<<"3. create, modify, visualize curves."<<endl;
        cout<<"4. run gazing curve recorder"<<endl;
        cout<<"q. quit"<<endl;
        cout<<"------ Your choice : ";
        string input;
        cin >> input;
        
        if (is_number(input)) {
            int c = boost::lexical_cast<int>(input);
            if (c<1 || c>4) {
                cout<<"error number"<<endl;
                continue;
            }
            
            if (c == 1) {
                fs::path targetProfilePath = chooseUserProfile(userBasePath, true);
                trainASMModel(targetProfilePath);
            }
            
            if (c == 2) {
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                visualizeASMModel(targetProfilePath);
            }
            
            if (c ==3) {
                invoke_CurveDrawer(curvesPath);
            }
            
            if (c == 4) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                invoke_CurveGazingRecorder(curvesPath,targetProfilePath,resultsPath);
            }
            
            
            
        } else if (boost::iequals(input, "q")) {
            cout<<"bye bye"<<endl;
            break;
        } else {
            cout<<"error input."<<endl;
        }
    }
}