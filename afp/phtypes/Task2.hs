{-# LANGUAGE GADTs, RankNTypes #-}

data Dyn = forall a. Dyn (Typ a) a

data Typ a where
    TInt :: Typ Int
    TBool :: Typ Bool
    TString :: Typ String
    TList :: Typ a -> Typ [a]
    TPair :: Typ a -> Typ b -> Typ (a, b)
    TDynamic :: Typ Dyn

data TypEq a b where
    TypEq :: Typ a -> Typ a -> TypEq a a

equal :: Typ a -> Typ b -> Maybe (TypEq a b)
equal TInt TInt = return $ TypEq TInt TInt
equal TBool TBool = return $ TypEq TBool TBool
equal TString TString = return $ TypEq TString TString
equal a@(TList a') b@(TList b') = do
    TypEq _ _ <- equal a' b'
    return $ TypEq a b
equal a@(TPair a1 a2) b@(TPair b1 b2) = do
    TypEq _ _ <- equal a1 b1
    TypEq _ _ <- equal a2 b2
    return $ TypEq a b
equal _ _ = Nothing

cast :: Typ a -> Dyn -> Maybe a
cast t' (Dyn t x) = do
    TypEq _ _ <- equal t t'
    return x
