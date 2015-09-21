/**
 *  @file   
 * 
 *  @brief  
 */

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TInterpreter.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"
#include "TVector.h"

#include "ResultsContainer.h"
#include "GroupedContainer.h"
#include "MaskContainer.h"

#pragma link C++ class std::vector<std::vector<int> >+;   

std::string toString(int a);
int toInt(std::string s);
float toFloat(std::string s);
void mergeResultsContainer(std::string fileName);
void saveResultsContainer(std::string fileName, std::vector<ResultsContainer*> resultsContainers);
void loadResultsContainer(std::string fileName, std::vector<ResultsContainer*> &resultsContainers);

int main(int argc,char *argv[])
{
    const int nArgs(argc - 1);

    if ((nArgs < 1) || (nArgs > 6))
    {
        std::cout << std::endl;
        std::cout << "Number of arguements given : " << argc << std::endl
                  << "Usage: ./Process.exe setNumber generateData [fileName] [sequenceNumberToRead] [voltagePulseToRead] [analyseData]" << std::endl << std::endl
                  << "  setNumer               : Set number to analse/save/merge. " << std::endl
                  << "  generateData           : [2 == merge data | 1 == save new data | 0 == load data from file]" << std::endl
                  << "  fileName               : Optional parameter to be set if creating new data. Base name of root file containing results containers for pixel analysis (please end in .root)." << std::endl
                  << "  sequenceNumberToRead   : Optional parameter to be set if creating new data. Text files with this sequence number will be converted to root files." << std::endl
                  << "  voltagePulseToRead     : Optional parameter to be set if creating new data. Text files with this voltage pulse will be converted to root files." << std::endl
                  << "  analyseData            : Optional parameter to be set if creating new data. [1 == analyse data | 0 == create data]" << std::endl;
        return 1;
    }

    const int setNumber(toInt(argv[1]));
    bool saveResults((toInt(argv[2]) == 1) ? true : false);
    bool mergeResults((toInt(argv[2]) == 2) ? true : false);
    bool analyseResults((nArgs == 6) ? ((toInt(argv[6]) == 1) ? true : false ) : true);

    const std::string fileName((nArgs == 6) ? argv[3] : "");
    const std::string sequenceNumberToRead((nArgs == 6) ? argv[4] : "");
    const std::string voltagePulseToRead((nArgs == 6) ? argv[5] : "");

    const std::string rootFileFolder("/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/RootFileResults/Set" + toString(setNumber));
    const std::string rawDataFileFolder("/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Results/RawResults_Set" + toString(setNumber) + "/Seq" + sequenceNumberToRead);

    std::string maskFile("");

    if (setNumber == 9)
        maskFile = "/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Masks/maskedPixels10032_SET9.dat";
    else if (setNumber == 10)
        maskFile = "/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Masks/maskedPixels10320_SET10.dat";
    else if (setNumber == 12)
        maskFile = "/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Masks/maskedPixels10494_SET12.dat";
    else if (setNumber == 13)
        maskFile = "/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Masks/maskedPixels11044_SET13.dat";
    else if (setNumber == 14)
        maskFile = "/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Masks/maskedPixels10905_SET14.dat";
    else if (setNumber == 15)
        maskFile = "/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Masks/maskedPixels10677_SET15.dat";

    const std::string interestingPlots("/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Plots/Set" + toString(setNumber) + "/InterestingPlots/");
    const std::string bulkPlots("/afs/cern.ch/work/s/sgreen/CLICpixTestPulse/Plots/Set" + toString(setNumber) + "/BulkPlots/");

    // Variables used in saving and analysing data
    int pixelGridSize(64);
    int parallelProcess = 1;

    if (saveResults)
    {
        std::cout << "===== Saving New Results =====" << std::endl;
        std::cout << "File name for saving                   : " << fileName << std::endl;
        std::cout << "Folder name for loading raw data files : " << rawDataFileFolder << std::endl;
        std::cout << "Folder name for saving root files      : " << rootFileFolder << std::endl;
        if (analyseResults)
        {
            std::cout << "Analysing output results." << std::endl;
        }
        else
        {
            std::cout << "Not analysing output results.  Just creating the file data." << std::endl;
        }
    }

    else if (mergeResults)
    {
        std::cout << "===== Merging Results =====" << std::endl;
        analyseResults = false;
    }

    else
    {
        std::cout << "===== Loading Results =====" << std::endl;
        std::cout << "Folder name for loading : " << rootFileFolder << std::endl;
    }

    gStyle->SetOptStat(kFALSE); 
 
    if (!(gInterpreter->IsLoaded("vector")))
        gInterpreter->ProcessLine("#include <vector>");
    gSystem->Exec("rm -f AutoDict*vector*vector*int*");
    gInterpreter->GenerateDictionary("vector<vector<int> >", "vector");

    if (saveResults)
    {
        int maskSpacing(4);

        std::vector<std::string> resultsFiles;

        DIR *dir;
        struct dirent *ent;

        const std::string sequenceStringToFind = "_Seq" + sequenceNumberToRead + "_";
        const std::string voltagePulseStringToFind = "_VoltagePulse" + voltagePulseToRead + "mV_TestPulse_";

        if ((dir = opendir(rawDataFileFolder.c_str())) != NULL) 
        {
            // Load all the files and directories within directory 
            while ((ent = readdir(dir)) != NULL) 
            {
                //printf ("Loading results text file: %s\n", ent->d_name);
                std::string potentialFileName = ent->d_name;
                if(potentialFileName.find("TestPulse") != std::string::npos and potentialFileName.find(sequenceStringToFind) != std::string::npos and potentialFileName.find(voltagePulseStringToFind) != std::string::npos)
                {
                    printf ("Loading results text file: %s\n", ent->d_name);
                    resultsFiles.push_back(ent->d_name);
                }
//                if(resultsFiles.size()>10)
//                    break;
            }
            closedir(dir);
        }
        else
        {
            // could not open directory
            perror ("");
            return EXIT_FAILURE;
        }

        int stepSize = (int)(ceil((float)(pixelGridSize)/(float)(parallelProcess)));
        int minRowCount(0);
        int maxRowCount(stepSize-1);
        int minColumnCount(0);
        int maxColumnCount(stepSize-1);

        for (int i = 0; i < (parallelProcess * parallelProcess); i++)
        {
            std::vector<ResultsContainer*> results;

            std::cout << "Parallelising the jobs.  On to run " << i+1 << " out of " << parallelProcess * parallelProcess << std::endl;

//            std::cout << minColumnCount << " " << maxColumnCount << " " << minRowCount << " " << maxRowCount << std::endl;

            if (i % parallelProcess == 0 and i != 0)
            {
                minColumnCount += stepSize;
                maxColumnCount += stepSize;
                minRowCount = 0;
                maxRowCount = stepSize-1;
            }
            else if (i != 0)
            {
                minRowCount += stepSize;
                maxRowCount += stepSize;
            }

//            std::cout << minColumnCount << " " << maxColumnCount << " " << minRowCount << " " << maxRowCount << std::endl;

            for (std::vector<std::string>::iterator it = resultsFiles.begin() ; it != resultsFiles.end(); ++it)
            {
                std::string currentResultsFile = rawDataFileFolder + "/" + *it;
                int fileNumber (it - resultsFiles.begin());
                if (fileNumber % 100 == 0)
                {
                    std::cout << "Currently processing file number " << fileNumber+1 << " out of " << resultsFiles.size() << std::endl;
                }

                std::ifstream myReadFile;
                myReadFile.open(currentResultsFile.c_str());

                // Read the results file - extract header info
                int setNumberFromFile(0), maskSpacingFromFile(0), sequenceFromFile(0), lineCount(0);
                float testPulseAmplitude(0.f);
                std::string chipIDFromFile(""), equalizationFileFromFile("");

                std::string descriptionString, value;
                while (myReadFile >> descriptionString >> value)
                {
                    lineCount++;

                    if (descriptionString.find("SET:") != std::string::npos)
                    {
                        setNumberFromFile = toInt(value);
                    }

                    else if (descriptionString.find("ChipID:") != std::string::npos)
                    {
                        chipIDFromFile = value;
                    }

                    else if (descriptionString.find("Test_Pulse_Amplitude(mV):") != std::string::npos)
                    {
                        testPulseAmplitude = toFloat(value);
                    }

                    else if (descriptionString.find("Mask_Spacing:") != std::string::npos)
                    {
                        maskSpacingFromFile = toInt(value);
                    }

                    else if (descriptionString.find("Sequence:") != std::string::npos)
                    {
                        sequenceFromFile = toInt(value);
                    }

                    else if (descriptionString.find("EqualizationFile:") != std::string::npos)
                    {
                        equalizationFileFromFile = value;
                    }

                    if (lineCount>6)
                    {
                        break;
                    }
                }

                // Read the results file - extract data
                int currentToT(0);
                int *array = new int[pixelGridSize*pixelGridSize];
                int postion = 0;

                while (myReadFile >> currentToT)
                {
                    if (lineCount>6)
                    {
                        array[postion] = currentToT;
                        postion++;
                    }
                }

                for (int x = 0; x < pixelGridSize*pixelGridSize; x++)
                {
                    int row = x % pixelGridSize;
                    int column = floor(x / pixelGridSize);

                    if (row%maskSpacing == sequenceFromFile%maskSpacing and column%maskSpacing == sequenceFromFile/maskSpacing and minRowCount <= row and row <= maxRowCount and minColumnCount <= row and column <= maxColumnCount)
                    {
                        // Define container
                        ResultsContainer *pResultsContainer = new ResultsContainer();
                        pResultsContainer->SetSetNumber(setNumberFromFile);
                        pResultsContainer->SetMaskSpacing(maskSpacingFromFile);
                        pResultsContainer->SetChipID(chipIDFromFile);
                        pResultsContainer->SetEqualizationFile(equalizationFileFromFile);
                        pResultsContainer->SetSeqNumber(sequenceFromFile);
                        pResultsContainer->SetX(row);
                        pResultsContainer->SetY(column);
                        pResultsContainer->SetIdentifier();

// Testing making a new container for every file.  Each continer goes into root file branch and can merge later.

                        // Check if container exists and if not make a new one
/*                        bool containerExists(false);

                        for (std::vector<ResultsContainer*>::iterator it = results.begin(); it != results.end(); ++it)
                        {
                            ResultsContainer *pPosResultsContainer = *it;
                            std::string possibleContainerIdentifier = pPosResultsContainer->GetIdentifier();

                            if (pResultsContainer->GetIdentifier() == possibleContainerIdentifier)
                            {
                                pPosResultsContainer->AddToT(testPulseAmplitude,array[x]);
                                containerExists = true;
                            }
                        }

                        if (!containerExists)
                        {
*/
                        pResultsContainer->AddToT(testPulseAmplitude,array[x]);
                        results.push_back(pResultsContainer);

//                        }
                    }
                }
            }
            std::string fileNameChain(fileName);
            fileNameChain.erase(fileNameChain.end() - 5, fileNameChain.end());
            fileNameChain = rootFileFolder + "/" + fileNameChain + "_Seq" + sequenceNumberToRead + "_TestPulse" + voltagePulseToRead + "mV_SerialNumber" + toString(i) + ".root";
            saveResultsContainer(fileNameChain,results);
            results.clear();
        }
    }

    if (mergeResults)
    {
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(rootFileFolder.c_str())) != NULL)
        {
            // Load all the files and directories within directory  
            while ((ent = readdir(dir)) != NULL)
            {
                //printf ("Loading results root file : %s\n", ent->d_name);
                std::string potentialFileName = ent->d_name;
                if(potentialFileName.find(".root") != std::string::npos and potentialFileName.find("Merge") == std::string::npos)
                {
                    const std::string fileNameToLoad = rootFileFolder + "/" + potentialFileName;
                    printf ("Marging results root file : %s\n", fileNameToLoad.c_str());
                    mergeResultsContainer(fileNameToLoad);
                }
            }
            closedir(dir);
        }
        else
        {
            // could not open directory
            perror ("");
            return EXIT_FAILURE;
        }
    }

    if(analyseResults)
    {
        std::vector<ResultsContainer*> loadedResults;
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(rootFileFolder.c_str())) != NULL)
        {
            // Load all the files and directories within directory  
            while ((ent = readdir(dir)) != NULL)
            {
                std::string potentialFileName = ent->d_name;
                if(potentialFileName.find(".root") != std::string::npos and potentialFileName.find("Merge") != std::string::npos)
                {
                    const std::string fileNameToLoad = rootFileFolder + "/" + potentialFileName;
                    printf ("Loading results root file : %s\n", fileNameToLoad.c_str());
                    loadResultsContainer(fileNameToLoad,loadedResults);
                }
            }
            closedir(dir);
        }
        else
        {
            perror ("");
            return EXIT_FAILURE;
        }

        std::vector<ResultsContainer*> selectedResults;

        for (int i = 0; i < loadedResults.size(); i++)
        {
            ResultsContainer *pResultsContainer = loadedResults.at(i);

            if (pResultsContainer->GetX() % 2 == 0)
            {
                selectedResults.push_back(pResultsContainer);
            }
        }

        GroupedContainer *pGroupedContainer = new GroupedContainer(selectedResults, "EvenColumns", bulkPlots);
        pGroupedContainer->PlotMeanToT();
        pGroupedContainer->PlotVarianceToT();
        pGroupedContainer->Fit();

        delete pGroupedContainer;

        selectedResults.clear();

        for (int i = 0; i < loadedResults.size(); i++)
        {
            ResultsContainer *pResultsContainer = loadedResults.at(i);

            if (pResultsContainer->GetX() % 2 != 0)
            {
                selectedResults.push_back(pResultsContainer);
            }
        }

        GroupedContainer *pGroupedContainer2 = new GroupedContainer(selectedResults, "OddColumns", bulkPlots);
        pGroupedContainer2->PlotMeanToT();
        pGroupedContainer2->PlotVarianceToT();
        pGroupedContainer2->Fit();

        delete pGroupedContainer2;

        MaskContainer *pMaskContainer = new MaskContainer(maskFile,pixelGridSize, interestingPlots);
        pMaskContainer->Plot();

        std::list<std::pair<int, int> > maskedPixels = pMaskContainer->GetMaskedPixels();
 
        std::vector<std::pair<int, int> > sharpRisePixels;
        std::vector<std::pair<int, int> > noisyPixels;

        TH2F *pTH2F_EvenColumn = new TH2F("TH2FEvenColumnResponse","Even Column Response - CLICpix HV-CMOS",91,0,182,16,0,16);
        TH2F *pTH2F_OddColumn = new TH2F("TH2FOddColumnResponse","Odd Column Response - CLICpix HV-CMOS",91,0,182,16,0,16);
        TH2F *pTH2F_SharpRisePixels = new TH2F("TH2FSharpRisePixels","Sharp Rise Pixels",64,0,64,64,0,64);
        TH2F *pTH2F_SharpRisePixelsMasked = new TH2F("TH2FSharpRisePixelsMasked","Sharp Rise Pixels With Mask Applied",64,0,64,64,0,64);
        TH2F *pTH2F_NoisyPixels = new TH2F("TH2FNoisyPixels","Noisy Pixels",64,0,64,64,0,64);
        TH2F *pTH2F_NoisyPixelsMasked = new TH2F("TH2FNoisyPixelsMasked","Noisy Pixels With Mask Applied",64,0,64,64,0,64);

        for (int i = 0; i < loadedResults.size(); i++)
        {
            ResultsContainer *pResultsContainer = loadedResults.at(i);
            pResultsContainer->Analyse();

            std::vector<double> testPulses = pResultsContainer->GetTestPulses();
            std::vector<double> averageToT = pResultsContainer->GetAverageToT();

            for (std::vector<double>::iterator it = testPulses.begin(); it != testPulses.end(); ++it)
            {
                int index = it - testPulses.begin();
                double currentTestPulse = *it;
                double currnetAverageToT = averageToT.at(index);

                if (currnetAverageToT > 10 and currentTestPulse < 60)
                {
                    std::pair<int,int> pixelPosition = std::make_pair(pResultsContainer->GetX(), pResultsContainer->GetY());

                    pTH2F_SharpRisePixels->Fill(pResultsContainer->GetX(), pResultsContainer->GetY());

                    if (std::find(maskedPixels.begin(), maskedPixels.end(), pixelPosition) == maskedPixels.end())
                    {
                        pTH2F_SharpRisePixelsMasked->Fill(pResultsContainer->GetX(), pResultsContainer->GetY());
                    }

                    if(std::find(sharpRisePixels.begin(), sharpRisePixels.end(), pixelPosition) != sharpRisePixels.end())
                    {
                        sharpRisePixels.push_back(pixelPosition);
                    }
                }

                if (currnetAverageToT < 5 and currentTestPulse > 80)
                {
                    std::pair<int,int> pixelPosition = std::make_pair(pResultsContainer->GetX(), pResultsContainer->GetY());

                    pTH2F_NoisyPixels->Fill(pResultsContainer->GetX(), pResultsContainer->GetY());

                    if (std::find(maskedPixels.begin(), maskedPixels.end(), pixelPosition) == maskedPixels.end())
                    {
                        pTH2F_NoisyPixelsMasked->Fill(pResultsContainer->GetX(), pResultsContainer->GetY());
                    }

                    if(std::find(noisyPixels.begin(), noisyPixels.end(), pixelPosition) != noisyPixels.end())
                    {
                        noisyPixels.push_back(pixelPosition);
                    }
                }
            }

            std::pair<int,int> pixelPosition = std::make_pair(pResultsContainer->GetX(), pResultsContainer->GetY());

            if (std::find(maskedPixels.begin(), maskedPixels.end(), pixelPosition) == maskedPixels.end())
            {
                if (pResultsContainer->GetX() % 2 != 0)
                {
//                selectedResults.push_back(pResultsContainer);
                    for (int j = 0; j < testPulses.size(); j++)
                    {
                        pTH2F_EvenColumn->Fill(testPulses.at(j), averageToT.at(j),(2/4096.f));
                    }
                }

                else
                {
                    for (int j = 0; j < testPulses.size(); j++)
                    {
                        pTH2F_OddColumn->Fill(testPulses.at(j), averageToT.at(j),(2/4096.f));
                    }
                }
            }
        }

        TCanvas *pCanvasEvenColumn = new TCanvas();
        pCanvasEvenColumn->cd();
        //pTH2F_EvenColumn->SetMinimum(200);
        //pTH2F_EvenColumn->SetMaximum(2500);
        pTH2F_EvenColumn->Draw("COLZ");
        std::string evenPixStringPng = interestingPlots + "OddColumnResponse.png";
        std::string evenPixStringPdf = interestingPlots + "OddColumnResponse.pdf";
        std::string evenPixStringDotC = interestingPlots + "OddColumnResponse.C";
        pCanvasEvenColumn->SaveAs(evenPixStringPng.c_str());
        pCanvasEvenColumn->SaveAs(evenPixStringPdf.c_str());
        pCanvasEvenColumn->SaveAs(evenPixStringDotC.c_str());

        TCanvas *pCanvasOddColumn = new TCanvas();
        pCanvasOddColumn->cd();
        //pTH2F_OddColumn->SetMinimum(200);
        pTH2F_OddColumn->Draw("COLZ");
        std::string oddPixStringPng1 = interestingPlots + "EvenColumnResponse.png";
        std::string oddPixStringPdf1 = interestingPlots + "EvenColumnResponse.pdf";
        std::string oddPixStringDotC1 = interestingPlots + "EvenColumnResponse.C";
        pCanvasOddColumn->SaveAs(oddPixStringPng1.c_str());
        pCanvasOddColumn->SaveAs(oddPixStringPdf1.c_str());
        pCanvasOddColumn->SaveAs(oddPixStringDotC1.c_str());

        TCanvas *pCanvasSharpRisePixels = new TCanvas();
        pCanvasSharpRisePixels->cd();
        pTH2F_SharpRisePixels->Draw("COLZ");
        std::string sharpRisePixelsPng = interestingPlots + "SharpRisePixels.png";
        std::string sharpRisePixelsPdf = interestingPlots + "SharpRisePixels.pdf";
        std::string sharpRisePixelsDotC = interestingPlots + "SharpRisePixels.C";
        pCanvasSharpRisePixels->SaveAs(sharpRisePixelsPng.c_str());
        pCanvasSharpRisePixels->SaveAs(sharpRisePixelsPdf.c_str());
        pCanvasSharpRisePixels->SaveAs(sharpRisePixelsDotC.c_str());

        TCanvas *pCanvasSharpRisePixelsMasked = new TCanvas();
        pCanvasSharpRisePixelsMasked->cd();
        pTH2F_SharpRisePixelsMasked->Draw("COLZ");
        std::string sharpRisePixelsMaskedPng = interestingPlots + "SharpRisePixelsMasked.png";
        std::string sharpRisePixelsMaskedPdf = interestingPlots + "SharpRisePixelsMasked.pdf";
        std::string sharpRisePixelsMaskedDotC = interestingPlots + "SharpRisePixelsMasked.C";
        pCanvasSharpRisePixelsMasked->SaveAs(sharpRisePixelsMaskedPng.c_str());
        pCanvasSharpRisePixelsMasked->SaveAs(sharpRisePixelsMaskedPdf.c_str());
        pCanvasSharpRisePixelsMasked->SaveAs(sharpRisePixelsMaskedDotC.c_str());

        TCanvas *pCanvasNoisyPixels = new TCanvas();
        pCanvasNoisyPixels->cd();
        pTH2F_NoisyPixels->Draw("COLZ");
        std::string noisyPixelsPng = interestingPlots + "NoisyPixels.png";
        std::string noisyPixelsPdf = interestingPlots + "NoisyPixels.pdf";
        std::string noisyPixelsDotC = interestingPlots + "NoisyPixels.C";
        pCanvasNoisyPixels->SaveAs(noisyPixelsPng.c_str());
        pCanvasNoisyPixels->SaveAs(noisyPixelsPdf.c_str());
        pCanvasNoisyPixels->SaveAs(noisyPixelsDotC.c_str());

        TCanvas *pCanvasNoisyPixelsMasked = new TCanvas();
        pCanvasNoisyPixelsMasked->cd();
        pTH2F_NoisyPixelsMasked->Draw("COLZ");
        std::string noisyPixelsMaskedPng = interestingPlots + "NoisyPixelsMasked.png";
        std::string noisyPixelsMaskedPdf = interestingPlots + "NoisyPixelsMasked.pdf";
        std::string noisyPixelsMaskedDotC = interestingPlots + "NoisyPixelsMasked.C";
        pCanvasNoisyPixelsMasked->SaveAs(noisyPixelsMaskedPng.c_str());
        pCanvasNoisyPixelsMasked->SaveAs(noisyPixelsMaskedPdf.c_str());
        pCanvasNoisyPixelsMasked->SaveAs(noisyPixelsMaskedDotC.c_str());

        std::string plotTitleA = "Fit Param A - CLICpix HV-CMOS Set " + toString(setNumber); 
        std::string plotTitleB = "Fit Param B - CLICpix HV-CMOS Set " + toString(setNumber); 
        std::string plotTitleC = "Fit Param C - CLICpix HV-CMOS Set " + toString(setNumber); 
        std::string plotTitleT = "Fit Param T - CLICpix HV-CMOS Set " + toString(setNumber); 

        TH2F *pTH2F_FitParamA = new TH2F("TH2FFitParamA",plotTitleA.c_str(),pixelGridSize,0,(float)(pixelGridSize),pixelGridSize,0,(float)(pixelGridSize));
        TH2F *pTH2F_FitParamB = new TH2F("TH2FFitParamB",plotTitleB.c_str(),pixelGridSize,0,(float)(pixelGridSize),pixelGridSize,0,(float)(pixelGridSize));
        TH2F *pTH2F_FitParamC = new TH2F("TH2FFitParamC",plotTitleC.c_str(),pixelGridSize,0,(float)(pixelGridSize),pixelGridSize,0,(float)(pixelGridSize));
        TH2F *pTH2F_FitParamT = new TH2F("TH2FFitParamT",plotTitleT.c_str(),pixelGridSize,0,(float)(pixelGridSize),pixelGridSize,0,(float)(pixelGridSize));

        std::vector<std::vector<ResultsContainer *> >separatePixelResultsContainer;
        std::vector<std::pair<int,int> > savedPixelsList;

        for (int i = 0; i < loadedResults.size(); i++)
        {
            ResultsContainer *pResultsContainer = loadedResults.at(i);
            std::pair<int,int> pixelPair = std::make_pair(pResultsContainer->GetX(),pResultsContainer->GetY());
            std::vector<std::pair<int,int> >::iterator it = std::find(savedPixelsList.begin(), savedPixelsList.end(), pixelPair);
            bool pixelFound = (it != savedPixelsList.end());
            if (pixelFound)
            {
                int position = it - savedPixelsList.begin();
                separatePixelResultsContainer.at(position).push_back(pResultsContainer);
            }
            else
            {
                std::vector<ResultsContainer *> resultsContainerVectorSeed;
                resultsContainerVectorSeed.push_back(pResultsContainer);
                separatePixelResultsContainer.push_back(resultsContainerVectorSeed);
                savedPixelsList.push_back(pixelPair);
            }
        }

        for (int i = 0; i < separatePixelResultsContainer.size(); i++)
        {
            std::vector<ResultsContainer *> pixelListToGroup = separatePixelResultsContainer.at(i);
            std::pair<int,int> pixelPair = savedPixelsList.at(i);
            int x = pixelPair.first;
            int y = pixelPair.second;
            std::cout << "Currently grouping together data for pixel (" << toString(x) << "," << toString(y) << ")." << std::endl;
            std::string pixelName = "Pixel_(" + toString(x) + "," + toString(y) + ")";
            GroupedContainer *pGroupedContainer = new GroupedContainer(pixelListToGroup,pixelName,bulkPlots);
            pGroupedContainer->PlotMeanToT();
            pGroupedContainer->PlotVarianceToT();
            pGroupedContainer->Fit();

            pTH2F_FitParamA->Fill(x,y,pGroupedContainer->GetFitA());
            pTH2F_FitParamB->Fill(x,y,pGroupedContainer->GetFitB());
            pTH2F_FitParamC->Fill(x,y,pGroupedContainer->GetFitC());
            pTH2F_FitParamT->Fill(x,y,pGroupedContainer->GetFitT());
        }

        TCanvas *pCanvasA = new TCanvas();
        pCanvasA->cd();
        pTH2F_FitParamA->Draw("COLZ");
        std::string fitParamAStringPng = interestingPlots + "FitParamA_Set" + toString(setNumber) + ".png";
        std::string fitParamAStringPdf = interestingPlots + "FitParamA_Set" + toString(setNumber) + ".pdf";
        std::string fitParamAStringDotC = interestingPlots + "FitParamA_Set" + toString(setNumber) + ".C";
        pCanvasA->SaveAs(fitParamAStringPng.c_str());
        pCanvasA->SaveAs(fitParamAStringPdf.c_str());
        pCanvasA->SaveAs(fitParamAStringDotC.c_str());

        TCanvas *pCanvasB = new TCanvas();
        pCanvasB->cd();
        pTH2F_FitParamB->Draw("COLZ");
        std::string fitParamBStringPng = interestingPlots + "FitParamB_Set" + toString(setNumber) + ".png";
        std::string fitParamBStringPdf = interestingPlots + "FitParamB_Set" + toString(setNumber) + ".pdf";
        std::string fitParamBStringDotC = interestingPlots + "FitParamB_Set" + toString(setNumber) + ".C";
        pCanvasB->SaveAs(fitParamBStringPng.c_str());
        pCanvasB->SaveAs(fitParamBStringPdf.c_str());
        pCanvasB->SaveAs(fitParamBStringDotC.c_str());

        TCanvas *pCanvasC = new TCanvas();
        pCanvasC->cd();
        pTH2F_FitParamC->Draw("COLZ");
        std::string fitParamCStringPng = interestingPlots + "FitParamC_Set" + toString(setNumber) + ".png";
        std::string fitParamCStringPdf = interestingPlots + "FitParamC_Set" + toString(setNumber) + ".pdf";
        std::string fitParamCStringDotC = interestingPlots + "FitParamC_Set" + toString(setNumber) + ".C";
        pCanvasC->SaveAs(fitParamCStringPng.c_str());
        pCanvasC->SaveAs(fitParamCStringPdf.c_str());
        pCanvasC->SaveAs(fitParamCStringDotC.c_str());

        TCanvas *pCanvasT = new TCanvas();
        pCanvasT->cd();
        pTH2F_FitParamT->Draw("COLZ");
        std::string fitParamTStringPng = interestingPlots + "FitParamT_Set" + toString(setNumber) + ".png";
        std::string fitParamTStringPdf = interestingPlots + "FitParamT_Set" + toString(setNumber) + ".pdf";
        std::string fitParamTStringDotC = interestingPlots + "FitParamT_Set" + toString(setNumber) + ".C";
        pCanvasT->SaveAs(fitParamTStringPng.c_str());
        pCanvasT->SaveAs(fitParamTStringPdf.c_str());
        pCanvasT->SaveAs(fitParamTStringDotC.c_str());
    }
}

