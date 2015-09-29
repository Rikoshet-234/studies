{-# LANGUAGE GADTs, RankNTypes #-}

import Control.Applicative (pure, (<$>), (<*>))
import Data.List (intercalate)

data Dyn = forall a. Dyn (Typ a) a

data Typ a where
    TInt :: Typ Int
    TBool :: Typ Bool
    TString :: Typ String
    TList :: Typ a -> Typ [a]
    TPair :: Typ a -> Typ b -> Typ (a, b)
    TDynamic :: Typ Dyn

str :: forall a. Typ a -> a -> String
str TInt x = "(" ++ show x ++ " :: Int)"
str TBool x = "(" ++ show x ++ " :: Bool)"
str TString x = "('" ++ x ++ "' :: String)"
str (TList t) xs
    = "[" ++ xs' ++ "]"
  where
    xs' = intercalate " " . map (str t) $ xs
str (TPair ta tb) (a, b) = "(" ++ str ta a ++ ", " ++ str tb b ++ ")"
str TDynamic (Dyn t x) = str t x
