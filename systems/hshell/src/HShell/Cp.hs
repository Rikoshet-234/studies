module HShell.Cp where

import HShell
import Data.List


data CopyMode = NormalCopy | HardLink | SymbolicLink

data CpOpts = CpOpts { cpRecursive :: Bool
                     , cpVerbose :: Bool
                     , cpUpdate :: Bool
                     , cpMode :: CopyMode
                     }

instance ShellOpts CpOpts where
    defaultOpts = CpOpts { cpRecursive = False
                         , cpVerbose = False
                         , cpUpdate = False
                         , cpMode = NormalCopy
                         }


cp :: CpOpts -> [FilePath] -> FilePath -> ShellScript ()
cp opts sources dest =
    ShellScript ((), concat ["cp"
                            , recursive, verbose, update, mode
                            , " ", input
                            , " ", dest
                            ])
  where
    arg str True  = str
    arg _   False = ""

    recursive = arg " --recursive" (cpRecursive opts)
    verbose   = arg " --verbose" (cpVerbose opts)
    update    = arg " --update" (cpUpdate opts)
    mode      = case cpMode opts of
        NormalCopy   -> ""
        HardLink     -> " --link"
        SymbolicLink -> " --symbolic-link"

    input = concat . intersperse " " . map show $ sources

cp' :: [FilePath] -> FilePath -> ShellScript ()
cp' = cp defaultOpts
