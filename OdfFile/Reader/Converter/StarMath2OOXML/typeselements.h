/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#ifndef TYPESELEMENTS_H
#define TYPESELEMENTS_H

namespace StarMath
{
enum class TypeElement{
	undefine,
	//global
	String,
	BinOperator,
	SetOperations,
	Operator,
	Bracket,
	BracketWithIndex,
	Grade,
	Mark,
	//BracketEnd,
	UnarSign,
	Attribute,
	SpecialSymbol,
	Function,
	Operation,
	Index,
	Matrix,
	Connection,
	Empty,
	//binoop
	cdot,
	times,
	over,
	plus,
	minus,
	frac,
	div,
	multipl,
	division,
	oplus,
	ominus,
	odot,
	otimes,
	odivide,
	circ,
	wideslash,
	widebslash,
	//logic
	And,
	Or,
	neg,
	//unary
	plus_minus,
	minus_plus,
	//op
	lim,
	sum,
	liminf,
	limsup,
	prod,
	coprod,
	Int,
	iint,
	iiint,
	lint,
	llint,
	lllint,
	//brace
	brace,
	round,
	square,
	ldbracket,
	lbrace,
	langle,
	lceil,
	lfloor,
	lline,
	ldline,
	//attribute
	ital,
	bold,
	phantom,
	overstrike,
	size,
	font,
	//top element
	acute,
	breve,
	dot,
	dddot,
	vec,
	tilde,
	check,
	grave,
	circle,
	ddot,
	bar,
	harpoon,
	hat,
	widevec,
	widetilde,
	overline,
	wideharpoon,
	widehat,
	underline,//top elements
	color,
	hex,
	rgb,
	black,
	green,
	aqua,
	yellow,
	lime,
	navy,
	purple,
	teal,
	blue,
	red,
	fuchsia,
	gray,
	maroon,
	olive,
	silver,
	coral,
	midnightblue,
	crimson,
	violet,
	orange,
	seagreen,
	hotpink,
	orangered,
	indigo,
	lavender,
	//color(without rgb and hex)
	mline,
	dlgrid,
	//setopetions
	intersection,
	Union,
	setminus,
	setquotient,
	subseteq,
	subset,
	supset,
	supseteq,
	nsubset,
	nsupseteq,
	nsupset,
	nsubseteq,
	in,
	notin,
	owns,
	//connection
	approx,
	sim,
	simeq,
	equiv,
	prop,
	parallel,
	ortho,
	divides,
	ndivides,
	toward,
	transl,
	transr,
	def,
	equals,
	notequals,
	learrow,
	learrowequals,
	leslant,
	riarrow,
	riarrowequals,
	geslant,
	dllearrow,
	dlriarrow,
	prec,
	succ,
	preccurlyeq,
	succcurlyeq,
	precsim,
	succsim,
	nprec,
	nsucc,
	dlarrow,
	dlrarrow,
	drarrow,
	//SpecialSymbol
	emptyset,
	aleph,
	setN,
	setZ,
	setQ,
	setR,
	setC,
	grid,
	transition,
	emptiness,
	interval,
	infinity,
	partial,
	nabla,
	exists,
	notexists,
	forall,
	hbar,
	lambdabar,
	Re,
	Im,
	wp,
	laplace,
	fourier,
	backepsilon,
	//function
	abs,
	fact,
	sqrt,
	nroot,//nroot?
	sin,
	cos,
	tan,
	cot,
	sinh,
	cosh,
	tanh,
	coth,
	arcsin,
	arccos,
	arctan,
	arccot,
	arsinh,
	arcosh,
	artanh,
	arcoth,
	ln,
	exp,
	log,
	//index
	upper,
	lower,
	lsup,
	lsub,
	csup,
	csub,
	//
	binom,
	stack,
	matrix,
	//bracket close
	rwbrace,
	rbrace,
	rround,
	rsquare,
	rdbracket,
	rangle,
	rceil,
	rfloor,
	rline,
	rdline,
	right,
	//op
	from,
	to,
	//bracketWithIndex
	overbrace,
	underbrace,
	//grade
	evaluate,
};
}
#endif // TYPESELEMENTS_H
