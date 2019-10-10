#include <Logger/Logger.h>
#include <astRules.h>
#include <AbstractSyntaxTree/process.h>
#include <AbstractSyntaxTree/ASTNode.h>
#include <General/builtinTypes.h>
#include <Parser/Production.h>
#include <Lexical/Tokens/IdToken.h>
#include <Lexical/Tokens/SyscallToken.h>
#include <Lexical/Tokens/DeclToken.h>
#include <Lexical/Tokens/ComparisionToken.h>
#include <AbstractSyntaxTree/ASTNodes/SeqNode.h>
#include <AbstractSyntaxTree/ASTNodes/IfConstructNode.h>
#include <AbstractSyntaxTree/ASTNodes/WhileNode.h>
#include <AbstractSyntaxTree/ASTNodes/ForNode.h>
#include <AbstractSyntaxTree/ASTNodes/IdNode.h>
#include <AbstractSyntaxTree/ASTNodes/FunctionNode.h>
#include <AbstractSyntaxTree/ASTNodes/CallNode.h>
#include <AbstractSyntaxTree/ASTNodes/SallocNode.h>
#include <AbstractSyntaxTree/ASTNodes/AssignNode.h>
#include <AbstractSyntaxTree/ASTNodes/ReassignNode.h>
#include <AbstractSyntaxTree/ASTNodes/SyscallNode.h>
#include <AbstractSyntaxTree/ASTNodes/MemberCallNode.h>
#include <AbstractSyntaxTree/ASTNodes/ExitNode.h>
#include <AbstractSyntaxTree/ASTNodes/ReturnNode.h>
#include <AbstractSyntaxTree/ASTNodes/LiteralNode.h>
#include <AbstractSyntaxTree/ASTNodes/AddNode.h>
#include <AbstractSyntaxTree/ASTNodes/SubtractNode.h>
#include <AbstractSyntaxTree/ASTNodes/MultiplicationNode.h>
#include <AbstractSyntaxTree/ASTNodes/DivisionNode.h>
#include <AbstractSyntaxTree/ASTNodes/ComparisionNode.h>
#include <AbstractSyntaxTree/ASTNodes/NotNode.h>
#include <AbstractSyntaxTree/ASTNodes/PtrAssignmentNode.h>
#include <AbstractSyntaxTree/ASTNodes/TypeDeclNode.h>
#include <AbstractSyntaxTree/ASTNodes/ModuloNode.h>
#include <AbstractSyntaxTree/ASTNodes/TypeNode.h>
#include <AbstractSyntaxTree/ASTNodes/TypeDeclBodyNode.h>

#include <Lexical/Tokens/SallocToken.h>
#include <AbstractSyntaxTree/ASTNodes/DereferenceNode.h>
#include <Types/UnverifiedType.h>
#include <Lexical/Tokens/TextToken.h>
#include <Error/Errors.h>
#include <Types/TypeTable.h>
#include <AbstractSyntaxTree/ASTNodes/MemberAccessNode.h>
#include <AbstractSyntaxTree/ASTNodes/InitializerListNode.h>
#include <AbstractSyntaxTree/ASTNodes/AndNode.h>
#include <AbstractSyntaxTree/ASTNodes/OrNode.h>
#include <AbstractSyntaxTree/ASTNodes/BooleanLiteralNode.h>

