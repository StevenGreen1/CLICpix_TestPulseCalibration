export ILCSOFT=/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03


#--------------------------------------------------------------------------------
#     LCCD
#--------------------------------------------------------------------------------
export LCCD="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/lccd/v01-03"


#--------------------------------------------------------------------------------
#     CondDBMySQL
#--------------------------------------------------------------------------------
export COND_DB_DEBUGLOG="/dev/stdout"
export CondDBMySQL="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/CondDBMySQL/CondDBMySQL_ILC-0-9-6"
export LD_LIBRARY_PATH="$CondDBMySQL/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     LCIO
#--------------------------------------------------------------------------------
export LCIO="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/lcio/v02-06"
export PYTHONPATH="$LCIO/src/python:$LCIO/examples/python:$PYTHONPATH"
export PATH="$LCIO/tools:$LCIO/bin:$PATH"
export LD_LIBRARY_PATH="$LCIO/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     ROOT
#--------------------------------------------------------------------------------
export ROOTSYS="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/root/5.34.30"
export PYTHONPATH="$ROOTSYS/lib:$PYTHONPATH"
export PATH="$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CMake
#--------------------------------------------------------------------------------
export PATH="/cvmfs/ilc.desy.de/sw/x86_64_gcc44_sl6/CMake/2.8.5/bin:$PATH"


#--------------------------------------------------------------------------------
#     ILCUTIL
#--------------------------------------------------------------------------------
export ilcutil="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/ilcutil/v01-02-01"
export LD_LIBRARY_PATH="$ilcutil/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Marlin
#--------------------------------------------------------------------------------
export MARLIN="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Marlin/v01-07-pre"
export PATH="$MARLIN/bin:$PATH"
export MARLIN_DLL="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinDD4hep/v00-01/lib/libMarlinDD4hep.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinReco/v01-12-pre/lib/libMarlinReco.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/PandoraAnalysis/v01-00-01/lib/libPandoraAnalysis.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinPandora/v02-00-00/lib/libMarlinPandora.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/LCFIVertex/v00-07/lib/libLCFIVertex.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/CEDViewer/v01-09-01-pre/lib/libCEDViewer.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Overlay/v00-14/lib/libOverlay.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/FastJetClustering/v00-02/lib/libFastJetClustering.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinFastJet/v00-02/lib/libMarlinFastJet.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/LCTuple/v01-04/lib/libLCTuple.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinKinfit/v00-01-05/lib/libMarlinKinfit.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinTrkProcessors/v02-00/lib/libMarlinTrkProcessors.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/ILDPerformance/v00-01/lib/libILDPerformance.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Clupatra/v00-11/lib/libClupatra.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Physsim/v00-02/lib/libPhyssim.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/FCalClusterer/v00-01/lib/libFCalClusterer.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/LCFIPlus/v00-05-03/lib/libLCFIPlus.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/ForwardTracking/v01-08/lib/libForwardTracking.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/ClicPerformance/v00-00-00-dev0/lib/libClicPerformance.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/DDMarlinPandora/v00-00-00-dev1/lib/libDDMarlinPandora.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinTPC/v01-00/lib/libMarlinTPC.so:/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Garlic/v3.0.3/lib/libGarlic.so:$MARLIN_DLL"


#--------------------------------------------------------------------------------
#     CLHEP
#--------------------------------------------------------------------------------
export CLHEP="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/CLHEP/2.1.4.1"
export CLHEP_BASE_DIR="$CLHEP"
export CLHEP_INCLUDE_DIR="$CLHEP/include"
export PATH="$CLHEP_BASE_DIR/bin:$PATH"
export LD_LIBRARY_PATH="$CLHEP_BASE_DIR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     RAIDA
#--------------------------------------------------------------------------------
export RAIDA_HOME="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/RAIDA/v01-06-02"
export PATH="$RAIDA_HOME/bin:$PATH"


#--------------------------------------------------------------------------------
#     GEAR
#--------------------------------------------------------------------------------
export GEAR="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/gear/v01-04-02"
export PATH="$GEAR/tools:$GEAR/bin:$PATH"
export LD_LIBRARY_PATH="$GEAR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     DD4hep
#--------------------------------------------------------------------------------
export DD4hepINSTALL="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/DD4hep/v00-13-pre"
export DD4HEP="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/DD4hep/v00-13-pre"
export PYTHONPATH="$DD4HEP/python:$DD4HEP/DDCore/python:$PYTHONPATH"
export PATH="$DD4HEP/bin:$PATH"
export LD_LIBRARY_PATH="$DD4HEP/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Geant4
#--------------------------------------------------------------------------------
export G4INSTALL="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/geant4/10.01"
export G4ENV_INIT="$G4INSTALL/bin/geant4.sh"
export G4SYSTEM="Linux-g++"


#--------------------------------------------------------------------------------
#     QT
#--------------------------------------------------------------------------------
export QTDIR="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/QT/4.7.4"
export QMAKESPEC="$QTDIR/mkspecs/linux-g++"
export PATH="$QTDIR/bin:$PATH"
export LD_LIBRARY_PATH="$QTDIR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     XercesC
#--------------------------------------------------------------------------------
export XercesC_HOME="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/xercesc/3.1.2"
export PATH="$XercesC_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$XercesC_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Boost
#--------------------------------------------------------------------------------
export BOOST_ROOT="/afs/desy.de/project/ilcsoft/sw/boost/1.58.0"


