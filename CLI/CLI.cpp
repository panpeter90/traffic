#include "CLI.h"
//This is start file

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  <Commnd Line user Interface>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void CLI::showConsoleHeader() {		
	cout << "####################################################################################################\n";
	cout << "  >>>                                          Car detection                                   <<<  \n";
	cout << "####################################################################################################\n\n";
}


void CLI::startInteractiveCLI() {
	int userOption = 1;
	string filename = "";
	int cameraDeviceNumber = 0;
	
	ConsoleInput::getInstance()->clearConsoleScreen();
	showConsoleHeader();
	
	int screenWidth = 1920; // ConsoleInput::getInstance()->getIntCin("  >> Screen width (used to arrange windows): ", "  => Width >= 100 !!!\n", 100);
	int screenHeight = 1080; // ConsoleInput::getInstance()->getIntCin("  >> Screen height (used to arrange windows): ", "  => Width >= 100 !!!\n", 100);
	bool optionsOneWindow = false; // ConsoleInput::getInstance()->getYesNoCin("  >> Use only one window for options trackbars? (Y/N): ");
	bool classifierTrained = false;

	do {
		try {
			ConsoleInput::getInstance()->clearConsoleScreen();
			showConsoleHeader();
		
			if (classifierTrained) {
				userOption = getUserOption();
				if (userOption == 1) {
					setupTraining();
				} else if (userOption == 2) {
					_imageDetector->evaluateDetector(TEST_IMGAGES_LIST);
				} else {
					if (userOption == 3 || userOption == 4) {
						filename = "";
						do {
							cout << "  >> Path to file: ";
							filename = ConsoleInput::getInstance()->getLineCin();

							if (filename == "") {
								cerr << "  => File path can't be empty!\n" << endl;
							}
						} while (filename == "");
					} else if (userOption == 5) {
						cameraDeviceNumber = ConsoleInput::getInstance()->getIntCin("  >> Insert the camera device number to use (default: 0): ", "  => Camera device number must be >= 0 !!!\n", 0);
					}
				
					ImageAnalysis imageAnalysis(_imagePreprocessor, _imageDetector);
					imageAnalysis.setScreenWidth(screenWidth);
					imageAnalysis.setScreenHeight(screenHeight);
					imageAnalysis.setOptionsOneWindow(optionsOneWindow);
					switch (userOption) {
						case 3: { if (!imageAnalysis.processImage(filename)) { cerr << "  => Failed to load image " << filename << "!" << endl; } break; }
						case 4: { if (!imageAnalysis.processVideo(filename)) { cerr << "  => Failed to load video " << filename << "!" << endl; } break; }
						case 5: { if (!imageAnalysis.processVideo(cameraDeviceNumber)) { cerr << "  => Failed to open camera " << cameraDeviceNumber << "!" << endl; } break; }
						default: break;
					}				
				}
			} else {
				setupTraining();
				classifierTrained = true;
			}

			if (userOption != 0) {
				cout << "\n\n" << endl;
				ConsoleInput::getInstance()->getUserInput();
			}
		} catch (...) {
			cerr << "\n\n\n!!!!! Caught unexpected exception !!!!!\n\n\n" << endl;
		}
	} while (userOption != 0);

	cout << "\n\n\n" << endl;
	showVersion();
	cout << "\n\n" << endl;
	ConsoleInput::getInstance()->getUserInput();
}


int CLI::getUserOption() {
	cout << " ## Detect car from:\n";
	cout << "   1 - Train classifier\n";
	cout << "   2 - Evaluate detector\n";
	cout << "   3 - Test detector from image\n";
	cout << "   4 - Test detector from video\n";
	cout << "   5 - Test detector from camera\n";
	cout << "   0 - Exit\n";

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [0, 4]: ", "Select one of the options above!", 0, 6);
}


