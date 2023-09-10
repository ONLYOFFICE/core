#ifndef TYPESELEMENTS_H
#define TYPESELEMENTS_H
namespace StarMath
{
enum TypeElement{
	Number,
	BinOperator,
	Operator,
	Bracket,
	UnarSign,
	Attribute,
	SpecialCharacter,
};
enum TypeBinOperator
{
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
};
enum TypeOperator
{
	lim,
	sum,
};
enum TypeBracket
{
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
};
enum TypeAttributeTop
{
	noneTop,
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
	overstrike,
	wideharpoon,
	widehat,
	underline,//top elements
};
enum TypeAttributeColor
{
	noneColor,
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
	violet,//color(without rgb and hex)
};
enum TypeCharacter
{
	mline,
	grid,
};
}
#endif // TYPESELEMENTS_H