#--------------------------------------------------------------------------------
#     GSL
#--------------------------------------------------------------------------------
export GSL_HOME="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/gsl/1.14"
export PATH="$GSL_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$GSL_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CED
#--------------------------------------------------------------------------------
export PATH="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/CED/v01-09-01/bin:$PATH"


#--------------------------------------------------------------------------------
#     Mokka
#--------------------------------------------------------------------------------
export MOKKA="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Mokka/mokka-08-05"
export PATH="$MOKKA/bin:$PATH"


#--------------------------------------------------------------------------------
#     MySQL
#--------------------------------------------------------------------------------
export MYSQL_HOME="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/mysql/5.0.45"
export MYSQL_LIBDIR="$MYSQL_HOME/lib64/mysql"
export MYSQL_PATH="$MYSQL_HOME"
export MYSQL="$MYSQL_HOME"
export PATH="$MYSQL_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$MYSQL_HOME/lib64/mysql:$MYSQL_HOME/lib64:$MYSQL_HOME/lib/mysql:$MYSQL_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CERNLIB
#--------------------------------------------------------------------------------
export CERN_ROOT="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/cernlib/2006"
export CERN="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/cernlib"
export CERN_LEVEL="2006"
export CVSCOSRC="$CERN_ROOT/src"
export PATH="$CERN_ROOT/bin:$PATH"


#--------------------------------------------------------------------------------
#     PandoraPFANew
#--------------------------------------------------------------------------------
export PANDORAPFANEW="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/PandoraPFANew/v02-00-00"
export LD_LIBRARY_PATH="$PANDORAPFANEW/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     CEDViewer
#--------------------------------------------------------------------------------
export PATH="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/CEDViewer/v01-09-01-pre/bin:$PATH"


#--------------------------------------------------------------------------------
#     FastJet
#--------------------------------------------------------------------------------
export FastJet_HOME="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/FastJet/3.1.2"
export PATH="$FastJet_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$FastJet_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     KalTest
#--------------------------------------------------------------------------------
export KALTEST="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/KalTest/v02-00"
export LD_LIBRARY_PATH="$KALTEST/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     aidaTT
#--------------------------------------------------------------------------------
export AIDATT="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/aidaTT/v00-02-pre"
export PATH="$AIDATT/bin:$PATH"
export LD_LIBRARY_PATH="$AIDATT/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     GBL
#--------------------------------------------------------------------------------
export GBL="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/GBL/V01-16-04"
export LD_LIBRARY_PATH="$GBL/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     MarlinTPC
#--------------------------------------------------------------------------------
export MARLINTPC="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/MarlinTPC/v01-00"
export PATH="$MARLINTPC/bin:$PATH"


#--------------------------------------------------------------------------------
#     Druid
#--------------------------------------------------------------------------------
export DRUID="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/Druid/2.2"
export PATH="$DRUID/bin:$PATH"


#--------------------------------------------------------------------------------
#     HepPDT
#--------------------------------------------------------------------------------
export HepPDT_HOME="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/heppdt/3.04.01"
export PATH="$HepPDT_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$HepPDT_HOME/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     GDML
#--------------------------------------------------------------------------------
export GDML="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/gdml/ilcsoft-v01-17-07"
export LD_LIBRARY_PATH="$GDML/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     LCDD
#--------------------------------------------------------------------------------
export LCDD="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/lcdd/ilcsoft-v01-17-07"
export GDML_SCHEMA_DIR="$LCDD"
export LD_LIBRARY_PATH="$LCDD/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     SLIC
#--------------------------------------------------------------------------------
export SLIC="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/slic/ilcsoft-v01-17-07"
export PATH="$SLIC/build/bin:$PATH"
export LD_LIBRARY_PATH="$SLIC/build/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     SlicPandora
#--------------------------------------------------------------------------------
export SLICPANDORA="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/slicPandora/ilcsoft-v01-17-07"
export PATH="$SLICPANDORA/build/bin:$PATH"
export LD_LIBRARY_PATH="$SLICPANDORA/build/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     lcgeo
#--------------------------------------------------------------------------------
export lcgeo_DIR="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/lcgeo/v00-04-01-pre"
export PATH="$lcgeo_DIR/bin:$PATH"
export LD_LIBRARY_PATH="$lcgeo_DIR/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     DD4hepExamples
#--------------------------------------------------------------------------------
export DD4hepExamples="/afs/cern.ch/eng/clic/work/ilcsoft/v01-17-07.p03/DD4hepExamples/v00-13-pre"
export PATH="$DD4hepExamples/bin:$PATH"
export LD_LIBRARY_PATH="$DD4hepExamples/lib:$LD_LIBRARY_PATH"


#--------------------------------------------------------------------------------
#     Java
#--------------------------------------------------------------------------------
export JAVA_HOME="/usr/lib/jvm/java-1.7.0-openjdk-1.7.0.79.x86_64"
export JDK_HOME="$JAVA_HOME"
export PATH="$JDK_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$JDK_HOME/jre/lib/i386:$JDK_HOME/jre/lib/i386/client:$LD_LIBRARY_PATH"

# --- source GEANT4 INIT script ---
test -r ${G4ENV_INIT} && { cd $(dirname ${G4ENV_INIT}) ; . ./$(basename ${G4ENV_INIT}) ; cd $OLDPWD ; }

# ---  Workaraund for OpenGl bug on SL6  ---
export LIBGL_ALWAYS_INDIRECT=1
