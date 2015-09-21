#include "GroupedContainer.h"

//std::string floatToString(float a);
//std::string intToString(int a);

//===========================================

GroupedContainer::GroupedContainer(std::vector<ResultsContainer*> vectorResultsContainer, std::string description, std::string plotLocation) :
    m_SetNumber(0),
    m_MaskSpacing(0),
    m_A(0.f),
    m_B(0.f),
    m_C(0.f),
    m_T(0.f),
    m_ChipID(""),
    m_EqualizationFile(""),
    m_Description(description),
    m_PlotLocation(plotLocation)
{
    for (std::vector<ResultsContainer*>::iterator it = vectorResultsContainer.begin(); it != vectorResultsContainer.end(); ++it)
    {
        ResultsContainer *pResultsContainer = *it;

        if (it == vectorResultsContainer.begin())
        {
            // Set basic parameters from first element in vector
            m_SetNumber = pResultsContainer->GetSetNumber();
            m_MaskSpacing = pResultsContainer->GetMaskSpacing();
            m_ChipID = pResultsContainer->GetChipID();
            m_EqualizationFile = pResultsContainer->GetEqualizationFile();
        }

        else
        {
            // Check other additions are for same sample
            try
            {
                if (m_SetNumber != pResultsContainer->GetSetNumber())
                    throw pResultsContainer->GetSetNumber();
            }
            catch (int e)
            {
                 std::cout << "An exception occurred. Trying to add set number " << e << " to container with set number " << m_SetNumber << std::endl;
            }

            try
            {
                if (m_MaskSpacing != pResultsContainer->GetMaskSpacing())
                    throw pResultsContainer->GetMaskSpacing();
            }
            catch (int e)
            {
                 std::cout << "An exception occurred. Trying to add mask spacing " << e << " to container with mask spacing " << m_MaskSpacing << std::endl;
            }

            try
            {
                if (m_ChipID != pResultsContainer->GetChipID())
                    throw pResultsContainer->GetChipID();
            }
            catch (std::string e)
            {
                 std::cout << "An exception occurred. Trying to add chipid " << e << " to container with chipid " << m_ChipID << std::endl;
            }

            try
            {
                if (m_EqualizationFile != pResultsContainer->GetEqualizationFile())
                    throw pResultsContainer->GetEqualizationFile();
            }
            catch (std::string e)
            {
                 std::cout << "An exception occurred. Trying to add equilization file " << e << " to container with equilization file " << m_EqualizationFile << std::endl;
            }
        }

        // Add pixel to list
        std::pair<int,int> pixelPosition = std::make_pair(pResultsContainer->GetX(),pResultsContainer->GetY());
        m_Pixels.push_back(pixelPosition);

        // Add ToTs
        std::vector<double> testPulses = pResultsContainer->GetTestPulses();
        std::vector<std::vector<int> > allToTs = pResultsContainer->GetToTs();

        for (std::vector<double>::iterator it = testPulses.begin() ; it != testPulses.end(); ++it)
        {
            double currentTestPulse = *it;
            int index = std::distance(testPulses.begin(), it);
            std::vector<int> currentToT = allToTs.at(index);
            std::pair<float,std::vector<int> > resultsPair = std::make_pair(currentTestPulse,currentToT);
            m_TestPulseAndToTs.push_back(resultsPair);
        }
    }
    this->Group();
    this->Analyse();
}

//===========================================

GroupedContainer::~GroupedContainer()
{
}

//===========================================

void GroupedContainer::Print()
{
    for (std::list<std::pair<int,int> >::iterator it = m_Pixels.begin(); it != m_Pixels.end(); ++it)
    {
        std::pair<int,int> currentPair = *it;
        int x = currentPair.first;
        int y = currentPair.second;
        std::cout << "Pixel : (" << x << "," << y << ") in container" << std::endl;
    }
}

//===========================================

void GroupedContainer::Group()
{
    std::list<float> groupedPulses;

    std::vector<float> groupedTestPulses;
    std::vector<std::vector<int> > groupedToTs;

    for (std::list<std::pair<float,std::vector<int> > >::iterator it = m_TestPulseAndToTs.begin(); it!=m_TestPulseAndToTs.end(); ++it)
    {
        std::pair<float,std::vector<int> > currentResult = *it;

        float currentTestPulse = currentResult.first;
        std::vector<int> currentToTList = currentResult.second;

        if (std::find(groupedPulses.begin(), groupedPulses.end(), currentTestPulse) == groupedPulses.end())
        {
            groupedTestPulses.push_back(currentTestPulse);
            groupedToTs.push_back(currentToTList);
            groupedPulses.push_back(currentTestPulse);
        }
        else
        {
            std::vector<float>::iterator iter = std::find(groupedTestPulses.begin(), groupedTestPulses.end(), currentTestPulse);
            int index = std::distance(groupedTestPulses.begin(), iter);

            for(std::vector<int>::const_iterator iter = currentToTList.begin(); iter != currentToTList.end(); ++iter)
            {
                int tot = *iter;
                groupedToTs.at(index).push_back(tot);
            }
        }
    }
    m_GroupedToT = groupedToTs;
    m_GroupedTestPulses = groupedTestPulses;
}

