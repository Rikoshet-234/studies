import Ast

import qualified Interpreter.Simple as IS
import qualified Interpreter.CallByValue as IV
import qualified Interpreter.CallByName as IN
import qualified Interpreter.CallByValueDefun as IVD
import qualified Interpreter.CallByNameDefun as IND

testPred :: Expr
testPred = App (Var "pred") (Const 1)

testEq :: Expr
testEq = If (App (App (Var "==") (Const 5)) (Const 5)) (Const 42) (Const 100)

testFact :: Int -> Expr
testFact n = LetRec "fact"
                     (Lambda "n" (If (App (App (Var "==") (Var "n")) (Const 0))
                                     (Const 1)
                                     (App (App (Var "*") (Var "n"))
                                          (App (Var "fact")
                                               (App (Var "pred") (Var "n"))))))
                     (App (Var "fact") (Const n))

testStrict :: Expr
testStrict =
    (App (App (Lambda "x" (Lambda "y" (Var "y"))) x) y)
  where
    x = testFact (-1)
    y = Const 42

main :: IO ()
main = do
    mapM_ putStrLn $
        [ "Simple predecessor test:"
        , show $ IS.interpret testPred
        , show $ IV.interpret testPred
        , show $ IN.interpret testPred
        , show $ IVD.interpret testPred
        , show $ IND.interpret testPred
        , ""
        , "Simple equality test:"
        , show $ IS.interpret testEq
        , show $ IV.interpret testEq
        , show $ IN.interpret testEq
        , show $ IVD.interpret testEq
        , show $ IND.interpret testEq
        , ""
        , "Factorial computation test:"
        , show $ IS.interpret (testFact 5)
        , show $ IV.interpret (testFact 5)
        , show $ IN.interpret (testFact 5)
        , show $ IVD.interpret (testFact 5)
        , show $ IND.interpret (testFact 5)
        , ""
        , "Non-strictness test:"
        , show $ IS.interpret testStrict
        --, show $ IV.interpret testStrict
        , show $ IN.interpret testStrict
        --, show $ IVD.interpret testStrict
        , show $ IND.interpret testStrict
        ]
