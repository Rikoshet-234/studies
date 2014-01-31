module HShell.Language where

import HShell


raw :: String -> ShellScript ()
raw source = ShellScript ((), source)


(@=) :: String -> String -> ShellScript String
var @= value = ShellScript ("$" ++ value, var ++ "=" ++ value)
