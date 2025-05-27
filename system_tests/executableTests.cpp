#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <filesystem>

#include <gtest/gtest.h>
#include "runningexternalprogram.h"

namespace
{
using namespace std;
using namespace ::testing;


std::filesystem::path getPathToIOTestFiles();

std::pair<std::string, std::string> getPairOfIOTestFilesForBasename(const std::string& baseFilename)
{
    auto basePath = getPathToIOTestFiles();
    auto inputFileBasename = baseFilename + ".in.txt";
    auto outputFileBasename = baseFilename + ".out.txt";

    auto inputFilePath = basePath / inputFileBasename;
    auto outputFilePath = basePath / outputFileBasename;

    if (std::filesystem::exists(inputFilePath) && std::filesystem::exists(outputFilePath))
    {
        return {inputFilePath.string(), outputFilePath.string()};
    }
    throw std::runtime_error("!Files '" + inputFilePath.string() + "', or '" + outputFilePath.string() + "' do not exist!");
}
std::filesystem::path getPathToIOTestFiles()
{
#ifdef CMAKE_CURRENT_SOURCE_DIR
    auto parentPath = std::filesystem::path(CMAKE_CURRENT_SOURCE_DIR);
#else
    auto parentPath = std::filesystem::path(__FILE__).parent_path();
#endif

    auto path2IoTestFiles = parentPath / "inputOutputData";
    if (std::filesystem::exists(path2IoTestFiles))
    {
        return path2IoTestFiles;
    }
    throw std::runtime_error("!Path '" + path2IoTestFiles.string() + "' does not exist!");
}

std::vector<std::string> readLinesOfFile(const string& filename)
{
    ifstream file(filename);
    if (!file)
    {
        throw std::invalid_argument("!Can not open file: " + filename);
    }
    std::vector<std::string> lines;

    std::string line;
    while (file && std::getline(file, line) && !line.empty())
    {
        lines.emplace_back(std::move(line));
    }
    return lines;
}

std::string lines2Text(const std::vector<std::string>& lines)
{
    std::string text;
    for (const auto& line : lines)
    {
        text += line + '\n';
    }
    return text;
}
} // namespace


struct CommandsParserSystemTester : ::testing::TestWithParam<std::string>
{
    std::string binaryPath;

    void SetUp() override
    {
#ifndef COMMANDS_PARSER_PATH
        ADD_FAILURE() << "CMake variable 'COMMANDS_PARSER_PATH' not defined!";
#else
        if (! std::filesystem::exists(COMMANDS_PARSER_PATH))
        {
            ADD_FAILURE() << "Binary '" << COMMANDS_PARSER_PATH << "' does not exists!";
        }
        binaryPath = COMMANDS_PARSER_PATH;
#endif
    }
};

TEST_F(CommandsParserSystemTester, executingOurProgramWithoutAnyInput_expectedNoOutput)
{
    const auto output = runCommandReturningOutput(binaryPath);

    ASSERT_EQ(0, output.linesOfStderr_.size());
    ASSERT_EQ(0, output.linesOfStdout_.size());   
    ASSERT_EQ(0, output.exitCode_);
}

TEST_F(CommandsParserSystemTester, testInvalidCommand)
{
    constexpr auto expectedLinesOfStderrOutput = 1;

    /// ponizszy "cytat" usłyszałem od pewnego programisty, proszę nie traktować go jako pewnika, bardziej jako sucharek:
    const auto inputText = "say Python is language for lazy C++ programmers, but Javascript is language for lazy Python programmers";
    const auto output = runCommandReturningOutput(binaryPath, /*inputFile=*/{}, inputText);
    ASSERT_EQ(0, output.linesOfStdout_.size())
            << "expectedNoOutput, but realOutput:\n"
            << lines2Text(output.linesOfStdout_);
    ASSERT_EQ(expectedLinesOfStderrOutput, output.linesOfStderr_.size());
    ASSERT_EQ(0, output.exitCode_);
}

TEST_P(CommandsParserSystemTester, testVariousScenariosUsingInputAndOutputFiles)
{
    const auto [inputFile, expectedOutputFile] = getPairOfIOTestFilesForBasename(GetParam());
    const auto expectedOutputFileLines = readLinesOfFile(expectedOutputFile);

    const auto output = runCommandReturningOutput(binaryPath, inputFile);
    ASSERT_EQ(expectedOutputFileLines.size(), output.linesOfStdout_.size())
            << "expectedOutput:\n"
            << lines2Text(readLinesOfFile(expectedOutputFile)) << '\n'
            << "realOutput:\n"
            << lines2Text(output.linesOfStdout_);
    ASSERT_EQ(0, output.linesOfStderr_.size()) << "unexpected stderror output:\n"
                                               << lines2Text(output.linesOfStderr_);
    ASSERT_EQ(0, output.exitCode_);

    for (size_t lineIndex=0; lineIndex < expectedOutputFileLines.size(); ++lineIndex)
    {
        ASSERT_EQ(expectedOutputFileLines[lineIndex], output.linesOfStdout_[lineIndex])
                << "indexOfLine=" << lineIndex << ", source file: " << inputFile << ", expected output file: " << expectedOutputFile;
    }
}

