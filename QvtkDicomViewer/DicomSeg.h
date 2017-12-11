#pragma once
#include <head_all.h>
#include <itkImageToVTKImageFilter.h>
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {ConnectedThresholdOutput1.png}
//    ARGUMENTS:    60 116 150 180
//  Software Guide : EndCommandLineArgs
void seg_connectedthres(int argc, char * argv[],QVTKWidget* qvtk);
//  Software Guide : BeginCommandLineArgs
//    INPUTS: {BrainProtonDensitySlice.png}
//    ARGUMENTS:   OtsuMultipleThresholdsOutput png 4
//  Software Guide : EndCommandLineArgs
void seg_ostu(int argc, char * argv[], QVTKWidget* qvtk);
void seg_neighconnected(int argc, char * argv[], QVTKWidget* qvtk);
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {ConfidenceConnectedOutput1.png}
//    ARGUMENTS:    60 116
//  Software Guide : EndCommandLineArgs
void seg_confidconnected(int argc, char * argv[], QVTKWidget* qvtk);
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {VisibleWomanEyeSlice.png}
//    OUTPUTS: {WatershedSegmentation1Output1.png}
//    ARGUMENTS:    2 10 0 0.05 1
//  Software Guide : EndCommandLineArgs
void seg_waterseg(int argc, char *argv[], QVTKWidget* qvtk);
static void PrintCommandLineUsage(const int argc, const char * const argv[]);
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {FastMarchingImageFilterOutput5.png}
//    ARGUMENTS:    81 114 1.0  -0.5  3.0   100 100
//    OUTPUTS: {FastMarchingFilterOutput1.png}
//    OUTPUTS: {FastMarchingFilterOutput2.png}
//    OUTPUTS: {FastMarchingFilterOutput3.png}
//  Software Guide : EndCommandLineArgs
void seg_fastmarching(int argc, char * argv[], QVTKWidget* qvtk);
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {ShapeDetectionLevelSetFilterOutput5.png}
//    ARGUMENTS:    81 114 5 1.0  -0.5  3.0   .05 1
//  Software Guide : EndCommandLineArgs
void seg_shapedectection(int argc, char *argv[], QVTKWidget* qvtk);