// parser.h
#pragma once
#if !defined(__SOLVER_H)
#define __SOLVER_H
/*
       number_digit = '0'|'1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9' 

       number = number number_digit | number_digit 

        identifier = 'pi'|'left'|'top'|'right'|'bottom'|'xstretch'|'ystretch'| 
                      'hasstroke'|'hasfill'|'width'|'height'|'logwidth'|'logheight' 

       unary_function = 'abs'|'sqrt'|'sin'|'cos'|'tan'|'atan'|'atan2' 
       binary_function = 'min'|'max' 
       ternary_function = 'if' 

        function_reference = '?' 'a-z,A-Z,0-9' ' ' 
       modifier_reference = '$' '0-9' ' ' 
                                  
       basic_expression =  
            number | 
            identifier | 
            function_reference |
            modifier_reference |
            unary_function '(' additive_expression ')' |
			binary_function '(' additive_expression ',' additive_expression ')' |
			ternary_function '(' additive_expression ',' additive_expression ',
                   ' additive_expression ')' | '(' additive_expression ')'
			unary_expression = '-' basic_expression
			multiplicative_expression = 
			  basic_expression |
			  multiplicative_expression '*' basic_expression |
			  multiplicative_expression '/' basic_expression
			additive_expression = 
              multiplicative_expression |
			  additive_expression '+' multiplicative_expression |
			  additive_expression '-' multiplicative_expression
*/
// Includes
//#include "stdafx.h"
#include <vector>
using namespace std;

namespace NSGuidesOdp
{
	class CFormulaManager;
}

struct TParserNode
{
   double value;
   TParserNode *left;
   TParserNode *right;
   TParserNode *third;

   TParserNode(double _value=0.0, TParserNode *_left=NULL, TParserNode *_right=NULL, TParserNode *_third=NULL)
      { value = _value; left = _left; right = _right; third = _third; }
};

struct TError
{
   CString error;
   int pos;

   TError() {};
   TError(CString _error, int _pos) { error=_error; pos=_pos; }
};

class TParser
{
  private:
   TParserNode *root;
   CString expr;
   CString curToken;//[MAX_TOKEN_LEN];
   enum { PARSER_PLUS, PARSER_MINUS, PARSER_MULTIPLY, PARSER_DIVIDE, PARSER_PERCENT, PARSER_POWER, PARSER_COMMA,
          PARSER_SIN, PARSER_COS, PARSER_TG, PARSER_CTG, PARSER_ARCSIN, PARSER_ARCCOS, PARSER_ARCTG, PARSER_ARCCTG, PARSER_SH, PARSER_CH, PARSER_TH, PARSER_CTH,
          PARSER_EXP, PARSER_LG, PARSER_LN, PARSER_SQRT, PARSER_ABS,
		  PARSER_MIN, PARSER_MAX, PARSER_ATAN2, 
		  PARSER_IF,
		  PARSER_GUIDE, PARSER_ADJUST, PARSER_L_BRACKET, PARSER_R_BRACKET, PARSER_E, PARSER_PI, PARSER_NUMBER, PARSER_END } typToken;
   int pos;

   double result;

   vector<TParserNode *> history;

  private:
   TParserNode *CreateNode(double _value=0.0, TParserNode *_left=NULL, TParserNode *_right=NULL, TParserNode *_third=NULL);

   TParserNode *Expr(NSGuidesOdp::CFormulaManager& pFManager);
   TParserNode *Expr1(NSGuidesOdp::CFormulaManager& pFManager);
   TParserNode *Expr2(NSGuidesOdp::CFormulaManager& pFManager);
   TParserNode *Expr3(NSGuidesOdp::CFormulaManager& pFManager);
   TParserNode *Expr4(NSGuidesOdp::CFormulaManager& pFManager);
   TParserNode *Expr5(NSGuidesOdp::CFormulaManager& pFManager);

   bool GetToken();
   bool IsSpace(void)  {CString string(_T(" "));          return (string.Find(expr[pos]) >= 0);}
   bool IsDelim(void)  {CString string(_T("+-*/%^()[]")); return (string.Find(expr[pos]) >= 0);}
   bool IsAdjust(void) {CString string(_T("$"));          return (string.Find(expr[pos]) >= 0);}
   bool IsGuide(void)  {CString string(_T("?"));          return (string.Find(expr[pos]) >= 0);}
   bool IsComma(void)  {CString string(_T(","));          return (string.Find(expr[pos]) >= 0);}
   
   bool IsLetter(void) { return ((expr[pos]>=_T('a') && expr[pos]<=_T('z')) ||
                                 (expr[pos]>=_T('A') && expr[pos]<=_T('Z'))); }
   bool IsDigit(void) { return (expr[pos]>=_T('0') && expr[pos]<=_T('9')); }
   bool IsPoint(void) { return (expr[pos]==_T('.')); }

   double CalcTree(TParserNode *tree);
   void  DelTree(TParserNode *tree);

   void SendError(int errNum);

  public:
   TParser() { result = 0.0; root = NULL; }
   ~TParser() { DelTree(root); root=NULL; }

   bool Compile(CString _expr, NSGuidesOdp::CFormulaManager& pFManager);
   void Decompile() { DelTree(root); root=NULL; }

   double Evaluate(void);

   double GetResult(void) { return result; }
};

#endif