#pragma once
#include "formula.h"
#include "../Path.h"
//#include "parser.cpp"
#include "parser.h"
#include "../PPTShape/formula.h"

#define OP_PLUS          0
#define OP_MINUS         1
#define OP_MULTIPLY      2
#define OP_DIVIDE        3
#define OP_PERCENT       4
#define OP_POWER         5
#define OP_UMINUS        6
#define OP_COMMA         7

#define OP_SIN           10
#define OP_COS           11
#define OP_TG            12
#define OP_CTG           13
#define OP_ARCSIN        14
#define OP_ARCCOS        15
#define OP_ARCTG         16
#define OP_ARCCTG        17
#define OP_SH            18
#define OP_CH            19
#define OP_TH            20
#define OP_CTH           21
#define OP_EXP           22
#define OP_LG            23
#define OP_LN            24
#define OP_SQRT          25
#define OP_ABS			 26
#define OP_MIN			 30
#define OP_MAX			 31
#define OP_ATAN2		 32
#define OP_IF			 40


namespace NSGuidesVML
{
	struct TParserNodeOdp
	{
		CFormParam FormParser;
	   double value;
	   TParserNodeOdp *left;
	   TParserNodeOdp *right;
	   TParserNodeOdp *third;

	   TParserNodeOdp(CFormParam _FormParser, double _value=0.0, TParserNodeOdp *_left=NULL, TParserNodeOdp *_right=NULL, TParserNodeOdp *_third=NULL)
		{ 
			 FormParser = _FormParser;
			  value = _value; 
			  left = _left; 
			  right = _right; 
			  third = _third; 
	   }
	};

	enum ParamPathType
	{
		ptVal = 0,
		ptAdj = 1,
		ptForm = 2,
		ptShape = 3
	};

	class TConvertParser : public TParser
	{
		private:
		   TParserNodeOdp *root;
		   CString expr;
		   CFormula pCurFormula;
		   LONG m_lIndexDst;		   
		public:
			CSimpleArray<CFormula> m_arFormulas;
			CSimpleMap<CString, LONG> mapFormula;
			LONG m_lPiFormulaNum;
		   
		public:
			CString curToken;
			double result;
			double temp;
			int pos;
			enum { PARSER_PLUS, PARSER_MINUS, PARSER_MULTIPLY, PARSER_DIVIDE, PARSER_PERCENT, PARSER_POWER, PARSER_COMMA,
			  PARSER_SIN, PARSER_COS, PARSER_TG, PARSER_CTG, PARSER_ARCSIN, PARSER_ARCCOS, PARSER_ARCTG, PARSER_ARCCTG, PARSER_SH, PARSER_CH, PARSER_TH, PARSER_CTH,
			  PARSER_EXP, PARSER_LG, PARSER_LN, PARSER_SQRT, PARSER_ABS,
			  PARSER_MIN, PARSER_MAX, PARSER_ATAN2, 
			  PARSER_IF,
			  PARSER_GUIDE, PARSER_ADJUST, PARSER_L_BRACKET, PARSER_R_BRACKET, PARSER_E, PARSER_PI, PARSER_NUMBER, PARSER_END, PARSER_ERR } typToken;
			vector<TParserNodeOdp *> history;

		TConvertParser()
		{
			result = 0.0; 
			root = NULL;
			m_lIndexDst = -1;
			m_lPiFormulaNum = -1;
		}

		~TConvertParser() 
		{ 
			root=NULL; 
		}

		NSGuidesVML::FormulaType GetType( int lNum )
		{
			switch (lNum)
			{
				case OP_PLUS:
				case OP_MINUS:
					return ftSum;
				case OP_MULTIPLY:
				case OP_DIVIDE:
					return ftProduct;
				case OP_SIN:
					return ftSin;
				case OP_COS:
					return ftCos;
				case OP_TG:
					return ftTan;
				case OP_SQRT:
					return ftSqrt;
				case OP_ABS:
					return ftAbsolute;
				case OP_MIN:
					return ftMin;
				case OP_MAX:
					return ftMax;
				case OP_ATAN2:
					return ftAtan2;
				case OP_IF:
					return ftIf;
				default:
					return ftVal;
			}
		}


		TParserNodeOdp *TConvertParser::CreateNode(CFormParam _FormParser, double _value, TParserNodeOdp *_left, TParserNodeOdp *_right, TParserNodeOdp *_third)
		{
		   TParserNodeOdp *pNode = new TParserNodeOdp(_FormParser, _value, _left, _right, _third);
		   history.push_back(pNode);
		   return pNode;
		}

		TParserNodeOdp *TConvertParser::CreateNode( CFormParam _FormParser, double _value)
		{
		   TParserNodeOdp *pNode = new TParserNodeOdp(_FormParser, _value, NULL, NULL, NULL);
		   history.push_back(pNode);
		   return pNode;
		}

		TParserNodeOdp *TConvertParser::CreateNode( CFormParam _FormParser, double _value, TParserNodeOdp *_left)
		{
		   TParserNodeOdp *pNode = new TParserNodeOdp(_FormParser, _value, _left, NULL, NULL);
		   history.push_back(pNode);
		   return pNode;
		}

