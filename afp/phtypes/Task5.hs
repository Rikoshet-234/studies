{-# LANGUAGE GADTs, RankNTypes #-}

data Dyn = forall a. Dyn (Typ a) a

data Typ a where
    TInt     :: Typ Int
    TBool    :: Typ Bool
    TString  :: Typ String
    TList    :: Typ a -> Typ [a]
    TPair    :: Typ a -> Typ b -> Typ (a, b)
    TDynamic :: Typ Dyn

type Trav = forall a. Typ a -> a -> a
type Query b = forall a. Typ a -> a -> b

(>>>) :: Trav -> Trav -> Trav
(>>>) f g t = f t . g t

imap :: Trav -> Trav
imap f (TList t) = map (f t)
imap f (TPair ta tb) = \(x, y) -> (f ta x, f tb y)
imap f _ = id

icrush :: (a -> a -> a) -> a -> Query a -> Query a
icrush p e f (TList t) = foldr (\x acc -> p acc (f t x)) e
icrush p e f (TPair ta tb) = \(x, y) -> p (f ta x) (f tb y)
icrush p e f _ = const e

allover :: Trav -> Trav
allover f = f >>> imap (allover f)

everything :: (a -> a -> a) -> a -> Query a -> Query a
everything p e f t x = p (f t x) (icrush p e f t x)

isum :: Query Int -> Query Int
isum = icrush (+) 0

total :: Query Int -> Query Int
total = everything (+) 0

bumpAll :: Trav
bumpAll =
    allover bump
  where
    bump :: Trav
    bump TInt = (+ 1)
    bump _ = id

sizeOfAll :: Query Int
sizeOfAll =
    total sizeOf
  where
    sizeOf :: Query Int
    sizeOf TString = length
    sizeOf (TList _) = length
    sizeOf _ = const 1

-- Tests

test :: Int
test = sizeOfAll (TList TInt) [3, 5, 6]

test' :: Int
test' = sizeOfAll (TList TString) ["abc", "def"]
