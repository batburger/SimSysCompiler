//
// Created by a_mod on 09.01.2019.
//

#ifndef SIMSYSCOMPILER_ASSIGMENTCONTEXT_H
#define SIMSYSCOMPILER_ASSIGMENTCONTEXT_H

#include <Lexical/IContext.h>

namespace ACC {
    class AssignmentContext : public IContext {
    private:
        const std::vector<ACC::IContext::match> legals;
        int refCounter = 1;
    public:
        AssignmentContext();
        AssignmentContext(const AssignmentContext& other);
        ~AssignmentContext();
        const std::vector<match> getLegals() override;
        Pattern escapeSequence() override;
    };
}

#endif //SIMSYSCOMPILER_ASSIGMENTCONTEXT_H