		TParserNodeOdp *TConvertParser::CreateNode( CFormParam _FormParser, double _value, TParserNodeOdp *_left, TParserNodeOdp *_right)
		{
		   TParserNodeOdp *pNode = new TParserNodeOdp(_FormParser, _value, _left, _right, NULL);
		   history.push_back(pNode);
		   return pNode;
		}

		double TConvertParser::Evaluate(CString strNum)
		{		  
		   CalcTree(root);
		   mapFormula.Add(strNum, m_lIndexDst);
		   return 0;
		}


		void TConvertParser::CalcTree(TParserNodeOdp *tree)
		{
		  
		   
		   if(tree->left==NULL && tree->right==NULL)
		   {
			  CFormula pFormula;
			  m_lIndexDst++;
			  pFormula.m_eFormulaType = ftVal;
			  pFormula.m_lParam1 = tree->FormParser.m_lParam;
			  pFormula.m_eType1 = tree->FormParser.m_eType;
			  m_arFormulas.Add(pFormula);
		   }
		   return;
		}

		bool TConvertParser::GetToken()
		{
		   curToken = _T("");
		   
		   if(pos >= expr.GetLength())
		   {
			  curToken = _T("");
			  typToken = PARSER_END;
			  return true;
		   }

		   while(IsSpace()) pos++;

		   if(IsDelim())
		   {
			  curToken = expr[pos++];
			  switch(curToken[0])
			  {
				 case _T('+'): typToken = PARSER_PLUS; return true;
				 case _T('-'): typToken = PARSER_MINUS; return true;
				 case _T('*'): typToken = PARSER_MULTIPLY; return true;
				 case _T('/'): typToken = PARSER_DIVIDE; return true;
				 case _T('%'): typToken = PARSER_PERCENT; return true;
				 case _T('^'): typToken = PARSER_POWER; return true;
				 case _T('['):
				 case _T('('): typToken = PARSER_L_BRACKET; return true;
				 case _T(']'):
				 case _T(')'): typToken = PARSER_R_BRACKET; return true;
			  }
		   }
		   else if(IsComma())
		   {
			   curToken = expr[pos++];
			   typToken = PARSER_COMMA;
			   return true;
		   }
		   else if(IsLetter())
		   {
			  int i=0;
			  curToken = _T("");
			  while(IsLetter() || IsDigit()) curToken += expr[pos++];

			  curToken.MakeLower();

			  if(curToken == _T("pi"))			{ typToken = PARSER_PI; return true; }
			  else if(curToken == _T("e"))      { typToken = PARSER_E; return true; }
			  else if(curToken == _T("sin"))    { typToken = PARSER_SIN; return true; }
			  else if(curToken == _T("cos"))    { typToken = PARSER_COS; return true; }
			  else if(curToken == _T("tg"))     { typToken = PARSER_TG; return true; }
			  else if(curToken == _T("ctg"))    { typToken = PARSER_CTG; return true; }
			  else if(curToken == _T("arcsin")) { typToken = PARSER_ARCSIN; return true; }
			  else if(curToken == _T("arccos")) { typToken = PARSER_ARCCOS; return true; }
			  else if(curToken == _T("arctg"))  { typToken = PARSER_ARCTG; return true; }
			  else if(curToken == _T("arcctg")) { typToken = PARSER_ARCCTG; return true; }
			  else if(curToken == _T("sh"))     { typToken = PARSER_SH; return true; }
			  else if(curToken == _T("ch"))     { typToken = PARSER_CH; return true; }
			  else if(curToken == _T("th"))     { typToken = PARSER_TH; return true; }
			  else if(curToken == _T("cth"))    { typToken = PARSER_CTH; return true; }
			  else if(curToken == _T("exp"))    { typToken = PARSER_EXP; return true; }
			  else if(curToken == _T("lg"))     { typToken = PARSER_LG; return true; }
			  else if(curToken == _T("ln"))     { typToken = PARSER_LN; return true; }
			  else if(curToken == _T("sqrt"))   { typToken = PARSER_SQRT; return true; }
			  else if(curToken == _T("abs"))	{ typToken = PARSER_ABS; return true; }

			  else if(curToken == _T("min"))    { typToken = PARSER_MIN; return true; }
			  else if(curToken == _T("max"))    { typToken = PARSER_MAX; return true; }
			  else if(curToken == _T("atan2"))  { typToken = PARSER_ATAN2; return true; }

			  else if(curToken == _T("if"))     { typToken = PARSER_IF; return true; }
			  //или может быть
			  else if(curToken == _T("left"))   { typToken = PARSER_GUIDE; return true; }
  			  else if(curToken == _T("right"))  { typToken = PARSER_GUIDE; return true; }
  			  else if(curToken == _T("top"))    { typToken = PARSER_GUIDE; return true; }
  			  else if(curToken == _T("bottom")) { typToken = PARSER_GUIDE; return true; }
  			  else if(curToken == _T("width"))  { typToken = PARSER_GUIDE; return true; }
  			  else if(curToken == _T("height")) { typToken = PARSER_GUIDE; return true; }
			  else SendError(0);
		   }
		   else if(IsAdjust())
		   {
			  int i=0;
			  curToken = _T("");
			  while((!IsSpace())&&(!IsDelim())) curToken += expr[pos++];
			  
			  typToken = PARSER_ADJUST;
			  return true;
		   }
		   else if(IsGuide())
		   {
			  int i=0;
			  curToken = _T("");
			  while((!IsSpace())&&(!IsDelim())) curToken += expr[pos++];
			  
			  typToken = PARSER_GUIDE;
			  return true;
		   }
		   else if(IsDigit() || IsPoint())
		   {
			  int i=0;
			  curToken = _T("");
			  while(IsDigit()) curToken += expr[pos++];
			  if(IsPoint())
			  {
				 curToken += expr[pos++];
				 while(IsDigit()) curToken += expr[pos++];
			  }
			  typToken = PARSER_NUMBER;
			  return true;
		   }
		   else
		   {
			  curToken = expr[pos++];
			  SendError(1);
		   }

		   return false;
		}      

