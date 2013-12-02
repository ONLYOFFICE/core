using System;
using System.Collections;
using System.Text.RegularExpressions;
namespace CSharpEval
{


    public enum Type { Variable, Value, Operator, Function, Result, Bracket, Comma, Error }
    public struct EqnElement
    {
        public string strName;
        public double dblValue;
        public Type elmType;
        public override string ToString()
        {
            return strName;
        }
    }
    public delegate EqnElement EvaluateFunctionDelegate(string name, params Object[] args);
    public class Eval
    {
        public double Result
        {
            get
            {
                return m_result;
            }
        }

        public ArrayList Equation
        {
            get
            {
                return (ArrayList)str_equation.Clone();
            }
        }
        public ArrayList Postfix
        {
            get
            {
                return (ArrayList)m_postfix.Clone();
            }
        }

        public EvaluateFunctionDelegate DefaultFunctionEvaluation
        {
            set
            {
                m_defaultFunctionEvaluation = value;
            }
        }

        public bool Error
        {
            get
            {
                return m_bError;
            }
        }

        public string ErrorDescription
        {
            get
            {
                return m_sErrorDescription;
            }
        }

        public ArrayList Variables
        {
            get
            {
                ArrayList var = new ArrayList();
                foreach (EqnElement Elmnt in str_equation)
                {
                    if ((Elmnt.elmType == Type.Variable) && (!var.Contains(Elmnt)))
                        var.Add(Elmnt);
                }
                return var;
            }
            set
            {
                foreach (EqnElement Elmnt in value)
                {
                    for (int i = 0; i < m_postfix.Count; i++)
                    {
                        if ((Elmnt.strName == ((EqnElement)m_postfix[i]).strName) && (((EqnElement)m_postfix[i]).elmType == Type.Variable))
                        {
                            EqnElement Elmnt1 = (EqnElement)m_postfix[i];
                            Elmnt1.dblValue = Elmnt.dblValue;
                            m_postfix[i] = Elmnt1;
                        }
                    }
                }
            }
        }

        public Eval()
        { }

