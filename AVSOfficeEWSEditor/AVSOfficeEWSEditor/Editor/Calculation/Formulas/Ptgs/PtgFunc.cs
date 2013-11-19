using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal abstract class PtgFunc : OperandPtg
    {
        private static Regex rx_num = new Regex("\\G([\\w\\d.]+)[-+*/^&%<=>:;\\(\\)]");
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_num.Match(formula, start_pos);
            if (match.Success)
            {
                GroupCollection groups = match.Groups;
                if (groups.Count == 2)
                {
                    start_pos += groups[1].Length;
                    operand_str = groups[1].Value;
                    return true;
                }
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            LinkedList<DataObjects.IDataObject> argv = new LinkedList<DataObjects.IDataObject>();
            for (int num = ParametersNum; num != 0; --num)
            {
                if (!ptgs_enum.MoveNext())
                {
                    if (ParametersNumMin <= ParametersNum - num)
                        return Calculate(argv, result_type);

                    throw new Events.ReportableException(new Events.FormulaFormatError("Argument #" + num + " of function " + ToString() + " was not found"));
                }
                DataObjects.IDataObject arg = ptgs_enum.Current.Calculate(ref ptgs_enum, PreferredResultType.as_is);
                argv.AddFirst(arg);
            }
            return Calculate(argv, result_type);
        }

        public abstract DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type);

        public override string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum)
        {	
            string arguments = "";
            for (int num = ParametersNum; num != 0; --num)
            {
                if (!ptgs_enum.MoveNext())
                {
                    if (ParametersNumMin <= ParametersNum - num && ToString() == new Functions.PtgFuncIf().ToString())
                    {
                        if (0 == ParametersNum - num || 1 == ParametersNum - num)
                        {
                            arguments += ",TRUE,FALSE";
                        }
                        else if (2 == ParametersNum - num)
                        {
                            arguments += ",FALSE";
                        }
                        else
                            throw new Events.ReportableException(new Events.FormulaFormatError("Argument #" + num + " of function " + ToString() + " was not found"));

                        return ToString() + "(" + arguments + ")";
                    }
                    else if (ParametersNumMin <= ParametersNum - num && ToString() == new Functions.PtgFuncIfError().ToString())
                    {
                        arguments += ",";
                        return ToString() + "(" + arguments + ")";
                    }

                    throw new Events.ReportableException(new Events.FormulaFormatError("Argument #" + num + " of function " + ToString() + " was not found"));
                }
                arguments = ptgs_enum.Current.Assemble(ref ptgs_enum) + (num != ParametersNum ? ("," + arguments) : "");
            }
            return ToString() + "(" + arguments + ")";
        }

        public abstract int ParametersNum { get; }
        public abstract int ParametersNumMin { get; }
        public abstract int ParametersNumMax { get; }
        public abstract string ArgumentsTip { get; }
        public abstract bool SetParametersNum(int parameters_num);
        public abstract new string ToString();
    }

    internal abstract class PtgFuncVar : PtgFunc
    {
        public override int ParametersNum
        {
            get { return parameters_num; }
        }
        public override int ParametersNumMin
        {
            get { return parameters_num; }
        }
        public override int ParametersNumMax
        {
            get { return parameters_num; }
        }

        public override bool SetParametersNum(int parameters_num)
        {
            this.parameters_num = parameters_num;
            return true;
        }
        private int parameters_num;
    }

    internal abstract class PtgFuncFixed : PtgFunc
    {
        public override bool SetParametersNum(int parameters_num)
        {
            return (ParametersNum == parameters_num || (ParametersNumMin <= parameters_num && parameters_num <= ParametersNumMax));
        }
    }
}