void CLI::setupTraining() {
	cout << "\n\n ## Training setup:\n" << endl;

	int featureDetectorSelection = selectFeatureDetector();
	cout << "\n\n\n";
	int descriptorExtractorSelection = selectDescriptorExtractor();
	cout << "\n\n\n";
	int descriptorMatcherSelection = selectDescriptorMatcher();
	cout << "\n\n\n";
	int bowTrainerSelection = selectBOWTrainer();
	cout << "\n\n\n";
	int classifierSelection = selectClassifier();
	cout << "\n\n\n";

	Ptr<FeatureDetector> featureDetector;
	Ptr<DescriptorExtractor> descriptorExtractor;
	Ptr<DescriptorMatcher> descriptorMatcher;
	Ptr<BOWTrainer> bowTrainer;

	stringstream trainingConfigsTag;

	switch (featureDetectorSelection) {
		case 1: { featureDetector = new cv::SiftFeatureDetector();			trainingConfigsTag << "_SIFT-Detector"; break; }
		case 2: { featureDetector = new cv::SurfFeatureDetector(400);		trainingConfigsTag << "_SURF-Detector"; break; }
		case 3: { featureDetector = new cv::GoodFeaturesToTrackDetector();	trainingConfigsTag << "_GFTT-Detector"; break; }
		case 4: { featureDetector = new cv::FastFeatureDetector();			trainingConfigsTag << "_FAST-Detector"; break; }		
		case 5: { featureDetector = new cv::OrbFeatureDetector();			trainingConfigsTag << "_ORB-Detector";  break; }
		case 6: { featureDetector = new cv::BRISK();						trainingConfigsTag << "_BRISK-Detector"; break; }
		case 7: { featureDetector = new cv::StarFeatureDetector();			trainingConfigsTag << "_STAR-Detector"; break; }
		case 8: { featureDetector = new cv::MserFeatureDetector();			trainingConfigsTag << "_MSER-Detector"; break; }	
		default: break;
	}

	switch (descriptorExtractorSelection) {
		case 1: { descriptorExtractor = new cv::SiftDescriptorExtractor();	trainingConfigsTag << "_SIFT-Extractor"; break; }
		case 2: { descriptorExtractor = new cv::SurfDescriptorExtractor();	trainingConfigsTag << "_SURF-Extractor"; break; }
		case 3: { descriptorExtractor = new cv::FREAK();					trainingConfigsTag << "_FREAK-Extractor"; break; }
		case 4: { descriptorExtractor = new cv::BriefDescriptorExtractor();	trainingConfigsTag << "_BRIEF-Extractor"; break; }		
		case 5: { descriptorExtractor = new cv::OrbDescriptorExtractor();	trainingConfigsTag << "_ORB-Extractor";  break; }
		case 6: { descriptorExtractor = new cv::BRISK();					trainingConfigsTag << "_BRISK-Extractor";  break; }
		
		default: break;
	}

	bool binaryDescriptor;
	int bfNormType;
	Ptr<cv::flann::IndexParams> flannIndexParams/* = new cv::flann::AutotunedIndexParams()*/;
	if (descriptorExtractorSelection > 2) { // binary descriptors
		binaryDescriptor = true;
		bfNormType = cv::NORM_HAMMING;
		//flannIndexParams = new cv::flann::HierarchicalClusteringIndexParams();
		flannIndexParams = new cv::flann::LshIndexParams(20, 10, 2);
	} else { // float descriptors
		binaryDescriptor = false;
		bfNormType = cv::NORM_L2;
		flannIndexParams = new cv::flann::KDTreeIndexParams();
	}

	switch (descriptorMatcherSelection) {
		case 1: { descriptorMatcher = new cv::FlannBasedMatcher(flannIndexParams);	trainingConfigsTag << "_Flann-Matcher"; break; }
		case 2: { descriptorMatcher = new cv::BFMatcher(bfNormType, false);			trainingConfigsTag << "_BF-Matcher"; break; }
		default: break;
	}

	switch (bowTrainerSelection) {
		case 1: { bowTrainer = new cv::BOWKMeansTrainer(VOCABULARY_WORD_COUNT);	trainingConfigsTag << "_KMeans-BowTrainer"; break; }
		default: break;
	}

	string trainingDataFilename = trainingConfigsTag.str();

	stringstream vocabularyFilenameSS;
	vocabularyFilenameSS << VOCABULARY_TAG << trainingConfigsTag.str();

	switch (classifierSelection) {
		case 1: { trainingConfigsTag << "_SVM-Classifier"; break; }
		case 2: { trainingConfigsTag << "_ANN-Classifier"; break; }
		case 3: { trainingConfigsTag << "_Bayes-Classifier"; break; }		
		case 4: { trainingConfigsTag << "_DT-Classifier"; break; }
		case 5: { trainingConfigsTag << "_BT-Classifier"; break; }
		case 6: { trainingConfigsTag << "_GBT-Classifier"; break; }
		case 7: { trainingConfigsTag << "_RT-Classifier"; break; }
		case 8: { trainingConfigsTag << "_ERT-Classifier"; break; }		
		//case 9: { trainingConfigsTag << "_KNN-Classifier"; break; }
		default: break;
	}
	
	stringstream classifierFilenameSS;	
	classifierFilenameSS << CLASSIFIER_TAG << trainingConfigsTag.str();

	_bowVocabulary = new BowVocabulary(featureDetector, descriptorExtractor, descriptorMatcher, bowTrainer, _imagePreprocessor, vocabularyFilenameSS.str(), CLASSIFIER_TAG + trainingDataFilename, binaryDescriptor);
	
	switch (classifierSelection) {
		case 1: { _imageClassifier = new ImageClassifierSVM(_bowVocabulary, classifierFilenameSS.str()); break; }
		case 2: { _imageClassifier = new ImageClassifierANN(_bowVocabulary, classifierFilenameSS.str()); break; }
		case 3: { _imageClassifier = new ImageClassifierBayes(_bowVocabulary, classifierFilenameSS.str()); break; }		
		case 4: { _imageClassifier = new ImageClassifierDecisionTrees(_bowVocabulary, classifierFilenameSS.str()); break; }
		case 5: { _imageClassifier = new ImageClassifierBoost(_bowVocabulary, classifierFilenameSS.str()); break; }
		case 6: { _imageClassifier = new ImageClassifierGradientBoostingTrees(_bowVocabulary, classifierFilenameSS.str()); break; }
		case 7: { _imageClassifier = new ImageClassifierRandomTrees(_bowVocabulary, classifierFilenameSS.str()); break; }
		case 8: { _imageClassifier = new ImageClassifierExtremelyRandomizedTrees(_bowVocabulary, classifierFilenameSS.str()); break; }				
		//case 9: { _imageClassifier = new ImageClassifierKNN(_bowVocabulary, classifierFilenameSS.str()); break; }
		default: break;
	}

	_imageClassifier->trainClassifier(VOCABULARY_IMAGES_LIST, CLASSIFIER_IMGAGES_LIST);

	_imageDetector = new ImageDetectorSlidingWindow(_imageClassifier);
}


