module Interpreter.Simple (interpret) where

import Ast

data Value
    = I Int
    | F (Value -> Value)

instance Show Value where
    show (I i) = show i
    show (F f) = "<fun>"

type Env = (Ident -> Value)

eval :: Env -> Expr -> Value
eval env (Const val) = I val
eval env (Var ident) = env ident
eval env (App opr opnd) =
    opr' opnd'
  where
    (F opr') = eval env opr
    opnd' = eval env opnd
eval env (Lambda ident body) = evalLambda env ident body
eval env (If cond btrue bfalse) =
    eval env (if cond' /= 0 then btrue else bfalse)
  where
    (I cond') = eval env cond
eval env (LetRec ident (Lambda bind lbody) body) =
    eval env' body
  where
    env' ident'
        | ident == ident' = evalLambda env' bind lbody
        | otherwise = env ident'

evalLambda :: Env -> Ident -> Expr -> Value
evalLambda env ident body =
    F $ \x -> eval (env' x) body
  where
    env' x ident'
        | ident == ident' = x
        | otherwise = env ident'

interpret :: Expr -> Value
interpret =
    eval init
  where
    init ident
        | ident == "==" = F $ \(I x) -> F $ \(I y) ->
            I (if x == y then 1 else 0)
        | ident == "+" = F $ \(I x) -> F $ \(I y) ->
            I (x + y)
        | ident == "*" = F $ \(I x) -> F $ \(I y) ->
            I (x * y)
        | ident == "pred" = F $ \(I x) -> I (x - 1)
        | otherwise = error $ "Unbound identifier " ++ show ident
