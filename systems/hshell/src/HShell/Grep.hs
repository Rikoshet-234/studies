module HShell.Grep where

import HShell
import Data.List


data RegexpMode = NormalRegexp | WordRegexp | LineRegexp

data GrepOpts = GrepOpts { grepIgnoreCase :: Bool
                         , grepInvertMatch :: Bool
                         , grepQuiet :: Bool
                         , grepRegexpMode :: RegexpMode
                         }

instance ShellOpts GrepOpts where
    defaultOpts = GrepOpts { grepIgnoreCase = False
                           , grepInvertMatch = False
                           , grepQuiet = False
                           , grepRegexpMode = NormalRegexp
                           }


grep :: GrepOpts -> String -> [FilePath] -> ShellScript ()
grep opts pattern files =
    ShellScript ((), concat ["grep"
                            , ignore, invert, quiet, mode
                            , " ", show pattern
                            , " ", input
                            ])
  where
    arg str True  = str
    arg _   False = ""

    ignore = arg " --ignore-case" (grepIgnoreCase opts)
    invert = arg " --invert-match" (grepInvertMatch opts)
    quiet  = arg " --quiet" (grepQuiet opts)
    mode   = case grepRegexpMode opts of
        NormalRegexp -> ""
        WordRegexp   -> " --word-regexp"
        LineRegexp   -> " --line-regexp"

    input = concat . intersperse " " . map show $ files

grep' :: String -> [FilePath] -> ShellScript ()
grep' = grep defaultOpts
