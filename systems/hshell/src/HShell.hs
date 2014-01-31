module HShell ( ShellScript(..), source
              , ShellOpts, defaultOpts, with
              , ShellPipeline, (~>), pipe
              ) where


newtype ShellScript a = ShellScript (a, String)

source :: ShellScript a -> String
source (ShellScript (_, src)) = src


instance Monad ShellScript where
    return x = ShellScript (x, "")
    ShellScript (x, line1) >>= f =
        ShellScript (y, line1 ++ "\n" ++ line2)
      where
        ShellScript (y, line2) = f x


class ShellOpts a where
    defaultOpts :: a

with :: ShellOpts a => a
with = defaultOpts


type ShellPipeline = [ShellScript ()]

infixr 7 ~>
(~>) :: ShellScript () -> ShellPipeline -> ShellPipeline
(~>) = (:)

pipe :: [ShellScript ()] -> ShellScript ()
pipe =
    foldr1 merge
  where
    merge (ShellScript (_, source1)) (ShellScript (_, source2)) =
        ShellScript ((), source1 ++ " | " ++ source2)
