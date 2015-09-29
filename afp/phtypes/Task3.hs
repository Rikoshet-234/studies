{-# LANGUAGE GADTs, RankNTypes #-}

import Control.Applicative (pure, (<$>), (<*>))

data Dyn = forall a. Dyn (Typ a) a

data Typ a where
    TInt :: Typ Int
    TBool :: Typ Bool
    TString :: Typ String
    TList :: Typ a -> Typ [a]
    TPair :: Typ a -> Typ b -> Typ (a, b)
    TTerm :: Typ a -> Typ (Term a)
    TDynamic :: Typ Dyn

lifter :: forall a b. Typ a -> Typ b -> Maybe (a -> b)
lifter TInt TInt = return id
lifter TBool TBool = return id
lifter TString TString = return id
lifter (TTerm TInt) (TTerm TInt) = return id
lifter (TTerm TBool) (TTerm TBool) = return id
lifter (TList a') (TList b') = map <$> lifter a' b'
lifter (TPair a' b') (TPair c' d') = do
    f <- lifter a' c'
    g <- lifter b' d'
    return $ \(x, y) -> (f x, g y)
lifter _ _ = Nothing

cast :: forall a. Typ a -> Dyn -> Maybe a
cast t (Dyn t' x) = lifter t' t <*> pure x

data Term a where
    Zero :: Term Int
    Succ :: Term Int -> Term Int
    Pred :: Term Int -> Term Int
    IsZero :: Term Int -> Term Bool
    If :: Term Bool -> Term a -> Term a -> Term a

eval :: forall a. Term a -> a
eval Zero = 0
eval (Succ exp) = eval exp + 1
eval (Pred exp) = eval exp - 1
eval (IsZero exp) = eval exp == 0
eval (If c t f)
    | eval c = eval t
    | otherwise = eval f

data RawTerm
    = RZero
    | RSucc RawTerm
    | RPred RawTerm
    | RIsZero RawTerm
    | RIf RawTerm RawTerm RawTerm

typecheck :: RawTerm -> Maybe Dyn
typecheck RZero = return $ Dyn (TTerm TInt) Zero
typecheck (RSucc exp) = do
    (Dyn (TTerm TInt) exp') <- typecheck exp
    return $ Dyn (TTerm TInt) (Succ exp') 
typecheck (RPred exp) = do
    (Dyn (TTerm TInt) exp') <- typecheck exp
    return $ Dyn (TTerm TInt) (Pred exp') 
typecheck (RIsZero exp) = do
    (Dyn (TTerm TInt) exp') <- typecheck exp
    return $ Dyn (TTerm TBool) (IsZero exp')
typecheck (RIf c t f) = do
    (Dyn (TTerm TBool) c') <- typecheck c
    (Dyn tt@(TTerm _) t') <- typecheck t
    f' <- cast tt =<< typecheck f
    return $ Dyn tt (If c' t' f')

evalDyn :: Dyn -> Maybe Dyn
evalDyn (Dyn (TTerm t) exp) = Just $ Dyn t (eval exp)
evalDyn _ = Nothing


-- Tests

testRaw :: RawTerm
testRaw = RIf (RIsZero RZero) (RSucc RZero) RZero

test :: Maybe Int
test = cast TInt =<< evalDyn =<< typecheck testRaw
