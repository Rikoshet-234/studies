module HShell.Mv where

import HShell
import Data.List


data MvOpts = MvOpts { mvForce :: Bool
                     , mvVerbose :: Bool
                     , mvUpdate :: Bool
                     }

instance ShellOpts MvOpts where
    defaultOpts = MvOpts { mvForce = False
                         , mvVerbose = False
                         , mvUpdate = False
                         }


mv :: MvOpts -> [FilePath] -> FilePath -> ShellScript ()
mv opts sources dest =
    ShellScript ((), concat ["mv"
                            , force, verbose, update
                            , " ", input
                            , " ", dest
                            ])
  where
    arg str True  = str
    arg _   False = ""

    force   = arg " --force" (mvForce opts)
    verbose = arg " --verbose" (mvVerbose opts)
    update  = arg " --update" (mvUpdate opts)

    input = concat . intersperse " " . map show $ sources

mv' :: [FilePath] -> FilePath -> ShellScript ()
mv' = mv defaultOpts