//============================================================

void mergeResultsContainer(std::string fileName)
{
    // Start
    int setNumber;
    int maskSpacing;
    int numberOfSeq;
    int seqNumber;
    int x;
    int y;
    std::string *chipID = NULL;
    std::string *equalizationFile = NULL;
    std::string *identifier = NULL;
    std::vector<double> *testPulses = NULL;
    std::vector<double> *averageToT = NULL;
    std::vector<double> *varianceToT = NULL;
    std::vector<std::vector<int> > *tots = NULL;

    TFile *pTFile = new TFile(fileName.c_str(),"READ");
    if (!pTFile) 
    {
       std::cout << "Problem loading root file : " << fileName << std::endl; 
    }

    TTree *pTTree = (TTree*)pTFile->Get("SavedContainersTree");

    pTTree->SetDirectory(pTFile);

    TBranch *pTBranchSetNumer = NULL;
    TBranch *pTBranchMaskSpacing = NULL;
    TBranch *pTBranchNumberOfSeq = NULL;
    TBranch *pTBranchSeqNumber = NULL;
    TBranch *pTBranchX = NULL;
    TBranch *pTBranchY = NULL;
    TBranch *pTBranchChipID = NULL;
    TBranch *pTBranchEqualizationFile = NULL;
    TBranch *pTBranchIdentifier = NULL;
    TBranch *pTBranchTestPulses = NULL;
    TBranch *pTBranchAverageToT = NULL;
    TBranch *pTBranchVarianceToT = NULL;
    TBranch *pTBranchVectorInt = NULL;

    pTTree->SetBranchAddress("setNumber", &setNumber, &pTBranchSetNumer);
    pTTree->SetBranchAddress("maskSpacing", &maskSpacing, &pTBranchMaskSpacing);
    pTTree->SetBranchAddress("numberOfSeq", &numberOfSeq, &pTBranchNumberOfSeq);
    pTTree->SetBranchAddress("seqNumber", &numberOfSeq, &pTBranchSeqNumber);
    pTTree->SetBranchAddress("pixelX", &x, &pTBranchX);
    pTTree->SetBranchAddress("pixelY", &y, &pTBranchY);
    pTTree->SetBranchAddress("chipID", &chipID, &pTBranchChipID);
    pTTree->SetBranchAddress("equalizationFile", &equalizationFile, &pTBranchEqualizationFile);
    pTTree->SetBranchAddress("identifier", &identifier, &pTBranchIdentifier);
    pTTree->SetBranchAddress("testPulses", &testPulses, &pTBranchTestPulses);
    pTTree->SetBranchAddress("averageToT", &averageToT, &pTBranchAverageToT);
    pTTree->SetBranchAddress("varianceToT", &varianceToT, &pTBranchVarianceToT);
    pTTree->SetBranchAddress("tots", &tots, &pTBranchVectorInt);

    int nEntries = pTTree->GetEntries();
    //pTTree->Print(); 

    std::map<std::string,ResultsContainer*> savedContainerMap;

    std::vector<ResultsContainer*> savedContainers;
    std::vector<int> savedContainerEventNumber;
    std::vector<std::string> savedContainerIdentifier;

    for (int i = 0; i < nEntries; i++) 
    {
        pTBranchSetNumer->GetEntry(i);
        pTBranchMaskSpacing->GetEntry(i);
        pTBranchNumberOfSeq->GetEntry(i);
        pTBranchSeqNumber->GetEntry(i);
        pTBranchX->GetEntry(i);
        pTBranchY->GetEntry(i);
        pTBranchChipID->GetEntry(i);
        pTBranchEqualizationFile->GetEntry(i);
        pTBranchIdentifier->GetEntry(i);
        pTBranchTestPulses->GetEntry(i);
        pTBranchAverageToT->GetEntry(i);
        pTBranchVarianceToT->GetEntry(i);
        pTBranchVectorInt->GetEntry(i);

        if ( std::find(savedContainerIdentifier.begin(),savedContainerIdentifier.end(),identifier->c_str()) == savedContainerIdentifier.end() ) 
        {
            // not found
            ResultsContainer *pResultsContainer = new ResultsContainer();
            pResultsContainer->SetSetNumber(setNumber);
            pResultsContainer->SetMaskSpacing(maskSpacing);
            pResultsContainer->SetSeqNumber(seqNumber);
            pResultsContainer->SetX(x);
            pResultsContainer->SetY(y);
            pResultsContainer->SetChipID(*chipID);
            pResultsContainer->SetEqualizationFile(*equalizationFile);
            pResultsContainer->SetIdentifier();
            pResultsContainer->SetTestPulses(*testPulses);
            pResultsContainer->SetAverageToT(*averageToT);
            pResultsContainer->SetVarianceToT(*varianceToT);
            pResultsContainer->SetToTs(*tots);
            savedContainers.push_back(pResultsContainer);
            savedContainerIdentifier.push_back(*identifier);
            savedContainerEventNumber.push_back(i);
        }
        else 
        {
            // found
            std::vector<std::string>::iterator it = find(savedContainerIdentifier.begin(),savedContainerIdentifier.end(),identifier->c_str());
            int position = it - savedContainerIdentifier.begin();
            ResultsContainer *pResultsContainer = savedContainers.at(position);
            pResultsContainer->AddToTVector(tots);
        }
    }
    std::string fileNameChain(fileName);
    fileNameChain.erase(fileNameChain.end() - 5, fileNameChain.end());
    fileNameChain =  fileNameChain + "_Merged.root";
    std::cout << "Saving file : " << fileNameChain << std::endl;
    saveResultsContainer(fileNameChain,savedContainers);
    pTFile->cd();
    pTFile->Close();
    delete pTFile, pTTree;
}

