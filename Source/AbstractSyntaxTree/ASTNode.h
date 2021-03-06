//
// Created by a_mod on 23.01.2019.
//

#ifndef SIMSYSCOMPILER_ASSNODE_H
#define SIMSYSCOMPILER_ASSNODE_H

#include <initializer_list>
#include <Parser/ParseNode.h>
#include <memory>
#include <General/GeneralDataStore.h>
#include <Types/Type.h>
#include <Structure/Structure.h>

namespace ACC {

    /*! A enum keeping track of the different kind of comparisions, that are possible.
     * */
    enum class ComparisionType {
        EQ, // Equal
        LT, // Less then
        GT, // Greater then
        NEQ, // Not equal
        LET, // Less or Equal Then
        GET // Greater or Equal Then
    };

    /*! A enum which contains all operations a `ASTNode` can do.
     * */
    enum class AstOperator {
        BOOLEAN_LITERAL,
        INITIALIZER_LIST,
        AND,
        OR,
        ADD,
        MEMBER_ACCESS,
        MEMBER_CALL,
        NOT,
        TYPE_DECL_BODY,
        TYPE_DECL,
        SUBTRACT,
        DEREFERENCE,
        MULTIPLICATION,
        DIVISION,
        LITERAL,
        SEQ,
        ASSIGN,
        REASSIGN,
        ID,
        SYSCALL,
        EXIT,
        FUNCTION,
        CALL,
        RETURN,
        TYPE_DEF,
        IF_CONSTRUCT,
        IF,
        ELIF,
        ELSE,
        EQUAL,
        NOT_EQUAL,
        LESS,
        GREATER,
        LESS_EQUAL,
        GREATER_EQUAL,
        WHILE,
        FOR,
        PTR_ASSIGN,
        SALLOC,

        __NONE,
        __CONTAINER
    };

    /*! This class is each individual node of the Abstract Syntax Tree.
     * The ASTNode has a unspecified amount of children, a operator in the form of an
     * `ASTOperator` and may keep arbitrary data in the form of an`GeneralDataStore`.
     * When refering to a "type" of `ASTNode` we usually mean a ASTNode associated with a ASTOperator and its class
     * inheriting from ASTNode. For each operator there is a class deriving of ASTNode, which is implementing
     * `ACC::ASTNode::generate`.
     * */
    class ASTNode {
    public:
        /*! Prints the tree from this node down. */
        void _print(std::string indent, bool isLast) const;

        /*! Constructor used for most types of ASTNodes. */
        ASTNode(AstOperator op, std::vector<ASTNode *> children);

        explicit ASTNode();

        ~ASTNode();

        [[nodiscard]] virtual std::string createRepresentation() const;

        /*! Generates the assembly code for this node.
         * Might call generate for other ASTNodes, calling generate on the root node of a Abstract Syntax Tree
         * will cause the generation of the assembly of the entire program.
         * @param code The `code` object, which will contain the assembly code.*/
        virtual std::shared_ptr<Structure> generate(Code &code);

        std::vector<ASTNode *> children;

        AstOperator op;

        size_t lineNum;
        std::string lineContent;

    };

}

#endif //SIMSYSCOMPILER_ASSNODE_H
