module Interpreter.CallByName (interpret) where

import Ast

data Value
    = I Int
    | L Ident Expr Env
    | N (Value -> Value)

instance Show Value where
    show (I value) = show value
    show (L ident _ _) = "(\\ " ++ ident ++ " . <body>)"
    show (N f) = "<native>"

type Comp = (Value -> Value) -> Value

type Env = (Ident -> Comp)

extend :: Ident -> Comp -> Env -> Env
extend ident value env ident'
    | ident == ident' = value
    | otherwise = env ident'

eval :: Env -> Expr -> Comp
eval env (Const val) = ($ I val)
eval env (Var ident) = env ident
eval env (App opr opnd) = \cont ->
    eval env opr (cont' cont)
  where
    cont' cont (L ident body lenv) =
        eval env' body cont
      where
        env' = extend ident (eval env opnd) lenv
    cont' cont (N f) = eval env opnd $ \a -> cont (f a)
eval env (Lambda ident body) = ($ L ident body env)
eval env (If cond btrue bfalse) = \cont ->
    eval env cond (cont' cont)
  where
    cont' cont (I value) = eval env (if value /= 0 then btrue else bfalse) cont
eval env (LetRec ident (Lambda bind lbody) body) =
    eval env' body
  where
    env' = extend ident ($ L bind lbody env') env

interpret :: Expr -> Value
interpret expr =
    eval init expr id
  where
    init ident cont
        | ident == "==" = cont $ N $ \(I x) -> N $ \(I y) ->
            I (if x == y then 1 else 0)
        | ident == "+" = cont $ N $ \(I x) -> N $ \(I y) ->
            I (x + y)
        | ident == "*" = cont $ N $ \(I x) -> N $ \(I y) ->
            I (x * y)
        | ident == "pred" = cont $ N $ \(I x) ->
            I (x - 1)
        | otherwise = error $ "Unbound identifier " ++ show ident