//===========================================

void GroupedContainer::Analyse()
{
    for (std::vector<float>::iterator it = m_GroupedTestPulses.begin() ; it != m_GroupedTestPulses.end(); ++it) 
    {
        float currentTestPulse = *it;
        std::vector<float>::iterator iter = std::find(m_GroupedTestPulses.begin(), m_GroupedTestPulses.end(), currentTestPulse);
        int index = std::distance(m_GroupedTestPulses.begin(), iter);
        std::vector<int> totList = m_GroupedToT.at(index);

        float runningToT(0.f);
        for (std::vector<int>::iterator iter = totList.begin() ; iter != totList.end(); ++iter)
        {
            float currentToT = *iter;
            runningToT += currentToT;
        }
        float averageToT = runningToT / (float)(totList.size());

        m_TestPulses.push_back(currentTestPulse);
        m_MeanToT.push_back(averageToT);

        float sum(0.f);
        for (std::vector<int>::iterator iter = totList.begin() ; iter != totList.end(); ++iter)
        {
            float currentToT = *iter;
            sum += pow(currentToT - averageToT,2);
        }
        m_VarianceToT.push_back(pow(sum / (float)(totList.size()),0.5));
    }            
}

//===========================================

void GroupedContainer::PlotMeanToT()
{
    TVectorT<float> tVectorToT(m_MeanToT.size(), &m_MeanToT[0]);
    TVectorT<float> tVectorPulseHeight(m_TestPulses.size(), &m_TestPulses[0]);

    TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToT);
    pTGraph->SetLineColor(2);
    pTGraph->SetMarkerColor(2);
    pTGraph->SetMarkerStyle(20);
    pTGraph->SetTitle("Pulse Height ToT Test Pulse");
    pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
    pTGraph->GetXaxis()->SetLimits(0.0,185.0);
    pTGraph->GetXaxis()->SetRangeUser(0.0,185.0);
    pTGraph->GetYaxis()->SetTitle("ToT");
    pTGraph->GetYaxis()->SetLimits(0.0,16.0);
    pTGraph->GetYaxis()->SetRangeUser(0.0,16.0);

    TCanvas *pCanvas = new TCanvas("PulseHeightToTTestPulse","Pulse Height ToT Test Pulse",200,10,3000,2500);

    std::string pngFileName = m_PlotLocation + "Set_" + IntToString(m_SetNumber) + "_ChipID_" + m_ChipID + "_" + m_Description + ".png";
//    std::string dotCFileName = m_PlotLocation + "Set_" + IntToString(m_SetNumber) + "_ChipID_" + m_ChipID + "_" + m_Description + ".C";

    pCanvas->cd();
    pTGraph->Draw("AP");
    pCanvas->SaveAs(pngFileName.c_str());
//    pCanvas->SaveAs(dotCFileName.c_str());
}

//===========================================

void GroupedContainer::PlotVarianceToT()
{
    TVectorT<float> tVectorToTVar(m_VarianceToT.size(), &m_VarianceToT[0]);
    TVectorT<float> tVectorPulseHeight(m_TestPulses.size(), &m_TestPulses[0]);

    TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToTVar);
    pTGraph->SetLineColor(2);
    pTGraph->SetMarkerColor(2);
    pTGraph->SetMarkerStyle(20);
    pTGraph->SetTitle("Pulse Height ToT Test Pulse");
    pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
    pTGraph->GetXaxis()->SetLimits(0.0,185.0);
    pTGraph->GetXaxis()->SetRangeUser(0.0,185.0);
    pTGraph->GetYaxis()->SetTitle("Variance of ToT");
//    pTGraph->GetYaxis()->SetLimits(0.0,16.0);
//    pTGraph->GetYaxis()->SetRangeUser(0.0,16.0);

    TCanvas *pCanvas = new TCanvas("PulseHeightToTTestPulse","Pulse Height ToT Test Pulse",200,10,3000,2500);

    std::string pngFileName = m_PlotLocation + "ToTVariance_PulseHeight_Set_" + IntToString(m_SetNumber) + "_ChipID_" + m_ChipID + "_" + m_Description + ".png";
//    std::string dotCFileName = m_PlotLocation + "ToTVariance_PulseHeight_Set_" + IntToString(m_SetNumber) + "_ChipID_" + m_ChipID + "_" + m_Description + ".C";

    pCanvas->cd();
    pTGraph->Draw("AP");
    pCanvas->SaveAs(pngFileName.c_str());
//    pCanvas->SaveAs(dotCFileName.c_str());
}

//===========================================

