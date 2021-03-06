/*---------------------------------------------------------------------------*\
 ##   ####  ######     |
 ##  ##     ##         | Copyright: ICE Stroemungsfoschungs GmbH
 ##  ##     ####       |
 ##  ##     ##         | http://www.ice-sf.at
 ##   ####  ######     |
-------------------------------------------------------------------------------
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright  held by original author
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is based on OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM; if not, write to the Free Software Foundation,
    Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Class
    pythonInterpreterWrapper

Description
    Makes sure that the object receives a python-Interpreter on its own

SourceFiles
    pythonInterpreterWrapper.C

Contributors/Copyright:
    2011-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

 SWAK Revision: $Id:  $
\*---------------------------------------------------------------------------*/

#ifndef pythonInterpreterWrapper_H
#define pythonInterpreterWrapper_H

#include "dictionary.H"
#include "Time.H"

#include "Python.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class pythonInterpreterWrapper Declaration
\*---------------------------------------------------------------------------*/


class pythonInterpreterWrapper
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    pythonInterpreterWrapper(const pythonInterpreterWrapper&);

    //- Disallow default bitwise assignment
    void operator=(const pythonInterpreterWrapper&);

    // Private data

    const objectRegistry& obr_;

    //- state of 'my' Python subinterpreter
    PyThreadState *pythonState_;

    //- whether or not to use the numpy library
    bool useNumpy_;

    //- should exceptions be tolerated or lead to a fatal error?
    bool tolerateExceptions_;

    //- Should a non-uniform variable result in a warning?
    bool warnOnNonUniform_;

    //- Is this correctly parallelized?
    bool isParallelized_;

    //- Should parallel runs only run on the master
    bool parallelMasterOnly_;

    //- List of global swak namespaces to get values

    //- variables in those namespaces should be inserted into the
    //- Python namespace before running the snipplet
    wordList swakToPythonNamespaces_;

    //- Namespace to write values to

    //- this global swak-namespace will get all values (if the user request such)
    word pythonToSwakNamespace_;

    //- Python variables that swak will receive

    //- the values of these variables will get written to the global swak namespace
    //- only variables that can be either interpreted as a scalar or a vector are
    //- supported
    wordList pythonToSwakVariables_;

    //- should we get an interactive Python-Shell after every code-Execution?

    //- used for debugging the Python-program
    bool interactiveAfterExecute_;

    //- used for debugging of failed Python-program
    bool interactiveAfterException_;

    void getGlobals();

    void setGlobals();
protected:
    //- how many instances exist (we need a separate interpreter for each of those)
    static label interpreterCount;

    //- Execute code. Return true if there was no problem
    bool executeCode(const string &code,bool putVariables,bool failOnException=false);

    //- Execute code and capture the stdout. Return true if there was no problem
    bool executeCodeCaptureOutput(const string &code,string &stdout,bool putVariables,bool failOnException=false);

    //- Execute code and return a value
    template <typename T,class Func>
    T evaluateCode(const string &code,bool failOnException=false);

    //- Execute code and see if it returns to true or false
    bool evaluateCodeTrueOrFalse(const string &code,bool failOnException=false);

     //- Execute code and use the return value as a float value
    scalar evaluateCodeScalar(const string &code,bool failOnException=false);

      //- Execute code and use the return value as a integer value
    label evaluateCodeLabel(const string &code,bool failOnException=false);

    //- Execute after the python-code has been called
    void doAfterExecution(bool fail,const string &code,bool putVariables,bool failOnException=false);

    //- Read code according to the dictionary
    void readCode(const dictionary &dict,const word &prefix,string &code,bool mustRead=true);

    //- set the state to "my" interpreter
    void setInterpreter();

    //- set the variable 'runTime' in the interpreter
    void setRunTime(const Time &time);

    //- Check whether we want to continue (parallel)
    bool parallelNoRun(bool doWarning=false);

    //- set up the environment with the basic variables
    void initEnvironment(const Time &t);

public:

    //- Runtime type information
    TypeName("pythonInterpreterWrapper");


    // Constructors

    //- Construct from components
    pythonInterpreterWrapper
    (
        const objectRegistry& obr,
        const dictionary&
    );

    virtual ~pythonInterpreterWrapper();

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
