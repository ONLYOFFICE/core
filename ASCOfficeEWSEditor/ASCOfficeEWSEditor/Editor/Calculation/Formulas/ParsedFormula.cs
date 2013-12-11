using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas
{
    internal abstract class ParsedFormula
    {
        internal ParsedFormula(Workbook workbook)
        {
            this.workbook = workbook;
            ptgs_list = new LinkedList<Ptg>();
            RecalcInProgress = false;
            formula_last_set = null;
        }

        public bool Parse(string formula, Cells.Offset offset)
        {
            m_oLastError = null;

            formula_last_set = formula;
            bool operand_expected = true; // This would help distinguish unary and binary and determine if an argument to a function is missed.
            Stack<Ptg> ptg_stack = new Stack<Ptg>();
            Worksheet ws_from;
            Worksheet ws_to;

            for (int start_pos = 0, strlen = formula.Length; start_pos != strlen; )
            {
                #region Operators
                if (is_operators(formula, start_pos) || PtgIsect.IsNextPtg(formula, start_pos))
                {
                    OperatorPtg found_operator = null;
                    if (PtgAdd.Extract(formula, ref start_pos, operand_expected))
                    {
                        found_operator = new PtgAdd();
                        operand_expected = true;
                    }
                    else if (PtgUplus.Extract(formula, ref start_pos, operand_expected))
                    {
                        found_operator = new PtgUplus();
                        operand_expected = true;
                    }
                    else if (PtgSub.Extract(formula, ref start_pos, operand_expected))
                    {
                        found_operator = new PtgSub();
                        operand_expected = true;
                    }
                    else if (PtgUminus.Extract(formula, ref start_pos, operand_expected))
                    {
                        found_operator = new PtgUminus();
                        operand_expected = true;
                    }
                    else if (PtgPercent.Extract(formula, ref start_pos, operand_expected))
                    {
                        found_operator = new PtgPercent();
                        operand_expected = false;
                    }
                    else
                    {
                        if (operand_expected)
                        {
                            m_oLastError = new Events.OperandExpectedError(formula, start_pos);
                            workbook.ReportError(m_oLastError);
                            ptgs_list.Clear();
                            return false;
                        }
                        if (PtgMul.Extract(formula, ref start_pos))
                        {
                            found_operator = new PtgMul();
                            operand_expected = true;
                        }
                        else if (PtgDiv.Extract(formula, ref start_pos))
                        {
                            found_operator = new PtgDiv();
                            operand_expected = true;
                        }
                        else if (PtgPower.Extract(formula, ref start_pos))
                        {
                            found_operator = new PtgPower();
                            operand_expected = true;
                        }
                        else if (PtgEq.Extract(formula, ref start_pos)) // Sequence is important
                        {
                            found_operator = new PtgEq();
                            operand_expected = true;
                        }
                        else if (PtgNe.Extract(formula, ref start_pos)) // Sequence is important
                        {
                            found_operator = new PtgNe();
                            operand_expected = true;
                        }
                        else if (PtgLe.Extract(formula, ref start_pos)) // Sequence is important
                        {
                            found_operator = new PtgLe();
                            operand_expected = true;
                        }
                        else if (PtgLt.Extract(formula, ref start_pos)) // Sequence is important
                        {
                            found_operator = new PtgLt();
                            operand_expected = true;
                        }
                        else if (PtgGe.Extract(formula, ref start_pos)) // Sequence is important
                        {
                            found_operator = new PtgGe();
                            operand_expected = true;
                        }
                        else if (PtgGt.Extract(formula, ref start_pos)) // Sequence is important
                        {
                            found_operator = new PtgGt();
                            operand_expected = true;
                        }
                        else if (PtgConcat.Extract(formula, ref start_pos))
                        {
                            found_operator = new PtgConcat();
                            operand_expected = true;
                        }
/*
                        else if (PtgUnion.Extract(formula, ref start_pos))
                        {
                            found_operator = new PtgConcat();
                            operand_expected = true;
                        }
*/
                        else if (PtgRange.Extract(formula, ref start_pos))
                        {
                            found_operator = new PtgRange();
                            operand_expected = true;
                        }
                        else if (PtgIsect.Extract(formula, ref start_pos)) // Must be the last of operators
                        {
                            found_operator = new PtgIsect();
                            operand_expected = true;
                        }
                        else
                        {
                            m_oLastError = new Events.UnknownOperatorError(formula, start_pos);
                            start_pos += 1;
                            operand_expected = true;
                        }
                    }

                    OperatorPtg operator_top;
                    while (ptg_stack.Count != 0 && (operator_top = (ptg_stack.Peek() as OperatorPtg)) != null &&
                        (found_operator.isRightAssociative() ? (operator_top.Priority < found_operator.Priority) :
                        (operator_top.Priority <= found_operator.Priority)))
                    {
                        ptgs_list.AddFirst(ptg_stack.Pop());
                    }
                    ptg_stack.Push(found_operator);
                }
                #endregion
                #region Parenthesis
                else if (PtgParen.Extract_Left(formula, ref start_pos))
                {
                    PtgFunc func = ptg_stack.Count != 0 ? ptg_stack.Peek() as PtgFunc : null;
                    operand_expected = true;
                    ptg_stack.Push(new PtgParen());
                }
                else if (PtgParen.Extract_Right(formula, ref start_pos))
                {
                    PtgParen left_p = null;
                    if (ptg_stack.Count != 0 && ((left_p = ptg_stack.Peek() as PtgParen) != null) && operand_expected)
                    {
                        if (left_p.ParametersNum > 1)  // This means there is at least one comma inside the parenthesis
                        {
                            ptgs_list.AddFirst(new PtgMissedArg());
                        }
                        else
                        {
                            left_p.DecrementParametersNum();
                        }
                    }
                    else
                    {
                        while (ptg_stack.Count != 0 && (left_p = ptg_stack.Peek() as PtgParen) == null)
                        {
                            ptgs_list.AddFirst(ptg_stack.Pop());
                        }
                    }
                    if (ptg_stack.Count == 0 || left_p == null)
                    {
                        m_oLastError = new Events.WrongParenthesisSequenceError(formula, start_pos);
                        workbook.ReportError(m_oLastError);
                        ptgs_list.Clear();
                        return false;
                    }
                    ptg_stack.Pop(); // pop PtgParen that is now stored in left_p
                    Ptg last_ptg = left_p;
                    PtgFunc func;
                    if (ptg_stack.Count != 0 && (func = ptg_stack.Peek() as PtgFunc) != null)
                    {
                        last_ptg = ptg_stack.Pop(); // pop PtgFunc
                        int num_parameters = left_p.ParametersNum;
                        if (num_parameters > 255)
                        {
                            m_oLastError = new Events.FormulaFormatError("Too many parameters to a function", formula, start_pos);
                            workbook.ReportError(m_oLastError);
                            ptgs_list.Clear();
                            return false;
                        }
                        if (!func.SetParametersNum(num_parameters))
                        {
                            m_oLastError = new Events.FormulaFormatError("Wrong parameters number (" + func.ParametersNum + " expected)", formula, start_pos);
                            workbook.ReportError(m_oLastError);
                            ptgs_list.Clear();
                            return false;
                        }
                    }
                    else // If there is no function name before the left parenthesis
                    {
                        for (int i = 0; i < left_p.ParametersNum - 1; ++i)
                        {
                            ptgs_list.AddFirst(new PtgUnion());
                        }
                    }
                    ptgs_list.AddFirst(last_ptg);
                    operand_expected = false;
                }
                #endregion
                #region Comma and PtgUnion
                else if (extract_comma(formula, ref start_pos))
                {
			        PtgParen left_p = ptg_stack.Count != 0 ? ptg_stack.Peek() as PtgParen : null;
			        if(left_p != null && operand_expected)
			        {
                        ptgs_list.AddFirst(new PtgMissedArg());
			        }
			        else
			        {
				        while(ptg_stack.Count != 0 && (left_p = ptg_stack.Peek() as PtgParen) == null)
				        {
                            ptgs_list.AddFirst(ptg_stack.Pop());
				        }
			        }
                    if (ptg_stack.Count == 0 || left_p == null)
			        {
                        m_oLastError = new Events.WrongParenthesisSequenceError(formula, start_pos);
                        if (formula.Length - 1 < start_pos)
                        {
                            workbook.ReportError(m_oLastError);
                            ptgs_list.Clear();
                            return false;
                        }
                        else
                        {
                            operand_expected = true;
                            continue;
                        }
			        }
                    left_p.IncrementParametersNum(); // The count of parameters will be transferred to PtgFuncVar
                    operand_expected = true;
                }
                #endregion
                #region Operands
                else
                {
                    if (!operand_expected)
                    {
                        m_oLastError = new Events.OperatorExpectedError(formula, start_pos);
                        workbook.ReportError(m_oLastError);
                        ptgs_list.Clear();
                        return false;
                    }
                    OperandPtg found_operand;
                    string operand_str;
                    if (PtgBool.Extract(formula, ref start_pos, out operand_str))
                    {
                        found_operand = new PtgBool(operand_str);
                    }
                    else if (PtgStr.Extract(formula, ref start_pos, out operand_str))
                    {
                        found_operand = new PtgStr(operand_str);
                    }
                    else if (PtgErr.Extract(formula, ref start_pos, out operand_str))
                    {
                        found_operand = new PtgErr(operand_str);
                    }
                    else if (extract_3D_part(formula, ref start_pos, out ws_from, out ws_to, workbook)) // Shall be placed strongly before extract_PtgName
                    {
                        if (PtgArea.Extract(formula, ref start_pos, out operand_str)) // Sequence is important (in pair with PtgRef3D)
                        {
                            found_operand = PtgArea3D.Create(operand_str, offset, ws_from, ws_to);
                        }
                        else if (PtgRef.Extract(formula, ref start_pos, out operand_str)) // Sequence is important (in pair with PtgArea3D)
                        {
                            found_operand = PtgRef3D.Create(operand_str, offset, ws_from, ws_to);
                        }
                        else if (PtgRefErr3D.Extract(formula, ref start_pos, out operand_str)) // Sequence is important (in pair with PtgArea3D)
                        {
                            found_operand = new PtgRefErr3D(operand_str, ws_from, ws_to);
                        }
				        else
				        {
                            m_oLastError = new Events.UnknownOperandError(formula, start_pos);
                            workbook.ReportError(m_oLastError);
                            ptgs_list.Clear();
                            return false;
                        }
                    }
                    else if (PtgName.Extract(formula, ref start_pos, out operand_str, workbook)) // Shall be placed strongly before PtgArea and PtgRef
                    {
                        found_operand = new PtgName(operand_str, workbook);
                    }
                    else if (PtgArea.Extract(formula, ref start_pos, out operand_str)) // Sequence is important (in pair with PtgRef)
                    {
                        found_operand = PtgArea.Create(operand_str, offset, ParentSheet);
                    }
                    else if (PtgRef.Extract(formula, ref start_pos, out operand_str)) // Sequence is important (in pair with PtgArea)
                    {
                        found_operand = PtgRef.Create(operand_str, offset, ParentSheet);
                    }
                    else if (PtgNum.Extract(formula, ref start_pos, out operand_str)) // Shall be placed after PtgArea but before UndefinedName
                    {
                        found_operand = new PtgNum(operand_str);
                    }
                    else if (PtgFunc.Extract(formula, ref start_pos, out operand_str))
                    {
                        PtgFunc func = workbook.Editor.FunctionsStorage.CreateInstance(operand_str.ToUpper());
                        if (func == null)
                        {
                            // TODO: process user-defined functions here
                            m_oLastError = new Events.UnknownFunctionError(operand_str, formula, start_pos - operand_str.Length);
                            operand_expected = false;
                            continue;
                        }
                        ptg_stack.Push(func);
                        operand_expected = false;
                        continue; // skip appending to the result queue
                    }
                    else
                    {
                        m_oLastError = new Events.UnknownOperandError(formula, start_pos);
                        workbook.ReportError(m_oLastError);
                        ptgs_list.Clear();
                        return false;
                    }

                    ptgs_list.AddFirst(found_operand);
                    operand_expected = false;
                }
                #endregion
            }
            if (operand_expected)
            {
                m_oLastError = new Events.OperandExpectedError(formula, formula.Length);
                workbook.ReportError(m_oLastError);
                ptgs_list.Clear();
                return false;
            }

            while (ptg_stack.Count != 0)
            {
                OperatorPtg operator_ptg = ptg_stack.Peek() as OperatorPtg;
                if (operator_ptg != null)
                {
                    ptgs_list.AddFirst(ptg_stack.Pop());
                }
                else
                {
                    m_oLastError = new Events.WrongParenthesisSequenceError(formula, formula.Length);
                    workbook.ReportError(m_oLastError);
                    ptgs_list.Clear();
                    return false;
                }
            }

            if (null != m_oLastError)
            {
                workbook.ReportError(m_oLastError);
                ptgs_list.Clear();
                m_oLastError = new Events.UnknownOperandError("", 0);
                return false;
            }
            return true;
        }

        #region Ungrouped regex verifiers
        private bool is_operators(string formula, int start_pos)
        {
            Match match = rx_operators.Match(formula, start_pos);
            return match.Success;
        }

        private bool extract_comma(string formula, ref int start_pos)
        {
            Match match = rx_num.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        internal static bool extract_3D_part(string formula, ref int start_pos, out Worksheet ws_from, out Worksheet ws_to, Workbook wb)
        {
            Match match;
            if ((match = rx_non_quoted_ref.Match(formula, start_pos)).Success || (match = rx_quoted_ref.Match(formula, start_pos)).Success)
            {
                GroupCollection groups = match.Groups;
                string name_from = groups["name_from"].Value.Replace("''", "'");
                string name_to = groups["name_to"].Value.Replace("''", "'");
                ws_from = wb.Sheets[name_from] as Worksheet;
                if (ws_from == null)
                {
                    ws_to = null;
                    return false;
                }
                if (name_to.Length == 0)
                {
                    ws_to = ws_from;
                }
                else
                {
                    ws_to = wb.Sheets[name_to] as Worksheet;
                    if (ws_to == null)
                    {
                        return false;
                    }
                }
                start_pos += match.Length;
                return true;
            }
            ws_from = ws_to = null;
            return false;
        }
        #endregion

        /// <summary>
        /// 
        /// </summary>
        /// <param name="removed_range"></param>
        /// <param name="removed_range_sheet"></param>
        /// <returns>Whether at least one reference changed</returns>
        public bool RemoveReferencesTo(Cells.CellRange removed_range, Worksheet removed_range_sheet)
        {
            bool at_least_one_changed = false;
            for (LinkedListNode<Ptg> current = ptgs_list.First; current != null; current = current.Next)
            {
                IRefOperandPtg ref_ptg = current.Value as IRefOperandPtg;
                if (ref_ptg != null)
                {
                    OperandPtg replacement_operand = ref_ptg.RemoveRange(removed_range, removed_range_sheet);
                    if (replacement_operand != null)
                    {
                        current.Value = replacement_operand;
                        at_least_one_changed = true;
                    }
                }
            }
            return at_least_one_changed;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="removed_sheet"></param>
        /// <returns>Returns is it possible to recalculate at the moment (TRUE) or the recalculation shall be put off (FALSE)</returns>
        public bool RemoveReferencesTo(Worksheet removed_sheet)
        {
            bool shall_be_delayed = false;
            for (LinkedListNode<Ptg> current = ptgs_list.First; current != null; current = current.Next)
            {
                OperandPtgRef3D _3d_ptg = current.Value as OperandPtgRef3D;
                if (_3d_ptg != null)
                {
                    OperandPtg replacement_operand = _3d_ptg.RemoveWorksheet(removed_sheet);
                    if (replacement_operand != null)
                    {
                        current.Value = replacement_operand;
                    }
                    else
                    {
                        shall_be_delayed = true;
                    }
                }
            }
            return !shall_be_delayed;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="moved_range"></param>
        /// <param name="offset"></param>
        /// <param name="affected_sheet"></param>
        /// <returns>Whether at least one reference changed</returns>
        public bool ResizeInsertionAffected(Cells.CellRange moved_range, Cells.Offset offset, Worksheet affected_sheet)
        {
            bool at_least_one_changed = false;
            for (LinkedListNode<Ptg> current = ptgs_list.First; current != null; current = current.Next)
            {
                IRefOperandPtg ref_ptg = current.Value as IRefOperandPtg;
                if (ref_ptg != null)
                {
                    OperandPtg replacement_operand = ref_ptg.ResizeInsertionAffected(moved_range, offset, affected_sheet);
                    if (replacement_operand != null)
                    {
                        current.Value = replacement_operand;
                        at_least_one_changed = true;
                    }
                }
            }
            return at_least_one_changed;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="subject"></param>
        /// <param name="offset"></param>
        /// <param name="affected_sheet"></param>
        /// <returns>Whether at least one reference removed</returns>
        public bool ShiftReferencesTo(ISubject subject, Cells.Offset offset, Worksheet affected_sheet)
        {
            bool at_least_one_shifted = false;
            for (LinkedListNode<Ptg> current = ptgs_list.First; current != null; current = current.Next)
            {
                IRefOperandPtg ref_ptg = current.Value as IRefOperandPtg;
                if (ref_ptg != null && ref_ptg.ReferencesCellOrRange(subject))
                {
                    OperandPtg replacement_operand = ref_ptg.Shift(offset, affected_sheet);
                    if (replacement_operand != null)
                    {
                        current.Value = replacement_operand;
                        at_least_one_shifted = true;
                    }
                }
            }
            return at_least_one_shifted;
        }

        /// <summary>
        /// Recalculates the formula value
        /// </summary>
        /// <returns>Result of the recalculation</returns>
        public virtual DataObjects.IDataObject Recalculate()
        {
            return Recalculate(PreferredResultType.value_type);
        }

        public virtual DataObjects.IDataObject Recalculate(PreferredResultType result_type)
        {
            RecalcInProgress = true;
            DataObjects.IDataObject result = null;
            if (ptgs_list.Count != 0)
            {
                LinkedList<Ptg>.Enumerator enumerator = ptgs_list.GetEnumerator();
                enumerator.MoveNext();
                try
                {
                    result = enumerator.Current.Calculate(ref enumerator, result_type);
                }
                catch (Events.ReportableException reportable_exception)
                {
                    workbook.ReportError(reportable_exception.error_description);
                }
            }
            RecalcInProgress = false;
            return result;
        }

        public virtual void UnregisterDependencies()
        {
            foreach (Ptg ptg in ptgs_list)
            {
                IHavingSubjectPtg ref_ptg = ptg as IHavingSubjectPtg;
                if (ref_ptg != null)
                {
                    ref_ptg.UnregisterDependency();
                }
            }
        }

        public void RegisterDependencies(IObserver observer)
        {
            foreach (Ptg ptg in ptgs_list)
            {
                IHavingSubjectPtg ref_ptg = ptg as IHavingSubjectPtg;
                if (ref_ptg != null)
                {
                    ref_ptg.RegisterDependency(observer);
                }
            }
        }

        public string Assemble()
        {
            if (ptgs_list.Count != 0)
            {
                LinkedList<Ptg>.Enumerator enumerator = ptgs_list.GetEnumerator();
                enumerator.MoveNext();
                try
                {
                    return formula_last_set = enumerator.Current.Assemble(ref enumerator);
                }
                catch (Events.ReportableException reportable_exception)
                {
                    workbook.ReportError(reportable_exception.error_description);
                }
            }
            return "";
        }

        public bool HasRelativeParts()
        {
            foreach (Ptg ptg in ptgs_list)
            {
                IRefOperandPtg ref_ptg = ptg as IRefOperandPtg;
                if(ref_ptg != null)
                {
                    if(ref_ptg.IsRelative)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        public override string ToString()
        {
            return formula_last_set ?? Assemble();
        }

        internal bool RecalcInProgress { get; private set; }

        internal Events.FormulaFormatError LastError { get { return m_oLastError; } }

        private Events.FormulaFormatError m_oLastError = null;

        private string formula_last_set;
        private Workbook workbook;
        private LinkedList<Ptg> ptgs_list;
        internal abstract Worksheet ParentSheet { get; }

        private static Regex rx_operators = new Regex("\\G *[-+*/^&%<=>:]");
        private static Regex rx_num = new Regex("\\G *[,;] *");

        private static Regex rx_non_quoted_ref = new Regex("\\G(?<name_from>\\w[\\w\\d.]*)(:(?<name_to>\\w[\\w\\d.]*))?!");
        private static Regex rx_quoted_ref = new Regex("\\G'(?<name_from>(''|[^]['\\/*?:])*)(:(?<name_to>(''|[^]['\\/*?:])*))?'!");
    }

    enum PreferredResultType
    {
        numeric_type,
        boolean_type,
        string_type,
        reference_type,
        value_type, // Means must be unreferenced or left the same if not a reference
        as_is // Means must be left the same
    }
}
