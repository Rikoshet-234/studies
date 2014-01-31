module HShell.Wc where

import HShell
import Data.List


data CountingMode = CountBytes | CountChars | CountLines | CountWords

data WcOpts = WcOpts { wcMode :: CountingMode }

instance ShellOpts WcOpts where
    defaultOpts = WcOpts { wcMode = CountWords }


wc :: WcOpts -> [FilePath] -> ShellScript ()
wc opts files =
    ShellScript ((), concat ["wc", mode, " ", input])
  where
    mode = case wcMode opts of
        CountBytes -> " --bytes"
        CountChars -> " --chars"
        CountLines -> " --lines"
        CountWords -> ""

    input = concat . intersperse " " . map show $ files

wc' :: [FilePath] -> ShellScript ()
wc' = wc defaultOpts
