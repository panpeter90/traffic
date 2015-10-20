#pragma once

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#define WINDOW_NAME_MAIN "0. Original image"
#define WINDOW_NAME_OPTIONS "Parameterization"
#define WINDOW_NAME_BILATERAL_FILTER "1. Bilateral filter"
#define WINDOW_NAME_BILATERAL_FILTER_OPTIONS "1.1. Bilateral filter options"
#define WINDOW_NAME_HISTOGRAM_EQUALIZATION "2. Global histogram equalization (not used)"
#define WINDOW_NAME_HISTOGRAM_EQUALIZATION_CLAHE "2. Histogram equalization CLAHE"
#define WINDOW_NAME_HISTOGRAM_EQUALIZATION_CLAHE_OPTIONS "2.1. Histogram equalization CLAHE options"
#define WINDOW_NAME_CONTRAST_AND_BRIGHTNESS "3. Contrast, brightness and bilateral filtering (2nd pass)"
#define WINDOW_NAME_CONTRAST_AND_BRIGHTNESS_OPTIONS "3.1. Contrast and brightness options"
#define WINDOW_NAME_TARGET_DETECTION_VOTER_MASK "4. Target detection voter mask"
#define WINDOW_NAME_TARGET_DETECTION "5. Target detection"

#define TRACK_BAR_NAME_BILATERAL_FILTER_DIST "1Dist"
#define TRACK_BAR_NAME_BILATERAL_FILTER_COLOR_SIG "1Color Sig"
#define TRACK_BAR_NAME_BILATERAL_FILTER_SPACE_SIG "1Space Sig"
#define TRACK_BAR_NAME_CLAHE_CLIP "2Clip"
#define TRACK_BAR_NAME_CLAHE_TILE_X "2Tile X"
#define TRACK_BAR_NAME_CLAHE_TILE_Y "2Tile Y"
#define TRACK_BAR_NAME_CONTRAST "3Contr*10"
#define TRACK_BAR_NAME_BRIGHTNESS "3Brigh*10"

#define ESC_KEYCODE 27

#define IMAGE_TOKEN ".image.png"
#define MASK_TOKEN ".mask."
#define MASK_EXTENSION ".png"

#define IMGS_DIRECTORY "imgs/imgsDB/"
#define VOCABULARY_IMAGES_LIST "imgs/listBuildVocabulary.txt"
#define CLASSIFIER_IMGAGES_LIST "imgs/listBuildSamples.txt"
#define TEST_IMGAGES_LIST "imgs/listTest.txt"

#define TRAINING_DIRECTORY "training/"
#define VOCABULARY_BUILD_OUTPUT_DIRECTORY "training/vocabularyBuildOutput/"
#define SAMPLES_BUILD_OUTPUT_DIRECTORY "training/samplesBuildOutput/"
#define TEST_OUTPUT_DIRECTORY "testResults/"

#define TARGET_DETECTIONS_RECTANGLE_COLOR Scalar(45,255,255)
#define TARGET_KEYPOINT_COLOR Scalar(0,255,0)
#define NONTARGET_KEYPOINT_COLOR Scalar(0,0,255)

#define IMAGE_OUTPUT_EXTENSION ".png"

#define FILENAME_SEPARATOR "__"

#define VOCABULARY_WORD_COUNT 1000
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </constants definitions> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<