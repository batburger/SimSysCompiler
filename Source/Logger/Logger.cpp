//
// Created by a_mod on 19.01.2019.
//

#include "Logger.h"
#include <chrono>
#include <iostream>

const std::string htmlPreamble = R"(<!DOCTYPE html>
<html>
<head>
    <script>
        window.onload = function () {
            var list = document.querySelectorAll("#heading");
            list.forEach((e) => {
                e.addEventListener("click", (event) => {
                    event.target.nextSibling.style.display = event.target.nextSibling.style.display == "none" ? ("") : ("none");
                })
            });
        }
    </script>

	<title>Log</title>
	<style type="text/css">
		#running{
			white-space: pre-wrap;
			width: 99vw;
			color: white;
			background-color: black;
			margin: 0 auto;
			font-family: 'Lucida Console', Monaco, monospace;
		}
		#heading{
			white-space: pre-wrap;
			background: #4c4c4c;
			width: 99vw;
			color: #3dff00;
			margin: 0 auto;
			text-align: center;
			font-family: 'Lucida Console', Monaco, monospace;
			font-weight: 800;
		}
        h1{
            font-family: 'Lucida Console', Monaco, monospace;
			font-weight: 800;
        }

	    #heading:hover{
			background: #2a2a2a;
            cursor: pointer;
        }
	</style>
</head>
<body>
<h1>Generated Log (press headings to expand/collapse)...</h1>
<div id="running">)";

const std::string htmlEnd = R"(</div></body></html>)";

namespace ANSI_CODES{
    const std::string blue = "\033[34;1m";
    const std::string magenta = "\033[35;1m";
    const std::string clear = "\033[0m";
    const std::string heading = "\033[32;1m";
    const std::string green = "\033[32;1m";
    const std::string red = "\033[31;1m";
    const std::string bold = "\033[1m";
    const std::string gray = "\033[90m";
};

ACC::Log::LogStream::LogStream(ACC::Log::Logger &logger, ACC::Log::LogLevel level) :
logger(logger), level(level){

}

ACC::Log::LogStream::~LogStream() {
    logger.lock();
    this->logger.loadColour(colour);
    logger.log(level, str());
    logger.unlock();
}

ACC::Log::LogStream &ACC::Log::LogStream::operator<<(ACC::Log::Colour colour) {
    this->colour = colour;
    return *this;
}

ACC::Log::LogStream ACC::Log::Logger::operator()(ACC::Log::LogLevel level) {
    return LogStream(*this, level);
}

void ACC::Log::Logger::log(ACC::Log::LogLevel level, std::string str) {
    logToConsole(level, str);
    logToFile(level, str);
    colour = Colour::Cleared;
}

ACC::Log::Logger::Logger() {
    auto start = std::chrono::system_clock::now().time_since_epoch().count();
   // path = "./log_"+std::to_string(start)+".html";
    path = "./log.html";
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::trunc);
    if(ofs.is_open()){
        ofs << htmlPreamble;
        ofs.close();
    }
    file = std::ofstream(path, std::ios_base::in | std::ios_base::out);
    head = htmlPreamble.size();
}

void ACC::Log::Logger::loadColour(ACC::Log::Colour colour) {
    this->colour = colour;
}

void ACC::Log::Logger::logToConsole(ACC::Log::LogLevel level, std::string str) {
    if(isSilent && level != LogLevel::Error)
        return;

    switch (colour){
        case Colour::Blue:
            std::cout << ANSI_CODES::blue;
            break;
        case Colour::Magenta:
            std::cout << ANSI_CODES::magenta;
            break;
        case Colour::Cleared:
            std::cout << ANSI_CODES::clear;
            break;
        case Colour::Red:
            std::cout << ANSI_CODES::red;
            break;
        case Colour::Green:
            std::cout << ANSI_CODES::green;
            break;
        case Colour::Bold:
            std::cout << ANSI_CODES::bold;
            break;
        case Colour::Gray:
            std::cout << ANSI_CODES::gray;
            break;
    }
    std::cout << str;
    std::cout << ANSI_CODES::clear;
}

void ACC::Log::Logger::logToFile(ACC::Log::LogLevel level, std::string str) {
    if(!file.is_open())
        return;

    for(size_t i = 0; i < str.size(); i++){
        if(str[i] == '<'){
            str.erase(i, 1);
            str.insert(i, "&lt;");
        } else if (str[i] == '>'){
                str.erase(i, 1);
                str.insert(i, "&gt;");
        }
    }

    switch (colour){
        case Colour::Blue:
            str = R"(<b style="color: #253df7;">)" + str;
            str += R"(</b>)";
            break;
        case Colour::Magenta:
            str = R"(<b style="color: magenta;">)" + str;
            str += R"(</b>)";
            break;
        case Colour::Cleared:
            break;
        case Colour::Red:
            str = R"(<b style="color: red;">)" + str;
            str += R"(</b>)";
            break;
        case Colour::Green:
            str = R"(<b style="color: green;">)" + str;
            str += R"(</b>)";
            break;

        case Colour::Bold:
            str = R"(<b style="color: white;">)" + str;
            str += R"(</b>)";
            break;

        case Colour::Gray:
            str = R"(<b style="color: gray;">)" + str;
            str += R"(</b>)";
            break;
    }

    file.seekp(head);
    file << str;
    file << htmlEnd;

    file.flush();

    head += str.size();
}

void ACC::Log::Logger::createHeading(std::string str) {
    this->lock();
    if(!isSilent)
        std::cout << ANSI_CODES::heading << str << ANSI_CODES::clear << std::endl;
    str = R"(</div><div id="heading">)" + str + R"(</div><div id="running" style="display: none;">)";
    file.seekp(head);
    file << str;
    file << htmlEnd;

    file.flush();

    head += str.size();
    this->unlock();
}

void ACC::Log::Logger::silence(bool b) {
    isSilent = b;
}

void ACC::Log::Logger::lock() {
    mtx.lock();
}

void ACC::Log::Logger::unlock() {
    mtx.unlock();
}

