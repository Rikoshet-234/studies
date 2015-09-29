{-# LANGUAGE GADTs, RankNTypes #-}

data Dir a b where
    DLit :: String -> Dir a a
    DStr :: Dir (String -> a) a
    DInt :: Dir (Int -> a) a
    DNode :: Dir a b -> Dir b c -> Dir a c

format :: Dir a String -> a
format =
    aux id ""
  where
    aux :: forall a b. (String -> b) -> String -> Dir a b -> a 
    aux k r (DLit s) = k $ r ++ s
    aux k r DStr = \s -> k $ r ++ s
    aux k r DInt = \n -> k $ r ++ show n
    aux k r (DNode a b) = aux (\r' -> aux k r' b) r a

format' :: Dir a String -> a
format' =
    aux (concat . reverse) []
  where
    aux :: forall a b. ([String] -> b) -> [String] -> Dir a b -> a
    aux k r (DLit s) = k $ s:r
    aux k r DStr = \s -> k $ s:r
    aux k r DInt = \n -> k $ show n:r
    aux k r (DNode a b) = aux (\r' -> aux k r' b) r a

type Concat = (String -> String)

format'' :: Dir a String -> a
format'' =
    aux ($ "") id
  where
    aux :: forall a b. (Concat -> b) -> Concat -> Dir a b -> a
    aux k r (DLit s) = k $ r . (s ++)
    aux k r DStr = \s -> k $ r . (s ++)
    aux k r DInt = \n -> k $ r . (show n ++)
    aux k r (DNode a b) = aux (\r' -> aux k r' b) r a

-- Tests

testDir = (DNode (DNode (DLit "a") (DStr)) (DLit "b"))

test = format testDir "hello"
test' = format' testDir "hello"
test'' = format'' testDir "hello"