		bool IsSpace(void)  {CString string(_T(" "));          return (string.Find(expr[pos]) >= 0);}
	    bool IsDelim(void)  {CString string(_T("+-*/%^()[]")); return (string.Find(expr[pos]) >= 0);}
	    bool IsAdjust(void) {CString string(_T("$"));          return (string.Find(expr[pos]) >= 0);}
	    bool IsGuide(void)  {CString string(_T("?"));          return (string.Find(expr[pos]) >= 0);}
	    bool IsComma(void)  {CString string(_T(","));          return (string.Find(expr[pos]) >= 0);}
	   
	    bool IsLetter(void) { return ((expr[pos]>=_T('a') && expr[pos]<=_T('z')) ||
									 (expr[pos]>=_T('A') && expr[pos]<=_T('Z'))); }
	    bool IsDigit(void) { return (expr[pos]>=_T('0') && expr[pos]<=_T('9')); }
	    bool IsPoint(void) { return (expr[pos]==_T('.')); }

		void TConvertParser::SendError(int errNum)
		{
		   static CString errs[7] = { _T(""),
									  _T(""),
									  _T("Unexpected end of expression"),
									  _T("End of expression expected"),
									  _T("'(' or '[' expected"),
									  _T("')' or ']' expected"),
									  _T("")
								 };
		   CString buffer;

		   int len = curToken.GetLength();
		   
		   if(curToken == _T(""))
			  curToken = _T("EOL");

		   switch(errNum)
		   {
			  case 0:
				 buffer.Format(_T("Unknown keyword: '%s'"), curToken);
				 errs[0] = buffer;
				 break;

			  case 1:
				 buffer.Format(_T("Unknown symbol: '%s'"), curToken);
				 errs[1] = buffer;
				 break;

			  case 6:
				 buffer.Format(_T("Unexpected '%s'"), curToken);
				 errs[6] = buffer;
				 break;
		   }

		   TError error(errs[errNum], pos-len);

		   for(unsigned int i=0; i<history.size(); i++)
			  delete history[i];
		   history.clear();

		   root = NULL;

		   throw error;

		   return;
		}

	    void TConvertParser::DelTree(TParserNodeOdp *tree)
		{
		   if(tree==NULL) return;

		   DelTree(tree->left);
		   DelTree(tree->right);

		   delete tree;

		   return;
		}

		bool Compile(CString _expr, NSGuidesOdp::CFormulaManager& pFManager)
		{
		   pos = 0;
		   expr = _expr;
		   curToken = _T("");
		   if(root!=NULL)
		   {
			  DelTree(root);
			  root = NULL;
		   }

		   history.clear();
		   CString strTempString(_T("+-%^,"));
		   while(strTempString.Find(expr[0]) >= 0) expr.Delete(0);

		   GetToken();

		   if(typToken==PARSER_END) 
			   SendError(2);
		   root = Expr(pFManager);
		   if(typToken!=PARSER_END && typToken != PARSER_ERR)  
			   SendError(3);

		   history.clear();

		   return true;
		}

		TParserNodeOdp *TConvertParser::Expr(NSGuidesOdp::CFormulaManager& pFManager)
		{
		   TParserNodeOdp *temp = Expr1(pFManager);
		   CFormParam pFormParser = temp->FormParser;

		   while(1)
		   {
			  if(typToken==PARSER_PLUS)
			  {				  
				 GetToken();
				 TParserNodeOdp *temp2 = Expr1(pFManager);

				 AddFormulaSum(temp, temp2);
				 m_lIndexDst++;
				 pFormParser.m_eType = ptFormula;
				 pFormParser.m_lParam = m_lIndexDst;

				 temp = CreateNode(pFormParser, OP_PLUS, temp, temp2);
			  }
			  else if(typToken==PARSER_MINUS)
			  {
				 GetToken();
				 TParserNodeOdp *temp2 = Expr1(pFManager);

				 AddFormulaSum2(temp, temp2);
				 m_lIndexDst++;
				 pFormParser.m_eType = ptFormula;
				 pFormParser.m_lParam = m_lIndexDst;

				 temp = CreateNode(pFormParser, OP_MINUS, temp, temp2);
			  }
			  else 
			  break;
		   }

		   return temp;
		}
   
