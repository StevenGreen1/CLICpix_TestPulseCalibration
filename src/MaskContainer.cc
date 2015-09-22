#include "MaskContainer.h"

//===========================================

MaskContainer::MaskContainer(std::string fileName, int gridSize, std::string plotPath) :
    m_FileName(fileName),
    m_GridSize(gridSize),
    m_PlotPath(plotPath)
{
    std::ifstream myReadFile;
    myReadFile.open(m_FileName.c_str());
    std::string firstLine;
    std::string pixelOrColumn;
    int x;
    int y;
    getline(myReadFile,firstLine);

    while(myReadFile >> pixelOrColumn >> y >> x)
    {
//        std::cout << "pixelOrColumn : " << pixelOrColumn << std::endl;
//        std::cout << "x             : " << x << std::endl;
//        std::cout << "y             : " << y << std::endl;

        if (pixelOrColumn == "p")
        {
            std::pair<int,int> maskedPixel = std::make_pair(x,y);
            m_MaskedPixels.push_back(maskedPixel);
        }
        else if (pixelOrColumn == "c")
        {
            for (int i = 0; i < m_GridSize; i++)
            {
                std::pair<int,int> maskedPixel = std::make_pair<int,int>(x,i);
                m_MaskedPixels.push_back(maskedPixel);
            }
        }
    }
}

//===========================================

MaskContainer::~MaskContainer()
{
}

//===========================================

void MaskContainer::Plot()
{
    TH2F *pTH2F = new TH2F("TH2FMaskedPixels","Masked Pixels",m_GridSize,0,(float)(m_GridSize),m_GridSize,0,(float)m_GridSize);
    pTH2F->SetTitle("Masked Pixels");
    pTH2F->GetXaxis()->SetTitle("Row");
    pTH2F->GetYaxis()->SetTitle("Column");

    for(std::list<std::pair<int,int> >::iterator it = m_MaskedPixels.begin(); it != m_MaskedPixels.end(); ++it)
    {
        std::pair<int,int> maskedPixel = *it;
        int x = maskedPixel.first;
        int y = maskedPixel.second;
        pTH2F->Fill(x,y);
    }

    TCanvas *pCanvas = new TCanvas();
    pCanvas->cd();
    pTH2F->Draw("COLZ");

    std::string pngPlotName = m_PlotPath + "MaskedPixels.png";
    std::string dotCPlotName = m_PlotPath + "MaskedPixels.C";
    std::string pdfPlotName = m_PlotPath + "MaskedPixels.pdf";

    pCanvas->SaveAs(pngPlotName.c_str());
    pCanvas->SaveAs(dotCPlotName.c_str());
    pCanvas->SaveAs(pdfPlotName.c_str());
}
