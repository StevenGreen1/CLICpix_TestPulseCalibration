#ifndef GROUPED_CONTINER_H
#define GROUPED_CONTINER_H

#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLine.h"
#include "TVector.h"

#include "ResultsContainer.h"

class GroupedContainer
{
    private:
        /*
         * Private Variables
         */
        int m_SetNumber;
        int m_MaskSpacing;
        float m_A;                                                               // Fit Parameter
        float m_B;                                                               // Fit Parameter
        float m_C;                                                               // Fit Parameter
        float m_T;                                                               // Fit Parameter
        std::string m_ChipID;
        std::string m_EqualizationFile;
        std::string m_Identifier;
        std::list<std::pair<int,int> > m_Pixels;
        std::list<std::pair<float,std::vector<int> > > m_TestPulseAndToTs;
        std::vector<float> m_TestPulses;
        std::vector<float> m_MeanToT;
        std::vector<float> m_VarianceToT;
        std::vector<std::vector<int> > m_GroupedToT;
        std::vector<float> m_GroupedTestPulses;
        std::string m_Description;
        const std::string m_PlotLocation;

    public:        
        /*
         * Default Constructor
         */
        GroupedContainer(std::vector<ResultsContainer*> vectorResultsContainer, std::string description, std::string plotLocation);

        /*
         * Default Destructor
         */
        ~GroupedContainer();

        /*
         * Get Parameter Functions
         */
        int GetSetNumber() {return m_SetNumber;}
        int GetMaskSpacing() {return m_MaskSpacing;}
        float GetFitA() {return m_A;}
        float GetFitB() {return m_B;}
        float GetFitC() {return m_C;}
        float GetFitT() {return m_T;}
        std::string GetChipID() {return m_ChipID;}
        std::string GetEqualizationFile() {return m_EqualizationFile;}
        std::string GetIdentifier() {return m_Identifier;}
        std::list<std::pair<int,int> > GetPixelList() {return m_Pixels;}

        /*
         * Set Parameter Functions
         */
        void SetSetNumber(int setNumber);
        void SetMaskSpacing(int maskSpacing);
        void SetChipID(std::string chipID);
        void SetEqualizationFile(std::string equalizationFile);
        void SetIdentifier();

        /*
         * Print pixels in grouped container
         */
        void Print();

        /*
         * Plot a TGraph of mean ToT vs pulse height for grouped container data
         */
        void PlotMeanToT();

        /*
         * Plot a TGraph of variance ToT vs pulse height for grouped container data
         */
        void PlotVarianceToT();

        /*
         * Group tot data for the separate results containers together based on pulse height 
         */
        void Group();

        /*
         * Analyse ToTs
         */
        void Analyse();

        /*
         * Fit ToT vs pulse height shape
         */
        void Fit();

        std::string IntToString(int a);
        std::string FloatToString(float a);
};

#endif