		TParserNodeOdp *TConvertParser::Expr1(NSGuidesOdp::CFormulaManager& pFManager)
		{
		   TParserNodeOdp *temp = Expr2(pFManager);
		   CFormParam pFormParser = temp->FormParser;

		   while(1)
		   {
			  if(typToken==PARSER_MULTIPLY)
			  {
				 GetToken();				 
				 TParserNodeOdp *temp2 = Expr2(pFManager);

				 AddFormulaProd(temp, temp2);
				 m_lIndexDst++;
				 pFormParser.m_eType = ptFormula;
				 pFormParser.m_lParam = m_lIndexDst;

				 temp = CreateNode(pFormParser, OP_MULTIPLY, temp, temp2);
			  }
			  else if(typToken==PARSER_DIVIDE)
			  {
				 GetToken();
				 TParserNodeOdp *temp2 = Expr2(pFManager);
				 
				 AddFormulaProd2(temp, temp2);
				 m_lIndexDst++;
				 pFormParser.m_eType = ptFormula;
				 pFormParser.m_lParam = m_lIndexDst;

				 temp = CreateNode(pFormParser, OP_DIVIDE, temp, temp2);
			  }
			  else 
			  break;
		   }

		   return temp;
		}

		TParserNodeOdp *TConvertParser::Expr2(NSGuidesOdp::CFormulaManager& pFManager)
		{
		   TParserNodeOdp *temp = Expr3(pFManager);
		   CFormParam pFormParser = temp->FormParser;

		   while(1)
		   {
			  if(typToken==PARSER_POWER)
			  {
				 GetToken();
				 temp = CreateNode(pFormParser, OP_POWER, temp, Expr2(pFManager));
			  }
			  else break;
		   }

		   return temp;
		}

		TParserNodeOdp *TConvertParser::Expr3(NSGuidesOdp::CFormulaManager& pFManager)
		{
		   TParserNodeOdp *temp;
		   CFormParam pFormParser;

		   if(typToken==PARSER_PLUS)
		   {
			  GetToken();
			  temp = Expr4(pFManager);
		   }
		   else if(typToken==PARSER_MINUS)
		   {
			  GetToken();
			  pFormParser.m_eType = ptValue;
			  pFormParser.m_lParam = -1;
			  temp = CreateNode(pFormParser, OP_UMINUS, Expr4(pFManager));
		   }
		   else
			  temp = Expr4(pFManager);

		   return temp;      
		}

		TParserNodeOdp *TConvertParser::Expr4(NSGuidesOdp::CFormulaManager& pFManager)
		{
		   TParserNodeOdp *temp;
		   CFormParam pFormParser;

		   if(typToken>=PARSER_SIN && typToken<=PARSER_ABS)
		   {
			  pFormParser.m_eType = ptFormula;
			  pFormParser.m_lParam = m_lIndexDst;
			  FormulaType eType = GetType(OP_SIN-PARSER_SIN+typToken);

			  temp = CreateNode(pFormParser, OP_SIN-PARSER_SIN+typToken);
			  GetToken();
			  if(typToken!=PARSER_L_BRACKET) SendError(4);
			  GetToken();
			  temp->left = Expr(pFManager);
			  if(typToken!=PARSER_R_BRACKET) SendError(5);
			  GetToken();

			  m_lIndexDst++;
			  temp->FormParser.m_lParam = m_lIndexDst;
			  GetFmla(temp, eType);
		   }
		   else if((typToken >= PARSER_MIN) && (typToken <= PARSER_ATAN2))
		   {
			   pFormParser.m_eType = ptFormula;
			   pFormParser.m_lParam = m_lIndexDst;
			   FormulaType eType = GetType(OP_MIN-PARSER_MIN+typToken);

			   temp = CreateNode(pFormParser, OP_MIN-PARSER_MIN+typToken);
			   GetToken();
			   if(typToken!=PARSER_L_BRACKET) SendError(4);
			   GetToken();
			   temp->left = Expr(pFManager);
			   if(typToken != PARSER_COMMA) SendError(6);
			   GetToken();
			   temp->right = Expr(pFManager);
			   if(typToken!=PARSER_R_BRACKET) SendError(5);
			   GetToken();

			   m_lIndexDst++;
			   temp->FormParser.m_lParam = m_lIndexDst;
			   GetFmla(temp, eType);
		   }
		   else if(typToken == PARSER_IF)
		   {
			   pFormParser.m_eType = ptFormula;
			   pFormParser.m_lParam = m_lIndexDst;
			   FormulaType eType = ftIf;

			   temp = CreateNode(pFormParser, OP_IF);
			   GetToken();
			   if(typToken!=PARSER_L_BRACKET) SendError(4);
			   GetToken();
			   temp->left = Expr(pFManager);
			   if(typToken != PARSER_COMMA) SendError(6);
			   GetToken();
			   temp->right = Expr(pFManager);
			   if(typToken != PARSER_COMMA) SendError(6);
			   GetToken();
			   temp->third = Expr(pFManager);
			   if(typToken!=PARSER_R_BRACKET) SendError(5);
			   GetToken();

			   m_lIndexDst++;
			   temp->FormParser.m_lParam = m_lIndexDst;
			   GetFmla(temp, eType);
		   }
		   else
			  temp = Expr5(pFManager);

		   return temp;
		}

