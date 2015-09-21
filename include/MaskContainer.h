#ifndef MASK_CONTINER_H
#define MASK_CONTINER_H

#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TStyle.h"

class MaskContainer
{
    private:
        /*
         * Private Variables
         */
        std::string m_FileName;
        int m_GridSize;
        std::list<std::pair<int,int> > m_MaskedPixels;
        std::string m_PlotPath;

    public:
        /*
         * Default Constructor
         */
        MaskContainer(std::string fileName, int gridSize, std::string plotPath);

        /*
         * Default Destructor
         */
        ~MaskContainer();

        /*
         * Make a plot of the masked pixels
         */
        void Plot();

        /*
         * Get Parameter Functions
         */
        std::list<std::pair<int,int> > GetMaskedPixels() {return m_MaskedPixels;}
};

#endif