        public void ParseEquation(string equation)
        {
            int state = 1;
            string temp = "";
            EqnElement stcElement;

            m_bError = false;
            m_sErrorDescription = "None";

            str_equation.Clear();
            m_postfix.Clear();

            int nPos = 0;
            //-- Remove all white spaces from the equation string --
            equation = equation.Trim();
            while ((nPos = equation.IndexOf(' ')) != -1)
                equation = equation.Remove(nPos, 1);
            equation = Regex.Replace(equation, "[-][-]", "+");
            equation = Regex.Replace(equation, "[^0-9)E][-]", "(0-");
            //equation = Regex.Replace(equation, "[,][-]", ",0-");

            if (equation.StartsWith("-")) equation = "0" + equation;
            for (int i = 0; i < equation.Length; i++)
            {
                switch (state)
                {
                    case 1:
                        if (Char.IsNumber(equation[i]))
                        {
                            state = 2;
                            temp += equation[i];
                        }
                        else if (Char.IsLetter(equation[i]))
                        {
                            state = 3;
                            temp += equation[i];
                        }
                        else
                        {
                            stcElement.strName = equation[i].ToString();
                            stcElement.dblValue = 0;
                            switch (stcElement.strName)
                            {
                                case ",":
                                    stcElement.elmType = Type.Comma;
                                    break;
                                case "(":
                                case ")":
                                case "[":
                                case "]":
                                case "{":
                                case "}":
                                    stcElement.elmType = Type.Bracket;
                                    break;
                                default:
                                    stcElement.elmType = Type.Operator;
                                    break;
                            }
                            str_equation.Add(stcElement);
                        }
                        break;
                    case 2:
                        if ((Char.IsNumber(equation[i])) || (equation[i] == '.'))
                            temp += equation[i];
                        else if (!Char.IsLetter(equation[i]))
                        {
                            state = 1;
                            stcElement.strName = temp;
                            stcElement.dblValue = Double.Parse(temp, System.Globalization.CultureInfo.InvariantCulture);
                            stcElement.elmType = Type.Value;
                            str_equation.Add(stcElement);
                            stcElement.strName = equation[i].ToString();
                            stcElement.dblValue = 0;
                            switch (stcElement.strName)
                            {
                                case ",":
                                    stcElement.elmType = Type.Comma;
                                    break;
                                case "(":
                                case ")":
                                case "[":
                                case "]":
                                case "{":
                                case "}":
                                    stcElement.elmType = Type.Bracket;
                                    break;
                                default:
                                    stcElement.elmType = Type.Operator;
                                    break;
                            }
                            str_equation.Add(stcElement);
                            temp = "";
                        }
                        break;
                    case 3:
                        if (Char.IsLetterOrDigit(equation[i]))
                            temp += equation[i];
                        else
                        {
                            state = 1;
                            stcElement.strName = temp;
                            stcElement.dblValue = 0;
                            if (equation[i] == '(')
                                stcElement.elmType = Type.Function;
                            else
                            {
                                if (stcElement.strName == "pi")
                                    stcElement.dblValue = System.Math.PI;
                                else if (stcElement.strName == "e")
                                    stcElement.dblValue = System.Math.E;
                                stcElement.elmType = Type.Variable;
                            }
                            str_equation.Add(stcElement);
                            stcElement.strName = equation[i].ToString();
                            stcElement.dblValue = 0;
                            switch (stcElement.strName)
                            {
                                case ",":
                                    stcElement.elmType = Type.Comma;
                                    break;
                                case "(":
                                case ")":
                                case "[":
                                case "]":
                                case "{":
                                case "}":
                                    stcElement.elmType = Type.Bracket;
                                    break;
                                default:
                                    stcElement.elmType = Type.Operator;
                                    break;
                            }
                            str_equation.Add(stcElement);
                            temp = "";
                        }
                        break;
                }
            }
            if (temp != "")
            {
                stcElement.strName = temp;
                if (state == 2)
                {
                    stcElement.dblValue = Double.Parse(temp, System.Globalization.CultureInfo.InvariantCulture);
                    stcElement.elmType = Type.Value;
                }
                else
                {
                    if (stcElement.strName == "pi")
                        stcElement.dblValue = System.Math.PI;
                    else if (stcElement.strName == "e")
                        stcElement.dblValue = System.Math.E;
                    else
                        stcElement.dblValue = 0;
                    stcElement.elmType = Type.Variable;
                }
                str_equation.Add(stcElement);
            }
        }

        public void Convert2PostFix()
        {
            EqnElement tpElmnt;
            Stack elmStack = new Stack();
            foreach (EqnElement Elmnt in str_equation)
            {
                if ((Elmnt.elmType == Type.Value) || (Elmnt.elmType == Type.Variable))
                    m_postfix.Add(Elmnt);
                else if ((Elmnt.strName == "(") || (Elmnt.strName == "[") || (Elmnt.strName == "{"))
                    elmStack.Push(Elmnt);
                else if ((Elmnt.strName == ")") || (Elmnt.strName == "]") || (Elmnt.strName == "}"))
                {
                    if (elmStack.Count > 0)
                    {
                        tpElmnt = (EqnElement)elmStack.Pop();
                        while ((tpElmnt.strName != "(") && (tpElmnt.strName != "[") && (tpElmnt.strName != "{"))
                        {
                            m_postfix.Add(tpElmnt);
                            tpElmnt = (EqnElement)elmStack.Pop();
                        }
                    }
                }
                else
                {
                    if (elmStack.Count > 0)
                    {
                        tpElmnt = (EqnElement)elmStack.Pop();
                        while ((elmStack.Count != 0) && ((tpElmnt.elmType == Type.Operator) || (tpElmnt.elmType == Type.Function) || (tpElmnt.elmType == Type.Comma)) && (Precedence(tpElmnt) >= Precedence(Elmnt)))
                        {
                            m_postfix.Add(tpElmnt);
                            tpElmnt = (EqnElement)elmStack.Pop();
                        }
                        if (((tpElmnt.elmType == Type.Operator) || (tpElmnt.elmType == Type.Function) || (tpElmnt.elmType == Type.Comma)) && (Precedence(tpElmnt) >= Precedence(Elmnt)))
                            m_postfix.Add(tpElmnt);
                        else
                            elmStack.Push(tpElmnt);
                    }
                    elmStack.Push(Elmnt);
                }
            }
            while (elmStack.Count > 0)
            {
                tpElmnt = (EqnElement)elmStack.Pop();
                m_postfix.Add(tpElmnt);
            }
        }