		TParserNodeOdp *TConvertParser::Expr5(NSGuidesOdp::CFormulaManager& pFManager)
		{
		   TParserNodeOdp *temp;
		   CFormParam pFormParser;
		   
		   switch(typToken)
		   {
			  case PARSER_ADJUST:
				  pFormParser.m_eType = ptAdjust;				  
				  pFormParser.m_lParam = pFManager.GetNum(curToken);

				 temp = CreateNode(pFormParser, pFManager.GetValue(curToken));
				 GetToken();
				 break;

			  case PARSER_GUIDE:
				 LONG lNum;
				 pFormParser.m_eType = ptFormula;
				 pFormParser.m_lParam = mapFormula.Lookup(curToken);

				 temp = CreateNode(pFormParser, pFManager.GetValue(curToken));
				 GetToken();
				 break;

			  case PARSER_NUMBER:
				 pFormParser.m_eType = ptValue;
				 pFormParser.m_lParam = XmlUtils::GetDouble(curToken);

				 temp = CreateNode(pFormParser, XmlUtils::GetDouble(curToken));
				 GetToken();
				 break;

			  case PARSER_PI:
				 pFormParser.m_eType = ptFormula;
				 if (m_lPiFormulaNum == -1)
				 {
					 /*перебить вычисление pi через арктангенс добавлением стандартного набора формул в пптхе*/

					 /*m_lIndexDst++;					 
					 CFormula pFormula1;
					 pFormula1.m_eFormulaType = ftAtan2;
					 pFormula1.m_lParam1 = 1;
					 pFormula1.m_eType1 = ptValue;
					 pFormula1.m_lParam2 = 1;
					 pFormula1.m_eType2 = ptValue;
					 m_arFormulas.Add(pFormula1);

					 m_lIndexDst++;					 
					 m_lPiFormulaNum = m_lIndexDst;
					 CFormula pFormula2;
					 pFormula2.m_eFormulaType = ftProduct;
					 pFormula2.m_lParam1 = 4;
					 pFormula2.m_eType1 = ptValue;
					 pFormula2.m_lParam2 = m_lIndexDst-1;
					 pFormula2.m_eType2 = ptFormula;
					 pFormula2.m_lParam3 = 1;
					 pFormula2.m_eType3 = ptValue;
					 m_arFormulas.Add(pFormula2);*/

					 m_lIndexDst++;		
					 m_lPiFormulaNum = m_lIndexDst;
					 CFormula pFormula1;
					 pFormula1.m_eFormulaType = ftProduct;
					 pFormula1.m_lParam1 = 22;
					 pFormula1.m_eType1 = ptValue;
					 pFormula1.m_lParam2 = 1;
					 pFormula1.m_eType2 = ptValue;
					 pFormula1.m_lParam3 = 7;
					 pFormula1.m_eType3 = ptValue;
					 m_arFormulas.Add(pFormula1);
				 }
				 pFormParser.m_lParam = m_lPiFormulaNum;

				 temp = CreateNode(pFormParser, (double)M_PI);
				 GetToken();
				 break;

			  case PARSER_L_BRACKET:
				 GetToken();
				 temp = Expr(pFManager);
				 if(typToken!=PARSER_R_BRACKET) SendError(5);
				 GetToken();
				 break;

				 //если ктото ошибся и формула содержит лишний символ возвращаем "0"
			  default:
				 pFormParser.m_eType = ptValue;
				 pFormParser.m_lParam = 0;

				 temp = CreateNode(pFormParser, 0);
				 typToken = PARSER_ERR;
				 break;
		   }

		   return temp;         
		}

		void Decompile() 
		{ 
			DelTree(root); 
			root=NULL;
		}

		void GetFmla(TParserNodeOdp* _root, FormulaType eFormType)
		{			
			switch (eFormType)
			{				
				case ftSqrt:				
				case ftAbsolute:
					AddFormula(eFormType, _root->left );
					break;
				case ftSin:
				case ftCos:
					AddFormula(eFormType, _root->left, 1 );
					break;
				case ftAtan2:
					AddFormula(eFormType, _root->right, _root->left);
					break;
				case ftMin:
				case ftMax:
				case ftTan:
					AddFormula(eFormType, _root->left, _root->right);
					break;
				case ftIf:
				case ftMod:
					AddFormula(eFormType, _root->left, _root->right, _root->third);
					break;
				default:
					break;
			}
			return;
		}


		void AddFormula(FormulaType eFormType, TParserNodeOdp* _left)
		{
			CFormula pFormula;
			pFormula.m_eFormulaType = eFormType; 
			if (_left != NULL)
			{
				 pFormula.m_lParam1 = _left->FormParser.m_lParam;
				 pFormula.m_eType1 = _left->FormParser.m_eType;
			}
			else 
			{
				 pFormula.m_lParam1 = m_lIndexDst;
				 pFormula.m_eType1 = ptFormula;
			}
			m_arFormulas.Add(pFormula);
			return;
		}

