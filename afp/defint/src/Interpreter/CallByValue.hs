module Interpreter.CallByValue (interpret) where

import Ast

data Value
    = I Int
    | L Ident Expr Env
    | N (Value -> Value)

instance Show Value where
    show (I value) = show value
    show (L ident _ _) = "(\\ " ++ ident ++ " . <body>)"
    show (N f) = "<native>"

type Env = (Ident -> Value)

extend :: Ident -> Value -> Env -> Env
extend ident value env ident'
    | ident == ident' = value
    | otherwise = env ident'

eval :: (Value -> a) -> Env -> Expr -> a
eval cont env (Const val) = cont $ I val
eval cont env (Var ident) = cont $ env ident
eval cont env (App opr opnd) =
    eval cont' env opr
  where
    cont' f = eval (apply f) env opnd
    apply (N f) a = cont $ f a
    apply (L ident body env) a = eval cont (extend ident a env) body
eval cont env (Lambda ident body) = cont $ L ident body env
eval cont env (If cond btrue bfalse) =
    eval cont' env cond
  where
    cont' (I value) = eval cont env (if value /= 0 then btrue else bfalse)
eval cont env (LetRec ident (Lambda bind lbody) body) =
    eval cont env' body
  where
    env' = extend ident (L bind lbody env') env

interpret :: Expr -> Value
interpret =
    eval id init
  where
    init ident
        | ident == "==" = N $ \(I x) -> N $ \(I y) ->
            I (if x == y then 1 else 0)
        | ident == "+" = N $ \(I x) -> N $ \(I y) ->
            I (x + y)
        | ident == "*" = N $ \(I x) -> N $ \(I y) ->
            I (x * y)
        | ident == "pred" = N $ \(I x) -> I (x - 1)
        | otherwise = error $ "Unbound identifier " ++ show ident