std::vector<ACC::Rule> ACC::data::getRules() {
    return { // vector
        {{Symbol::start, {Symbol::ptr_assign}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});

        }},
        {{Symbol::start, {Symbol::assignment, Symbol::EOS}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},
        {{Symbol::start, {Symbol::keyword, Symbol::EOS}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},
        {{Symbol::start, {Symbol::function, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},
        {{Symbol::start, {Symbol::if_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},
        {{Symbol::start, {Symbol::for_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},
        {{Symbol::start, {Symbol::while_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},

        {{Symbol::start, {Symbol::type_decl}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new SeqNode(AstOperator::SEQ, {process(children[0], tree, nullptr)});
        }},


        {{Symbol::start, {Symbol::assignment, Symbol::EOS, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[2], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::function, Symbol::EXTENT, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[2], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::if_construct, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
                auto vec = {process(children[1], tree, nullptr), process(children[0], tree, nullptr)};
                return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::ptr_assign, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::while_construct, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::for_construct, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::keyword, Symbol::EOS, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[2], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},

        {{Symbol::start, {Symbol::type_decl, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},
        {{Symbol::start, {Symbol::trait, Symbol::EXTENT, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[2], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},

        {{Symbol::start, {Symbol::expr, Symbol::EOS, Symbol::start}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            auto vec = {process(children[2], tree, nullptr), process(children[0], tree, nullptr)};
            return new SeqNode(AstOperator::SEQ, vec);
        }},

        {{Symbol::start, {Symbol::expr, Symbol::EOS}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            return process(children[0], tree, nullptr);
        }},

        {{Symbol::ptr_assign, {Symbol::expr, Symbol::ASSIGN, Symbol::expr, Symbol::EOS}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[0]->children[1], tree, nullptr), process(children[2], tree, nullptr)};
            return new PtrAssignmentNode(AstOperator::PTR_ASSIGN, vec);
        }},

        {{Symbol::assignment, {Symbol::expr, Symbol::ASSIGN, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::vector<ASTNode*> vec = {process(children[0], tree, nullptr),
                                         process(children[2], tree, nullptr)};
            return new ReassignNode(AstOperator::REASSIGN, vec);
        }},

        {{Symbol::assignment, {Symbol::STAR, Symbol::expr, Symbol::ASSIGN, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::vector<ASTNode*> vec = {process(children[1], tree, nullptr),
                                         process(children[3], tree, nullptr)};
            return new PtrAssignmentNode(AstOperator::PTR_ASSIGN, vec);
        }},

        {{Symbol::assignment, {Symbol::VAR, Symbol::TEXT, Symbol::COLON, Symbol::type, Symbol::ASSIGN, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::vector<ASTNode*> vec = {
                    new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[1]->token)->data),
                    process(children[3], tree, nullptr),
                    process(children[5], tree, nullptr)};
            return new AssignNode(AstOperator::ASSIGN, vec);
        }},


        {{Symbol::if_construct, {Symbol::IF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            return new IfConstructNode(AstOperator::IF_CONSTRUCT, {new ASTNode(AstOperator::IF, vec)});
        }},

        {{Symbol::while_construct, {Symbol::WHILE, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            return new WhileNode(AstOperator::WHILE, vec);
        }},

        {{Symbol::if_construct, {Symbol::IF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT, Symbol::elseIf_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            auto node = new IfConstructNode(AstOperator::IF_CONSTRUCT, {new ASTNode(AstOperator::IF, vec)});

            process(children[6], tree, node);
            return node;
        }},

        {{Symbol::if_construct, {Symbol::IF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT, Symbol::elseIf_construct, Symbol::else_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            auto node = new IfConstructNode(AstOperator::IF_CONSTRUCT, {new ASTNode(AstOperator::IF, vec)});

            process(children[6], tree, node);

            node->children.push_back(process(children[7], tree, nullptr));

            return node;
        }},

        {{Symbol::if_construct, {Symbol::IF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT, Symbol::else_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto ifVec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            auto constructVec = {new ASTNode(AstOperator::IF, ifVec), process(children[6], tree, nullptr)};
            return new IfConstructNode(AstOperator::IF_CONSTRUCT, constructVec);

        }},

        {{Symbol::elseIf_construct, {Symbol::ELIF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            carry->children.push_back(new ASTNode(AstOperator::ELIF, vec));
            return nullptr;
        }},

        {{Symbol::elseIf_construct, {Symbol::ELIF, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT, Symbol::elseIf_construct}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[4], tree, nullptr)};
            carry->children.push_back(new ASTNode(AstOperator::ELIF, vec));
            process(children[6], tree, carry);
            return nullptr;
        }},

        {{Symbol::else_construct, {Symbol::ELSE, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[3], tree, nullptr)};
            return new ASTNode(AstOperator::ELSE, vec);

        }},

        {{Symbol::for_construct, {Symbol::FOR, Symbol::expr, Symbol::ARROW, Symbol::expr, Symbol::COLON, Symbol::INDENT, Symbol::start, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::vector<ASTNode*> vec = {
                    process(children[1], tree, nullptr),
                    process(children[3], tree, nullptr),
                    process(children[6], tree, nullptr)};
            return new ForNode(AstOperator::FOR, vec);

        }},


            {{Symbol::for_construct, {Symbol::FOR, Symbol::CMP, Symbol::type, Symbol::CMP, Symbol::TEXT,
                                      Symbol::ASSIGN, Symbol::expr, Symbol::ARROW, Symbol::expr, Symbol::COLON,
                                      Symbol::INDENT, Symbol::start, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){

                std::vector<ASTNode*> assignVec = {
                        new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[4]->token)->data),
                        process(children[2], tree, nullptr),
                        process(children[6], tree, nullptr)};
                auto ass = new AssignNode(AstOperator::ASSIGN, assignVec);

                std::vector<ASTNode*> vec = {
                    ass,
                    process(children[8], tree, nullptr),
                    process(children[11], tree, nullptr)};
            return new ForNode(AstOperator::FOR, vec);

        }},


        {{Symbol::function, {Symbol::FUNCTION, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsDecl, Symbol::CLOSED_BRACKET,
                             Symbol::ARROW, Symbol::type, Symbol::COLON, Symbol::INDENT, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){

            std::vector<ASTNode*> vec;
            vec.push_back(new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[1]->token)->data));

            vec.push_back(process(children[6], tree, nullptr));

            auto params = children[3];
            while(params != nullptr){
                std::vector<ASTNode*> paramsVec = {
                        new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(params->children[0]->token)->data),
                        process(params->children[2], tree, nullptr)
                };
                vec.push_back(new ASTNode(AstOperator::__CONTAINER, paramsVec));

                if(params->children.size() == 5)
                    params = params->children[4];
                else
                    params = nullptr;
            }

            vec.push_back(process(children[9], tree, nullptr));

            return new FunctionNode(AstOperator::FUNCTION, vec);
        }},

        {{Symbol::function, {Symbol::FUNCTION, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET, Symbol::ARROW, Symbol::type,
                                    Symbol::COLON, Symbol::INDENT, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){

            std::vector<ASTNode*> vec;
            vec.push_back(new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[1]->token)->data));
            vec.push_back(process(children[5], tree, nullptr));

            vec.push_back(process(children[8], tree, nullptr));

            return new FunctionNode(AstOperator::FUNCTION, vec);
        }},


        {{Symbol::trait, {Symbol::TRAIT, Symbol::CMP, Symbol::TEXT, Symbol::CMP, Symbol::TEXT,
                          Symbol::OPEN_BRACKET, Symbol::paramsDecl, Symbol::CLOSED_BRACKET,
                          Symbol::ARROW, Symbol::type, Symbol::COLON, Symbol::INDENT, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){

            std::vector<ASTNode*> vec;

            auto typeName = dynamic_cast<TextToken*>(children[2]->token)->data;

            auto prefix = "?" + typeName + ".";
            vec.push_back(new IdNode(AstOperator::ID, prefix + dynamic_cast<TextToken*>(children[4]->token)->data));

            vec.push_back(process(children[9], tree, nullptr)); //type

            vec.push_back(new ASTNode(AstOperator::__CONTAINER, {
                    new IdNode(AstOperator::ID, "this"),
                    new TypeNode(AstOperator::TYPE_DEF, UnverifiedType::createPtr(typeName))
            }));

            auto params = children[6];
            while(params != nullptr){
                std::vector<ASTNode*> paramsVec = {
                        new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(params->children[0]->token)->data),
                        process(params->children[2], tree, nullptr)
                };
                vec.push_back(new ASTNode(AstOperator::__CONTAINER, paramsVec));

                if(params->children.size() == 5)
                    params = params->children[4];
                else
                    params = nullptr;
            }

            vec.push_back(process(children[12], tree, nullptr));

            return new FunctionNode(AstOperator::FUNCTION, vec);
        }},

        {{Symbol::trait, {Symbol::TRAIT,Symbol::CMP, Symbol::TEXT, Symbol::CMP, Symbol::TEXT,
                          Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET, Symbol::ARROW, Symbol::type,
                          Symbol::COLON, Symbol::INDENT, Symbol::start}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){

            auto typeName = dynamic_cast<TextToken*>(children[2]->token)->data;
            auto prefix = "?" + typeName + ".";

            std::vector<ASTNode*> vec;
            vec.push_back(new IdNode(AstOperator::ID,prefix + dynamic_cast<TextToken*>(children[4]->token)->data));
            vec.push_back(process(children[8], tree, nullptr)); //Type

            vec.push_back(new ASTNode(AstOperator::__CONTAINER, {
                    new IdNode(AstOperator::ID, "this"),
                    new TypeNode(AstOperator::TYPE_DEF, UnverifiedType::createPtr(typeName))
            }));

            vec.push_back(process(children[11], tree, nullptr)); // start

            return new FunctionNode(AstOperator::FUNCTION, vec);
        }},

        {{Symbol::call, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsList, Symbol::CLOSED_BRACKET}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){

            std::vector<ASTNode*> vec;
            vec.push_back(new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data));

            ParseNode* params = children[2];
            while(params != nullptr){
                vec.push_back(process(params->children[0], tree, nullptr));
                if(params->children.size() == 3)
                    params = params->children[2];
                else
                    params = nullptr;
            }

            return new CallNode(AstOperator::CALL, vec);
        }},


        {{Symbol::call, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::vector<ASTNode*> vec = {new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data)};
             return new CallNode(AstOperator::CALL, vec);
        }},


        {{Symbol::type, {Symbol::TEXT, Symbol::CMP, Symbol::TEXT, Symbol::CMP}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::string str = dynamic_cast<TextToken*>(children[2]->token)->data;
            UnverifiedType t = UnverifiedType::createPtr(str);

            return new TypeNode(AstOperator::TYPE_DEF, t);
        }},

        {{Symbol::type, {Symbol::TEXT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::string str = dynamic_cast<TextToken*>(children[0]->token)->data;
            UnverifiedType t = UnverifiedType(str);

            return new TypeNode(AstOperator::TYPE_DEF, t);
        }},

        
        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr,
                            Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr,
                            Symbol::COMMA, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[3], tree, nullptr), process(children[5],
                                                                                                          tree, nullptr)
                    , process(children[7], tree, nullptr), process(children[9], tree, nullptr), process(children[11], tree,
                                                                                                        nullptr),
                        process(children[13], tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},

        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr,
                            Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr}},
         [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[3], tree, nullptr), process(children[5],
                                                                                                          tree, nullptr)
                    , process(children[7], tree, nullptr), process(children[9], tree, nullptr), process(children[11],
                                                                                                        tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},


        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr,
                            Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[3], tree, nullptr), process(children[5],
                                                                                                          tree, nullptr)
                    , process(children[7], tree, nullptr), process(children[9], tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},


        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr,
                            Symbol::COMMA, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[3], tree, nullptr), process(children[5],
                                                                                                          tree, nullptr)
                    , process(children[7], tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},


        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA, Symbol::expr, Symbol::COMMA, Symbol::expr}},
         [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[3], tree, nullptr), process(children[5],
                                                                                                          tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},


        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr, Symbol::COMMA, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr), process(children[3], tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},

        {{Symbol::keyword, {Symbol::SYSCALL, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto vec = {process(children[1], tree, nullptr)};
            return new SyscallNode(AstOperator::SYSCALL, vec);
        }},
        
        {{Symbol::keyword, {Symbol::EXIT, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new ExitNode(AstOperator::EXIT,
                               {process(children[1], tree, nullptr)});
        }},

        {{Symbol::keyword, {Symbol::RETURN, Symbol::expr}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            return new ReturnNode(AstOperator::RETURN,
                               {process(children[1], tree, nullptr)});
        }},

        {{Symbol::keyword, {Symbol::SALLOC, Symbol::expr, Symbol::COMMA, Symbol::TEXT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::vector<ASTNode*> vec = {process(children[1], tree, nullptr),
                        new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[3]->token)->data)};
            return new SallocNode(AstOperator::SALLOC, vec);
        }},


        {{Symbol::type_decl, {Symbol::TYPE, Symbol::TEXT, Symbol::COLON, Symbol::INDENT, Symbol::type_decl_body, Symbol::EXTENT}}, [](auto children, auto carry, ACC::AbstractSyntaxTree& tree){
            auto out = new TypeDeclNode(AstOperator::TYPE_DECL, dynamic_cast<TextToken*>(children[1]->token)->data);

            tree.typeExports.push_back(TypeExport(dynamic_cast<TextToken*>(children[1]->token)->data));

            process(children[4], tree, out);

            return out;
        }},

        {{Symbol::type_decl_body, {Symbol::TEXT, Symbol::COLON, Symbol::type, Symbol::EOS}}, [](auto children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            TypeNode* typeNode = (TypeNode*)process(children[2], tree, nullptr);

            carry->children.push_back(
                    new TypeDeclBodyNode(
                            AstOperator::TYPE_DECL_BODY,
                            {typeNode},
                            dynamic_cast<TextToken*>(children[0]->token)->data));

            tree.typeExports.rbegin()->fields.emplace_back(dynamic_cast<TextToken*>(children[0]->token)->data, typeNode);

            return nullptr;
        }},

        {{Symbol::type_decl_body, {Symbol::TEXT, Symbol::COLON, Symbol::type, Symbol::EOS, Symbol::type_decl_body}}, [](auto children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            TypeNode* typeNode = (TypeNode*)process(children[2], tree, nullptr);


            carry->children.push_back(
                    new TypeDeclBodyNode(
                            AstOperator::TYPE_DECL_BODY,
                            {typeNode},
                            dynamic_cast<TextToken*>(children[0]->token)->data));

            tree.typeExports.rbegin()->fields.emplace_back(dynamic_cast<TextToken*>(children[0]->token)->data, typeNode);

            process(children[4], tree, carry);

            return nullptr;
        }},


        {{Symbol::expr, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
                return new CallNode(AstOperator::CALL,
                                   {new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data)});
        }},

        {{Symbol::expr, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            auto call = new CallNode(AstOperator::CALL,
                               {new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data)});
            return process(children[3], tree, call);
        }},

        {{Symbol::expr, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsList, Symbol::CLOSED_BRACKET}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            std::vector<ASTNode*> vec;
            vec.push_back(new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data));

            ParseNode* params = children[2];
            while(params != nullptr){
                vec.push_back(process(params->children[0], tree, nullptr));
                if(params->children.size() == 3)
                    params = params->children[2];
                else
                    params = nullptr;
            }

            return new CallNode(AstOperator::CALL, vec);
        }},

        {{Symbol::expr, {Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsList, Symbol::CLOSED_BRACKET, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            std::vector<ASTNode*> vec;
            vec.push_back(new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data));

            ParseNode* params = children[2];
            while(params != nullptr){
                vec.push_back(process(params->children[0], tree, nullptr));
                if(params->children.size() == 3)
                    params = params->children[2];
                else
                    params = nullptr;
            }

            auto call = new CallNode(AstOperator::CALL, vec);
            return process(children[4], tree, call);
        }},

        {{Symbol::expr, {Symbol::TEXT}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            std::string str = dynamic_cast<TextToken*>(children[0]->token)->data;

            bool isNumber = true;
            for(auto const & c : str){
                if(c < 48 || c > 57)
                    isNumber = false;
            }

            if(isNumber){
                GeneralDataStore value;
                value.storeT(std::stoul(str));
                Type t = Type("num", 8);
                return new LiteralNode(AstOperator::LITERAL, value, t);

            } else {
                return new IdNode(AstOperator::ID, dynamic_cast<TextToken*>(children[0]->token)->data);
            }

        }},

        {{Symbol::expr, {Symbol::TEXT, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            std::string str = dynamic_cast<TextToken*>(children[0]->token)->data;

            bool isNumber = true;
            for(auto const & c : str){
                if(c < 48 || c > 57)
                    isNumber = false;
            }

            if(isNumber){
                GeneralDataStore value;
                value.storeT(std::stoul(str));
                Type t = Type("num", 8);
                auto literal = new LiteralNode(AstOperator::LITERAL, value, t);
                return process(children[1], tree, literal);
            } else {
                return process(children[1], tree,
                               new IdNode(AstOperator::ID, dynamic_cast<TextToken *>(children[0]->token)->data));
            }

        }},

        {{Symbol::expr, {Symbol::DOUBLE_QUOTE, Symbol::TEXT, Symbol::DOUBLE_QUOTE}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::string str = dynamic_cast<TextToken*>(children[1]->token)->data;

            GeneralDataStore value;
            Type t = Type::createPtr("char");

            value.storeT(str);

            return new LiteralNode(AstOperator::LITERAL, value, t);
        }},


        {{Symbol::expr, {Symbol::QUOTE, Symbol::TEXT, Symbol::QUOTE}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::string str = dynamic_cast<TextToken*>(children[1]->token)->data;

            GeneralDataStore value;
            Type t = Type("char", 1);

            value.storeT(str[0]);

            return new LiteralNode(AstOperator::LITERAL, value, t);
        }},

         {{Symbol::expr, {Symbol::OPEN_BRACKET, Symbol::expr, Symbol::CLOSED_BRACKET}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree){
             return process(children[1], tree, nullptr);
         }},

        {{Symbol::expr, {Symbol::OPEN_BRACKET, Symbol::expr, Symbol::CLOSED_BRACKET, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree){
            return process(children[3], tree, process(children[1], tree, nullptr));
        }},

         {{Symbol::expr, {Symbol::STAR, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
             if(carry != nullptr) // Its binary therefor multiplications
                 return new MultiplicationNode(AstOperator::MULTIPLICATION, {carry, process(children[1], tree, nullptr)});

             // Its unary therefor dereference
             return new DereferenceNode(AstOperator::DEREFERENCE, {process(children[1], tree, nullptr)});
         }},

        {{Symbol::expr, {Symbol::TRUE, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            return process(children[1], tree, new BooleanLiteralNode(AstOperator::BOOLEAN_LITERAL, true));
        }},

        {{Symbol::expr, {Symbol::FALSE, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            return process(children[1], tree, new BooleanLiteralNode(AstOperator::BOOLEAN_LITERAL, false));
        }},

        {{Symbol::expr, {Symbol::TRUE}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            return new BooleanLiteralNode(AstOperator::BOOLEAN_LITERAL, true);
        }},

        {{Symbol::expr, {Symbol::FALSE}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            return new BooleanLiteralNode(AstOperator::BOOLEAN_LITERAL, false);
        }},

        {{Symbol::expr, {Symbol::AND, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};
            return new AndNode(AstOperator::AND, vec);
        }},

        {{Symbol::expr, {Symbol::OR, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};
            return new OrNode(AstOperator::OR, vec);
        }},

        {{Symbol::expr, {Symbol::PLUS, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};
            return new AddNode(AstOperator::ADD, vec);
        }},

        {{Symbol::expr, {Symbol::MINUS, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};
            return new SubtractNode(AstOperator::SUBTRACT, vec);
        }},

        {{Symbol::expr, {Symbol::SLASH, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};
            return new DivisionNode(AstOperator::DIVISION, vec);
        }},

        {{Symbol::expr, {Symbol::MODULO, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};
            return new ModuloNode(AstOperator::DIVISION, vec);
        }},

        {{Symbol::expr, {Symbol::CMP, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) -> ASTNode*{
            auto vec = {carry, process(children[1], tree, nullptr)};

            switch (dynamic_cast<ComparisionToken*>(children[0]->token)->kind){
                case ComparisionTokenKind::Equal:
                    return new ComparisionNode(AstOperator::EQUAL, vec, ComparisionType::EQ);
                case ComparisionTokenKind::NotEqual:
                    return new ComparisionNode(AstOperator::NOT_EQUAL, vec, ComparisionType::NEQ);
                case ComparisionTokenKind::Less:
                    return new ComparisionNode(AstOperator::LESS, vec, ComparisionType::LT);
                case ComparisionTokenKind::Greater:
                    return new ComparisionNode(AstOperator::GREATER, vec, ComparisionType::GT);
                case ComparisionTokenKind::LessEqual:
                    return new ComparisionNode(AstOperator::LESS_EQUAL, vec, ComparisionType::LET);
                case ComparisionTokenKind::GreaterEqual:
                    return new ComparisionNode(AstOperator::GREATER_EQUAL, vec, ComparisionType::GET);
            }
        }},

        {{Symbol::expr, {Symbol::NOT, Symbol::expr}},[](std::vector < ACC::ParseNode * >  children, auto carry, ACC::AbstractSyntaxTree& tree) {
            return new NotNode(AstOperator::NOT, {process(children[1], tree, nullptr)});
        }},

        {{Symbol::expr, {Symbol::DOT, Symbol::TEXT}}, [](std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            // In expression obj.b `obj` would be in carry

            auto member = dynamic_cast<TextToken*>(children[1]->token)->data;

            return new MemberAccessNode(AstOperator::MEMBER_ACCESS, {carry},  member);
        }},

        {{Symbol::expr, {Symbol::DOT, Symbol::TEXT, Symbol::expr }}, [](std::vector<ParseNode*> children, auto carry, ACC::AbstractSyntaxTree& tree){
            std::string member = dynamic_cast<TextToken*>(children[1]->token)->data;

            auto out = new MemberAccessNode(AstOperator::MEMBER_ACCESS, {carry},  member);
            return process(children[2], tree, out);
        }},

        {{Symbol::expr, {Symbol::DOT, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::CLOSED_BRACKET}}, [](std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            // In expression obj.b `obj` would be in carry

            auto traitName = dynamic_cast<TextToken*>(children[1]->token)->data;

            return new MemberCallNode(AstOperator::MEMBER_CALL, traitName, {carry});
        }},

        {{Symbol::expr, {Symbol::DOT, Symbol::TEXT, Symbol::OPEN_BRACKET, Symbol::paramsList, Symbol::CLOSED_BRACKET}}, [](std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            // In expression obj.b `obj` would be in carry

            auto traitName = dynamic_cast<TextToken*>(children[1]->token)->data;

            std::vector<ASTNode*> vec = {carry};

            ParseNode* params = children[3];
            while(params != nullptr){
                vec.push_back(process(params->children[0], tree, nullptr));
                if(params->children.size() == 3)
                    params = params->children[2];
                else
                    params = nullptr;
            }

            return new MemberCallNode(AstOperator::MEMBER_CALL, traitName, vec);
        }},

        {{Symbol::expr, {Symbol::OPEN_CURLY, Symbol::initializer_list, Symbol::CLOSED_CURLY}},  [](std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            auto out = new InitializerListNode(AstOperator::INITIALIZER_LIST, {});

            process(children[1], tree, out);
            return out;
        }},

        {{Symbol::initializer_list, {Symbol::expr, Symbol::COMMA,  Symbol::initializer_list}}, [](std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            carry->children.push_back(process(children[0], tree, nullptr));

            process(children[2], tree, carry);

            return nullptr;
        }},

        {{Symbol::initializer_list, {Symbol::expr}}, [](std::vector<ParseNode*> children, ASTNode* carry, ACC::AbstractSyntaxTree& tree){
            carry->children.push_back(process(children[0], tree, nullptr));
            return nullptr;
        }},

    };
}