		void AddFormula(FormulaType eFormType, TParserNodeOdp* _left, LONG lPar)
		{
			CFormula pFormula;
			pFormula.m_eFormulaType = eFormType; 

			pFormula.m_lParam1 = lPar;
			pFormula.m_eType1 = ptValue;

			if (_left != NULL)
			{
				 pFormula.m_lParam2 = _left->FormParser.m_lParam;
				 pFormula.m_eType2 = _left->FormParser.m_eType;
			}
			else 
			{
				 pFormula.m_lParam2 = m_lIndexDst;
				 pFormula.m_eType2 = ptFormula;
			}
			m_arFormulas.Add(pFormula);
			return;
		}

		void AddFormula(FormulaType eFormType, TParserNodeOdp* _left, TParserNodeOdp* _right)
		{
			 CFormula pFormula;
			pFormula.m_eFormulaType = eFormType; 
			 if (_left != NULL)
			 {
				 pFormula.m_lParam1 = _left->FormParser.m_lParam;
				 pFormula.m_eType1 = _left->FormParser.m_eType;
			 }
			 else 
			 {
				 pFormula.m_lParam1 = m_lIndexDst;
				 pFormula.m_eType1 = ptFormula;
			 }

			 if (_right != NULL)
			 {
				 pFormula.m_lParam2 = _right->FormParser.m_lParam;
				 pFormula.m_eType2 = _right->FormParser.m_eType;
			 }
			 else 
			 {
				 pFormula.m_lParam2 = m_lIndexDst;
				 pFormula.m_eType2 = ptFormula;
			 }
			 m_arFormulas.Add(pFormula);
			 return;
		}

		void AddFormula(FormulaType eFormType, TParserNodeOdp* _left, TParserNodeOdp* _right, TParserNodeOdp* _third)
		{
			CFormula pFormula;
			pFormula.m_eFormulaType = eFormType; 
			 if (_left != NULL)
			 {
				 pFormula.m_lParam1 = _left->FormParser.m_lParam;
				 pFormula.m_eType1 = _left->FormParser.m_eType;
			 }
			 else 
			 {
				 pFormula.m_lParam1 = m_lIndexDst;
				 pFormula.m_eType1 = ptFormula;
			 }

			 if (_right != NULL)
			 {
				 pFormula.m_lParam2 = _right->FormParser.m_lParam;
				 pFormula.m_eType2 = _right->FormParser.m_eType;
			 }
			 else 
			 {
				 pFormula.m_lParam2 = m_lIndexDst;
				 pFormula.m_eType2 = ptFormula;
			 }

			 if (_third != NULL)
			 {
				 pFormula.m_lParam3 = _third->FormParser.m_lParam;
				 pFormula.m_eType3 = _third->FormParser.m_eType;
			 }
			 else 
			 {
				 pFormula.m_lParam3 = m_lIndexDst;
				 pFormula.m_eType3 = ptFormula;
			 }
			 m_arFormulas.Add(pFormula);
			 return;
		}

		void AddFormulaSum(TParserNodeOdp* _left, TParserNodeOdp* _right)
		{
			 CFormula pFormula;
			 pFormula.m_eFormulaType = ftSum;
				 if (_left != NULL)
				 {
					 pFormula.m_lParam1 = _left->FormParser.m_lParam;
					 pFormula.m_eType1 = _left->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam1 = m_lIndexDst;
					 pFormula.m_eType1 = ptFormula;
				 }

				 if (_right != NULL)
				 {
					 pFormula.m_lParam2 = _right->FormParser.m_lParam;
					 pFormula.m_eType2 = _right->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam2 = m_lIndexDst;
					 pFormula.m_eType2 = ptFormula;
				 }
				 pFormula.m_lParam3 = 0;
				 pFormula.m_eType3 = ptValue;

				 m_arFormulas.Add(pFormula);
			 return;
		}

		void AddFormulaSum2(TParserNodeOdp* _left, TParserNodeOdp* _right)
		{
			 CFormula pFormula;
			 pFormula.m_eFormulaType = ftSum;
				 if (_left != NULL)
				 {
					 pFormula.m_lParam1 = _left->FormParser.m_lParam;
					 pFormula.m_eType1 = _left->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam1 = m_lIndexDst;
					 pFormula.m_eType1 = ptFormula;
				 }

				 if (_right != NULL)
				 {
					 pFormula.m_lParam3 = _right->FormParser.m_lParam;
					 pFormula.m_eType3 = _right->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam3 = m_lIndexDst;
					 pFormula.m_eType3 = ptFormula;
				 }
				 pFormula.m_lParam2 = 0;
				 pFormula.m_eType2 = ptValue;

				 m_arFormulas.Add(pFormula);	
			return;
		}

		void AddFormulaProd(TParserNodeOdp* _left, TParserNodeOdp* _right)
		{
			 CFormula pFormula;
			 pFormula.m_eFormulaType = ftProduct;
				 if (_left != NULL)
				 {
					 pFormula.m_lParam1 = _left->FormParser.m_lParam;
					 pFormula.m_eType1 = _left->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam1 = m_lIndexDst;
					 pFormula.m_eType1 = ptFormula;
				 }

				 if (_right != NULL)
				 {
					 pFormula.m_lParam2 = _right->FormParser.m_lParam;
					 pFormula.m_eType2 = _right->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam2 = m_lIndexDst;
					 pFormula.m_eType2 = ptFormula;
				 }
				 pFormula.m_lParam3 = 1;
				 pFormula.m_eType3 = ptValue;

				 m_arFormulas.Add(pFormula);	
			 return;
		}

