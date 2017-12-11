#pragma once
#include<head_all.h>
class CommandIterationUpdate_test : public itk::Command
{
public:
	typedef CommandIterationUpdate_test   Self;
	typedef itk::Command             Superclass;
	typedef itk::SmartPointer<Self>  Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_test();

public:
	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef const OptimizerType*                     OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event);
	void Execute(const itk::Object * object, const itk::EventObject & event);

};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceShifted13x17y.png}
//    OUTPUTS: {ImageRegistration1Output.png}
//    OUTPUTS: {ImageRegistration1DifferenceAfter.png}
//    OUTPUTS: {ImageRegistration1DifferenceBefore.png}
//  Software Guide : EndCommandLineArgs
void reg_test(int argc, char *argv[],QVTKWidget* qvtk);

class CommandIterationUpdate_2Dtransform : public itk::Command
{
public:
	typedef  CommandIterationUpdate_2Dtransform   Self;
	typedef  itk::Command             Superclass;
	typedef itk::SmartPointer<Self>   Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_2Dtransform();

public:
	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef   const OptimizerType *                  OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event);
	void Execute(const itk::Object * object, const itk::EventObject & event);
};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceR10X13Y17S12.png}
//    OUTPUTS: {ImageRegistration7Output.png}
//    OUTPUTS: {ImageRegistration7DifferenceBefore.png}
//    OUTPUTS: {ImageRegistration7DifferenceAfter.png}
//    ARGUMENTS:    1.0   1.0   0.0
//  Software Guide : EndCommandLineArgs
void reg_2Dtransform(int argc, char *argv[], QVTKWidget* qvtk);

class CommandIterationUpdate_AffinTrans : public itk::Command
{
public:
	typedef  CommandIterationUpdate_AffinTrans   Self;
	typedef  itk::Command             Superclass;
	typedef itk::SmartPointer<Self>   Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_AffinTrans();

public:
	typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
	typedef   const OptimizerType *                  OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event);

	void Execute(const itk::Object * object, const itk::EventObject & event);
};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceR10X13Y17.png}
//    OUTPUTS: {ImageRegistration9Output.png}
//    OUTPUTS: {ImageRegistration9DifferenceBefore.png}
//    OUTPUTS: {ImageRegistration9DifferenceAfter.png}
//    ARGUMENTS:    1.0 300
//  Software Guide : EndCommandLineArgs
void reg_AffineTrans(int argc, char *argv[], QVTKWidget* qvtk);

template <typename TRegistration>
class RegistrationInterfaceCommand : public itk::Command
{
public:
	typedef  RegistrationInterfaceCommand   Self;
	typedef  itk::Command                   Superclass;
	typedef  itk::SmartPointer<Self>        Pointer;
	itkNewMacro(Self);

protected:
	RegistrationInterfaceCommand() {};
public:
	typedef   TRegistration      RegistrationType;
	typedef   RegistrationType * RegistrationPointer;
	typedef   itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
	typedef   OptimizerType * OptimizerPointer;
	void Execute(itk::Object * object,
		const itk::EventObject & event);

	// Software Guide : BeginLatex
	//
	// Another version of the \code{Execute()} method accepting a \code{const}
	// input object is also required since this method is defined as pure virtual
	// in the base class.  This version simply returns without taking any action.
	//
	// Software Guide : EndLatex

	// Software Guide : BeginCodeSnippet
	void Execute(const itk::Object *, const itk::EventObject &) ITK_OVERRIDE;
};
class CommandIterationUpdate_Multi : public itk::Command
{
public:
	typedef  CommandIterationUpdate_Multi   Self;
	typedef  itk::Command             Superclass;
	typedef  itk::SmartPointer<Self>  Pointer;
	itkNewMacro(Self);

protected:
	CommandIterationUpdate_Multi() : m_CumulativeIterationIndex(0) {};

public:
	typedef   itk::RegularStepGradientDescentOptimizerv4<double>  OptimizerType;
	typedef   const OptimizerType *                               OptimizerPointer;

	void Execute(itk::Object *caller, const itk::EventObject & event) ITK_OVERRIDE;

	void Execute(const itk::Object * object, const itk::EventObject & event) ITK_OVERRIDE;
private:
	unsigned int m_CumulativeIterationIndex;
};
//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainT1SliceBorder20.png}
//    INPUTS:  {BrainProtonDensitySliceShifted13x17y.png}
//    OUTPUTS: {MultiResImageRegistration1Output.png}
//    ARGUMENTS: 128
//    OUTPUTS: {MultiResImageRegistration1CheckerboardBefore.png}
//    OUTPUTS: {MultiResImageRegistration1CheckerboardAfter.png}
//  Software Guide : EndCommandLineArgs
void reg_Multi(int argc, char* argv[], QVTKWidget* qvtk);