//============================================================

void saveResultsContainer(std::string fileName, std::vector<ResultsContainer*> resultsContainers)
{
    // Start
    TFile *pTFile = new TFile(fileName.c_str(), "RECREATE");
    TTree *pTTree = new TTree("SavedContainersTree", "SavedContainersTree");

    int setNumber;
    int maskSpacing;
    int numberOfSeq;
    int seqNumber;
    int x;
    int y;
    std::string chipID;
    std::string equalizationFile;
    std::string identifier;
    std::vector<double> testPulses;
    std::vector<double> averageToT;
    std::vector<double> varianceToT;
    std::vector<std::vector<int> > tots;

    pTTree->SetDirectory(pTFile);
    pTTree->Branch("setNumber", &setNumber, "setNumber/I");
    pTTree->Branch("maskSpacing", &maskSpacing, "maskSpacing/I");
    pTTree->Branch("numberOfSeq", &numberOfSeq, "numberOfSeq/I");
    pTTree->Branch("seqNumber", &seqNumber, "seqNumber/I");
    pTTree->Branch("pixelX", &x, "pixelX/I");
    pTTree->Branch("pixelY", &y, "pixelY/I");
    pTTree->Branch("chipID", &chipID);
    pTTree->Branch("equalizationFile", &equalizationFile);
    pTTree->Branch("identifier", &identifier);
    pTTree->Branch("testPulses", &testPulses);
    pTTree->Branch("averageToT", &averageToT);
    pTTree->Branch("varianceToT", &varianceToT);
    pTTree->Branch("tots", &tots);

    // Fill
    for (std::vector<ResultsContainer*>::iterator it = resultsContainers.begin(); it != resultsContainers.end(); ++it)
    {
        ResultsContainer *pResultsContainer = *it;
        setNumber = pResultsContainer->GetSetNumber();
        maskSpacing = pResultsContainer->GetMaskSpacing();
        numberOfSeq = pResultsContainer->GetNumberOfSeq();
        seqNumber = pResultsContainer->GetSeqNumber();
        x = pResultsContainer->GetX();
        y = pResultsContainer->GetY();
        chipID = pResultsContainer->GetChipID();
        equalizationFile = pResultsContainer->GetEqualizationFile();
        identifier = pResultsContainer->GetIdentifier();
        testPulses = pResultsContainer->GetTestPulses();
        averageToT = pResultsContainer->GetAverageToT();
        varianceToT = pResultsContainer->GetVarianceToT();
        tots = pResultsContainer->GetToTs();
        pTTree->Fill();
        delete pResultsContainer;
    }

    // End
    pTFile->cd();
    pTTree->Write();
    pTFile->Close();
    delete pTFile, pTTree;
    testPulses.clear();
    averageToT.clear();
    varianceToT.clear();
    tots.clear();
}

