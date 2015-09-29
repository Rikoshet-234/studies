Monads in functional programming
================================

First a few imports to make the whole file compile (they will be used in the exercises later):

\begin{code}
import Control.Monad.Identity
import Control.Monad.List
import Control.Monad.State
import Data.Char
\end{code}


Exercise 1
----------

Since both `foldl` and `reverse` are tail recursive, so is their composition.

\begin{code}
map' :: (a -> b) -> [a] -> [b]
map' f = reverse . foldl (\xs x -> f x : xs) []
\end{code}


Exercise 2
----------

Okay, here's the definiton from the exercise:

\begin{code}
cfold :: (a -> b -> (b -> b) -> b) -> b -> [a] -> b
cfold f e []     = e
cfold f e (x:xs) = f x e (\y -> cfold f y xs) 
\end{code}

Now, both `foldr` and `foldl` are quite easy to define in terms of `cfold`:

\begin{code}
foldl' :: (b -> a -> b) -> b -> [a] -> b
foldl' f = cfold (\x e c -> c (f e x))

foldr' :: (a -> b -> b) -> b -> [a] -> b
foldr' f = cfold (\x e c -> f x (c e))
\end{code}


Exercise 3
----------

Let's try implementing one and see what will happen.

\begin{code}
newtype MaybeT m a = MaybeT (Maybe (m a))
\end{code}

So, we want this weird `MaybeT m` to be a monad. Thus, we need to implement two things: `return :: a -> MaybeT a` and `(>>=) :: MaybeT m a -> (a -> MaybeT m b) -> MaybeT m b`. While the former one pose no real trouble, the latter one does.

To implement `(>>=)` we would need a way to extract a value of type `a` from its context `MaybeT m a` in order to feed it to the right argument of `(>>=)`. `runMaybeT :: MaybeT a -> Maybe (m a)` would allow us to remove `MaybeT` constructor. First layer off, two more to go. Now, we are left with `Maybe (m a)`. Fortunately for us, we can pattern match against two values. In the `Nothing` case, there is no `m a` so we can just leave with `MaybeT Nothing`. So far so good. We only need to cover `Just` case where we are left with `m a`. So how we can peek on what is inside? Well, the only tool we have is `(>>=)` of `m`. So, we can extract some `x` of type `a` from `m` context but we are required to return something of type `m b` - and `f` applied to `x` returns `MaybeT m b`. We *could* pattern match of the result of `f x` to extract something of type `m b` but that would lead to a dead end in the case of `Nothing`. So nope, it won't work that way unless we have something like `unsafePerformM :: m a -> a`.

Hence, the only `(>>=)` that typechecks is `_ >>= _ = MaybeT Nothing`. But it does not obey some laws for `return` defined as `MaybeT . Just . return`. Thus, the only lawful `return` is `MaybeT . const Nothing`. So, the final implementation is as follows: 

\begin{code}
instance Monad m => Monad (MaybeT m) where
    return  = MaybeT . const Nothing
    _ >>= _ = MaybeT Nothing
\end{code}

Seems useful.


Exercise 4
----------

To be honest, I don't get this exercise. At all. What does `lift` and `liftM` have in common (except for similar name)? `liftM`, also known as `fmap` can be seen in two ways, depending whether we put unnecessary pair of parenthesis or not:

~~~ haskell
fmap :: Functor f => (a -> b) -> f a -> f b
fmap :: Functor f => (a -> b) -> (f a -> f b)
~~~

So we can regard it as a function that takes a function `(a -> b)` and a value in a context `f a`, unwraps value from the context, applies given function and puts result back into the context (hence the *map* name). Or, an equivalent view on this: it is a function that takes a function `(a -> b)` and returns a function that operates on values in the context `(f a -> f b)` (hence the *lift* name).

And `lift` is something that takes value in one context and puts it inside some other context (actually, it *boxes* an already *boxed* value). So, if I were to draw some analogy here, I would rather choose `return` (also known as `pure`) as it is somewhat similar: it takes some  value and puts it in some default context. And the question why `return` can't be defined in universal way is somewhat pointless, isn't it? The same goes for `lift`.


Exercise 5.2
------------

This exercise clearly wants us to use `State` monad avaliable in standard Haskell library. As a state we will keep two values: current character and that-other-number (uhm, number of times that we run out of avaliable characters in the alphabet?). So, there it is:

\begin{code}
type NameSupply = State (Char, Int)
\end{code}

Having this, symbol generation is just a matter of peeking at the current state, converting it to nice looking string and increasing the state.

\begin{code}
genSymbol :: NameSupply String
genSymbol = do
    (c, n) <- get
    put $ if c == 'z'
          then ('a', n + 1)
          else (chr (ord c + 1), n)
    return $ [c] ++ (if n > 0 then show n else "")
\end{code}

... and something to evaluate the whole monad:

\begin{code}
runNameSupply :: NameSupply a -> a
runNameSupply = flip evalState ('a', 0)
\end{code}

We can verify that it works by looking at the results of some examples, such as `runNameSupply $ sequence $ replicate 100 genSymbol`.


Exercise 6
----------

Well, the semantics are obvious: we had some suspended computation that finally was about to return some value of type `r`, now we want to wrap this in additional monad. So the type is as follows:

\begin{code}
newtype ContT r m a = ContT { runContT :: ((a -> m r) -> m r)}
\end{code}

We want `ContT r m` to be a monad instance. Guided by the types, we need to define `return :: Monad m => a -> ContT r m a` and `(>>=) :: Monad m => ContT r m a -> (a -> ContT r m b) -> ContT r m b`.

First one is simple:

~~~ haskell
return x = ContT (\k -> k x)
~~~

Now, the fun part. Recall the definiton of `(>>=)` for `Cont r` monad:

~~~ haskell
Cont c >>= f = Cont $ \k -> c $ \x -> runCont (f x) k
~~~

So, `k` was of type `a -> r`, now it will be of type `a -> m r`. `f` was of type `a -> ((b -> r) -> r)` (if not for the `ContT` wrapper but we can ignore that as it does no harm), now it is of type `a -> ((b -> m r) -> m r)`. So it matches and looks like the definition is virtually the same:

~~~ haskell
ContT c >>= f = ContT $ \k -> c $ \x -> runContT (f x) k
~~~

Yay. Okay, putting it all together we get:

\begin{code}
instance Monad m => Monad (ContT r m) where
    return x = ContT (\k -> k x)
    ContT c >>= f = ContT $ \k -> c $ \x -> runContT (f x) k
\end{code}

We are also asked to explain the semantics of following types:

\begin{code}
type ListCont r = ContT r []
type ContList r = ListT (ContT r Identity)
\end{code}

Since we like to think of a list monad as a nondeterminism, I will try to stick to that. The `Cont r a` was equivalent to some suspended computation of type `(a -> r) -> r`. Now, the ordering of stacking monad transformations affects where we put nondeterminism. For `ListCont` we get `(a -> [r]) -> [r]` and for `ContList` we get `([a] -> r) -> r`. And that is pretty much it I think.
