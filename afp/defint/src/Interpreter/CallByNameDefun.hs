module Interpreter.CallByNameDefun (interpret) where

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
    | CApp Expr Cont Env
    | CIf Expr Expr Cont Env
    | CPred Cont
    | CEq1 Cont
    | CEq2 Cont Value
    | CMult1 Cont
    | CMult2 Cont Value

data Susp
    = E Env Expr
    | V Value

type Env = [(Ident, Susp)]

lookup :: Ident -> Env -> Susp
lookup ident ((ident', value):env)
    | ident == ident' = value
    | otherwise = lookup ident env

apply :: Cont -> Env -> Value -> Expr -> Value
apply k env Pred expr = eval (CPred k) env expr
apply k env Eq1 expr = eval (CEq1 k) env expr
apply k env (Eq2 v) expr = eval (CEq2 k v) env expr
apply k env Mult1 expr = eval (CMult1 k) env expr
apply k env (Mult2 v) expr = eval (CMult2 k v) env expr
apply k env (L id body env') expr = eval k ((id, E env expr):env') body

cont :: Value -> Cont -> Value
cont v Id = v
cont f (CApp opnd k env) = apply k env f opnd
cont (I v) (CIf t f k env) = eval k env (if v /= 0 then t else f)
cont (I v) (CPred k) = cont (I $ v - 1) k
cont v (CEq1 k) = cont (Eq2 v) k
cont (I a) (CEq2 k (I b)) = cont (I $ if (a == b) then 1 else 0) k
cont v (CMult1 k) = cont (Mult2 v) k
cont (I a) (CMult2 k (I b)) = cont (I $ a * b) k

eval :: Cont -> Env -> Expr -> Value
eval k env (Const val) = cont (I val) k
eval k env (Var ident) =
    case (lookup ident env) of
        E env' expr -> eval k env' expr
        V value -> cont value k
eval k env (App opr opnd) = eval (CApp opnd k env) env opr
eval k env (Lambda ident body) = cont (L ident body env) k
eval k env (If cond btrue bfalse) = eval (CIf btrue bfalse k env) env cond
eval k env (LetRec ident (Lambda lident lbody) body) =
    eval k env' body
  where
    env' = (ident, V $ L lident lbody env'):env

interpret :: Expr -> Value
interpret expr =
    eval Id init expr
  where
    init =
        [ ("==", V Eq1)
        , ("*", V Mult1)
        , ("pred", V Pred)
        ]
