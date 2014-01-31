module HShell.Rm where

import HShell
import Data.List


data RmOpts = RmOpts { rmForce :: Bool
                     , rmVerbose :: Bool
                     , rmUpdate :: Bool
                     }

instance ShellOpts RmOpts where
    defaultOpts = RmOpts { rmForce = False
                         , rmVerbose = False
                         , rmUpdate = False
                         }


rm :: RmOpts -> [FilePath] -> ShellScript ()
rm opts files =
    ShellScript ((), concat ["rm", force, update, verbose, " ", input])
  where
    arg str True  = str
    arg _   False = ""

    force   = arg " --force" (rmForce opts)
    update  = arg " --update" (rmUpdate opts)
    verbose = arg " --verbose" (rmVerbose opts)

    input = concat . intersperse " " . map show $ files

rm' :: [FilePath] -> ShellScript ()
rm' = rm defaultOpts
