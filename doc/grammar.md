#X <- Terminal symbol
X can be
    UpperCase
    LowerCase
    AlphaNum
    Num
    _
    letters, digits, other symbols

Complete        := Program #EOF
Program         := Define
                := Program Define
Define          := #new DefineLines
DefineLines     := DefineLine
                := DefineLines #, DefineLine
DefineLine      := Head #-#> Body
Head            := Identifier#<FuncMatch#> FuncMatch
Body            := Expression
FuncMatch       := Variable
                := Identifier#<#>
                := Identifier#<FuncMatch#>
                := #[FuncMatch #, FuncMatch#]
                := Constant
                := #_
Variable        := #UpperCase#{AlphaNum|_}*
Identifier      := #LowerCase#{AlphaNum|_}*
Constant        := #0 | #{1...9}#{Num}*
Expression      := Variable
                := Identifier
                := Constant
                := #(Expression#)
                := Expression Expression
                := Identifier#<Expression#>
                := #[Expression #, Expression#]
                := FuncMatch #= Expression
