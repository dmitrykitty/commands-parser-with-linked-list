#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include "runningexternalprogram.h"

#ifdef SYSTEM_TESTS_WITH_BOOST /// use boost library to call process
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>

std::unique_ptr<boost::process::child> createSubprocessObject(const std::string& command, const std::string& inputFilePath, const std::string& inputText,
                                                              boost::process::ipstream& stdoutStream, boost::process::ipstream& stderrStream);
ProgramOutputs readOutputFromProcess(std::unique_ptr<boost::process::child>& c, boost::process::ipstream& stdoutStream, boost::process::ipstream& stderrStream);



ProgramOutputs runCommandReturningOutput(const std::string &command,
                                         const std::string &inputFilePath,
                                         const std::string &inputText)
{
    using namespace std::chrono_literals;
    using namespace boost::process;

    ipstream stdoutStream, stderrStream;

    std::unique_ptr<child> c = createSubprocessObject(command,
                                                      inputFilePath,
                                                      inputText,
                                                      stdoutStream,
                                                      stderrStream);
    if (!c->wait_for(2s)) {
        c->terminate();
    }

    return readOutputFromProcess(c, stdoutStream, stderrStream);
}

std::unique_ptr<boost::process::child> createSubprocessObject(const std::string& command, const std::string& inputFilePath, const std::string& inputText,
                                                              boost::process::ipstream& stdoutStream, boost::process::ipstream& stderrStream)
{
    using namespace std::chrono_literals;
    using namespace boost::process;

    if (inputFilePath.empty() && inputText.empty())
    {
        return std::make_unique<child>(command, std_out > stdoutStream, std_err > stderrStream);
    }
    else if (inputFilePath.size())
    {
        return std::make_unique<child>(command, std_out > stdoutStream, std_err > stderrStream, std_in < inputFilePath);
    }
    else //if (inputText.size())
    {
        opstream stdinStream;
        stdinStream << inputText << std::endl;
        stdinStream.close();
        return std::make_unique<child>(command, std_out > stdoutStream, std_err > stderrStream, std_in < stdinStream);
    }
}

ProgramOutputs readOutputFromProcess(std::unique_ptr<boost::process::child>& c, boost::process::ipstream& stdoutStream, boost::process::ipstream& stderrStream)
{
    ProgramOutputs outputs;

    std::string line;
    while (stdoutStream && std::getline(stdoutStream, line) && !line.empty())
    {
        outputs.linesOfStdout_.emplace_back(std::move(line));
    }
    while (stderrStream && std::getline(stderrStream, line) && !line.empty())
    {
        outputs.linesOfStderr_.emplace_back(std::move(line));
    }

    c->wait();

    outputs.exitCode_ = c->exit_code();

    return outputs;
}
#else  /// use tiny-process-library to call process

#include <process.hpp>

using TinyProcessLib::Process;

namespace
{
void sendInput2Process(const std::string &inputText,
                       const std::string &inputFilePath,
                       bool inputAvailable,
                       Process &process);

int wait4ProcessReturningStatus(Process& process);

std::vector<std::string> text2Lines(const std::string & text);
} // namespace

ProgramOutputs runCommandReturningOutput(const std::string &command,
                                         const std::string &inputFilePath,
                                         const std::string &inputText)
{

    std::string allStdout, allStderr;
    auto stdoutReader = [&allStdout](const char *bytes, size_t n) {
        allStdout += std::string(bytes, n);
    };
    auto stderrReader = [&allStderr](const char *bytes, size_t n) {
        allStderr += std::string(bytes, n);
    };

    const bool inputAvailable = inputText.size() > 0 || !inputFilePath.empty();

    Process process(command,
                    /*path=*/"",
                    stdoutReader,
                    stderrReader,
                    /*open_stdin=*/inputAvailable);

    sendInput2Process(inputText, inputFilePath, inputAvailable, process);

    const ProgramOutputs returnValue{
        .exitCode_ = wait4ProcessReturningStatus(process),
        .linesOfStdout_ = text2Lines(allStdout),
        .linesOfStderr_ = text2Lines(allStderr),
    };

    if (returnValue.exitCode_ < 0)
    {
        process.kill(true);
    }

    return returnValue;
}

namespace
{
void sendInput2Process(const std::string &inputText,
                       const std::string &inputFilePath,
                       bool inputAvailable,
                       Process &process)
{
    if (!inputText.empty()) {
        process.write(inputText + '\n');
    }
    if (!inputFilePath.empty()) {
        std::ifstream file(inputFilePath);
        if (!file) {
            throw std::runtime_error("Can not open file: " + inputFilePath);
        }
        for (std::string line; getline(file, line);) {
            process.write(line + '\n');
        }
    }

    process.close_stdin();
}

int wait4ProcessReturningStatus(Process& process)
{
    int exitCode{};
    constexpr size_t maxWaitingSeconds = 5;
    size_t waitingTime = {};
    while (waitingTime < maxWaitingSeconds + 1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++waitingTime;

        if (process.try_get_exit_status(exitCode))
        {
            return exitCode;
        }
    }
    if (waitingTime >= maxWaitingSeconds) {
        return -1;
    }
    return exitCode;
}

std::vector<std::string> text2Lines(const std::string & text)
{
    constexpr const char newlineCharacter = '\n';

    const auto newlinesCount = std::count(text.begin(), text.end(), newlineCharacter);
    std::vector<std::string> lines;
    lines.reserve(newlinesCount + 1);

    size_t startingPosition{};
    while (true)
    {
        const auto currentPosition = text.find(newlineCharacter, startingPosition);
        if (currentPosition == std::string::npos)
        {
            auto lastLine = text.substr(startingPosition);
            if (!lastLine.empty())
            {
                lines.emplace_back(std::move(lastLine));
            }
            break;
        }
        else
        {
            const auto characterCount = currentPosition - startingPosition;
            auto line = text.substr(startingPosition, characterCount);
            startingPosition += characterCount + 1;
            lines.emplace_back(std::move(line));
        }
    }

    return lines;
}
} // namespace

#endif // SYSTEM_TESTS_WITH_BOOST