void GroupedContainer::Fit()
{
    float lowerLimitFit(0.f);
    float lowerLimitMeanToT(std::numeric_limits<float>::max());
    float upperLimitFit(180.f);

    // Set limits on tot fit
    for (std::vector<float>::iterator iter = m_MeanToT.begin() ; iter != m_MeanToT.end(); ++iter)
    {
        int position(iter - m_MeanToT.begin());
        float meanToT = *iter;
        if (meanToT > 1.0 && lowerLimitMeanToT > meanToT)
        {
            lowerLimitFit = m_TestPulses.at(position);
            lowerLimitMeanToT = meanToT;
        }
    }

    TF1 *pTF1 = new TF1("myfit","([0]*x) + [1] - ([2] / (x - [3]))", 0, 185);
    pTF1->SetParLimits(0,0,0.3);
    pTF1->SetParLimits(1,0,50);
    pTF1->SetParLimits(2,0,1000);
    //pTF1->SetParLimits(3,lowerLimitFit*0.5,lowerLimitFit);
    pTF1->SetParLimits(3,0,lowerLimitFit);

    TVectorT<float> tVectorToT(m_MeanToT.size(), &m_MeanToT[0]);
    TVectorT<float> tVectorPulseHeight(m_TestPulses.size(), &m_TestPulses[0]);

    // Check to see if there is saturation as this can be used to constrain the fit.  
    // Also check range to see if pixel is just producing noise.
    int maxToTCounter(0);

    for (std::vector<float>::iterator iter = m_MeanToT.begin() ; iter != m_MeanToT.end(); ++iter)
    {
        float meanToT =  *iter;
        int index = iter - m_MeanToT.begin();
        //std::cout << "For index " << index << " the mean ToT is " << meanToT << std::endl;

        if (std::fabs(meanToT-15) < 0.25)
        {
            int index = iter - m_MeanToT.begin();
            if (upperLimitFit > m_TestPulses.at(index))
            {
                upperLimitFit = m_TestPulses.at(index);
                //std::cout << "Fit upper limit : " << upperLimitFit << std::endl;
            }
            maxToTCounter++;
        }
        //std::cout << "The number of max ToTs is " << maxToTCounter << std::endl;
    }

    TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToT);
    pTGraph->SetLineColor(kBlack);
    pTGraph->SetMarkerColor(kBlack);
    pTGraph->SetMarkerStyle(20);
    pTGraph->SetTitle("Pulse Height ToT Test Pulse");
    pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
    pTGraph->GetXaxis()->SetLimits(0.0,185.0);
    pTGraph->GetXaxis()->SetRangeUser(0.0,185.0);
    pTGraph->GetYaxis()->SetTitle("ToT");
    pTGraph->GetYaxis()->SetLimits(0.0,16.0);
    pTGraph->GetYaxis()->SetRangeUser(0.0,16.0);

    pTF1->SetRange(lowerLimitFit,0,upperLimitFit,16);
    pTGraph->Fit(pTF1,"M","",lowerLimitFit,upperLimitFit);
    m_A = pTF1->GetParameter(0);
    m_B = pTF1->GetParameter(1);
    m_C = pTF1->GetParameter(2);
    m_T = pTF1->GetParameter(3);

//    std::cout << "a: " << a << std::endl;
//    std::cout << "b: " << b << std::endl;
//    std::cout << "c: " << c << std::endl;
//    std::cout << "t: " << t << std::endl;

    TCanvas *pCanvas = new TCanvas("PulseHeightToTTestPulse","Pulse Height ToT Test Pulse",200,10,3000,2500);

    TLine *pTLine = new TLine(0.f,1.f,180.f,1.f);
    pTLine->SetLineStyle(2);
    pTLine->SetLineColor(kRed);

    TLine *pTLineUL = new TLine(upperLimitFit,0.f,upperLimitFit,16.f);
    pTLineUL->SetLineStyle(2);
    pTLineUL->SetLineColor(kRed);

    TLine *pTLineLL = new TLine(lowerLimitFit,0.f,lowerLimitFit,16.f);
    pTLineLL->SetLineStyle(2);
    pTLineLL->SetLineColor(kRed);

    pTGraph->Draw("AP");
    pTF1->Draw("same");
    pTLine->Draw("same");
    pTLineUL->Draw("same");
    pTLineLL->Draw("same");

    std::string pngFileName = m_PlotLocation + "ToT_PulseHeight_Set_" + IntToString(m_SetNumber) + "_ChipID_" + m_ChipID + "_" + m_Description + "_Fit.png";
    std::string dotCFileName = m_PlotLocation + "ToT_PulseHeight_Set_" + IntToString(m_SetNumber) + "_ChipID_" + m_ChipID + "_" + m_Description + "_Fit.C";

    pCanvas->SaveAs(pngFileName.c_str());
    pCanvas->SaveAs(dotCFileName.c_str());
}

//===========================================

std::string GroupedContainer::IntToString(int a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

//===========================================

std::string GroupedContainer::FloatToString(float a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}
