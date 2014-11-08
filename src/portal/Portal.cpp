//
//  Portal.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-9-1.
//
//

#include <jzp_lib/jzplib_all.h>

void invoke_annotate(fs::path userProfileDir) {
    cout<<"------- Invoking ./annotate --------------------"<<endl;
    string cmdpath = (fs::current_path()/"annotate").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_shape_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_shape_model -----------"<<endl;
    string cmdpath = (fs::current_path()/"train_shape_model").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_patch_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_patch_model -----------"<<endl;
    string cmdpath = (fs::current_path()/"train_patch_model").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_face_detector(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_face_detector ---------"<<endl;
    string cmdpath = (fs::current_path()/"train_face_detector").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_train_face_tracker(fs::path userProfileDir) {
    cout<<"------- Invoking ./train_face_tracker ----------"<<endl;
    string cmdpath = (fs::current_path()/"train_face_tracker").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_shape_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_shape_model -------"<<endl;
    string cmdpath = (fs::current_path()/"visualize_shape_model").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_patch_model(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_patch_model -------"<<endl;
    string cmdpath = (fs::current_path()/"visualize_patch_model").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_face_detector(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_face_detector -----"<<endl;
    string cmdpath = (fs::current_path()/"visualize_face_detector").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_visualize_face_tracker(fs::path userProfileDir) {
    cout<<"------- Invoking ./visualize_face_tracker ------"<<endl;
    string cmdpath = (fs::current_path()/"visualize_face_tracker").string();
    string cmd(cmdpath+" "+userProfileDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_PupilTracker(fs::path userProfileDir,fs::path cameraFile) {
    cout<<"------- Invoking ./PupilTracker ----------------"<<endl;
    string cmdpath = (fs::current_path()/"PupilTracker").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+cameraFile.string();
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_HeadPoseAdviser(fs::path userProfileDir,fs::path cameraFile) {
    cout<<"------- Invoking ./HeadPoseAdviser ----------"<<endl;
    string cmdpath = (fs::current_path()/"HeadPoseAdviser").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+cameraFile.string();
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_CurveDrawer(fs::path curvesDir) {
    cout<<"------- Invoking ./CurveDrawer -----------------"<<endl;
    string cmdpath = (fs::current_path()/"CurveDrawer").string();
    string cmd(cmdpath+" "+curvesDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_CurveGazingRecorder(fs::path curvesDir, fs::path userProfileDir, fs::path cameraProfilePath, fs::path outputDir) {
    cout<<"------- Invoking ./CurveGazingRecorder ---------"<<endl;
    string cmdpath = (fs::current_path()/"CurveGazingRecorder").string();
    string cmd(cmdpath+" "+curvesDir.string()+" "+userProfileDir.string()+" "+cameraProfilePath.string()+" "+outputDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_CameraCalibrator(fs::path cameraBaseDir) {
    cout<<"------- Invoking ./CameraCalibrator -----------------"<<endl;
    string cmdpath = (fs::current_path()/"CameraCalibrator").string();
    string cmd(cmdpath+" "+cameraBaseDir.string());
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_RegeneratePupilData(fs::path userProfileDir, fs::path videoFilePath) {
    cout<<"------- Invoking ./PupilTracker(noshow) ----------------"<<endl;
    string cmdpath = (fs::current_path()/"PupilTracker").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+videoFilePath.string()+" noshow";
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_AnnotationFlip(fs::path userProfileDir) {
    cout<<"------- Invoking ./AnnotationFlip ------------------"<<endl;
    string cmdpath = (fs::current_path()/"AnnotationFlip").string();
    string cmd = cmdpath+" "+userProfileDir.string();
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;

}

void invoke_VideoParsing(fs::path userProfileDir,fs::path cameraProfilePath, fs::path videoFilePath) {
    cout<<"------- Invoking ./Pupil Tracker (video parsing) ------------------"<<endl;
    string cmdpath = (fs::current_path()/"PupilTracker").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+cameraProfilePath.string()+" "+videoFilePath.string();
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_Scan3DFaceModel(fs::path userProfileDir,fs::path cameraProfilePath, fs::path testFile, float mnh, float maxnh, float mph, float maxph, float step) {
    cout<<"------- Invoking ./Scan3DFaceModel (video parsing) ------------------"<<endl;
    string cmdpath = (fs::current_path()/"Scan3DFaceModel").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+cameraProfilePath.string()+" "+testFile.string()+" "+boost::lexical_cast<string>(mnh)+" "+boost::lexical_cast<string>(maxnh)+" "+boost::lexical_cast<string>(mph)+" "+boost::lexical_cast<string>(maxph)+" "+boost::lexical_cast<string>(step);
    cout<<cmd<<endl;
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void invoke_Scan3DFaceModelWithScanSequenceFile(fs::path userProfileDir,fs::path cameraProfilePath, fs::path testFile, fs::path scanSequenceFile) {
    cout<<"------- Invoking ./Scan3DFaceModel (video parsing) ------------------"<<endl;
    string cmdpath = (fs::current_path()/"Scan3DFaceModel").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+cameraProfilePath.string()+" "+testFile.string()+" "+scanSequenceFile.string();
    cout<<cmd<<endl;
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

    userProfilePaths = listFilesRecursivelyWithExtension(userBasePath, "annotations.yaml", "");
    for (int i = 0; i < userProfilePaths.size(); i++) {
        userProfilePaths[i] = userProfilePaths[i].parent_path();
    }
    return userProfilePaths;
}

fs::path chooseCameraProfile(fs::path cameraBasePath) {
    vector<fs::path> existingProfiles = listFilesRecursivelyWithExtension(cameraBasePath, "CM", "yaml");
    int index = 0;
    while (existingProfiles.size()>0) {
        
        cout<<"There are "<<existingProfiles.size()<<" camera profiles:"<<endl;
        for (index = 0; index < existingProfiles.size(); index++) {
            cout<<"("<<index<<") "<<existingProfiles[index].string()<<endl;
        }
        string input;
        cout<<"which one do you choose ? ";
        cin >> input;
        
        if (is_number(input) && boost::lexical_cast<int>(input) >=0 && boost::lexical_cast<int>(input) < existingProfiles.size()) {
            return existingProfiles[boost::lexical_cast<int>(input)];
        }
    }
    cout<<"There is no user profile. You should create an new one."<<endl;
    return fs::path();
}

fs::path chooseUserProfile(fs::path userBasePath, bool withNew) {
    vector<fs::path> existingProfiles = listExistingUserProfiles(userBasePath);
    int index = 0;
    while (existingProfiles.size()>0 || withNew) {
        
        cout<<"There are "<<existingProfiles.size()<<" user profiles:"<<endl;
        for (index = 0; index < existingProfiles.size(); index++) {
            cout<<"("<<index<<") "<<existingProfiles[index].string()<<endl;
        }
        if (withNew) {
            cout<<"([n]ew) to create new profile"<<endl;
        }
        string input;
        cout<<"which one do you choose ? ";
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
    cout<<"There is no user profile. You should create an new one."<<endl;
    return fs::path();
}

vector<fs::path> chooseVideoFile(fs::path videoParingPath) {
    vector<fs::path> videoFiles = listFilesWithExtension(videoParingPath, "", "vision.mov");
    vector<fs::path> selectedFiles;
    int index = 0;
    while (videoFiles.size()>0) {
        
        cout<<"There are "<<videoFiles.size()<<" videos:"<<endl;
        for (index = 0; index < videoFiles.size(); index++) {
            cout<<"("<<index<<") "<<videoFiles[index].string()<<endl;
        }
        cout<<"(98) select video files from range_1 to range_2."<<endl;
        cout<<"(99) select all video files."<<endl;
        string input;
        cout<<"which one do you choose ? ";
        cin >> input;
        
        if (is_number(input) && boost::lexical_cast<int>(input) >=0 && boost::lexical_cast<int>(input) < videoFiles.size()) {
            selectedFiles.push_back(videoFiles[boost::lexical_cast<int>(input)]);
            return selectedFiles;
        }
        if (is_number(input) && boost::lexical_cast<int>(input) == 98) {
            cout<<"input range_1 and range_2:";
            vector<string> numberParts;
            cin.get();
            string input;
            getline(cin,input);
            boost::split(numberParts, input, boost::is_any_of(" "));
            int  rfrom,rend;
            rfrom = boost::lexical_cast<int>(numberParts[0]);
            rend = boost::lexical_cast<int>(numberParts[1]);

            for (int i = rfrom ; i <=rend; i++) {
                selectedFiles.push_back(videoFiles[i]);
            }
            return selectedFiles;
        }
        if (is_number(input) && boost::lexical_cast<int>(input) == 99) {
            for (int i = 0 ; i < videoFiles.size(); i++) {
                selectedFiles.push_back(videoFiles[i]);
            }
            return selectedFiles;
        }
    }
    cout<<"There is no video for parsing."<<endl;
    return selectedFiles;
}

void regeneratePupilCoordiantesFromExistingTests(fs::path resultsPath) {
    vector<fs::path> videoFiles = listFilesRecursivelyWithExtension(resultsPath, "", "avi");
    
    if (videoFiles.empty()) {
        cout<<" No .avi file is found in the result path"<<endl;
        return ;
    }
    
    cout<<endl;
    for (int i = 0; i < videoFiles.size(); i++) {
        fs::path cp = videoFiles[i];
        cout<<" processing ("<<i+1<<"/"<<videoFiles.size()<<") "<<cp.string()<<" ..."<<endl;
        invoke_RegeneratePupilData(cp.parent_path()/"user_profile", cp);
    }
}





int main(int argc, const char * argv[])
{
    
    fs::path  basePath, userBasePath, curvesPath, resultsPath, cameraCalibPath, videoParingPath;
    if (argc <=1) {
        cout<<"usage:"<<argv[0]<<" base_dir"<<" [result_sub_dir]"<<endl;
    }
    if (argc >=2) {
        basePath = fs::path(argv[1]);
        cout<<"base path: "<<basePath<<endl;
        fs::create_directories(basePath);
    }
    userBasePath = basePath/"user_profile";
    curvesPath = basePath/"curves";
    resultsPath = basePath/"results";
    cameraCalibPath = basePath /"camera_calibration";
    videoParingPath = basePath /"video_parsing";
    fs::create_directories(userBasePath);
    fs::create_directories(curvesPath);
    fs::create_directories(resultsPath);
    fs::create_directories(cameraCalibPath);
    fs::create_directories(videoParingPath);
    
    if (argc == 3) {
        resultsPath /= string(argv[2]);
        fs::create_directories(resultsPath);
        if (fs::is_directory(resultsPath) == false) {
            cout<<"illegal result_sub_dir name"<<endl;
            return 0;
        }
    }
    
    while (true) {
        cout<<"\nAvaliable Choices:"<<endl;
        cout<<"1. train or modify ASM face models."<<endl;
        cout<<"2. visualize existing ASM face models"<<endl;
        cout<<"3. create, modify, visualize curves."<<endl;
        cout<<"4. run gazing curve recorder"<<endl;
        cout<<"5. run pupil tracker."<<endl;
        cout<<"7. run chessboard camera calibration."<<endl;
        cout<<"8. regenerate pupil tracking coordinates for existing tests."<<endl;
        cout<<"9. run head pose adviser."<<endl;
        cout<<"10. flip the annotation for chosen profile."<<endl;
        cout<<"11. parse video file."<<endl;
        cout<<"12. scan pupiltracker variable for video"<<endl;
        cout<<"q. quit"<<endl;
        cout<<"------ Your choice : ";
        string input;
        cin >> input;
        
        if (is_number(input)) {
            int c = boost::lexical_cast<int>(input);
            if (c<1 || c>12) {
                cout<<"error number"<<endl;
                continue;
            }
            
            if (c == 1) {
                fs::path targetProfilePath = chooseUserProfile(userBasePath, true);
                trainASMModel(targetProfilePath);
            }
            
            if (c == 2) {
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                if (targetProfilePath.empty() == false) {
                    visualizeASMModel(targetProfilePath);
                }
                
            }
            
            if (c ==3) {
                invoke_CurveDrawer(curvesPath);
            }
            
            if (c == 4) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                cout<<"Choose camera profile:"<<endl;
                fs::path cameraProfilePath = chooseCameraProfile(cameraCalibPath);
                if (targetProfilePath.empty() == false) {
                    invoke_CurveGazingRecorder(curvesPath,targetProfilePath,cameraProfilePath,resultsPath);
                }
                
            }
            
            if (c == 5) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                fs::path cameraProfilePath = chooseCameraProfile(cameraCalibPath);
                if (targetProfilePath.empty() == false) {
                    invoke_PupilTracker(targetProfilePath,cameraProfilePath);
                }

            }
            
            if (c == 7) {
                invoke_CameraCalibrator(cameraCalibPath);
            }
            
            if (c == 8) {
                regeneratePupilCoordiantesFromExistingTests(resultsPath);
            }
            
            if (c == 9) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                fs::path cameraProfilePath = chooseCameraProfile(cameraCalibPath);
                if (targetProfilePath.empty() == false) {
                    invoke_HeadPoseAdviser(targetProfilePath,cameraProfilePath);
                }
            }
            
            if (c == 10) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                if (targetProfilePath.empty() == false) {
                    invoke_AnnotationFlip(targetProfilePath);
                }
            }
            
            if (c == 11) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                if (targetProfilePath.empty()) {
                    continue;
                }
                cout<<"Choose camera profile:"<<endl;
                fs::path cameraProfilePath = chooseCameraProfile(cameraCalibPath);
                if (cameraProfilePath.empty()) {
                    continue;
                }
                cout<<"Choose video file for parsing:"<<endl;
                vector<fs::path> videoFilePaths = chooseVideoFile(videoParingPath);
                if (videoFilePaths.empty()) {
                    continue;
                }
                for (int i = 0 ; i < videoFilePaths.size(); i++) {
                    invoke_VideoParsing(targetProfilePath, cameraProfilePath, videoFilePaths[i]);
                }
            }
            
            if (c == 12) {
                cout<<"Choose user profile:"<<endl;
                fs::path targetProfilePath = chooseUserProfile(userBasePath, false);
                if (targetProfilePath.empty()) {
                    continue;
                }
                cout<<"Choose camera profile:"<<endl;
                fs::path cameraProfilePath = chooseCameraProfile(cameraCalibPath);
                if (cameraProfilePath.empty()) {
                    continue;
                }
                cout<<"Choose video file for parsing:"<<endl;
                vector<fs::path> videoFilePaths = chooseVideoFile(videoParingPath);
                if (videoFilePaths.empty()) {
                    continue;
                }
                
                cout << "input scan range \"minNoseHeight maxNoseHeight minMidLipHeight maxMidLipHeight step (all zero for using ScanSequence.txt file)\":";
                vector<string> numberParts;
                cin.get();
                string input;
                getline(cin,input);
                boost::split(numberParts, input, boost::is_any_of(" "));
                float minNoseHeight,maxNoseHeight,minMidLipHeight,maxMidLipHeight,step;
                minNoseHeight = boost::lexical_cast<float>(numberParts[0]);
                maxNoseHeight = boost::lexical_cast<float>(numberParts[1]);
                minMidLipHeight = boost::lexical_cast<float>(numberParts[2]);
                maxMidLipHeight = boost::lexical_cast<float>(numberParts[3]);
                step = boost::lexical_cast<float>(numberParts[4]);
                for (int i = 0 ; i < videoFilePaths.size(); i++) {
                    fs::path videoFile =videoFilePaths[i];
                    fs::path testFile = videoFile.parent_path() / (videoFile.stem().string()+".test");
                    if (minNoseHeight+maxNoseHeight+minMidLipHeight+maxMidLipHeight+step != 0)
                        invoke_Scan3DFaceModel(targetProfilePath,cameraProfilePath,testFile,minNoseHeight,maxNoseHeight,minMidLipHeight,maxMidLipHeight,step);
                    else {
                        fs::path scanSequenceFile = videoFile.parent_path() / "ScanSequence.txt";
                        invoke_Scan3DFaceModelWithScanSequenceFile(targetProfilePath,cameraProfilePath,testFile,scanSequenceFile);
                    }
                }

            }
            
            
        } else if (boost::iequals(input, "q")) {
            cout<<"bye bye"<<endl;
            break;
        } else {
            cout<<"error input."<<endl;
        }
    }
}