		void AddFormulaProd2(TParserNodeOdp* _left, TParserNodeOdp* _right)
		{
			 CFormula pFormula;
			 pFormula.m_eFormulaType = ftProduct;
				 if (_left != NULL)
				 {
					 pFormula.m_lParam1 = _left->FormParser.m_lParam;
					 pFormula.m_eType1 = _left->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam1 = m_lIndexDst;
					 pFormula.m_eType1 = ptFormula;
				 }

				 if (_right != NULL)
				 {
					 pFormula.m_lParam3 = _right->FormParser.m_lParam;
					 pFormula.m_eType3 = _right->FormParser.m_eType;
				 }
				 else 
				 {
					 pFormula.m_lParam3 = m_lIndexDst;
					 pFormula.m_eType3 = ptFormula;
				 }
				 pFormula.m_lParam2 = 1;
				 pFormula.m_eType2 = ptValue;

				 m_arFormulas.Add(pFormula)	;	
			 return;
		}

		
	};

	class CFormulaConverterODP2PPT
	{
		public:
			CSimpleMap<CString, LONG> mapFmla;
			CSimpleArray<CFormula> m_arFormulas;
			CString strPath;
			PPTShapes::ShapeType ePPTType;
			CString strRect;
			CSimpleArray<CHandle_> m_arHandles;
			TConvertParser pODPParser;
			LONG m_lPiFormulaNum;
		private: 
			double nCount;

		public:

			CFormulaConverterODP2PPT()
			{
				strPath = _T("");
			}

			~CFormulaConverterODP2PPT()
			{
			}



			bool IsSpace(int pos, CString expr)  {CString string(_T(" "));          return (string.Find(expr[pos]) >= 0);}
			bool IsDelim(int pos, CString expr)  {CString string(_T("+-*/%^()[]")); return (string.Find(expr[pos]) >= 0);}
			bool IsGuide(int pos, CString expr)  {CString string(_T("?"));          return (string.Find(expr[pos]) >= 0);}

			void MoveFmla( CString strFmla, CString strFVal, LONG &lNewFmlaNum, NSGuidesOdp::CFormulaManager *pManager)
			{		
				for (int j=0; j < strFmla.GetLength(); j++ )
				{								
					if(IsGuide(j, strFmla))
					{
						CString curToken = _T("");
						while((!IsSpace(j, strFmla))&&(!IsDelim(j, strFmla))) curToken += strFmla[j++];
						LONG lNum = mapFmla.FindKey(curToken);
						if (lNum == -1)
						{
							LONG lVal = pManager->mapGuides.FindKey(curToken);
							MoveFmla( pManager->strGuides[lVal], curToken, lNewFmlaNum, pManager);
						}
					}
				}
				mapFmla.Add(strFVal, lNewFmlaNum);
				lNewFmlaNum++;
				return;
			}