        public void EvaluatePostfix()
        {
            EqnElement tpElmnt1, tpElmnt2, tpResult;

            Stack elmStack = new Stack();
            ArrayList fnParam = new ArrayList();
            m_bError = false;
            foreach (EqnElement Elmnt in m_postfix)
            {
                if ((Elmnt.elmType == Type.Value) || (Elmnt.elmType == Type.Variable) || (Elmnt.elmType == Type.Result))
                    elmStack.Push(Elmnt);
                else if (Elmnt.elmType == Type.Operator)
                {

                    tpElmnt1 = (EqnElement)elmStack.Pop();

                    tpElmnt2 = (EqnElement)elmStack.Pop();
                    tpResult = Evaluate(tpElmnt2, Elmnt, tpElmnt1);
                    if (tpResult.elmType == Type.Error)
                    {
                        m_bError = true;
                        m_sErrorDescription = tpResult.strName;
                        return;
                    }
                    elmStack.Push(tpResult);
                }
                else if (Elmnt.elmType == Type.Function)
                {
                    fnParam.Clear();
                    tpElmnt1 = (EqnElement)elmStack.Pop();
                    if ((tpElmnt1.elmType == Type.Value) || (tpElmnt1.elmType == Type.Variable) || (tpElmnt1.elmType == Type.Result))
                    {
                        tpResult = EvaluateFunction(Elmnt.strName, tpElmnt1);
                        if (tpResult.elmType == Type.Error)
                        {
                            m_bError = true;
                            m_sErrorDescription = tpResult.strName;
                            return;
                        }
                        elmStack.Push(tpResult);
                    }
                    else if (tpElmnt1.elmType == Type.Comma)
                    {
                        while (tpElmnt1.elmType == Type.Comma)
                        {
                            tpElmnt1 = (EqnElement)elmStack.Pop();
                            fnParam.Add(tpElmnt1);
                            tpElmnt1 = (EqnElement)elmStack.Pop();
                        }
                        fnParam.Add(tpElmnt1);
                        tpResult = EvaluateFunction(Elmnt.strName, fnParam.ToArray());
                        if (tpResult.elmType == Type.Error)
                        {
                            m_bError = true;
                            m_sErrorDescription = tpResult.strName;
                            return;
                        }
                        elmStack.Push(tpResult);
                    }
                    else
                    {
                        elmStack.Push(tpElmnt1);
                        tpResult = EvaluateFunction(Elmnt.strName);
                        if (tpResult.elmType == Type.Error)
                        {
                            m_bError = true;
                            m_sErrorDescription = tpResult.strName;
                            return;
                        }
                        elmStack.Push(tpResult);
                    }
                }
                else if (Elmnt.elmType == Type.Comma)
                {
                    elmStack.Push(Elmnt);
                }
            }
            if (elmStack.Count == 1)
            {
                tpResult = (EqnElement)elmStack.Pop();
                m_result = tpResult.dblValue;
            }
        }

        protected int Precedence(EqnElement Elmnt)
        {
            switch (Elmnt.elmType)
            {
                case Type.Bracket:
                    return 5;
                case Type.Function:
                    return 4;
                case Type.Comma:
                    return 0;
            }
            switch (Elmnt.strName)
            {
                case "^":
                    return 3;
                case "/":
                case "*":
                case "%":
                    return 2;
                case "+":
                case "-":
                    return 1;
            }
            return -1;
        }