int CLI::selectFeatureDetector() {	
	cout << "  => Select feature detector:\n";
	cout << "    1 - SIFT\n";
	cout << "    2 - SURF\n";
	cout << "    3 - GFTT\n";
	cout << "    4 - FAST\n";	
	cout << "    5 - ORB\n";
	cout << "    6 - BRISK\n";
	cout << "    7 - STAR\n";
	cout << "    8 - MSER\n";

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [1, 7]: ", "Select one of the options above!", 1, 9);
}


int CLI::selectDescriptorExtractor() {	
	cout << "  => Select descriptor extractor:\n";
	cout << "    1 - SIFT\n";
	cout << "    2 - SURF\n";
	cout << "    3 - FREAK\n";
	cout << "    4 - BRIEF\n";			
	cout << "    5 - ORB\n";
	cout << "    6 - BRISK\n";	

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [1, 6]: ", "Select one of the options above!", 1, 7);
}


int CLI::selectDescriptorMatcher() {	
	cout << "  => Select descriptor matcher:\n";
	cout << "    1 - FlannBasedMatcher\n";
	cout << "    2 - BFMatcher\n";

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [1, 2]: ", "Select one of the options above!", 1, 3);
}


int CLI::selectBOWTrainer() {		
	cout << "  => Select Bag of Words trainer:\n";
	cout << "    1 - BOWKMeansTrainer\n";	

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [1]: ", "Select one of the options above!", 1, 2);
}


int CLI::selectClassifier() {
	cout << "  => Select classifier:\n";
	cout << "    1 - Support Vector Machines\n";
	cout << "    2 - Artificial Neural Networks\n";
	cout << "    3 - Normal Bayes Classifier\n";	
	cout << "    4 - Decision Trees\n";
	cout << "    5 - Boosting\n";
	cout << "    6 - Gradient Boosting Trees\n";
	cout << "    7 - Random Trees\n";
	cout << "    8 - Extremely Randomized Trees\n";	
	//cout << "    9 - K-Nearest Neighbors\n";

	return ConsoleInput::getInstance()->getIntCin("\n >>> Option [1]: ", "Select one of the options above!", 1, 9);
}


void CLI::showVersion() {
	cout << "+==================================================================================================+" << endl;
	cout << "|  Version 1.0 developed in 2013 for Computer Vision course (5th year, 1st semester, MIEIC, FEUP)  |" << endl;
	cout << "|  Author: Carlos Miguel Correia da Costa (carlos.costa@fe.up.pt / carloscosta.cmcc@gmail.com)     |" << endl;
	cout << "+==================================================================================================+" << endl;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  </Commnd Line user Interface>  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
