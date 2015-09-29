module Interpreter.CallByValueDefun (interpret) where

import Ast
import Prelude hiding (lookup)

data Value
    = I Int
    | L Ident Expr Env
    | Pred
    | Eq1
    | Eq2 Value
    | Mult1
    | Mult2 Value

instance Show Value where
    show (I value) = show value
    show (L ident _ _) = "(\\ " ++ ident ++ " . <body>)"
    show Pred = "pred"
    show Eq1 = "eq1"
    show (Eq2 value) = "eq2(" ++ show value ++ ")"
    show Mult1 = "eq1"
    show (Mult2 value) = "eq2(" ++ show value ++ ")"

data Cont
    = Id
    | AppF Expr Cont Env
    | AppA Value Cont
    | Branch Expr Expr Cont Env

type Env = [(Ident, Value)]

lookup :: Ident -> Env -> Value
lookup ident ((ident', value):env)
    | ident == ident' = value
    | otherwise = lookup ident env

apply :: Cont -> Value -> Value -> Value
apply k Pred (I a) = cont (I (a - 1)) k
apply k Eq1 a = cont (Eq2 a) k
apply k (Eq2 (I a)) (I b) = cont (I $ if (a == b) then 1 else 0) k
apply k Mult1 a = cont (Mult2 a) k
apply k (Mult2 (I a)) (I b) = cont (I $ a * b) k
apply k (L ident body env) a = eval k ((ident, a):env) body

cont :: Value -> Cont -> Value
cont v Id = v
cont f (AppF opnd k env) = eval (AppA f k) env opnd
cont a (AppA f k) = apply k f a
cont (I v) (Branch t f k env) = eval k env (if v /= 0 then t else f)  

eval :: Cont -> Env -> Expr -> Value
eval k env (Const val) = cont (I val) k
eval k env (Var ident) = cont (lookup ident env) k
eval k env (App opr opnd) = eval (AppF opnd k env) env opr
eval k env (Lambda ident body) = cont (L ident body env) k
eval k env (If cond btrue bfalse) = eval (Branch btrue bfalse k env) env cond
eval k env (LetRec ident (Lambda bind lbody) body) =
    eval k env' body
  where
    env' = (ident, L bind lbody env'):env

interpret :: Expr -> Value
interpret expr =
    eval Id init expr
  where
    init =
        [ ("==", Eq1)
        , ("*", Mult1)
        , ("pred", Pred)
        ]
