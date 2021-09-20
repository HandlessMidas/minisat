#ifndef Minisat_Bench_h
#define Minisat_Bench_h

#include <stdio.h>

#include "minisat/utils/ParseUtils.h"
#include "minisat/core/SolverTypes.h"

namespace Minisat {

template<class B>
static int parseVar(B& in) {
    int val = 0;
    skipWhitespace(in);
    if (*in != 'G') fprintf(stderr, "PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
    ++in;
    val = parseInt(in);
    return val - 1;
}

template<class B, class Solver>
static void addInOut(B& in, Solver& S, int var) {
    vec<Lit> lits;
    lits.push(mkLit(var));
    S.addClause_(lits);
}

template<class B, class Solver>
static void addOp(B& in, Solver& S, int g) {
    vec <Lit> lits;
    if (eagerMatch(in, "OR(")) {
        int h1 = parseVar(in);
        if (*in != ',') printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
        in++;
        skipWhitespace(in);
        int h2 = parseVar(in);

        lits.push(mkLit(g));
        lits.push(~mkLit(h1))
        s.addClause_(lits);
        lits.clear();

        lits.push(mkLit(g));
        lits.push(~mkLit(h2));
        s.addClause_(lits);
        lits.clear();

        lits.push(~mkLit(g));
        lits.push(mkLit(h1));
        lits.push(mkLit(h2));
        s.addClause_(lits);
    } else if (eagerMatch(in, "AND(")) {
        int h1 = parseVar(in);
        if (*in != ',') printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
        in++;
        skipWhitespace(in);
        int h2 = parseVar(in);

        lits.push(~mkLit(g));
        lits.push(mkLit(h1))
        s.addClause_(lits);
        lits.clear();

        lits.push(~mkLit(g));
        lits.push(mkLit(h2));
        s.addClause_(lits);
        lits.clear();

        lits.push(mkLit(g));
        lits.push(~mkLit(h1));
        lits.push(~mkLit(h2));
        s.addClause_(lits);
    } else if (eagerMatch(in, "NOT(")) {
        int h = parseVar(in);
        
        lits.push(mkLit(g));
        lits.push(mkLit(h))
        s.addClause_(lits);
        lits.clear();

        lits.push(~mkLit(g));
        lits.push(~mkLit(h));
        s.addClause_(lits);
    }
}

template<class B, class Solver>
static void parse_BENCH_main(B& in, Solver& S) {
    int var = 0;
    for (;;) {
        skipWhitespace(in);
        if (*in == EOF) break;
        else if (*in == 'I' || *in == 'O') {
            if (eagerMatch(in, "INPUT(") || eagerMatch(in, "OUTPUT(")) {
                var = parseVar(in);
                addInOut(in, S, var);
                if (*in != ')') printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
                ++in;
            } else {
                printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
            }
        } else if (*in == 'G') {
            var = parseVar(in);
            skipWhitespace(in);
            if (*in != '=') printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
            ++in;
            skipWhitespace(in);
            addOp(in, S);            
        } else if (*in == '#')
            skipLine(in);
        else{
            printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
        }
    }
}

template<class Solver>
static void parse_BENCH(gzFile input_stream, Solver& S) {
    StreamBuffer in(input_stream);
    parse_BENCH_main(in, S);
}

}

#endif