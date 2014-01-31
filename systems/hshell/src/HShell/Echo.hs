module HShell.Echo where

import HShell
import Data.List


data EchoOpts = EchoArgs { echoNewline :: Bool }

instance ShellOpts EchoOpts where
    defaultOpts = EchoArgs { echoNewline = True }


echo :: EchoOpts -> [String] -> ShellScript ()
echo opts args | echoNewline opts = ShellScript ((), "echo " ++ input)
               | otherwise        = ShellScript ((), "echo -n " ++ input)
  where
    input = concat . intersperse " " . map show $ args

echo' :: [String] -> ShellScript ()
echo' = echo defaultOpts
