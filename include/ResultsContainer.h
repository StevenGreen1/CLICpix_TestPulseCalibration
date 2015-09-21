#ifndef RESULTS_CONTINER_H
#define RESULTS_CONTINER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

class ResultsContainer
{
    private:
        /*
         * Private Variables
         */
        int m_SetNumber;
        int m_MaskSpacing;
        int m_NumberOfSeq;
        int m_SeqNumber;
        int m_X;
        int m_Y;
        std::string m_ChipID;
        std::string m_EqualizationFile;
        std::string m_Identifier;
        std::vector<double> m_TestPulses;
        std::vector<double> m_AverageToT;
        std::vector<double> m_VarianceToT;
        std::vector<std::vector<int> > m_ToTs;

    public:        
        /*
         * Default Constructor
         */
        ResultsContainer();

        /*
         * Default Destructor
         */
        ~ResultsContainer();

        /*
         * Get Parameter Functions
         */
        int GetSetNumber() {return m_SetNumber;}
        int GetMaskSpacing() {return m_MaskSpacing;}
        int GetNumberOfSeq() {return m_NumberOfSeq;}
        int GetSeqNumber() {return m_SeqNumber;}
        int GetX() {return m_X;}
        int GetY() {return m_Y;}
        std::string GetChipID() {return m_ChipID;}
        std::string GetEqualizationFile() {return m_EqualizationFile;}
        std::string GetIdentifier() {return m_Identifier;}
        std::vector<double> GetTestPulses() {return m_TestPulses;}
        std::vector<double> GetAverageToT() {return m_AverageToT;}
        std::vector<double> GetVarianceToT() {return m_VarianceToT;}
        std::vector<std::vector<int> > GetToTs() {return m_ToTs;}

        /*
         * Set Parameter Functions
         */
        void SetSetNumber(int setNumber);
        void SetMaskSpacing(int maskSpacing);
        void SetSeqNumber(int seqNumber);
        void SetX(int x);
        void SetY(int y);
        void SetChipID(std::string chipID);
        void SetEqualizationFile(std::string equalizationFile);
        void SetIdentifier();
        void SetTestPulses(std::vector<double> testPulses);
        void SetAverageToT(std::vector<double> averageToT);
        void SetVarianceToT(std::vector<double> varianceToT);
        void SetToTs(std::vector<std::vector<int> > tots);
        
        /*
         * Add ToT measurement to container
         */
        void AddToT(float testPulse, int tot);

        /*
         * Analyse ToTs
         */
        void Analyse();

        /*
         * Add ToT Vector To Results Container ToTs
         */ 
        void AddToTVector(std::vector<std::vector<int> > *tots);

        std::string IntToString(int a);
        std::string FloatToString(float a);
};

#endif