//============================================================

void loadResultsContainer(std::string fileName, std::vector<ResultsContainer*> &resultsContainers)
{
    std::cout << "Number of containers loaded so far : " << resultsContainers.size() << std::endl;
    int setNumber;
    int maskSpacing;
    int numberOfSeq;
    int seqNumber;
    int x;
    int y;
    std::string *chipID = NULL;
    std::string *equalizationFile = NULL;
    std::string *identifier = NULL;
    std::vector<double> *testPulses = NULL;
    std::vector<double> *averageToT = NULL;
    std::vector<double> *varianceToT = NULL;
    std::vector<std::vector<int> > *tots = NULL;

    TFile *pTFile = new TFile(fileName.c_str(),"READ");
    if (!pTFile) 
    {
       std::cout << "Problem loading root file : " << fileName << std::endl; 
    }

    TTree *pTTree = (TTree*)pTFile->Get("SavedContainersTree");

    pTTree->SetDirectory(pTFile);

    TBranch *pTBranchSetNumer = NULL;
    TBranch *pTBranchMaskSpacing = NULL;
    TBranch *pTBranchNumberOfSeq = NULL;
    TBranch *pTBranchSeqNumber = NULL;
    TBranch *pTBranchX = NULL;
    TBranch *pTBranchY = NULL;
    TBranch *pTBranchChipID = NULL;
    TBranch *pTBranchEqualizationFile = NULL;
    TBranch *pTBranchIdentifier = NULL;
    TBranch *pTBranchTestPulses = NULL;
    TBranch *pTBranchAverageToT = NULL;
    TBranch *pTBranchVarianceToT = NULL;
    TBranch *pTBranchVectorInt = NULL;

    pTTree->SetBranchAddress("setNumber", &setNumber, &pTBranchSetNumer);
    pTTree->SetBranchAddress("maskSpacing", &maskSpacing, &pTBranchMaskSpacing);
    pTTree->SetBranchAddress("numberOfSeq", &numberOfSeq, &pTBranchNumberOfSeq);
    pTTree->SetBranchAddress("seqNumber", &numberOfSeq, &pTBranchSeqNumber);
    pTTree->SetBranchAddress("pixelX", &x, &pTBranchX);
    pTTree->SetBranchAddress("pixelY", &y, &pTBranchY);
    pTTree->SetBranchAddress("chipID", &chipID, &pTBranchChipID);
    pTTree->SetBranchAddress("equalizationFile", &equalizationFile, &pTBranchEqualizationFile);
    pTTree->SetBranchAddress("identifier", &identifier, &pTBranchIdentifier);
    pTTree->SetBranchAddress("testPulses", &testPulses, &pTBranchTestPulses);
    pTTree->SetBranchAddress("averageToT", &averageToT, &pTBranchAverageToT);
    pTTree->SetBranchAddress("varianceToT", &varianceToT, &pTBranchVarianceToT);
    pTTree->SetBranchAddress("tots", &tots, &pTBranchVectorInt);

    int nEntries = pTTree->GetEntries();
    //pTTree->Print(); 

    for (int i = 0; i < nEntries; i++) 
    {
        pTBranchSetNumer->GetEntry(i);
        pTBranchMaskSpacing->GetEntry(i);
        pTBranchNumberOfSeq->GetEntry(i);
        pTBranchSeqNumber->GetEntry(i);
        pTBranchX->GetEntry(i);
        pTBranchY->GetEntry(i);
        pTBranchChipID->GetEntry(i);
        pTBranchEqualizationFile->GetEntry(i);
        pTBranchIdentifier->GetEntry(i);
        pTBranchTestPulses->GetEntry(i);
        pTBranchAverageToT->GetEntry(i);
        pTBranchVarianceToT->GetEntry(i);
        pTBranchVectorInt->GetEntry(i);

        ResultsContainer *pResultsContainer = new ResultsContainer();
        pResultsContainer->SetSetNumber(setNumber);
        pResultsContainer->SetMaskSpacing(maskSpacing);
        pResultsContainer->SetSeqNumber(seqNumber);
        pResultsContainer->SetX(x);
        pResultsContainer->SetY(y);
        pResultsContainer->SetChipID(*chipID);
        pResultsContainer->SetEqualizationFile(*equalizationFile);
        pResultsContainer->SetIdentifier();
        pResultsContainer->SetTestPulses(*testPulses);
        pResultsContainer->SetAverageToT(*averageToT);
        pResultsContainer->SetVarianceToT(*varianceToT);
        pResultsContainer->SetToTs(*tots);
        resultsContainers.push_back(pResultsContainer);
    }
    pTFile->Close();
    delete pTFile, pTTree;
}

//============================================================

std::string toString(int a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

//============================================================

int toInt(std::string s)
{
    std::stringstream ss;
    ss << s;
    int i;
    ss >> i;
    return i;
}

//============================================================

float toFloat(std::string s)
{
    std::stringstream ss;
    ss << s;
    float f;
    ss >> f;
    return f;
}


