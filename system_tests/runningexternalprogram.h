#ifndef RUNNINGEXTERNALPROGRAM_H
#define RUNNINGEXTERNALPROGRAM_H

#include <string>
#include <vector>


struct ProgramOutputs
{
    int exitCode_ {};
    std::vector<std::string> linesOfStdout_, linesOfStderr_;
};

ProgramOutputs runCommandReturningOutput(const std::string& command,
                                         const std::string& inputFilePath = {},
                                         const std::string& inputText = {});

#endif // RUNNINGEXTERNALPROGRAM_H