INSTANTIATE_TEST_SUITE_P(CommandsParserSystemTester,
                         CommandsParserSystemTester,
                         testing::Values("sumStringObject",
                                         "sumDoubleObject",
                                         "mulDifferentTypes",
                                         "getByIndexComplexObject"));

/* TEST_P(CommandsParserSystemTester, testVariousScenariosUsingInputAndOutputFiles) jest zamiast poniższych:
TEST_F(CommandsParserSystemTester, addingManyTypesThenSumOfStrings_expectedSumOfStringsPrinted)
{
    const auto [inputFile, expectedOutputFile] = getPairOfIOTestFilesForBasename("sumStringObject");
    const auto expectedOutputFileLines = readLinesOfFile(expectedOutputFile);

    const auto output = runCommandReturningOutput(binaryPath, inputFile);
    ASSERT_EQ(expectedOutputFileLines.size(), output.linesOfStdout_.size());
    ASSERT_EQ(0, output.linesOfStderr_.size());
    ASSERT_EQ(0, output.exitCode_);

    for (size_t lineIndex=0; lineIndex < expectedOutputFileLines.size(); ++lineIndex)
    {
        ASSERT_EQ(expectedOutputFileLines[lineIndex], output.linesOfStdout_[lineIndex])
                << "indexOfLine=" << lineIndex << ", source file: " << inputFile << ", expected output file: " << expectedOutputFile;
    }
}

TEST_F(CommandsParserSystemTester, addingManyTypesThenSumOfDoubles_expectedSumOfDoublesPrinted)
{
    const auto [inputFile, expectedOutputFile] = getPairOfIOTestFilesForBasename("sumDoubleObject");
    const auto expectedOutputFileLines = readLinesOfFile(expectedOutputFile);

    const auto output = runCommandReturningOutput(binaryPath, inputFile);
    ASSERT_EQ(expectedOutputFileLines.size(), output.linesOfStdout_.size());
    ASSERT_EQ(0, output.linesOfStderr_.size());
    ASSERT_EQ(0, output.exitCode_);

    for (size_t lineIndex=0; lineIndex < expectedOutputFileLines.size(); ++lineIndex)
    {
        ASSERT_EQ(expectedOutputFileLines[lineIndex], output.linesOfStdout_[lineIndex])
                << "indexOfLine=" << lineIndex << ", source file: " << inputFile << ", expected output file: " << expectedOutputFile;
    }
}

TEST_F(CommandsParserSystemTester, multiplyAllObjectsThenSum_expectedSumDoubledOfAllTypes)
{
    const auto [inputFile, expectedOutputFile] = getPairOfIOTestFilesForBasename("mulDifferentTypes");
    const auto expectedOutputFileLines = readLinesOfFile(expectedOutputFile);

    const auto output = runCommandReturningOutput(binaryPath, inputFile);
    ASSERT_EQ(expectedOutputFileLines.size(), output.linesOfStdout_.size());
    ASSERT_EQ(0, output.linesOfStderr_.size());
    ASSERT_EQ(0, output.exitCode_);

    for (size_t lineIndex=0; lineIndex < expectedOutputFileLines.size(); ++lineIndex)
    {
        ASSERT_EQ(expectedOutputFileLines[lineIndex], output.linesOfStdout_[lineIndex])
                << "indexOfLine=" << lineIndex << ", source file: " << inputFile << ", expected output file: " << expectedOutputFile;
    }
}

TEST_F(CommandsParserSystemTester, sumStringObjectWhenNoStringObject_expectedNoOutput)
{
    const auto [inputFile, expectedOutputFile] = getPairOfIOTestFilesForBasename("sumStringObjectWhenNoStringObject");
    const auto expectedOutputFileLines = readLinesOfFile(expectedOutputFile);

    const auto output = runCommandReturningOutput(binaryPath, inputFile);
    ASSERT_EQ(expectedOutputFileLines.size(), output.linesOfStdout_.size());
    ASSERT_EQ(0, output.linesOfStderr_.size());
    ASSERT_EQ(0, output.exitCode_);

    for (size_t lineIndex=0; lineIndex < expectedOutputFileLines.size(); ++lineIndex)
    {
        ASSERT_EQ(expectedOutputFileLines[lineIndex], output.linesOfStdout_[lineIndex])
                << "indexOfLine=" << lineIndex << ", source file: " << inputFile << ", expected output file: " << expectedOutputFile;
    }
}
*/