			void ConvertOdp (COdpShape* pODPShape)
			{	
				//сортируем порядок формул
				CString strFmla = _T("");
				LONG lSize;
				LONG nFmlaSize = pODPShape->FManager.mapGuides.GetSize();
				for (int i=0; i<nFmlaSize; i++)
				{
					lSize = mapFmla.GetSize();
					CString strKey = pODPShape->FManager.mapGuides.GetKeyAt(i);
					LONG lFmlaNum = mapFmla.FindKey(strKey);
					if (lFmlaNum == -1)
					{
						strFmla = pODPShape->FManager.strGuides[i];
						MoveFmla( strFmla, strKey, lSize, &pODPShape->FManager);
					}
				}	

				//guids				

				int nGuidCount = mapFmla.GetSize();
				for (int i=0; i<nGuidCount; ++i)
				{
					CString strFmlaNum = mapFmla.GetKeyAt(i);
					LONG lNum = pODPShape->FManager.mapGuides.FindKey(strFmlaNum);
					CString strCurToken = pODPShape->FManager.strGuides[lNum];
					pODPParser.Compile(strCurToken, pODPShape->FManager);
					pODPParser.Evaluate(strFmlaNum);
					pODPParser.Decompile();				
				}

				m_arFormulas = pODPParser.m_arFormulas;

				//path
				CSimpleArray<CString> oArray;

				int lIndex = pODPShape->m_strPath.Find('W');
				if ( 0 == lIndex)
					pODPShape->m_strPath.Replace('W', 'V');

				NSStringUtils::ParseString(_T(" "), pODPShape->m_strPath, &oArray);
				nCount = oArray.GetSize();	
				CString strCurr = _T("");
				CSimpleArray<ParamPathType> arParPathType;

				for (int i=0; i<nCount; i++)
				{
					CString strCurr = oArray[i];
					LONG lNum;

					if ( strCurr[0] == '?' )
					{
						arParPathType.Add(ptForm);
						lNum = pODPParser.mapFormula.Lookup(strCurr);
						strCurr.Format(_T("%d"), lNum);
						strPath += _T("@") + strCurr;
					}
					else if (strCurr[0] == '$' )
					{
						arParPathType.Add(ptAdj);
						strPath += _T("#") + strCurr.Mid(1);
					}
					else if ((strCurr[0] >= '0') && (strCurr[0] <= '9'))
					{
						arParPathType.Add(ptVal);
						if (arParPathType[i-1] == ptShape)							
							strPath += strCurr;
						else 
							strPath += _T(",") + strCurr;
					}
					else 
					{
						arParPathType.Add(ptShape);
						strPath += ConvertName(strCurr);
					}
				}

				//strRect
				if (pODPShape->m_strRect == _T("") && pODPShape->m_oPath.m_arParts.GetSize() != 0)
					pODPShape->m_strRect.Format(_T("0 0 %d %d"), pODPShape->m_oPath.m_arParts[0].width, pODPShape->m_oPath.m_arParts[0].height);
				strRect = ConvertXmlPar(pODPShape->m_strRect, &pODPParser.mapFormula, false);
				while (true)
				{
					int lIndex = strRect.Find(' ');
					if (-1 != lIndex)
						strRect.Replace(' ', ',');
					else 
						break;
				}

				//handle
				CHandle_ oODPHandle;
				CHandle_ oPPTHandle;
				LONG nHndleCount = pODPShape->m_arHandles.GetSize();
				for (int i=0; i<nHndleCount; i++)
				{
					oODPHandle = pODPShape->m_arHandles[i];
					if (oODPHandle.position)
						oPPTHandle.position = ConvertXmlPar(oODPHandle.position, &pODPParser.mapFormula, false);
					if (oODPHandle.polar)
						oPPTHandle.polar = ConvertXmlPar(oODPHandle.polar, &pODPParser.mapFormula, false);
					if (oODPHandle.radiusrange)
						oPPTHandle.radiusrange = ConvertXmlPar(oODPHandle.radiusrange, &pODPParser.mapFormula, true);
					if (oODPHandle.xrange)
						oPPTHandle.xrange = ConvertXmlPar(oODPHandle.xrange, &pODPParser.mapFormula, true);
					if (oODPHandle.yrange)
						oPPTHandle.yrange = ConvertXmlPar(oODPHandle.yrange, &pODPParser.mapFormula, true);

					m_arHandles.Add(oPPTHandle);
				}

				//pi
				m_lPiFormulaNum = pODPParser.m_lPiFormulaNum;
				return;
			}			

			CString ConvertXmlPar(CString strPar, CSimpleMap<CString, LONG> *oMap, BOOL bSameVals)
			{				
				CString strRes = _T("");
				CSimpleArray<CString> arParam;
				NSStringUtils::ParseString(_T(" "), strPar, &arParam);
				LONG nCountParam = arParam.GetSize();

				//bSameVals - проверка на одинаковые начальные и конечные значения в handle
				//для xrange и yrange
				//тк в ппт такие переменные просто отсутствуют
				if (bSameVals && nCountParam>1)
				{
					if (arParam[0] == arParam[1])
						return strRes;
				}

				for (int i=0; i<nCountParam; i++)
				{
					CString strCurr = arParam[i];
					if ( strCurr[0] == '?' )
					{
						LONG lNum = oMap->Lookup(strCurr);
						strCurr.Format(_T("%d"), lNum);
						strRes += _T("@") + strCurr;
					}
					else if (strCurr[0] == '$' )
						strRes += _T("#") + strCurr.Mid(1);					
					else if (strCurr == _T("top") || strCurr == _T("left"))
						strRes += _T("topLeft");
					else if (strCurr == _T("right") || strCurr == _T("bottom"))
						strRes += _T("bottomRight");
					else //if ((strCurr[0] >= '0') && (strCurr[0] <= '9'))
						strRes += strCurr;

					if (i != nCountParam-1)
						strRes += _T(",");
				}
				return strRes;
			}

			CString ConvertName(CString strName)
			{
					 if	((_T("M") == strName))		return _T("m");
				else if ((_T("L") == strName))		return _T("l");
				else if ((_T("C") == strName))		return _T("c");
				else if ((_T("Z") == strName))		return _T("x");
				else if ((_T("N") == strName))		return _T("e");
				else if ((_T("F") == strName))		return _T("nf");
				else if ((_T("S") == strName))		return _T("ns");
				else if ((_T("T") == strName))		return _T("ae");
				else if ((_T("U") == strName))		return _T("al");
				else if ((_T("A") == strName))		return _T("at");
				else if ((_T("B") == strName))		return _T("ar");
				else if ((_T("W") == strName))		return _T("wa");
				else if ((_T("V") == strName))		return _T("wr");
				else if ((_T("X") == strName))		return _T("qx");
				else if ((_T("Y") == strName))		return _T("qy");
				else if ((_T("Q") == strName))		return _T("qb");
				else return _T("");
			}

			void ConvertType ( OdpShapes::ShapeType eODPType)
			{
				 

				 if( eODPType == 1)
					 ePPTType = PPTShapes::ShapeType::sptCWedgeRoundRectCallout;
				 return;
			}


	};
	
}