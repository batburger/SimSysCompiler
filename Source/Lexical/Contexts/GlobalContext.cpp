//
// Created by a_mod on 06.01.2019.
//

#include "GlobalContext.h"
#include <regex>
#include <Lexical/Tokens/IdToken.h>
#include <Lexical/Contexts/AssignmentContext.h>
#include <Lexical/Contexts/KeywordContext.h>
#include <errors.h>
#include <Lexical/Tokens/VarToken.h>
#include <evaluators.h>

// var [a-zA-Z]+
ACC::GlobalContext::GlobalContext()
        : legals(
        {
                {Pattern({"[a-zA-Z_]+ | +[a-zA-Z_]+ "}), Instruction(InstructionId::NEW_TOKEN, new evaluator(data::id_eval))},
                {Pattern({"var| +var"}), Instruction(InstructionId::NEW_TOKEN, new evaluator(data::var_eval))},

                {Pattern({"print"}), Instruction(InstructionId::CHANGE_CONTEXT, new context_switcher(data::keyword_switch))},
                {Pattern({"="}), Instruction(InstructionId::CHANGE_CONTEXT, new context_switcher(data::assignment_switch))},
        }
)
{
}

ACC::GlobalContext::GlobalContext(const ACC::GlobalContext &other) :
        legals(other.legals){
    refCounter++;
}

ACC::GlobalContext::~GlobalContext() {
    refCounter--;
    if(refCounter != 0)
        return;

    for(const auto& legal : legals){
        auto instruction = legal.second;
        if(instruction.id == InstructionId::NEW_TOKEN)
            delete static_cast<Instruction::token_func*>(instruction.func);
        else if (instruction.id == InstructionId::CHANGE_CONTEXT)
            delete static_cast<Instruction::context_func*>(instruction.func);
    }
}


const std::vector<ACC::IContext::match> ACC::GlobalContext::getLegals() {
    return legals;
}

ACC::Pattern ACC::GlobalContext::escapeSequence() {
   return Pattern({"}"});
}