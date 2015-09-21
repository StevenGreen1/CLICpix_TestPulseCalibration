#include "ResultsContainer.h"

//===========================================

ResultsContainer::ResultsContainer() :
    m_SetNumber(0),
    m_MaskSpacing(0),
    m_NumberOfSeq(0),
    m_ChipID(""),
    m_EqualizationFile(""),
    m_Identifier("")
{
}

//===========================================

ResultsContainer::~ResultsContainer()
{
}

//===========================================

void ResultsContainer::SetSetNumber(int setNumber)
{
    m_SetNumber = setNumber;
}

//===========================================

void ResultsContainer::SetMaskSpacing(int maskSpacing)
{
    m_MaskSpacing = maskSpacing;
    m_NumberOfSeq = maskSpacing * maskSpacing;
}

//===========================================

void ResultsContainer::SetSeqNumber(int seqNumber)
{
    m_SeqNumber = seqNumber;
}

//===========================================

void ResultsContainer::SetX(int x)
{
    m_X = x;
}

//===========================================

void ResultsContainer::SetY(int y)
{
    m_Y = y;
}

//===========================================

void ResultsContainer::SetChipID(std::string chipID)
{
    m_ChipID = chipID;
}

//===========================================

void ResultsContainer::SetEqualizationFile(std::string equalizationFile)
{
    m_EqualizationFile = equalizationFile;
}

//===========================================

void ResultsContainer::SetIdentifier()
{
    m_Identifier = "SetNumer:" + IntToString(m_SetNumber) + ":MaskSpacing:" + IntToString(m_MaskSpacing) + ":ChipID:" + m_ChipID + ":EqualizationFile:" + m_EqualizationFile + ":PixelX:" + IntToString(m_X) + ":PixelY:" + IntToString(m_Y);
    //m_Identifier = "SetNumer:" + IntToString(m_SetNumber) + ":MaskSpacing:" + IntToString(m_MaskSpacing) + ":ChipID:" + m_ChipID + ":EqualizationFile:" + m_EqualizationFile + ":Seq:" + IntToString(m_SeqNumber) + ":PixelX:" + IntToString(m_X) + ":PixelY:" + IntToString(m_Y);
    //m_Identifier = "SetNumer:" + IntToString(m_SetNumber) + ":MaskSpacing:" + IntToString(m_MaskSpacing) + ":ChipID:" + m_ChipID + ":EqualizationFile:" + m_EqualizationFile + ":TestPulse:" + FloatToString(m_TestPulse) + ":Seq:" + IntToString(m_SeqNumber);
}

//===========================================

void ResultsContainer::SetTestPulses(std::vector<double> testPulses)
{
    m_TestPulses.clear();
    m_TestPulses = testPulses;
}

//===========================================

void ResultsContainer::SetAverageToT(std::vector<double> averageToT)
{
    m_AverageToT.clear();
    m_AverageToT = averageToT;
}

//===========================================

void ResultsContainer::SetVarianceToT(std::vector<double> varianceToT)
{
    m_VarianceToT.clear();
    m_VarianceToT = varianceToT;
}

//===========================================

void ResultsContainer::SetToTs(std::vector<std::vector<int> > tots)
{
    m_ToTs.clear();
    m_ToTs = tots;
}

//===========================================

void ResultsContainer::AddToT(float testPulse, int tot)
{
    bool pulseMesaured(false);

    std::vector<int> emptyVector;

    if (std::find(m_TestPulses.begin(), m_TestPulses.end(), testPulse) == m_TestPulses.end())
    {
        m_TestPulses.push_back(testPulse * 1000);
        m_ToTs.push_back(emptyVector);
    }

    std::vector<double>::iterator iter = std::find(m_TestPulses.begin(), m_TestPulses.end(), testPulse);
    int index = std::distance(m_TestPulses.begin(), iter) - 1;
    m_ToTs.at(index).push_back(tot);
}

//===========================================

void ResultsContainer::AddToTVector(std::vector<std::vector<int> > *tots)
{
    for (int i = 0; i < tots->size(); i++)
    {
        std::vector<int> totsToAdd = tots->at(i);
        for (int j = 0; j < totsToAdd.size(); j++)
        {
            m_ToTs.at(i).push_back(totsToAdd.at(j));
        }
    }
}

//===========================================

void ResultsContainer::Analyse()
{
    for (std::vector<std::vector<int> >::iterator iter = m_ToTs.begin(); iter != m_ToTs.end(); ++iter)
    {
        std::vector<int> totVectorFixedPulseHeight = *iter;

        double runningToT(0.f);
        for (std::vector<int>::iterator it = totVectorFixedPulseHeight.begin() ; it != totVectorFixedPulseHeight.end(); ++it)
        {
            double currentToT = *it;
            runningToT += currentToT;
        }

        double currentAverageToT = runningToT / (float)(totVectorFixedPulseHeight.size());
        m_AverageToT.push_back(currentAverageToT);

        double sum(0.f);
        for (std::vector<int>::iterator it = totVectorFixedPulseHeight.begin() ; it != totVectorFixedPulseHeight.end(); ++it)
        {
            double currentToT = *it;
            sum += pow(currentToT - currentAverageToT,2);
        }
        m_VarianceToT.push_back(pow(sum / (float)(totVectorFixedPulseHeight.size()),0.5));
    }
}

//===========================================

std::string ResultsContainer::IntToString(int a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

//===========================================

std::string ResultsContainer::FloatToString(float a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

//===========================================

