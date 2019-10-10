#include <fstream>
#include <iostream>
#include <Lexical/LexicalAnalysis.h>
#include <Parser/ParseTree.h>
#include <Logger/Logger.h>
#include <AbstractSyntaxTree/AbstractSyntaxTree.h>
#include <Assembly/Assembly.h>
#include <Error/SyntaxError.h>
#include <Lexical/IToken.h>
#include <Error/Errors.h>

using namespace ACC;

struct Options{
    std::string inputFile;
    std::string outputFile;
    bool log = false;
};

void runToolchainLinux(std::string filePath){
    std::string nasmCommand = "nasm -f elf64 "+ filePath +".asm";
    std::string ldCommand = "ld " + filePath + ".o" + " -o " + filePath;


    LOG.createHeading("Running toolchain for 64 bit Linux (nasm, ld) ...");

    LOG() << Log::Colour::Magenta << "Running nasm ";
    LOG() << Log::Colour::Blue << "("+nasmCommand+")" << std::endl << "[Nasm> ";
    LOG() << Log::Colour::Cleared << " ";
    fflush(stdout);
    int exitNasm = system(nasmCommand.c_str());
    LOG() << "" << std::endl;
    LOG() << Log::Colour::Cleared << "... exited with: " + std::to_string(exitNasm) << std::endl;
    if(exitNasm != 0){
        LOG(Log::LogLevel::Error) << Log::Colour::Red << "nasm failed; giving up on the toolchain" << std::endl;
        return;
    }

    LOG() << "" << std::endl;

    LOG() << Log::Colour::Magenta << "Running ld ";
    LOG() << Log::Colour::Blue << "("+ldCommand+")" << std::endl << "[LD> ";
    LOG() << Log::Colour::Cleared << " ";
    fflush(stdout);
    int exitLd = system(ldCommand.c_str());
    LOG() << "" << std::endl;
    LOG() << Log::Colour::Cleared << "... exited with: " + std::to_string(exitLd) << std::endl;
    if(exitLd){
        LOG(Log::LogLevel::Error) << Log::Colour::Red << "ld failed; giving up on the toolchain" << std::endl;
        return;
    }

}

void usage(){
    std::cout << "Usage: acc <input-file>" << std::endl;
    std::cout << "Optionally: acc <input-file> -l -o <output-file>" << std::endl;
    exit(1);
}

Options getOptions(const std::vector<std::string> &in){
    Options out;
    if(in.size() < 2)
        usage();
    out.inputFile = in[1];
    out.outputFile = out.inputFile+".out";

    if(in.size() == 2){
        return out;
    }

    for(size_t i = 2; i < in.size(); i++){
         if(in[i] == "-l")
             out.log = true;
         else if (in[i] == "-o"){
             if(i+1 >= in.size())
                 usage();
             out.outputFile = in[i+1];
             i++;
         }
         else{
             usage();
         }
    }


    return out;
}

// acc <file> -l -o fuck

int main(int argc, char** argv) {
    Options options = getOptions(std::vector<std::string>(argv, argv + argc));

    LOG.silence(!options.log);
    auto l = LexicalAnalysis(options.inputFile);
    l.addZeroExit();
    l.printToken();

    ParseTree p;

    try{
        p.generate(l);
    }
    catch (errors::SyntaxError& err){
        LOG.createHeading("Syntax Errors...");

        LOG(Log::LogLevel::Error) << Log::Colour::Red << "There are syntax errors." << std::endl;
        LOG(Log::LogLevel::Error) << Log::Colour::Cleared << "In line: ";

        LOG(Log::LogLevel::Error) << Log::Colour::Blue << "" << err.lineNum << std::endl;
        LOG(Log::LogLevel::Error) << Log::Colour::Gray << err.lineContent << std::endl;

        LOG(Log::LogLevel::Error) << Log::Colour::Cleared << "Unexpected ";
        LOG(Log::LogLevel::Error) << Log::Colour::Blue << err.unexpected->getIdForErrReporting();

        LOG(Log::LogLevel::Error) << Log::Colour::Cleared << ", are you missing ";
        LOG(Log::LogLevel::Error) << Log::Colour::Blue <<  data::symbolToStringForErrReporting(err.expected);
        LOG(Log::LogLevel::Error) << Log::Colour::Cleared << " ?" << std::endl;

        LOG.del();
        return 2;
    }

    p.getRoot()->print();
    auto a = AbstractSyntaxTree(p);
    a.print();
    a.doTypeResolution();

    auto i = Assembly();

    try{
        a.getRoot()->generate(i.getCode());
    }catch (errors::ASTError& err) {
        LOG.createHeading("Compile Errors...");
        LOG(Log::LogLevel::Error) << Log::Colour::Red << err.msg << std::endl;

        LOG(Log::LogLevel::Error) << Log::Colour::Cleared << "In line: ";
        LOG(Log::LogLevel::Error) << Log::Colour::Blue << "" << err.lineNum << std::endl;
        LOG(Log::LogLevel::Error) << Log::Colour::Gray << err.lineContent << std::endl;

        LOG.del();
        return 3;

    }

    i.print();
    i.writeToFile(options.outputFile + ".asm");

    runToolchainLinux(options.outputFile);


    LOG.del();
    return 0;
 }