        protected EqnElement Evaluate(EqnElement Elmnt1, EqnElement opr, EqnElement Elmnt2)
        {
            EqnElement result;
            result.strName = Elmnt1.strName + opr.strName + Elmnt2.strName;
            result.elmType = Type.Result;
            result.dblValue = 0;
            switch (opr.strName)
            {
                case "^":
                    result.dblValue = System.Math.Pow(Elmnt1.dblValue, Elmnt2.dblValue);
                    break;
                case "/":
                    {
                        if (Elmnt2.dblValue > 0)
                            result.dblValue = Elmnt1.dblValue / Elmnt2.dblValue;
                        else
                        {
                            result.strName = "Divide by Zero.";
                            result.elmType = Type.Error;
                        }
                        break;
                    }
                case "*":
                    result.dblValue = Elmnt1.dblValue * Elmnt2.dblValue;
                    break;
                case "%":
                    result.dblValue = Elmnt1.dblValue % Elmnt2.dblValue;
                    break;
                case "+":
                    result.dblValue = Elmnt1.dblValue + Elmnt2.dblValue;
                    break;
                case "-":
                    result.dblValue = Elmnt1.dblValue - Elmnt2.dblValue;
                    break;
                default:
                    result.elmType = Type.Error;
                    result.strName = "Undefine operator: " + opr.strName + ".";
                    break;
            }
            if (result.dblValue.ToString(System.Globalization.CultureInfo.InvariantCulture).Contains("E-"))
            {
                result.dblValue = 0;
            }
            return result;
        }

        protected EqnElement EvaluateFunction(string name, params Object[] args)
        {
            EqnElement result;
            result.strName = "";
            result.elmType = Type.Result;
            result.dblValue = 0;
            switch (name)
            {
                case "cos":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Cos(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "sin":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Sin(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "tan":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Tan(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "cosh":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Cosh(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "sinh":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Sinh(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "tanh":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Tanh(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "log":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Log10(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "ln":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Log(((EqnElement)args[0]).dblValue, 2);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "logn":
                    if (args.Length == 2)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + "'" + ((EqnElement)args[1]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Log(((EqnElement)args[0]).dblValue, ((EqnElement)args[1]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "sqrt":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Sqrt(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "abs":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Abs(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "acos":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Acos(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "asin":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Asin(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "atan":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Atan(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;

                case "exp":
                    if (args.Length == 1)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Exp(((EqnElement)args[0]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "atan2":
                    if (args.Length == 2)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Atan2(((EqnElement)args[0]).dblValue, ((EqnElement)args[1]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "min":
                    if (args.Length == 2)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Min(((EqnElement)args[0]).dblValue, ((EqnElement)args[1]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                case "max":
                    if (args.Length == 2)
                    {
                        result.strName = name + "(" + ((EqnElement)args[0]).dblValue.ToString() + ")";
                        result.dblValue = System.Math.Max(((EqnElement)args[0]).dblValue, ((EqnElement)args[1]).dblValue);
                    }
                    else
                    {
                        result.strName = "Invalid number of parameters in: " + name + ".";
                        result.elmType = Type.Error;
                    }
                    break;
                default:
                    if (m_defaultFunctionEvaluation != null)
                        result = m_defaultFunctionEvaluation(name, args);
                    else
                    {
                        result.strName = "Function: " + name + ", not found.";
                        result.elmType = Type.Error;
                    }
                    break;
            }
            if (result.dblValue.ToString(System.Globalization.CultureInfo.InvariantCulture).Contains("E-"))
            {
                result.dblValue = 0;
            }
            return result;
        }

        protected bool m_bError = false;
        protected string m_sErrorDescription = "None";
        protected double m_result = 0;
        protected ArrayList str_equation = new ArrayList();
        protected ArrayList m_postfix = new ArrayList();
        protected EvaluateFunctionDelegate m_defaultFunctionEvaluation;
    }
}

