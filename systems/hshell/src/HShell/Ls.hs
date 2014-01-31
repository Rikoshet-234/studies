module HShell.Ls where

import HShell
import Data.List


data LsOpts = LsOpts { lsAll :: Bool
                     , lsLong :: Bool
                     , lsIgnore :: Maybe String
                     , lsReverse :: Bool
                     , lsRecursive :: Bool
                     }

instance ShellOpts LsOpts where
    defaultOpts = LsOpts { lsAll = False
                         , lsLong = False
                         , lsIgnore = Nothing
                         , lsReverse = False
                         , lsRecursive = False
                         }


ls :: LsOpts -> [FilePath] -> ShellScript ()
ls opts args =
    ShellScript ((), concat ["ls"
                            , all, long, ignore, reverse, recursive
                            , " ", input
                            ])
  where
    arg str True  = str
    arg _   False = ""

    all       = arg " --all" (lsAll opts)
    long      = arg " --long" (lsLong opts)
    reverse   = arg " --reverse" (lsReverse opts)
    recursive = arg " --recursive" (lsRecursive opts)
    ignore    = case lsIgnore opts of
        Just pattern -> " --ignore=" ++ (show pattern)
        Nothing      -> ""

    input = concat . intersperse " " . map show $ args

ls' :: [FilePath] -> ShellScript ()
ls' = ls defaultOpts
