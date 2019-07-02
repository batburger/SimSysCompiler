#include <fstream>
#include <iostream>
#include <Lexical/LexicalAnalysis.h>
#include <Parser/ParseTree.h>
#include <Logger/Logger.h>
#include <AbstractSyntaxTree/AbstractSyntaxTree.h>
#include <Assembly/Assembly.h>

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
        LOG() << Log::Colour::Red << "nasm failed; giving up on the toolchain" << std::endl;
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
        LOG() << Log::Colour::Red << "ld failed; giving up on the toolchain" << std::endl;
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
    auto p = ParseTree(l);
    p.getRoot()->print();
    auto a = AbstractSyntaxTree(p);
    a.print();
    auto i = Assembly(a);
    i.print();
    i.writeToFile(options.outputFile + ".asm");

    runToolchainLinux(options.outputFile);


    LOG.del();
    return 0;
 }