module Ast where

type Ident = String

data Expr
    = Const Int
    | Var Ident
    | App Expr Expr
    | Lambda Ident Expr
    | If Expr Expr Expr
    | LetRec Ident Expr Expr
