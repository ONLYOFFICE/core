#include "parser_data_v5_x_x.h"

namespace StarMathParser::v5_x_x
{

const std::shared_ptr<TVecParserPriorityPair> vDividers = std::make_shared<TVecParserPriorityPair>(
    TVecParserPriorityPair{
        {"##", -1},
        {"#", -1}
    }
);

const std::shared_ptr<TVecParserPriorityPair> vOperators = std::make_shared<TVecParserPriorityPair>(
	TVecParserPriorityPair{
		{"=", 30}, // a = b
		{"+", 35},
		{"-", 35},
		{"/", 45},
		{"*", 45},
		{"^", 50},
		{"%", 45},
		{"'", 45},
		{"{", 99},
		{"}", 99},
		{"(", 100},
		{")", 100},

// a cdot b    = a⋅b
// a times b   = a×b
// neg a       = ¬a
// a and b     = a∧b
// a over b    = a/b
// a div b     = a÷b
// a or b      = a ∨ b
// a circ b    = a°b
// a <> b      = a≠2
// a approx 2 a≈2
// a divides b = a∣b
// a ndivides b = a∤b
// a<2         = a2
// a>2         = a2
// a simeq b   = a≃b
// a parallel b = a∥b
// a ortho b   = a⊥b
// a leslant b = ab
// a geslant b = ab
// a sim b     = a~b
// a equiv b   = a≡b
// a <= b      = a≤b
// a >= b      = a≥b
// a prop b    = a∝b
// a toward b  = ab
// a dlarrow b = a⇐b
// a dlrarrow b = a⇔b
// a drarrow b = a⇒b
// a in B      = a∈B
// a notin B   = a∉B
// A owns b    = A∋b
// emptyset    = ∅
// A intersection B = A∩B
// A union B   = A∪B
// A setminus B = A∖B
// A slash B   = A/ B
// aleph       = ℵ
// A subset B  = A⊂B
// A subseteq B = A⊆B
// A supset B  = A⊃B
// A supseteq B = A⊇B
// A nsubset B = A⊄B
// A nsubseteq B = A⊈B
// A nsupset B = A⊅B
// A nsupseteq B = A⊉B
//
// CONSTATNTS
// setN = ℕ
// setZ = ℤ
// setQ = ℚ
// setR = ℝ
// setC = ℂ



//    {"+-", 100},
//    {"-+", 100},
//    {"over", 100},
//    {"wideslash", 100},
	}
);


const std::vector<std::pair<TString, int>> vFunctions = {
	{"sqrt", 10},
	{"sin", 10},
	{"cos", 10},
	{"tan", 10},
};

}
